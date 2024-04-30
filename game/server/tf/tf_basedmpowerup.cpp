//====== Copyright © 1996-2013, Valve Corporation, All rights reserved. =======//
//
// Purpose: Base class for Deathmatch powerups 
//
//=============================================================================//

#include "cbase.h"
#include "items.h"
#include "tf_gamerules.h"
#include "tf_shareddefs.h"
#include "tf_player.h"
#include "tf_team.h"
#include "tf_basedmpowerup.h"
#include "tf_announcer.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//=============================================================================

BEGIN_DATADESC( CTFBaseDMPowerup )
	DEFINE_KEYFIELD( m_flRespawnDelay, FIELD_FLOAT, "RespawnTime" ),
	DEFINE_KEYFIELD( m_flInitialSpawnDelay, FIELD_FLOAT, "InitialSpawnDelay" ),
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CTFBaseDMPowerup, DT_TFBaseDMPowerup )
END_SEND_TABLE()

IMPLEMENT_AUTO_LIST( ITFBaseDMPowerupAutoList )

//=============================================================================

//-----------------------------------------------------------------------------
// Purpose: Constructor 
//-----------------------------------------------------------------------------
CTFBaseDMPowerup::CTFBaseDMPowerup()
{
	// Default duration to 30 seconds, adjust in base classes as necessary.
	m_flEffectDuration = 30.0f;
	
	// 2 minutes respawn time.
	m_flRespawnDelay = 120.0f;
}

CTFBaseDMPowerup *CTFBaseDMPowerup::Create( const Vector &vecOrigin, const QAngle &vecAngles, CBaseEntity *pOwner, const char *pszClassname, float flDuration )
{
	CTFBaseDMPowerup *pPowerup = dynamic_cast<CTFBaseDMPowerup *>( CBaseEntity::CreateNoSpawn( pszClassname, vecOrigin, vecAngles, pOwner ) );
	
	if ( pPowerup )
	{
		pPowerup->SetEffectDuration( flDuration );
		pPowerup->AddSpawnFlags( SF_NORESPAWN );
		pPowerup->DropSingleInstance( vec3_origin, ToBaseCombatCharacter( pOwner ), 0.3f, 0.1f );
	}

	return pPowerup;
}

//-----------------------------------------------------------------------------
// Purpose: Precache 
//-----------------------------------------------------------------------------
void CTFBaseDMPowerup::Precache( void )
{
	PrecacheModel( GetPowerupModel() );
	PrecacheScriptSound( GetPickupSound() );
	
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: Spawn function 
//-----------------------------------------------------------------------------
void CTFBaseDMPowerup::Spawn( void )
{
	Precache();
	SetModel( GetPowerupModel() );
	SetRenderMode( kRenderTransColor );
	
	BaseClass::Spawn();
	
	// Don't bounce.
	SetElasticity( 0.0f );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CTFBaseDMPowerup::UpdateTransmitState()
{
	return SetTransmitState( FL_EDICT_ALWAYS );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFBaseDMPowerup::HideOnPickedUp( void )
{
	SetRenderColorA( 80 );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFBaseDMPowerup::UnhideOnRespawn( void )
{
	SetRenderColorA( 255 );
	EmitSound( "Item.Materialize" );
	g_TFAnnouncer.Speak( GetSpawnAnnouncement() );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFBaseDMPowerup::OnIncomingSpawn( void )
{
	g_TFAnnouncer.Speak( GetIncomingAnnouncement() );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CTFBaseDMPowerup::ValidTouch( CBasePlayer *pPlayer )
{
	CTFPlayer *pTFPlayer = ToTFPlayer( pPlayer );
	
	if ( !pTFPlayer || pTFPlayer->HasTheFlag() )
		return false;
	
	return BaseClass::ValidTouch( pPlayer );
}

//-----------------------------------------------------------------------------
// Purpose: Touch function
//-----------------------------------------------------------------------------
bool CTFBaseDMPowerup::MyTouch( CBasePlayer *pPlayer )
{
	CTFPlayer *pTFPlayer = ToTFPlayer( pPlayer );
	
	if ( pTFPlayer && ValidTouch( pPlayer ) )
	{
		// Keep the higher duration
		if ( pTFPlayer->m_Shared.GetConditionDuration( GetCondition() ) < GetEffectDuration() )
		{
			// Add the condition and duration from derived classes
			pTFPlayer->m_Shared.AddCond( GetCondition(), GetEffectDuration() );
		}
		
		// Give full health
		if ( !m_bDropped )
		{
			pTFPlayer->TakeHealth( pTFPlayer->GetMaxHealth(), HEAL_NOTIFY );
			pTFPlayer->m_Shared.HealNegativeConds();
		}
		
		pTFPlayer->EmitSound( GetPickupSound() );
		
		if ( !TFGameRules()->IsDeathmatch() && TFGameRules()->IsTeamplay() )
		{
			for ( int i = FIRST_GAME_TEAM; i < GetNumberOfTeams(); i++ )
			{
				if ( i != pPlayer->GetTeamNumber() )
				{
					CTeamRecipientFilter filter( i, true );
					g_TFAnnouncer.Speak( filter, GetEnemyPickupAnnouncement() );
				}
				else
				{
					CTeamRecipientFilter filter( i, true );
					filter.RemoveRecipient( pPlayer );
					g_TFAnnouncer.Speak( filter, GetTeamPickupAnnouncement() );
				}
			}
		}
		else
		{
			CTeamRecipientFilter filter( FIRST_GAME_TEAM, true );
			filter.RemoveRecipient( pPlayer );
			g_TFAnnouncer.Speak( filter, GetEnemyPickupAnnouncement() );
		}
		
		return true;
	}
	
	return false;
}
