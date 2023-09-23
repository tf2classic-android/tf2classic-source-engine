#ifndef TF_WEAPON_CUBEMAP_H
#define TF_WEAPON_CUBEMAP_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase.h"

#ifdef CLIENT_DLL
#define CTFWeaponCubemap C_TFWeaponCubemap
#endif

class CTFWeaponCubemap : public CTFWeaponBase
{
public:
	DECLARE_CLASS( CTFWeaponCubemap, CTFWeaponBase );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	virtual int GetWeaponID( void ) const
	{
		return TF_WEAPON_CUBEMAP;
	}

	virtual void PrimaryAttack( void )
	{
	}

	virtual void SecondaryAttack( void )
	{
	}
};

#endif // TF_WEAPON_CUBEMAP_H
