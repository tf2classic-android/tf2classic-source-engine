//========= Copyright Valve Corporation, All rights reserved. ============//
// tf_bot_engineer_build_teleport_entrance.cpp
// Engineer building a teleport entrance right outside of the spawn room
// Michael Booth, May 2009
// Addenums by the TF2Classic team

#include "cbase.h"
#include "nav_mesh.h"
#include "tf_player.h"
#include "tf_obj.h"
#include "tf_weapon_builder.h"
#include "bot/tf_bot.h"
#include "bot/behavior/engineer/tf_bot_engineer_build_teleport_entrance.h"
#include "bot/behavior/engineer/tf_bot_engineer_move_to_build.h"
#include "bot/behavior/tf_bot_get_ammo.h"
#include "func_respawnroom.h"

extern ConVar tf_bot_path_lookahead_range;

ConVar tf_bot_max_teleport_entrance_travel( "tf_bot_max_teleport_entrance_travel", "2000", FCVAR_CHEAT, "Don't plant teleport entrances farther than this travel distance from our spawn room" );
ConVar tf_bot_teleport_build_surface_normal_limit( "tf_bot_teleport_build_surface_normal_limit", "0.99", FCVAR_CHEAT, "If the ground normal Z component is less that this value, Engineer bots won't place their entrance teleporter" );
ConVar tf_bot_debug_teleporter("tf_bot_debug_teleporter", "0", FCVAR_CHEAT );

//---------------------------------------------------------------------------------------------
ActionResult< CTFBot >	CTFBotEngineerBuildTeleportEntrance::OnStart( CTFBot *me, Action< CTFBot > *priorAction )
{
	return Continue();
}


//---------------------------------------------------------------------------------------------
ActionResult< CTFBot >	CTFBotEngineerBuildTeleportEntrance::Update( CTFBot *me, float interval )
{
	// Wait until waiting for players or freezetime ends
	if( TFGameRules()->IsInWaitingForPlayers() || TFGameRules()->State_Get() == GR_STATE_PREROUND )
	{
		return Continue();
	}

	CTFNavArea *myArea = me->GetLastKnownArea();

	CBaseObject *myTeleportEntrance = me->GetObjectOfType( OBJ_TELEPORTER, MODE_TELEPORTER_ENTRANCE );
	if ( myTeleportEntrance )
	{
		return ChangeTo( new CTFBotEngineerMoveToBuild, "Teleport entrance built" );
	}

	if ( !myArea )
	{
		return ChangeTo( new CTFBotEngineerMoveToBuild(), "No nav mesh for teleporter entrance!" );
	}

	// Seems to fail always fail
	/*if( myArea->GetIncursionDistance(me->GetTeamNumber()) > tf_bot_max_teleport_entrance_travel.GetFloat() )
	{
		return ChangeTo( new CTFBotEngineerMoveToBuild, "Too far from our spawn room to build teleporter entrance" );
	}*/

	// make sure we go back to our resupply cabinet after planting the teleporter entrance before we move on
	if ( !me->IsAmmoFull() && CTFBotGetAmmo::IsPossible( me ) )
	{
		return SuspendFor( new CTFBotGetAmmo, "Refilling ammo" );
	}

	CUtlVector<CTFNavArea *> near_exit_areas;

	if ( near_exit_areas.IsEmpty() )
	{
		// collect all areas covering spawn exits
		TheTFNavMesh()->CollectSpawnRoomThresholdAreas( &near_exit_areas, me->GetTeamNumber() );

		if ( near_exit_areas.IsEmpty() )
		{
			return ChangeTo( new CTFBotEngineerMoveToBuild(), "No spawn areas for teleporter entrance!" );
		}

		// expand search to areas *near* spawn exits
		for ( CTFNavArea *area : near_exit_areas )
		{
			if ( !me->GetLocomotionInterface()->IsAreaTraversable( area ) ) continue;

			if ( PointInRespawnRoom( NULL, area->GetCenter() ) ) continue;

			if( tf_bot_debug_teleporter.GetBool() )
			{
				area->DrawFilled( 168, 168, 168, 255, 4.0f );
			}
		}

		// shuffle the vector so that we can iterate over it sequentially but still effectively make a random choice
		near_exit_areas.Shuffle();
	}

	if ( !m_path.IsValid() )
	{
		if ( !near_exit_areas.IsEmpty() )
		{
			Vector goal = near_exit_areas.Random()->GetCenter();
			m_path.Compute( me, goal, CTFBotPathCost( me, DEFAULT_ROUTE ) );

			if ( tf_bot_debug_teleporter.GetBool() )
			{
				NDebugOverlay::Circle( goal, QAngle(-90.0f, 0.0f, 0.0f), 16.0f, 255,255, 255, 255, true, 4.0f );
			}
		}
	}

	m_path.Update( me );

	// build
	CTFWeaponBase *myGun = me->GetActiveTFWeapon();
	if ( myGun )
	{
		CTFWeaponBuilder *builder = dynamic_cast< CTFWeaponBuilder * >( myGun );
		if ( builder )
		{
			// don't build on slopes - causes player blockages
			Vector forward;
			me->EyeVectors( &forward );

			const float placementRange = 30.0f;
			forward *= placementRange;

			trace_t result;
			UTIL_TraceLine( me->WorldSpaceCenter() + Vector( forward.x, forward.y, 0.0f ), me->WorldSpaceCenter() + Vector( forward.x, forward.y, -200.0f ), MASK_PLAYERSOLID, me, COLLISION_GROUP_NONE, &result );

			if ( builder->IsValidPlacement() && result.DidHit() && result.plane.normal.z > tf_bot_teleport_build_surface_normal_limit.GetFloat() )
			{
				// place it down
				me->PressFireButton();
			}
		}
		else
		{
			// switch to teleporter builder
			me->StartBuildingObjectOfType( OBJ_TELEPORTER, MODE_TELEPORTER_ENTRANCE );
		}
	}

	return Continue();
}


//---------------------------------------------------------------------------------------------
EventDesiredResult< CTFBot > CTFBotEngineerBuildTeleportEntrance::OnStuck( CTFBot *me )
{
	m_path.Invalidate();

	return TryContinue();
}
