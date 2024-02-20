#=============================================================================
#	GameUI.cmake
#
#	Project Script
#=============================================================================

set( GAMEUI_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	GAMEUI_SOURCE_FILES

	#$Folder	"Source Files"
	#{
		"${GAMEUI_DIR}/BackgroundMenuButton.cpp"
		"${GAMEUI_DIR}/BasePanel.cpp"
		"${GAMEUI_DIR}/GameConsole.cpp"
		"${GAMEUI_DIR}/GameUI_Interface.cpp"
		"${GAMEUI_DIR}/ModInfo.cpp"
		"${GAMEUI_DIR}/MouseMessageForwardingPanel.cpp"
		"$<$<NOT:${IS_POSIX}>:${SRCDIR}/Tracker/common/msgbuffer.cpp>"
		"$<$<NOT:${IS_POSIX}>:${SRCDIR}/Tracker/common/netapi.cpp>"
		"${SRCDIR}/common/GameUI/ObjectList.cpp"
		"${GAMEUI_DIR}/PanelListPanel.cpp"
		"${GAMEUI_DIR}/RunGameEngine.cpp"
		"${SRCDIR}/common/GameUI/scriptobject.cpp"
		"$<$<NOT:${IS_POSIX}>:${SRCDIR}/Tracker/common/Socket.cpp>"
		"${GAMEUI_DIR}/Sys_Utils.cpp"
		"${GAMEUI_DIR}/TextEntryBox.cpp"
		"${GAMEUI_DIR}/TGAImagePanel.cpp"
		"${SRCDIR}/public/vgui_controls/vgui_controls.cpp"
		"${GAMEUI_DIR}/VGuiSystemModuleLoader.cpp"
		"${GAMEUI_DIR}/BonusMapsDatabase.cpp"
		"${GAMEUI_DIR}/BonusMapsDatabase.h"
		"${SRCDIR}/common/language.cpp"
		"${SRCDIR}/common/imageutils.cpp"
		"${GAMEUI_DIR}/SaveGameBrowserDialog.cpp"
		"${GAMEUI_DIR}/gameui_util.cpp"
		"${GAMEUI_DIR}/gameui_util.h"
		"$<$<NOT:${IS_POSIX}>:${GAMEUI_DIR}/ChangeGameDialog.cpp>"
		"$<$<NOT:${IS_POSIX}>:${GAMEUI_DIR}/ChangeGameDialog.h>"
	#}

	#$Folder	"Header Files"
	#{
		"${GAMEUI_DIR}/BackgroundMenuButton.h"
		"${GAMEUI_DIR}/BasePanel.h"
		"${GAMEUI_DIR}/BaseSaveGameDialog.h"
		"${GAMEUI_DIR}/CreateMultiplayerGameBotPage.h"
		"${GAMEUI_DIR}/CreateMultiplayerGameDialog.h"
		"${GAMEUI_DIR}/CreateMultiplayerGameGameplayPage.h"
		"${GAMEUI_DIR}/CreateMultiplayerGameServerPage.h"
		"${GAMEUI_DIR}/EngineInterface.h"
		"${GAMEUI_DIR}/GameConsole.h"
		"${GAMEUI_DIR}/GameUI_Interface.h"
		"${GAMEUI_DIR}/LoadingDialog.h"
		"${SRCDIR}/vgui2/src/Memorybitmap.h"
		"${GAMEUI_DIR}/ModInfo.h"
		"${GAMEUI_DIR}/MouseMessageForwardingPanel.h"
		"${GAMEUI_DIR}/PanelListPanel.h"
		"${SRCDIR}/common/GameUI/scriptobject.h"
		"${GAMEUI_DIR}/Sys_Utils.h"
		"${GAMEUI_DIR}/TextEntryBox.h"
		"${GAMEUI_DIR}/TGAImagePanel.h"
		"${GAMEUI_DIR}/VGuiSystemModuleLoader.h"
		"${GAMEUI_DIR}/SaveGameBrowserDialog.h"
	#}

	#$Folder	"Public Header Files"
	#{
		"${SRCDIR}/public/iachievementmgr.h"
		"${SRCDIR}/public/game/client/IGameClientExports.h"
		"${SRCDIR}/common/GameUI/IGameUI.h"
		"${SRCDIR}/public/IGameUIFuncs.h"
		"${SRCDIR}/public/tier1/interface.h"
		"${SRCDIR}/common/IObjectContainer.h"
		"${SRCDIR}/common/IRunGameEngine.h"
		"${SRCDIR}/common/IVguiModule.h"
		"${SRCDIR}/common/IVGuiModuleLoader.h"
		"${SRCDIR}/common/GameUI/ObjectList.h"
		"${SRCDIR}/public/savegame_version.h"
		"${SRCDIR}/tracker/common/TrackerMessageFlags.h"
		"${SRCDIR}/common/language.h"
		"${SRCDIR}/common/imageutils.h"
	#}

	#$Folder	"Controls"
	#{
		"${GAMEUI_DIR}/BitmapImagePanel.cpp"
		"${GAMEUI_DIR}/BitmapImagePanel.h"
		"${GAMEUI_DIR}/CommandCheckButton.cpp"
		"${GAMEUI_DIR}/CommandCheckButton.h"
		"${GAMEUI_DIR}/CvarNegateCheckButton.cpp"
		"${GAMEUI_DIR}/CvarNegateCheckButton.h"
		"${SRCDIR}/common/GameUI/cvarslider.cpp"
		"${SRCDIR}/common/GameUI/cvarslider.h"
		"${GAMEUI_DIR}/CvarTextEntry.cpp"
		"${GAMEUI_DIR}/CvarTextEntry.h"
		"${GAMEUI_DIR}/CvarToggleCheckButton.cpp"
		"${GAMEUI_DIR}/CvarToggleCheckButton.h"
		"${GAMEUI_DIR}/KeyToggleCheckButton.cpp"
		"${GAMEUI_DIR}/KeyToggleCheckButton.h"
		"${GAMEUI_DIR}/LabeledCommandComboBox.cpp"
		"${GAMEUI_DIR}/LabeledCommandComboBox.h"
		"${GAMEUI_DIR}/URLButton.cpp"
		"${GAMEUI_DIR}/URLButton.h"
		"${GAMEUI_DIR}/vcontrolslistpanel.cpp"
		"${GAMEUI_DIR}/vcontrolslistpanel.h"
	#}

	#$Folder	"Dialogs"
	#{
		"${GAMEUI_DIR}/BenchmarkDialog.cpp"
		"${GAMEUI_DIR}/BenchmarkDialog.h"
		"${GAMEUI_DIR}/BonusMapsDialog.cpp"
		"${GAMEUI_DIR}/BonusMapsDialog.h"
		"${GAMEUI_DIR}/CommentaryDialog.cpp"
		"${GAMEUI_DIR}/CommentaryDialog.h"
		"${GAMEUI_DIR}/CommentaryExplanationDialog.cpp"
		"${GAMEUI_DIR}/CommentaryExplanationDialog.h"
		"${GAMEUI_DIR}/CustomTabExplanationDialog.cpp"
		"${GAMEUI_DIR}/CustomTabExplanationDialog.h"
		"${GAMEUI_DIR}/GameConsoleDialog.cpp"
		"${GAMEUI_DIR}/GameConsoleDialog.h"
		"${GAMEUI_DIR}/LoadGameDialog_Xbox.cpp"
		"${GAMEUI_DIR}/LoadGameDialog.cpp"
		"${GAMEUI_DIR}/LoadGameDialog.h"
		"${GAMEUI_DIR}/MultiplayerAdvancedDialog.cpp"
		"${GAMEUI_DIR}/MultiplayerAdvancedDialog.h"
		"${GAMEUI_DIR}/NewGameDialog.cpp"
		"${GAMEUI_DIR}/NewGameDialog.h"
		"${GAMEUI_DIR}/PlayerListDialog.cpp"
		"${GAMEUI_DIR}/PlayerListDialog.h"
		"${GAMEUI_DIR}/SaveGameDialog_Xbox.cpp"
		"${GAMEUI_DIR}/SaveGameDialog.cpp"
		"${GAMEUI_DIR}/SaveGameDialog.h"
		"${GAMEUI_DIR}/LoadCommentaryDialog.cpp"
		"${GAMEUI_DIR}/LoadingDialog.cpp"
		"${GAMEUI_DIR}/BaseSaveGameDialog.cpp"
		"${GAMEUI_DIR}/CreateMultiplayerGameBotPage.cpp"
		"${GAMEUI_DIR}/CreateMultiplayerGameDialog.cpp"
		"${GAMEUI_DIR}/CreateMultiplayerGameGameplayPage.cpp"
		"${GAMEUI_DIR}/CreateMultiplayerGameServerPage.cpp"
		"${GAMEUI_DIR}/OptionsDialog_Xbox.cpp"
		"${GAMEUI_DIR}/ControllerDialog.cpp"
		"${GAMEUI_DIR}/ControllerDialog.h"
	#}

	#$Folder	"Matchmaking"
	#{
		"${GAMEUI_DIR}/matchmaking/achievementsdialog.cpp"
		"${GAMEUI_DIR}/matchmaking/achievementsdialog.h"
		"${GAMEUI_DIR}/matchmaking/basedialog.cpp"
		"${GAMEUI_DIR}/matchmaking/basedialog.h"
		"${GAMEUI_DIR}/matchmaking/dialogmenu.cpp"
		"${GAMEUI_DIR}/matchmaking/dialogmenu.h"
		"${GAMEUI_DIR}/matchmaking/leaderboarddialog.cpp"
		"${GAMEUI_DIR}/matchmaking/leaderboarddialog.h"
		"${GAMEUI_DIR}/matchmaking/matchmakingbasepanel.cpp"
		"${GAMEUI_DIR}/matchmaking/matchmakingbasepanel.h"
		"${GAMEUI_DIR}/matchmaking/pausedialog.cpp"
		"${GAMEUI_DIR}/matchmaking/pausedialog.h"
		"${GAMEUI_DIR}/matchmaking/sessionlobbydialog.cpp"
		"${GAMEUI_DIR}/matchmaking/sessionlobbydialog.h"
		"${GAMEUI_DIR}/matchmaking/sessionoptionsdialog.cpp"
		"${GAMEUI_DIR}/matchmaking/sessionoptionsdialog.h"
		"${GAMEUI_DIR}/matchmaking/sessionbrowserdialog.cpp"
		"${GAMEUI_DIR}/matchmaking/sessionbrowserdialog.h"
		"${GAMEUI_DIR}/matchmaking/welcomedialog.cpp"
		"${GAMEUI_DIR}/matchmaking/welcomedialog.h"
	#}

	#$Folder	"Options Dialog"
	#{
		"${GAMEUI_DIR}/OptionsDialog.cpp"
		"${GAMEUI_DIR}/OptionsDialog.h"
		"${GAMEUI_DIR}/OptionsSubAudio.cpp"
		"${GAMEUI_DIR}/OptionsSubAudio.h"
		"${GAMEUI_DIR}/OptionsSubDifficulty.cpp"
		"${GAMEUI_DIR}/OptionsSubDifficulty.h"
		"${GAMEUI_DIR}/OptionsSubGame.cpp"
		"${GAMEUI_DIR}/OptionsSubGame.h"
		"${GAMEUI_DIR}/OptionsSubKeyboard.cpp"
		"${GAMEUI_DIR}/OptionsSubKeyboard.h"
		"${GAMEUI_DIR}/OptionsSubMouse.cpp"
		"${GAMEUI_DIR}/OptionsSubMouse.h"
		"${GAMEUI_DIR}/OptionsSubTouch.cpp"
		"${GAMEUI_DIR}/OptionsSubTouch.h"
		"${GAMEUI_DIR}/OptionsSubMultiplayer.cpp"
		"${GAMEUI_DIR}/OptionsSubMultiplayer.h"
		"${GAMEUI_DIR}/OptionsSubPortal.cpp"
		"${GAMEUI_DIR}/OptionsSubPortal.h"
		"${GAMEUI_DIR}/OptionsSubVideo.cpp"
		"${GAMEUI_DIR}/OptionsSubVideo.h"
		"${GAMEUI_DIR}/OptionsSubVoice.cpp"
		"${GAMEUI_DIR}/OptionsSubVoice.h"
	#}
)

source_group(
	"Source Files"

	FILES
	#$Folder	"Source Files"
	#{
		"${GAMEUI_DIR}/BackgroundMenuButton.cpp"
		"${GAMEUI_DIR}/BasePanel.cpp"
		"${GAMEUI_DIR}/GameConsole.cpp"
		"${GAMEUI_DIR}/GameUI_Interface.cpp"
		"${GAMEUI_DIR}/ModInfo.cpp"
		"${GAMEUI_DIR}/MouseMessageForwardingPanel.cpp"
		"${SRCDIR}/Tracker/common/msgbuffer.cpp"
		"${SRCDIR}/Tracker/common/netapi.cpp"
		"${SRCDIR}/common/GameUI/ObjectList.cpp"
		"${GAMEUI_DIR}/PanelListPanel.cpp"
		"${GAMEUI_DIR}/RunGameEngine.cpp"
		"${SRCDIR}/common/GameUI/scriptobject.cpp"
		"${SRCDIR}/Tracker/common/Socket.cpp"
		"${GAMEUI_DIR}/Sys_Utils.cpp"
		"${GAMEUI_DIR}/TextEntryBox.cpp"
		"${GAMEUI_DIR}/TGAImagePanel.cpp"
		"${SRCDIR}/public/vgui_controls/vgui_controls.cpp"
		"${GAMEUI_DIR}/VGuiSystemModuleLoader.cpp"
		"${GAMEUI_DIR}/BonusMapsDatabase.cpp"
		"${GAMEUI_DIR}/BonusMapsDatabase.h"
		"${SRCDIR}/common/language.cpp"
		"${SRCDIR}/common/imageutils.cpp"
		"${GAMEUI_DIR}/SaveGameBrowserDialog.cpp"
		"${GAMEUI_DIR}/gameui_util.cpp"
		"${GAMEUI_DIR}/gameui_util.h"
	#}
)

source_group(
	"Header Files"

	FILES
	#$Folder	"Header Files"
	#{
		"${GAMEUI_DIR}/BackgroundMenuButton.h"
		"${GAMEUI_DIR}/BasePanel.h"
		"${GAMEUI_DIR}/BaseSaveGameDialog.h"
		"${GAMEUI_DIR}/CreateMultiplayerGameBotPage.h"
		"${GAMEUI_DIR}/CreateMultiplayerGameDialog.h"
		"${GAMEUI_DIR}/CreateMultiplayerGameGameplayPage.h"
		"${GAMEUI_DIR}/CreateMultiplayerGameServerPage.h"
		"${GAMEUI_DIR}/EngineInterface.h"
		"${GAMEUI_DIR}/GameConsole.h"
		"${GAMEUI_DIR}/GameUI_Interface.h"
		"${GAMEUI_DIR}/LoadingDialog.h"
		"${SRCDIR}/vgui2/src/Memorybitmap.h"
		"${GAMEUI_DIR}/ModInfo.h"
		"${GAMEUI_DIR}/MouseMessageForwardingPanel.h"
		"${GAMEUI_DIR}/PanelListPanel.h"
		"${SRCDIR}/common/GameUI/scriptobject.h"
		"${GAMEUI_DIR}/Sys_Utils.h"
		"${GAMEUI_DIR}/TextEntryBox.h"
		"${GAMEUI_DIR}/TGAImagePanel.h"
		"${GAMEUI_DIR}/VGuiSystemModuleLoader.h"
		"${GAMEUI_DIR}/SaveGameBrowserDialog.h"
	#}
)

source_group(
	"Public Header Files"

	FILES
	#$Folder	"Public Header Files"
	#{
		"${SRCDIR}/public/iachievementmgr.h"
		"${SRCDIR}/public/game/client/IGameClientExports.h"
		"${SRCDIR}/common/GameUI/IGameUI.h"
		"${SRCDIR}/public/IGameUIFuncs.h"
		"${SRCDIR}/public/tier1/interface.h"
		"${SRCDIR}/common/IObjectContainer.h"
		"${SRCDIR}/common/IRunGameEngine.h"
		"${SRCDIR}/common/IVguiModule.h"
		"${SRCDIR}/common/IVGuiModuleLoader.h"
		"${SRCDIR}/common/GameUI/ObjectList.h"
		"${SRCDIR}/public/savegame_version.h"
		"${SRCDIR}/tracker/common/TrackerMessageFlags.h"
		"${SRCDIR}/common/language.h"
		"${SRCDIR}/common/imageutils.h"
	#}
)

source_group(
	"Controls"

	FILES
	#$Folder	"Controls"
	#{
		"${GAMEUI_DIR}/BitmapImagePanel.cpp"
		"${GAMEUI_DIR}/BitmapImagePanel.h"
		"${GAMEUI_DIR}/CommandCheckButton.cpp"
		"${GAMEUI_DIR}/CommandCheckButton.h"
		"${GAMEUI_DIR}/CvarNegateCheckButton.cpp"
		"${GAMEUI_DIR}/CvarNegateCheckButton.h"
		"${GAMEUI_DIR}/cvarslider.cpp"
		"${GAMEUI_DIR}/cvarslider.h"
		"${GAMEUI_DIR}/CvarTextEntry.cpp"
		"${GAMEUI_DIR}/CvarTextEntry.h"
		"${GAMEUI_DIR}/CvarToggleCheckButton.cpp"
		"${GAMEUI_DIR}/CvarToggleCheckButton.h"
		"${GAMEUI_DIR}/KeyToggleCheckButton.cpp"
		"${GAMEUI_DIR}/KeyToggleCheckButton.h"
		"${GAMEUI_DIR}/LabeledCommandComboBox.cpp"
		"${GAMEUI_DIR}/LabeledCommandComboBox.h"
		"${GAMEUI_DIR}/URLButton.cpp"
		"${GAMEUI_DIR}/URLButton.h"
		"${GAMEUI_DIR}/vcontrolslistpanel.cpp"
		"${GAMEUI_DIR}/vcontrolslistpanel.h"
	#}
)

source_group(
	"Dialogs"

	FILES
	#$Folder	"Dialogs"
	#{
		"${GAMEUI_DIR}/BenchmarkDialog.cpp"
		"${GAMEUI_DIR}/BenchmarkDialog.h"
		"${GAMEUI_DIR}/BonusMapsDialog.cpp"
		"${GAMEUI_DIR}/BonusMapsDialog.h"
		"${GAMEUI_DIR}/CommentaryDialog.cpp"
		"${GAMEUI_DIR}/CommentaryDialog.h"
		"${GAMEUI_DIR}/CommentaryExplanationDialog.cpp"
		"${GAMEUI_DIR}/CommentaryExplanationDialog.h"
		"${GAMEUI_DIR}/CustomTabExplanationDialog.cpp"
		"${GAMEUI_DIR}/CustomTabExplanationDialog.h"
		"${GAMEUI_DIR}/GameConsoleDialog.cpp"
		"${GAMEUI_DIR}/GameConsoleDialog.h"
		"${GAMEUI_DIR}/LoadGameDialog_Xbox.cpp"
		"${GAMEUI_DIR}/LoadGameDialog.cpp"
		"${GAMEUI_DIR}/LoadGameDialog.h"
		"${GAMEUI_DIR}/MultiplayerAdvancedDialog.cpp"
		"${GAMEUI_DIR}/MultiplayerAdvancedDialog.h"
		"${GAMEUI_DIR}/NewGameDialog.cpp"
		"${GAMEUI_DIR}/NewGameDialog.h"
		"${GAMEUI_DIR}/PlayerListDialog.cpp"
		"${GAMEUI_DIR}/PlayerListDialog.h"
		"${GAMEUI_DIR}/SaveGameDialog_Xbox.cpp"
		"${GAMEUI_DIR}/SaveGameDialog.cpp"
		"${GAMEUI_DIR}/SaveGameDialog.h"
		"${GAMEUI_DIR}/LoadCommentaryDialog.cpp"
		"${GAMEUI_DIR}/LoadingDialog.cpp"
		"${GAMEUI_DIR}/BaseSaveGameDialog.cpp"
		"${GAMEUI_DIR}/CreateMultiplayerGameBotPage.cpp"
		"${GAMEUI_DIR}/CreateMultiplayerGameDialog.cpp"
		"${GAMEUI_DIR}/CreateMultiplayerGameGameplayPage.cpp"
		"${GAMEUI_DIR}/CreateMultiplayerGameServerPage.cpp"
		"${GAMEUI_DIR}/OptionsDialog_Xbox.cpp"
		"${GAMEUI_DIR}/ControllerDialog.cpp"
		"${GAMEUI_DIR}/ControllerDialog.h"
	#}
)

source_group(
	"Matchmaking"

	FILES
	#$Folder	"Matchmaking"
	#{
		"${GAMEUI_DIR}/matchmaking/achievementsdialog.cpp"
		"${GAMEUI_DIR}/matchmaking/achievementsdialog.h"
		"${GAMEUI_DIR}/matchmaking/basedialog.cpp"
		"${GAMEUI_DIR}/matchmaking/basedialog.h"
		"${GAMEUI_DIR}/matchmaking/dialogmenu.cpp"
		"${GAMEUI_DIR}/matchmaking/dialogmenu.h"
		"${GAMEUI_DIR}/matchmaking/leaderboarddialog.cpp"
		"${GAMEUI_DIR}/matchmaking/leaderboarddialog.h"
		"${GAMEUI_DIR}/matchmaking/matchmakingbasepanel.cpp"
		"${GAMEUI_DIR}/matchmaking/matchmakingbasepanel.h"
		"${GAMEUI_DIR}/matchmaking/pausedialog.cpp"
		"${GAMEUI_DIR}/matchmaking/pausedialog.h"
		"${GAMEUI_DIR}/matchmaking/sessionlobbydialog.cpp"
		"${GAMEUI_DIR}/matchmaking/sessionlobbydialog.h"
		"${GAMEUI_DIR}/matchmaking/sessionoptionsdialog.cpp"
		"${GAMEUI_DIR}/matchmaking/sessionoptionsdialog.h"
		"${GAMEUI_DIR}/matchmaking/sessionbrowserdialog.cpp"
		"${GAMEUI_DIR}/matchmaking/sessionbrowserdialog.h"
		"${GAMEUI_DIR}/matchmaking/welcomedialog.cpp"
		"${GAMEUI_DIR}/matchmaking/welcomedialog.h"
	#}
)

if( IS_GAME_HL2MP )
	source_group(
		"Custom Options Dialog"

		FILES
		#$Folder "Custom Options Dialog"
		#{
			"${GAMEUI_DIR}/CustomOptionsDialog.cpp"
			"${GAMEUI_DIR}/CustomOptionsDialog.h"
			"${GAMEUI_DIR}/CustomOptionsSubHL2MP.cpp"
			"${GAMEUI_DIR}/CustomOptionsSubHL2MP.h"
			"${GAMEUI_DIR}/CustomOptionsSubGameplay.cpp"
			"${GAMEUI_DIR}/CustomOptionsSubGameplay.h"
		#}
	)
endif()

source_group(
	"Options Dialog"

	FILES
	#$Folder	"Options Dialog"
	#{
		"${GAMEUI_DIR}/OptionsDialog.cpp"
		"${GAMEUI_DIR}/OptionsDialog.h"
		"${GAMEUI_DIR}/OptionsSubAudio.cpp"
		"${GAMEUI_DIR}/OptionsSubAudio.h"
		"${GAMEUI_DIR}/OptionsSubDifficulty.cpp"
		"${GAMEUI_DIR}/OptionsSubDifficulty.h"
		"${GAMEUI_DIR}/OptionsSubGame.cpp"
		"${GAMEUI_DIR}/OptionsSubGame.h"
		"${GAMEUI_DIR}/OptionsSubKeyboard.cpp"
		"${GAMEUI_DIR}/OptionsSubKeyboard.h"
		"${GAMEUI_DIR}/OptionsSubMouse.cpp"
		"${GAMEUI_DIR}/OptionsSubMouse.h"
		"${GAMEUI_DIR}/OptionsSubTouch.cpp"
		"${GAMEUI_DIR}/OptionsSubTouch.h"
		"${GAMEUI_DIR}/OptionsSubMultiplayer.cpp"
		"${GAMEUI_DIR}/OptionsSubMultiplayer.h"
		"${GAMEUI_DIR}/OptionsSubPortal.cpp"
		"${GAMEUI_DIR}/OptionsSubPortal.h"
		"${GAMEUI_DIR}/OptionsSubVideo.cpp"
		"${GAMEUI_DIR}/OptionsSubVideo.h"
		"${GAMEUI_DIR}/OptionsSubVoice.cpp"
		"${GAMEUI_DIR}/OptionsSubVoice.h"
	#}
)

add_library(
	gameui SHARED
	${GAMEUI_SOURCE_FILES}
)

set_property(
	TARGET gameui PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	gameui
	""
	"GameUI"
	"${GAMEDIR}/bin"
)

target_include_directories(
	gameui PRIVATE

	"${GAMEUI_DIR}"
	"${SRCDIR}/vgui2/include"
	"${SRCDIR}/vgui2/controls"
	"${SRCDIR}/common/GameUI"
	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
	"${THIRDPARTYDIR}/libjpeg-turbo-3.0.1"
)

target_compile_definitions(
	gameui PRIVATE

	GAMEUI_EXPORTS
	VERSION_SAFE_STEAM_API_INTERFACES
)

target_link_libraries(
	gameui PRIVATE

	"$<${IS_WINDOWS}:ws2_32>"
	"$<${IS_WINDOWS}:odbc32>"
	"$<${IS_WINDOWS}:odbccp32>"
	"$<${IS_WINDOWS}:shlwapi>"

	#"$<${IS_OSX}:iconv>" # SanyaSho: iconv is linked with tier1
	"$<${IS_OSX}:z>"

	"$<$<AND:${IS_LINUX},$<NOT:${IS_DEDICATED}>,$<NOT:${IS_ANDROID}>>:rt>"

	bitmap
	matsys_controls
	tier2
	tier3
	vgui_controls
	vtf
	mathlib
	"$<$<AND:$<NOT:${IS_DEDICATED}>,${IS_WINDOWS}>:${LIBCOMMON}/libjpeg${STATIC_LIB_EXT}>"
	"$<$<AND:$<NOT:${IS_DEDICATED}>,${IS_POSIX}>:jpeg>"
	# steam_api
	"$<$<AND:$<NOT:${IS_DEDICATED}>,${IS_WINDOWS}>:${LIBCOMMON}/libpng${STATIC_LIB_EXT}>"
	"$<$<AND:$<NOT:${IS_DEDICATED}>,${IS_POSIX}>:png>"
	"$<$<NOT:${IS_DEDICATED}>:libz${STATIC_LIB_EXT}>"
)

target_use_steamapi(
	gameui
)

if( ${USE_SDL} )
	target_use_sdl2(
		gameui
	)
endif()
