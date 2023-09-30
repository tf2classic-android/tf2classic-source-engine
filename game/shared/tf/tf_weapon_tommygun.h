#if !defined( TF_WEAPON_TOMMYGUN_H )
#define TF_WEAPON_TOMMYGUN_H

#if defined( WIN32 )
#pragma once
#endif

#include "tf_weaponbase.h"
#include "tf_weaponbase_gun.h"

#include "tf_weapon_smg.h"

#if defined( CLIENT_DLL )
#define CTFTommyGun C_TFTommyGun
#endif

class CTFTommyGun : public CTFSMG
{
public:
	DECLARE_CLASS( CTFTommyGun, CTFSMG );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CTFTommyGun();
	virtual ~CTFTommyGun();

	virtual ETFWeaponID GetWeaponID( void ) const
	{
		return TF_WEAPON_TOMMYGUN;
	}

	virtual bool CanFireAccurateShot( int nBulletsPerShot ); // returns 0

	virtual bool Deploy();

	virtual CBaseEntity *FireProjectile( CTFPlayer *pTFPlayer );

#if defined( CLIENT_DLL )
	virtual void GetWeaponCrosshairScale( float &flScale );
#endif

	virtual float GetWeaponSpread();

	virtual void ItemBusyFrame();

	virtual void ItemPostFrame();

#if defined( GAME_DLL )
	virtual void SpreadCooldown();
#endif

	virtual void WeaponReset();

protected:
	CNetworkVar( float, m_flSpreadScale );

};

#endif // !TF_WEAPON_TOMMYGUN_H
