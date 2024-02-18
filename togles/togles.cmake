#=============================================================================
#	togles.cmake
#
#	Project Script
#=============================================================================

set( TOGLES_SRCDIR "${CMAKE_CURRENT_LIST_DIR}/linuxwin/" )
set( TOGLES_INCDIR "${SRCDIR}/public/togl/linuxwin/" )

set(
	TOGLES_SOURCE_FILES

	# Source Files
	"${TOGLES_SRCDIR}/dx9asmtogl2.cpp"
	"${TOGLES_SRCDIR}/dxabstract.cpp"
	"${TOGLES_SRCDIR}/glentrypoints.cpp"
	"${TOGLES_SRCDIR}/glmgr.cpp"
	"${TOGLES_SRCDIR}/glmgrbasics.cpp"
	"$<${IS_OSX}:${TOGLES_SRCDIR}/glmgrcocoa.mm>"
	"${TOGLES_SRCDIR}/cglmtex.cpp"
	"${TOGLES_SRCDIR}/cglmfbo.cpp"
	"${TOGLES_SRCDIR}/cglmprogram.cpp"
	"${TOGLES_SRCDIR}/cglmbuffer.cpp"
	"${TOGLES_SRCDIR}/cglmquery.cpp"
	"${TOGLES_SRCDIR}/asanstubs.cpp"
	"${TOGLES_SRCDIR}/decompress.c"

	# Header Files
	"${TOGLES_SRCDIR}/dx9asmtogl2.h"
	"${TOGLES_SRCDIR}/glmgr_flush.inl"
	"$<${IS_OSX}:${TOGLES_SRCDIR}/intelglmallocworkaround.h>"
	"$<${IS_OSX}:${TOGLES_SRCDIR}/mach_override.h>"

	# Public Header Files
	"${SRCDIR}/public/togl/rendermechanism.h"
	"${TOGLES_INCDIR}/dxabstract.h"
	"${TOGLES_INCDIR}/dxabstract_types.h"
	"${TOGLES_INCDIR}/glbase.h"
	"${TOGLES_INCDIR}/glentrypoints.h"
	"${TOGLES_INCDIR}/glmgr.h"
	"${TOGLES_INCDIR}/glmdebug.h"
	"${TOGLES_INCDIR}/glmgrbasics.h"
	"${TOGLES_INCDIR}/glmgrext.h"
	"${TOGLES_INCDIR}/glmdisplay.h"
	"${TOGLES_INCDIR}/glmdisplaydb.h"
	"${TOGLES_INCDIR}/glfuncs.h"
	"${TOGLES_INCDIR}/cglmtex.h"
	"${TOGLES_INCDIR}/cglmfbo.h"
	"${TOGLES_INCDIR}/cglmprogram.h"
	"${TOGLES_INCDIR}/cglmbuffer.h"
	"${TOGLES_INCDIR}/cglmquery.h"
)

add_library(
	togles SHARED
	${TOGLES_SOURCE_FILES}
)

set_property(
	TARGET togles PROPERTY FOLDER "Shared Libs"
)

if( ${IS_POSIX} )
	set( _LIB_PREFIX "lib" )
else()
	set( _LIB_PREFIX "" )
endif()

set_install_properties(
	togles
	"${_LIB_PREFIX}"
	"togles"
	"${GAMEDIR}/bin"
)

target_include_directories(
	togles PRIVATE

	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
)

target_compile_definitions(
	togles PRIVATE

	"TOGL_DLL_EXPORT"
	"$<$<NOT:${IS_OSX}>:PROTECTED_THINGS_ENABLE>"
	"$<$<NOT:${IS_OSX}>:strncpy=use_Q_strncpy_instead>"
	"$<$<NOT:${IS_OSX}>:_snprintf=use_Q_snprintf_instead>"
)

target_link_libraries(
	togles PRIVATE

	"$<${IS_OSX}:-framework Carbon>"
	"$<${IS_OSX}:-framework OpenGL>"
	"$<${IS_OSX}:-framework Quartz>"
	"$<${IS_OSX}:-framework Cocoa>"
	"$<${IS_OSX}:-framework IOKit>"

	tier2
	mathlib
)
