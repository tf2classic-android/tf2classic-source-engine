#include "cbase.h"
#include "tf_mainmenupanel.h"
#include "controls/tf_advbutton.h"
#include "controls/tf_advslider.h"
#include "vgui_controls/SectionedListPanel.h"
#include "vgui_controls/ImagePanel.h"
#include "tf_notificationmanager.h"
#include "engine/IEngineSound.h"
#include "vgui_avatarimage.h"

using namespace vgui;
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar tf2c_mainmenu_music("tf2c_mainmenu_music", "1", FCVAR_ARCHIVE, "Toggle music in the main menu");

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFMainMenuPanel::CTFMainMenuPanel(vgui::Panel* parent, const char *panelName) : CTFMenuPanelBase(parent, panelName)
{
	Init();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFMainMenuPanel::~CTFMainMenuPanel()
{

}

bool CTFMainMenuPanel::Init()
{
	BaseClass::Init();

	m_psMusicStatus = MUSIC_FIND;
	m_pzMusicLink[0] = '\0';
	m_nSongGuid = 0;

	if (steamapicontext->SteamUser())
	{
		m_SteamID = steamapicontext->SteamUser()->GetSteamID();
	}

	m_iShowFakeIntro = 4;
	m_pVersionLabel = NULL;
	m_pNotificationButton = NULL;
	m_pProfileAvatar = NULL;
	m_pFakeBGImage = NULL;

	bInMenu = true;
	bInGame = false;
	return true;
}


void CTFMainMenuPanel::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	LoadControlSettings("resource/UI/main_menu/MainMenuPanel.res");
	m_pVersionLabel = dynamic_cast<CExLabel *>(FindChildByName("VersionLabel"));
	m_pNotificationButton = dynamic_cast<CTFButton *>(FindChildByName("NotificationButton"));
	m_pProfileAvatar = dynamic_cast<CAvatarImagePanel *>(FindChildByName("AvatarImage"));
	m_pFakeBGImage = dynamic_cast<vgui::ImagePanel *>(FindChildByName("FakeBGImage"));

	SetVersionLabel();
}	

void CTFMainMenuPanel::PerformLayout()
{
	BaseClass::PerformLayout();

	if( m_pProfileAvatar )
	{
		m_pProfileAvatar->SetPlayer(m_SteamID, k_EAvatarSize64x64);
		m_pProfileAvatar->SetShouldDrawFriendIcon(false);
	}

	char szNickName[64];
#if 0
	Q_snprintf(szNickName, sizeof(szNickName),
		(steamapicontext->SteamFriends() ? steamapicontext->SteamFriends()->GetPersonaName() : "Unknown"));
#else
	ConVarRef playername( "name" );
	Q_snprintf(szNickName, sizeof(szNickName),
		"%s", playername.GetString() ); // smells like semen in the air
#endif
	SetDialogVariable("nickname", szNickName);

	OnNotificationUpdate();
	AutoLayout();

	if (m_iShowFakeIntro > 0)
	{
		char szBGName[128];
		engine->GetMainMenuBackgroundName(szBGName, sizeof(szBGName));
		char szImage[128];
		Q_snprintf(szImage, sizeof(szImage), "../console/%s", szBGName);
		int width, height;
		surface()->GetScreenSize(width, height);
		float fRatio = (float)width / (float)height;
		bool bWidescreen = (fRatio < 1.5 ? false : true);
		if (bWidescreen)
			Q_strcat(szImage, "_widescreen", sizeof(szImage));
		m_pFakeBGImage->SetImage(szImage);
		m_pFakeBGImage->SetVisible(true);
		m_pFakeBGImage->SetAlpha(255);
	}
};

void CTFMainMenuPanel::OnCommand(const char* command)
{
	if (!Q_strcmp(command, "newquit"))
	{
		MAINMENU_ROOT->ShowPanel(QUIT_MENU);
	}
	else if (!Q_strcmp(command, "newoptionsdialog"))
	{
		MAINMENU_ROOT->ShowPanel(OPTIONSDIALOG_MENU);
	}
	else if (!Q_strcmp(command, "newloadout"))
	{
		MAINMENU_ROOT->ShowPanel(LOADOUT_MENU);
	}
	else if (!Q_strcmp(command, "newcreateserver"))
	{
		MAINMENU_ROOT->ShowPanel(CREATESERVER_MENU);
	}
	else if (!Q_strcmp(command, "newstats"))
	{
		MAINMENU_ROOT->ShowPanel(STATSUMMARY_MENU);
	}
	else if (!Q_strcmp(command, "checkversion"))
	{
		//MAINMENU_ROOT->CheckVersion();
	}
	else if (!Q_strcmp(command, "shownotification"))
	{
		if (m_pNotificationButton)
		{
			m_pNotificationButton->SetGlowing(false);
		}
		MAINMENU_ROOT->ShowPanel(NOTIFICATION_MENU);
	}
	else if (!Q_strcmp(command, "testnotification"))
	{
		wchar_t resultString[128];
		V_snwprintf(resultString, sizeof(resultString), L"test %d", GetNotificationManager()->GetNotificationsCount());
		MessageNotification Notification(L"Yoyo", resultString, time( NULL ) );
		GetNotificationManager()->SendNotification(Notification);
	}
	else if (!Q_strcmp(command, "randommusic"))
	{
		enginesound->StopSoundByGuid(m_nSongGuid);
	}
	else if (Q_strcmp(command, "gamemenucommand "))
	{
		engine->ClientCmd(command);
	}
	else
	{
		BaseClass::OnCommand(command);
	}
}

void CTFMainMenuPanel::OnTick()
{
	BaseClass::OnTick();

	if( tf2c_mainmenu_music.GetBool() && !bInGameLayout )
	{
		if ((m_psMusicStatus == MUSIC_FIND || m_psMusicStatus == MUSIC_STOP_FIND) && !enginesound->IsSoundStillPlaying(m_nSongGuid))
		{
			GetRandomMusic(m_pzMusicLink, sizeof(m_pzMusicLink));
			m_psMusicStatus = MUSIC_PLAY;
		}
		else if( ( m_psMusicStatus == MUSIC_PLAY || m_psMusicStatus == MUSIC_STOP_PLAY ) && m_pzMusicLink[0] != '\0' )
		{
			enginesound->StopSoundByGuid(m_nSongGuid);
			ConVar *snd_musicvolume = cvar->FindVar("snd_musicvolume");
			float fVolume = (snd_musicvolume ? snd_musicvolume->GetFloat() : 1.0f);
			enginesound->EmitAmbientSound(m_pzMusicLink, fVolume, PITCH_NORM, 0);			
			m_nSongGuid = enginesound->GetGuidForLastSoundEmitted();
			m_psMusicStatus = MUSIC_FIND;
		}
	}
	else if( m_psMusicStatus == MUSIC_FIND )
	{
		enginesound->StopSoundByGuid(m_nSongGuid);
		m_psMusicStatus = (m_nSongGuid == 0 ? MUSIC_STOP_FIND : MUSIC_STOP_PLAY);
	}
};

void CTFMainMenuPanel::OnThink()
{
	BaseClass::OnThink();

	if (m_iShowFakeIntro > 0)
	{
		m_iShowFakeIntro--;
		if (m_iShowFakeIntro == 0)
		{
			vgui::GetAnimationController()->RunAnimationCommand(m_pFakeBGImage, "Alpha", 0, 1.0f, 0.5f, vgui::AnimationController::INTERPOLATOR_SIMPLESPLINE);
		}
	}

	if (m_pFakeBGImage->IsVisible() && m_pFakeBGImage->GetAlpha() == 0)
	{
		m_pFakeBGImage->SetVisible(false);
	}
};

void CTFMainMenuPanel::Show()
{
	BaseClass::Show();
	vgui::GetAnimationController()->RunAnimationCommand(this, "Alpha", 255, 0.0f, 0.5f, vgui::AnimationController::INTERPOLATOR_SIMPLESPLINE);
};

void CTFMainMenuPanel::Hide()
{
	BaseClass::Hide();
	vgui::GetAnimationController()->RunAnimationCommand(this, "Alpha", 0, 0.0f, 0.1f, vgui::AnimationController::INTERPOLATOR_LINEAR);
};


void CTFMainMenuPanel::DefaultLayout()
{
	BaseClass::DefaultLayout();
};

void CTFMainMenuPanel::GameLayout()
{
	BaseClass::GameLayout();
};

void CTFMainMenuPanel::PlayMusic()
{

}

void CTFMainMenuPanel::OnNotificationUpdate()
{
	if (m_pNotificationButton)
	{
		if (GetNotificationManager()->GetNotificationsCount() > 0)
		{
			m_pNotificationButton->SetVisible(true);
		}
		else
		{
			m_pNotificationButton->SetVisible(false);
		}

		if (GetNotificationManager()->GetUnreadNotificationsCount() > 0)
		{
			m_pNotificationButton->SetGlowing(true);
		}
		else
		{
			m_pNotificationButton->SetGlowing(false);
		}
	}

	if (GetNotificationManager()->IsOutdated())
	{
		if (m_pVersionLabel)
		{
			m_pVersionLabel->SetFgColor(Color(255, 20, 50, 100));
		}
	}
};

extern ConVar tf2c_cl_buildnum;
void CTFMainMenuPanel::SetVersionLabel()  //GetVersionString
{
	if (m_pVersionLabel)
	{
		char verString[64];
		Q_snprintf(verString, sizeof(verString), "Version: %s\nBuild: %d", GetNotificationManager()->GetVersionName(), tf2c_cl_buildnum.GetInt());
		m_pVersionLabel->SetText(verString);
	}
};

void CTFMainMenuPanel::GetRandomMusic(char *pszBuf, int iBufLength)
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
