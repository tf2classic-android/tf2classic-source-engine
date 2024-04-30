//====== Copyright © 1996-2013, Valve Corporation, All rights reserved. =======//
//
// Purpose: FILL IN
//
//=============================================================================//

#ifndef POWERUP_SPEEDBOOST_H
#define POWERUP_SPEEDBOOST_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_basedmpowerup.h"

//=============================================================================

class CTFPowerupSpeedBoost : public CTFBaseDMPowerup
{
public:
	DECLARE_CLASS( CTFPowerupSpeedBoost, CTFBaseDMPowerup );
	DECLARE_DATADESC();

	CTFPowerupSpeedBoost();

	virtual const char *GetPowerupModel( void ) { return "models/items/powerup_speed.mdl"; }
	virtual const char *GetPickupSound( void ) { return "PowerupSpeedBoost.Touch"; }
	
	virtual ETFCond GetCondition( void ) { return TF_COND_POWERUP_SPEEDBOOST; }
	
	virtual int GetIncomingAnnouncement( void ) { return TF_ANNOUNCER_DM_HASTE_INCOMING; }
	virtual int GetSpawnAnnouncement( void ) { return TF_ANNOUNCER_DM_HASTE_SPAWN; }
	virtual int GetTeamPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_HASTE_TEAMPICKUP; }
	virtual int GetEnemyPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_HASTE_ENEMYPICKUP; }
};

#endif // POWERUP_SPEEDBOOST_H
