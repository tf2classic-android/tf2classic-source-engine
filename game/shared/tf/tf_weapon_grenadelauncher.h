//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
//
//=============================================================================
#ifndef TF_WEAPON_GRENADELAUNCHER_H
#define TF_WEAPON_GRENADELAUNCHER_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_gun.h"
#include "tf_weaponbase_grenadeproj.h"

// Client specific.
#ifdef CLIENT_DLL
#define CTFGrenadeLauncher C_TFGrenadeLauncher
#define CTFGrenadeLauncher_Merc C_TFGrenadeLauncher_Merc
#endif

//=============================================================================
//
// TF Weapon Grenade Launcher.
//
//=============================================================================
class CTFGrenadeLauncher : public CTFWeaponBaseGun
{
public:

	DECLARE_CLASS( CTFGrenadeLauncher, CTFWeaponBaseGun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CTFGrenadeLauncher();
	~CTFGrenadeLauncher();

	virtual void	Spawn( void );
	virtual ETFWeaponID GetWeaponID( void ) const { return TF_WEAPON_GRENADELAUNCHER; }
	virtual float	GetProjectileSpeed( void );

private:
	CTFGrenadeLauncher( const CTFGrenadeLauncher & ) {}
};

//=============================================================================
//
// TF Weapon Grenade Launcher (Mercenary).
//
//=============================================================================
class CTFGrenadeLauncher_Merc : public CTFGrenadeLauncher
{
public:

	DECLARE_CLASS( CTFGrenadeLauncher_Merc, CTFGrenadeLauncher );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CTFGrenadeLauncher_Merc()
	{
	}

	virtual ~CTFGrenadeLauncher_Merc()
	{
	}

	virtual ETFWeaponID GetWeaponID( void ) const
	{
		return TF_WEAPON_GRENADELAUNCHER_MERC;
	}

	virtual float GetProjectileSpeed( void );
};
#endif // TF_WEAPON_GRENADELAUNCHER_H
