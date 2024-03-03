//=============================================================================//
//
// Purpose: FILL IN
//
//=============================================================================//

#ifndef POWERUP_MEGAHEALTH_H
#define POWERUP_MEGAHEALTH_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_basedmpowerup.h"

//=============================================================================

class CTFPowerupMegaHealth : public CTFBaseDMPowerup
{
public:
	DECLARE_CLASS( CTFPowerupMegaHealth, CTFBaseDMPowerup );

	virtual const char *GetPowerupModel( void ) { return "models/items/powerup_health.mdl"; }
	virtual const char *GetPickupSound( void ) { return "PowerupMegaHealth.Touch"; }

	//virtual int GetIncomingAnnouncement( void ) { return TF_ANNOUNCER_DM_MEGAHEALTH_INCOMING; }
	//virtual int GetSpawnAnnouncement( void ) { return TF_ANNOUNCER_DM_MEGAHEALTH_SPAWN; }
	//virtual int GetTeamPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_MEGAHEALTH_TEAMPICKUP; }
	//virtual int GetEnemyPickupAnnouncement( void ) { return TF_ANNOUNCER_DM_MEGAHEALTH_ENEMYPICKUP; }

	virtual bool MyTouch( CBasePlayer *pPlayer );
};

#endif // POWERUP_MEGAHEALTH_H
