#=============================================================================
#	togl.cmake
#
#	Project Script
#=============================================================================

set( TOGL_SRCDIR "${CMAKE_CURRENT_LIST_DIR}/linuxwin/" )
set( TOGL_INCDIR "${SRCDIR}/public/togl/linuxwin/" )

set(
	TOGL_SOURCE_FILES

	# Source Files
	"${TOGL_SRCDIR}/dx9asmtogl2.cpp"
	"${TOGL_SRCDIR}/dxabstract.cpp"
	"${TOGL_SRCDIR}/glentrypoints.cpp"
	"${TOGL_SRCDIR}/glmgr.cpp"
	"${TOGL_SRCDIR}/glmgrbasics.cpp"
	"$<${IS_OSX}:${TOGL_SRCDIR}/glmgrcocoa.mm>"
	"$<${IS_OSX}:${TOGL_SRCDIR}/intelglmallocworkaround.cpp>"
	"$<${IS_OSX}:${TOGL_SRCDIR}/mach_override.c>"
	"${TOGL_SRCDIR}/cglmtex.cpp"
	"${TOGL_SRCDIR}/cglmfbo.cpp"
	"${TOGL_SRCDIR}/cglmprogram.cpp"
	"${TOGL_SRCDIR}/cglmbuffer.cpp"
	"${TOGL_SRCDIR}/cglmquery.cpp"
	"${TOGL_SRCDIR}/asanstubs.cpp"

	# Header Files
	"${TOGL_SRCDIR}/dx9asmtogl2.h"
	"${TOGL_SRCDIR}/glmgr_flush.inl"
	"$<${IS_OSX}:${TOGL_SRCDIR}/intelglmallocworkaround.h>"
	"$<${IS_OSX}:${TOGL_SRCDIR}/mach_override.h>"

	# Public Header Files
	"${SRCDIR}/public/togl/rendermechanism.h"
	"${TOGL_INCDIR}/dxabstract.h"
	"${TOGL_INCDIR}/dxabstract_types.h"
	"${TOGL_INCDIR}/glbase.h"
	"${TOGL_INCDIR}/glentrypoints.h"
	"${TOGL_INCDIR}/glmgr.h"
	"${TOGL_INCDIR}/glmdebug.h"
	"${TOGL_INCDIR}/glmgrbasics.h"
	"${TOGL_INCDIR}/glmgrext.h"
	"${TOGL_INCDIR}/glmdisplay.h"
	"${TOGL_INCDIR}/glmdisplaydb.h"
	"${TOGL_INCDIR}/glfuncs.h"
	"${TOGL_INCDIR}/cglmtex.h"
	"${TOGL_INCDIR}/cglmfbo.h"
	"${TOGL_INCDIR}/cglmprogram.h"
	"${TOGL_INCDIR}/cglmbuffer.h"
	"${TOGL_INCDIR}/cglmquery.h"
)

add_library(
	togl SHARED
	${TOGL_SOURCE_FILES}
)

set_property(
	TARGET togl PROPERTY FOLDER "Shared Libs"
)

if( ${IS_POSIX} )
	set( _LIB_PREFIX "lib" )
else()
	set( _LIB_PREFIX "" )
endif()

set_install_properties(
	togl
	"${_LIB_PREFIX}"
	"togl"
	"${GAMEDIR}/bin"
)

target_include_directories(
	togl PRIVATE

	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
)

target_compile_definitions(
	togl PRIVATE

	"TOGL_DLL_EXPORT"
	"$<$<NOT:${IS_OSX}>:PROTECTED_THINGS_ENABLE>"
	"$<$<NOT:${IS_OSX}>:strncpy=use_Q_strncpy_instead>"
	"$<$<NOT:${IS_OSX}>:_snprintf=use_Q_snprintf_instead>"
)

target_link_libraries(
	togl PRIVATE

	"$<${IS_OSX}:-framework Carbon>"
	"$<${IS_OSX}:-framework OpenGL>"
	"$<${IS_OSX}:-framework Quartz>"
	"$<${IS_OSX}:-framework Cocoa>"
	"$<${IS_OSX}:-framework IOKit>"

	tier2
	mathlib
)
