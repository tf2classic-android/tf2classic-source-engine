//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "tf_hud_winpanel.h"
#include "tf_hud_statpanel.h"
#include "vgui_controls/AnimationController.h"
#include "iclientmode.h"
#include "engine/IEngineSound.h"
#include "c_tf_playerresource.h"
#include "c_tf_team.h"
#include "tf_clientscoreboard.h"
#include <vgui_controls/Label.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include "vgui_avatarimage.h"
#include "fmtstr.h"
#include "tf_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar tf2c_domination_points_per_round;
extern ConVar mp_bonusroundtime;

vgui::IImage* GetDefaultAvatarImage( int iPlayerIndex );

DECLARE_HUDELEMENT_DEPTH( CTFWinPanel, 1 );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFWinPanel::CTFWinPanel( const char *pElementName ) : EditablePanel( NULL, "WinPanel" ), CHudElement( pElementName )
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );
	m_bShouldBeVisible = false;
	SetAlpha( 0 );

	// This is needed for custom colors.
	SetScheme( scheme()->LoadSchemeFromFile( "Resource/ClientScheme_tf2c.res", "ClientScheme_tf2c" ) );

	m_pBGPanel = new EditablePanel( this, "WinPanelBGBorder" );
	m_pTeamScorePanel = new EditablePanel( this, "TeamScoresPanel" );
	m_pBlueBG = new EditablePanel( m_pTeamScorePanel, "BlueScoreBG" );
	m_pRedBG = new EditablePanel( m_pTeamScorePanel, "RedScoreBG" );

	m_pBlackBorder = NULL;
	m_pBlueBorder = NULL;
	m_pRedBorder = NULL;
	m_pGreenBorder = NULL;
	m_pYellowBorder = NULL;

	m_flTimeUpdateTeamScore = 0.f;
	m_iBlueTeamScore = 0;
	m_iRedTeamScore = 0;
	m_iGreenTeamScore = 0;
	m_iYellowTeamScore = 0;
	m_iScoringTeam = 0;
	
	m_flTimeSwitchTeams = 0.f;
	m_bShowingGreenYellow = false;

	RegisterForRenderGroup( "mid" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFWinPanel::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFWinPanel::Reset()
{
	m_bShouldBeVisible = false;
	m_flTimeUpdateTeamScore = 0.0f;
	m_iScoringTeam = 0;
	m_flTimeSwitchTeams = 0.f;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFWinPanel::Init()
{
	// listen for events
	ListenForGameEvent( "teamplay_win_panel" );
	ListenForGameEvent( "teamplay_round_start" );
	ListenForGameEvent( "teamplay_game_over" );
	ListenForGameEvent( "tf_game_over" );

	m_bShouldBeVisible = false;

	CHudElement::Init();
}

void CTFWinPanel::SetVisible( bool state )
{
	if ( state == IsVisible() )
		return;

	if ( state )
	{
		HideLowerPriorityHudElementsInGroup( "mid" );
	}
	else
	{
		UnhideLowerPriorityHudElementsInGroup( "mid" );
	}

	BaseClass::SetVisible( state );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFWinPanel::FireGameEvent( IGameEvent * event )
{
	const char *pEventName = event->GetName();

	if ( Q_strcmp( "teamplay_round_start", pEventName ) == 0 )
	{
		m_bShouldBeVisible = false;
	}
	else if ( Q_strcmp( "teamplay_game_over", pEventName ) == 0 )
	{
		m_bShouldBeVisible = false;
	}
	else if ( Q_strcmp( "tf_game_over", pEventName ) == 0 )
	{
		m_bShouldBeVisible = false;
	}
	else if ( Q_strcmp( "teamplay_win_panel", pEventName ) == 0 )
	{
		if ( !g_PR )
			return;

		if (TFGameRules()->IsDeathmatch())
		{
			m_bShouldBeVisible = false;
			return;
		}

		int iWinningTeam = event->GetInt( "winning_team" );
		int iWinReason = event->GetInt( "winreason" );
		int iFlagCapLimit = event->GetInt( "flagcaplimit" );
		bool bRoundComplete = (bool) event->GetInt( "round_complete" );
		int iRoundsRemaining = event->GetInt( "rounds_remaining" );

		LoadControlSettings( "resource/UI/WinPanel.res" );		
		InvalidateLayout( false, true );

		SetDialogVariable( "WinningTeamLabel", "" );
		SetDialogVariable( "AdvancingTeamLabel", "" );
		SetDialogVariable( "WinReasonLabel", "" );
		SetDialogVariable( "DetailsLabel", "" );

		// set the appropriate background image and label text
		const char *pTeamLabel = NULL;
		const char *pTopPlayersLabel = NULL;
		const wchar_t *pLocalizedTeamName = NULL;
		// this is an area defense, but not a round win, if this was a successful defend until time limit but not a complete round
		bool bIsAreaDefense = ( ( WINREASON_DEFEND_UNTIL_TIME_LIMIT == iWinReason ) && !bRoundComplete );
		switch ( iWinningTeam )
		{
		case TF_TEAM_BLUE:
			m_pBGPanel->SetBorder( m_pBlueBorder );
			pTeamLabel = ( bRoundComplete ? "#Winpanel_BlueWins" : ( bIsAreaDefense ? "#Winpanel_BlueDefends" : "#Winpanel_BlueAdvances" ) );
			pTopPlayersLabel = "#Winpanel_BlueMVPs";
			pLocalizedTeamName =  g_pVGuiLocalize->Find( "TF_BlueTeam_Name" );
			break;
		case TF_TEAM_RED:
			m_pBGPanel->SetBorder( m_pRedBorder );
			pTeamLabel = ( bRoundComplete ? "#Winpanel_RedWins" : ( bIsAreaDefense ? "#Winpanel_RedDefends" : "#Winpanel_RedAdvances" ) );
			pTopPlayersLabel = "#Winpanel_RedMVPs";
			pLocalizedTeamName =  g_pVGuiLocalize->Find( "TF_RedTeam_Name" );
			break;
		case TF_TEAM_GREEN:
			m_pBGPanel->SetBorder( m_pGreenBorder );
			pTeamLabel = (bRoundComplete ? "#Winpanel_GreenWins" : (bIsAreaDefense ? "#Winpanel_GreenDefends" : "#Winpanel_GreenAdvances"));
			pTopPlayersLabel = "#Winpanel_GreenMVPs";
			pLocalizedTeamName = g_pVGuiLocalize->Find("TF_GreenTeam_Name");
			break;
		case TF_TEAM_YELLOW:
			m_pBGPanel->SetBorder( m_pYellowBorder );
			pTeamLabel = (bRoundComplete ? "#Winpanel_YellowWins" : (bIsAreaDefense ? "#Winpanel_YellowDefends" : "#Winpanel_YellowAdvances"));
			pTopPlayersLabel = "#Winpanel_YellowMVPs";
			pLocalizedTeamName = g_pVGuiLocalize->Find("TF_YellowTeam_Name");
			break;
		case TEAM_UNASSIGNED:	// stalemate
			m_pBGPanel->SetBorder( m_pBlackBorder );
			pTeamLabel = "#Winpanel_Stalemate";
			pTopPlayersLabel = "#Winpanel_TopPlayers";
			break;
		default:
			Assert( false );
			break;
		}

		SetDialogVariable( bRoundComplete ? "WinningTeamLabel" : "AdvancingTeamLabel", g_pVGuiLocalize->Find( pTeamLabel ) );
		SetDialogVariable( "TopPlayersLabel", g_pVGuiLocalize->Find( pTopPlayersLabel ) );

		wchar_t wzWinReason[512]=L"";
		switch ( iWinReason )
		{
		case WINREASON_ALL_POINTS_CAPTURED:
			g_pVGuiLocalize->ConstructString( wzWinReason, sizeof( wzWinReason ), g_pVGuiLocalize->Find( "#Winreason_AllPointsCaptured" ), 1, pLocalizedTeamName );
			break;
		case WINREASON_FLAG_CAPTURE_LIMIT:
			{
				wchar_t wzFlagCaptureLimit[16];
				_snwprintf( wzFlagCaptureLimit, ARRAYSIZE( wzFlagCaptureLimit), L"%i", iFlagCapLimit );
				g_pVGuiLocalize->ConstructString( wzWinReason, sizeof( wzWinReason ), g_pVGuiLocalize->Find( "#Winreason_FlagCaptureLimit" ), 2, 
					pLocalizedTeamName, wzFlagCaptureLimit );
			}			
			break;
		case WINREASON_OPPONENTS_DEAD:
			g_pVGuiLocalize->ConstructString( wzWinReason, sizeof( wzWinReason ), g_pVGuiLocalize->Find( "#Winreason_OpponentsDead" ), 1, pLocalizedTeamName );
			break;
		case WINREASON_DEFEND_UNTIL_TIME_LIMIT:
			g_pVGuiLocalize->ConstructString( wzWinReason, sizeof( wzWinReason ), g_pVGuiLocalize->Find( "#Winreason_DefendedUntilTimeLimit" ), 1, pLocalizedTeamName );
			break;
		case WINREASON_STALEMATE:
			g_pVGuiLocalize->ConstructString( wzWinReason, sizeof( wzWinReason ), g_pVGuiLocalize->Find( "#Winreason_Stalemate" ), 0 );
			break;
		case TF_WINREASON_VIPESCAPED:
			g_pVGuiLocalize->ConstructString( wzWinReason, sizeof( wzWinReason ), g_pVGuiLocalize->Find( "#Winreason_VIP_Escaped" ), 1, pLocalizedTeamName );
			break;
		case TF_WINREASON_DOMINATION:
			{
				wchar_t wsPoints[32] = L"";
				_snwprintf( wsPoints, ARRAYSIZE( wsPoints ), L"%d", tf2c_domination_points_per_round.GetInt() );
				g_pVGuiLocalize->ConstructString( wzWinReason, sizeof( wzWinReason ), g_pVGuiLocalize->Find( "#Winreason_RoundScoreLimit" ), 2, pLocalizedTeamName, wsPoints );
			}
			break;
		default:
			Assert( false );
			break;
		}
		SetDialogVariable( "WinReasonLabel", wzWinReason );

		if ( !bRoundComplete && ( WINREASON_STALEMATE != iWinReason ) )
		{			
			// if this was a mini-round, show # of capture points remaining
			wchar_t wzNumCapturesRemaining[16];
			wchar_t wzCapturesRemainingMsg[256]=L"";
			_snwprintf( wzNumCapturesRemaining, ARRAYSIZE( wzNumCapturesRemaining ), L"%i", iRoundsRemaining );
			g_pVGuiLocalize->ConstructString( wzCapturesRemainingMsg, sizeof( wzCapturesRemainingMsg ), 
				g_pVGuiLocalize->Find( 1 == iRoundsRemaining ? "#Winpanel_CapturePointRemaining" : "Winpanel_CapturePointsRemaining" ),
				1, wzNumCapturesRemaining );
			SetDialogVariable( "DetailsLabel", wzCapturesRemainingMsg );
		}
		else if ( ( WINREASON_ALL_POINTS_CAPTURED == iWinReason ) || ( WINREASON_FLAG_CAPTURE_LIMIT == iWinReason ) )
		{
			// if this was a full round that ended with point capture or flag capture, show the winning cappers
			const char *pCappers = event->GetString( "cappers" );
			int iCappers = Q_strlen( pCappers );
			if ( iCappers > 0 )
			{	
				char szPlayerNames[256]="";
				wchar_t wzPlayerNames[256]=L"";
				wchar_t wzCapMsg[512]=L"";
				for ( int i = 0; i < iCappers; i++ )
				{
					Q_strncat( szPlayerNames, g_PR->GetPlayerName( (int) pCappers[i] ), ARRAYSIZE( szPlayerNames ) );
					if ( i < iCappers - 1 )
					{
						Q_strncat( szPlayerNames, ", ", ARRAYSIZE( szPlayerNames ) );
					}
				}
				g_pVGuiLocalize->ConvertANSIToUnicode( szPlayerNames, wzPlayerNames, sizeof( wzPlayerNames ) );
				g_pVGuiLocalize->ConstructString( wzCapMsg, sizeof( wzCapMsg ), g_pVGuiLocalize->Find( "#Winpanel_WinningCapture" ), 1, wzPlayerNames );
				SetDialogVariable( "DetailsLabel", wzCapMsg );
			}
		}

		// get the current & previous team scores
		m_iBlueTeamScore = event->GetInt( "blue_score", 0 );
		m_iRedTeamScore = event->GetInt( "red_score", 0 );
		m_iGreenTeamScore = event->GetInt("green_score", 0);
		m_iYellowTeamScore = event->GetInt("yellow_score", 0);
		m_iScoringTeam = event->GetInt( "scoring_team", 0 );
		
		if( bRoundComplete )
		{
			SwitchScorePanels( m_iScoringTeam > 3, true );
			m_flTimeSwitchTeams = (float)(mp_bonusroundtime.GetFloat() * 0.5) + gpGlobals->curtime;
		}

		// look for the top 3 players sent in the event
		for ( int i = 1; i <= 3; i++ )
		{
			bool bShow = false;
			char szPlayerIndexVal[64]="", szPlayerScoreVal[64]="";
			// get player index and round points from the event
			Q_snprintf( szPlayerIndexVal, ARRAYSIZE( szPlayerIndexVal ), "player_%d", i );
			Q_snprintf( szPlayerScoreVal, ARRAYSIZE( szPlayerScoreVal ), "player_%d_points", i );
			int iPlayerIndex = event->GetInt( szPlayerIndexVal, 0 );
			int iRoundScore = event->GetInt( szPlayerScoreVal, 0 );
			// round score of 0 means no player to show for that position (not enough players, or didn't score any points that round)
			if ( iRoundScore > 0 )
				bShow = true;

#if !defined( _X360 )
			CAvatarImagePanel *pPlayerAvatar = dynamic_cast<CAvatarImagePanel *>( FindChildByName( CFmtStr( "Player%dAvatar", i ) ) );
			
			if ( pPlayerAvatar )
			{
				pPlayerAvatar->ClearAvatar();
				pPlayerAvatar->SetShouldScaleImage( true );
				pPlayerAvatar->SetShouldDrawFriendIcon( false );
				
				if ( bShow )
				{
					pPlayerAvatar->SetDefaultAvatar( GetDefaultAvatarImage( iPlayerIndex ) );
					pPlayerAvatar->SetPlayer( iPlayerIndex );
				}
				pPlayerAvatar->SetVisible( bShow );
			}
#endif
			vgui::Label *pPlayerName = dynamic_cast<Label *>( FindChildByName( CFmtStr( "Player%dName", i ) ) );
			vgui::Label *pPlayerClass = dynamic_cast<Label *>( FindChildByName( CFmtStr( "Player%dClass", i ) ) );
			vgui::Label *pPlayerScore = dynamic_cast<Label *>( FindChildByName( CFmtStr( "Player%dScore", i ) ) );
			
			if ( !pPlayerName || !pPlayerClass || !pPlayerScore )
				return;

			if ( bShow )
			{
				// set the player labels to team color
				Color clr = g_PR->GetTeamColor( g_PR->GetTeam( iPlayerIndex ) );				
				pPlayerName->SetFgColor( clr );
				pPlayerClass->SetFgColor( clr );
				pPlayerScore->SetFgColor( clr );

				// set label contents
				pPlayerName->SetText( g_PR->GetPlayerName( iPlayerIndex ) );
				pPlayerClass->SetText( g_aPlayerClassNames[ g_TF_PR ? g_TF_PR->GetPlayerClass( iPlayerIndex ) : TF_CLASS_SCOUT ] );
				pPlayerScore->SetText( CFmtStr( "%d", iRoundScore ) );
			}

			// show or hide labels for this player position
			pPlayerName->SetVisible( bShow );
			pPlayerClass->SetVisible( bShow );
			pPlayerScore->SetVisible( bShow );
		}

		m_bShouldBeVisible = true;

		MoveToFront();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Applies scheme settings
//-----------------------------------------------------------------------------
void CTFWinPanel::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/UI/WinPanel.res" );

	m_pBlackBorder = pScheme->GetBorder( "TFFatLineBorder" );
	m_pBlueBorder = pScheme->GetBorder( "TFFatLineBorderBlueBG" );
	m_pRedBorder = pScheme->GetBorder( "TFFatLineBorderRedBG" );
	m_pGreenBorder = pScheme->GetBorder( "TFFatLineBorderGreenBG" );
	m_pYellowBorder = pScheme->GetBorder( "TFFatLineBorderYellowBG" );
}

//-----------------------------------------------------------------------------
// Purpose: returns whether panel should be drawn
//-----------------------------------------------------------------------------
bool CTFWinPanel::ShouldDraw()
{
	if ( !m_bShouldBeVisible )
		return false;

	return CHudElement::ShouldDraw();
}

//-----------------------------------------------------------------------------
// Purpose: panel think method
//-----------------------------------------------------------------------------
void CTFWinPanel::OnThink()
{
	// if we've scheduled ourselves to update the team scores, handle it now
	if ( m_flTimeUpdateTeamScore > 0 && gpGlobals->curtime >= m_flTimeUpdateTeamScore )
	{
		// play a sound
		CLocalPlayerFilter filter;
		C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "Hud.EndRoundScored" );

		// update the team scores
		m_pTeamScorePanel->SetDialogVariable( "blueteamscore", GetTeamScore( GetLeftTeam(), false ) );
		m_pTeamScorePanel->SetDialogVariable( "redteamscore", GetTeamScore( GetRightTeam(), false ) );

		m_flTimeUpdateTeamScore = 0;
	}
	
	if( m_flTimeSwitchTeams > 0 && gpGlobals->curtime >= m_flTimeSwitchTeams )
	{
		SwitchScorePanels( !m_bShowingGreenYellow, true );
		m_flTimeSwitchTeams = 0;
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int CTFWinPanel::GetLeftTeam( void )
{
	return m_bShowingGreenYellow ? TF_TEAM_GREEN : TF_TEAM_BLUE;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int CTFWinPanel::GetRightTeam( void )
{
	return m_bShowingGreenYellow ? TF_TEAM_YELLOW : TF_TEAM_RED;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int CTFWinPanel::GetTeamScore( int iTeam, bool bPrevious )
{
	int iScore = 0;

	switch ( iTeam )
	{
	case TF_TEAM_RED:
		iScore = m_iRedTeamScore;
		break;
	case TF_TEAM_BLUE:
		iScore = m_iBlueTeamScore;
		break;
	case TF_TEAM_GREEN:
		iScore = m_iGreenTeamScore;
		break;
	case TF_TEAM_YELLOW:
		iScore = m_iYellowTeamScore;
		break;
	}

	// If this is the winning team then their previous score is 1 point lower.
	if ( bPrevious && iTeam == m_iScoringTeam )
		iScore--;

	return iScore;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFWinPanel::SwitchScorePanels( bool bShow, bool bSetScores )
{
	m_bShowingGreenYellow = bShow;

	if( bShow )
	{
		m_pBlueBG->SetBorder( m_pGreenBorder );
		m_pRedBG->SetBorder( m_pYellowBorder );
	}
	else
	{
		m_pBlueBG->SetBorder( m_pBlueBorder );
		m_pRedBG->SetBorder( m_pRedBorder );
	}
	
	if( bSetScores )
	{
		m_pTeamScorePanel->SetDialogVariable( "blueteamscore", GetTeamScore( GetLeftTeam(), false ) );
		m_pTeamScorePanel->SetDialogVariable( "redteamscore", GetTeamScore( GetRightTeam(), false ) );
		
		const wchar_t *wszLeftTeam = GetLocalizedTeamName( TF_TEAM_BLUE );
		const wchar_t *wszRightTeam = GetLocalizedTeamName( TF_TEAM_RED );
		
		if( m_bShowingGreenYellow )
		{
			wszLeftTeam = GetLocalizedTeamName( TF_TEAM_GREEN );
			wszRightTeam = GetLocalizedTeamName( TF_TEAM_YELLOW );
		}
		
		m_pTeamScorePanel->SetDialogVariable( "blueteamname", wszLeftTeam );
		m_pTeamScorePanel->SetDialogVariable( "redteamname", wszRightTeam );
		
		// TODO(SanyaSho): some code checks should be here..
		
		int iScoringTeam = m_bShowingGreenYellow ? TF_TEAM_YELLOW : TF_TEAM_RED;
		if( m_iScoringTeam == iScoringTeam )
			m_flTimeUpdateTeamScore = gpGlobals->curtime + 2.0;
	}
}
