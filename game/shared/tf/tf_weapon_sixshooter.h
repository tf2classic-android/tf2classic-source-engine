#if !defined( TF_WEAPON_SIXSHOOTER_H )
#define TF_WEAPON_SIXSHOOTER_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase.h"
#include "tf_weaponbase_gun.h"

#if defined( CLIENT_DLL )
#define CTFSixShooter C_TFSixShooter
#endif

class CTFSixShooter : public CTFWeaponBaseGun
{
public:
	DECLARE_CLASS( CTFSixShooter, CTFWeaponBaseGun );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CTFSixShooter();
	virtual ~CTFSixShooter();

	virtual ETFWeaponID GetWeaponID( void ) const
	{
		return TF_WEAPON_SIXSHOOTER;
	}
};

#endif // !defined( TF_WEAPON_SIXSHOOTER )
