#=============================================================================
#	vaudio_minimp3.cmake
#
#	Project Script
#=============================================================================

set( ENGINE_SRC_DIR "${SRCDIR}/engine" )
set( VAUDIO_MINIMP3_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VAUDIO_MINIMP3_SOURCE_FILES

	# Source Files
	"${VAUDIO_MINIMP3_DIR}/mp3codecs.cpp"

	# Header Files
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/vaudio/ivaudio.h"
	#"${VAUDIO_MINIMP3_DIR}/minimp3.h"
)

add_library(
	vaudio_minimp3 SHARED
	${VAUDIO_MINIMP3_SOURCE_FILES}
)

set_property(
	TARGET vaudio_minimp3 PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	vaudio_minimp3
	""
	"vaudio_minimp3"
	"${GAMEDIR}/bin"
)

target_include_directories(
	vaudio_minimp3 PRIVATE

	"${ENGINE_SRC_DIR}"
	"${THIRDPARTYDIR}/minimp3/"
)
