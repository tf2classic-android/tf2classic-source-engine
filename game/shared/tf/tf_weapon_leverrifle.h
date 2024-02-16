#ifndef TF_WEAPON_LEVERRIFLE_H
#define TF_WEAPON_LEVERRIFLE_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_weapon_sniperrifle.h"

#if defined( CLIENT_DLL )
#define CTFLeverRifle C_TFLeverRifle
#endif

#define TF_WEAPON_LEVERRIFLE_CHARGE_PER_SEC		100.0
#define TF_WEAPON_LEVERRIFLE_UNCHARGE_PER_SEC		180.0

class CTFLeverRifle : public CTFSniperRifle
{
	DECLARE_CLASS( CTFLeverRifle, CTFSniperRifle );
public:
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	
	CTFLeverRifle();
	virtual ~CTFLeverRifle();
	
	virtual ETFWeaponID GetWeaponID( void ) const OVERRIDE { return TF_WEAPON_LEVERRIFLE; }
	
	bool CanHeadshot() const { return true; }
	
	virtual float GetProjectileDamage() OVERRIDE
	{
		return (fminf(fmaxf((float)(GetChargedDamage() - 50.0) * 0.009f, 0.0), 1.0) + 1.0) * 75.0;
	};
	
	void MakeTracer( const Vector &vecTracerSrc, const trace_t &tr );
	
	float GetChargeSpeed() OVERRIDE { return TF_WEAPON_LEVERRIFLE_CHARGE_PER_SEC; }
	float GetUnchargeSpeed() OVERRIDE { return TF_WEAPON_LEVERRIFLE_UNCHARGE_PER_SEC; }
};

#endif // TF_WEAPON_LEVERRIFLE_H
