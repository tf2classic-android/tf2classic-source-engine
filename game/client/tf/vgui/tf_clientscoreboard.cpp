//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include <vgui_controls/SectionedListPanel.h>
#include <vgui_controls/Menu.h>
#include <vgui_controls/MenuItem.h>
#include <vgui_controls/ImageList.h>
#include "shareddefs.h"
#include "tf_hud_playerstatus.h"
#include "tf_playermodelpanel.h"
#include "tf_shareddefs.h"
#include "tier3/tier3.h"
#include "vgui_avatarimage.h"
#include "tf_clientscoreboard.h"
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <inputsystem/iinputsystem.h>
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/ImagePanel.h"
#include "voice_status.h"
#include "gamevars_shared.h"
#include "c_tf_playerresource.h"
#include "c_tf_player.h"
#include "c_tf_team.h"
#include "tf_hud_statpanel.h"
#include "tf_gamerules.h"

using namespace vgui;

ConVar cl_hud_playerclass_use_playermodel( "cl_hud_playerclass_use_playermodel", "0", FCVAR_ARCHIVE, "Use player model in player class HUD." );
ConVar tf_scoreboard_mouse_mode( "tf_scoreboard_mouse_mode", "0", FCVAR_ARCHIVE, "", true, 0, true, 2 );

//extern ConVar cl_hud_playerclass_use_playermodel;

bool IsInCommentaryMode( void );
const char *GetMapDisplayName( const char *mapName );

//-----------------------------------------------------------------------------
// Purpose: Returns a localized string of form "1 point", "2 points", etc for specified # of points
//-----------------------------------------------------------------------------
const wchar_t *GetPointsString( int iPoints )
{
	wchar_t wzScoreVal[128];
	static wchar_t wzScore[128];
	_snwprintf( wzScoreVal, ARRAYSIZE( wzScoreVal ), L"%i", iPoints );
	if ( 1 == iPoints ) 
	{
		g_pVGuiLocalize->ConstructString( wzScore, sizeof(wzScore), g_pVGuiLocalize->Find( "#TF_ScoreBoard_Point" ), 1, wzScoreVal );
	}
	else
	{
		g_pVGuiLocalize->ConstructString( wzScore, sizeof(wzScore), g_pVGuiLocalize->Find( "#TF_ScoreBoard_Points" ), 1, wzScoreVal );
	}
	return wzScore;
}


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFClientScoreBoardDialog::CTFClientScoreBoardDialog( IViewPort *pViewPort, const char *pszName, int iTeamCount ) : EditablePanel( NULL, pszName )
{
	m_iTeamCount = iTeamCount;
	m_nCloseKey = BUTTON_CODE_INVALID;

	SetProportional( true );
	SetKeyBoardInputEnabled( false );
	SetMouseInputEnabled( false );
	MakePopup( true );

	// set the scheme before any child control is created
	SetScheme( "ClientScheme" );

	memset( m_pPlayerLists, 0, sizeof( m_pPlayerLists ) );

	// Create player lists.
	if( m_iTeamCount > 2 )
	{
		for ( int i = 0; i < m_iTeamCount; i++ )
		{
			m_pPlayerLists[i] = new SectionedListPanel( this, VarArgs( "%sPlayerList", g_aTeamNames[i] ) );
		}
	}

	m_pPlayerLists[TEAM_UNASSIGNED]->SetEnabled( false );
	m_pPlayerLists[TEAM_UNASSIGNED]->SetVisible( false );
	m_pPlayerLists[TEAM_UNASSIGNED]->SetDrawHeaders( false );
	m_pPlayerLists[TEAM_SPECTATOR]->SetEnabled( false );
	m_pPlayerLists[TEAM_SPECTATOR]->SetVisible( false );
	m_pPlayerLists[TEAM_SPECTATOR]->SetDrawHeaders( false );

	m_pLabelPlayerName = new CExLabel( this, "PlayerNameLabel", "" );
	m_pLabelMapName = new CExLabel( this, "MapName", "" );
	m_pImagePanelHorizLine = new ImagePanel( this, "HorizontalLine" );
	m_pClassImage = new CTFClassImage( this, "ClassImage" );
	m_pClassModelPanel = new CTFPlayerModelPanel( this, "classmodelpanel" );
	m_pLocalPlayerStatsPanel = new vgui::EditablePanel( this, "LocalPlayerStatsPanel" );
	m_pLocalPlayerDuelStatsPanel = new vgui::EditablePanel( this, "LocalPlayerDuelStatsPanel" );
	m_pServerTimeLeftValue = NULL;
	m_iImageDead = 0;
	m_iImageDominated = 0;
	m_iImageNemesis = 0;
	memset( m_iClassEmblem, 0, sizeof( m_iClassEmblem ) );
	memset( m_iClassEmblemDead, 0, sizeof( m_iClassEmblemDead ) );
	memset( m_iImageDominations, 0, sizeof( m_iImageDominations ) );
	memset( m_iDefaultAvatars, 0, sizeof( m_iDefaultAvatars ) );
	memset( m_iDeviceIcons, 0, sizeof( m_iDeviceIcons ) );

	m_iSelectedPlayerIndex = 0;

	m_pContextMenu = new Menu( this, "contextmenu" );
	m_pContextMenu->AddCheckableMenuItem( "#TF_ScoreBoard_Mute", "mute", this );
	m_pContextMenu->AddMenuItem( "#TF_ScoreBoard_Kick", "kick", this );
	m_pContextMenu->AddMenuItem( "#TF_ScoreBoard_Spectate", "spectate", this );
	m_pContextMenu->AddMenuItem( "#TF_ScoreBoard_ShowProfile", "showprofile", this );

	m_pImageList = NULL;
	m_mapAvatarsToImageList.SetLessFunc( DefLessFunc( CSteamID ) );

	ListenForGameEvent( "server_spawn" );

	SetVisible( false );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFClientScoreBoardDialog::~CTFClientScoreBoardDialog()
{
	delete m_pImageList;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::PerformLayout()
{
	BaseClass::PerformLayout();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( GetResFilename() );

	delete m_pImageList;
	m_pImageList = new ImageList( false );

	m_mapAvatarsToImageList.RemoveAll();

	m_iImageDead = m_pImageList->AddImage( scheme()->GetImage( "../hud/leaderboard_dead", true ) );
	m_iImageDominated = m_pImageList->AddImage( scheme()->GetImage( "../hud/leaderboard_dominated", true ) );
	m_iImageNemesis = m_pImageList->AddImage( scheme()->GetImage( "../hud/leaderboard_nemesis", true ) );

	for( int i = TF_FIRST_NORMAL_CLASS; i < TF_CLASS_COUNT; i++ )
	{
		m_iClassEmblem[i] = m_pImageList->AddImage( scheme()->GetImage( g_aPlayerClassEmblems[i - 1], true ) );
		m_iClassEmblemDead[i] = m_pImageList->AddImage( scheme()->GetImage( g_aPlayerClassEmblemsDead[i - 1], true ) );
	}
	
	for ( int i = 0; i < TF_SCOREBOARD_MAX_DOMINATIONS; i++ )
	{
		m_iImageDominations[i] = m_pImageList->AddImage( scheme()->GetImage( VarArgs( "../hud/leaderboard_dom%d", i + 1 ), true ) );
	}
	
	m_iDeviceIcons[DEVICE_COMPUTER] = m_pImageList->AddImage( scheme()->GetImage( "../vgui/scoreboard/tango_computer", true ) );
	m_iDeviceIcons[DEVICE_PHONE] = m_pImageList->AddImage( scheme()->GetImage( "../vgui/scoreboard/tango_phone", true ) );
	m_iDeviceIcons[DEVICE_BOT] = m_pImageList->AddImage( scheme()->GetImage( "../vgui/scoreboard/tango_applications-system", true ) );

	// resize the images to our resolution
	for ( int i = 1; i < m_pImageList->GetImageCount(); i++ )
	{
		int wide = 13, tall = 13;
		m_pImageList->GetImage( i )->SetSize( scheme()->GetProportionalScaledValueEx( GetScheme(), wide ), scheme()->GetProportionalScaledValueEx( GetScheme(), tall ) );
	}

	CAvatarImage *pImage = new CAvatarImage();
	pImage->SetAvatarSize( 32, 32 );	// Deliberately non scaling
	pImage->SetDefaultImage( scheme()->GetImage( "../vgui/avatar_default", true ) );
	m_iDefaultAvatars[0] = m_pImageList->AddImage( pImage );

	for ( int i = FIRST_GAME_TEAM; i < m_iTeamCount; i++ )
	{
		const char *pszImage = VarArgs( "../vgui/avatar_default_%s", g_aTeamLowerNames[i] );

		CAvatarImage *pImage = new CAvatarImage();
		pImage->SetAvatarSize( 32, 32 );	// Deliberately non scaling
		pImage->SetDefaultImage( scheme()->GetImage( pszImage, true ) );
		m_iDefaultAvatars[i] = m_pImageList->AddImage( pImage );
	}

	if( m_iTeamCount > 2 )
	{
		for ( int i = 0; i < m_iTeamCount; i++ )
		{
			SetPlayerListImages( m_pPlayerLists[i] );
		}
	}

	SetBgColor( Color( 0, 0, 0, 0 ) );
	SetBorder( NULL );
	SetVisible( false );

	m_hTimeLeftFont = pScheme->GetFont( "ScoreboardMedium", true );
	m_hTimeLeftNotSetFont = pScheme->GetFont( "ScoreboardVerySmall", true );
	m_pLocalPlayerDuelStatsPanel->SetVisible( false );

	m_pServerTimeLeftValue = dynamic_cast<CExLabel *>( FindChildByName( "ServerTimeLeftValue" ) );

	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::ShowPanel( bool bShow )
{
	// Catch the case where we call ShowPanel before ApplySchemeSettings, eg when
	// going from windowed <-> fullscreen
	if ( m_pImageList == NULL )
	{
		InvalidateLayout( true, true );
	}

	// Don't show in commentary mode
	if ( IsInCommentaryMode() )
	{
		bShow = false;
	}

	if ( IsVisible() == bShow )
	{
		return;
	}

	int iRenderGroup = gHUD.LookupRenderGroupIndexByName( "global" );

	if ( bShow )
	{
		SetVisible( true );
		MoveToFront();

		SetKeyBoardInputEnabled( false );
		SetMouseInputEnabled( tf_scoreboard_mouse_mode.GetInt() == 1 );

		gHUD.LockRenderGroup( iRenderGroup );

		// Clear the selected item, this forces the default to the local player
		SectionedListPanel *pList = GetSelectedPlayerList();
		if ( pList )
		{
			pList->ClearSelection();
		}
	}
	else
	{
		SetVisible( false );

		m_pContextMenu->SetVisible( false );
		SetKeyBoardInputEnabled( false );
		SetMouseInputEnabled( false );
		m_nCloseKey = BUTTON_CODE_INVALID;

		gHUD.UnlockRenderGroup( iRenderGroup );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::OnCommand( const char* command )
{
	C_TFPlayer *pLocalPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pLocalPlayer )
		return;

	if ( !Q_strcmp( command, "mute" ) )
	{
		bool bMuted = GetClientVoiceMgr()->IsPlayerBlocked( m_iSelectedPlayerIndex );
		GetClientVoiceMgr()->SetPlayerBlockedState( m_iSelectedPlayerIndex, !bMuted );
	}
	else if ( !Q_strcmp( command, "kick" ) )
	{
		if ( g_TF_PR )
		{
			int iUserID = g_TF_PR->GetUserID( m_iSelectedPlayerIndex );

			if ( iUserID )
			{
				engine->ClientCmd( VarArgs( "callvote Kick %d", iUserID ) );
			}
		}
	}
	else if ( !Q_strcmp( command, "spectate" ) )
	{
		engine->ClientCmd( VarArgs( "spec_player %d", m_iSelectedPlayerIndex ) );
	}
	else if ( !Q_strcmp( command, "showprofile" ) )
	{
		CSteamID steamID;
		if ( g_TF_PR && g_TF_PR->GetSteamID( m_iSelectedPlayerIndex, &steamID ) )
		{
			steamapicontext->SteamFriends()->ActivateGameOverlayToUser( "steamid", steamID );
		}
	}
	else
	{
		BaseClass::OnCommand( command );
	}
}

//-----------------------------------------------------------------------------
// Call every frame
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::OnThink()
{
	BaseClass::OnThink();

	// NOTE: this is necessary because of the way input works.
	// If a key down message is sent to vgui, then it will get the key up message
	// Sometimes the scoreboard is activated by other vgui menus, 
	// sometimes by console commands. In the case where it's activated by
	// other vgui menus, we lose the key up message because this panel
	// doesn't accept keyboard input. It *can't* accept keyboard input
	// because another feature of the dialog is that if it's triggered
	// from within the game, you should be able to still run around while
	// the scoreboard is up. That feature is impossible if this panel accepts input.
	// because if a vgui panel is up that accepts input, it prevents the engine from
	// receiving that input. So, I'm stuck with a polling solution.
	// 
	// Close key is set to non-invalid when something other than a keybind
	// brings the scoreboard up, and it's set to invalid as soon as the 
	// dialog becomes hidden.
	if ( m_nCloseKey != BUTTON_CODE_INVALID )
	{
		if ( !g_pInputSystem->IsButtonDown( m_nCloseKey ) )
		{
			m_nCloseKey = BUTTON_CODE_INVALID;
			gViewPortInterface->ShowPanel( this, false );
			GetClientVoiceMgr()->StopSquelchMode();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::UpdatePlayerAvatar( int playerIndex, KeyValues *kv )
{
	if ( !g_TF_PR )
		return;

	if ( g_TF_PR->IsFakePlayer( playerIndex ) )
	{
		// Show default avatars for bots.
		kv->SetInt( "avatar", GetDefaultAvatar( playerIndex ) );
	}
	else
	{
		// FIXME: STEAM
		kv->SetInt( "avatar", GetDefaultAvatar( playerIndex ) );
		// Get their avatar from Steam.
		/*
		CSteamID steamIDForPlayer;
		if ( g_TF_PR->GetSteamID( playerIndex, &steamIDForPlayer ) )
		{
			// See if we already have that avatar in our list
			int iMapIndex = m_mapAvatarsToImageList.Find( steamIDForPlayer );
			int iImageIndex;
			if ( iMapIndex == m_mapAvatarsToImageList.InvalidIndex() )
			{
				CAvatarImage *pImage = new CAvatarImage();
				pImage->SetAvatarSteamID( steamIDForPlayer, k_EAvatarSize64x64 );
				pImage->SetAvatarSize( 32, 32 );	// Deliberately non scaling
				iImageIndex = m_pImageList->AddImage( pImage );

				m_mapAvatarsToImageList.Insert( steamIDForPlayer, iImageIndex );
			}
			else
			{
				iImageIndex = m_mapAvatarsToImageList[iMapIndex];
			}

			kv->SetInt( "avatar", iImageIndex );

			CAvatarImage *pAvIm = (CAvatarImage *)m_pImageList->GetImage( iImageIndex );
			pAvIm->UpdateFriendStatus();
		}
		*/
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CTFClientScoreBoardDialog::GetDefaultAvatar( int playerIndex )
{
	return m_iDefaultAvatars[g_TF_PR->GetTeam( playerIndex )];
}

//-----------------------------------------------------------------------------
// Purpose: Keyboard input hook. Return 0 if handled
//-----------------------------------------------------------------------------
int	CTFClientScoreBoardDialog::HudElementKeyInput( int down, ButtonCode_t keynum, const char *pszCurrentBinding )
{
	if ( IsVisible() && tf_scoreboard_mouse_mode.GetInt() == 2 && keynum == MOUSE_RIGHT && down && !IsMouseInputEnabled() )
	{
		SetMouseInputEnabled( true );
		return 0;
	}

	return 1;
}

//-----------------------------------------------------------------------------
// Called by vgui panels that activate the client scoreboard
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::OnPollHideCode( int code )
{
	m_nCloseKey = (ButtonCode_t)code;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::ShowContextMenu( KeyValues *data )
{
	Panel *pItem = (Panel*)data->GetPtr( "SubPanel" );
	if ( !pItem )
		return;

	SectionedListPanel *pList = (SectionedListPanel *)pItem->GetParent();
	if ( !pList )
		return;

	int iItem = data->GetInt( "itemID" );

	KeyValues *pData = pList->GetItemData( iItem );
	m_iSelectedPlayerIndex = pData->GetInt( "playerIndex", 0 );

	if ( g_PR->IsLocalPlayer( m_iSelectedPlayerIndex ) )
		return;

	C_TFPlayer *pLocalPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pLocalPlayer )
		return;

	bool bMuted = GetClientVoiceMgr()->IsPlayerBlocked( m_iSelectedPlayerIndex );
	m_pContextMenu->GetMenuItem( 0 )->SetChecked( bMuted );

	// Can only votekick teammates.
	bool bCanKick = ( g_PR->GetTeam( m_iSelectedPlayerIndex ) == GetLocalPlayerTeam() );
	m_pContextMenu->GetMenuItem( 1 )->SetEnabled( bCanKick );

	// Cannot mute bots or show their profile.
	bool bIsBot = g_PR->IsFakePlayer( m_iSelectedPlayerIndex );
	m_pContextMenu->GetMenuItem( 0 )->SetEnabled( !bIsBot );
	m_pContextMenu->GetMenuItem( 3 )->SetEnabled( !bIsBot );

	bool bCanSpectate = false;

	// Only when in manual spectator mode.
	if ( pLocalPlayer->GetObserverMode() > OBS_MODE_FIXED && g_PR->IsAlive( m_iSelectedPlayerIndex ) )
	{
		if ( pLocalPlayer->GetTeamNumber() == TEAM_SPECTATOR )
		{
			bCanSpectate = true;
		}
		else
		{
			switch ( mp_forcecamera.GetInt() )
			{
			case OBS_ALLOW_ALL:
				bCanSpectate = true;
				break;
			case OBS_ALLOW_TEAM:
				bCanSpectate = !g_TF_PR->IsEnemyPlayer( m_iSelectedPlayerIndex );
				break;
			case OBS_ALLOW_NONE:
				bCanSpectate = false;
				break;
			}
		}
	}

	m_pContextMenu->GetMenuItem( 2 )->SetEnabled( bCanSpectate );

	Menu::PlaceContextMenu( this, m_pContextMenu );
}

//-----------------------------------------------------------------------------
// Purpose: Resets the scoreboard panel
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::Reset()
{
	m_flNextUpdateTime = 0.0f;

	if( m_iTeamCount > 2 )
	{
		for ( int i = 0; i < m_iTeamCount; i++ )
		{
			InitPlayerList( m_pPlayerLists[i] );
		}
	}

	// SanyaSho: this code is not in 2017
	if ( m_pServerTimeLeftValue )
		m_pServerTimeLeftValue->SetFgColor( Color( 255, 255, 0, 255 ) );
}

//-----------------------------------------------------------------------------
// Purpose: Inits the player list in a list panel
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::InitPlayerList( SectionedListPanel *pPlayerList )
{
	pPlayerList->SetVerticalScrollbar( false );
	pPlayerList->RemoveAll();
	pPlayerList->RemoveAllSections();
	pPlayerList->AddSection( 0, "Players", TFPlayerSortFunc );
	pPlayerList->SetSectionAlwaysVisible( 0, true );
	pPlayerList->SetSectionFgColor( 0, Color( 255, 255, 255, 255 ) );
	pPlayerList->SetBgColor( Color( 0, 0, 0, 0 ) );
	pPlayerList->SetBorder( NULL );
	
	pPlayerList->AddColumnToSection( 0, "device", "", SectionedListPanel::COLUMN_IMAGE | SectionedListPanel::COLUMN_CENTER, m_iDeviceWidth );

	// Avatars are always displayed at 32x32 regardless of resolution
	pPlayerList->AddColumnToSection( 0, "avatar", "", SectionedListPanel::COLUMN_IMAGE | SectionedListPanel::COLUMN_CENTER, m_iAvatarWidth );

	// Stretch player name column so that it fills up all unused space.
	pPlayerList->AddColumnToSection(
				0,
				"name",
				"#TF_Scoreboard_Name",
				0,
				pPlayerList->GetWide()
				+ -15
				- 2 * this->m_iNemesisWidth
				- this->m_iDeviceWidth
				- this->m_iAvatarWidth
				- this->m_iStatusWidth
				- this->m_iPingWidth
				- this->m_iScoreWidth
				- this->m_iClassWidth,
				0);
	pPlayerList->AddColumnToSection( 0, "status", "", SectionedListPanel::COLUMN_IMAGE, m_iStatusWidth );
	pPlayerList->AddColumnToSection( 0, "domination", "", SectionedListPanel::COLUMN_IMAGE, m_iNemesisWidth );
	pPlayerList->AddColumnToSection( 0, "nemesis", "", SectionedListPanel::COLUMN_IMAGE, m_iNemesisWidth );
	pPlayerList->AddColumnToSection( 0, "class", "", SectionedListPanel::COLUMN_IMAGE, m_iClassWidth );
	pPlayerList->AddColumnToSection( 0, "score", "#TF_Scoreboard_Score", SectionedListPanel::COLUMN_RIGHT, m_iScoreWidth );
	pPlayerList->AddColumnToSection( 0, "ping", "#TF_Scoreboard_Ping", SectionedListPanel::COLUMN_RIGHT, m_iPingWidth );
}

//-----------------------------------------------------------------------------
// Purpose: Builds the image list to use in the player list
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::SetPlayerListImages( vgui::SectionedListPanel *pPlayerList )
{
	pPlayerList->SetImageList( m_pImageList, false );
	pPlayerList->SetVisible( true );
}

//-----------------------------------------------------------------------------
// Purpose: Updates the dialog
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::Update()
{
	UpdateTeamInfo();
	UpdatePlayerList();
	UpdateSpectatorList();
	UpdateArenaWaitingToPlayList();
	UpdatePlayerDetails();
	MoveToCenterOfScreen();

	// Not really sure where to put this
	if ( TFGameRules() )
	{
		if ( mp_timelimit.GetInt() > 0 )
		{
			if ( TFGameRules()->GetTimeLeft() > 0 )
			{
				if ( m_pServerTimeLeftValue )
				{
					m_pServerTimeLeftValue->SetFont( m_hTimeLeftFont );
				}
				
				int iTimeLeft = TFGameRules()->GetTimeLeft();

				wchar_t wszHours[5] = L"";
				wchar_t wszMinutes[3] = L"";
				wchar_t wszSeconds[3] = L"";

				if ( iTimeLeft >= 3600 )
				{
					V_swprintf_safe( wszHours, L"%d", iTimeLeft / 3600 );
					V_swprintf_safe( wszMinutes, L"%02d", ( iTimeLeft / 60 ) % 60 );
					V_swprintf_safe( wszSeconds, L"%02d", iTimeLeft % 60 );
				}
				else
				{
					V_swprintf_safe( wszMinutes, L"%d", iTimeLeft / 60 );
					V_swprintf_safe( wszSeconds, L"%02d", iTimeLeft % 60 );
				}

				wchar_t wzTimeLabelOld[256] = L"";
				wchar_t wzTimeLabelNew[256] = L"";

				if ( iTimeLeft >= 3600 )
				{
					g_pVGuiLocalize->ConstructString( wzTimeLabelOld, sizeof( wzTimeLabelOld ), g_pVGuiLocalize->Find( "#Scoreboard_TimeLeft" ), 3, wszHours, wszMinutes, wszSeconds );
					g_pVGuiLocalize->ConstructString( wzTimeLabelNew, sizeof( wzTimeLabelNew ), g_pVGuiLocalize->Find( "#Scoreboard_TimeLeftNew" ), 3, wszHours, wszMinutes, wszSeconds );
				}
				else
				{
					g_pVGuiLocalize->ConstructString( wzTimeLabelOld, sizeof( wzTimeLabelOld ), g_pVGuiLocalize->Find( "#Scoreboard_TimeLeftNoHours" ), 2, wszMinutes, wszSeconds );
					g_pVGuiLocalize->ConstructString( wzTimeLabelNew, sizeof( wzTimeLabelNew ), g_pVGuiLocalize->Find( "#Scoreboard_TimeLeftNoHoursNew" ), 2, wszMinutes, wszSeconds );
				}

				SetDialogVariable( "servertimeleft", wzTimeLabelOld );
				SetDialogVariable( "servertime", wzTimeLabelNew );
			}
			else // Timer is set and has run out
			{
				if ( m_pServerTimeLeftValue )
				{
					m_pServerTimeLeftValue->SetFont( m_hTimeLeftNotSetFont );
				}

				SetDialogVariable( "servertimeleft", g_pVGuiLocalize->Find( "#Scoreboard_ChangeOnRoundEnd" ) );
				SetDialogVariable( "servertime", g_pVGuiLocalize->Find( "#Scoreboard_ChangeOnRoundEndNew" ) );
			}
		}
		else
		{
			if ( m_pServerTimeLeftValue )
			{
				m_pServerTimeLeftValue->SetFont( m_hTimeLeftNotSetFont );
			}

			SetDialogVariable( "servertimeleft", g_pVGuiLocalize->Find( "#Scoreboard_NoTimeLimit" ) );
			SetDialogVariable( "servertime", g_pVGuiLocalize->Find( "#Scoreboard_NoTimeLimitNew" ) );
		}
	}

	// update every second
	m_flNextUpdateTime = gpGlobals->curtime + 1.0f;
}

bool CTFClientScoreBoardDialog::NeedsUpdate( void )
{
	return ( m_flNextUpdateTime < gpGlobals->curtime );
}

//-----------------------------------------------------------------------------
// Purpose: Updates information about teams
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::UpdateTeamInfo( void )
{
	int iTotalPlayers = 0;

	// update the team sections in the scoreboard
	for ( int teamIndex = FIRST_GAME_TEAM; teamIndex < GetNumberOfTeams(); teamIndex++ )
	{
		C_TFTeam *team = GetGlobalTFTeam( teamIndex );
		if ( !team )
			continue;

		// update # of players on each team
		wchar_t string1[1024];
		wchar_t wNumPlayers[6];
		V_swprintf_safe( wNumPlayers, L"%i", team->Get_Number_Players() );
		if ( team->Get_Number_Players() == 1 )
		{
			g_pVGuiLocalize->ConstructString( string1, sizeof( string1 ), g_pVGuiLocalize->Find( "#TF_ScoreBoard_Player" ), 1, wNumPlayers );
		}
		else
		{
			g_pVGuiLocalize->ConstructString( string1, sizeof( string1 ), g_pVGuiLocalize->Find( "#TF_ScoreBoard_Players" ), 1, wNumPlayers );
		}

		iTotalPlayers += team->Get_Number_Players();

		// set # of players for team in dialog
		SetDialogVariable( VarArgs( "%steamplayercount", g_aTeamLowerNames[teamIndex] ), string1 );

		// set team score in dialog
		SetDialogVariable( VarArgs( "%steamscore", g_aTeamLowerNames[teamIndex] ), team->Get_Score() );

		// set team name
		SetDialogVariable( VarArgs( "%steamname", g_aTeamLowerNames[teamIndex] ), team->GetTeamName() );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Updates the player list
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::UpdatePlayerList( void )
{
	int iSelectedPlayerIndex = GetLocalPlayerIndex();

	// Save off which player we had selected
	SectionedListPanel *pList = GetSelectedPlayerList();

	if ( pList )
	{
		int itemID = pList->GetSelectedItem();

		if ( itemID >= 0 )
		{
			KeyValues *pInfo = pList->GetItemData( itemID );
			if ( pInfo )
			{
				iSelectedPlayerIndex = pInfo->GetInt( "playerIndex" );
			}
		}
	}

	if( m_iTeamCount > 2 )
	{
		for ( int i = 0; i < m_iTeamCount; i++ )
		{
			m_pPlayerLists[i]->RemoveAll();
		}
	}

	if ( !g_TF_PR )
		return;

	for ( int playerIndex = 1; playerIndex <= MAX_PLAYERS; playerIndex++ )
	{
		if ( !g_TF_PR->IsConnected( playerIndex ) )
			continue;

		int iTeam = g_PR->GetTeam( playerIndex );

		if ( iTeam < FIRST_GAME_TEAM )
			continue;
			
		C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
		if( !pPlayer )
			continue;

		KeyValues *pKeyValues = new KeyValues( "data" );

		pKeyValues->SetInt( "playerIndex", playerIndex );
		pKeyValues->SetInt( "device", g_TF_PR->IsFakePlayer( playerIndex ) ? m_iDeviceIcons[DEVICE_BOT] : g_TF_PR->IsMobilePlayer( playerIndex ) ? m_iDeviceIcons[DEVICE_PHONE] : m_iDeviceIcons[DEVICE_COMPUTER] );
		pKeyValues->SetString( "name", g_TF_PR->GetPlayerName( playerIndex ) );
		pKeyValues->SetInt( "score", g_TF_PR->GetTotalScore( playerIndex ) );

		if ( pPlayer->GetTeamNumber() == TEAM_UNASSIGNED || pPlayer->GetTeamNumber() == iTeam )
		{
			int iClass = g_TF_PR->GetPlayerClass( playerIndex );
			if( g_PR->IsLocalPlayer( playerIndex ) && !g_TF_PR->IsAlive( playerIndex ) )
			{
				if( pPlayer->m_Shared.GetDesiredPlayerClassIndex() != TF_CLASS_COUNT_ALL )
				{
					iClass = pPlayer->m_Shared.GetDesiredPlayerClassIndex();
				}
			}
			
			pKeyValues->SetInt( "class", g_TF_PR->IsAlive( playerIndex ) ? m_iClassEmblem[iClass] : m_iClassEmblemDead[iClass] );
		}

		if ( g_TF_PR->IsPlayerDominating( playerIndex ) )
		{
			// if local player is dominated by this player, show a nemesis icon
			pKeyValues->SetInt( "nemesis", m_iImageNemesis );
			//pKeyValues->SetString( "class", "#TF_Nemesis" );
		}
		else if ( g_TF_PR->IsPlayerDominated( playerIndex ) )
		{
			// if this player is dominated by the local player, show the domination icon
			pKeyValues->SetInt( "nemesis", m_iImageDominated );
			//pKeyValues->SetString( "class", "#TF_Dominated" );
		}

		// display whether player is alive or dead (all players see this for all other players on both teams)
		pKeyValues->SetInt( "status", g_TF_PR->IsAlive( playerIndex ) ? 0 : m_iImageDead );

		// Show number of dominations.
		int iDominations = Min( g_TF_PR->GetNumberOfDominations( playerIndex ), TF_SCOREBOARD_MAX_DOMINATIONS );
		pKeyValues->SetInt( "domination", iDominations > 0 ? m_iImageDominations[iDominations - 1] : 0 );
		int iPing = g_PR->GetPing( playerIndex );

		if ( iPing < 1 )
		{
			if ( g_PR->IsFakePlayer( playerIndex ) )
			{
				pKeyValues->SetString( "ping", "#TF_Scoreboard_Bot" );
			}
			else
			{
				pKeyValues->SetString( "ping", "" );
			}
		}
		else
		{
			pKeyValues->SetInt( "ping", iPing );
		}

		UpdatePlayerAvatar( playerIndex, pKeyValues );

		int itemID = m_pPlayerLists[iTeam]->AddItem( 0, pKeyValues );

		Color clr;
#if SOONSOON
		if ( !TFGameRules()->IsFreeForAll() )
#else
		if ( !TFGameRules()->IsDeathmatch() )
#endif
		{
			clr = g_PR->GetTeamColor( g_PR->GetTeam( playerIndex ) );
		}
		else
		{
			clr = g_TF_PR->GetPlayerColor( playerIndex );
		}
		clr[3] = g_TF_PR->IsAlive( playerIndex ) ? 204 : 51;

		m_pPlayerLists[iTeam]->SetItemFgColor( itemID, clr );
		
		if( iSelectedPlayerIndex == playerIndex )
		{
			m_pPlayerLists[iTeam]->SetSelectedItem( itemID );
		}
		else
		{
			m_pPlayerLists[iTeam]->SetItemBgColor( itemID, Color( 0, 0, 0, 64 ) );
		}
		
		pKeyValues->deleteThis();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Updates the spectator list
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::UpdateSpectatorList( void )
{
	if ( !g_TF_PR )
		return;

	// Spectators
	char szSpectatorList[512] = "";
	int nSpectators = 0;

	for ( int i = 1; i <= MAX_PLAYERS; i++ )
	{
		if ( !g_TF_PR->IsConnected( i ) )
			continue;

#if SOONSOON
		// TODO: ARENA
		//if ( TFGameRules()->IsInDuelMode() && !g_TF_PR->IsArenaSpectator( i ) )
		//	continue;

		if( TFGameRules()->GetRetroModeType() == TF_GAMESUBTYPE_DUEL )
			continue;
#endif

		if ( g_TF_PR->GetTeam( i ) == TEAM_SPECTATOR )
		{
			if ( nSpectators > 0 )
			{
				V_strcat_safe( szSpectatorList, ", " );
			}

			V_strcat_safe( szSpectatorList, g_TF_PR->GetPlayerName( i ) );
			nSpectators++;
		}
	}

	wchar_t wzSpectators[512] = L"";
	if ( nSpectators > 0 )
	{
		const char *pchFormat = ( 1 == nSpectators ? "#ScoreBoard_Spectator" : "#ScoreBoard_Spectators" );

		wchar_t wzSpectatorCount[16];
		wchar_t wzSpectatorList[1024];
		V_swprintf_safe( wzSpectatorCount, L"%i", nSpectators );
		g_pVGuiLocalize->ConvertANSIToUnicode( szSpectatorList, wzSpectatorList, sizeof( wzSpectatorList ) );
		g_pVGuiLocalize->ConstructString( wzSpectators, sizeof( wzSpectators ), g_pVGuiLocalize->Find( pchFormat ), 2, wzSpectatorCount, wzSpectatorList );
	}
	SetDialogVariable( "spectators", wzSpectators );
}

//-----------------------------------------------------------------------------
// Purpose: Updates details about a player
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::UpdatePlayerDetails( void )
{
	//ClearPlayerDetails(); // SanyaSho: are u sure?

	if( !g_TF_PR )
		return;

	C_TFPlayer *pLocalPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pLocalPlayer )
		return;

	int playerIndex = pLocalPlayer->entindex();

	// Make sure the selected player is still connected. 
	if ( !g_TF_PR->IsConnected( playerIndex ) ) 
		return;

	if ( engine->IsHLTV() )
	{
		SetDialogVariable( "playername", g_TF_PR->GetPlayerName( playerIndex ) );
		return;
	}

	RoundStats_t &roundStats = GetStatPanel()->GetRoundStatsCurrentGame();

	if ( m_pLocalPlayerStatsPanel )
	{
		m_pLocalPlayerStatsPanel->SetDialogVariable( "kills", g_TF_PR->GetPlayerScore( playerIndex ) );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "deaths", g_TF_PR->GetDeaths( playerIndex ) );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "assists", roundStats.m_iStat[TFSTAT_KILLASSISTS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "destruction", roundStats.m_iStat[TFSTAT_BUILDINGSDESTROYED] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "captures", roundStats.m_iStat[TFSTAT_CAPTURES] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "defenses", roundStats.m_iStat[TFSTAT_DEFENSES] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "dominations", roundStats.m_iStat[TFSTAT_DOMINATIONS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "revenge", roundStats.m_iStat[TFSTAT_REVENGE] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "healing", roundStats.m_iStat[TFSTAT_HEALING] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "invulns", roundStats.m_iStat[TFSTAT_INVULNS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "teleports", roundStats.m_iStat[TFSTAT_TELEPORTS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "headshots", roundStats.m_iStat[TFSTAT_HEADSHOTS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "backstabs", roundStats.m_iStat[TFSTAT_BACKSTABS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "bonus", roundStats.m_iStat[TFSTAT_BONUS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "support", 0 );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "damage", roundStats.m_iStat[TFSTAT_DAMAGE] );
	}
	SetDialogVariable( "playername", g_TF_PR->GetPlayerName( playerIndex ) );
	SetDialogVariable( "playerscore", GetPointsString( g_TF_PR->GetTotalScore( playerIndex ) ) );

	Color clr = g_PR->GetTeamColor( g_PR->GetTeam( playerIndex ) );
	m_pLabelPlayerName->SetFgColor( clr );
	m_pImagePanelHorizLine->SetFillColor( clr );

	int iClass = pLocalPlayer->m_Shared.GetDesiredPlayerClassIndex();
	int iTeam = pLocalPlayer->GetTeamNumber();
	if ( ( iTeam >= FIRST_GAME_TEAM ) && ( iClass >= TF_FIRST_NORMAL_CLASS ) && ( iClass < TF_CLASS_COUNT_ALL ) )
	{
		if ( cl_hud_playerclass_use_playermodel.GetBool() )
		{
			if ( !m_pClassModelPanel->IsVisible() )
			{
				m_pClassModelPanel->SetVisible( true );
			}
			
			if ( m_pClassImage->IsVisible() )
			{
				m_pClassImage->SetVisible( false );
			}
			
			m_pClassModelPanel->SetToPlayerClass( iClass );
			m_pClassModelPanel->SetTeam( iTeam, true, false );
			m_pClassModelPanel->SetModelTintColor( pLocalPlayer->m_vecPlayerColor );
			m_pClassModelPanel->LoadItems();
			
		}
		else
		{
			if ( m_pClassModelPanel->IsVisible() )
			{
				m_pClassModelPanel->SetVisible( false );
			}
			
			if ( !m_pClassImage->IsVisible() )
			{
				m_pClassImage->SetVisible( true );
			}
			
			m_pClassImage->SetClass( iTeam, iClass, 0 );
		}
	} 
	else
	{
		m_pClassImage->SetVisible( false );
		m_pClassModelPanel->SetVisible( false );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::UpdateArenaWaitingToPlayList( void )
{
#if SOONSOON
	// TODO: ARENA
	//if ( !g_TF_PR || !TFGameRules()->IsInDuelMode() )
	if( !g_TF_PR || !(TFGameRules()->GetRetroModeType() == TF_GAMESUBTYPE_DUEL) )
	{
		SetDialogVariable( "waitingtoplay", "" );
		return;
	}
#else
	if( !g_TF_PR )
	{
		SetDialogVariable( "waitingtoplay", "" );
		return;
	}
#endif

	// Spectators
	char szSpectatorList[512] = "";
	int nSpectators = 0;

	for ( int i = 1; i <= MAX_PLAYERS; i++ )
	{
		if ( !g_TF_PR->IsConnected( i ) )
			continue;

		// TODO: ARENA
		if ( g_TF_PR->GetTeam( i ) == TEAM_SPECTATOR /*&& !g_TF_PR->IsArenaSpectator( i )*/ )
		{
			if ( nSpectators > 0 )
			{
				V_strcat_safe( szSpectatorList, ", " );
			}

			V_strcat_safe( szSpectatorList, g_TF_PR->GetPlayerName( i ) );
			nSpectators++;
		}
	}

	wchar_t wzSpectators[512] = L"";
	if ( nSpectators > 0 )
	{
		const char *pchFormat = ( 1 == nSpectators ? "#TF_Arena_ScoreBoard_Spectator" : "#TF_Arena_ScoreBoard_Spectators" );

		wchar_t wzSpectatorCount[16];
		wchar_t wzSpectatorList[1024];
		V_swprintf_safe( wzSpectatorCount, L"%i", nSpectators );
		g_pVGuiLocalize->ConvertANSIToUnicode( szSpectatorList, wzSpectatorList, sizeof( wzSpectatorList ) );
		g_pVGuiLocalize->ConstructString( wzSpectators, sizeof( wzSpectators ), g_pVGuiLocalize->Find( pchFormat ), 2, wzSpectatorCount, wzSpectatorList );
	}
	SetDialogVariable( "waitingtoplay", wzSpectators );
}

//-----------------------------------------------------------------------------
// Purpose: Used for sorting players
//-----------------------------------------------------------------------------
bool CTFClientScoreBoardDialog::TFPlayerSortFunc( vgui::SectionedListPanel *list, int itemID1, int itemID2 )
{
	KeyValues *it1 = list->GetItemData( itemID1 );
	KeyValues *it2 = list->GetItemData( itemID2 );
	Assert( it1 && it2 );

	// first compare score
	int v1 = it1->GetInt( "score" );
	int v2 = it2->GetInt( "score" );
	if ( v1 > v2 )
		return true;
	else if ( v1 < v2 )
		return false;

	// if score is the same, use player index to get deterministic sort
	int iPlayerIndex1 = it1->GetInt( "playerIndex" );
	int iPlayerIndex2 = it2->GetInt( "playerIndex" );
	return ( iPlayerIndex1 > iPlayerIndex2 );
}

//-----------------------------------------------------------------------------
// Purpose: Event handler
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::FireGameEvent( IGameEvent *event )
{
	const char *type = event->GetName();

	if ( FStrEq( type, "server_spawn" ) )
	{
		// set server name in scoreboard
		const char *hostname = event->GetString( "hostname" );
		wchar_t wzHostName[256];
		wchar_t wzServerLabel[256];
		g_pVGuiLocalize->ConvertANSIToUnicode( hostname, wzHostName, sizeof( wzHostName ) );
		g_pVGuiLocalize->ConstructString( wzServerLabel, sizeof( wzServerLabel ), g_pVGuiLocalize->Find( "#Scoreboard_Server" ), 1, wzHostName );
		SetDialogVariable( "server", wzServerLabel );

		// Set the level name after the server spawn
		SetDialogVariable( "mapname", GetMapDisplayName( event->GetString( "mapname" ) ) );
	}

	if ( IsVisible() )
	{
		Update();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
SectionedListPanel *CTFClientScoreBoardDialog::GetSelectedPlayerList( void )
{
	if( m_iTeamCount <= 2 )
		return NULL;
	
	for ( int i = 0; i < m_iTeamCount; i++ )
	{
		SectionedListPanel *pList = m_pPlayerLists[i];
		if ( pList->GetSelectedItem() >= 0 )
		{
			return pList;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Center the dialog on the screen.  (vgui has this method on
//			Frame, but we're an EditablePanel, need to roll our own.)
//-----------------------------------------------------------------------------
void CTFClientScoreBoardDialog::MoveToCenterOfScreen()
{
	int wx, wy, ww, wt;
	surface()->GetWorkspaceBounds( wx, wy, ww, wt );
	SetPos( ( ww - GetWide() ) / 2, ( wt - GetTall() ) / 2 );
}
