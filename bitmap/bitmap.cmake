#=============================================================================
#	bitmap.cmake
#
#	Project Script
#=============================================================================

set( BITMAP_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	BITMAP_SOURCE_FILES

	# Source Files
	"${BITMAP_DIR}/colorconversion.cpp"
	"${BITMAP_DIR}/float_bm.cpp"
	"${BITMAP_DIR}/float_bm2.cpp"
	"${BITMAP_DIR}/float_bm3.cpp"
	"$<${IS_WINDOWS}:${BITMAP_DIR}/float_bm4.cpp>"
	"${BITMAP_DIR}/float_bm_bilateral_filter.cpp"
	"${BITMAP_DIR}/float_cube.cpp"
	"${BITMAP_DIR}/imageformat.cpp"
	"${BITMAP_DIR}/psd.cpp"
	"${BITMAP_DIR}/resample.cpp"
	"${BITMAP_DIR}/tgaloader.cpp"
	"${BITMAP_DIR}/tgawriter.cpp"
	"${BITMAP_DIR}/bitmap.cpp"

	# Header Files
	"${SRCDIR}/public/bitmap/bitmap.h"
	"${SRCDIR}/public/bitmap/float_bm.h"
	"${SRCDIR}/public/bitmap/imageformat.h"
	"${SRCDIR}/public/bitmap/psd.h"
	"${SRCDIR}/public/bitmap/tgaloader.h"
	"${SRCDIR}/public/bitmap/tgawriter.h"
	"${THIRDPARTYDIR}/stb/stb_dxt.h"
)

add_library(
	bitmap STATIC
	${BITMAP_SOURCE_FILES}
)

set_property(
	TARGET bitmap PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	bitmap PRIVATE

	"$<${IS_WINDOWS}:${DX9SDKDIR}/Include>"
	"${THIRDPARTYDIR}/stb"
)
