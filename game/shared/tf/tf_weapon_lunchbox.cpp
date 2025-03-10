//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================
#include "cbase.h"
#include "tf_weapon_lunchbox.h"

#ifdef GAME_DLL
#include "tf_player.h"
#include "tf_powerup.h"
#else
#include "c_tf_player.h"
#endif

CREATE_SIMPLE_WEAPON_TABLE( TFLunchBox, tf_weapon_lunchbox )

#define TF_SANDVICH_PLATE_MODEL "models/items/plate.mdl"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFLunchBox::WeaponReset( void )
{
	BaseClass::WeaponReset();

#ifdef GAME_DLL
	// Reset "eaten" status.
	SetBodygroup( 0, 0 );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTFLunchBox::Deploy( void )
{
	if ( BaseClass::Deploy() )
	{
		// Reset "eaten" status.
		SetBodygroup( 0, 0 );
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFLunchBox::PrimaryAttack( void )
{
	CTFPlayer *pOwner = GetTFPlayerOwner();

	if ( !pOwner )
		return;

	if ( !CanAttack() )
		return;

#ifdef GAME_DLL
	pOwner->Taunt();
#endif

	m_flNextPrimaryAttack = gpGlobals->curtime + 1.0f;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFLunchBox::SecondaryAttack( void )
{
	CTFPlayer *pOwner = GetTFPlayerOwner();
	if ( !pOwner )
		return;

	if ( !CanAttack() )
		return;

#ifdef GAME_DLL
	// Remove the previous dropped lunch box.
	if ( m_hDroppedLunch.Get() )
	{
		UTIL_Remove( m_hDroppedLunch.Get() );
		m_hDroppedLunch = NULL;
	}

	// Throw a sandvich plate down on the ground.
	Vector vecSrc, vecThrow;
	QAngle angThrow;
	vecSrc = pOwner->EyePosition();

	// A bit below the eye position.
	vecSrc.z -= 8.0f;

	CTFPowerup *pPowerup = static_cast<CTFPowerup *>( CBaseEntity::Create( "item_healthkit_medium", vecSrc, vec3_angle, pOwner ) );
	if ( !pPowerup )
		return;

	// Throw it down.
	angThrow = pOwner->EyeAngles();
	angThrow[PITCH] -= 10.0f;
	AngleVectors( angThrow, &vecThrow );
	vecThrow *= 500;

	pPowerup->DropSingleInstance( vecThrow, pOwner, 0.3f, 0.1f );

	// Don't collide with the player owner for the first portion of its life
	pPowerup->m_flNextCollideTime = gpGlobals->curtime + 0.5f;

	pPowerup->SetModel( TF_SANDVICH_PLATE_MODEL );
	UTIL_SetSize( pPowerup, -Vector( 17, 17, 10 ), Vector( 17, 17, 10 ) );

	m_hDroppedLunch = pPowerup;
#endif

	// Switch away from it immediately, don't want it to stick around.
	pOwner->RemoveAmmo( 1, m_iPrimaryAmmoType );
	pOwner->SwitchToNextBestWeapon( this );

	StartEffectBarRegen();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFLunchBox::DepleteAmmo( void )
{
	CTFPlayer *pOwner = GetTFPlayerOwner();
	if ( !pOwner )
	{
		return;
	}

	if ( pOwner->HealthFraction() >= 1.0f )
	{
		return;
	}

	// Switch away from it immediately, don't want it to stick around.
	pOwner->RemoveAmmo( 1, m_iPrimaryAmmoType );
	pOwner->SwitchToNextBestWeapon( this );

	StartEffectBarRegen();
}

#ifdef GAME_DLL

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFLunchBox::Precache( void )
{
	UTIL_PrecacheOther( "item_healthkit_medium" );
	PrecacheModel( TF_SANDVICH_PLATE_MODEL );

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFLunchBox::ApplyBiteEffects( void )
{
	// Heal 30 HP per second for a total 120 HP.
	CTFPlayer *pOwner = GetTFPlayerOwner();

	if ( pOwner )
	{
		pOwner->TakeHealth( 30, DMG_GENERIC );
	}

	// Show bite marks.
	SetBodygroup( 0, 1 );
}

#endif