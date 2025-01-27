#=============================================================================
#	server_cstrike.cmake
#
#	Project Script
#=============================================================================

include( "${CMAKE_CURRENT_LIST_DIR}/server_base.cmake" )
include( "${CMAKE_CURRENT_LIST_DIR}/nav_mesh.cmake" )
include( "${SRCDIR}/cmake/misc/vpc_groups.cmake" )

set( SERVER_CSTRIKE_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	SERVER_CSTRIKE_SOURCE_FILES

	#$Folder	"Source Files"
	#{
		#-"${SERVER_CSTRIKE_DIR}/AI_ConCommands.cpp"
		"${SERVER_CSTRIKE_DIR}/hl2/Func_Monitor.cpp"
		"${SERVER_CSTRIKE_DIR}/h_cycler.cpp"
		"${SERVER_CSTRIKE_DIR}/h_cycler.h"
		"${SRCDIR}/game/shared/predicted_viewmodel.cpp"
		"${SRCDIR}/game/shared/predicted_viewmodel.h"
		"${SRCDIR}/game/shared/hl2/survival_gamerules.cpp"
		"${SERVER_CSTRIKE_DIR}/team_objectiveresource.cpp"
		"${SERVER_CSTRIKE_DIR}/team_objectiveresource.h"
		"${SERVER_CSTRIKE_DIR}/team_spawnpoint.cpp"
		"${SERVER_CSTRIKE_DIR}/team_spawnpoint.h"
		"${SRCDIR}/game/shared/teamplayroundbased_gamerules.cpp"

		#$Folder	"CounterStrike DLL"
		#{
			"${SRCDIR}/game/shared/cstrike/cs_achievement_constants.h"
			"${SRCDIR}/game/shared/cstrike/achievements_cs.cpp"
			"${SRCDIR}/game/shared/cstrike/achievements_cs.h"
			"${SRCDIR}/game/shared/cstrike/basecsgrenade_projectile.cpp"
			"${SRCDIR}/game/shared/cstrike/basecsgrenade_projectile.h"
			"${SRCDIR}/game/shared/cstrike/cs_ammodef.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_ammodef.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_autobuy.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_autobuy.h"
			"${SRCDIR}/game/shared/cstrike/cs_blackmarket.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_client.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_client.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_eventlog.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_gameinterface.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_gamemovement.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_gamerules.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_gamerules.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_gamestats.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_gamestats.h"			
			"${SRCDIR}/game/shared/cstrike/cs_gamestats_shared.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_gamestats_shared.h"
			"${SRCDIR}/game/shared/steamworks_gamestats.cpp"
			"${SRCDIR}/game/shared/steamworks_gamestats.h"			
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_hltvdirector.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_player.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_player.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_player_resource.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_player_resource.h"
			"${SRCDIR}/game/shared/cstrike/cs_player_shared.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_playeranimstate.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_playeranimstate.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_playermove.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_shareddefs.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_shareddefs.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_team.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_team.h"
			"${SRCDIR}/game/shared/cstrike/cs_usermessages.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/cs_vehicle_jeep.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_weapon_parse.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_weapon_parse.h"
			"${SRCDIR}/game/shared/cstrike/flashbang_projectile.cpp"
			"${SRCDIR}/game/shared/cstrike/flashbang_projectile.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/func_bomb_target.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/func_bomb_target.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/func_buy_zone.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/func_hostage_rescue.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/funfact_cs.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/funfact_cs.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/funfactmgr_cs.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/funfactmgr_cs.h"
			"${SRCDIR}/game/shared/cstrike/fx_cs_shared.cpp"
			"${SRCDIR}/game/shared/cstrike/fx_cs_shared.h"
			"${SRCDIR}/game/shared/cstrike/hegrenade_projectile.cpp"
			"${SRCDIR}/game/shared/cstrike/hegrenade_projectile.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/info_view_parameters.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/item_ammo.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/item_assaultsuit.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/item_defuser.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/item_kevlar.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/item_nvgs.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/mapinfo.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/mapinfo.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/point_surroundtest.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/smokegrenade_projectile.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/smokegrenade_projectile.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/te_radioicon.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/te_shotgun_shot.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/te_shotgun_shot.h"
			"${SERVER_CSTRIKE_DIR}/cstrike/holiday_gift.cpp"
			"${SERVER_CSTRIKE_DIR}/cstrike/holiday_gift.h"
			
			"${SRCDIR}/game/shared/cstrike/cs_urlretrieveprices.cpp"
			#{
			#	$Configuration
			#	{
			#		$Compiler
			#		{
			#			$Create/UsePrecompiledHeader	"Not Using Precompiled Headers"
			#		}
			#	}
			#}

			"${SRCDIR}/game/shared/cstrike/cs_urlretrieveprices.h"
			
			#$Folder	"Weapon"
			#{
				"${SRCDIR}/game/shared/cstrike/weapon_ak47.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_aug.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_awp.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_basecsgrenade.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_basecsgrenade.h"
				"${SRCDIR}/game/shared/cstrike/weapon_c4.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_c4.h"
				"${SRCDIR}/game/shared/cstrike/weapon_csbase.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_csbase.h"
				"${SRCDIR}/game/shared/cstrike/weapon_csbasegun.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_csbasegun.h"
				"${SRCDIR}/game/shared/cstrike/weapon_deagle.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_elite.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_famas.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_fiveseven.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_flashbang.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_flashbang.h"
				"${SRCDIR}/game/shared/cstrike/weapon_g3sg1.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_galil.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_glock.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_hegrenade.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_hegrenade.h"
				"${SRCDIR}/game/shared/cstrike/weapon_knife.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_m249.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_m3.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_m4a1.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_mac10.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_mp5navy.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_p228.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_p90.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_scout.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_sg550.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_sg552.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_smokegrenade.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_tmp.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_ump45.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_usp.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_xm1014.cpp"
			#}
			
			#$Folder	"Bot"
			#{
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot.h"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_chatter.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_chatter.h"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_event.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_event_bomb.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_event_player.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_event_weapon.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_init.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_listen.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_manager.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_manager.h"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_nav.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_pathfind.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_radio.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_statemachine.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_update.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_vision.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_weapon.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_bot_weapon_id.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_gamestate.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/bot/cs_gamestate.h"

				#$Folder	"States"
				#{
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_attack.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_buy.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_defuse_bomb.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_escape_from_bomb.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_fetch_bomb.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_follow.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_hide.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_hunt.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_idle.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_investigate_noise.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_move_to.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_open_door.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_plant_bomb.cpp"
					"${SERVER_CSTRIKE_DIR}/cstrike/bot/states/cs_bot_use_entity.cpp"
				#}

				#$Folder	"Shared"
				#{
					"${SRCDIR}/game/shared/cstrike/bot/bot.cpp"
					"${SRCDIR}/game/shared/cstrike/bot/bot.h"
					"${SRCDIR}/game/shared/cstrike/bot/bot_constants.h"
					"${SRCDIR}/game/shared/cstrike/bot/bot_hide.cpp"
					"${SRCDIR}/game/shared/cstrike/bot/bot_manager.cpp"
					"${SRCDIR}/game/shared/cstrike/bot/bot_manager.h"
					"${SRCDIR}/game/shared/cstrike/bot/bot_profile.cpp"
					"${SRCDIR}/game/shared/cstrike/bot/bot_profile.h"
					"${SRCDIR}/game/shared/cstrike/bot/bot_util.cpp"
					"${SRCDIR}/game/shared/cstrike/bot/bot_util.h"
					"${SRCDIR}/game/shared/cstrike/bot/shared_util.cpp"
					"${SRCDIR}/game/shared/cstrike/bot/shared_util.h"
				#}
			#}

			#$Folder	"Hostage"
			#{
				"${SERVER_CSTRIKE_DIR}/cstrike/hostage/cs_simple_hostage.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/hostage/cs_simple_hostage.h"
			#}
			
			#$Folder	"Navigation Mesh"
			#{
				"${SERVER_CSTRIKE_DIR}/cstrike/cs_nav.h"
				"${SERVER_CSTRIKE_DIR}/cstrike/cs_nav_area.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/cs_nav_area.h"
				"${SERVER_CSTRIKE_DIR}/cstrike/cs_nav_generate.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/cs_nav_mesh.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/cs_nav_mesh.h"
				"${SERVER_CSTRIKE_DIR}/cstrike/cs_nav_pathfind.h"
				"${SERVER_CSTRIKE_DIR}/cstrike/cs_nav_path.cpp"
				"${SERVER_CSTRIKE_DIR}/cstrike/cs_nav_path.h"
			#}
		#}

		#$Folder "NextBot"
		#{
			"${SERVER_CSTRIKE_DIR}/NextBot/NextBot.cpp"
			"${SERVER_CSTRIKE_DIR}/NextBot/NextBot.h"
			"${SERVER_CSTRIKE_DIR}/NextBot/NextBotBehavior.h"
			"${SERVER_CSTRIKE_DIR}/NextBot/NextBotManager.cpp"
			"${SERVER_CSTRIKE_DIR}/NextBot/NextBotManager.h"
			"${SERVER_CSTRIKE_DIR}/NextBot/NextBotUtil.h"
			"${SERVER_CSTRIKE_DIR}/NextBot/NextBotKnownEntity.h"
			"${SERVER_CSTRIKE_DIR}/NextBot/NextBotGroundLocomotion.cpp"
			"${SERVER_CSTRIKE_DIR}/NextBot/NextBotGroundLocomotion.h"
			"${SERVER_CSTRIKE_DIR}/NextBot/simple_bot.cpp"
			"${SERVER_CSTRIKE_DIR}/NextBot/simple_bot.h"

			#$Folder "NextBotInterfaces"
			#{
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotBodyInterface.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotBodyInterface.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotComponentInterface.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotComponentInterface.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotEventResponderInterface.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotHearingInterface.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotIntentionInterface.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotIntentionInterface.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotInterface.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotInterface.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotLocomotionInterface.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotLocomotionInterface.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotVisionInterface.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotVisionInterface.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/NextBotContextualQueryInterface.h"
			#}

			#$Folder "NextBotPath"
			#{
				"${SERVER_CSTRIKE_DIR}/NextBot/Path/NextBotChasePath.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/Path/NextBotChasePath.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/Path/NextBotRetreatPath.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/Path/NextBotPath.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/Path/NextBotPath.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/Path/NextBotPathFollow.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/Path/NextBotPathFollow.h"
			#}
			
			#$Folder "NextBotPlayer"
			#{
				"${SERVER_CSTRIKE_DIR}/NextBot/Player/NextBotPlayerBody.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/Player/NextBotPlayerBody.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/Player/NextBotPlayerLocomotion.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/Player/NextBotPlayerLocomotion.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/Player/NextBotPlayer.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/Player/NextBotPlayer.h"
			#}

			#$Folder "NextBotCommonBehaviors"
			#{
				"${SERVER_CSTRIKE_DIR}/NextBot/Behavior/BehaviorBackUp.h"
				"${SERVER_CSTRIKE_DIR}/NextBot/Behavior/BehaviorMoveTo.h"
			#}

			#$Folder "NextBotNavMeshEntities"
			#{
				"${SERVER_CSTRIKE_DIR}/NextBot/NavMeshEntities/func_nav_prerequisite.cpp"
				"${SERVER_CSTRIKE_DIR}/NextBot/NavMeshEntities/func_nav_prerequisite.h"
			#}
		#}
	#}
)

set(
	SERVER_CSTRIKE_EXCLUDE_SOURCES
	"${SERVER_CSTRIKE_DIR}/AI_ConCommands.cpp"
)

add_library(
	server_cstrike MODULE
	${SERVER_CSTRIKE_SOURCE_FILES}
)

if( ${IS_WINDOWS} )
target_group_sources(
	server_cstrike
)
endif()

set_install_properties(
	server_cstrike
	""
	"server"
	"${GAMEDIR}/${BUILD_FOLDER}/bin"
)

target_include_directories(
	server_cstrike PRIVATE

	"${SRCDIR}/game/shared/cstrike/control"
	"${SRCDIR}/game/shared/cstrike"
	"${SERVER_CSTRIKE_DIR}/cstrike"
	"${SERVER_CSTRIKE_DIR}/cstrike/bot"
	"${SERVER_CSTRIKE_DIR}/cstrike/bot/states"
	"${SRCDIR}/game/shared/cstrike/bot"
	"${SERVER_CSTRIKE_DIR}/cstrike/hostage"
	"${SERVER_CSTRIKE_DIR}/cstrike/control"
)

target_compile_definitions(
	server_cstrike PRIVATE

	BOTS
	CSTRIKE_DLL
	NEXT_BOT
)

target_link_libraries(
	server_cstrike PRIVATE

	"$<${IS_WINDOWS}:ws2_32>"
)

target_use_server_base(
	server_cstrike
	SERVER_CSTRIKE_EXCLUDE_SOURCES
)

target_use_nav_mesh(
	server_cstrike
)
