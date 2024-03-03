//====== Copyright © 1996-2013, Valve Corporation, All rights reserved. =======//
//
// Purpose: FILL IN
//
//=============================================================================//

#ifndef POWERUP_CLOAK_H
#define POWERUP_CLOAK_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_basedmpowerup.h"

//=============================================================================

class CTFPowerupCloak : public CTFBaseDMPowerup
{
public:
	DECLARE_CLASS( CTFPowerupCloak, CTFBaseDMPowerup );
	DECLARE_DATADESC();

	CTFPowerupCloak();

	virtual const char *GetPowerupModel( void ) { return "models/items/powerup_invis.mdl"; }
	virtual const char *GetPickupSound( void ) { return "PowerupCloak.Touch"; }

	virtual ETFCond GetCondition( void ) { return TF_COND_POWERUP_CLOAK; }
	
	virtual int GetIncomingAnnouncement( void ) { return TF_ANNOUNCER_DM_CLOAK_INCOMING; }
	virtual int GetSpawnAnnouncement( void ) { return TF_ANNOUNCER_DM_CLOAK_SPAWN; }
	virtual int GetTeamPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_CLOAK_TEAMPICKUP; }
	virtual int GetEnemyPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_CLOAK_ENEMYPICKUP; }
};

#endif // POWERUP_CLOAK_H
