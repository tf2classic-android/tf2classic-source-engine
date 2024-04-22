//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: TF's custom C_PlayerResource
//
// $NoKeywords: $
//=============================================================================//

#ifndef C_TF_PLAYERRESOURCE_H
#define C_TF_PLAYERRESOURCE_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_shareddefs.h"
#include "c_playerresource.h"

#if !defined( PUBLIC_BUILD )
extern ConVar tf2c_dev_platform_override;
#endif

class C_TF_PlayerResource : public C_PlayerResource
{
	DECLARE_CLASS( C_TF_PlayerResource, C_PlayerResource );
public:
	DECLARE_CLIENTCLASS();

	C_TF_PlayerResource();
	virtual ~C_TF_PlayerResource();

	int		GetTotalScore( int iIndex ) { return GetArrayValue( iIndex, m_iTotalScore, 0 ); }
	int		GetMaxHealth( int iIndex ) { return GetArrayValue( iIndex, m_iMaxHealth, 1 ); }
	int		GetPlayerClass( int iIndex ) { return GetArrayValue( iIndex, m_iPlayerClass, TF_CLASS_UNDEFINED ); }
	int		GetNumberOfDominations( int iIndex ) { return GetArrayValue( iIndex, m_iActiveDominations, 0 ); }
	bool		IsArenaSpectator( int iIndex ) { return GetArrayValue( iIndex, m_bArenaSpectator ); }
	const Vector	&GetPlayerColorVector( int iIndex );
	Color		GetPlayerColor( int iIndex );
	int		GetWinAnimation( int iIndex ) { return GetArrayValue( iIndex, m_iWinAnimations, 1 ); }
	int		GetKillstreak( int iIndex ) { return GetArrayValue( iIndex, m_iKillstreak, 0 ); }

	bool		IsPlayerDominated( int iIndex );
	bool		IsPlayerDominating( int iIndex );
	bool		IsEnemyPlayer( int iIndex );
	int		GetUserID( int iIndex );
	bool		GetSteamID( int iIndex, CSteamID *pID );

	int		GetCountForPlayerClass( int iTeam, int iClass, bool bExcludeLocalPlayer = false );
	int		GetNumPlayersForTeam( int iTeam, bool bAlive = false );

	bool IsMobilePlayer( int iIndex ) const
	{
#if !defined( PUBLIC_BUILD )
		if( tf2c_dev_platform_override.GetInt() != -1 )
			return tf2c_dev_platform_override.GetBool();
#endif

		return m_bIsMobile[iIndex];
	}

protected:
	int GetArrayValue( int iIndex, int *pArray, int defaultVal );
	bool GetArrayValue( int iIndex, bool *pArray, bool defaultVal = false );

	int		m_iTotalScore[MAX_PLAYERS + 1];
	int		m_iMaxHealth[MAX_PLAYERS + 1];
	int		m_iPlayerClass[MAX_PLAYERS + 1];
	int		m_iActiveDominations[MAX_PLAYERS + 1];
	bool		m_bArenaSpectator[MAX_PLAYERS + 1];
	int		m_iKillstreak[MAX_PLAYERS + 1];
	int		m_iWinAnimations[MAX_PLAYERS + 1];
	Vector		m_vecColors[MAX_PLAYERS + 1];
	int		m_iDisguiseTeam[MAX_PLAYERS + 1];
	int		m_iDisguiseTarget[MAX_PLAYERS + 1];
	bool		m_bIsMobile[MAX_PLAYERS + 1];
};

extern C_TF_PlayerResource *g_TF_PR;

#endif // C_TF_PLAYERRESOURCE_H
