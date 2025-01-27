#=============================================================================
#	client_cstrike.cmake
#
#	Project Script
#=============================================================================

include( "${CMAKE_CURRENT_LIST_DIR}/client_base.cmake" )
include( "${SRCDIR}/cmake/misc/vpc_groups.cmake" )

set( CLIENT_CSTRIKE_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	CLIENT_CSTRIKE_SOURCE_FILES

	#$Folder "Replay"
	#{
		"${CLIENT_CSTRIKE_DIR}/cstrike/cs_replay.cpp"
		"${CLIENT_CSTRIKE_DIR}/cstrike/cs_replay.h"
	#}

	#$Folder	"Source Files"
	#{
		#-"${SRCDIR}/game/shared/weapon_parse_default.cpp"
		
		"${CLIENT_CSTRIKE_DIR}/c_team_objectiveresource.cpp"
		"${CLIENT_CSTRIKE_DIR}/c_team_objectiveresource.h"
		"${CLIENT_CSTRIKE_DIR}/c_team_train_watcher.cpp"
		"${CLIENT_CSTRIKE_DIR}/c_team_train_watcher.h"
		"${CLIENT_CSTRIKE_DIR}/hud_base_account.cpp"
		"${CLIENT_CSTRIKE_DIR}/hud_base_account.h"
		"${CLIENT_CSTRIKE_DIR}/hud_voicestatus.cpp"
		"${CLIENT_CSTRIKE_DIR}/hud_baseachievement_tracker.cpp"
		"${CLIENT_CSTRIKE_DIR}/hud_baseachievement_tracker.h"
		"${CLIENT_CSTRIKE_DIR}/hud_vote.h"
		"${CLIENT_CSTRIKE_DIR}/hud_vote.cpp"
		"${SRCDIR}/game/shared/predicted_viewmodel.cpp"
		"${SRCDIR}/game/shared/predicted_viewmodel.h"
		
		#$Folder	"CounterStrike DLL"
		#{
			"${SRCDIR}/game/shared/cstrike/cs_achievement_constants.h"
			"${SRCDIR}/game/shared/cstrike/cs_achievementdefs.h"
			"${SRCDIR}/game/shared/cs_achievements_and_stats_interface.cpp"
			"${SRCDIR}/game/shared/cs_achievements_and_stats_interface.h"
			"${SRCDIR}/game/shared/cstrike/achievements_cs.cpp"
			"${SRCDIR}/game/shared/cstrike/achievements_cs.h"
			"${SRCDIR}/game/shared/cstrike/basecsgrenade_projectile.cpp"
			"${SRCDIR}/game/shared/cstrike/basecsgrenade_projectile.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/buy_presets/buy_preset.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/buy_presets/buy_preset_debug.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/buy_presets/buy_preset_debug.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/buy_presets/buy_preset_weapon_info.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/buy_presets/buy_presets.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/buy_presets/buy_presets.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_cs_hostage.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_cs_hostage.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_cs_player.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_cs_player.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_cs_playerresource.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_cs_playerresource.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_cs_team.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_cs_team.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_csrootpanel.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_csrootpanel.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_plantedc4.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_plantedc4.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_te_radioicon.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/c_te_shotgun_shot.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/clientmode_csnormal.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/clientmode_csnormal.h"
			"${SRCDIR}/game/shared/cstrike/cs_ammodef.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_ammodef.h"
			"${SRCDIR}/game/shared/cstrike/cs_gamemovement.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_gamerules.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_gamerules.h"
			"${SRCDIR}/game/shared/cstrike/cs_gamestats_shared.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_gamestats_shared.h"
			"${SRCDIR}/game/shared/steamworks_gamestats.cpp"
			"${SRCDIR}/game/shared/steamworks_gamestats.h"			
			"${CLIENT_CSTRIKE_DIR}/cstrike/cs_in_main.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_player_shared.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_playeranimstate.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_playeranimstate.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/cs_prediction.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_shareddefs.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/cs_client_gamestats.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/cs_client_gamestats.h"
			"${SRCDIR}/game/shared/cstrike/cs_usermessages.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/cs_view_scene.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/cs_view_scene.h"
			"${SRCDIR}/game/shared/cstrike/cs_weapon_parse.cpp"
			"${SRCDIR}/game/shared/cstrike/cs_weapon_parse.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/fx_cs_blood.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/fx_cs_blood.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/fx_cs_impacts.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/fx_cs_knifeslash.cpp"
			"${CLIENT_CSTRIKE_DIR}/cstrike/fx_cs_muzzleflash.cpp"
			"${SRCDIR}/game/shared/cstrike/fx_cs_shared.cpp"
			"${SRCDIR}/game/shared/cstrike/fx_cs_shared.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/fx_cs_weaponfx.cpp"
			"${SRCDIR}/game/shared/cstrike/bot/shared_util.cpp"
			"${SRCDIR}/game/shared/cstrike/bot/shared_util.h"
			"${CLIENT_CSTRIKE_DIR}/cstrike/vgui_rootpanel_cs.cpp"
			
			#$Folder "HUD Elements"
			#{
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_ammo.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_chat.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_chat.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_damageindicator.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_freezepanel.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_freezepanel.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_playerhealth.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_playerhealth.h"			
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_health.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_scope.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_target_id.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_weaponselection.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_account.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_armor.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_c4.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_deathnotice.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_defuser.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_flashbang.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_hostagerescue.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_progressbar.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_radar.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_radar.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_roundtimer.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_scenarioicon.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/hud_shopping_cart.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_achievement_announce.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_achievement_announce.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/cs_hud_achievement_tracker.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/radio_status.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/radio_status.h"
			#}
			
			#$Folder "Weapon"
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
				"${SRCDIR}/game/shared/cstrike/weapon_knife.h"
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
				"${SRCDIR}/game/shared/cstrike/weapon_smokegrenade.h"
				"${SRCDIR}/game/shared/cstrike/weapon_tmp.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_ump45.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_usp.cpp"
				"${SRCDIR}/game/shared/cstrike/weapon_xm1014.cpp"
			#}

			#$Folder	"vgui"
			#{
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/achievement_stats_summary.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/achievement_stats_summary.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/achievements_page.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/achievements_page.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/stats_summary.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/stats_summary.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/stat_card.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/stat_card.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/base_stats_page.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/base_stats_page.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/match_stats_page.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/match_stats_page.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/lifetime_stats_page.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/lifetime_stats_page.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/bordered_panel.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/bordered_panel.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/backgroundpanel.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/backgroundpanel.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/buymouseoverpanelbutton.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/buypreset_imageinfo.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/buypreset_listbox.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/buypreset_listbox.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/buypreset_panel.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/buypreset_weaponsetlabel.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/career_box.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/career_box.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/career_button.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/career_button.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/counterstrikeviewport.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/counterstrikeviewport.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstrikebuyequipmenu.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstrikebuyequipmenu.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstrikebuymenu.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstrikebuymenu.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstrikebuysubmenu.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstrikeclassmenu.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstrikeclassmenu.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstrikeclientscoreboard.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstrikeclientscoreboard.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstrikespectatorgui.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstrikespectatorgui.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstriketeammenu.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstriketeammenu.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstriketextwindow.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/cstriketextwindow.h"
				"${CLIENT_CSTRIKE_DIR}/cstrike/vgui_c4panel.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/vgui_viewc4panel.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/win_panel_round.cpp"
				"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI/win_panel_round.h"
			#}

			#$Folder	"NextBot"
			#{
				"${CLIENT_CSTRIKE_DIR}/NextBot/C_NextBot.cpp"
				"${CLIENT_CSTRIKE_DIR}/NextBot/C_NextBot.h"
			#}
		#}

		#$Folder	"game_controls"
		#{
			"${CLIENT_CSTRIKE_DIR}/game_controls/buymenu.cpp"
			"${CLIENT_CSTRIKE_DIR}/game_controls/buysubmenu.cpp"
			"${CLIENT_CSTRIKE_DIR}/game_controls/classmenu.cpp"
		#}
	#}
	
	#$Folder	"Header Files"
	#{
		#$Folder	"game_controls header files"
		#{
			"${CLIENT_CSTRIKE_DIR}/game_controls/buymenu.h"
			"${CLIENT_CSTRIKE_DIR}/game_controls/buysubmenu.h"
			"${CLIENT_CSTRIKE_DIR}/game_controls/classmenu.h"
		#}
	#}
)

set(
	CLIENT_CSTRIKE_EXCLUDE_SOURCES
	"${SHARED_BASE_DIR}/weapon_parse_default.cpp"
)

add_library(
	client_cstrike MODULE
	${CLIENT_CSTRIKE_SOURCE_FILES}
)

if( ${IS_WINDOWS} )
target_group_sources(
	client_cstrike
)
endif()

set_install_properties(
	client_cstrike
	""
	"client"
	"${GAMEDIR}/${BUILD_FOLDER}/bin"
)

target_include_directories(
	client_cstrike PRIVATE

	"${SRCDIR}/game/shared/cstrike/control"
	"${CLIENT_CSTRIKE_DIR}/cstrike"
	"${CLIENT_CSTRIKE_DIR}/cstrike/control"
	"${CLIENT_CSTRIKE_DIR}/cstrike/VGUI"
	"${SRCDIR}/game/shared/cstrike"
)

target_compile_definitions(
	client_cstrike PRIVATE

	CSTRIKE_DLL
	NEXT_BOT
)

target_link_libraries(
	client_cstrike PRIVATE

	vtf
)

target_use_client_base(
	client_cstrike
	CLIENT_CSTRIKE_EXCLUDE_SOURCES
)
