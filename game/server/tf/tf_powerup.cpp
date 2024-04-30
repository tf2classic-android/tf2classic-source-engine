//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======//
//
// Purpose: CTF AmmoPack.
//
//=============================================================================//
#include "cbase.h"
#include "tf_powerup.h"
#include "tf_gamerules.h"
#include "particle_parse.h"
#include "util_shared.h"

//=============================================================================
float PackRatios[POWERUP_SIZES] =
{
	0.1,	// TF2C TINY
	0.2,	// SMALL
	0.5,	// MEDIUM
	1.0,	// FULL
	2.0,	// TF2C MEGA
};

//=============================================================================
//
// CTF Powerup tables.
//
IMPLEMENT_SERVERCLASS_ST( CTFPowerup, DT_TFPowerup )
	SendPropBool( SENDINFO( m_bDisabled ) ),
	SendPropBool( SENDINFO( m_bRespawning ) ),
	SendPropTime( SENDINFO( m_flRespawnStartTime ) ),
	SendPropTime( SENDINFO( m_flRespawnTime ) ),
END_SEND_TABLE()

BEGIN_DATADESC( CTFPowerup )

	// Keyfields.
	DEFINE_KEYFIELD( m_bDisabled, FIELD_BOOLEAN, "StartDisabled" ),

	// Inputs.
	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableWithEffect", InputEnableWithEffect ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableWithEffect", InputDisableWithEffect ),
	DEFINE_INPUTFUNC( FIELD_VOID, "RespawnNow", InputRespawnNow ),

	// Outputs.
	DEFINE_OUTPUT( m_outputOnRespawn, "OnRespawn" ),
	DEFINE_OUTPUT( m_outputOn15SecBeforeRespawn, "On15SecBeforeRespawn" ),
	DEFINE_OUTPUT( m_outputOnTeam1Touch, "OnTeam1Touch" ),
	DEFINE_OUTPUT( m_outputOnTeam2Touch, "OnTeam2Touch" ),
	DEFINE_OUTPUT( m_outputOnTeam3Touch, "OnTeam3Touch" ),
	DEFINE_OUTPUT( m_outputOnTeam4Touch, "OnTeam4Touch" ),

	DEFINE_THINKFUNC( RespawnThink ),

END_DATADESC();

IMPLEMENT_AUTO_LIST( ITFPowerupAutoList );

//=============================================================================
//
// CTF Powerup functions.
//

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFPowerup::CTFPowerup()
{
	m_bDisabled = false;
	m_bRespawning = false;
	m_flNextCollideTime = 0.0f;
	m_flOwnerPickupEnableTime = 0.0f;

	// 10 seconds respawn time by default. Override in derived classes.
	m_flRespawnDelay = 10.0f;
	m_flInitialSpawnDelay = 0.0f;
	m_bDropped = false;
	m_bFire15SecRemain = false;
	memset( m_bEnabledModes, 0, sizeof( m_bEnabledModes ) );
	
	// SanyaSho: HACK
	for ( int i = 1; i < TF_GAMETYPE_COUNT; i++ )
	{
		m_bEnabledModes[i] = true;
	}

	UseClientSideAnimation();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFPowerup::Precache( void )
{
	BaseClass::Precache();

	PrecacheParticleSystem( "ExplosionCore_buildings" );

	PrecacheMaterial( "vgui/flagtime_full" );
	PrecacheMaterial( "vgui/flagtime_empty" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFPowerup::Spawn( void )
{
	BaseClass::Spawn();

	AddEffects( EF_ITEM_BLINK );
	
	SetOriginalSpawnOrigin( GetLocalOrigin() );
	SetOriginalSpawnAngles( GetAbsAngles() );

	VPhysicsDestroyObject();
	SetMoveType( MOVETYPE_NONE );
	SetSolidFlags( FSOLID_NOT_SOLID | FSOLID_TRIGGER );

	if ( ValidateForGameType( TFGameRules()->GetGameType() ) == true )
	{
		SetDisabled( m_bDisabled );

		ResetSequence( LookupSequence( "idle" ) );

		if ( m_flInitialSpawnDelay > 0.0f && !m_bDropped )
		{
			// Don't spawn immediately.
			Respawn();

			// Override the respawn time
			SetRespawnTime( m_flInitialSpawnDelay );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTFPowerup::KeyValue( const char *szKeyName, const char *szValue )
{
	/*for ( int i = 1; i < TF_GAMETYPE_COUNT; i++ )
	{
		if ( FStrEq( szKeyName, UTIL_VarArgs( "EnabledIn%s", g_aGameTypeInfo[i].name ) ) )
		{
			m_bEnabledModes[i] = !!atoi( szValue );
			return true;
		}
	}*/

	return BaseClass::KeyValue( szKeyName, szValue );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CBaseEntity* CTFPowerup::Respawn( void )
{
	m_bRespawning = true;

	HideOnPickedUp();
	SetTouch( NULL );

	RemoveAllDecals(); //remove any decals
	
	RemoveEffects( EF_ITEM_BLINK );

	// Set respawn time.
	SetRespawnTime( m_flRespawnDelay );
	m_flRespawnStartTime = gpGlobals->curtime;

	if ( !m_bDisabled )
	{
		SetContextThink( &CTFPowerup::RespawnThink, gpGlobals->curtime, "RespawnThinkContext" );
	}

	return this;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFPowerup::Materialize( void )
{
	m_bRespawning = false;
	UnhideOnRespawn();
	SetTouch( &CItem::ItemTouch );

	m_outputOnRespawn.FireOutput( this, this );
	SetContextThink( NULL, 0, "RespawnThinkContext" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFPowerup::HideOnPickedUp( void )
{
	AddEffects( EF_NODRAW );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFPowerup::UnhideOnRespawn( void )
{
	EmitSound( "Item.Materialize" );
	AddEffects( EF_ITEM_BLINK );
	RemoveEffects( EF_NODRAW );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTFPowerup::ValidTouch( CBasePlayer *pPlayer )
{
	if ( IsRespawning() )
	{
		return false;
	}

	// Is the item enabled?
	if ( IsDisabled() )
	{
		return false;
	}

	// Only touch a live player.
	if ( !pPlayer || !pPlayer->IsPlayer() || !pPlayer->IsAlive() )
	{
		return false;
	}

	// Team number and does it match?
	int iTeam = GetTeamNumber();
	if ( iTeam && ( pPlayer->GetTeamNumber() != iTeam ) )
	{
		return false;
	}
	
	// Don't collide with the owner for the first portion of our life if we're a lunchbox item
	if ( m_flNextCollideTime > gpGlobals->curtime && pPlayer == GetOwnerEntity() )
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTFPowerup::MyTouch( CBasePlayer *pPlayer )
{
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTFPowerup::ItemCanBeTouchedByPlayer( CBasePlayer *pPlayer )
{
	// Owner can't pick it up for some time after dropping it.
	if ( pPlayer == GetOwnerEntity() && gpGlobals->curtime < m_flOwnerPickupEnableTime )
		return false;

	return BaseClass::ItemCanBeTouchedByPlayer( pPlayer );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFPowerup::SetRespawnTime( float flDelay )
{
	m_flRespawnTime = gpGlobals->curtime + flDelay;
	m_bFire15SecRemain = ( flDelay >= 15.0f );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFPowerup::RespawnThink( void )
{
	if ( m_bFire15SecRemain && m_flRespawnTime - gpGlobals->curtime <= 15.0f )
	{
		m_bFire15SecRemain = false;
		m_outputOn15SecBeforeRespawn.FireOutput( this, this );
		OnIncomingSpawn();
	}

	if ( gpGlobals->curtime >= m_flRespawnTime )
	{
		Materialize();
	}
	else
	{
		SetContextThink( &CTFPowerup::RespawnThink, gpGlobals->curtime, "RespawnThinkContext" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFPowerup::DropSingleInstance( const Vector &vecVelocity, CBaseCombatCharacter *pOwner, float flOwnerPickupDelay, float flRestTime, float flRemoveTime /*= 30.0f*/ )
{
	SetOwnerEntity( pOwner );
	AddSpawnFlags( SF_NORESPAWN );
	m_bDropped = true;
	DispatchSpawn( this );

	SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_BOUNCE );
	SetAbsVelocity( vecVelocity );
	SetSolid( SOLID_BBOX );

	if ( flRestTime != 0.0f )
		ActivateWhenAtRest( flRestTime );

	m_flOwnerPickupEnableTime = gpGlobals->curtime + flOwnerPickupDelay;

	// Remove after 30 seconds.
	SetContextThink( &CBaseEntity::SUB_Remove, gpGlobals->curtime + flRemoveTime, "PowerupRemoveThink" );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFPowerup::InputEnable( inputdata_t &inputdata )
{
	SetDisabled( false );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFPowerup::InputDisable( inputdata_t &inputdata )
{
	SetDisabled( true );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFPowerup::InputEnableWithEffect( inputdata_t &inputdata )
{
	DispatchParticleEffect( "ExplosionCore_buildings", GetAbsOrigin(), vec3_angle );
	SetDisabled( false );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFPowerup::InputDisableWithEffect( inputdata_t &inputdata )
{
	DispatchParticleEffect( "ExplosionCore_buildings", GetAbsOrigin(), vec3_angle );
	SetDisabled( true );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CTFPowerup::IsDisabled( void )
{
	return m_bDisabled;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFPowerup::InputToggle( inputdata_t &inputdata )
{
	if ( m_bDisabled )
	{
		SetDisabled( false );
	}
	else
	{
		SetDisabled( true );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFPowerup::SetDisabled( bool bDisabled )
{
	m_bDisabled = bDisabled;

	if ( m_bDisabled )
	{
		AddEffects( EF_NODRAW );
		SetContextThink( NULL, 0, "RespawnThinkContext" );
	}
	else
	{
		RemoveEffects( EF_NODRAW );
		
		if ( m_bRespawning )
		{
			HideOnPickedUp();
			m_bFire15SecRemain = ( m_flRespawnTime - gpGlobals->curtime >= 15.0f );
			SetContextThink( &CTFPowerup::RespawnThink, gpGlobals->curtime, "RespawnThinkContext" );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFPowerup::InputRespawnNow( inputdata_t &inputdata )
{
	if ( m_bRespawning )
	{
		Materialize();
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFPowerup::FireOutputsOnPickup( CBasePlayer *pPlayer )
{
	if ( TFGameRules()->IsTeamplay() )
	{
		switch ( pPlayer->GetTeamNumber() )
		{
		case TF_TEAM_RED:
			m_outputOnTeam1Touch.FireOutput( pPlayer, this );
			break;
		case TF_TEAM_BLUE:
			m_outputOnTeam2Touch.FireOutput( pPlayer, this );
			break;
		case TF_TEAM_GREEN:
			m_outputOnTeam3Touch.FireOutput( pPlayer, this );
			break;
		case TF_TEAM_YELLOW:
			m_outputOnTeam4Touch.FireOutput( pPlayer, this );
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CTFPowerup::ValidateForGameType( int  iType )
{
	if ( m_bDropped )
		return true;

	if ( iType && ( !m_bEnabledModes[iType] || TFGameRules()->IsInstagib() ) )
	{
		UTIL_Remove( this );
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFPowerup::UpdatePowerupsForGameType( int  iType )
{
	// Run through all pickups and update their status based on the selected gamemode.
	for( int i = 0; i < ITFPowerupAutoList::AutoList().Count(); ++i )
	{
		CTFPowerup *pPowerup = static_cast< CTFPowerup * >( ITFPowerupAutoList::AutoList()[i] );
		if( !pPowerup )
			continue;

		pPowerup->ValidateForGameType( iType );
	}
}
