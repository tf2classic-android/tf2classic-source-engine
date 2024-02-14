#=============================================================================
#	dmxloader.cmake
#
#	Project Script
#=============================================================================

set( DMXLOADER_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	DMXLOADER_SOURCE_FILES

	# Source Files
	"${DMXLOADER_DIR}/dmxattribute.cpp"
	"${DMXLOADER_DIR}/dmxelement.cpp"
	"${DMXLOADER_DIR}/dmxloader.cpp"
	"${DMXLOADER_DIR}/dmxloadertext.cpp"
	"${DMXLOADER_DIR}/dmxserializationdictionary.cpp"

	# Header Files
	"${DMXLOADER_DIR}/dmxserializationdictionary.h"

	# Public Header Files
	"${SRCDIR}/public/dmxloader/dmxattribute.h"
	"${SRCDIR}/public/dmxloader/dmxelement.h"
	"${SRCDIR}/public/dmxloader/dmxloader.h"
)

add_library(
	dmxloader STATIC
	${DMXLOADER_SOURCE_FILES}
)

set_property(
	TARGET dmxloader PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	dmxloader PRIVATE

	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL2>"
)

target_compile_definitions(
	dmxloader PRIVATE

	DMXLOADER_LIB
)
