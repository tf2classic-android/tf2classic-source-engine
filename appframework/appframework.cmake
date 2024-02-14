#=============================================================================
#	appframework.cmake
#
#	Project Script
#=============================================================================

set( APPFRAMEWORK_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	APPFRAMEWORK_SOURCE_FILES

	#$Folder	"Source Files"
	#{
		"${APPFRAMEWORK_DIR}/AppSystemGroup.cpp"
		"${SRCDIR}/public/filesystem_init.cpp"
		"$<${IS_WINDOWS}:${APPFRAMEWORK_DIR}/vguimatsysapp.cpp>"		#[$WIN32]
		"$<${IS_WINDOWS}:${APPFRAMEWORK_DIR}/winapp.cpp>"				#[$WIN32]
		"$<${IS_POSIX}:${APPFRAMEWORK_DIR}/posixapp.cpp>"				#[$POSIX]
		"$<${USE_SDL}:${APPFRAMEWORK_DIR}/sdlmgr.cpp>"					#[$SDL]
		"$<$<AND:${IS_OSX},$<NOT:${IS_DEDICATED}>>:${APPFRAMEWORK_DIR}/glmrendererinfo_osx.mm>"		#[$OSXALL]
	#}

	#$Folder	"Interface"
	#{
		"${SRCDIR}/public/appframework/AppFramework.h"
		"${SRCDIR}/public/appframework/IAppSystem.h"
		"${SRCDIR}/public/appframework/IAppSystemGroup.h"
		"${SRCDIR}/public/appframework/tier2app.h"
		"${SRCDIR}/public/appframework/tier3app.h"
		"${SRCDIR}/public/appframework/VguiMatSysApp.h"
		"${SRCDIR}/public/appframework/ilaunchermgr.h"
	#}
)

add_library(
	appframework STATIC
	${APPFRAMEWORK_SOURCE_FILES}
)

set_property(
	TARGET appframework PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	appframework PRIVATE

	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL2>"
)

target_link_libraries(
	appframework PRIVATE

	"$<${IS_OSX}:-framework Carbon>"
	"$<${IS_OSX}:-framework OpenGL>"
	"$<${IS_OSX}:-framework Quartz>"
	"$<${IS_OSX}:-framework Cocoa>"
	"$<${IS_OSX}:-framework IOKit>"
)
