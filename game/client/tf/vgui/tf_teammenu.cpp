//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "cbase.h"
#include "tf_teammenu.h"
#include "iclientmode.h"
#include "basemodelpanel.h"
#include <vgui_controls/AnimationController.h>
#include "IGameUIFuncs.h" // for key bindings
#include "tf_gamerules.h"
#include "c_team.h"
#include "tf_viewport.h"

using namespace vgui;

DECLARE_BUILD_FACTORY( CTFTeamButton );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFTeamButton::CTFTeamButton( vgui::Panel *parent, const char *panelName ) : CExButton( parent, panelName, "" )
{
	m_szModelPanel[0] = '\0';
	m_iTeam = TEAM_UNASSIGNED;
	m_flHoverTimeToWait = -1;
	m_flHoverTime = -1;
	m_bMouseEntered = false;
	m_bTeamDisabled = false;

	vgui::ivgui()->AddTickSignal( GetVPanel() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamButton::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );

	V_strcpy_safe( m_szModelPanel, inResourceData->GetString( "associated_model", "" ) );
	m_iTeam = inResourceData->GetInt( "team", TEAM_UNASSIGNED );
	m_flHoverTimeToWait = inResourceData->GetFloat( "hover", -1 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamButton::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetDefaultColor( GetFgColor(), Color( 0, 0, 0, 0 ) );
	SetArmedColor( GetButtonFgColor(), Color( 0, 0, 0, 0 ) );
	SetDepressedColor( GetButtonFgColor(), Color( 0, 0, 0, 0 ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamButton::SendAnimation( const char *pszAnimation )
{
	if ( !m_szModelPanel[0] )
		return;

	Panel *pParent = GetParent();
	if ( pParent )
	{
		CModelPanel *pModel = dynamic_cast<CModelPanel*>( pParent->FindChildByName( m_szModelPanel ) );
		if ( pModel )
		{
			KeyValues *kvParms = new KeyValues( "SetAnimation" );
			if ( kvParms )
			{
				kvParms->SetString( "animation", pszAnimation );
				PostMessage( pModel, kvParms );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamButton::SetDefaultAnimation( const char *pszName )
{
	if ( !m_szModelPanel[0] )
		return;

	Panel *pParent = GetParent();
	if ( pParent )
	{
		CModelPanel *pModel = dynamic_cast<CModelPanel*>( pParent->FindChildByName( m_szModelPanel ) );
		if ( pModel )
		{
			pModel->SetDefaultAnimation( pszName );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTFTeamButton::IsTeamFull()
{
	return ( m_iTeam > TEAM_UNASSIGNED &&
		TFGameRules() &&
		TFGameRules()->WouldChangeUnbalanceTeams( m_iTeam, GetLocalPlayerTeam() ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamButton::OnCursorEntered()
{
	BaseClass::OnCursorEntered();

	SetMouseEnteredState( true );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamButton::OnCursorExited()
{
	BaseClass::OnCursorExited();

	SetMouseEnteredState( false );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamButton::SetMouseEnteredState( bool state )
{
	if ( state )
	{
		m_bMouseEntered = true;

		if ( m_flHoverTimeToWait > 0 )
		{
			m_flHoverTime = gpGlobals->curtime + m_flHoverTimeToWait;
		}
		else
		{
			m_flHoverTime = -1;
		}

		if ( m_bTeamDisabled )
		{
			SendAnimation( "enter_disabled" );
		}
		else
		{
			SendAnimation( "enter_enabled" );
		}
	}
	else
	{
		m_bMouseEntered = false;
		m_flHoverTime = -1;

		if ( m_bTeamDisabled )
		{
			SendAnimation( "exit_disabled" );
		}
		else
		{
			SendAnimation( "exit_enabled" );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamButton::OnTick()
{
	// check to see if our state has changed
	bool bDisabled = IsTeamFull();

	if ( bDisabled != m_bTeamDisabled )
	{
		m_bTeamDisabled = bDisabled;

		if ( m_bMouseEntered )
		{
			// something has changed, so reset our state
			SetMouseEnteredState( true );
		}
		else
		{
			// the mouse isn't currently over the button, but we should update the status
			if ( m_bTeamDisabled )
			{
				SendAnimation( "idle_disabled" );
			}
			else
			{
				SendAnimation( "idle_enabled" );
			}
		}
	}

	if ( ( m_flHoverTime > 0 ) && ( m_flHoverTime < gpGlobals->curtime ) )
	{
		m_flHoverTime = -1;

		if ( m_bTeamDisabled )
		{
			SendAnimation( "hover_disabled" );
		}
		else
		{
			SendAnimation( "hover_enabled" );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamButton::FireActionSignal( void )
{
	// Don't fire the command if the button is disabled.
	if ( m_bTeamDisabled )
		return;

	BaseClass::FireActionSignal();
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFTeamMenu::CTFTeamMenu( IViewPort *pViewPort, const char *pName ) : BaseClass( NULL, pName )
{
	m_iScoreBoardKey = BUTTON_CODE_INVALID; // this is looked up in Activate()
	m_iTeamMenuKey = BUTTON_CODE_INVALID;

	// initialize dialog
	SetTitle( "", true );

	// load the new scheme early!!
	SetScheme( "ClientScheme" );
	SetMoveable( false );
	SetSizeable( false );

	// hide the system buttons
	SetTitleBarVisible( false );
	SetProportional( true );

	SetMinimizeButtonVisible( false );
	SetMaximizeButtonVisible( false );
	SetCloseButtonVisible( false );
	SetVisible( false );
	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );

	memset( m_pTeamButtons, 0, sizeof( m_pTeamButtons ) );

	m_pSpecLabel = new CExLabel( this, "TeamMenuSpectate", "" );

	m_pHighlanderLabel = new CExLabel( this, "HighlanderLabel", "" );
	m_pHighlanderLabelShadow = new CExLabel( this, "HighlanderLabelShadow", "" );
	m_pTeamFullLabel = new CExLabel( this, "TeamsFullLabel", "" );
	m_pTeamFullLabelShadow = new CExLabel( this, "TeamsFullLabelShadow", "" );

	m_pTeamsFullArrow = new CTFImagePanel( this, "TeamsFullArrow" );

#ifdef _X360
	m_pFooter = new CTFFooter( this, "Footer" );
#else
	m_pCancelButton = new CExButton( this, "CancelButton", "#TF_Cancel" );
#endif
	
	vgui::ivgui()->AddTickSignal( GetVPanel() );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFTeamMenu::~CTFTeamMenu()
{
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
void CTFTeamMenu::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( GetResFilename() );

	SetHighlanderTeamsFullPanels( false );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamMenu::CreateTeamButtons( void )
{
	// Using Unassigned for Auto Team button.
	m_pTeamButtons[TEAM_UNASSIGNED] = new CTFTeamButton( this, "teambutton2" );
	m_pTeamButtons[TEAM_SPECTATOR] = new CTFTeamButton( this, "teambutton3" );
	m_pTeamButtons[TF_TEAM_RED] = new CTFTeamButton( this, "teambutton0" );
	m_pTeamButtons[TF_TEAM_BLUE] = new CTFTeamButton( this, "teambutton1" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamMenu::ShowPanel( bool bShow )
{
	if ( bShow == IsVisible() )
		return;

	if ( !gameuifuncs || !gViewPortInterface || !engine )
		return;

	if ( bShow )
	{
		GetTFViewPort()->ShowClassMenu( false );
		engine->CheckPoint( "TeamMenu" );

		Activate();
		SetMouseInputEnabled( true );

		// get key bindings if shown
		m_iTeamMenuKey = gameuifuncs->GetButtonCodeForBind( "changeteam" );
		m_iScoreBoardKey = gameuifuncs->GetButtonCodeForBind( "showscores" );

		// Highlight the button matching the player's team.
		int iTeam = GetLocalPlayerTeam();

		if ( iTeam < FIRST_GAME_TEAM )
		{
			iTeam = TEAM_UNASSIGNED;
		}

		if ( m_pTeamButtons[iTeam] )
		{
			if ( IsConsole() )
			{
				m_pTeamButtons[iTeam]->OnCursorEntered();
				m_pTeamButtons[iTeam]->SetDefaultAnimation( "enter_enabled" );
			}
			GetFocusNavGroup().SetCurrentFocus( m_pTeamButtons[iTeam]->GetVPanel(), m_pTeamButtons[iTeam]->GetVPanel() );
		}
	}
	else
	{
		SetHighlanderTeamsFullPanels( false );

		SetVisible( false );
		SetMouseInputEnabled( false );

		if ( IsConsole() )
		{
			// Close the door behind us
			CTFTeamButton *pButton = dynamic_cast<CTFTeamButton *> ( GetFocusNavGroup().GetCurrentFocus() );
			if ( pButton )
			{
				pButton->OnCursorExited();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamMenu::SetHighlanderTeamsFullPanels( bool bEnabled )
{
	bool bTeamsDisabled = true;

	for ( int i = FIRST_GAME_TEAM; i < GetNumberOfTeams(); i++ )
	{
		C_Team *pTeam = GetGlobalTeam( i );
		if ( pTeam && pTeam->Get_Number_Players() < 9 )
		{
			bTeamsDisabled = false;
			break;
		}
	}

	m_pHighlanderLabel->SetVisible( bEnabled );
	m_pHighlanderLabelShadow->SetVisible( bEnabled );

	m_pTeamFullLabel->SetVisible( bTeamsDisabled && bEnabled );
	m_pTeamFullLabelShadow->SetVisible( bTeamsDisabled && bEnabled );

	ConVarRef mp_allowspectators( "mp_allowspectators" );
	if ( bEnabled && mp_allowspectators.IsValid() && mp_allowspectators.GetBool() && bTeamsDisabled )
	{
		m_pTeamsFullArrow->SetVisible( true );
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( m_pTeamsFullArrow, "TeamsFullArrowAnimate" );
	}
	else
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( m_pTeamsFullArrow, "TeamsFullArrowAnimateEnd" );
		m_pTeamsFullArrow->SetVisible( false );
	}
}

//-----------------------------------------------------------------------------
// Purpose: called to update the menu with new information
//-----------------------------------------------------------------------------
void CTFTeamMenu::Update( void )
{
	if ( GetLocalPlayerTeam() != TEAM_UNASSIGNED )
	{
#ifdef _X360
		m_pFooter->ShowButtonLabel( "cancel", true );
#else
		m_pCancelButton->SetVisible( true );
#endif
	}
	else
	{
#ifdef _X360
		m_pFooter->ShowButtonLabel( "cancel", false );
#else
		m_pCancelButton->SetVisible( false );
#endif
	}
}

#ifdef _X360
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamMenu::Join_Team( const CCommand &args )
{
	if ( args.ArgC() > 1 )
	{
		char cmd[256];
		V_sprintf_safe( cmd, "jointeam_nomenus %s", args.Arg( 1 ) );
		OnCommand( cmd );
	}
}
#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamMenu::OnKeyCodePressed( KeyCode code )
{
	if ( ( m_iTeamMenuKey != BUTTON_CODE_INVALID && m_iTeamMenuKey == code ) ||
		code == KEY_XBUTTON_BACK ||
		code == KEY_XBUTTON_B )
	{
		if ( GetLocalPlayerTeam() != TEAM_UNASSIGNED )
		{
			ShowPanel( false );
		}
	}
	else if ( code == KEY_SPACE )
	{
		engine->ClientCmd( "jointeam auto" );
		Close();
	}
	else if ( code == KEY_XBUTTON_A || code == KEY_XBUTTON_RTRIGGER )
	{
		// select the active focus
		if ( GetFocusNavGroup().GetCurrentFocus() )
		{
			ipanel()->SendMessage( GetFocusNavGroup().GetCurrentFocus()->GetVPanel(), new KeyValues( "PressButton" ), GetVPanel() );
		}
	}
	else if ( code == KEY_XBUTTON_RIGHT || code == KEY_XSTICK1_RIGHT )
	{
		CTFTeamButton *pButton;

		pButton = dynamic_cast<CTFTeamButton *> ( GetFocusNavGroup().GetCurrentFocus() );
		if ( pButton )
		{
			pButton->OnCursorExited();
			GetFocusNavGroup().RequestFocusNext( pButton->GetVPanel() );
		}
		else
		{
			GetFocusNavGroup().RequestFocusNext( NULL );
		}

		pButton = dynamic_cast<CTFTeamButton *> ( GetFocusNavGroup().GetCurrentFocus() );
		if ( pButton )
		{
			pButton->OnCursorEntered();
		}
	}
	else if ( code == KEY_XBUTTON_LEFT || code == KEY_XSTICK1_LEFT )
	{
		CTFTeamButton *pButton;

		pButton = dynamic_cast<CTFTeamButton *> ( GetFocusNavGroup().GetCurrentFocus() );
		if ( pButton )
		{
			pButton->OnCursorExited();
			GetFocusNavGroup().RequestFocusPrev( pButton->GetVPanel() );
		}
		else
		{
			GetFocusNavGroup().RequestFocusPrev( NULL );
		}

		pButton = dynamic_cast<CTFTeamButton *> ( GetFocusNavGroup().GetCurrentFocus() );
		if ( pButton )
		{
			pButton->OnCursorEntered();
		}
	}
	else if ( m_iScoreBoardKey != BUTTON_CODE_INVALID && m_iScoreBoardKey == code )
	{
		GetTFViewPort()->ShowScoreboard( true, code );
	}
	else
	{
		BaseClass::OnKeyCodePressed( code );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Called when the user picks a team
//-----------------------------------------------------------------------------
void CTFTeamMenu::OnCommand( const char *command )
{
	if ( Q_strstr( command, "jointeam " ) || Q_strstr( command, "jointeam_nomenus " ) )
	{
		engine->ClientCmd( command );
	}

	Close();
}

extern const char *g_aTeamLowerNames[TF_TEAM_COUNT];

//-----------------------------------------------------------------------------
// Frame-based update
//-----------------------------------------------------------------------------
void CTFTeamMenu::OnTick()
{
	if ( !IsVisible() || !TFGameRules() )
		return;

	// update the number of players on each team
	for ( int i = FIRST_GAME_TEAM; i < GetNumberOfTeams(); i++ )
	{
		C_Team *pTeam = GetGlobalTeam( i );
		if ( !pTeam )
			continue;

		SetDialogVariable( VarArgs( "%scount", g_aTeamLowerNames[i] ), pTeam->Get_Number_Players() );
	}

	if ( m_pTeamButtons[TEAM_SPECTATOR] )
	{
		ConVarRef mp_allowspectators( "mp_allowspectators" );
		if ( mp_allowspectators.IsValid() )
		{
			if ( mp_allowspectators.GetBool() )
			{
				m_pTeamButtons[TEAM_SPECTATOR]->SetVisible( true );
				m_pSpecLabel->SetVisible( true );
			}
			else
			{
				m_pTeamButtons[TEAM_SPECTATOR]->SetVisible( false );
				m_pSpecLabel->SetVisible( false );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFTeamMenu::OnThink( void )
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( pPlayer )
	{
		pPlayer->m_Local.m_iHideHUD |= HIDEHUD_HEALTH;
	}

	BaseClass::OnThink();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFTeamMenu::OnClose()
{
	ShowPanel( false );

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( pPlayer )
	{
		pPlayer->m_Local.m_iHideHUD &= ~HIDEHUD_HEALTH;
	}

	BaseClass::OnClose();
}

//-----------------------------------------------------------------------------
// Purpose: Four team menu
//-----------------------------------------------------------------------------
CTFFourTeamMenu::CTFFourTeamMenu( IViewPort *pViewPort, const char *pName ) : BaseClass( pViewPort, pName )
{
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFFourTeamMenu::CreateTeamButtons( void )
{
	m_pTeamButtons[TEAM_UNASSIGNED] = new CTFTeamButton( this, "teambutton4" );
	m_pTeamButtons[TEAM_SPECTATOR] = new CTFTeamButton( this, "teambutton5" );
	m_pTeamButtons[TF_TEAM_RED] = new CTFTeamButton( this, "teambutton0" );
	m_pTeamButtons[TF_TEAM_BLUE] = new CTFTeamButton( this, "teambutton1" );
	m_pTeamButtons[TF_TEAM_GREEN] = new CTFTeamButton( this, "teambutton2" );
	m_pTeamButtons[TF_TEAM_YELLOW] = new CTFTeamButton( this, "teambutton3" );
}

//-----------------------------------------------------------------------------
// Purpose: FFA team menu
//-----------------------------------------------------------------------------
CTFDeathmatchTeamMenu::CTFDeathmatchTeamMenu( IViewPort *pViewPort, const char *pName ) : BaseClass( pViewPort, pName )
{
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFDeathmatchTeamMenu::CreateTeamButtons( void )
{
	m_pTeamButtons[TEAM_UNASSIGNED] = new CTFTeamButton( this, "teambutton2" );
	m_pTeamButtons[TEAM_SPECTATOR] = new CTFTeamButton( this, "teambutton3" );
}
