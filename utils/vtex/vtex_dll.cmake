#=============================================================================
#	vtex_dll.cmake
#
#	Project Script
#=============================================================================

set( UTILS_SRC_DIR "${SRCDIR}/utils" )
set( VTEX_DLL_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VTEX_DLL_SOURCE_FILES

	# Source Files
	"${UTILS_SRC_DIR}/common/cmdlib.cpp"
	"${SRCDIR}/public/filesystem_helpers.cpp"
	"${SRCDIR}/public/filesystem_init.cpp"
	"${UTILS_SRC_DIR}/common/filesystem_tools.cpp"
	"${VTEX_DLL_DIR}/vtex.cpp"
	"${SRCDIR}/common/imageutils.cpp"

	# Header Files
	"${SRCDIR}/public/ivtex.h"
)

add_library(
	vtex_dll SHARED
	${VTEX_DLL_SOURCE_FILES}
)

set_property(
	TARGET vtex_dll PROPERTY FOLDER "Utils//VTF Utils//Shared Libs"
)

set_install_properties(
	vtex_dll
	""
	"vtex_dll"
	"${GAMEDIR}/bin"
)

target_include_directories(
	vtex_dll PRIVATE

	"${UTILS_SRC_DIR}/common"
)

target_compile_definitions(
	vtex_dll PRIVATE

	VTEX_DLL
	UTILS
	VTEX_DLL_EXPORTS
	PROTECTED_THINGS_DISABLE
)

target_link_libraries(
	vtex_dll PRIVATE

	bitmap
	mathlib
	tier2
	vtf

	"$<${IS_WINDOWS}:${LIBCOMMON}/libjpeg${STATIC_LIB_EXT}>"
	"$<${IS_WINDOWS}:${LIBCOMMON}/libz${STATIC_LIB_EXT}>"
	"$<${IS_WINDOWS}:${LIBCOMMON}/libpng${STATIC_LIB_EXT}>"
	"$<${IS_POSIX}:jpeg>"
	"$<${IS_POSIX}:png>"
	"$<${IS_POSIX}:z>"
)
