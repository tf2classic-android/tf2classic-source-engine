#ifndef TF_WEAPON_LEVERRIFLE_H
#define TF_WEAPON_LEVERRIFLE_H
#include "tf_weapon_sniperrifle.h"
#ifdef _WIN32
#pragma once
#endif

#include "tf_weapon_sniperrifle.h"

#if defined( CLIENT_DLL )
#define CTFLeverRifle C_TFLeverRifle
#endif

class CTFLeverRifle : public CTFSniperRifle
{
	DECLARE_CLASS( CTFLeverRifle, CTFSniperRifle );
public:
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	
	CTFLeverRifle();
	virtual ~CTFLeverRifle();
	
	virtual ETFWeaponID GetWeaponID( void ) const { return TF_WEAPON_LEVERRIFLE; }
	
	bool CanHeadshot() const { return true; }
	
	virtual float GetProjectileDamage()
	{
		return (fminf(fmaxf((float)(GetChargedDamage() - 50.0) * 0.009f, 0.0), 1.0) + 1.0) * 75.0;
	};
	
	void MakeTracer( const Vector &vecTracerSrc, const trace_t &tr );
};

#endif // TF_WEAPON_LEVERRIFLE_H
