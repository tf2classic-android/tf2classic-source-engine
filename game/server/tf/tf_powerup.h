//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======//
//
// Purpose: CTF AmmoPack.
//
//=============================================================================//
#ifndef TF_POWERUP_H
#define TF_POWERUP_H

#include "util_shared.h"
#ifdef _WIN32
#pragma once
#endif

#include "items.h"
#include "tf_shareddefs.h"

// FIXME(SanyaSho): MOVE TO TF_SHAREDDEFS.H!
#define TF_GAMETYPE_COUNT TF_GAMETYPE_VIP + 1

enum powerupsize_t
{
	POWERUP_TINY,
	POWERUP_SMALL,
	POWERUP_MEDIUM,
	POWERUP_FULL,
	POWERUP_MEGA,

	POWERUP_SIZES,
};

extern float PackRatios[POWERUP_SIZES];

//=============================================================================
//
// CTF Powerup class.
//

DECLARE_AUTO_LIST( ITFPowerupAutoList );
class CTFPowerup : public CItem, public ITFPowerupAutoList
{
public:
	DECLARE_CLASS( CTFPowerup, CItem );
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	CTFPowerup();

	virtual void	Precache( void );
	virtual void	Spawn( void );
	virtual bool	KeyValue( const char *szKeyName, const char *szValue );
	virtual CBaseEntity*	Respawn( void );
	virtual void	Materialize( void );
	virtual void	HideOnPickedUp( void );
	virtual void	UnhideOnRespawn( void );
	virtual void	OnIncomingSpawn( void ) {}
	virtual bool	ValidTouch( CBasePlayer *pPlayer );
	virtual bool	MyTouch( CBasePlayer *pPlayer );
	virtual bool	ItemCanBeTouchedByPlayer( CBasePlayer *pPlayer );

	void			SetRespawnTime( float flDelay );
	void			RespawnThink( void );
	bool			IsDisabled( void );
	void			SetDisabled( bool bDisabled );
	void			FireOutputsOnPickup( CBasePlayer *pPlayer );

	void			DropSingleInstance( const Vector &vecVelocity, CBaseCombatCharacter *pOwner, float flOwnerPickupDelay, float flRestTime, float flRemoveTime = 30.0f );

	// Input handlers
	void			InputEnable( inputdata_t &inputdata );
	void			InputDisable( inputdata_t &inputdata );
	void			InputEnableWithEffect( inputdata_t &inputdata );
	void			InputDisableWithEffect( inputdata_t &inputdata );
	void			InputToggle( inputdata_t &inputdata );
	void			InputRespawnNow( inputdata_t &inputdata );

	bool			IsRespawning() const { return m_bRespawning; }

	bool			ValidateForGameType( int  iType );
	static void		UpdatePowerupsForGameType( int  iType );
	
	virtual powerupsize_t	GetPowerupSize( void ) { return POWERUP_FULL; }
	float			m_flNextCollideTime;
	
protected:
	float m_flRespawnDelay;
	float m_flInitialSpawnDelay;
	bool m_bDropped;

private:
	CNetworkVar( bool, m_bDisabled );
	CNetworkVar( bool, m_bRespawning );
	CNetworkVar( float, m_flRespawnStartTime );
	CNetworkVar( float, m_flRespawnTime );

	float m_flOwnerPickupEnableTime;
	bool m_bFire15SecRemain;
	bool m_bEnabledModes[TF_GAMETYPE_COUNT];

	COutputEvent m_outputOnRespawn;
	COutputEvent m_outputOn15SecBeforeRespawn;
	COutputEvent m_outputOnTeam1Touch;
	COutputEvent m_outputOnTeam2Touch;
	COutputEvent m_outputOnTeam3Touch;
	COutputEvent m_outputOnTeam4Touch;
};

#endif // TF_POWERUP_H


