#=============================================================================
#	server_tf2classic.cmake
#
#	Project Script
#=============================================================================

include( "${CMAKE_CURRENT_LIST_DIR}/server_base.cmake" )
include( "${CMAKE_CURRENT_LIST_DIR}/nav_mesh.cmake" )
include( "${SRCDIR}/cmake/misc/vpc_groups.cmake" )

set( SERVER_TF2CLASSIC_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	SERVER_TF2CLASSIC_SOURCE_FILES

	#$Folder	"Source Files"
	#{
		#$Folder "NextBot"
		#{
			"${SERVER_BASE_DIR}/NextBot/NextBot.cpp"
			"${SERVER_BASE_DIR}/NextBot/NextBot.h"
			"${SERVER_BASE_DIR}/NextBot/NextBotBehavior.h"
			"${SERVER_BASE_DIR}/NextBot/NextBotManager.cpp"
			"${SERVER_BASE_DIR}/NextBot/NextBotManager.h"
			"${SERVER_BASE_DIR}/NextBot/NextBotUtil.h"
			"${SERVER_BASE_DIR}/NextBot/NextBotKnownEntity.h"
			"${SERVER_BASE_DIR}/NextBot/NextBotGroundLocomotion.cpp"
			"${SERVER_BASE_DIR}/NextBot/NextBotGroundLocomotion.h"
			"${SERVER_BASE_DIR}/NextBot/simple_bot.cpp"
			"${SERVER_BASE_DIR}/NextBot/simple_bot.h"

			#$Folder "NextBotInterfaces"
			#{
				"${SERVER_BASE_DIR}/NextBot/NextBotBodyInterface.cpp"
				"${SERVER_BASE_DIR}/NextBot/NextBotBodyInterface.h"
				"${SERVER_BASE_DIR}/NextBot/NextBotComponentInterface.cpp"
				"${SERVER_BASE_DIR}/NextBot/NextBotComponentInterface.h"
				"${SERVER_BASE_DIR}/NextBot/NextBotEventResponderInterface.h"
				"${SERVER_BASE_DIR}/NextBot/NextBotHearingInterface.h"
				"${SERVER_BASE_DIR}/NextBot/NextBotIntentionInterface.cpp"
				"${SERVER_BASE_DIR}/NextBot/NextBotIntentionInterface.h"
				"${SERVER_BASE_DIR}/NextBot/NextBotInterface.cpp"
				"${SERVER_BASE_DIR}/NextBot/NextBotInterface.h"
				"${SERVER_BASE_DIR}/NextBot/NextBotLocomotionInterface.cpp"
				"${SERVER_BASE_DIR}/NextBot/NextBotLocomotionInterface.h"
				"${SERVER_BASE_DIR}/NextBot/NextBotVisionInterface.cpp"
				"${SERVER_BASE_DIR}/NextBot/NextBotVisionInterface.h"
				"${SERVER_BASE_DIR}/NextBot/NextBotContextualQueryInterface.h"
			#}

			#$Folder "NextBotPath"
			#{
				"${SERVER_BASE_DIR}/NextBot/Path/NextBotChasePath.cpp"
				"${SERVER_BASE_DIR}/NextBot/Path/NextBotChasePath.h"
				"${SERVER_BASE_DIR}/NextBot/Path/NextBotRetreatPath.h"
				"${SERVER_BASE_DIR}/NextBot/Path/NextBotPath.cpp"
				"${SERVER_BASE_DIR}/NextBot/Path/NextBotPath.h"
				"${SERVER_BASE_DIR}/NextBot/Path/NextBotPathFollow.cpp"
				"${SERVER_BASE_DIR}/NextBot/Path/NextBotPathFollow.h"
			#}

			#$Folder "NextBotPlayer"
			#{
				"${SERVER_BASE_DIR}/NextBot/Player/NextBotPlayerBody.cpp"
				"${SERVER_BASE_DIR}/NextBot/Player/NextBotPlayerBody.h"
				"${SERVER_BASE_DIR}/NextBot/Player/NextBotPlayerLocomotion.cpp"
				"${SERVER_BASE_DIR}/NextBot/Player/NextBotPlayerLocomotion.h"
				"${SERVER_BASE_DIR}/NextBot/Player/NextBotPlayer.cpp"
				"${SERVER_BASE_DIR}/NextBot/Player/NextBotPlayer.h"
			#}

			#$Folder "NextBotCommonBehaviors"
			#{
				"${SERVER_BASE_DIR}/NextBot/Behavior/BehaviorBackUp.h"
				"${SERVER_BASE_DIR}/NextBot/Behavior/BehaviorMoveTo.h"
			#}

			#$Folder "NextBotNavMeshEntities"
			#{
				"${SERVER_BASE_DIR}/NextBot/NavMeshEntities/func_nav_prerequisite.cpp"
				"${SERVER_BASE_DIR}/NextBot/NavMeshEntities/func_nav_prerequisite.h"
			#}
		#}

		"${SERVER_BASE_DIR}/env_projectedtexture.h"
		"${SERVER_BASE_DIR}/ai_relationship.cpp"
		"${SERVER_BASE_DIR}/basegrenade_concussion.cpp"
		"${SERVER_BASE_DIR}/basegrenade_contact.cpp"
		"${SERVER_BASE_DIR}/basegrenade_timed.cpp"
		"${SERVER_BASE_DIR}/EntityFlame.h"
		"${SERVER_BASE_DIR}/hl2/Func_Monitor.cpp"
		"${SERVER_BASE_DIR}/grenadethrown.cpp"
		"${SERVER_BASE_DIR}/grenadethrown.h"
		"${SERVER_BASE_DIR}/monstermaker.cpp"
		"${SERVER_BASE_DIR}/monstermaker.h"
		"${SERVER_BASE_DIR}/physics_bone_follower.h"
		"${SHARED_BASE_DIR}/ragdoll_shared.h"
		"${SHARED_BASE_DIR}/solidsetdefaults.h"
		"${SHARED_BASE_DIR}/hl2/survival_gamerules.cpp"
		"${SERVER_BASE_DIR}/team_objectiveresource.cpp"
		"${SERVER_BASE_DIR}/team_objectiveresource.h"
		"${SERVER_BASE_DIR}/team_control_point.cpp"
		"${SERVER_BASE_DIR}/team_control_point.h"
		"${SERVER_BASE_DIR}/team_control_point_master.cpp"
		"${SERVER_BASE_DIR}/team_control_point_master.h"
		"${SERVER_BASE_DIR}/team_control_point_round.cpp"
		"${SERVER_BASE_DIR}/team_control_point_round.h"
		"${SERVER_BASE_DIR}/team_train_watcher.cpp"
		"${SERVER_BASE_DIR}/team_train_watcher.h"
		"${SHARED_BASE_DIR}/teamplayroundbased_gamerules.cpp"
		"${SHARED_BASE_DIR}/touchlink.h"
		"${SERVER_BASE_DIR}/trigger_area_capture.cpp"
		"${SERVER_BASE_DIR}/trigger_area_capture.h"
		"${SHARED_BASE_DIR}/teamplay_round_timer.cpp"
		"${SHARED_BASE_DIR}/teamplay_round_timer.h"
		
		"${SHARED_BASE_DIR}/playerclass_info_parse.cpp"
		"${SHARED_BASE_DIR}/playerclass_info_parse.h"
		
		#$Folder	"Multiplayer"
		#{
			"${SHARED_BASE_DIR}/Multiplayer/multiplayer_animstate.cpp"
			"${SHARED_BASE_DIR}/Multiplayer/multiplayer_animstate.h"
		#}
		
		#$Folder	"Econ"
		#{
			"${SHARED_BASE_DIR}/econ/attribute_manager.cpp"
			"${SHARED_BASE_DIR}/econ/attribute_manager.h"
			"${SHARED_BASE_DIR}/econ/econ_entity.cpp"
			"${SHARED_BASE_DIR}/econ/econ_entity.h"
			"${SHARED_BASE_DIR}/econ/econ_item_schema.cpp"
			"${SHARED_BASE_DIR}/econ/econ_item_schema.h"
			"${SHARED_BASE_DIR}/econ/econ_item_system.cpp"
			"${SHARED_BASE_DIR}/econ/econ_item_system.h"
			"${SHARED_BASE_DIR}/econ/econ_wearable.cpp"
			"${SHARED_BASE_DIR}/econ/econ_wearable.h"
			"${SHARED_BASE_DIR}/econ/econ_item_view.cpp"
			"${SHARED_BASE_DIR}/econ/econ_item_view.h"
			"${SHARED_BASE_DIR}/econ/game_item_schema.h"
			"${SHARED_BASE_DIR}/econ/ihasattributes.h"
		#}
		
		#$Folder	"TF"
		#{
			"${SHARED_BASE_DIR}/tf/tf_cues_manager.cpp"
			"${SHARED_BASE_DIR}/tf/tf_cues_manager.h"
			"${SHARED_BASE_DIR}/tf/tf_music_controller.cpp"
			"${SHARED_BASE_DIR}/tf/tf_music_controller.h"

			#$Folder	"Bot"
			#{
				#$Folder	"Behavior"
				#{
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_behavior.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_behavior.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_tactical_monitor.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_tactical_monitor.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_scenario_monitor.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_scenario_monitor.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_attack.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_attack.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_seek_and_destroy.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_seek_and_destroy.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_retreat_to_cover.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_retreat_to_cover.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_melee_attack.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_melee_attack.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_approach_object.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_approach_object.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_get_health.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_get_health.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_get_ammo.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_get_ammo.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_dead.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_dead.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_move_to_vantage_point.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_move_to_vantage_point.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_taunt.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_taunt.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_use_teleporter.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_use_teleporter.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_destroy_enemy_sentry.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_destroy_enemy_sentry.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_escort.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_escort.h"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_use_item.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_use_item.h"						
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_mvm_deploy_bomb.cpp"
					"${SERVER_BASE_DIR}/tf/bot/behavior/tf_bot_mvm_deploy_bomb.h"
					
					#$Folder	"Scenario"
					#{
						#$Folder	"CapturePoint"
						#{
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_point/tf_bot_capture_point.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_point/tf_bot_capture_point.h"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_point/tf_bot_defend_point.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_point/tf_bot_defend_point.h"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_point/tf_bot_defend_point_block_capture.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_point/tf_bot_defend_point_block_capture.h"
						#}

						#$Folder	"Payload"
						#{
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/payload/tf_bot_payload_push.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/payload/tf_bot_payload_push.h"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/payload/tf_bot_payload_block.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/payload/tf_bot_payload_block.h"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/payload/tf_bot_payload_guard.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/payload/tf_bot_payload_guard.h"
						#}

						#$Folder	"CaptureTheFlag"
						#{
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_the_flag/tf_bot_fetch_flag.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_the_flag/tf_bot_fetch_flag.h"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_the_flag/tf_bot_deliver_flag.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_the_flag/tf_bot_deliver_flag.h"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_the_flag/tf_bot_escort_flag_carrier.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_the_flag/tf_bot_escort_flag_carrier.h"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_the_flag/tf_bot_attack_flag_defenders.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/capture_the_flag/tf_bot_attack_flag_defenders.h"
						#}

						#$Folder	"Deathmatch"
						#{
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/deathmatch/tf_bot_deathmatch.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/deathmatch/tf_bot_deathmatch.h"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/deathmatch/tf_bot_taunt_deathmatch.cpp"
							"${SERVER_BASE_DIR}/tf/bot/behavior/scenario/deathmatch/tf_bot_taunt_deathmatch.h"
						#}
					#}
					
					#$Folder	"Engineer"
					#{
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_build.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_build.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_move_to_build.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_move_to_build.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_build_teleport_entrance.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_build_teleport_entrance.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_build_teleport_exit.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_build_teleport_exit.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_build_sentrygun.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_build_sentrygun.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_build_dispenser.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_build_dispenser.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_building.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/engineer/tf_bot_engineer_building.h"
					#}

					#$Folder	"Medic"
					#{
						"${SERVER_BASE_DIR}/tf/bot/behavior/medic/tf_bot_medic_heal.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/medic/tf_bot_medic_heal.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/medic/tf_bot_medic_retreat.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/medic/tf_bot_medic_retreat.h"
					#}

					#$Folder	"Sniper"
					#{
						"${SERVER_BASE_DIR}/tf/bot/behavior/sniper/tf_bot_sniper_attack.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/sniper/tf_bot_sniper_attack.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/sniper/tf_bot_sniper_lurk.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/sniper/tf_bot_sniper_lurk.h"
					#}

					#$Folder	"Spy"
					#{
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_infiltrate.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_infiltrate.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_backstab.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_backstab.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_sap.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_sap.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_escape.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_escape.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_attack.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_attack.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_leave_spawn_room.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_leave_spawn_room.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_lurk.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_lurk.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_hide.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/spy/tf_bot_spy_hide.h"
					#}

					#$Folder	"Demoman"
					#{
						"${SERVER_BASE_DIR}/tf/bot/behavior/demoman/tf_bot_prepare_stickybomb_trap.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/demoman/tf_bot_prepare_stickybomb_trap.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/demoman/tf_bot_stickybomb_sentrygun.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/demoman/tf_bot_stickybomb_sentrygun.h"
					#}
					
					#$Folder	"NavEntities"
					#{
						"${SERVER_BASE_DIR}/tf/bot/behavior/nav_entities/tf_bot_nav_ent_move_to.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/nav_entities/tf_bot_nav_ent_move_to.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/nav_entities/tf_bot_nav_ent_wait.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/nav_entities/tf_bot_nav_ent_wait.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/nav_entities/tf_bot_nav_ent_destroy_entity.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/nav_entities/tf_bot_nav_ent_destroy_entity.h"
					#}
					
					#$Folder "Training"
					#{
						"${SERVER_BASE_DIR}/tf/bot/behavior/training/tf_bot_training.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/training/tf_bot_training.h"
					#}
					
					#$Folder "Missions"
					#{
						"${SERVER_BASE_DIR}/tf/bot/behavior/missions/tf_bot_mission_destroy_sentries.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/missions/tf_bot_mission_destroy_sentries.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/missions/tf_bot_mission_reprogrammed.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/missions/tf_bot_mission_reprogrammed.h"
						"${SERVER_BASE_DIR}/tf/bot/behavior/missions/tf_bot_mission_suicide_bomber.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/missions/tf_bot_mission_suicide_bomber.h"
					#}

					#$Folder "Squad"
					#{
						"${SERVER_BASE_DIR}/tf/bot/behavior/squad/tf_bot_escort_squad_leader.cpp"
						"${SERVER_BASE_DIR}/tf/bot/behavior/squad/tf_bot_escort_squad_leader.h"
					#}
				#}
				
				#$Folder "MapEntities"
				#{
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_generator.cpp"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_generator.h"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_proxy.cpp"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_proxy.h"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_hint_engineer_nest.cpp"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_hint_engineer_nest.h"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_hint_sentrygun.cpp"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_hint_sentrygun.h"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_hint_teleporter_exit.cpp"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_hint_teleporter_exit.h"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_hint_entity.cpp"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_hint_entity.h"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_roster.cpp"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_roster.h"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_hint.cpp"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_bot_hint.h"						
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_spawner_boss.cpp"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_spawner_boss.h"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_spawner.cpp"
						"${SERVER_BASE_DIR}/tf/bot/map_entities/tf_spawner.h"
				#}
				
				"${SERVER_BASE_DIR}/tf/bot/tf_bot.cpp"
				"${SERVER_BASE_DIR}/tf/bot/tf_bot.h"
				"${SERVER_BASE_DIR}/tf/bot/tf_bot_manager.cpp"
				"${SERVER_BASE_DIR}/tf/bot/tf_bot_manager.h"
				"${SERVER_BASE_DIR}/tf/bot/tf_bot_vision.cpp"
				"${SERVER_BASE_DIR}/tf/bot/tf_bot_vision.h"
				"${SERVER_BASE_DIR}/tf/bot/tf_bot_body.cpp"
				"${SERVER_BASE_DIR}/tf/bot/tf_bot_body.h"
				"${SERVER_BASE_DIR}/tf/bot/tf_bot_squad.cpp"
				"${SERVER_BASE_DIR}/tf/bot/tf_bot_squad.h"
				"${SERVER_BASE_DIR}/tf/bot/tf_bot_locomotion.cpp"
				"${SERVER_BASE_DIR}/tf/bot/tf_bot_locomotion.h"
			#}
			
			#$Folder "NavMesh"
			#{
				"${SERVER_BASE_DIR}/tf/nav_mesh/tf_nav_mesh.h"
				"${SERVER_BASE_DIR}/tf/nav_mesh/tf_nav_mesh.cpp"
				"${SERVER_BASE_DIR}/tf/nav_mesh/tf_nav_mesh_edit.h"
				"${SERVER_BASE_DIR}/tf/nav_mesh/tf_nav_mesh_edit.cpp"
				"${SERVER_BASE_DIR}/tf/nav_mesh/tf_nav_area.h"
				"${SERVER_BASE_DIR}/tf/nav_mesh/tf_nav_area.cpp"
				"${SERVER_BASE_DIR}/tf/nav_mesh/tf_path_follower.h"
				"${SERVER_BASE_DIR}/tf/nav_mesh/tf_path_follower.cpp"
				"${SERVER_BASE_DIR}/tf/nav_mesh/tf_nav_interface.cpp"
			#}

			#$Folder	"DM Powerups"
			#{
				"${SERVER_BASE_DIR}/tf/tf_basedmpowerup.cpp"
				"${SERVER_BASE_DIR}/tf/tf_basedmpowerup.h"
				"${SERVER_BASE_DIR}/tf/tf_powerup_cloak.cpp"
				"${SERVER_BASE_DIR}/tf/tf_powerup_cloak.h"
				"${SERVER_BASE_DIR}/tf/tf_powerup_critdamage.cpp"
				"${SERVER_BASE_DIR}/tf/tf_powerup_critdamage.h"
				"${SERVER_BASE_DIR}/tf/tf_powerup_custom.cpp"
				"${SERVER_BASE_DIR}/tf/tf_powerup_custom.h"
				"${SERVER_BASE_DIR}/tf/tf_powerup_ragemode.cpp"
				"${SERVER_BASE_DIR}/tf/tf_powerup_ragemode.h"
				"${SERVER_BASE_DIR}/tf/tf_powerup_shield.cpp"
				"${SERVER_BASE_DIR}/tf/tf_powerup_shield.h"
				"${SERVER_BASE_DIR}/tf/tf_powerup_speedboost.cpp"
				"${SERVER_BASE_DIR}/tf/tf_powerup_speedboost.h"
				"${SERVER_BASE_DIR}/tf/tf_powerup_megahealth.cpp"
				"${SERVER_BASE_DIR}/tf/tf_powerup_megahealth.h"
			#}
			"${SERVER_BASE_DIR}/tf/entity_ammopack.cpp"
			"${SERVER_BASE_DIR}/tf/entity_ammopack.h"
			"${SERVER_BASE_DIR}/tf/entity_armor.cpp"
			"${SERVER_BASE_DIR}/tf/entity_armor.h"
			"${SERVER_BASE_DIR}/tf/entity_forcerespawn.cpp"
			"${SERVER_BASE_DIR}/tf/entity_forcerespawn.h"
			"${SERVER_BASE_DIR}/tf/entity_game_text_tf.cpp"
			"${SERVER_BASE_DIR}/tf/entity_healthkit.cpp"
			"${SERVER_BASE_DIR}/tf/entity_healthkit.h"	
			"${SERVER_BASE_DIR}/tf/entity_intermission.cpp"
			"${SERVER_BASE_DIR}/tf/entity_intermission.h"
			"${SERVER_BASE_DIR}/tf/entity_roundwin.cpp"
			"${SERVER_BASE_DIR}/tf/entity_roundwin.h"
			"${SERVER_BASE_DIR}/tf/entity_tfstart.cpp"
			"${SERVER_BASE_DIR}/tf/entity_tfstart.h"
			"${SERVER_BASE_DIR}/tf/entity_weaponspawn.cpp"
			"${SERVER_BASE_DIR}/tf/entity_weaponspawn.h"
			"${SERVER_BASE_DIR}/tf/entity_player_equip.cpp"
			"${SERVER_BASE_DIR}/tf/func_capture_zone.cpp"
			"${SERVER_BASE_DIR}/tf/func_capture_zone.h"	
			"${SERVER_BASE_DIR}/tf/func_changeclass.cpp"
			"${SERVER_BASE_DIR}/tf/func_changeclass.h"
			"${SERVER_BASE_DIR}/tf/func_no_build.cpp"
			"${SERVER_BASE_DIR}/tf/func_no_build.h"
			"${SERVER_BASE_DIR}/tf/func_nogrenades.cpp"
			"${SERVER_BASE_DIR}/tf/func_nogrenades.h"
			"${SERVER_BASE_DIR}/tf/func_regenerate.cpp"
			"${SERVER_BASE_DIR}/tf/func_regenerate.h"
			"${SERVER_BASE_DIR}/tf/func_restock_zone.cpp"
			"${SERVER_BASE_DIR}/tf/func_restock_zone.h"
			"${SERVER_BASE_DIR}/tf/func_respawnroom.cpp"
			"${SERVER_BASE_DIR}/tf/func_respawnroom.h"
			"${SERVER_BASE_DIR}/tf/te_tfblood.cpp"
			"${SERVER_BASE_DIR}/tf/te_tfblood.h"
			"${SERVER_BASE_DIR}/tf/tf_ammo_pack.cpp"
			"${SERVER_BASE_DIR}/tf/tf_ammo_pack.h"
			"${SERVER_BASE_DIR}/tf/tf_bot_temp.cpp"
			"${SERVER_BASE_DIR}/tf/tf_bot_temp.h"	
			"${SERVER_BASE_DIR}/tf/tf_client.cpp"
			"${SERVER_BASE_DIR}/tf/tf_client.h"
			"${SERVER_BASE_DIR}/tf/tf_eventlog.cpp"
			"${SERVER_BASE_DIR}/tf/tf_filters.cpp"
			"${SERVER_BASE_DIR}/tf/tf_fx.cpp"
			"${SERVER_BASE_DIR}/tf/tf_fx.h"
			"${SERVER_BASE_DIR}/tf/tf_gameinterface.cpp"
			"${SERVER_BASE_DIR}/tf/tf_gamestats.cpp"
			"${SERVER_BASE_DIR}/tf/tf_gamestats.h"
			"${SERVER_BASE_DIR}/tf/tf_hltvdirector.cpp"
			"${SERVER_BASE_DIR}/tf/tf_obj.cpp"
			"${SERVER_BASE_DIR}/tf/tf_obj.h"
			"${SERVER_BASE_DIR}/tf/tf_obj_dispenser.cpp"
			"${SERVER_BASE_DIR}/tf/tf_obj_dispenser.h"
			"${SERVER_BASE_DIR}/tf/tf_obj_sapper.cpp"
			"${SERVER_BASE_DIR}/tf/tf_obj_sapper.h"	
			"${SERVER_BASE_DIR}/tf/tf_obj_sentrygun.cpp"
			"${SERVER_BASE_DIR}/tf/tf_obj_sentrygun.h"
			"${SERVER_BASE_DIR}/tf/tf_obj_teleporter.cpp"
			"${SERVER_BASE_DIR}/tf/tf_obj_teleporter.h"
			"${SERVER_BASE_DIR}/tf/tf_objective_resource.cpp"
			"${SERVER_BASE_DIR}/tf/tf_objective_resource.h"
			"${SERVER_BASE_DIR}/tf/tf_player.cpp"
			"${SERVER_BASE_DIR}/tf/tf_player.h"	
			"${SERVER_BASE_DIR}/tf/tf_player_resource.cpp"
			"${SERVER_BASE_DIR}/tf/tf_player_resource.h"
			"${SERVER_BASE_DIR}/tf/tf_playerclass.cpp"
			"${SERVER_BASE_DIR}/tf/tf_playerclass.h"
			"${SERVER_BASE_DIR}/tf/tf_playermove.cpp"
			"${SERVER_BASE_DIR}/tf/tf_powerup.cpp"
			"${SERVER_BASE_DIR}/tf/tf_powerup.h"
			"${SERVER_BASE_DIR}/tf/tf_projectile_rocket.cpp"
			"${SERVER_BASE_DIR}/tf/tf_projectile_rocket.h"	
			"${SERVER_BASE_DIR}/tf/tf_team.cpp"
			"${SERVER_BASE_DIR}/tf/tf_team.h"
			"${SERVER_BASE_DIR}/tf/tf_turret.cpp"
			"${SERVER_BASE_DIR}/tf/tf_turret.h"	
			"${SERVER_BASE_DIR}/tf/tf_voteissues.cpp"
			"${SERVER_BASE_DIR}/tf/tf_voteissues.h"	
			"${SERVER_BASE_DIR}/tf/tf_weapon_builder.cpp"
			"${SERVER_BASE_DIR}/tf/tf_weapon_builder.h"	
			"${SERVER_BASE_DIR}/tf/func_respawnflag.cpp"
			"${SERVER_BASE_DIR}/tf/func_respawnflag.h"	
			"${SERVER_BASE_DIR}/tf/func_flagdetectionzone.cpp"
			"${SERVER_BASE_DIR}/tf/func_flagdetectionzone.h"	
			"${SERVER_BASE_DIR}/tf/tf_vehicle_jeep.cpp"	
			"${SERVER_BASE_DIR}/tf/tf_generic_bomb.cpp"
			"${SERVER_BASE_DIR}/tf/tf_generic_bomb.h"	
			"${SERVER_BASE_DIR}/tf/tf_dropped_weapon.cpp"
			"${SERVER_BASE_DIR}/tf/tf_dropped_weapon.h"	
			"${SERVER_BASE_DIR}/tf/tf_pushentity.cpp"	

			
			#$Folder	"Unused"
			#{
				"${SERVER_BASE_DIR}/tf/entity_grenadepack.cpp"
				"${SERVER_BASE_DIR}/tf/entity_grenadepack.h"
				"${SERVER_BASE_DIR}/tf/tf_mapitems.cpp"
				"${SERVER_BASE_DIR}/tf/tf_mapitems.h"
				"${SERVER_BASE_DIR}/tf/tf_timer.cpp"
				"${SERVER_BASE_DIR}/tf/tf_timer.h"
			#}

			#$Folder	"Shared"
			#{
				"${SHARED_BASE_DIR}/tf/tf_announcer.cpp"
				"${SHARED_BASE_DIR}/tf/tf_announcer.h"
				"${SHARED_BASE_DIR}/tf/tf_merc_customizations.cpp"
				"${SHARED_BASE_DIR}/tf/tf_merc_customizations.h"
				"${SHARED_BASE_DIR}/tf/buildnum.cpp"
				"${SHARED_BASE_DIR}/tf/buildnum.h"

				#$Folder	"Weapons"
				#{	
					#$Folder	"Custom"
					#{
						"${SHARED_BASE_DIR}/tf/tf_projectile_arrow.cpp"
						"${SHARED_BASE_DIR}/tf/tf_projectile_arrow.h"
						"${SHARED_BASE_DIR}/tf/tf_projectile_flare.cpp"
						"${SHARED_BASE_DIR}/tf/tf_projectile_flare.h"
						"${SHARED_BASE_DIR}/tf/tf_projectile_plasma.cpp"
						"${SHARED_BASE_DIR}/tf/tf_projectile_plasma.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_assaultrifle.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_assaultrifle.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_chainsaw.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_chainsaw.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_compound_bow.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_compound_bow.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_crowbar.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_crowbar.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_displacer.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_displacer.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_flaregun.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_flaregun.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_grenade_mirv.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_grenade_mirv.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_hammerfists.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_hammerfists.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_hunterrifle.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_hunterrifle.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_lunchbox.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_lunchbox.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_mirv.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_mirv.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_nailgun.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_nailgun.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_tranq.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_tranq.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_umbrella.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_umbrella.h"

						"${SHARED_BASE_DIR}/tf/tf_weapon_tommygun.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_tommygun.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_cubemap.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_cubemap.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_sixshooter.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_sixshooter.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_stengun.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_stengun.h"
						"${SHARED_BASE_DIR}/tf/tf_weapon_leverrifle.cpp"
						"${SHARED_BASE_DIR}/tf/tf_weapon_leverrifle.h"
					#}
					"${SHARED_BASE_DIR}/tf/tf_weapon_bat.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_bat.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_bonesaw.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_bonesaw.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_bottle.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_bottle.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_club.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_club.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_fireaxe.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_fireaxe.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_fists.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_fists.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_flamethrower.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_flamethrower.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_grenade_pipebomb.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_grenade_pipebomb.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_grenade_stickybomb.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_grenade_stickybomb.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_grenadelauncher.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_grenadelauncher.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_invis.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_invis.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_knife.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_knife.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_medigun.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_medigun.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_minigun.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_minigun.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_parse.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_parse.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_pda.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_pda.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_pipebomblauncher.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_pipebomblauncher.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_pistol.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_pistol.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_revolver.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_revolver.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_rocketlauncher.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_rocketlauncher.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_shotgun.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_shotgun.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_shovel.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_shovel.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_smg.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_smg.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_sniperrifle.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_sniperrifle.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_syringegun.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_syringegun.h"
					"${SHARED_BASE_DIR}/tf/tf_weapon_wrench.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weapon_wrench.h"
					"${SHARED_BASE_DIR}/tf/tf_weaponbase.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weaponbase.h"
					"${SHARED_BASE_DIR}/tf/tf_weaponbase_grenadeproj.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weaponbase_grenadeproj.h"
					"${SHARED_BASE_DIR}/tf/tf_weaponbase_gun.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weaponbase_gun.h"
					"${SHARED_BASE_DIR}/tf/tf_weaponbase_melee.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weaponbase_melee.h"
					"${SHARED_BASE_DIR}/tf/tf_weaponbase_rocket.cpp"
					"${SHARED_BASE_DIR}/tf/tf_weaponbase_rocket.h"
				#}				
				"${SHARED_BASE_DIR}/tf/achievements_tf.cpp"
				"${SHARED_BASE_DIR}/tf/baseobject_shared.cpp"
				"${SHARED_BASE_DIR}/tf/baseobject_shared.h"
				"${SHARED_BASE_DIR}/tf/entity_capture_flag.cpp"
				"${SHARED_BASE_DIR}/tf/entity_capture_flag.h"
				"${SHARED_BASE_DIR}/tf/ihasbuildpoints.h"
				"${SHARED_BASE_DIR}/tf/script_parser.cpp"
				"${SHARED_BASE_DIR}/tf/script_parser.h"
				"${SHARED_BASE_DIR}/tf/tf_fx_shared.cpp"
				"${SHARED_BASE_DIR}/tf/tf_fx_shared.h"
				"${SHARED_BASE_DIR}/tf/tf_gamemovement.cpp"
				"${SHARED_BASE_DIR}/tf/tf_gamerules.cpp"
				"${SHARED_BASE_DIR}/tf/tf_gamerules.h"
				"${SHARED_BASE_DIR}/tf/tf_gamestats_shared.cpp"
				"${SHARED_BASE_DIR}/tf/tf_gamestats_shared.h"
				"${SHARED_BASE_DIR}/tf/tf_inventory.cpp"
				"${SHARED_BASE_DIR}/tf/tf_inventory.h"
				"${SHARED_BASE_DIR}/tf/tf_item.cpp"
				"${SHARED_BASE_DIR}/tf/tf_item.h"	
				"${SHARED_BASE_DIR}/tf/tf_obj_baseupgrade_shared.cpp"
				"${SHARED_BASE_DIR}/tf/tf_obj_baseupgrade_shared.h"
				"${SHARED_BASE_DIR}/tf/tf_player_shared.cpp"	
				"${SHARED_BASE_DIR}/tf/tf_player_shared.h"
				"${SHARED_BASE_DIR}/tf/tf_playeranimstate.cpp"
				"${SHARED_BASE_DIR}/tf/tf_playeranimstate.h"	
				"${SHARED_BASE_DIR}/tf/tf_playerclass_info_parse.cpp"
				"${SHARED_BASE_DIR}/tf/tf_playerclass_shared.cpp"
				"${SHARED_BASE_DIR}/tf/tf_playerclass_shared.h"
				"${SHARED_BASE_DIR}/tf/tf_projectile_base.cpp"	
				"${SHARED_BASE_DIR}/tf/tf_projectile_base.h"
				"${SHARED_BASE_DIR}/tf/tf_projectile_nail.cpp"
				"${SHARED_BASE_DIR}/tf/tf_projectile_nail.h"
				"${SHARED_BASE_DIR}/tf/tf_shareddefs.cpp"
				"${SHARED_BASE_DIR}/tf/tf_shareddefs.h"
				"${SHARED_BASE_DIR}/tf/tf_usermessages.cpp"	
				"${SHARED_BASE_DIR}/tf/tf_viewmodel.cpp"
				"${SHARED_BASE_DIR}/tf/tf_viewmodel.h"
				"${SHARED_BASE_DIR}/tf/tf_wearable.cpp"
				"${SHARED_BASE_DIR}/tf/tf_wearable.h"
			#}
		#}
	#}
)

set(
	SERVER_TF2CLASSIC_EXCLUDE_SOURCES
)

add_library(
	server_tf2classic MODULE
	${SERVER_TF2CLASSIC_SOURCE_FILES}
)

if( ${IS_WINDOWS} )
target_group_sources(
	server_tf2classic
)
endif()

set_install_properties(
	server_tf2classic
	""
	"server"
	"${GAMEDIR}/${BUILD_FOLDER}/bin"
)

target_include_directories(
	server_tf2classic PRIVATE

	"${SHARED_BASE_DIR}/tf"
	"${SERVER_BASE_DIR}/tf"
	"${SHARED_BASE_DIR}/Multiplayer"
	"${SHARED_BASE_DIR}/econ"
	"${SERVER_BASE_DIR}/tf/bot"
	"${SERVER_BASE_DIR}/tf/nav_mesh"
)

target_compile_definitions(
	server_tf2classic PRIVATE

	TF_CLASSIC
	GLOWS_ENABLE
	NEXT_BOT
	#TF2C_AUTHDATA_KEY={0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41}
	#TF2C_AUTHDATA_XOR={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
	#TF2C_AUTHDATA_BYTE=0x00
)

target_use_server_base(
	server_tf2classic
	SERVER_TF2CLASSIC_EXCLUDE_SOURCES
)

target_use_nav_mesh(
	server_tf2classic
)
