#=============================================================================
#	scenefilecache.cmake
#
#	Project Script
#=============================================================================

set( SCENEFILECACHE_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	SCENEFILECACHE_SOURCE_FILES

	# Source Files
	"${SCENEFILECACHE_DIR}/SceneFileCache.cpp"

	# Header Files
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/scenefilecache/ISceneFileCache.h"
	"${SRCDIR}/public/tier1/utlbuffer.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"
)

add_library(
	scenefilecache SHARED
	${SCENEFILECACHE_SOURCE_FILES}
)

set_property(
	TARGET scenefilecache PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	scenefilecache
	""
	"scenefilecache"
	"${GAMEDIR}/bin"
)

target_include_directories(
	scenefilecache PRIVATE

	"${SRCDIR}/game/shared"
)

target_compile_definitions(
	scenefilecache PRIVATE

	_WINDOWS # SanyaSho: ???????
	PROTECTED_THINGS_ENABLE
)
