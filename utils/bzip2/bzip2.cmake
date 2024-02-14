#=============================================================================
#	bzip2.cmake
#
#	Project Script
#=============================================================================

set( BZIP2_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	BZIP2_SOURCE_FILES

	"${BZIP2_DIR}/blocksort.c"
	"${BZIP2_DIR}/bzip2.c"
	"${BZIP2_DIR}/bzlib.c"
	"${BZIP2_DIR}/compress.c"
	"${BZIP2_DIR}/crctable.c"
	"${BZIP2_DIR}/decompress.c"
	"${BZIP2_DIR}/huffman.c"
	"${BZIP2_DIR}/randtable.c"
)

add_library(
	bz2 STATIC
	${BZIP2_SOURCE_FILES}
)

set_property(
	TARGET bz2 PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	bz2 PRIVATE

	"${BZIP2_DIR}"
)
