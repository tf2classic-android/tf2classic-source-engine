//=============================================================================//
//
// Purpose: Deathmatch HUD
//
//=============================================================================//

#include "cbase.h"
#include "tf_hud_deathmatchstatus.h"
#include "c_tf_playerresource.h"
#include "tf_hud_freezepanel.h"
#include "c_tf_team.h"
#include "tf_gamerules.h"
#include "functionproxy.h"
#include "clientsteamcontext.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//vgui::IImage* GetDefaultAvatarImage( int iPlayerIndex );
extern ConVar fraglimit;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFHudDeathMatchObjectives::CTFHudDeathMatchObjectives( Panel *parent, const char *name ) : EditablePanel( parent, name )
{
	m_pLocalPlayerPanel = new EditablePanel( this, "LocalPlayerPanel" );
	m_pBestPlayerPanel = new EditablePanel( this, "BestPlayerPanel" );
	m_pPlayerAvatar = new CAvatarImagePanel( m_pLocalPlayerPanel, "PlayerAvatar" );
	m_pRivalAvatar = new CAvatarImagePanel( m_pBestPlayerPanel, "PlayerAvatar" );

	m_pPlayingTo = new CExLabel( this, "PlayingTo", "0" );
	m_pPlayingToBG = NULL;

	m_iRivalPlayer = 0;

	ivgui()->AddTickSignal( GetVPanel(), 100 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTFHudDeathMatchObjectives::IsVisible( void )
{
	if ( IsTakingAFreezecamScreenshot() )
		return false;

	if ( GetLocalPlayerTeam() < FIRST_GAME_TEAM )
		return false;

	if ( !TFGameRules() || TFGameRules()->IsInWaitingForPlayers() || TFGameRules()->State_Get() == GR_STATE_PREGAME )
		return false;

	return BaseClass::IsVisible();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudDeathMatchObjectives::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	// load control settings...
	LoadControlSettings( "resource/UI/HudObjectiveDeathMatchPanel.res" );

	m_pPlayingToBG = dynamic_cast<CTFImagePanel *>( FindChildByName( "PlayingToBG" ) );

	m_pPlayerAvatar->SetShouldDrawFriendIcon( false );
	m_pRivalAvatar->SetShouldDrawFriendIcon( false );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudDeathMatchObjectives::LevelInit( void )
{
	m_iRivalPlayer = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudDeathMatchObjectives::Reset()
{
#if defined( ENABLE_STEAM_AVATARS )
	m_pPlayerAvatar->SetPlayer( ClientSteamContext().GetLocalPlayerSteamID(), k_EAvatarSize32x32 );
#else
	m_pPlayerAvatar->SetPlayer( C_TFPlayer::GetLocalPlayer(), k_EAvatarSize32x32 );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudDeathMatchObjectives::SetPlayingToLabelVisible( bool bVisible )
{
	m_pPlayingTo->SetVisible( bVisible );
	if ( m_pPlayingToBG )
	{
		m_pPlayingToBG->SetVisible( bVisible );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudDeathMatchObjectives::OnTick( void )
{
	if ( !g_PR )
		return;

	C_TF_PlayerResource *g_TF_PR = dynamic_cast<C_TF_PlayerResource *>( g_PR );

	if ( !IsVisible() || !TFGameRules() || !g_TF_PR )
		return;

	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer )
		return;

	// Get the local player's score.
	int iLocalIndex = pPlayer->entindex();
	int iLocalScore = g_TF_PR->GetPlayerScore( iLocalIndex );

	m_pLocalPlayerPanel->SetDialogVariable( "score", iLocalScore );

	// Figure out who's the leading player.
	int iBestIndex = 0;
	int iBestScore = 0;

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		if ( g_TF_PR->IsConnected( i ) && g_PR->GetTeam( i ) >= FIRST_GAME_TEAM )
		{
			int iScore = g_TF_PR->GetPlayerScore( i );

			if ( i != iLocalIndex && iScore >= iBestScore )
			{
				iBestScore = iScore;
				iBestIndex = i;
			}
		}
	}

	// Set their score and avatar.
	if ( iBestIndex )
	{
		m_pBestPlayerPanel->SetDialogVariable( "score", iBestScore );
	}
	else
	{
		m_pBestPlayerPanel->SetDialogVariable( "score", "" );
	}

	if ( iBestIndex != m_iRivalPlayer )
	{
		m_iRivalPlayer = iBestIndex;

		m_pRivalAvatar->SetPlayer( iBestIndex, k_EAvatarSize32x32 );
		//m_pRivalAvatar->SetDefaultAvatar( GetDefaultAvatarImage( iBestIndex ) );
	}

	// Show the leading player on the top.
	if ( iLocalScore >= iBestScore )
	{
		m_pLocalPlayerPanel->SetZPos( 2 );
		m_pBestPlayerPanel->SetZPos( 1 );
	}
	else
	{
		m_pLocalPlayerPanel->SetZPos( 1 );
		m_pBestPlayerPanel->SetZPos( 2 );
	}

	// Update "Playing to" panel with the frag limit.
	int iFragLimit = TFGameRules()->GetFragLimit();

	if ( iFragLimit > 0 )
	{
		SetPlayingToLabelVisible( true );
		SetDialogVariable( "fraglimit", iFragLimit );
	}
	else
	{
		SetPlayingToLabelVisible( false );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CBestPlayerTintColor : public CResultProxy
{
public:
	void OnBind( void *pC_BaseEntity )
	{
		if ( !g_PR )
			return;

		C_TF_PlayerResource *g_TF_PR = dynamic_cast<C_TF_PlayerResource *>( g_PR );

		if ( !g_TF_PR )
			return;

		int iLocalIndex = GetLocalPlayerIndex();
		int iBestIndex = 0;
		int iBestScore = 0;

		for ( int i = 1; i <= gpGlobals->maxClients; i++ )
		{
			if ( g_TF_PR->IsConnected( i ) && g_PR->GetTeam( i ) >= FIRST_GAME_TEAM )
			{
				int iScore = g_TF_PR->GetPlayerScore( i );

				if ( i != iLocalIndex && iScore >= iBestScore )
				{
					iBestScore = iScore;
					iBestIndex = i;
				}
			}
		}

		if ( iBestIndex )
		{
			m_pResult->SetVecValue( g_TF_PR->GetPlayerColorVector( iBestIndex ).Base(), 3 );
		}
		else
		{
			m_pResult->SetVecValue( 0, 0, 0 );
		}
	}
};

EXPOSE_INTERFACE( CBestPlayerTintColor, IMaterialProxy, "BestPlayerTintColor" IMATERIAL_PROXY_INTERFACE_VERSION );
