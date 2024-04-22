//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: TF's custom CPlayerResource
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "tf_shareddefs.h"
#include "tf_player_resource.h"
#include "tf_player.h"
#include "tf_gamestats.h"
#include "tf_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Datatable
IMPLEMENT_SERVERCLASS_ST( CTFPlayerResource, DT_TFPlayerResource )
	SendPropArray3( SENDINFO_ARRAY3( m_iTotalScore ), SendPropInt( SENDINFO_ARRAY( m_iTotalScore ), 13 ) ),
	SendPropArray3( SENDINFO_ARRAY3( m_iMaxHealth ), SendPropInt( SENDINFO_ARRAY( m_iMaxHealth ), 10, SPROP_UNSIGNED ) ),
	SendPropArray3( SENDINFO_ARRAY3( m_iPlayerClass ), SendPropInt( SENDINFO_ARRAY( m_iPlayerClass ), 5, SPROP_UNSIGNED ) ),
	SendPropArray3( SENDINFO_ARRAY3( m_iActiveDominations ), SendPropInt( SENDINFO_ARRAY( m_iActiveDominations ), Q_log2( MAX_PLAYERS ) + 1, SPROP_UNSIGNED ) ),
	SendPropArray3( SENDINFO_ARRAY3( m_bArenaSpectator ), SendPropBool( SENDINFO_ARRAY( m_bArenaSpectator ) ) ),
	SendPropArray3( SENDINFO_ARRAY3( m_iKillstreak ), SendPropInt( SENDINFO_ARRAY( m_iKillstreak ), 10, SPROP_UNSIGNED ) ),
	SendPropArray3( SENDINFO_ARRAY3( m_iWinAnimations ), SendPropInt( SENDINFO_ARRAY( m_iWinAnimations ), 8, SPROP_UNSIGNED ) ),
	SendPropArray3( SENDINFO_ARRAY3( m_vecColors ), SendPropVector( SENDINFO_ARRAY( m_vecColors ), 8, 0, 0.0f, 1.0f ) ),
	SendPropArray3( SENDINFO_ARRAY3( m_iDisguiseTeam ), SendPropInt( SENDINFO_ARRAY( m_iDisguiseTeam ), 3, SPROP_UNSIGNED ) ),
	SendPropArray3( SENDINFO_ARRAY3( m_iDisguiseTarget ), SendPropInt( SENDINFO_ARRAY( m_iDisguiseTarget ), 7, SPROP_UNSIGNED ) ),
	SendPropArray3( SENDINFO_ARRAY3( m_bIsMobile ), SendPropBool( SENDINFO_ARRAY( m_bIsMobile ) ) ),
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( tf_player_manager, CTFPlayerResource );

CTFPlayerResource::CTFPlayerResource( void )
{
	for( int i = 0; i < MAX_PLAYERS + 1; i++ )
	{
		m_iTotalScore.Set( i, 0 );
		m_iMaxHealth.Set( i, 0 );
		m_iPlayerClass.Set( i, 0 );
		m_iActiveDominations.Set( i, 0 );
		m_bArenaSpectator.Set( i, false );
		m_iKillstreak.Set( i, 0 );
		m_iWinAnimations.Set( i, 0 );
		m_vecColors.Set( i, Vector( 1.f, 1.f, 1.f ) );
		m_iDisguiseTeam.Set( i, 0 );
		m_iDisguiseTarget.Set( i, 0 );
		m_bIsMobile.Set( i, false );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFPlayerResource::UpdatePlayerData( void )
{
	BaseClass::UpdatePlayerData();

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CTFPlayer *pPlayer = (CTFPlayer *)UTIL_PlayerByIndex( i );
		if ( pPlayer && pPlayer->IsConnected() )
		{
			CTFPlayerClass *pClass = pPlayer->GetPlayerClass();
			m_iMaxHealth.Set( i, pClass->GetMaxHealth() );
			m_iPlayerClass.Set( i, pClass->GetClassIndex() );

			RoundStats_t *pRoundStats = &CTF_GameStats.FindPlayerStats( pPlayer )->statsAccumulated;
#if SOONSOON
			if ( TFGameRules()->GetRetroModeType() == TF_GAMESUBTYPE_DUEL )
			{
				// In Duel, we want to show per-round stats instead.
				pRoundStats = &CTF_GameStats.FindPlayerStats( pPlayer )->statsCurrentRound;
			}
#endif

			int iTotalScore = CTFGameRules::CalcPlayerScore( pRoundStats );
			m_iTotalScore.Set( i, iTotalScore );

			m_iActiveDominations.Set( i, pPlayer->GetNumberOfDominations() );
			m_bArenaSpectator.Set( i, /*pPlayer->IsArenaSpectator()*/ false ); // FIXME
			m_iKillstreak.Set( i, pPlayer->m_Shared.GetKillstreak() );
			m_iWinAnimations.Set( i, pPlayer->GetClientConVarIntValue( "tf2c_merc_winanim" ) );
			m_vecColors.Set( i, pPlayer->m_vecPlayerColor );
			m_iDisguiseTeam.Set( i, pPlayer->m_Shared.GetDisguiseTeam() );
			m_iDisguiseTarget.Set( i, pPlayer->m_Shared.GetDisguiseTargetIndex() );
			m_bIsMobile.Set( i, pPlayer->IsMobilePlayer() );
		}
	}
}

void CTFPlayerResource::Spawn( void )
{
	for ( int i = 0; i < MAX_PLAYERS + 1; i++ )
	{
		m_iTotalScore.Set( i, 0 );
		m_iMaxHealth.Set( i, 1 );
		m_iPlayerClass.Set( i, TF_CLASS_UNDEFINED );
		m_iActiveDominations.Set( i, 0 );
		m_bArenaSpectator.Set( i, false );
		m_iKillstreak.Set( i, 0 );
		m_iWinAnimations.Set( i, 0 );
		m_vecColors.Set( i, Vector( 0.0, 0.0, 0.0 ) );
		m_iDisguiseTeam.Set( i, 0 );
		m_iDisguiseTarget.Set( i, 0 );
		m_bIsMobile.Set( i, false );
	}

	BaseClass::Spawn();
}

//-----------------------------------------------------------------------------
// Purpose: Gets a value from an array member
//-----------------------------------------------------------------------------
int CTFPlayerResource::GetTotalScore( int iIndex )
{
	CTFPlayer *pPlayer = (CTFPlayer*)UTIL_PlayerByIndex( iIndex );

	if ( pPlayer && pPlayer->IsConnected() )
	{
		return m_iTotalScore[iIndex];
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
Color CTFPlayerResource::GetPlayerColor( int iIndex )
{
	return Color( m_vecColors[iIndex].x * 255.0, m_vecColors[iIndex].y * 255.0, m_vecColors[iIndex].z * 255.0, 255 );
}
