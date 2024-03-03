//=============================================================================//
//
// Purpose: CTF AmmoPack.
//
//=============================================================================//
#ifndef C_TF_POWERUP_H
#define C_TF_POWERUP_H

#ifdef _WIN32
#pragma once
#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_TFPowerup : public C_BaseAnimating
{
public:
	DECLARE_CLASS( C_TFPowerup, C_BaseAnimating );
	DECLARE_CLIENTCLASS();

	C_TFPowerup();

	virtual void OnDataChanged( DataUpdateType_t updateType );
	virtual int DrawModel( int flags );

	virtual bool ShouldShowRespawnTimer( void ) { return false; }
	virtual float GetRespawnTimerSize( void ) { return 20.0f; }
	virtual Color GetRespawnTimerColor( void ) { return COLOR_WHITE; }

	ShadowType_t ShadowCastType( void ) ;

protected:
	bool m_bDisabled;
	bool m_bRespawning;
	float m_flRespawnStartTime;
	float m_flRespawnTime;

private:
	CMaterialReference m_ReturnProgressMaterial_Empty;
	CMaterialReference m_ReturnProgressMaterial_Full;
};

#endif // C_TF_POWERUP_H
