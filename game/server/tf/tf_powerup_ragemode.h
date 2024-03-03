//====== Copyright © 1996-2013, Valve Corporation, All rights reserved. =======//
//
// Purpose: The ragemode powerup (name is a W.I.P). Equips the mercenary with
//			the Hammerfist weapon for a short period of time.
//
//=============================================================================//

#ifndef POWERUP_RAGEMODE_H
#define POWERUP_RAGEMODE_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_basedmpowerup.h"

//=============================================================================

class CTFPowerupRagemode : public CTFBaseDMPowerup
{
public:
	DECLARE_CLASS( CTFPowerupRagemode, CTFBaseDMPowerup );
	DECLARE_DATADESC();

	CTFPowerupRagemode();

	virtual const char *GetPowerupModel( void ) { return "models/items/powerup_berserk.mdl"; }
	virtual const char *GetPickupSound( void ) { return "PowerupBerserk.Touch"; }

	virtual ETFCond GetCondition( void ) { return TF_COND_POWERUP_RAGEMODE; }
	
	virtual int GetIncomingAnnouncement( void ) { return TF_ANNOUNCER_DM_BERSERK_INCOMING; }
	virtual int GetSpawnAnnouncement( void ) { return TF_ANNOUNCER_DM_BERSERK_SPAWN; }
	virtual int GetTeamPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_BERSERK_TEAMPICKUP; }
	virtual int GetEnemyPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_BERSERK_ENEMYPICKUP; }
};

#endif // POWERUP_RAGEMODE_H
