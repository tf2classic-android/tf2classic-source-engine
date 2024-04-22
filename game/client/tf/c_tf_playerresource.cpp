//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: TF's custom C_PlayerResource
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "c_tf_playerresource.h"
#include <shareddefs.h>
#include "tf_shareddefs.h"
#include "hud.h"
#include "tf_gamerules.h"
#include "clientsteamcontext.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#if !defined( PUBLIC_BUILD )
ConVar tf2c_dev_platform_override( "tf2c_dev_platform_override", "", FCVAR_NONE, "Override GetPlayerPlatform value." );
#endif

IMPLEMENT_CLIENTCLASS_DT( C_TF_PlayerResource, DT_TFPlayerResource, CTFPlayerResource )
	RecvPropArray3( RECVINFO_ARRAY( m_iTotalScore ), RecvPropInt( RECVINFO( m_iTotalScore[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_iMaxHealth ), RecvPropInt( RECVINFO( m_iMaxHealth[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_iPlayerClass ), RecvPropInt( RECVINFO( m_iPlayerClass[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_iActiveDominations ), RecvPropInt( RECVINFO( m_iActiveDominations[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_bArenaSpectator ), RecvPropBool( RECVINFO( m_bArenaSpectator[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_iKillstreak ), RecvPropInt( RECVINFO( m_iKillstreak[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_iWinAnimations ), RecvPropInt( RECVINFO( m_iWinAnimations[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_vecColors ), RecvPropVector( RECVINFO( m_vecColors[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_iDisguiseTeam ), RecvPropInt( RECVINFO( m_iDisguiseTeam[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_iDisguiseTarget ), RecvPropInt( RECVINFO( m_iDisguiseTarget[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_bIsMobile ), RecvPropBool( RECVINFO( m_bIsMobile[0] ) ) ),
END_RECV_TABLE()

C_TF_PlayerResource *g_TF_PR = NULL;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TF_PlayerResource::C_TF_PlayerResource()
{
	m_Colors[TEAM_UNASSIGNED] = COLOR_TF_SPECTATOR;
	m_Colors[TEAM_SPECTATOR] = COLOR_TF_SPECTATOR;
	m_Colors[TF_TEAM_RED] = COLOR_RED;
	m_Colors[TF_TEAM_BLUE] = COLOR_BLUE;
	m_Colors[TF_TEAM_GREEN] = COLOR_GREEN;
	m_Colors[TF_TEAM_YELLOW] = COLOR_YELLOW;

	g_TF_PR = this;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TF_PlayerResource::~C_TF_PlayerResource()
{
	g_TF_PR = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Gets a value from an array member
//-----------------------------------------------------------------------------
int C_TF_PlayerResource::GetArrayValue( int iIndex, int *pArray, int iDefaultVal )
{
	if ( !IsConnected( iIndex ) )
	{
		return iDefaultVal;
	}
	return pArray[iIndex];
}

//-----------------------------------------------------------------------------
// Purpose: Gets a value from an array member
//-----------------------------------------------------------------------------
bool C_TF_PlayerResource::GetArrayValue( int iIndex, bool *pArray, bool bDefaultVal )
{
	if ( !IsConnected( iIndex ) )
	{
		return bDefaultVal;
	}
	return pArray[iIndex];
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const Vector &C_TF_PlayerResource::GetPlayerColorVector( int iIndex )
{
	if ( !IsConnected( iIndex ) )
	{
		// White color.
		static Vector vecWhite( 1, 1, 1 );
		return vecWhite;
	}

	return m_vecColors[iIndex];
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
Color C_TF_PlayerResource::GetPlayerColor( int iIndex )
{
	const Vector &vecColor = GetPlayerColorVector( iIndex );

	return Color( vecColor.x * 255.0f, vecColor.y * 255.0f, vecColor.z * 255.0f, 255 );
}

//-----------------------------------------------------------------------------
// Purpose: Returns if this player is an enemy of the local player.
//-----------------------------------------------------------------------------
bool C_TF_PlayerResource::IsEnemyPlayer( int iIndex )
{
	int iTeam = GetTeam( iIndex );
	int iLocalTeam = GetLocalPlayerTeam();

	// Spectators are nobody's enemy.
	if ( iLocalTeam < FIRST_GAME_TEAM )
		return false;

	// In FFA everybody is an enemy. Except for ourselves.
#if SOONSOON
	if ( !TFGameRules()->IsTeamplay() )
#else
	if ( TFGameRules()->IsDeathmatch() )
#endif
		return !IsLocalPlayer( iIndex );

	// Players from other teams are enemies.
	return ( iTeam != iLocalTeam );
}

//-----------------------------------------------------------------------------
// Purpose: Returns if this player is dominated by the local player.
//-----------------------------------------------------------------------------
bool C_TF_PlayerResource::IsPlayerDominated( int iIndex )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer )
		return false;

	return pPlayer->m_Shared.IsPlayerDominated( iIndex );
}

//-----------------------------------------------------------------------------
// Purpose: Returns if this player is dominating the local player.
//-----------------------------------------------------------------------------
bool C_TF_PlayerResource::IsPlayerDominating( int iIndex )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer )
		return false;

	return pPlayer->m_Shared.IsPlayerDominatingMe( iIndex );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int C_TF_PlayerResource::GetUserID( int iIndex )
{
	if ( !IsConnected( iIndex ) )
		return 0;

	player_info_t pi;
	if ( engine->GetPlayerInfo( iIndex, &pi ) )
	{
		return pi.userID;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool C_TF_PlayerResource::GetSteamID( int iIndex, CSteamID *pID )
{
	if ( !IsConnected( iIndex ) )
		return false;

	// Copied from C_BasePlayer.
	player_info_t pi;
	if ( engine->GetPlayerInfo( iIndex, &pi ) && pi.friendsID && ClientSteamContext().BLoggedOn() )
	{
		pID->InstancedSet( pi.friendsID, 1, ClientSteamContext().GetConnectedUniverse(), k_EAccountTypeIndividual );
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int C_TF_PlayerResource::GetCountForPlayerClass( int iTeam, int iClass, bool bExcludeLocalPlayer /*=false*/ )
{
	int count = 0;
	int iLocalPlayerIndex = GetLocalPlayerIndex();

	for ( int i = 1; i <= MAX_PLAYERS; i++ )
	{
		if ( bExcludeLocalPlayer && ( i == iLocalPlayerIndex ) )
		{
			continue;
		}

		if ( ( GetTeam( i ) == iTeam ) && ( GetPlayerClass( i ) == iClass ) )
		{
			count++;
		}
	}

	return count;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int C_TF_PlayerResource::GetNumPlayersForTeam( int iTeam, bool bAlive /*= false*/ )
{
	int count = 0;

	for ( int i = 1; i <= MAX_PLAYERS; i++ )
	{
		if ( !IsConnected( i ) )
			continue;

		if ( GetTeam( i ) == iTeam && ( !bAlive || IsAlive( i ) ) )
		{
			count++;
		}
	}

	return count;
}
