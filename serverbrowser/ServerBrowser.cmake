#=============================================================================
#	serverbrowser.cmake
#
#	Project Script
#=============================================================================

set( SERVERBROWSER_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	SERVERBROWSER_SOURCE_FILES

	# Source Files
	"${SERVERBROWSER_DIR}/BaseGamesPage.cpp"
	#"${SERVERBROWSER_DIR}/BlacklistedServers.cpp"
	#"${SERVERBROWSER_DIR}/CustomGames.cpp"
	"${SERVERBROWSER_DIR}/DialogAddServer.cpp"
	"${SERVERBROWSER_DIR}/DialogGameInfo.cpp"
	"${SERVERBROWSER_DIR}/DialogServerPassword.cpp"
	"${SERVERBROWSER_DIR}/FavoriteGames.cpp"
	#"${SERVERBROWSER_DIR}/FriendsGames.cpp"
	"${SERVERBROWSER_DIR}/HistoryGames.cpp"
	"${SERVERBROWSER_DIR}/InternetGames.cpp"
	"${SERVERBROWSER_DIR}/LanGames.cpp"
	"${SERVERBROWSER_DIR}/ModList.cpp"
	"${SERVERBROWSER_DIR}/ServerBrowser.cpp"
	"${SERVERBROWSER_DIR}/ServerBrowserDialog.cpp"
	"${SERVERBROWSER_DIR}/ServerContextMenu.cpp"
	"${SERVERBROWSER_DIR}/ServerListCompare.cpp"
	#"${SERVERBROWSER_DIR}/SpectateGames.cpp"
	"${SERVERBROWSER_DIR}/VACBannedConnRefusedDialog.cpp"
	"${SERVERBROWSER_DIR}/VACBannedConnRefusedDialog.h"

	"${SERVERBROWSER_DIR}/QuickListPanel.cpp"

	"${SRCDIR}/public/vgui_controls/vgui_controls.cpp"

	"${SRCDIR}/common/ServerBrowser/blacklisted_server_manager.cpp"

	# Header Files
	"${SERVERBROWSER_DIR}/BaseGamesPage.h"
	#"${SERVERBROWSER_DIR}/BlacklistedServers.h"
	#"${SERVERBROWSER_DIR}/CustomGames.h"
	"${SERVERBROWSER_DIR}/DialogAddServer.h"
	"${SERVERBROWSER_DIR}/DialogGameInfo.h"
	"${SERVERBROWSER_DIR}/DialogServerPassword.h"
	"${SERVERBROWSER_DIR}/FavoriteGames.h"
	#"${SERVERBROWSER_DIR}/FriendsGames.h"
	"${SERVERBROWSER_DIR}/HistoryGames.h"
	"${SERVERBROWSER_DIR}/InternetGames.h"
	"${SERVERBROWSER_DIR}/LanGames.h"
	"${SERVERBROWSER_DIR}/ModList.h"
	"${SERVERBROWSER_DIR}/ServerBrowser.h"
	"${SERVERBROWSER_DIR}/ServerBrowserDialog.h"
	"${SERVERBROWSER_DIR}/ServerContextMenu.h"
	"${SERVERBROWSER_DIR}/ServerListCompare.h"
	#"${SERVERBROWSER_DIR}/SpectateGames.h"
	"${SERVERBROWSER_DIR}/QuickListPanel.h"

	"${SRCDIR}/common/ServerBrowser/blacklisted_server_manager.h"

	# Exposed interfaces
	"${SRCDIR}/common/ServerBrowser/IServerBrowser.h"
)

add_library(
	serverbrowser SHARED
	${SERVERBROWSER_SOURCE_FILES}
)

set_property(
	TARGET serverbrowser PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	serverbrowser
	""
	"ServerBrowser"
	"${GAMEDIR}/bin"
)

target_include_directories(
	serverbrowser PRIVATE

	"${SRCDIR}/game/shared"
)

target_compile_definitions(
	serverbrowser PRIVATE

	VERSION_SAFE_STEAM_API_INTERFACES
	SERVERBROWSER_EXPORTS
	GAME_SRC
	$<$<NOT:${IS_64BIT}>:_USE_32BIT_TIME_T>
)

target_link_libraries(
	serverbrowser PRIVATE

	"$<${IS_WINDOWS}:advapi32>"
	"$<${IS_WINDOWS}:wsock32>"
	"$<${IS_WINDOWS}:ws2_32>"
	"$<${IS_WINDOWS}:user32>"

	"$<${IS_X360}:Xonline>"

	tier2
	tier3
	vgui_controls
	mathlib
)

target_use_steamapi(
	serverbrowser
)
