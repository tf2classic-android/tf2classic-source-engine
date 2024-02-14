#=============================================================================
#	vtf.cmake
#
#	Project Script
#=============================================================================

set( VTF_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VTF_SOURCE_FILES

	# Source Files
	"$<${IS_WINDOWS}:${VTF_DIR}/s3tc_decode.cpp>"
	"${VTF_DIR}/vtf.cpp"

	# Header Files
	"$<${IS_WINDOWS}:${VTF_DIR}/s3tc_decode.h>"
	"${SRCDIR}/public/vtf/vtf.h"
	"${VTF_DIR}/cvtf.h"
)

add_library(
	vtf STATIC
	${VTF_SOURCE_FILES}
)

set_property(
	TARGET vtf PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	vtf PRIVATE

	"${SRCDIR}/public/vtf"
	"$<${IS_WINDOWS}:${DX9SDKDIR}/Include>"
)
