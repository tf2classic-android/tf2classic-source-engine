// tf_bot_deathmatch.cpp
// Run. Think. Shoot. Live
// SanyaSho, February 2024
// IDA Pro 8.3 + TF2C 2017
// Used for TF2C Android port

#include "cbase.h"
#include "tf_player.h"
#include "tf_gamerules.h"
#include "team_control_point_master.h"
#include "bot/tf_bot.h"
#include "bot/behavior/tf_bot_attack.h"
#include "bot/behavior/scenario/deathmatch/tf_bot_deathmatch.h"
#include "nav_mesh.h"
#include "entity_weaponspawn.h"
#include "tf_basedmpowerup.h"
#include "tf_dropped_weapon.h"

ConVar tf_bot_debug_deathmatch( "tf_bot_debug_deathmatch", "0", FCVAR_CHEAT );

CTFBotDeathmatch::CTFBotDeathmatch()
{
	m_ctRecomputePath.Start( -1.f );
}

ActionResult< CTFBot > CTFBotDeathmatch::OnStart( CTFBot *me, Action< CTFBot > *priorAction )
{
	m_PathFollower.SetMinLookAheadDistance( me->GetDesiredPathLookAheadRange() );

	RecomputeSeekPath( me );

	return Continue();
}

ActionResult< CTFBot > CTFBotDeathmatch::Update( CTFBot *me, float interval )
{
	const CKnownEntity *threat = me->GetVisionInterface()->GetPrimaryKnownThreat();
	if( threat )
	{
		CBaseEntity *pEntity = threat->GetEntity();
		if( pEntity && tf_bot_debug_deathmatch.GetBool() )
		{
			float flRangeTo = me->GetRangeTo( pEntity );

			DevMsg( "CTFBotDeathmatch: primary known threat #%d \"%s\" @ dist %.0f\n", pEntity->entindex(), pEntity->GetClassname(), flRangeTo );
		}

		if( me->IsRangeLessThan( threat->GetLastKnownPosition(), 1000.0f ) )
		{
			return SuspendFor( new CTFBotAttack, "Going after an enemy" );
		}
	}

	// SanyaSho: always try to change our weapon (weapon spawner only)
	me->PressUseButton( 0.2f );

	// move towards our seek goal
	m_PathFollower.Update( me );

	if( !m_PathFollower.IsValid() && m_ctRecomputePath.IsElapsed() )
	{
		m_ctRecomputePath.Start( 1.0f );

		RecomputeSeekPath( me );
	}

	return Continue();
}

ActionResult< CTFBot > CTFBotDeathmatch::OnResume( CTFBot *me, Action< CTFBot > *interruptingAction )
{
	RecomputeSeekPath( me );

	return Continue();
}

EventDesiredResult< CTFBot > CTFBotDeathmatch::OnStuck( CTFBot *me )
{
	RecomputeSeekPath( me );

	return TryContinue();
}

EventDesiredResult< CTFBot > CTFBotDeathmatch::OnMoveToSuccess( CTFBot *me, const Path *path )
{
	RecomputeSeekPath( me );

	return TryContinue();
}

EventDesiredResult< CTFBot > CTFBotDeathmatch::OnMoveToFailure( CTFBot *me, const Path *path, MoveToFailureType reason )
{
	RecomputeSeekPath( me );

	return TryContinue();
}

QueryResultType	CTFBotDeathmatch::ShouldRetreat( const INextBot *meBot ) const
{
	return ANSWER_UNDEFINED;
}

QueryResultType CTFBotDeathmatch::ShouldHurry( const INextBot *me ) const
{
	return ANSWER_UNDEFINED;
}

CBaseEntity *CTFBotDeathmatch::ChooseGoalEntity( CTFBot *me )
{
	CUtlVector< CBaseEntity * > goalVector;

	// Find weaponspawners
	FIND_PICKUP_AND_ADD_TO_LIST( CWeaponSpawner, IWeaponSpawnerAutoList, goalVector );

	// Find powerups
	FIND_PICKUP_AND_ADD_TO_LIST( CTFBaseDMPowerup, ITFBaseDMPowerupAutoList, goalVector );

	// NEW: Find some dropped weapons on floor
	FIND_OBJECT_AND_ADD_TO_LIST_NO_CHECK( CTFDroppedWeapon, ITFDroppedWeaponAutoList, goalVector );

	if( tf_bot_debug_deathmatch.GetBool() )
	{
		FOR_EACH_VEC( goalVector, i )
		{
			TheNavMesh->AddToSelectedSet( TheNavMesh->GetNearestNavArea( goalVector[i]->WorldSpaceCenter() ) );
		}
	}

	// pick a new goal
	if( goalVector.Count() > 0 )
	{
		return goalVector[ RandomInt( 0, goalVector.Count()-1 ) ];
	}

	return NULL;
}

void CTFBotDeathmatch::RecomputeSeekPath( CTFBot *me )
{
	m_goalEntity = ChooseGoalEntity( me );
	if( m_goalEntity )
	{
		CTFBotPathCost cost( me, SAFEST_ROUTE );
		m_PathFollower.Compute( me, m_goalEntity->WorldSpaceCenter(), cost );
	}
	else
	{
		m_PathFollower.Invalidate();
	}
}

EventDesiredResult< CTFBot > CTFBotDeathmatch::OnTerritoryContested( CTFBot *me, int territoryID )
{
	return TryDone( RESULT_IMPORTANT, "Defending the point" );
}

EventDesiredResult< CTFBot > CTFBotDeathmatch::OnTerritoryCaptured( CTFBot *me, int territoryID )
{
	return TryDone( RESULT_IMPORTANT, "Giving up due to point capture" );
}

EventDesiredResult< CTFBot > CTFBotDeathmatch::OnTerritoryLost( CTFBot *me, int territoryID )
{
	return TryDone( RESULT_IMPORTANT, "Giving up due to point lost" );
}
