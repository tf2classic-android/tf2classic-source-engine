#=============================================================================
#	tier3.cmake
#
#	Project Script
#=============================================================================

set( TIER3_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	TIER3_SOURCE_FILES

	# Source Files
	"${TIER3_DIR}/tier3.cpp"
	"${TIER3_DIR}/mdlutils.cpp"
	"${TIER3_DIR}/choreoutils.cpp"
	"${TIER3_DIR}/scenetokenprocessor.cpp"
	"${TIER3_DIR}/studiohdrstub.cpp"

	# Header Files
	"${SRCDIR}/public/tier3/tier3.h"
	"${SRCDIR}/public/tier3/mdlutils.h"
	"${SRCDIR}/public/tier3/choreoutils.h"
	"${SRCDIR}/public/tier3/scenetokenprocessor.h"
)

add_library(
	tier3 STATIC
	${TIER3_SOURCE_FILES}
)

set_property(
	TARGET tier3 PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	tier3 PRIVATE

	"${SRCDIR}/public/tier3"
)
