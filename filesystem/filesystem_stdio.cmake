#=============================================================================
#	filesystem_stdio.cmake
#
#	Project Script
#=============================================================================

set( FILESYSTEM_STDIO_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	FILESYSTEM_STDIO_SOURCE_FILES

	# Source Files
	"${FILESYSTEM_STDIO_DIR}/basefilesystem.cpp"
	"${FILESYSTEM_STDIO_DIR}/packfile.cpp"
	"${FILESYSTEM_STDIO_DIR}/filetracker.cpp"
	"${FILESYSTEM_STDIO_DIR}/filesystem_async.cpp"
	"${FILESYSTEM_STDIO_DIR}/filesystem_stdio.cpp"
	"${SRCDIR}/public/kevvaluescompiler.cpp"
	"${SRCDIR}/public/zip_utils.cpp"
	"${FILESYSTEM_STDIO_DIR}/QueuedLoader.cpp"
	"$<${IS_POSIX}:${FILESYSTEM_STDIO_DIR}/linux_support.cpp>"

	# Header Files
	"${FILESYSTEM_STDIO_DIR}/basefilesystem.h"
	"${FILESYSTEM_STDIO_DIR}/packfile.h"
	"${FILESYSTEM_STDIO_DIR}/filetracker.h"
	"${FILESYSTEM_STDIO_DIR}/threadsaferefcountedobject.h"
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/bspfile.h"
	"${SRCDIR}/public/bspflags.h"
	"${SRCDIR}/public/mathlib/bumpvects.h"
	"${SRCDIR}/public/tier1/characterset.h"
	"${SRCDIR}/public/tier0/dbg.h"
	"${SRCDIR}/public/tier0/fasttimer.h"
	"${SRCDIR}/public/filesystem.h"
	"${SRCDIR}/public/ifilelist.h"
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/mathlib/mathlib.h"
	"${SRCDIR}/public/tier0/platform.h"
	"${SRCDIR}/public/tier1/strtools.h"
	"${SRCDIR}/public/tier1/utlmemory.h"
	"${SRCDIR}/public/tier1/utlrbtree.h"
	"${SRCDIR}/public/tier1/utlsymbol.h"
	"${SRCDIR}/public/tier1/utlvector.h"
	"${SRCDIR}/public/mathlib/vector.h"
	"${SRCDIR}/public/mathlib/vector2d.h"
	"${SRCDIR}/public/mathlib/vector4d.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"
	"${SRCDIR}/public/keyvaluescompiler.h"
	"${SRCDIR}/public/filesystem/IQueuedLoader.h"
)

add_library(
	filesystem_stdio SHARED
	${FILESYSTEM_STDIO_SOURCE_FILES}
)

set_property(
	TARGET filesystem_stdio PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	filesystem_stdio
	""
	"filesystem_stdio"
	"${GAMEDIR}/bin"
)

target_include_directories(
	filesystem_stdio PRIVATE

	"${SRCDIR}/tier0"
	"$<${USE_SDL}:${THIRDPARTYDIR}/SDL>"
)

target_compile_definitions(
	filesystem_stdio PRIVATE

	FILESYSTEM_STDIO_EXPORTS
	DONT_PROTECT_FILEIO_FUNCTIONS
	#PROTECTED_THINGS_ENABLE # TODO(SanyaSho): causes link errors on WIN64
	SUPPORT_PACKED_STORE # CMAKETODO(SanyaSho): [!$WIN64]
)

if( ${IS_WINDOWS} )
target_compile_options(
	filesystem_stdio PRIVATE

	/EHsc
)
endif()

target_link_libraries(
	filesystem_stdio PRIVATE

	tier2
)

target_include_crypto(
	filesystem_stdio
)
