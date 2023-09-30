#if !defined( TF_WEAPON_STENGUN_H )
#define TF_WEAPON_STENGUN_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase.h"
#include "tf_weaponbase_gun.h"
#include "tf_weapon_smg.h"

#if defined( CLIENT_DLL )
#define CTFStenGun C_TFStenGun
#endif

class CTFStenGun : public CTFSMG
{
public:
	DECLARE_CLASS( CTFStenGun, CTFSMG );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CTFStenGun();
	virtual ~CTFStenGun();

	virtual ETFWeaponID GetWeaponID( void ) const
	{
		return TF_WEAPON_STENGUN;
	}
};

#endif // !defined( TF_WEAPON_STENGUN_H )
