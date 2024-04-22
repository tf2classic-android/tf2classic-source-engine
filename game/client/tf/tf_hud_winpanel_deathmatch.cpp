//=============================================================================//
//
// Purpose: 
//
//=============================================================================//
#include "cbase.h"
#include "tf_hud_winpanel_deathmatch.h"
#include "iclientmode.h"
#include <game/client/iviewport.h>
#include <vgui/ILocalize.h>
#include "vgui_avatarimage.h"
#include "tf_controls.h"
#include "c_tf_playerresource.h"
#include "tf_gamerules.h"
#include "tf_merc_customizations.h"
#include "animation.h"
#include "tf_viewport.h"
#include <engine/IEngineSound.h>
#include "tf_music_controller.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

DECLARE_HUDELEMENT( CTFWinPanelDeathmatch );

CTFWinPanelDeathmatch::CTFWinPanelDeathmatch( const char *pszName ) : CHudElement( pszName ), BaseClass( NULL, "WinPanelDeathmatch" )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	for ( int i = 0; i < 3; i++ )
	{
		m_pPlayerNames[i] = new CExLabel( this, VarArgs( "Player%dName", i + 1 ), "" );
		m_pPlayerKills[i] = new CExLabel( this, VarArgs( "Player%dKills", i + 1 ), "" );
		m_pPlayerDeaths[i] = new CExLabel( this, VarArgs( "Player%dDeaths", i + 1 ), "" );
		m_pPlayerModels[i] = new CTFPlayerModelPanel( this, VarArgs( "Player%dModel", i + 1 ) );
	}

	m_flShowAt = -1.0f;
	m_bHiddenScoreboard = false;
	m_iWinAnimation = 0;

	ListenForGameEvent( "deathmatch_results" );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFWinPanelDeathmatch::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/ui/WinPanelDeathmatch.res" );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CTFWinPanelDeathmatch::ShouldDraw( void )
{
	if ( m_flShowAt != -1.0f && gpGlobals->curtime >= m_flShowAt )
	{
		// Time to show ourselves, hide the scoreboard.
		if ( !m_bHiddenScoreboard )
		{
			GetTFViewPort()->ShowScoreboard( false );
			m_bHiddenScoreboard = true;

			// Play the win animation for the winning player.
			if ( m_pPlayerModels[0] && m_pPlayerModels[0]->IsVisible() )
			{
				win_anim_t *pAnimData = g_TFMercCustomizations.GetAnimationById( m_iWinAnimation );
				if ( pAnimData )
				{
					int iSequence = m_pPlayerModels[0]->LookupSequence( pAnimData->szSequence );
					if ( iSequence != -1 )
					{
						m_pPlayerModels[0]->SetSequence( iSequence, true );
					}
				}
			}
		}
	}
	else
	{
		return false;
	}

	return CHudElement::ShouldDraw();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFWinPanelDeathmatch::LevelInit( void )
{
	m_flShowAt = -1.0f;
	m_bHiddenScoreboard = false;
	m_iWinAnimation = 0;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFWinPanelDeathmatch::FireGameEvent( IGameEvent *event )
{
	bool bPlayerFirst = false;

	if ( !g_TF_PR )
		return;

	// look for the top 3 players sent in the event
	for ( int i = 0; i < 3; i++ )
	{
		bool bShow = false;
		char szPlayerIndexVal[64] = "", szPlayerScoreVal[64] = "", szPlayerKillsVal[64] = "", szPlayerDeathsVal[64] = "";
		// get player index and round points from the event
		V_sprintf_safe( szPlayerIndexVal, "player_%d", i + 1 );
		V_sprintf_safe( szPlayerScoreVal, "player_%d_points", i + 1 );
		V_sprintf_safe( szPlayerKillsVal, "player_%d_kills", i + 1 );
		V_sprintf_safe( szPlayerDeathsVal, "player_%d_deaths", i + 1 );
		int iPlayerIndex = event->GetInt( szPlayerIndexVal, 0 );
		int iRoundScore = event->GetInt( szPlayerScoreVal, 0 );
		int iPlayerKills = event->GetInt( szPlayerKillsVal, 0 );
		int iPlayerDeaths = event->GetInt( szPlayerDeathsVal, 0 );

		// round score of 0 means no player to show for that position (not enough players, or didn't score any points that round)
		if ( iRoundScore > 0 )
			bShow = true;

#if 0
		CAvatarImagePanel *pPlayerAvatar = dynamic_cast<CAvatarImagePanel *>( FindChildByName( VarArgs( "Player%dAvatar", i + 1 ) ) );

		if ( pPlayerAvatar )
		{
			pPlayerAvatar->ClearAvatar();
			if ( bShow )
			{
				pPlayerAvatar->SetPlayer( GetSteamIDForPlayerIndex( iPlayerIndex ), k_EAvatarSize32x32 );
				pPlayerAvatar->SetAvatarSize( 32, 32 );
			}
			pPlayerAvatar->SetVisible( bShow );
		}
#endif

		if ( bShow )
		{
			// set the player labels to team color
			Color clr = g_TF_PR->GetPlayerColor( iPlayerIndex );
			m_pPlayerNames[i]->SetFgColor( clr );
			m_pPlayerKills[i]->SetFgColor( clr );
			m_pPlayerDeaths[i]->SetFgColor( clr );

			// set label contents
			m_pPlayerNames[i]->SetText( g_PR->GetPlayerName( iPlayerIndex ) );

			wchar_t wszKills[16], wszKillsLabel[64];
			wchar_t wszDeaths[16], wszDeathsLabel[64];
			V_swprintf_safe( wszKills, L"%d", iPlayerKills );
			V_swprintf_safe( wszDeaths, L"%d", iPlayerDeaths );

			g_pVGuiLocalize->ConstructString( wszKillsLabel, sizeof( wszKillsLabel ), g_pVGuiLocalize->Find( "#TF_Winpanel_Deathmatch_Kills" ), 1, wszKills );
			g_pVGuiLocalize->ConstructString( wszDeathsLabel, sizeof( wszDeathsLabel ), g_pVGuiLocalize->Find( "#TF_Winpanel_Deathmatch_Deaths" ), 1, wszDeaths );

			m_pPlayerKills[i]->SetText( wszKillsLabel );
			m_pPlayerDeaths[i]->SetText( wszDeathsLabel );

			if ( i == 0 && iPlayerIndex == GetLocalPlayerIndex() )
				bPlayerFirst = true;

			m_pPlayerModels[i]->SetToPlayerClass( g_TF_PR->GetPlayerClass( iPlayerIndex ) );
			m_pPlayerModels[i]->SetTeam( g_TF_PR->GetTeam( iPlayerIndex ), true, false );
			//m_pPlayerModels[i]->ClearCarriedItems();
			m_pPlayerModels[i]->SetModelTintColor( g_TF_PR->GetPlayerColorVector( iPlayerIndex ) );

			// Remember win animation to play later.
			if ( i == 0 )
			{
				m_iWinAnimation = g_TF_PR->GetWinAnimation( iPlayerIndex );
			}
		}

		// show or hide labels for this player position
		m_pPlayerNames[i]->SetVisible( bShow );
		m_pPlayerKills[i]->SetVisible( bShow );
		m_pPlayerDeaths[i]->SetVisible( bShow );
		m_pPlayerModels[i]->SetVisible( bShow );
	}

	if( !g_pMusicController || !g_pMusicController->IsPlaying() && !g_pMusicController->IsPlayingEnding() )
	{
		bool bPlayWinMusic = ( bPlayerFirst || GetLocalPlayerTeam() < FIRST_GAME_TEAM );
		CLocalPlayerFilter filter;
		C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, bPlayWinMusic ? "music.dm_winpanel_first" : "music.dm_winpanel" );
	}
	
	m_flShowAt = gpGlobals->curtime + 4.5f;
}
