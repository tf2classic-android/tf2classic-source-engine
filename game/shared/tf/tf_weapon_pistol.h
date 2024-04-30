//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
//
//=============================================================================
#ifndef TF_WEAPON_PISTOL_H
#define TF_WEAPON_PISTOL_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_gun.h"

// Client specific.
#ifdef CLIENT_DLL
#define CTFPistol C_TFPistol
#define CTFPistol_Scout C_TFPistol_Scout
#define CTFPistol_Merc C_TFPistol_Merc
#endif

// The faster the player fires, the more inaccurate he becomes
#define	PISTOL_ACCURACY_SHOT_PENALTY_TIME		0.2f	// Applied amount of time each shot adds to the time we must recover from
#define	PISTOL_ACCURACY_MAXIMUM_PENALTY_TIME	1.5f	// Maximum time penalty we'll allow

//=============================================================================
//
// TF Weapon Pistol.
//
class CTFPistol : public CTFWeaponBaseGun
{
public:

	DECLARE_CLASS( CTFPistol, CTFWeaponBaseGun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CTFPistol() {}
	~CTFPistol() {}

	virtual void	PrimaryAttack( void );

	virtual ETFWeaponID GetWeaponID( void ) const { return TF_WEAPON_PISTOL; }

	DECLARE_DM_ACTTABLE();

private:
	CTFPistol( const CTFPistol & ) {}
};

// Scout specific version
class CTFPistol_Scout : public CTFPistol
{
public:
	DECLARE_CLASS( CTFPistol_Scout, CTFPistol );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	virtual ETFWeaponID GetWeaponID( void ) const { return TF_WEAPON_PISTOL_SCOUT; }
};

// Mercenary
class CTFPistol_Merc : public CTFPistol
{
public:
	DECLARE_CLASS( CTFPistol_Merc, CTFPistol );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	virtual ETFWeaponID GetWeaponID( void ) const
	{
		return TF_WEAPON_PISTOL_MERC;
	}
};

#endif // TF_WEAPON_PISTOL_H
