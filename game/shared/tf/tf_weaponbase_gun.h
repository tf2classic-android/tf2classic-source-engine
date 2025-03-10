//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: Weapon Base Gun 
//
//=============================================================================

#ifndef TF_WEAPONBASE_GUN_H
#define TF_WEAPONBASE_GUN_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_shareddefs.h"
#include "tf_weaponbase.h"

#if defined( CLIENT_DLL )
#define CTFWeaponBaseGun C_TFWeaponBaseGun
#endif

#define ZOOM_CONTEXT		"ZoomContext"
#define ZOOM_REZOOM_TIME	1.4f

//=============================================================================
//
// Weapon Base Melee Gun
//
class CTFWeaponBaseGun : public CTFWeaponBase
{
public:

	DECLARE_CLASS( CTFWeaponBaseGun, CTFWeaponBase );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
#if !defined( CLIENT_DLL ) 
	DECLARE_DATADESC();
#endif

	CTFWeaponBaseGun();

	virtual void ItemPostFrame( void );
	virtual void ItemBusyFrame( void );
	virtual void PrimaryAttack();
	virtual void SecondaryAttack( void );
	virtual bool Holster( CBaseCombatWeapon *pSwitchingTo );
	virtual void WeaponReset( void );

	// Derived classes call this to fire a bullet.
	//bool TFBaseGunFire( void );

	virtual void DoFireEffects();

	void ToggleZoom( void );

	void HandleSoftZoom( bool bZoomIn );

	virtual CBaseEntity *FireProjectile( CTFPlayer *pPlayer );
	virtual void		GetProjectileFireSetup( CTFPlayer *pPlayer, Vector vecOffset, Vector *vecSrc, QAngle *angForward, bool bHitTeammates = true, bool bUseHitboxes = false );
	void				GetProjectileReflectSetup( CTFPlayer *pPlayer, const Vector &vecPos, Vector *vecDeflect, bool bHitTeammates = true, bool bUseHitboxes = false );

	void FireBullet( CTFPlayer *pPlayer );
	CBaseEntity *FireRocket( CTFPlayer *pPlayer, int iType );
	CBaseEntity *FireNail( CTFPlayer *pPlayer, int iSpecificNail );
	CBaseEntity *FireArrow( CTFPlayer *pPlayer, int iType );
	CBaseEntity *FireGrenade( CTFPlayer *pPlayer, int iType );

	virtual float GetWeaponSpread( void );
	virtual float GetProjectileSpeed( void );
	virtual float GetProjectileGravity( void );
	virtual bool  IsFlameArrow( void ) { return false; }

	int GetAmmoPerShot();

	void UpdatePunchAngles( CTFPlayer *pPlayer );
	virtual float GetProjectileDamage( void );

	virtual void ZoomIn( void );
	virtual void ZoomOut( void );
	void ZoomOutIn( void );

	virtual void PlayWeaponShootSound( void );

private:
	CNetworkVar( int, m_iBurstSize );

	CTFWeaponBaseGun( const CTFWeaponBaseGun & );

	bool m_bInSoftZoom;
};

#endif // TF_WEAPONBASE_GUN_H
