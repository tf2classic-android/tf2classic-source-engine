//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include "c_tf_projectile_rocket.h"
#include "particles_new.h"
#include "tf_gamerules.h"

IMPLEMENT_NETWORKCLASS_ALIASED( TFProjectile_Rocket, DT_TFProjectile_Rocket )

BEGIN_NETWORK_TABLE( C_TFProjectile_Rocket, DT_TFProjectile_Rocket )
END_NETWORK_TABLE()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TFProjectile_Rocket::C_TFProjectile_Rocket( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TFProjectile_Rocket::~C_TFProjectile_Rocket( void )
{
	ParticleProp()->StopEmission();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_TFProjectile_Rocket::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );

	if ( updateType == DATA_UPDATE_CREATED )
	{
		CreateRocketTrails();
	}

	// Watch team changes and change trail accordingly.
	if ( m_iOldTeamNum && m_iOldTeamNum != m_iTeamNum )
	{
		ParticleProp()->StopEmission();
		CreateRocketTrails();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_TFProjectile_Rocket::CreateRocketTrails( void )
{
	if ( IsDormant() )
		return;

	if( m_iType == TF_PROJECTILE_ROCKET_CLASSIC )
	{
		ParticleProp()->Create( "rocket_trail_classic", PATTACH_POINT_FOLLOW, "trail" );

		if( m_bCritical )
		{
			const char *pszClassicEffectName = ConstructTeamParticle( "rocket_trail_classic_crit_%s", GetTeamNumber(), true );
			CNewParticleEffect *pNewParticle = ParticleProp()->Create( pszClassicEffectName, PATTACH_ABSORIGIN_FOLLOW );

			C_TFPlayer *pTFPlayer = ToTFPlayer( GetOwnerEntity() );
			if( pTFPlayer )
			{
				pTFPlayer->m_Shared.SetParticleToMercColor( pNewParticle );
			}
		}
	}
	else
	{
		if ( enginetrace->GetPointContents( GetAbsOrigin() ) & MASK_WATER )
		{
			ParticleProp()->Create( "rockettrail_underwater", PATTACH_POINT_FOLLOW, "trail" );
		}
		else
		{
			ParticleProp()->Create( GetTrailParticleName(), PATTACH_POINT_FOLLOW, "trail" );
		}

		if ( m_bCritical )
		{
			const char *pszEffectName = ConstructTeamParticle( "critical_rocket_%s", GetTeamNumber(), true );
			CNewParticleEffect *pParticle = ParticleProp()->Create( pszEffectName, PATTACH_ABSORIGIN_FOLLOW );

			C_TFPlayer *pPlayer = ToTFPlayer( GetOwnerEntity() );
			if ( pPlayer )
			{
				pPlayer->m_Shared.SetParticleToMercColor( pParticle );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *C_TFProjectile_Rocket::GetTrailParticleName( void )
{
	if( TFGameRules() && TFGameRules()->IsDeathmatch() )
	{
		return "rockettrail_dm";
	}

	return "rockettrail";
}
