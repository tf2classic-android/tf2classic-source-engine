#include "cbase.h"
#include "tf_mainmenupanel.h"
#include "controls/tf_advbutton.h"
#include "controls/tf_advslider.h"
#include "vgui_controls/SectionedListPanel.h"
#include "vgui_controls/ImagePanel.h"
#include "tf_notificationmanager.h"
#include "engine/IEngineSound.h"
#include "vgui_avatarimage.h"
#include "git_info.h"
#include <vgui/ISurface.h>
#include <vgui_controls/AnimationController.h>
#include "filesystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

ConVar tf2c_mainmenu_music( "tf2c_mainmenu_music", "1", FCVAR_ARCHIVE, "Toggle music in the main menu" );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFMainMenuPanel::CTFMainMenuPanel( Panel* parent, const char *panelName ) : CTFMenuPanelBase( parent, panelName )
{
	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );

	m_pVersionLabel = new CExLabel( this, "VersionLabel", "" );
	m_pNotificationButton = new CTFButton( this, "NotificationButton", "" );
	m_pProfileAvatar = new CAvatarImagePanel( this, "AvatarImage" );
	m_pFakeBGImage = new ImagePanel( this, "FakeBGImage" );

	m_psMusicStatus = MUSIC_FIND;
	m_pzMusicLink[0] = '\0';
	m_nSongGuid = 0;

	if ( steamapicontext->SteamUser() )
	{
		m_SteamID = steamapicontext->SteamUser()->GetSteamID();
	}

	m_iShowFakeIntro = 4;

	ivgui()->AddTickSignal( GetVPanel() );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFMainMenuPanel::~CTFMainMenuPanel()
{

}

void CTFMainMenuPanel::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/UI/main_menu/MainMenuPanel.res" );

	SetVersionLabel();
}

void CTFMainMenuPanel::PerformLayout()
{
	m_pProfileAvatar->SetPlayer( m_SteamID, k_EAvatarSize64x64 );
	m_pProfileAvatar->SetShouldDrawFriendIcon( false );

	char szNickName[64];
#if 0
	Q_snprintf(szNickName, sizeof(szNickName),
		(steamapicontext->SteamFriends() ? steamapicontext->SteamFriends()->GetPersonaName() : "Unknown"));
#else
	ConVarRef playername( "name" );
	Q_snprintf(szNickName, sizeof(szNickName), "%s", playername.GetString() );
#endif
	SetDialogVariable("nickname", szNickName);

	OnNotificationUpdate();

	if ( m_iShowFakeIntro > 0 )
	{
		char szBGName[128];
		engine->GetMainMenuBackgroundName( szBGName, sizeof( szBGName ) );
		char szImage[128];
		V_sprintf_safe( szImage, "../console/%s", szBGName );
		int width, height;
		surface()->GetScreenSize( width, height );
		float fRatio = (float)width / (float)height;
		bool bWidescreen = ( fRatio < 1.5 ? false : true );
		if ( bWidescreen )
			V_strcat_safe( szImage, "_widescreen" );
		m_pFakeBGImage->SetImage( szImage );
		m_pFakeBGImage->SetVisible( true );
		m_pFakeBGImage->SetAlpha( 255 );
	}
}

void CTFMainMenuPanel::OnCommand( const char* command )
{
	if ( !V_stricmp( command, "newquit" ) )
	{
		guiroot->ShowPanel( QUIT_MENU );
	}
	else if ( !V_stricmp( command, "newoptionsdialog" ) )
	{
		guiroot->ShowPanel( OPTIONSDIALOG_MENU );
	}
	else if ( !V_stricmp( command, "newloadout" ) )
	{
		guiroot->ShowPanel( LOADOUT_MENU );
	}
	else if ( !V_stricmp( command, "newcreateserver" ) )
	{
		guiroot->ShowPanel( CREATESERVER_MENU );
	}
	else if ( !V_stricmp( command, "newstats" ) )
	{
		guiroot->ShowPanel( STATSUMMARY_MENU );
	}
	else if ( !V_stricmp( command, "checkversion" ) )
	{
		//MAINMENU_ROOT->CheckVersion();
	}
	else if (!V_stricmp( command, "shownotification" ) )
	{
		if ( m_pNotificationButton )
		{
			m_pNotificationButton->SetGlowing( false );
		}
		guiroot->ShowPanel( NOTIFICATION_MENU );
	}
	else if ( !V_stricmp( command, "testnotification" ) )
	{
		wchar_t resultString[128];
		V_swprintf_safe( resultString, L"test %d", GetNotificationManager()->GetNotificationsCount() );
		MessageNotification Notification(L"Yoyo", resultString, time( NULL ) );
		GetNotificationManager()->SendNotification( Notification );
	}
	else if ( !V_stricmp( command, "randommusic" ) )
	{
		enginesound->StopSoundByGuid( m_nSongGuid );
	}
	else if ( V_stristr( command, "gamemenucommand " ) )
	{
		engine->ClientCmd( command );
	}
	else
	{
		BaseClass::OnCommand( command );
	}
}

void CTFMainMenuPanel::OnTick()
{
	if ( tf2c_mainmenu_music.GetBool() && !guiroot->IsInLevel() )
	{
		if ( ( m_psMusicStatus == MUSIC_FIND || m_psMusicStatus == MUSIC_STOP_FIND ) && !enginesound->IsSoundStillPlaying( m_nSongGuid ) )
		{
			GetRandomMusic(m_pzMusicLink, sizeof(m_pzMusicLink));
			m_psMusicStatus = MUSIC_PLAY;
		}
		else if ( ( m_psMusicStatus == MUSIC_PLAY || m_psMusicStatus == MUSIC_STOP_PLAY ) && m_pzMusicLink[0] != '\0' )
		{
			enginesound->StopSoundByGuid(m_nSongGuid);
			ConVar *snd_musicvolume = cvar->FindVar("snd_musicvolume");
			float fVolume = (snd_musicvolume ? snd_musicvolume->GetFloat() : 1.0f);
			enginesound->EmitAmbientSound(m_pzMusicLink, fVolume, PITCH_NORM, 0);			
			m_nSongGuid = enginesound->GetGuidForLastSoundEmitted();
			m_psMusicStatus = MUSIC_FIND;
		}
	}
	else if ( m_psMusicStatus == MUSIC_FIND )
	{
		enginesound->StopSoundByGuid( m_nSongGuid );
		m_psMusicStatus = ( m_nSongGuid == 0 ? MUSIC_STOP_FIND : MUSIC_STOP_PLAY );
	}
}

void CTFMainMenuPanel::OnThink()
{
	if ( m_iShowFakeIntro > 0 )
	{
		m_iShowFakeIntro--;
		if ( m_iShowFakeIntro == 0 )
		{
			GetAnimationController()->RunAnimationCommand( m_pFakeBGImage, "Alpha", 0, 1.0f, 0.5f, AnimationController::INTERPOLATOR_SIMPLESPLINE );
		}
	}

	if ( m_pFakeBGImage->IsVisible() && m_pFakeBGImage->GetAlpha() == 0 )
	{
		m_pFakeBGImage->SetVisible( false );
	}
}

void CTFMainMenuPanel::Show()
{
	BaseClass::Show();

	RequestFocus();
}

void CTFMainMenuPanel::Hide()
{
	BaseClass::Hide();
}

void CTFMainMenuPanel::HideFakeIntro( void )
{
	m_iShowFakeIntro = 0;
	m_pFakeBGImage->SetVisible( false );
}

void CTFMainMenuPanel::OnNotificationUpdate()
{
	if ( m_pNotificationButton )
	{
		if ( GetNotificationManager()->GetNotificationsCount() > 0 )
		{
			m_pNotificationButton->SetVisible( true );
		}
		else
		{
			m_pNotificationButton->SetVisible( false );
		}

		if ( GetNotificationManager()->GetUnreadNotificationsCount() > 0 )
		{
			m_pNotificationButton->SetGlowing( true );
		}
		else
		{
			m_pNotificationButton->SetGlowing( false );
		}
	}

	if ( GetNotificationManager()->IsOutdated() )
	{
		if ( m_pVersionLabel )
		{
			m_pVersionLabel->SetFgColor( Color( 255, 20, 50, 200 ) );
		}
	}
};

extern ConVar tf2c_buildnum;
void CTFMainMenuPanel::SetVersionLabel()  //GetVersionString
{
	if ( m_pVersionLabel )
	{
		char verString[128];
		Q_snprintf( verString, sizeof( verString ), "Version: %s\nBuild: %d\nCommit: %s", GetNotificationManager()->GetVersionName(), tf2c_buildnum.GetInt(), GetNotificationManager()->GetVersionCommit() );
		m_pVersionLabel->SetText( verString );
	}
};

void CTFMainMenuPanel::GetRandomMusic( char *pszBuf, int iBufLength )
{
	Assert(iBufLength);

	char szPath[MAX_PATH];

	// Check that there's music available
	if (!g_pFullFileSystem->FileExists("sound/ui/gamestartup1.mp3"))
	{
		Assert(false);
		*pszBuf = '\0';
	}

	// Discover tracks, 1 through n
	int iLastTrack = 0;
	do
	{
		Q_snprintf(szPath, sizeof(szPath), "sound/ui/gamestartup%d.mp3", ++iLastTrack);
	} while (g_pFullFileSystem->FileExists(szPath));

	// Pick a random one
	Q_snprintf(szPath, sizeof(szPath), "ui/gamestartup%d.mp3", RandomInt(1, iLastTrack - 1));
	Q_strncpy(pszBuf, szPath, iBufLength);
}
