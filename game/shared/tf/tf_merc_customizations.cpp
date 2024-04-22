#include "cbase.h"
#include "tf_gamerules.h"
#include "tier2/tier2.h"
#include "filesystem.h"
#include "tf_merc_customizations.h"

#if defined( CLIENT_DLL )
#include "c_tf_player.h"
#else
#include "tf_player.h"
#include "enginecallback.h"
#endif

#if defined( CLIENT_DLL )
ConVar tf2c_merc_particle( "tf2c_merc_particle", "1", FCVAR_ARCHIVE | FCVAR_USERINFO, "Sets merc's respawn particle index" );
ConVar tf2c_merc_winanim( "tf2c_merc_winanim", "1", FCVAR_ARCHIVE | FCVAR_USERINFO, "Sets FFA win animation." );
#endif

ConVar tf2c_dev_log_merc_customizations( "tf2c_dev_log_merc_customizations", "0", FCVAR_CHEAT | FCVAR_REPLICATED );
#define DoIfEnabled( x ) if( tf2c_dev_log_merc_customizations.GetBool() ) x

CTFMercCustomizations g_TFMercCustomizations( "CTFMercCustomizations" );

CTFMercCustomizations::CTFMercCustomizations( const char *pszName ) : CAutoGameSystem( pszName )
{
}

bool CTFMercCustomizations::Init()
{
	KeyValues *pKV = new KeyValues( "respawn_particles" );
	if( !pKV )
	{
		// impossible
		return false;
	}

	if( pKV->LoadFromFile( filesystem, MERC_CUSTOM_FILE, "MOD" ) )
	{
#if defined( CLIENT_DLL )
		KeyValues *pWinAnimations= pKV->FindKey( "WinAnimations" );
		if( pWinAnimations )
		{
			DoIfEnabled( ConColorMsg( Color( 22, 95, 67, 255 ), "WinAnimations:\n" ) );
			
			for( KeyValues *pData = pWinAnimations->GetFirstSubKey(); pData != NULL; pData = pData->GetNextKey() )
			{
				int iID = Q_atoi( pData->GetName() );
				if( iID < 1 ) // skip 0 and negative
					continue;

				const char *pszSequence = pData->GetString( "sequence" );
				const char *pszName = pData->GetString( "name" );

				win_anim_t newAnim;
				newAnim.id = iID;
				V_strncpy( newAnim.szSequence, pszSequence, sizeof( newAnim.szSequence ) );
				V_strncpy( newAnim.szName, pszName, sizeof( newAnim.szName ) );
				DoIfEnabled( ConColorMsg( Color( 22, 95, 67, 255 ), "\tAnimation %d %s\n\tSequence: %s\n", iID, newAnim.szName, newAnim.szSequence ) );
				m_WinAnimations.AddToTail( newAnim );
			}
		}
#endif
		KeyValues *pSpawnEffects = pKV->FindKey( "SpawnEffects" );
		if( pSpawnEffects )
		{
			DoIfEnabled( ConColorMsg( Color( 44, 69, 29, 255 ), "SpawnEffects:\n" ) );
			
			for( KeyValues *pData = pSpawnEffects->GetFirstSubKey(); pData != NULL; pData = pData->GetNextKey() )
			{
				int iID = Q_atoi( pData->GetName() );
				if( iID < 1 ) // skip 0 and negative
					continue;

				const char *pszEffect = pData->GetString( "effect" );
				const char *pszName = pData->GetString( "name" );

				respawn_particle_t newParticle;
				newParticle.id = iID;
				V_strncpy( newParticle.szEffect, pszEffect, sizeof( newParticle.szEffect ) );
				V_strncpy( newParticle.szName, pszName, sizeof( newParticle.szName ) );
				DoIfEnabled( ConColorMsg( Color( 44, 69, 29, 255 ), "\tParticle %d %s\n\tEffect: %s\n", iID, newParticle.szName, newParticle.szEffect ) );
				m_Particles.AddToTail( newParticle );
			}
		}
	}
	else
	{
		pKV->deleteThis();
		Warning( "Failed to load respawn particle data from %s!\n", MERC_CUSTOM_FILE );
		return false;
	}

	pKV->deleteThis();
	return true;
}

void CTFMercCustomizations::LevelInitPreEntity()
{
#if defined( GAME_DLL )
	FOR_EACH_VEC( m_Particles, i )
	{
		PrecacheParticleSystem( m_Particles[i].szEffect );
	}
#endif
}

#if defined( CLIENT_DLL )
respawn_particle_t *CTFMercCustomizations::GetParticle( int index )
{
	if( m_Particles.Count() <= 0 )
	{
		return NULL;
	}

	return &m_Particles[index];
}

win_anim_t *CTFMercCustomizations::GetAnimationById( int id )
{
	if( m_WinAnimations.Count() <= 0 )
	{
		return NULL;
	}

	FOR_EACH_VEC( m_WinAnimations, i )
	{
		if( m_WinAnimations[i].id == id )
			return &m_WinAnimations[i];
	}

	return NULL;
}

win_anim_t *CTFMercCustomizations::GetAnimation( int index )
{
	if( m_WinAnimations.Count() <= 0 )
	{
		return NULL;
	}

	return &m_WinAnimations[index];
}
#else
respawn_particle_t *CTFMercCustomizations::GetParticleById( int id )
{
	if( m_Particles.Count() <= 0 )
	{
		return NULL;
	}

	FOR_EACH_VEC( m_Particles, i )
	{
		if( m_Particles[i].id == id )
			return &m_Particles[i];
	}

	return NULL;
}

respawn_particle_t *CTFMercCustomizations::GetRandomParticle()
{
	if( m_Particles.Count() <= 0 )
	{
		return NULL;
	}

	return &m_Particles.Random();
}
#endif
