#=============================================================================
#	launcher.cmake
#
#	Project Script
#=============================================================================

set( LAUNCHER_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	LAUNCHER_SOURCE_FILES

	# Source Files
	"${SRCDIR}/public/filesystem_init.cpp"
	"${LAUNCHER_DIR}/launcher.cpp"
	"${LAUNCHER_DIR}/reslistgenerator.cpp"
	"$<${IS_ANDROID}:${LAUNCHER_DIR}/android/main.cpp>"
	#"$<${IS_ANDROID}:${LAUNCHER_DIR}/android/crashhandler.cpp>" # SanyaSho: disable stolen crashhandler
	"$<${IS_ANDROID}:${LAUNCHER_DIR}/android/crashhandler_stub.cpp>"

	# Header Files
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/tier0/commonmacros.h"
	"${SRCDIR}/public/tier0/dbg.h"
	"${SRCDIR}/common/engine_launcher_api.h"
	"${SRCDIR}/public/tier0/fasttimer.h"
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/tier0/icommandline.h"
	"${LAUNCHER_DIR}/ifilesystem.h"
	"${SRCDIR}/public/vgui/IHTML.h"
	"${SRCDIR}/public/vgui/IImage.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/vgui/ISurface.h"
	"${SRCDIR}/public/vgui/KeyCode.h"
	"${SRCDIR}/public/tier0/mem.h"
	"${SRCDIR}/public/tier0/memalloc.h"
	"${SRCDIR}/public/vgui/MouseCode.h"
	"${SRCDIR}/public/tier0/platform.h"
	"${SRCDIR}/public/tier0/protected_things.h"
	"${LAUNCHER_DIR}/reslistgenerator.h"
	"${SRCDIR}/public/string_t.h"
	"${SRCDIR}/public/tier1/strtools.h"
	"${SRCDIR}/public/mathlib/vector2d.h"
	"${SRCDIR}/public/vgui/VGUI.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"
)

add_library(
	launcher SHARED
	${LAUNCHER_SOURCE_FILES}
)

set_property(
	TARGET launcher PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	launcher
	""
	"launcher"
	"${GAMEDIR}/bin"
)

target_include_directories(
	launcher PRIVATE

	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
	"$<${IS_ANDROID}:${LAUNCHER_DIR}/android/libunwind>"
)

target_compile_definitions(
	launcher PRIVATE

	LAUNCHERONLY
)

target_link_libraries(
	launcher PRIVATE

	"$<${IS_WINDOWS}:shlwapi>"
	"$<${IS_WINDOWS}:winmm>"
	"$<${IS_WINDOWS}:wsock32>"
	"$<${IS_WINDOWS}:odbc32>"
	"$<${IS_WINDOWS}:odbccp32>"
	"$<${IS_WINDOWS}:dinput8>"
	#"$<${IS_WINDOWS}:ddraw>"

	"$<${IS_OSX}:-framework Carbon>"
	"$<${IS_OSX}:-framework AppKit>"
	"$<${IS_OSX}:-framework OpenGL>"
	"$<${IS_OSX}:-framework IOKit>"

	"$<$<AND:${IS_X360},$<CONFIG:Debug>>:Xonlined>"
	"$<$<AND:${IS_X360},$<CONFIG:Release>>:Xonline>"

	appframework
	tier2
	tier3
	"$<$<AND:${USE_GL},$<NOT:${IS_TOGLES}>>:togl>"
	"$<$<BOOL:${ENABLE_TOGLES}>:togles>"
	"$<${IS_ANDROID}:unwind>"
)

target_use_steamapi(
	launcher
)

if( ${USE_SDL} )
	target_use_sdl2(
		launcher
	)
endif()
