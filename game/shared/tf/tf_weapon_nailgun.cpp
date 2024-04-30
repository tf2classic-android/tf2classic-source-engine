//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
//
//=============================================================================
#include "cbase.h"
#include "tf_weapon_nailgun.h"

//=============================================================================
//
// Weapon Nailgun tables.
//
//=============================================================================
CREATE_SIMPLE_WEAPON_TABLE( TFNailgun, tf_weapon_nailgun )

//=============================================================================
//
// Weapon Nailgun functions.
//
//=============================================================================
void CTFNailgun::Precache()
{
	BaseClass::Precache();
#ifndef CLIENT_DLL
	PrecacheTeamParticles( "nailtrails_scout_%s", true );
	PrecacheTeamParticles( "nailtrails_scout_%s_crit", true );
#endif
}

//=============================================================================
//
// Weapon Super Nailgun tables.
//
//=============================================================================
CREATE_SIMPLE_WEAPON_TABLE( TFSuperNailgun, tf_weapon_supernailgun )
