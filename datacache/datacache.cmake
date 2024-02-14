#=============================================================================
#	datacache.cmake
#
#	Project Script
#=============================================================================

set( DATACACHE_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	DATACACHE_SOURCE_FILES

	# Source Files
	"${DATACACHE_DIR}/datacache.cpp"
	"${DATACACHE_DIR}/mdlcache.cpp"
	"${SRCDIR}/public/studio.cpp"
	"${SRCDIR}/public/studio_virtualmodel.cpp"
	"${SRCDIR}/common/studiobyteswap.cpp"

	# Header Files
	"${DATACACHE_DIR}/datacache.h"
	"${DATACACHE_DIR}/datacache_common.h"
	"${SRCDIR}/public/studio.h"
	"${SRCDIR}/common/studiobyteswap.h"

	# Interface
	"${SRCDIR}/public/datacache/idatacache.h"
	"${SRCDIR}/public/datacache/imdlcache.h"
)

add_library(
	datacache SHARED
	${DATACACHE_SOURCE_FILES}
)

set_property(
	TARGET datacache PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	datacache
	""
	"datacache"
	"${GAMEDIR}/bin"
)

target_compile_definitions(
	datacache PRIVATE

	MDLCACHE_DLL_EXPORT
)

target_link_libraries(
	datacache PRIVATE

	tier2
	tier3
)
