//====== Copyright © 1996-2013, Valve Corporation, All rights reserved. =======//
//
// Purpose: FILL IN
//
//=============================================================================//

#ifndef POWERUP_SHIELD_H
#define POWERUP_SHIELD_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_basedmpowerup.h"

//=============================================================================

class CTFPowerupShield : public CTFBaseDMPowerup
{
public:
	DECLARE_CLASS( CTFPowerupShield, CTFBaseDMPowerup );
	DECLARE_DATADESC();

	CTFPowerupShield();

	virtual const char *GetPowerupModel( void ) { return "models/items/powerup_shield.mdl"; }
	virtual const char *GetPickupSound( void ) { return "PowerupUber.Touch"; }

	virtual ETFCond GetCondition( void ) { return TF_COND_POWERUP_SHIELD; }
	
	virtual int GetIncomingAnnouncement( void ) { return TF_ANNOUNCER_DM_SHIELD_INCOMING; }
	virtual int GetSpawnAnnouncement( void ) { return TF_ANNOUNCER_DM_SHIELD_SPAWN; }
	virtual int GetTeamPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_SHIELD_TEAMPICKUP; }
	virtual int GetEnemyPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_SHIELD_ENEMYPICKUP; }
};

#endif // POWERUP_SHIELD_H
