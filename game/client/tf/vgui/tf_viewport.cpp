//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Client DLL VGUI2 Viewport
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "input.h"
#include "spectatorgui.h"
#include "tf_shareddefs.h"
#include "tf_viewport.h"
#include "c_tf_player.h"
#include "tf_gamerules.h"
#include "voice_status.h"
#include "tf_clientscoreboard.h"
#include "tf_mapinfomenu.h"
#include "tf_teammenu.h"
#include "tf_classmenu.h"
#include "intromenu.h"
#include "tf_intromenu.h"
#include "tf_hud_notification_panel.h"
#include "tf_fourteamscoreboard.h"
#include "tf_deathmatchscoreboard.h"
#include "tf_spectatorgui.h"
#include "tf_roundinfo.h"
#include "tf_textwindow.h"

#if !defined( ANDROID )
ConVar cl_tf2c_devkey( "cl_tf2c_devkey", "", FCVAR_HIDDEN | FCVAR_USERINFO | FCVAR_ARCHIVE | FCVAR_DONTRECORD );
#endif

#define ENABLE_NEW_CLASSMENU 0

/*
CON_COMMAND( spec_help, "Show spectator help screen")
{
	if ( gViewPortInterface )
		gViewPortInterface->ShowPanel( PANEL_INFO, true );
}

CON_COMMAND( spec_menu, "Activates spectator menu")
{
	bool bShowIt = true;

	C_CSPlayer *pPlayer = C_CSPlayer::GetLocalCSPlayer();

	if ( pPlayer && !pPlayer->IsObserver() )
		return;

	if ( args.ArgC() == 2 )
	{
		bShowIt = atoi( args[ 1 ] ) == 1;
	}

	if ( gViewPortInterface )
		gViewPortInterface->ShowPanel( PANEL_SPECMENU, bShowIt );
}
*/

CON_COMMAND( showmapinfo, "Show map info panel" )
{
	if ( !gViewPortInterface )
		return;

	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();

	// don't let the player open the team menu themselves until they're a spectator or they're on a regular team and have picked a class
	if ( pPlayer )
	{
		if ( ( pPlayer->GetTeamNumber() == TEAM_SPECTATOR ) || 
		     ( ( pPlayer->GetTeamNumber() != TEAM_UNASSIGNED ) && ( pPlayer->GetPlayerClass()->GetClassIndex() != TF_CLASS_UNDEFINED ) ) )
		{
			// close all the other panels that could be open
			gViewPortInterface->ShowPanel( PANEL_TEAM, false );
#if ENABLE_NEW_CLASSMENU
			gViewPortInterface->ShowPanel( PANEL_CLASS, false );
#else
			gViewPortInterface->ShowPanel( PANEL_CLASS_RED, false );
			gViewPortInterface->ShowPanel( PANEL_CLASS_BLUE, false );
			gViewPortInterface->ShowPanel( PANEL_CLASS_GREEN, false );
			gViewPortInterface->ShowPanel( PANEL_CLASS_YELLOW, false );

#endif
			gViewPortInterface->ShowPanel( PANEL_INTRO, false );
			gViewPortInterface->ShowPanel( PANEL_ROUNDINFO, false );
			gViewPortInterface->ShowPanel( PANEL_FOURTEAMSELECT, false );
			gViewPortInterface->ShowPanel( PANEL_DEATHMATCHTEAMSELECT, false );

			gViewPortInterface->ShowPanel( PANEL_MAPINFO, true );
		}
	}
}

TFViewport::TFViewport()
{
}

//-----------------------------------------------------------------------------
// Purpose: called when the VGUI subsystem starts up
//			Creates the sub panels and initialises them
//-----------------------------------------------------------------------------
void TFViewport::Start( IGameUIFuncs *pGameUIFuncs, IGameEventManager2 * pGameEventManager )
{
	BaseClass::Start( pGameUIFuncs, pGameEventManager );
}

void TFViewport::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	gHUD.InitColors( pScheme );

	SetPaintBackgroundEnabled( false );

	// Precache some font characters for the 360
 	if ( IsX360() )
 	{
 		wchar_t *pAllChars = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.!:";
 		wchar_t *pNumbers = L"0123456789";
 
 		vgui::surface()->PrecacheFontCharacters( pScheme->GetFont( "ScoreboardTeamName" ), pAllChars );
 		vgui::surface()->PrecacheFontCharacters( pScheme->GetFont( "ScoreboardMedium" ), pAllChars );
 		vgui::surface()->PrecacheFontCharacters( pScheme->GetFont( "ScoreboardSmall" ), pAllChars );
 		vgui::surface()->PrecacheFontCharacters( pScheme->GetFont( "ScoreboardVerySmall" ), pAllChars );
 		vgui::surface()->PrecacheFontCharacters( pScheme->GetFont( "TFFontMedium" ), pAllChars );
 		vgui::surface()->PrecacheFontCharacters( pScheme->GetFont( "TFFontSmall" ), pAllChars );
 		vgui::surface()->PrecacheFontCharacters( pScheme->GetFont( "HudFontMedium" ), pAllChars );
 		vgui::surface()->PrecacheFontCharacters( pScheme->GetFont( "HudFontMediumSmallSecondary" ), pAllChars );
 		vgui::surface()->PrecacheFontCharacters( pScheme->GetFont( "HudFontSmall" ), pAllChars );
 		vgui::surface()->PrecacheFontCharacters( pScheme->GetFont( "DefaultSmall" ), pAllChars );
 
 		vgui::surface()->PrecacheFontCharacters( pScheme->GetFont( "ScoreboardTeamScore" ), pNumbers );
 	}
}

//
// This is the main function of the viewport. Right here is where we create our class menu, 
// team menu, and anything else that we want to turn on and off in the UI.
//
IViewPortPanel* TFViewport::CreatePanelByName( const char *szPanelName )
{
	IViewPortPanel* newpanel = NULL;

	// overwrite MOD specific panel creation
	if ( Q_strcmp( PANEL_SCOREBOARD, szPanelName ) == 0 )
	{
		newpanel = new CTFClientScoreBoardDialog( this, szPanelName, 4 );
	}
	else if( Q_strcmp( PANEL_SPECGUI, szPanelName ) == 0 )
	{
		newpanel = new CTFSpectatorGUI( this );
	}
	else if( Q_strcmp( PANEL_SPECMENU, szPanelName ) == 0 )
	{
		// FIXME: Should it be empty?
	}
	else if( Q_strcmp( PANEL_OVERVIEW, szPanelName ) == 0 )
	{
		// FIXME: Should it be empty?
	}
	else if( Q_strcmp( PANEL_INFO, szPanelName ) == 0 )
	{
		newpanel = new CTFTextWindow( this );
	}
	else if ( Q_strcmp( PANEL_MAPINFO, szPanelName ) == 0 )
	{
		newpanel = new CTFMapInfoMenu( this );
	}
	else if( Q_strcmp( PANEL_ROUNDINFO, szPanelName ) == 0 )
	{
		newpanel = new CTFRoundInfo( this );
	}
	else if ( Q_strcmp( PANEL_TEAM, szPanelName ) == 0 )
	{
		CTFTeamMenu *pTeamMenu = new CTFTeamMenu( this, szPanelName );	
		pTeamMenu->CreateTeamButtons();
		newpanel = pTeamMenu;
	}
	//else if( Q_strcmp( PANEL_TEAM_ARENA, szPanelName ) == 0 ) // CTFArenaTeamMenu
	//{
	//}
	else if ( Q_strcmp( PANEL_FOURTEAMSELECT, szPanelName ) == 0 )
	{
		CTFTeamMenu *pTeamMenu = new CTFFourTeamMenu( this, szPanelName );
		pTeamMenu->CreateTeamButtons();
		newpanel = pTeamMenu;
	}
	else if ( Q_strcmp( PANEL_DEATHMATCHTEAMSELECT, szPanelName ) == 0 )
	{
		CTFTeamMenu *pTeamMenu = new CTFDeathmatchTeamMenu( this, szPanelName );
		pTeamMenu->CreateTeamButtons();
		newpanel = pTeamMenu;
	}
#if ENABLE_NEW_CLASSMENU
	else if ( Q_strcmp( PANEL_CLASS, szPanelName ) == 0 )
	{
		newpanel = new CTFClassMenu( this );	
	}
#else
	else if ( Q_strcmp( PANEL_CLASS_RED, szPanelName ) == 0 )
	{
		newpanel = new CTFClassMenu_Red( this );	
	}
	else if ( Q_strcmp( PANEL_CLASS_BLUE, szPanelName ) == 0 )
	{
		newpanel = new CTFClassMenu_Blue( this );	
	}
	else if ( Q_strcmp( PANEL_CLASS_GREEN, szPanelName ) == 0)
	{
		newpanel = new CTFClassMenu_Green(this);
	}
	else if ( Q_strcmp( PANEL_CLASS_YELLOW, szPanelName ) == 0)
	{
		newpanel = new CTFClassMenu_Yellow(this);
	}
#endif
	else if ( Q_strcmp( PANEL_INTRO, szPanelName ) == 0 )
	{
		newpanel = new CTFIntroMenu( this );
	}
	else if( Q_strcmp( PANEL_FOURTEAMSCOREBOARD, szPanelName ) == 0 )
	{
		newpanel = new CTFFourTeamScoreBoardDialog( this, szPanelName );
	}
	else if( Q_strcmp( PANEL_DEATHMATCHSCOREBOARD, szPanelName ) == 0 )
	{
		newpanel = new CTFDeathMatchScoreBoardDialog( this, szPanelName );
	}
	else
	{
		// create a generic base panel, don't add twice
		newpanel = BaseClass::CreatePanelByName( szPanelName );
	}

	return newpanel; 
}

void TFViewport::CreateDefaultPanels( void )
{
	AddNewPanel( CreatePanelByName( PANEL_MAPINFO ), "PANEL_MAPINFO" );
	AddNewPanel( CreatePanelByName( PANEL_TEAM ), "PANEL_TEAM" );
	// PANEL_ARENA_TEAM
#if ENABLE_NEW_CLASSMENU
	AddNewPanel( CreatePanelByName( PANEL_CLASS ), "PANEL_CLASS" );
#else
	AddNewPanel( CreatePanelByName( PANEL_CLASS_RED ), "PANEL_CLASS_RED" );
	AddNewPanel( CreatePanelByName( PANEL_CLASS_BLUE ), "PANEL_CLASS_BLUE" );
	AddNewPanel( CreatePanelByName( PANEL_CLASS_GREEN ), "PANEL_CLASS_GREEN" );
	AddNewPanel( CreatePanelByName( PANEL_CLASS_YELLOW ), "PANEL_CLASS_YELLOW" );
#endif
	AddNewPanel( CreatePanelByName( PANEL_INTRO ), "PANEL_INTRO" );
	AddNewPanel( CreatePanelByName( PANEL_ROUNDINFO ), "PANEL_ROUNDINFO" );
	
	AddNewPanel( CreatePanelByName( PANEL_FOURTEAMSELECT ), "PANEL_FOURTEAMSELECT" );
	AddNewPanel( CreatePanelByName( PANEL_DEATHMATCHTEAMSELECT ), "PANEL_DEATHMATCHTEAMSELECT" );
	AddNewPanel( CreatePanelByName( PANEL_FOURTEAMSCOREBOARD ), "PANEL_FOURTEAMSCOREBOARD" );
	AddNewPanel( CreatePanelByName( PANEL_DEATHMATCHSCOREBOARD ), "PANEL_DEATHMATCHSCOREBOARD" );
	
	BaseClass::CreateDefaultPanels();
}

int TFViewport::GetDeathMessageStartHeight( void )
{
	int y = YRES(2);
	
	if( g_pSpectatorGUI && g_pSpectatorGUI->IsVisible() )
	{
		return YRES(2) + g_pSpectatorGUI->GetTopBarHeight();
	}
	else if( TFGameRules() && TFGameRules()->IsFourTeamGame() /*&& (TFGameRules()->IsInKothMode())*/ ) // FIXME: TFGameRules()->IsInKothMode() || LOBYTE(g_pGameRules[123].__vftable)
	{
		return YRES( 30 );
	}
	
	return y;
}

//-----------------------------------------------------------------------------
// Purpose: Helper function for handling multiple scoreboard.
//-----------------------------------------------------------------------------
const char *TFViewport::GetModeSpecificScoreboardName( void )
{
#if SOONSOON
	if( TFGameRules() && TFGameRules()->IsFreeForAll() )
#else
	if( TFGameRules() && TFGameRules()->IsDeathmatch() )
#endif
		return PANEL_DEATHMATCHSCOREBOARD;
	else if( TFGameRules() && TFGameRules()->IsFourTeamGame() )
		return PANEL_FOURTEAMSCOREBOARD;

	return PANEL_SCOREBOARD;
}

//-----------------------------------------------------------------------------
// Purpose: Use this when you need to open the scoreboard unless you want a specific scoreboard panel.
//-----------------------------------------------------------------------------
void TFViewport::ShowScoreboard( bool bState, int nPollHideCode /*= BUTTON_CODE_INVALID*/ )
{
	const char *pszScoreboard = GetModeSpecificScoreboardName();
	ShowPanel( pszScoreboard, bState );

	if ( bState && nPollHideCode != BUTTON_CODE_INVALID )
	{
		// The scoreboard was opened by another dialog so we need to send the close button code.
		// See CTFClientScoreBoardDialog::OnThink for the explanation.
		PostMessageToPanel( pszScoreboard, new KeyValues( "PollHideCode", "code", nPollHideCode ) );
	}
}

void TFViewport::CC_ScoresDown( const CCommand &args )
{
	if( !TFGameRules() )
		return;
	
	ShowPanel( GetModeSpecificScoreboardName(), true );
	GetClientVoiceMgr()->StopSquelchMode();
}

void TFViewport::CC_ScoresUp( const CCommand &args )
{
	if( !TFGameRules() )
		return;
		
	ShowPanel( GetModeSpecificScoreboardName(), false );
	GetClientVoiceMgr()->StopSquelchMode();
}

void TFViewport::CC_ToggleScores( const CCommand &args )
{
	if( !TFGameRules() )
		return;
		
	if ( !gViewPortInterface )
		return;
	
	IViewPortPanel *scoreboard = gViewPortInterface->FindPanelByName( GetModeSpecificScoreboardName() );
	
	if ( !scoreboard )
		return;
	
	if ( scoreboard->IsVisible() )
	{
		gViewPortInterface->ShowPanel( scoreboard, false );
		GetClientVoiceMgr()->StopSquelchMode();
	}
	else
	{
		gViewPortInterface->ShowPanel( scoreboard, true );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void TFViewport::ShowTeamMenu( bool bState )
{
	if ( !TFGameRules() )
		return;

#if SOONSOON
	if( TFGameRules()->IsFreeForAll() || TFGameRules()->GetRetroModeType() == TF_GAMESUBTYPE_INFECTION )
#else
	if( TFGameRules()->IsDeathmatch() )
#endif
	{
		ShowPanel( PANEL_DEATHMATCHTEAMSELECT, bState );
	}
	// else if arena && !arena queue
	else if( TFGameRules()->IsFourTeamGame() )
	{
		ShowPanel( PANEL_FOURTEAMSELECT, bState );
	}
	else
	{
		ShowPanel( PANEL_TEAM, bState );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void TFViewport::CC_ChangeTeam( const CCommand &args )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer )
		return;

	if ( engine->IsHLTV() )
		return;

	// don't let the player open the team menu themselves until they're on a team
	if ( pPlayer->GetTeamNumber() == TEAM_UNASSIGNED )
		return;

	// Losers can't change team during bonus time.
	if ( TFGameRules()->State_Get() == GR_STATE_TEAM_WIN &&
		pPlayer->GetTeamNumber() >= FIRST_GAME_TEAM &&
		pPlayer->GetTeamNumber() != TFGameRules()->GetWinningTeam() )
		return;

	ShowTeamMenu( true );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void TFViewport::ShowClassMenu( bool bState )
{
#if ENABLE_NEW_CLASSMENU
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( bState && ( pPlayer && pPlayer->CanShowClassMenu() ) )
	{
		// Need to set the class menu to the proper team when showing it.
		PostMessageToPanel( PANEL_CLASS, new KeyValues( "ShowToTeam", "iTeam", pPlayer->GetTeamNumber() ) );
	}
	else
	{
		ShowPanel( PANEL_CLASS, false );
	}
#else
	// SanyaSho: get rid of this when we've rewrite CTFClassMenu
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	
	if ( pPlayer && pPlayer->CanShowClassMenu() )
	{
		switch( pPlayer->GetTeamNumber() )
		{
			case TF_TEAM_RED:
				gViewPortInterface->ShowPanel( PANEL_CLASS_RED, bState );
				break;
			case TF_TEAM_BLUE:
				gViewPortInterface->ShowPanel( PANEL_CLASS_BLUE, bState );
				break;
			case TF_TEAM_GREEN:
				gViewPortInterface->ShowPanel( PANEL_CLASS_GREEN, bState );
				break;
			case TF_TEAM_YELLOW:
				gViewPortInterface->ShowPanel( PANEL_CLASS_YELLOW, bState );
				break;
			default:
				break;
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void TFViewport::CC_ChangeClass( const CCommand &args )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer )
		return;

	if ( engine->IsHLTV() )
		return;

	// TODO: ARENA
	if ( pPlayer->CanShowClassMenu() )
	{
		ShowClassMenu( true );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void TFViewport::OnScreenSizeChanged( int iOldWide, int iOldTall )
{
	BaseClass::OnScreenSizeChanged( iOldWide, iOldTall );

	// we've changed resolution, let's try to figure out if we need to show any of our menus
	if ( !gViewPortInterface )
		return;

	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( pPlayer )
	{
		// are we on a team yet?
		if ( pPlayer->GetTeamNumber() == TEAM_UNASSIGNED )
		{
			engine->ClientCmd( "show_motd" );
		}
		else if ( ( pPlayer->GetTeamNumber() != TEAM_SPECTATOR ) && ( pPlayer->m_Shared.GetDesiredPlayerClassIndex() == TF_CLASS_UNDEFINED ) )
		{
			ShowClassMenu( true );
		}
	}
}
