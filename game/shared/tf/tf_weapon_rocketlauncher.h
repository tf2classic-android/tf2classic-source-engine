//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// TF Rocket Launcher
//
//=============================================================================
#ifndef TF_WEAPON_ROCKETLAUNCHER_H
#define TF_WEAPON_ROCKETLAUNCHER_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_gun.h"
#include "tf_weaponbase_rocket.h"

// Client specific.
#ifdef CLIENT_DLL
#define CTFRocketLauncher C_TFRocketLauncher
#define CTFRocketLauncher_Merc C_TFRocketLauncher_Merc
#endif

//=============================================================================
//
// TF Weapon Rocket Launcher.
//
//=============================================================================
class CTFRocketLauncher : public CTFWeaponBaseGun
{
public:

	DECLARE_CLASS( CTFRocketLauncher, CTFWeaponBaseGun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CTFRocketLauncher();
	~CTFRocketLauncher();

#ifndef CLIENT_DLL
	virtual void	Precache();
#endif
	virtual int		GetWeaponID( void ) const			{ return TF_WEAPON_ROCKETLAUNCHER; }
	virtual CBaseEntity *FireProjectile( CTFPlayer *pPlayer );
	virtual void	ItemPostFrame( void );
	virtual bool	Deploy( void );
	virtual bool	DefaultReload( int iClipSize1, int iClipSize2, int iActivity );

#ifdef CLIENT_DLL
	virtual void CreateMuzzleFlashEffects( C_BaseEntity *pAttachEnt, int nIndex );
#endif

private:
	float	m_flShowReloadHintAt;

	CTFRocketLauncher( const CTFRocketLauncher & ) {}
};


//=============================================================================
//
// TF Weapon Rocket Launcher (QuakeRPG)
//
//=============================================================================
class CTFRocketLauncher_Merc : public CTFRocketLauncher
{
public:

	DECLARE_CLASS( CTFRocketLauncher_Merc, CTFRocketLauncher );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CTFRocketLauncher_Merc()
	{
		/* Nothing */
	}

	virtual ~CTFRocketLauncher_Merc()
	{
		/* Nothing */
	}

	virtual int GetWeaponID( void ) const
	{
		return TF_WEAPON_ROCKETLAUNCHER_MERC;
	}
};
#endif // TF_WEAPON_ROCKETLAUNCHER_H
