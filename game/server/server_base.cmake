#=============================================================================
#	server_base.cmake
#
#	Project Script
#=============================================================================

include_guard( GLOBAL )

set( SERVER_BASE_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( SHARED_BASE_DIR "${SRCDIR}/game/shared" )

set(
	SERVER_BASE_SOURCE_FILES

	#$Folder "Replay"
	#{
		"${SERVER_BASE_DIR}/gamedll_replay.cpp"
		"${SRCDIR}/common/replay/ireplaysessionrecorder.h"
		
		#"${SHARED_BASE_DIR}/replay_gamestats_shared.cpp"
		#"${SHARED_BASE_DIR}/replay_gamestats_shared.h"
	#}

	#$Folder	"Source Files"
	#{
		"${SHARED_BASE_DIR}/achievement_saverestore.cpp"
		"${SHARED_BASE_DIR}/achievement_saverestore.h"
		"${SHARED_BASE_DIR}/achievementmgr.cpp"
		"${SHARED_BASE_DIR}/achievementmgr.h"
		"${SHARED_BASE_DIR}/activitylist.cpp"
		"${SHARED_BASE_DIR}/activitylist.h"
		"${SERVER_BASE_DIR}/ai_activity.cpp"
		"${SHARED_BASE_DIR}/ai_activity.h"
		"${SERVER_BASE_DIR}/ai_baseactor.cpp"
		"${SERVER_BASE_DIR}/ai_baseactor.h"
		"${SERVER_BASE_DIR}/ai_basehumanoid.cpp"
		"${SERVER_BASE_DIR}/ai_basehumanoid.h"
		"${SERVER_BASE_DIR}/ai_basenpc.cpp"
		"${SERVER_BASE_DIR}/ai_basenpc.h"
		"${SERVER_BASE_DIR}/ai_basenpc_flyer.cpp"
		"${SERVER_BASE_DIR}/ai_basenpc_flyer.h"
		"${SERVER_BASE_DIR}/ai_basenpc_flyer_new.cpp"
		"${SERVER_BASE_DIR}/ai_basenpc_flyer_new.h"
		"${SERVER_BASE_DIR}/ai_basenpc_movement.cpp"
		"${SERVER_BASE_DIR}/ai_basenpc_physicsflyer.cpp"
		"${SERVER_BASE_DIR}/ai_basenpc_physicsflyer.h"
		"${SERVER_BASE_DIR}/ai_basenpc_schedule.cpp"
		"${SERVER_BASE_DIR}/ai_basenpc_squad.cpp"
		"${SERVER_BASE_DIR}/ai_behavior.cpp"
		"${SERVER_BASE_DIR}/ai_behavior.h"
		"${SERVER_BASE_DIR}/ai_behavior_assault.cpp"
		"${SERVER_BASE_DIR}/ai_behavior_assault.h"
		"${SERVER_BASE_DIR}/ai_behavior_fear.cpp"
		"${SERVER_BASE_DIR}/ai_behavior_fear.h"
		"${SERVER_BASE_DIR}/ai_behavior_follow.cpp"
		"${SERVER_BASE_DIR}/ai_behavior_follow.h"
		"${SERVER_BASE_DIR}/ai_behavior_lead.cpp"
		"${SERVER_BASE_DIR}/ai_behavior_lead.h"
		"${SERVER_BASE_DIR}/ai_behavior_rappel.cpp"
		"${SERVER_BASE_DIR}/ai_behavior_rappel.h"
		"${SERVER_BASE_DIR}/ai_behavior_standoff.cpp"
		"${SERVER_BASE_DIR}/ai_behavior_standoff.h"
		"${SERVER_BASE_DIR}/ai_blended_movement.cpp"
		"${SERVER_BASE_DIR}/ai_blended_movement.h"
		"${SERVER_BASE_DIR}/ai_component.h"
		"${SERVER_BASE_DIR}/ai_concommands.cpp"
		"${SERVER_BASE_DIR}/ai_condition.cpp"
		"${SERVER_BASE_DIR}/ai_condition.h"
		"${SERVER_BASE_DIR}/AI_Criteria.cpp"
		"${SERVER_BASE_DIR}/AI_Criteria.h"
		"${SERVER_BASE_DIR}/ai_debug.h"
		"${SHARED_BASE_DIR}/ai_debug_shared.h"
		"${SERVER_BASE_DIR}/ai_default.cpp"
		"${SERVER_BASE_DIR}/ai_default.h"
		"${SERVER_BASE_DIR}/ai_dynamiclink.cpp"
		"${SERVER_BASE_DIR}/ai_dynamiclink.h"
		"${SERVER_BASE_DIR}/ai_event.cpp"
		"${SERVER_BASE_DIR}/ai_goalentity.cpp"
		"${SERVER_BASE_DIR}/ai_goalentity.h"
		"${SERVER_BASE_DIR}/ai_hint.cpp"
		"${SERVER_BASE_DIR}/ai_hint.h"
		"${SERVER_BASE_DIR}/ai_hull.cpp"
		"${SERVER_BASE_DIR}/ai_hull.h"
		"${SERVER_BASE_DIR}/ai_initutils.cpp"
		"${SERVER_BASE_DIR}/ai_initutils.h"
		"${SERVER_BASE_DIR}/AI_Interest_Target.cpp"
		"${SERVER_BASE_DIR}/AI_Interest_Target.h"
		"${SERVER_BASE_DIR}/ai_link.cpp"
		"${SERVER_BASE_DIR}/ai_link.h"
		"${SERVER_BASE_DIR}/ai_localnavigator.cpp"
		"${SERVER_BASE_DIR}/ai_localnavigator.h"
		"${SERVER_BASE_DIR}/ai_looktarget.cpp"
		"${SERVER_BASE_DIR}/ai_looktarget.h"
		"${SERVER_BASE_DIR}/ai_memory.cpp"
		"${SERVER_BASE_DIR}/ai_memory.h"
		"${SERVER_BASE_DIR}/ai_motor.cpp"
		"${SERVER_BASE_DIR}/ai_motor.h"
		"${SERVER_BASE_DIR}/ai_moveprobe.cpp"
		"${SERVER_BASE_DIR}/ai_moveprobe.h"
		"${SERVER_BASE_DIR}/ai_moveshoot.cpp"
		"${SERVER_BASE_DIR}/ai_moveshoot.h"
		"${SERVER_BASE_DIR}/ai_movesolver.cpp"
		"${SERVER_BASE_DIR}/ai_movesolver.h"
		"${SERVER_BASE_DIR}/ai_movetypes.h"
		"${SERVER_BASE_DIR}/ai_namespaces.cpp"
		"${SERVER_BASE_DIR}/ai_namespaces.h"
		"${SERVER_BASE_DIR}/ai_navgoaltype.h"
		"${SERVER_BASE_DIR}/ai_navigator.cpp"
		"${SERVER_BASE_DIR}/ai_navigator.h"
		"${SERVER_BASE_DIR}/ai_navtype.h"
		"${SERVER_BASE_DIR}/ai_network.cpp"
		"${SERVER_BASE_DIR}/ai_network.h"
		"${SERVER_BASE_DIR}/ai_networkmanager.cpp"
		"${SERVER_BASE_DIR}/ai_networkmanager.h"
		"${SERVER_BASE_DIR}/ai_node.cpp"
		"${SERVER_BASE_DIR}/ai_node.h"
		"${SERVER_BASE_DIR}/ai_npcstate.h"
		"${SERVER_BASE_DIR}/ai_obstacle_type.h"
		"${SERVER_BASE_DIR}/ai_pathfinder.cpp"
		"${SERVER_BASE_DIR}/ai_pathfinder.h"
		"${SERVER_BASE_DIR}/ai_planesolver.cpp"
		"${SERVER_BASE_DIR}/ai_planesolver.h"
		"${SERVER_BASE_DIR}/ai_playerally.cpp"
		"${SERVER_BASE_DIR}/ai_playerally.h"
		"${SERVER_BASE_DIR}/AI_ResponseSystem.cpp"
		"${SERVER_BASE_DIR}/AI_ResponseSystem.h"
		"${SERVER_BASE_DIR}/ai_route.cpp"
		"${SERVER_BASE_DIR}/ai_route.h"
		"${SERVER_BASE_DIR}/ai_routedist.h"
		"${SERVER_BASE_DIR}/ai_saverestore.cpp"
		"${SERVER_BASE_DIR}/ai_saverestore.h"
		"${SERVER_BASE_DIR}/ai_schedule.cpp"
		"${SERVER_BASE_DIR}/ai_schedule.h"
		"${SERVER_BASE_DIR}/ai_scriptconditions.cpp"
		"${SERVER_BASE_DIR}/ai_scriptconditions.h"
		"${SERVER_BASE_DIR}/ai_senses.cpp"
		"${SERVER_BASE_DIR}/ai_senses.h"
		"${SERVER_BASE_DIR}/ai_sentence.cpp"
		"${SERVER_BASE_DIR}/ai_sentence.h"
		"${SERVER_BASE_DIR}/ai_speech.cpp"
		"${SERVER_BASE_DIR}/ai_speech.h"
		"${SERVER_BASE_DIR}/ai_speechfilter.cpp"
		"${SERVER_BASE_DIR}/ai_speechfilter.h"
		"${SERVER_BASE_DIR}/ai_squad.cpp"
		"${SERVER_BASE_DIR}/ai_squad.h"
		"${SERVER_BASE_DIR}/ai_squadslot.cpp"
		"${SERVER_BASE_DIR}/ai_squadslot.h"
		"${SERVER_BASE_DIR}/ai_tacticalservices.cpp"
		"${SERVER_BASE_DIR}/ai_tacticalservices.h"
		"${SERVER_BASE_DIR}/ai_task.cpp"
		"${SERVER_BASE_DIR}/ai_task.h"
		"${SERVER_BASE_DIR}/ai_trackpather.cpp"
		"${SERVER_BASE_DIR}/ai_trackpather.h"
		"${SERVER_BASE_DIR}/ai_utils.cpp"
		"${SERVER_BASE_DIR}/ai_utils.h"
		"${SERVER_BASE_DIR}/ai_waypoint.cpp"
		"${SERVER_BASE_DIR}/ai_waypoint.h"
		"${SHARED_BASE_DIR}/ammodef.cpp"
		"${SHARED_BASE_DIR}/animation.cpp"
		"${SHARED_BASE_DIR}/animation.h"
		"${SHARED_BASE_DIR}/apparent_velocity_helper.h"
		"${SHARED_BASE_DIR}/base_playeranimstate.cpp"
		"${SERVER_BASE_DIR}/base_transmit_proxy.cpp"
		"${SHARED_BASE_DIR}/baseachievement.cpp"
		"${SHARED_BASE_DIR}/baseachievement.h"
		"${SERVER_BASE_DIR}/baseanimating.cpp"
		"${SERVER_BASE_DIR}/baseanimating.h"
		"${SERVER_BASE_DIR}/BaseAnimatingOverlay.cpp"
		"${SERVER_BASE_DIR}/BaseAnimatingOverlay.h"
		"${SERVER_BASE_DIR}/basecombatcharacter.cpp"
		"${SERVER_BASE_DIR}/basecombatcharacter.h"
		"${SHARED_BASE_DIR}/basecombatcharacter_shared.cpp"
		"${SERVER_BASE_DIR}/basecombatweapon.cpp"
		"${SERVER_BASE_DIR}/basecombatweapon.h"
		"${SHARED_BASE_DIR}/basecombatweapon_shared.cpp"
		"${SHARED_BASE_DIR}/basecombatweapon_shared.h"
		"${SERVER_BASE_DIR}/baseentity.cpp"
		"${SERVER_BASE_DIR}/baseentity.h"
		"${SHARED_BASE_DIR}/baseentity_shared.cpp"
		"${SHARED_BASE_DIR}/baseentity_shared.h"
		"${SERVER_BASE_DIR}/baseflex.cpp"
		"${SERVER_BASE_DIR}/baseflex.h"
		"${SHARED_BASE_DIR}/basegrenade_shared.cpp"
		"${SHARED_BASE_DIR}/basegrenade_shared.h"
		"${SERVER_BASE_DIR}/basemultiplayerplayer.cpp"
		"${SERVER_BASE_DIR}/basemultiplayerplayer.h"
		"${SHARED_BASE_DIR}/baseparticleentity.cpp"
		"${SHARED_BASE_DIR}/baseparticleentity.h"
		"${SHARED_BASE_DIR}/baseplayer_shared.cpp"
		"${SHARED_BASE_DIR}/baseplayer_shared.h"
		"${SHARED_BASE_DIR}/baseprojectile.cpp"
		"${SHARED_BASE_DIR}/baseprojectile.h"
		"${SERVER_BASE_DIR}/BasePropDoor.h"
		"${SERVER_BASE_DIR}/basetoggle.h"
		"${SERVER_BASE_DIR}/baseviewmodel.cpp"
		"${SERVER_BASE_DIR}/baseviewmodel.h"
		"${SHARED_BASE_DIR}/baseviewmodel_shared.cpp"
		"${SHARED_BASE_DIR}/baseviewmodel_shared.h"
		"${SHARED_BASE_DIR}/beam_shared.cpp"
		"${SHARED_BASE_DIR}/beam_shared.h"
		"${SERVER_BASE_DIR}/bitstring.cpp"
		"${SERVER_BASE_DIR}/bitstring.h"
		"${SERVER_BASE_DIR}/bmodels.cpp"
		"${SRCDIR}/public/bone_setup.h"
		"${SERVER_BASE_DIR}/buttons.cpp"
		"${SERVER_BASE_DIR}/buttons.h"
		"${SERVER_BASE_DIR}/cbase.cpp"
		"${SERVER_BASE_DIR}/cbase.h"
		"${SHARED_BASE_DIR}/choreoactor.h"
		"${SHARED_BASE_DIR}/choreochannel.h"
		"${SHARED_BASE_DIR}/choreoevent.h"
		"${SHARED_BASE_DIR}/choreoscene.h"
		"${SERVER_BASE_DIR}/client.cpp"
		"${SERVER_BASE_DIR}/client.h"
		"${SHARED_BASE_DIR}/collisionproperty.cpp"
		"${SHARED_BASE_DIR}/collisionproperty.h"
		"${SRCDIR}/public/collisionutils.h"
		"${SERVER_BASE_DIR}/colorcorrection.cpp"
		"${SERVER_BASE_DIR}/colorcorrectionvolume.cpp"
		"${SERVER_BASE_DIR}/CommentarySystem.cpp"
		"${SERVER_BASE_DIR}/controlentities.cpp"
		"${SERVER_BASE_DIR}/cplane.cpp"
		"${SERVER_BASE_DIR}/CRagdollMagnet.cpp"
		"${SERVER_BASE_DIR}/CRagdollMagnet.h"
		"${SERVER_BASE_DIR}/damagemodifier.cpp"
		"${SHARED_BASE_DIR}/death_pose.cpp"
		"${SHARED_BASE_DIR}/debugoverlay_shared.cpp"
		"${SHARED_BASE_DIR}/debugoverlay_shared.h"
		"${SHARED_BASE_DIR}/decals.cpp"
		"${SERVER_BASE_DIR}/doors.cpp"
		"${SERVER_BASE_DIR}/doors.h"
		"${SERVER_BASE_DIR}/dynamiclight.cpp"
		"${SRCDIR}/public/edict.h"
		"${SRCDIR}/public/editor_sendcommand.h"
		"${SHARED_BASE_DIR}/effect_color_tables.h"
		"${SHARED_BASE_DIR}/effect_dispatch_data.cpp"
		"${SERVER_BASE_DIR}/effects.cpp"
		"${SERVER_BASE_DIR}/effects.h"
		"${SERVER_BASE_DIR}/EffectsServer.cpp"
		"${SHARED_BASE_DIR}/ehandle.cpp"
		"${SRCDIR}/public/eiface.h"
		"${SERVER_BASE_DIR}/enginecallback.h"
		"${SERVER_BASE_DIR}/entityapi.h"
		"${SERVER_BASE_DIR}/entityblocker.cpp"
		"${SERVER_BASE_DIR}/entityblocker.h"
		"${SERVER_BASE_DIR}/EntityDissolve.cpp"
		"${SERVER_BASE_DIR}/EntityDissolve.h"
		"${SERVER_BASE_DIR}/EntityFlame.cpp"
		"${SERVER_BASE_DIR}/entityinput.h"
		"${SERVER_BASE_DIR}/entitylist.cpp"
		"${SERVER_BASE_DIR}/entitylist.h"
		"${SHARED_BASE_DIR}/entitylist_base.cpp"
		"${SERVER_BASE_DIR}/entityoutput.h"
		"${SERVER_BASE_DIR}/EntityParticleTrail.cpp"
		"${SERVER_BASE_DIR}/EntityParticleTrail.h"
		"${SHARED_BASE_DIR}/EntityParticleTrail_Shared.cpp"
		"${SHARED_BASE_DIR}/entityparticletrail_shared.h"
		"${SERVER_BASE_DIR}/env_debughistory.cpp"
		"${SERVER_BASE_DIR}/env_debughistory.h"
		"${SHARED_BASE_DIR}/env_detail_controller.cpp"
		"${SERVER_BASE_DIR}/env_effectsscript.cpp"
		"${SERVER_BASE_DIR}/env_entity_maker.cpp"
		"${SERVER_BASE_DIR}/env_particlescript.cpp"
		"${SERVER_BASE_DIR}/env_player_surface_trigger.cpp"
		"${SERVER_BASE_DIR}/env_player_surface_trigger.h"
		"${SERVER_BASE_DIR}/env_projectedtexture.cpp"
		"${SERVER_BASE_DIR}/env_screenoverlay.cpp"
		"${SERVER_BASE_DIR}/env_texturetoggle.cpp"
		"${SERVER_BASE_DIR}/env_tonemap_controller.cpp"
		"${SHARED_BASE_DIR}/env_wind_shared.cpp"
		"${SHARED_BASE_DIR}/env_wind_shared.h"
		"${SERVER_BASE_DIR}/env_zoom.cpp"
		"${SERVER_BASE_DIR}/env_zoom.h"
		"${SERVER_BASE_DIR}/EnvBeam.cpp"
		"${SERVER_BASE_DIR}/EnvFade.cpp"
		"${SERVER_BASE_DIR}/EnvHudHint.cpp"
		"${SERVER_BASE_DIR}/EnvLaser.cpp"
		"${SERVER_BASE_DIR}/EnvLaser.h"
		"${SERVER_BASE_DIR}/EnvMessage.cpp"
		"${SERVER_BASE_DIR}/EnvMessage.h"
		"${SERVER_BASE_DIR}/envmicrophone.cpp"
		"${SERVER_BASE_DIR}/envmicrophone.h"
		"${SERVER_BASE_DIR}/EnvShake.cpp"
		"${SERVER_BASE_DIR}/EnvSpark.cpp"
		"${SERVER_BASE_DIR}/envspark.h"
		"${SRCDIR}/public/event_flags.h"
		"${SERVER_BASE_DIR}/event_tempentity_tester.h"
		"${SHARED_BASE_DIR}/eventlist.cpp"
		"${SHARED_BASE_DIR}/eventlist.h"
		"${SERVER_BASE_DIR}/EventLog.cpp"
		"${SERVER_BASE_DIR}/eventqueue.h"
		"${SERVER_BASE_DIR}/explode.cpp"
		"${SERVER_BASE_DIR}/explode.h"
		"${SERVER_BASE_DIR}/filters.cpp"
		"${SERVER_BASE_DIR}/filters.h"
		"${SERVER_BASE_DIR}/fire.cpp"
		"${SERVER_BASE_DIR}/fire.h"
		"${SERVER_BASE_DIR}/fire_smoke.cpp"
		"${SERVER_BASE_DIR}/fire_smoke.h"
		"${SERVER_BASE_DIR}/fish.cpp"
		"${SERVER_BASE_DIR}/fish.h"
		"${SERVER_BASE_DIR}/fogcontroller.cpp"
		"${SERVER_BASE_DIR}/fourwheelvehiclephysics.cpp"
		"${SERVER_BASE_DIR}/fourwheelvehiclephysics.h"
		"${SERVER_BASE_DIR}/func_areaportal.cpp"
		"${SERVER_BASE_DIR}/func_areaportalbase.cpp"
		"${SERVER_BASE_DIR}/func_areaportalbase.h"
		"${SERVER_BASE_DIR}/func_areaportalwindow.cpp"
		"${SERVER_BASE_DIR}/func_areaportalwindow.h"
		"${SERVER_BASE_DIR}/func_break.cpp"
		"${SERVER_BASE_DIR}/func_break.h"
		"${SERVER_BASE_DIR}/func_breakablesurf.cpp"
		"${SERVER_BASE_DIR}/func_breakablesurf.h"
		"${SERVER_BASE_DIR}/func_dust.cpp"
		"${SHARED_BASE_DIR}/func_dust_shared.h"
		"${SHARED_BASE_DIR}/func_ladder.cpp"
		"${SERVER_BASE_DIR}/func_ladder_endpoint.cpp"
		"${SERVER_BASE_DIR}/func_lod.cpp"
		"${SERVER_BASE_DIR}/func_movelinear.cpp"
		"${SERVER_BASE_DIR}/func_movelinear.h"
		"${SERVER_BASE_DIR}/func_occluder.cpp"
		"${SERVER_BASE_DIR}/func_reflective_glass.cpp"
		"${SERVER_BASE_DIR}/func_smokevolume.cpp"
		"${SERVER_BASE_DIR}/game.cpp"
		"${SERVER_BASE_DIR}/game.h"
		"${SERVER_BASE_DIR}/game_ui.cpp"
		"${SERVER_BASE_DIR}/gameinterface.cpp"
		"${SERVER_BASE_DIR}/gameinterface.h"
		"${SHARED_BASE_DIR}/gamemovement.cpp"
		"${SHARED_BASE_DIR}/gamemovement.h"
		"${SHARED_BASE_DIR}/gamerules.cpp"
		"${SHARED_BASE_DIR}/gamerules.h"
		"${SHARED_BASE_DIR}/gamerules_register.cpp"
		"${SHARED_BASE_DIR}/GameStats.cpp"
		"${SHARED_BASE_DIR}/gamestats.h"
		"${SHARED_BASE_DIR}/gamestringpool.cpp"
		"${SHARED_BASE_DIR}/gamestringpool.h"
		"${SERVER_BASE_DIR}/gametrace_dll.cpp"
		"${SHARED_BASE_DIR}/gamevars_shared.cpp"
		"${SHARED_BASE_DIR}/gamevars_shared.h"
		"${SERVER_BASE_DIR}/gameweaponmanager.cpp"
		"${SERVER_BASE_DIR}/gameweaponmanager.h"
		"${SERVER_BASE_DIR}/genericactor.cpp"
		"${SERVER_BASE_DIR}/genericmonster.cpp"
		"${SERVER_BASE_DIR}/gib.cpp"
		"${SERVER_BASE_DIR}/gib.h"
		"${SERVER_BASE_DIR}/globals.cpp"
		"${SERVER_BASE_DIR}/globalstate.cpp"
		"${SERVER_BASE_DIR}/globalstate.h"
		"${SERVER_BASE_DIR}/globalstate_private.h"
		"${SERVER_BASE_DIR}/guntarget.cpp"
		"${SERVER_BASE_DIR}/h_ai.cpp"
		"${SERVER_BASE_DIR}/hierarchy.cpp"
		"${SERVER_BASE_DIR}/hierarchy.h"
		"${SRCDIR}/common/hl2orange.spa.h"
		"${SERVER_BASE_DIR}/hltvdirector.cpp"
		"${SERVER_BASE_DIR}/hltvdirector.h"
		"${SHARED_BASE_DIR}/hintmessage.cpp"
		"${SHARED_BASE_DIR}/hintmessage.h"
		"${SHARED_BASE_DIR}/hintsystem.cpp"
		"${SHARED_BASE_DIR}/hintsystem.h"
		"${SHARED_BASE_DIR}/ichoreoeventcallback.h"
		"${SHARED_BASE_DIR}/igamesystem.cpp"
		"${SHARED_BASE_DIR}/igamesystem.h"
		"${SERVER_BASE_DIR}/info_camera_link.cpp"
		"${SERVER_BASE_DIR}/info_camera_link.h"
		"${SERVER_BASE_DIR}/info_overlay_accessor.cpp"
		"${SERVER_BASE_DIR}/init_factory.h"
		"${SERVER_BASE_DIR}/intermission.cpp"
		"${SRCDIR}/public/interpolatortypes.h"
		"${SHARED_BASE_DIR}/interval.h"
		"${SRCDIR}/public/iregistry.h"
		"${SHARED_BASE_DIR}/iscenetokenprocessor.h"
		"${SERVER_BASE_DIR}/iservervehicle.h"
		"${SERVER_BASE_DIR}/item_world.cpp"
		"${SERVER_BASE_DIR}/items.h"
		"${SRCDIR}/public/ivoiceserver.h"
		"${SRCDIR}/public/keyframe/keyframe.h"
		"${SERVER_BASE_DIR}/lightglow.cpp"
		"${SERVER_BASE_DIR}/lights.cpp"
		"${SERVER_BASE_DIR}/lights.h"
		"${SERVER_BASE_DIR}/locksounds.h"
		"${SERVER_BASE_DIR}/logic_measure_movement.cpp"
		"${SERVER_BASE_DIR}/logic_navigation.cpp"
		"${SERVER_BASE_DIR}/logicauto.cpp"
		"${SERVER_BASE_DIR}/logicentities.cpp"
		"${SERVER_BASE_DIR}/logicrelay.cpp"
		"${SERVER_BASE_DIR}/mapentities.cpp"
		"${SHARED_BASE_DIR}/mapentities_shared.cpp"
		"${SERVER_BASE_DIR}/maprules.cpp"
		"${SERVER_BASE_DIR}/maprules.h"
		"${SERVER_BASE_DIR}/MaterialModifyControl.cpp"
		"${SRCDIR}/public/mathlib/mathlib.h"
		"${SERVER_BASE_DIR}/message_entity.cpp"
		"${SRCDIR}/public/model_types.h"
		"${SERVER_BASE_DIR}/modelentities.cpp"
		"${SHARED_BASE_DIR}/ModelSoundsCache.cpp"
		"${SERVER_BASE_DIR}/movehelper_server.cpp"
		"${SERVER_BASE_DIR}/movehelper_server.h"
		"${SERVER_BASE_DIR}/movement.cpp"
		"${SHARED_BASE_DIR}/movevars_shared.cpp"
		"${SERVER_BASE_DIR}/movie_explosion.h"
		"${SHARED_BASE_DIR}/multiplay_gamerules.cpp"
		"${SHARED_BASE_DIR}/multiplay_gamerules.h"
		"${SERVER_BASE_DIR}/ndebugoverlay.cpp"
		"${SERVER_BASE_DIR}/ndebugoverlay.h"
		"${SERVER_BASE_DIR}/networkstringtable_gamedll.h"
		"${SRCDIR}/public/networkstringtabledefs.h"
		"${SERVER_BASE_DIR}/npc_vehicledriver.cpp"
		"${SHARED_BASE_DIR}/obstacle_pushaway.cpp"
		"${SHARED_BASE_DIR}/obstacle_pushaway.h"
		"${SERVER_BASE_DIR}/particle_fire.h"
		"${SERVER_BASE_DIR}/particle_light.cpp"
		"${SERVER_BASE_DIR}/particle_light.h"
		"${SHARED_BASE_DIR}/particle_parse.cpp"
		"${SHARED_BASE_DIR}/particle_parse.h"
		"${SERVER_BASE_DIR}/particle_smokegrenade.h"
		"${SERVER_BASE_DIR}/particle_system.cpp"
		"${SHARED_BASE_DIR}/particlesystemquery.cpp"
		"${SERVER_BASE_DIR}/pathcorner.cpp"
		"${SERVER_BASE_DIR}/pathtrack.cpp"
		"${SERVER_BASE_DIR}/pathtrack.h"
		"${SRCDIR}/public/vphysics/performance.h"
		"${SERVER_BASE_DIR}/phys_controller.cpp"
		"${SERVER_BASE_DIR}/phys_controller.h"
		"${SERVER_BASE_DIR}/physconstraint.cpp"
		"${SERVER_BASE_DIR}/physconstraint.h"
		"${SERVER_BASE_DIR}/physics.cpp"
		"${SERVER_BASE_DIR}/physics.h"
		"${SERVER_BASE_DIR}/physics_bone_follower.cpp"
		"${SERVER_BASE_DIR}/physics_cannister.cpp"
		"${SERVER_BASE_DIR}/physics_collisionevent.h"
		"${SERVER_BASE_DIR}/physics_fx.cpp"
		"${SERVER_BASE_DIR}/physics_impact_damage.cpp"
		"${SERVER_BASE_DIR}/pushentity.h"
		"${SERVER_BASE_DIR}/physics_main.cpp"
		"${SHARED_BASE_DIR}/physics_main_shared.cpp"
		"${SERVER_BASE_DIR}/physics_npc_solver.cpp"
		"${SERVER_BASE_DIR}/physics_npc_solver.h"
		"${SERVER_BASE_DIR}/physics_prop_ragdoll.cpp"
		"${SERVER_BASE_DIR}/physics_prop_ragdoll.h"
		"${SHARED_BASE_DIR}/physics_saverestore.cpp"
		"${SHARED_BASE_DIR}/physics_saverestore.h"
		"${SHARED_BASE_DIR}/physics_shared.cpp"
		"${SHARED_BASE_DIR}/physics_shared.h"
		"${SERVER_BASE_DIR}/physobj.cpp"
		"${SERVER_BASE_DIR}/physobj.h"
		"${SERVER_BASE_DIR}/player.cpp"
		"${SERVER_BASE_DIR}/player.h"
		"${SERVER_BASE_DIR}/player_command.cpp"
		"${SERVER_BASE_DIR}/player_command.h"
		"${SERVER_BASE_DIR}/player_lagcompensation.cpp"
		"${SERVER_BASE_DIR}/player_pickup.cpp"
		"${SERVER_BASE_DIR}/player_pickup.h"
		"${SERVER_BASE_DIR}/player_resource.cpp"
		"${SERVER_BASE_DIR}/player_resource.h"
		"${SERVER_BASE_DIR}/playerinfomanager.cpp"
		"${SERVER_BASE_DIR}/playerlocaldata.cpp"
		"${SERVER_BASE_DIR}/playerlocaldata.h"
		"${SERVER_BASE_DIR}/plugin_check.cpp"
		"${SHARED_BASE_DIR}/point_bonusmaps_accessor.cpp"
		"${SHARED_BASE_DIR}/point_bonusmaps_accessor.h"
		"${SERVER_BASE_DIR}/point_camera.cpp"
		"${SERVER_BASE_DIR}/point_camera.h"
		"${SERVER_BASE_DIR}/point_devshot_camera.cpp"
		"${SERVER_BASE_DIR}/point_playermoveconstraint.cpp"
		"${SHARED_BASE_DIR}/point_posecontroller.cpp"
		"${SHARED_BASE_DIR}/point_posecontroller.h"
		"${SERVER_BASE_DIR}/point_spotlight.cpp"
		"${SERVER_BASE_DIR}/point_template.cpp"
		"${SERVER_BASE_DIR}/point_template.h"
		"${SERVER_BASE_DIR}/pointanglesensor.cpp"
		"${SERVER_BASE_DIR}/PointAngularVelocitySensor.cpp"
		"${SERVER_BASE_DIR}/pointhurt.cpp"
		"${SERVER_BASE_DIR}/pointteleport.cpp"
		"${SRCDIR}/public/mathlib/polyhedron.h"
		"${SHARED_BASE_DIR}/positionwatcher.h"
		"${SHARED_BASE_DIR}/precache_register.cpp"
		"${SHARED_BASE_DIR}/precache_register.h"
		"${SHARED_BASE_DIR}/predictableid.cpp"
		"${SHARED_BASE_DIR}/predictableid.h"
		"${SERVER_BASE_DIR}/props.cpp"
		"${SERVER_BASE_DIR}/props.h"
		"${SHARED_BASE_DIR}/props_shared.cpp"
		"${SHARED_BASE_DIR}/querycache.cpp"
		"${SERVER_BASE_DIR}/ragdoll_manager.cpp"
		"${SHARED_BASE_DIR}/ragdoll_shared.cpp"
		"${SERVER_BASE_DIR}/RagdollBoogie.cpp"
		"${SERVER_BASE_DIR}/RagdollBoogie.h"
		"${SERVER_BASE_DIR}/recipientfilter.cpp"
		"${SERVER_BASE_DIR}/recipientfilter.h"
		"${SERVER_BASE_DIR}/rope.cpp"
		"${SERVER_BASE_DIR}/rope.h"
		"${SHARED_BASE_DIR}/rope_helpers.cpp"
		"${SRCDIR}/public/rope_physics.h"
		"${SRCDIR}/public/rope_shared.h"
		"${SHARED_BASE_DIR}/saverestore.cpp"
		"${SHARED_BASE_DIR}/saverestore.h"
		"${SHARED_BASE_DIR}/saverestore_bitstring.h"
		"${SERVER_BASE_DIR}/saverestore_gamedll.cpp"
		"${SHARED_BASE_DIR}/saverestore_utlsymbol.h"
		"${SHARED_BASE_DIR}/saverestore_utlvector.h"
		"${SHARED_BASE_DIR}/SceneCache.cpp"
		"${SERVER_BASE_DIR}/sceneentity.cpp"
		"${SERVER_BASE_DIR}/sceneentity.h"
		"${SHARED_BASE_DIR}/sceneentity_shared.cpp"
		"${SERVER_BASE_DIR}/scratchpad_gamedll_helpers.cpp"
		"${SERVER_BASE_DIR}/scripted.cpp"
		"${SERVER_BASE_DIR}/scripted.h"
		"${SERVER_BASE_DIR}/scriptedtarget.cpp"
		"${SERVER_BASE_DIR}/scriptedtarget.h"
		"${SHARED_BASE_DIR}/scriptevent.h"
		"${SERVER_BASE_DIR}/sendproxy.cpp"
		"${SHARED_BASE_DIR}/sequence_Transitioner.cpp"
		"${SRCDIR}/game/server/serverbenchmark_base.cpp"
		"${SRCDIR}/game/server/serverbenchmark_base.h"
		"${SRCDIR}/public/server_class.h"
		"${SERVER_BASE_DIR}/ServerNetworkProperty.cpp"
		"${SERVER_BASE_DIR}/ServerNetworkProperty.h"
		"${SERVER_BASE_DIR}/shadowcontrol.cpp"
		"${SRCDIR}/public/shattersurfacetypes.h"
		"${SHARED_BASE_DIR}/sheetsimulator.h"
		"${SRCDIR}/public/simple_physics.h"
		"${SHARED_BASE_DIR}/simtimer.cpp"
		"${SHARED_BASE_DIR}/simtimer.h"
		"${SHARED_BASE_DIR}/singleplay_gamerules.cpp"
		"${SHARED_BASE_DIR}/singleplay_gamerules.h"
		"${SERVER_BASE_DIR}/SkyCamera.cpp"
		"${SERVER_BASE_DIR}/slideshow_display.cpp"
		"${SERVER_BASE_DIR}/sound.cpp"
		"${SHARED_BASE_DIR}/SoundEmitterSystem.cpp"
		"${SERVER_BASE_DIR}/soundent.cpp"
		"${SERVER_BASE_DIR}/soundent.h"
		"${SHARED_BASE_DIR}/soundenvelope.cpp"
		"${SRCDIR}/public/SoundParametersInternal.cpp"
		"${SERVER_BASE_DIR}/soundscape.cpp"
		"${SERVER_BASE_DIR}/soundscape.h"
		"${SERVER_BASE_DIR}/soundscape_system.cpp"
		"${SERVER_BASE_DIR}/spark.h"
		"${SERVER_BASE_DIR}/spotlightend.cpp"
		"${SERVER_BASE_DIR}/spotlightend.h"
		"${SHARED_BASE_DIR}/Sprite.cpp"
		"${SHARED_BASE_DIR}/Sprite.h"
		"${SERVER_BASE_DIR}/sprite_perfmonitor.cpp"
		"${SHARED_BASE_DIR}/SpriteTrail.h"
		"${SRCDIR}/public/vphysics/stats.h"
		"${SRCDIR}/public/stringregistry.h"
		"${SHARED_BASE_DIR}/studio_shared.cpp"
		"${SERVER_BASE_DIR}/subs.cpp"
		"${SERVER_BASE_DIR}/sun.cpp"
		"${SERVER_BASE_DIR}/tactical_mission.cpp"
		"${SERVER_BASE_DIR}/tactical_mission.h"
		"${SHARED_BASE_DIR}/takedamageinfo.cpp"
		"${SERVER_BASE_DIR}/tanktrain.cpp"
		"${SERVER_BASE_DIR}/team.cpp"
		"${SERVER_BASE_DIR}/team.h"
		"${SHARED_BASE_DIR}/teamplay_gamerules.cpp"
		"${SHARED_BASE_DIR}/teamplay_gamerules.h"
		"${SHARED_BASE_DIR}/tempentity.h"
		"${SERVER_BASE_DIR}/TemplateEntities.cpp"
		"${SERVER_BASE_DIR}/TemplateEntities.h"
		"${SERVER_BASE_DIR}/tesla.cpp"
		"${SHARED_BASE_DIR}/test_ehandle.cpp"
		"${SERVER_BASE_DIR}/test_proxytoggle.cpp"
		"${SERVER_BASE_DIR}/test_stressentities.cpp"
		"${SERVER_BASE_DIR}/testfunctions.cpp"
		"${SERVER_BASE_DIR}/testtraceline.cpp"
		"${SERVER_BASE_DIR}/textstatsmgr.cpp"
		"${SERVER_BASE_DIR}/timedeventmgr.cpp"
		"${SERVER_BASE_DIR}/trains.cpp"
		"${SERVER_BASE_DIR}/trains.h"
		"${SERVER_BASE_DIR}/triggers.cpp"
		"${SERVER_BASE_DIR}/triggers.h"
		"${SHARED_BASE_DIR}/usercmd.cpp"
		"${SERVER_BASE_DIR}/util.cpp"
		"${SERVER_BASE_DIR}/util.h"
		"${SHARED_BASE_DIR}/util_shared.cpp"
		"${SERVER_BASE_DIR}/variant_t.cpp"
		"${SERVER_BASE_DIR}/vehicle_base.cpp"
		"${SERVER_BASE_DIR}/vehicle_baseserver.cpp"
		"${SERVER_BASE_DIR}/vehicle_sounds.h"
		"${SHARED_BASE_DIR}/vehicle_viewblend_shared.cpp"
		"${SERVER_BASE_DIR}/vguiscreen.cpp"
		"${SERVER_BASE_DIR}/vguiscreen.h"
		"${SRCDIR}/public/mathlib/vmatrix.h"
		"${SHARED_BASE_DIR}/voice_common.h"
		"${SHARED_BASE_DIR}/voice_gamemgr.cpp"
		"${SHARED_BASE_DIR}/voice_gamemgr.h"
		"${SERVER_BASE_DIR}/waterbullet.cpp"
		"${SERVER_BASE_DIR}/waterbullet.h"
		"${SERVER_BASE_DIR}/WaterLODControl.cpp"
		"${SERVER_BASE_DIR}/wcedit.cpp"
		"${SERVER_BASE_DIR}/wcedit.h"
		"${SHARED_BASE_DIR}/weapon_parse.cpp"
		"${SHARED_BASE_DIR}/weapon_parse.h"
		"${SHARED_BASE_DIR}/weapon_proficiency.cpp"
		"${SHARED_BASE_DIR}/weapon_proficiency.h"
		"${SERVER_BASE_DIR}/weight_button.cpp"
		"${SERVER_BASE_DIR}/world.cpp"
		"${SERVER_BASE_DIR}/world.h"
		"${SHARED_BASE_DIR}/mp_shareddefs.cpp"
		"${SHARED_BASE_DIR}/SharedFunctorUtils.h"
		"${SRCDIR}/game/server/vote_controller.h"
		"${SRCDIR}/game/server/vote_controller.cpp"

		"${SRCDIR}/public/haptics/haptic_msgs.cpp"
		"$<${IS_WINDOWS}:${SRCDIR}/public/haptics/haptic_utils.cpp>"

		# Not using precompiled header cbase.h

		"${SRCDIR}/public/bone_setup.cpp"					#/
		"${SRCDIR}/public/collisionutils.cpp"				#/
		"${SRCDIR}/public/dt_send.cpp"						#/
		"${SRCDIR}/public/dt_utlvector_common.cpp"			#/
		"${SRCDIR}/public/dt_utlvector_send.cpp"			#/
		"${SRCDIR}/public/editor_sendcommand.cpp"			#/
		"${SRCDIR}/public/filesystem_helpers.cpp"			#/
		"${SERVER_BASE_DIR}/gamehandle.cpp"					#/
		"${SERVER_BASE_DIR}/h_export.cpp"					#/
		"${SERVER_BASE_DIR}/init_factory.cpp"				#/
		"${SRCDIR}/public/interpolatortypes.cpp"			#/
		"${SHARED_BASE_DIR}/interval.cpp"					#/
		"${SRCDIR}/public/keyframe/keyframe.cpp"			#/
		"${SRCDIR}/common/language.cpp"						#/
		"$<$<NOT:${IS_CLANGCL}>:${SRCDIR}/public/map_utils.cpp>"					#/
		"${SRCDIR}/public/networkvar.cpp"					#/
		"${SRCDIR}/common/randoverride.cpp"					#/
		"${SRCDIR}/public/registry.cpp"						#/
		"${SRCDIR}/public/rope_physics.cpp"					#/
		"${SRCDIR}/public/scratchpad3d.cpp"					#/
		"${SRCDIR}/public/ScratchPadUtils.cpp"				#/
		"${SRCDIR}/public/server_class.cpp"					#/
		"${SHARED_BASE_DIR}/sheetsimulator.cpp"				#/
		"${SRCDIR}/public/simple_physics.cpp"				#/
		"${SRCDIR}/public/stringregistry.cpp"				#/
		"${SRCDIR}/public/studio.cpp"						#/
		"${SERVER_BASE_DIR}/GameStats_BasicStatsFunctions.cpp"
		#{
		#	$Configuration
		#	{
		#		$Compiler
		#		{
		#			$Create/UsePrecompiledHeader	"Not Using Precompiled Headers"
		#		}
		#	}
		#}

		#$Folder	"Precompiled Header"
		#{
		#	"${SERVER_BASE_DIR}/stdafx.cpp"
		#	{
		#		$Configuration
		#		{
		#			$Compiler
		#			{
		#				$Create/UsePrecompiledHeader	"Create Precompiled Header (/Yc)"
		#			}
		#		}
		#	}
		#}

		#$Folder	"temporary entities"
		#{
			"${SERVER_BASE_DIR}/basetempentity.cpp"
			"${SERVER_BASE_DIR}/event_tempentity_tester.cpp"
			"${SERVER_BASE_DIR}/movie_explosion.cpp"
			"${SERVER_BASE_DIR}/particle_fire.cpp"
			"${SERVER_BASE_DIR}/particle_smokegrenade.cpp"
			"${SERVER_BASE_DIR}/plasma.cpp"
			"${SERVER_BASE_DIR}/plasma.h"
			"${SERVER_BASE_DIR}/smoke_trail.h"
			"${SERVER_BASE_DIR}/smokestack.cpp"
			"${SERVER_BASE_DIR}/smokestack.h"
			"${SERVER_BASE_DIR}/smoke_trail.cpp"
			"${SHARED_BASE_DIR}/SpriteTrail.cpp"
			"${SERVER_BASE_DIR}/steamjet.cpp"
			"${SERVER_BASE_DIR}/steamjet.h"
			"${SERVER_BASE_DIR}/te.cpp"
			"${SERVER_BASE_DIR}/te.h"
			"${SERVER_BASE_DIR}/te_armorricochet.cpp"
			"${SERVER_BASE_DIR}/te_basebeam.cpp"
			"${SERVER_BASE_DIR}/te_basebeam.h"
			"${SERVER_BASE_DIR}/te_beamentpoint.cpp"
			"${SERVER_BASE_DIR}/te_beaments.cpp"
			"${SERVER_BASE_DIR}/te_beamfollow.cpp"
			"${SERVER_BASE_DIR}/te_beamlaser.cpp"
			"${SERVER_BASE_DIR}/te_beampoints.cpp"
			"${SERVER_BASE_DIR}/te_beamring.cpp"
			"${SERVER_BASE_DIR}/te_beamringpoint.cpp"
			"${SERVER_BASE_DIR}/te_beamspline.cpp"
			"${SERVER_BASE_DIR}/te_bloodsprite.cpp"
			"${SERVER_BASE_DIR}/te_bloodstream.cpp"
			"${SERVER_BASE_DIR}/te_breakmodel.cpp"
			"${SERVER_BASE_DIR}/te_bspdecal.cpp"
			"${SERVER_BASE_DIR}/te_bubbles.cpp"
			"${SERVER_BASE_DIR}/te_bubbletrail.cpp"
			"${SERVER_BASE_DIR}/te_clientprojectile.cpp"
			"${SERVER_BASE_DIR}/te_decal.cpp"
			"${SERVER_BASE_DIR}/te_dynamiclight.cpp"
			"${SERVER_BASE_DIR}/te_effect_dispatch.cpp"
			"${SERVER_BASE_DIR}/te_energysplash.cpp"
			"${SERVER_BASE_DIR}/te_explosion.cpp"
			"${SERVER_BASE_DIR}/te_fizz.cpp"
			"${SERVER_BASE_DIR}/te_footprintdecal.cpp"
			"${SERVER_BASE_DIR}/te_glassshatter.cpp"
			"${SERVER_BASE_DIR}/te_glowsprite.cpp"
			"${SERVER_BASE_DIR}/te_impact.cpp"
			"${SERVER_BASE_DIR}/te_killplayerattachments.cpp"
			"${SERVER_BASE_DIR}/te_largefunnel.cpp"
			"${SERVER_BASE_DIR}/te_muzzleflash.cpp"
			"${SERVER_BASE_DIR}/te_particlesystem.cpp"
			"${SERVER_BASE_DIR}/te_particlesystem.h"
			"${SERVER_BASE_DIR}/te_physicsprop.cpp"
			"${SERVER_BASE_DIR}/te_playerdecal.cpp"
			"${SERVER_BASE_DIR}/te_projecteddecal.cpp"
			"${SERVER_BASE_DIR}/te_showline.cpp"
			"${SERVER_BASE_DIR}/te_smoke.cpp"
			"${SERVER_BASE_DIR}/te_sparks.cpp"
			"${SERVER_BASE_DIR}/te_sprite.cpp"
			"${SERVER_BASE_DIR}/te_spritespray.cpp"
			"${SERVER_BASE_DIR}/te_worlddecal.cpp"
			"${SHARED_BASE_DIR}/usermessages.cpp"
		#}
	#}

	#$Folder	"Header Files"
	#{
		"${SHARED_BASE_DIR}/ammodef.h"
		"${SHARED_BASE_DIR}/base_playeranimstate.h"
		"${SERVER_BASE_DIR}/base_transmit_proxy.h"
		"${SRCDIR}/public/basehandle.h"
		"${SERVER_BASE_DIR}/basetempentity.h"
		"${SRCDIR}/public/tier0/basetypes.h"
		"${SHARED_BASE_DIR}/beam_flags.h"
		"${SRCDIR}/public/tier1/bitbuf.h"
		"${SRCDIR}/public/bitvec.h"
		"${SRCDIR}/public/bone_accessor.h"
		"${SRCDIR}/public/bspfile.h"
		"${SRCDIR}/public/bspflags.h"
		"${SRCDIR}/public/mathlib/bumpvects.h"
		"${SRCDIR}/public/tier1/characterset.h"
		"${SRCDIR}/public/tier1/checksum_md5.h"
		"${SRCDIR}/public/client_class.h"
		"${SRCDIR}/public/client_textmessage.h"
		"${SRCDIR}/public/cmodel.h"
		"${SRCDIR}/public/vphysics/collision_set.h"
		"${SRCDIR}/public/Color.h"
		"${SRCDIR}/public/tier0/commonmacros.h"
		"${SRCDIR}/public/mathlib/compressed_light_cube.h"
		"${SRCDIR}/public/mathlib/compressed_vector.h"
		"${SRCDIR}/public/const.h"
		"${SRCDIR}/public/vphysics/constraints.h"
		"${SRCDIR}/public/coordsize.h"
		"${SERVER_BASE_DIR}/cplane.h"
		"${SERVER_BASE_DIR}/damagemodifier.h"
		"${SRCDIR}/public/datamap.h"
		"${SRCDIR}/public/tier0/dbg.h"
		"${SHARED_BASE_DIR}/death_pose.h"
		"${SHARED_BASE_DIR}/decals.h"
		"${SRCDIR}/public/dlight.h"
		"${SRCDIR}/public/dt_common.h"
		"${SRCDIR}/public/dt_recv.h"
		"${SRCDIR}/public/dt_send.h"
		"${SRCDIR}/public/dt_utlvector_common.h"
		"${SRCDIR}/public/dt_utlvector_send.h"
		"${SHARED_BASE_DIR}/effect_dispatch_data.h"
		"${SHARED_BASE_DIR}/ehandle.h"
		"${SHARED_BASE_DIR}/entitydatainstantiator.h"
		"${SHARED_BASE_DIR}/entitylist_base.h"
		"${SHARED_BASE_DIR}/env_detail_controller.h"
		"${SERVER_BASE_DIR}/EventLog.h"
		"${SHARED_BASE_DIR}/expressionsample.h"
		"${SRCDIR}/public/tier0/fasttimer.h"
		"${SRCDIR}/public/filesystem.h"
		"${SRCDIR}/public/filesystem_helpers.h"
		"${SRCDIR}/public/tier1/fmtstr.h"
		"${SERVER_BASE_DIR}/fogcontroller.h"
		"${SRCDIR}/public/vphysics/friction.h"
		"${SHARED_BASE_DIR}/func_ladder.h"
		"${SHARED_BASE_DIR}/gameeventdefs.h"
		"${SHARED_BASE_DIR}/GameEventListener.h"
		"${SHARED_BASE_DIR}/gamerules_register.h"
		"${SRCDIR}/public/gametrace.h"
		"${SERVER_BASE_DIR}/globals.h"
		"${SRCDIR}/public/globalvars_base.h"
		"${SHARED_BASE_DIR}/groundlink.h"
		"${SRCDIR}/public/iachievementmgr.h"
		"${SRCDIR}/public/appframework/IAppSystem.h"
		"${SRCDIR}/public/icliententity.h"
		"${SRCDIR}/public/iclientnetworkable.h"
		"${SRCDIR}/public/iclientrenderable.h"
		"${SRCDIR}/public/iclientunknown.h"
		"${SRCDIR}/public/engine/ICollideable.h"
		"${SRCDIR}/public/tier0/icommandline.h"
		"${SRCDIR}/public/icvar.h"
		"${SHARED_BASE_DIR}/IEffects.h"
		"${SRCDIR}/public/engine/IEngineSound.h"
		"${SRCDIR}/public/engine/IEngineTrace.h"
		"${SRCDIR}/public/igameevents.h"
		"${SHARED_BASE_DIR}/igamemovement.h"
		"${SRCDIR}/public/ihandleentity.h"
		"${SRCDIR}/public/ihltv.h"
		"${SRCDIR}/public/ihltvdirector.h"
		"${SRCDIR}/public/vstdlib/IKeyValuesSystem.h"
		"${SERVER_BASE_DIR}/ilagcompensationmanager.h"
		"${SRCDIR}/public/vgui/ILocalize.h"
		"${SRCDIR}/public/materialsystem/imaterial.h"
		"${SRCDIR}/public/materialsystem/imaterialsystem.h"
		"${SRCDIR}/public/materialsystem/imaterialvar.h"
		"${SHARED_BASE_DIR}/imovehelper.h"
		"${SHARED_BASE_DIR}/in_buttons.h"
		"${SRCDIR}/public/inetchannelinfo.h"
		"${SHARED_BASE_DIR}/iplayeranimstate.h"
		"${SHARED_BASE_DIR}/ipredictionsystem.h"
		"${SRCDIR}/public/irecipientfilter.h"
		"${SRCDIR}/public/isaverestore.h"
		"${SRCDIR}/public/iscratchpad3d.h"
		"${SRCDIR}/public/iserverentity.h"
		"${SRCDIR}/public/iservernetworkable.h"
		"${SRCDIR}/public/iserverunknown.h"
		"${SRCDIR}/public/SoundEmitterSystem/isoundemittersystembase.h"
		"${SRCDIR}/public/ispatialpartition.h"
		"${SRCDIR}/public/engine/IStaticPropMgr.h"
		"${SHARED_BASE_DIR}/itempents.h"
		"${SRCDIR}/public/engine/ivdebugoverlay.h"
		"${SHARED_BASE_DIR}/IVehicle.h"
		"${SRCDIR}/public/engine/ivmodelinfo.h"
		"${SRCDIR}/public/tier1/KeyValues.h"
		"${SRCDIR}/common/language.h"
		"${SRCDIR}/public/tier0/l2cache.h"
		"${SERVER_BASE_DIR}/logicrelay.h"
		"${SRCDIR}/public/map_utils.h"
		"${SERVER_BASE_DIR}/mapentities.h"
		"${SHARED_BASE_DIR}/mapentities_shared.h"
		"${SRCDIR}/public/tier0/mem.h"
		"${SRCDIR}/public/tier0/memalloc.h"
		"${SRCDIR}/public/tier0/memdbgoff.h"
		"${SRCDIR}/public/tier0/memdbgon.h"
		"${SERVER_BASE_DIR}/modelentities.h"
		"${SHARED_BASE_DIR}/movevars_shared.h"
		"${SRCDIR}/public/networkvar.h"
		"${SERVER_BASE_DIR}/npc_vehicledriver.h"
		"${SHARED_BASE_DIR}/npcevent.h"
		"${SRCDIR}/public/vphysics/object_hash.h"
		"${SERVER_BASE_DIR}/particle_system.h"
		"${SERVER_BASE_DIR}/physics_cannister.h"
		"${SERVER_BASE_DIR}/physics_fx.h"
		"${SERVER_BASE_DIR}/physics_impact_damage.h"
		"${SRCDIR}/public/tier0/platform.h"
		"${SRCDIR}/public/vphysics/player_controller.h"
		"${SERVER_BASE_DIR}/playerinfomanager.h"
		"${SHARED_BASE_DIR}/playernet_vars.h"
		"${SRCDIR}/public/PlayerState.h"
		"${SHARED_BASE_DIR}/precipitation_shared.h"
		"${SHARED_BASE_DIR}/predictable_entity.h"
		"${SHARED_BASE_DIR}/predictioncopy.h"
		"${SHARED_BASE_DIR}/querycache.h"
		"${SHARED_BASE_DIR}/props_shared.h"
		"${SRCDIR}/public/tier0/protected_things.h"
		"${SRCDIR}/public/vstdlib/random.h"
		"${SHARED_BASE_DIR}/rope_helpers.h"
		"${SHARED_BASE_DIR}/saverestore_stringtable.h"
		"${SHARED_BASE_DIR}/saverestore_utlclass.h"
		"${SHARED_BASE_DIR}/saverestore_utlmap.h"
		"${SHARED_BASE_DIR}/saverestore_utlrbtree.h"
		"${SRCDIR}/public/saverestoretypes.h"
		"${SRCDIR}/public/scratchpad3d.h"
		"${SERVER_BASE_DIR}/scratchpad_gamedll_helpers.h"
		"${SRCDIR}/public/ScratchPadUtils.h"
		"${SERVER_BASE_DIR}/sendproxy.h"
		"${SRCDIR}/public/shake.h"
		"${SHARED_BASE_DIR}/shared_classnames.h"
		"${SHARED_BASE_DIR}/shareddefs.h"
		"${SHARED_BASE_DIR}/sharedInterface.h"
		"${SHARED_BASE_DIR}/shot_manipulator.h"
		"${SERVER_BASE_DIR}/SkyCamera.h"
		"${SRCDIR}/public/soundchars.h"
		"${SHARED_BASE_DIR}/soundenvelope.h"
		"${SRCDIR}/public/soundflags.h"
		"${SERVER_BASE_DIR}/soundscape_system.h"
		"${SRCDIR}/public/stdstring.h"
		"${SRCDIR}/public/string_t.h"
		"${SRCDIR}/public/tier1/stringpool.h"
		"${SRCDIR}/public/tier1/strtools.h"
		"${SRCDIR}/public/studio.h"
		"${SHARED_BASE_DIR}/sun_shared.h"
		"${SHARED_BASE_DIR}/takedamageinfo.h"
		"${SERVER_BASE_DIR}/te_effect_dispatch.h"
		"${SERVER_BASE_DIR}/tesla.h"
		"${SERVER_BASE_DIR}/test_stressentities.h"
		"${SERVER_BASE_DIR}/textstatsmgr.h"
		"${SRCDIR}/public/texture_group_names.h"
		"${SERVER_BASE_DIR}/timedeventmgr.h"
		"${SHARED_BASE_DIR}/usercmd.h"
		"${SHARED_BASE_DIR}/usermessages.h"
		"${SHARED_BASE_DIR}/util_shared.h"
		"${SRCDIR}/public/UtlCachedFileData.h"
		"${SRCDIR}/public/tier1/utldict.h"
		"${SRCDIR}/public/tier1/utlfixedmemory.h"
		"${SRCDIR}/public/tier1/utlhash.h"
		"${SRCDIR}/public/tier1/utllinkedlist.h"
		"${SRCDIR}/public/tier1/utlmap.h"
		"${SRCDIR}/public/tier1/utlmemory.h"
		"${SRCDIR}/public/tier1/utlmultilist.h"
		"${SRCDIR}/public/tier1/utlpriorityqueue.h"
		"${SRCDIR}/public/tier1/utlrbtree.h"
		"${SRCDIR}/public/tier1/UtlSortVector.h"
		"${SRCDIR}/public/tier1/utlvector.h"
		"${SERVER_BASE_DIR}/variant_t.h"
		"${SRCDIR}/public/vcollide.h"
		"${SRCDIR}/public/vcollide_parse.h"
		"${SRCDIR}/public/mathlib/vector.h"
		"${SRCDIR}/public/mathlib/vector2d.h"
		"${SRCDIR}/public/mathlib/vector4d.h"
		"${SERVER_BASE_DIR}/vehicle_base.h"
		"${SERVER_BASE_DIR}/vehicle_baseserver.h"
		"${SHARED_BASE_DIR}/vehicle_viewblend_shared.h"
		"${SRCDIR}/public/vphysics/vehicles.h"
		"${SRCDIR}/public/vgui/VGUI.h"
		"${SRCDIR}/public/vphysics_interface.h"
		"${SHARED_BASE_DIR}/vphysics_sound.h"
		"${SRCDIR}/public/mathlib/vplane.h"
		"${SRCDIR}/public/tier0/vprof.h"
		"${SRCDIR}/public/vstdlib/vstdlib.h"
		"${SRCDIR}/public/winlite.h"
		"${SRCDIR}/public/worldsize.h"
		"${SRCDIR}/public/zip_uncompressed.h"
		"${SHARED_BASE_DIR}/mp_shareddefs.h"
		"${SHARED_BASE_DIR}/econ/ihasowner.h"
	#}

	#$Folder	"Tools Framework"
	#{
		"${SERVER_BASE_DIR}/entity_tools_server.cpp"
		"${SERVER_BASE_DIR}/toolframework_server.cpp"
		"${SERVER_BASE_DIR}/toolframework_server.h"
	#}
)

set_source_files_properties(
	"${SRCDIR}/public/bone_setup.cpp"					#/
	"${SRCDIR}/public/collisionutils.cpp"				#/
	"${SRCDIR}/public/dt_send.cpp"						#/
	"${SRCDIR}/public/dt_utlvector_common.cpp"			#/
	"${SRCDIR}/public/dt_utlvector_send.cpp"			#/
	"${SRCDIR}/public/editor_sendcommand.cpp"			#/
	"${SRCDIR}/public/filesystem_helpers.cpp"			#/
	"${SERVER_BASE_DIR}/gamehandle.cpp"					#/
	"${SERVER_BASE_DIR}/h_export.cpp"					#/
	"${SERVER_BASE_DIR}/init_factory.cpp"				#/
	"${SRCDIR}/public/interpolatortypes.cpp"			#/
	"${SHARED_BASE_DIR}/interval.cpp"					#/
	"${SRCDIR}/public/keyframe/keyframe.cpp"			#/
	"${SRCDIR}/common/language.cpp"						#/
	"${SRCDIR}/public/map_utils.cpp"					#/
	"${SRCDIR}/public/networkvar.cpp"					#/
	"${SRCDIR}/common/randoverride.cpp"					#/
	"${SRCDIR}/public/registry.cpp"						#/
	"${SRCDIR}/public/rope_physics.cpp"					#/
	"${SRCDIR}/public/scratchpad3d.cpp"					#/
	"${SRCDIR}/public/ScratchPadUtils.cpp"				#/
	"${SRCDIR}/public/server_class.cpp"					#/
	"${SHARED_BASE_DIR}/sheetsimulator.cpp"				#/
	"${SRCDIR}/public/simple_physics.cpp"				#/
	"${SRCDIR}/public/stringregistry.cpp"				#/
	"${SRCDIR}/public/studio.cpp"						#/
	"${SERVER_BASE_DIR}/GameStats_BasicStatsFunctions.cpp"

	PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

function( target_use_server_base target EXCLUDE_SOURCES )

	set(
		USED_SOURCES
		${SERVER_BASE_SOURCE_FILES}
	)

	if( ${EXCLUDE_SOURCES} )
		list( REMOVE_ITEM USED_SOURCES ${${EXCLUDE_SOURCES}} )
	endif()

	set_property( TARGET ${target} PROPERTY FOLDER "Game" )

	target_sources(
		${target} PRIVATE
		${USED_SOURCES}
	)

	target_include_directories(
		${target} PRIVATE

		"${SERVER_BASE_DIR}"
		"${SERVER_BASE_DIR}/NextBot"
		"${SHARED_BASE_DIR}"
		"${SHARED_BASE_DIR}/econ" # SanyaSho: can we remove this?

		"${SRCDIR}/utils/common"
	)

	target_compile_definitions(
		${target} PRIVATE

		GAME_DLL
		VECTOR
		VERSION_SAFE_STEAM_API_INTERFACES
		PROTECTED_THINGS_ENABLE
		#sprintf=use_Q_snprintf_instead_of_sprintf # TODO:
		#strncpy=use_Q_strncpy_instead # TODO:
		_snprintf=use_Q_snprintf_instead
		$<${IS_POSIX}:SWDS>
		$<${IS_WINDOWS}:fopen=dont_use_fopen>
	)

	target_precompile_headers(
		${target} PRIVATE
		"${SERVER_BASE_DIR}/cbase.h"
	)

	target_link_libraries(
		${target} PRIVATE

		$<${IS_WINDOWS}:winmm>

		choreoobjects
		particles
		dmxloader
		mathlib
		tier2
		tier3
	)

	target_use_steamapi(
		${target}
	)

endfunction()
