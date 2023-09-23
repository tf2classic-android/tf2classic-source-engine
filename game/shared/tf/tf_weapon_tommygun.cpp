#include "cbase.h"

#if defined( CLIENT_DLL )
#include "c_tf_player.h"
#else
#include "tf_player.h"
#endif

#include "tf_weapon_tommygun.h"

IMPLEMENT_NETWORKCLASS_ALIASED( TFTommyGun, DT_TFTommyGun )

BEGIN_NETWORK_TABLE( CTFTommyGun, DT_TFTommyGun )
	//
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFTommyGun )
	//
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_tommygun, CTFTommyGun );
PRECACHE_REGISTER( tf_weapon_tommygun );

CTFTommyGun::CTFTommyGun()
{
#if defined( CLIENT_DLL )
	m_flSpreadScale = 0.0f;
#endif
}

CTFTommyGun::~CTFTommyGun()
{
}

bool CTFTommyGun::CanFireAccurateShot( int nBulletsPerShot )
{
	return false;
}

bool CTFTommyGun::Deploy()
{
	if( !BaseClass::Deploy() )
		return false;

	m_flSpreadScale = 0.0f;

	return true;
}

CBaseEntity *CTFTommyGun::FireProjectile( CTFPlayer *pTFPlayer )
{
	float pPlayera = Approach( 1.0, m_flSpreadScale, 0.05 );

	if( m_flSpreadScale != pPlayera )
		m_flSpreadScale = pPlayera;

	return BaseClass::FireProjectile( pTFPlayer );
}

#if defined( CLIENT_DLL )
void CTFTommyGun::GetWeaponCrosshairScale( float &flScale )
{
	flScale = 1.0f;
	flScale = ( m_flSpreadScale + 1.0f );
}
#endif

float CTFTommyGun::GetWeaponSpread()
{
	return ( BaseClass::GetWeaponSpread() * m_flSpreadScale );
}

void CTFTommyGun::ItemBusyFrame()
{
#if defined( GAME_DLL )
	SpreadCooldown();
#else
	float v2 = 0.0f;
	if( ( gpGlobals->curtime - m_flLastFireTime ) >= 0.5f )
	{
		v2 = Approach( 0.0, m_flSpreadScale, ( gpGlobals->frametime * 2.0 ) );
		if( m_flSpreadScale != v2 )
			m_flSpreadScale = v2;
	}
#endif

	BaseClass::ItemBusyFrame();
}

void CTFTommyGun::ItemPostFrame()
{
#if defined( GAME_DLL )
	SpreadCooldown();
#else
	float v2 = 0.0f;
	if( ( gpGlobals->curtime - m_flLastFireTime ) >= 0.5f )
	{
		v2 = Approach( 0.0, m_flSpreadScale, gpGlobals->frametime * 2.0 );
		if( m_flSpreadScale != v2 )
			m_flSpreadScale = v2;
	}
#endif

	BaseClass::ItemPostFrame();
}

#if defined( GAME_DLL )
void CTFTommyGun::SpreadCooldown()
{
	if( ( gpGlobals->curtime - m_flLastFireTime ) >= 0.5f )
	{
		float v3 = 0.0f;
		v3 = Approach( 0.0, m_flSpreadScale, ( gpGlobals->frametime * 2.0 ) );
		if( m_flSpreadScale != v3 )
		{
			m_flSpreadScale = v3;
		}
	}
}
#endif

void CTFTommyGun::WeaponReset()
{
	BaseClass::WeaponReset();

	m_flSpreadScale = 0.0f;
}
