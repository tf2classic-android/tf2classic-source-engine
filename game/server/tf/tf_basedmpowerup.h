//====== Copyright © 1996-2013, Valve Corporation, All rights reserved. =======//
//
// Purpose: Base class for Deathmatch powerups 
//
//=============================================================================//

#ifndef BASE_DM_POWERUP_H
#define BASE_DM_POWERUP_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_powerup.h"
#include "tf_shareddefs.h"
#include "tf_announcer.h"

//=============================================================================

DECLARE_AUTO_LIST( ITFBaseDMPowerupAutoList )
class CTFBaseDMPowerup : public CTFPowerup, public ITFBaseDMPowerupAutoList
{
public:
	DECLARE_CLASS( CTFBaseDMPowerup, CTFPowerup );
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

	CTFBaseDMPowerup();
	
	virtual void    Spawn( void );
	virtual void    Precache( void );
	virtual int             UpdateTransmitState( void );
	
	virtual void    HideOnPickedUp( void );
	virtual void    UnhideOnRespawn( void );
	virtual void    OnIncomingSpawn( void );
	virtual bool    ValidTouch( CBasePlayer *pPlayer );
	virtual bool    MyTouch( CBasePlayer *pPlayer );
	
	float	GetEffectDuration( void ) { return m_flEffectDuration; }
	void	SetEffectDuration( float flTime ) { m_flEffectDuration = flTime; }
	
	virtual const char *GetPickupSound( void ) { return "HealthKit.Touch"; }
	virtual const char *GetPowerupModel( void ) { return "models/class_menu/random_class_icon.mdl"; }
	virtual ETFCond GetCondition( void ) { return TF_COND_LAST; } // Should trigger an assert.
	virtual int GetIncomingAnnouncement( void ) { return TF_ANNOUNCER_DM_CRIT_INCOMING; }
	virtual int GetSpawnAnnouncement( void ) { return TF_ANNOUNCER_DM_CRIT_SPAWN; }
	virtual int GetTeamPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_CRIT_TEAMPICKUP; }
	virtual int GetEnemyPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_CRIT_ENEMYPICKUP; }

	static CTFBaseDMPowerup *Create( const Vector &vecOrigin, const QAngle &vecAngles, CBaseEntity *pOwner, const char *pszClassname, float flDuration );

protected:
	float		m_flEffectDuration;
};

#endif // BASE_DM_POWERUP_H
