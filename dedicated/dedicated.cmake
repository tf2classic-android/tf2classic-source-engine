#=============================================================================
#	dedicated.cmake
#
#	Project Script
#=============================================================================

set( DEDICATED_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	DEDICATED_SOURCE_FILES

	# Source Files
	#"$<${IS_WINDOWS}:${DEDICATED_DIR}/dedicated.rc>"

	"${DEDICATED_DIR}/filesystem.cpp"
	"${SRCDIR}/public/filesystem_init.cpp"
	"${SRCDIR}/common/netapi.cpp"
	"${SRCDIR}/common/SteamAppStartup.cpp"
	"${DEDICATED_DIR}/sys_common.cpp"
	"${DEDICATED_DIR}/sys_ded.cpp"
	"$<${IS_WINDOWS}:${DEDICATED_DIR}/sys_windows.cpp>"

	"$<${IS_POSIX}:${DEDICATED_DIR}/sys_linux.cpp>"

	# Console
	"${DEDICATED_DIR}/console/conproc.cpp"
	"${DEDICATED_DIR}/console/textconsole.cpp"
	"$<${IS_POSIX}:${DEDICATED_DIR}/console/TextConsoleUnix.cpp>"
	"$<${IS_WINDOWS}:${DEDICATED_DIR}/console/TextConsoleWin32.cpp>"

	# VGUI
	#"$<${IS_WINDOWS}:${DEDICATED_DIR}/vgui/CreateMultiplayerGameServerPage.cpp>"
	#"$<${IS_WINDOWS}:${DEDICATED_DIR}/vgui/MainPanel.cpp>"
	"$<${IS_WINDOWS}:${SRCDIR}/public/vgui_controls/vgui_controls.cpp>"
	"$<${IS_WINDOWS}:${DEDICATED_DIR}/vgui/vguihelpers.cpp>"

	# FileSystem
	"${SRCDIR}/filesystem/filetracker.cpp"
	"${SRCDIR}/filesystem/basefilesystem.cpp"
	"${SRCDIR}/filesystem/packfile.cpp"
	"${SRCDIR}/filesystem/filesystem_async.cpp"
	"${SRCDIR}/filesystem/filesystem_stdio.cpp"
	"${SRCDIR}/filesystem/QueuedLoader.cpp"
	"${SRCDIR}/public/zip_utils.cpp"
	"$<${IS_POSIX}:${SRCDIR}/filesystem/linux_support.cpp>"

	# Header Files
	"${SRCDIR}/filesystem/filetracker.h"
	"${SRCDIR}/filesystem/threadsaferefcountedobject.h"
	"${SRCDIR}/public/ifilelist.h"
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/tier0/commonmacros.h"
	"${SRCDIR}/public/tier0/dbg.h"
	"${DEDICATED_DIR}/dedicated.h"
	"${SRCDIR}/public/engine_hlds_api.h"
	"${SRCDIR}/public/tier0/fasttimer.h"
	"${SRCDIR}/public/filesystem.h"
	"${SRCDIR}/common/IAdminServer.h"
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/tier0/icommandline.h"
	"${SRCDIR}/public/idedicatedexports.h"
	"${SRCDIR}/common/IManageServer.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${DEDICATED_DIR}/isys.h"
	"${SRCDIR}/public/mathlib/mathlib.h"
	"${SRCDIR}/common/netapi.h"
	"${SRCDIR}/common/GameUI/ObjectList.h"
	"${SRCDIR}/public/tier0/platform.h"
	"${SRCDIR}/public/tier0/protected_things.h"
	"${SRCDIR}/common/SteamAppStartup.h"
	"${SRCDIR}/public/string_t.h"
	"${SRCDIR}/public/tier1/strtools.h"
	"${SRCDIR}/public/mathlib/vector.h"
	"${SRCDIR}/public/mathlib/vector2d.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"

	# Console Headers
	"${DEDICATED_DIR}/console/conproc.h"
	"${SRCDIR}/common/IObjectContainer.h"
	"${DEDICATED_DIR}/console/textconsole.h"
	"${DEDICATED_DIR}/console/TextConsoleWin32.h"

	# VGUI Headers
	"${DEDICATED_DIR}/vgui/CreateMultiplayerGameServerPage.h"
	"${DEDICATED_DIR}/vgui/MainPanel.h"
	"${DEDICATED_DIR}/vgui/vguihelpers.h"
)

set_source_files_properties(
        "$<${IS_POSIX}:${DEDICATED_DIR}/sys_linux.cpp>"
        PROPERTIES
        COMPILE_FLAGS
        /EHsc
)

add_library(
	dedicated SHARED
	${DEDICATED_SOURCE_FILES}
)

set_property(
	TARGET dedicated PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	dedicated
	""
	"dedicated"
	"${GAMEDIR}/bin"
)

target_include_directories(
	dedicated PRIVATE

	"${SRCDIR}/public"
	"${SRCDIR}/common"
	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
	"${DEDICATED_DIR}"
	"$<${IS_LINUX}:${THIRDPARTYDIR}/libedit-20221030-3.1/src>"
)

target_compile_definitions(
	dedicated PRIVATE

	DEDICATED
	LAUNCHERONLY
	SUPPORT_PACKED_STORE # CMAKETODO(SanyaSho): [!$WIN64]
)

if( ${IS_WINDOWS} )
target_compile_options(
	dedicated PRIVATE

	/EHsc
)
endif()

target_link_libraries(
	dedicated PRIVATE

	"$<${IS_WINDOWS}:wsock32>"
	"$<${IS_WINDOWS}:odbc32>"
	"$<${IS_WINDOWS}:odbccp32>"
	"$<${IS_WINDOWS}:winmm>"

	"$<${IS_OSX}:-framework AppKit>"
	"$<${IS_OSX}:edit>"
	"$<${IS_OSX}:curses>"

	appframework
	mathlib
	tier2
	tier3
	#"$<${IS_WINDOWS}:vgui_controls>"
	"$<${IS_LINUX}:edit>"
)

target_include_crypto(
	dedicated
)

target_use_steamapi(
	dedicated
)

if( ${USE_SDL} )
	target_use_sdl2(
		dedicated
	)
endif()
