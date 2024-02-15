#=============================================================================
#	tier2.cmake
#
#	Project Script
#=============================================================================

set( TIER2_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	TIER2_SOURCE_FILES

	# Source Files
	"${TIER2_DIR}/beamsegdraw.cpp"
	"${TIER2_DIR}/defaultfilesystem.cpp"
	"${TIER2_DIR}/dmconnect.cpp"
	"${TIER2_DIR}/fileutils.cpp"
	"${TIER2_DIR}/keybindings.cpp"
	"${SRCDIR}/public/map_utils.cpp"
	"${SRCDIR}/public/materialsystem/MaterialSystemUtil.cpp"
	"${TIER2_DIR}/camerautils.cpp"
	"${TIER2_DIR}/meshutils.cpp"
	"${TIER2_DIR}/renderutils.cpp"
	"${TIER2_DIR}/riff.cpp"
	"${TIER2_DIR}/soundutils.cpp"
	"${TIER2_DIR}/tier2.cpp"
	"${TIER2_DIR}/util_init.cpp"
	"${TIER2_DIR}/utlstreambuffer.cpp"
	"${TIER2_DIR}/vconfig.cpp"
	"${TIER2_DIR}/keyvaluesmacros.cpp"
	"${TIER2_DIR}/p4helpers.cpp"

	# Header Files
	"${SRCDIR}/public/tier2/beamsegdraw.h"
	"${SRCDIR}/public/tier2/fileutils.h"
	"${SRCDIR}/public/tier2/camerautils.h"
	"${SRCDIR}/public/tier2/meshutils.h"
	"${SRCDIR}/public/tier2/keybindings.h"
	"${SRCDIR}/public/tier2/renderutils.h"
	"${SRCDIR}/public/tier2/riff.h"
	"${SRCDIR}/public/tier2/soundutils.h"
	"${SRCDIR}/public/tier2/tier2.h"
	"${SRCDIR}/public/tier2/utlstreambuffer.h"
	"${SRCDIR}/public/tier2/vconfig.h"
	"${SRCDIR}/public/tier2/keyvaluesmacros.h"
)

add_library(
	tier2 STATIC
	${TIER2_SOURCE_FILES}
)

set_property(
	TARGET tier2 PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	tier2 PRIVATE

	"${SRCDIR}/public/tier2"
	"$<${IS_WINDOWS}:${DX9SDKDIR}/Include>"
	"$<${USE_SDL}:${THIRDPARTYDIR}/SDL>"
)
