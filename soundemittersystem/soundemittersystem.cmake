#=============================================================================
#	soundemittersystem.cmake
#
#	Project Script
#=============================================================================

set( SOUNDEMITTERSYSTEM_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	SOUNDEMITTERSYSTEM_SOURCE_FILES

	# Source Files
	"${SRCDIR}/game/shared/interval.cpp"
	"${SOUNDEMITTERSYSTEM_DIR}/soundemittersystembase.cpp"
	"${SRCDIR}/public/SoundParametersInternal.cpp"

	# Header Files
	"${SOUNDEMITTERSYSTEM_DIR}/soundemittersystembase.h"
	"${SOUNDEMITTERSYSTEM_DIR}/cbase.h"
	"${SRCDIR}/game/shared/interval.h"

	# Public Header Files
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/Color.h"
	"${SRCDIR}/public/tier0/commonmacros.h"
	"${SRCDIR}/public/tier0/dbg.h"
	"${SRCDIR}/public/tier0/fasttimer.h"
	"${SRCDIR}/public/filesystem.h"
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/tier0/icommandline.h"
	"${SRCDIR}/public/engine/IEngineSound.h"
	"${SRCDIR}/public/vstdlib/IKeyValuesSystem.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/irecipientfilter.h"
	"${SRCDIR}/public/SoundEmitterSystem/isoundemittersystembase.h"
	"${SRCDIR}/public/tier1/KeyValues.h"
	"${SRCDIR}/public/tier0/mem.h"
	"${SRCDIR}/public/tier0/memdbgoff.h"
	"${SRCDIR}/public/tier0/memdbgon.h"
	"${SRCDIR}/public/tier0/platform.h"
	"${SRCDIR}/public/tier0/protected_things.h"
	"${SRCDIR}/public/vstdlib/random.h"
	"${SRCDIR}/public/soundchars.h"
	"${SRCDIR}/public/soundflags.h"
	"${SRCDIR}/public/string_t.h"
	"${SRCDIR}/public/tier1/strtools.h"
	"${SRCDIR}/public/tier1/utlbuffer.h"
	"${SRCDIR}/public/tier1/utldict.h"
	"${SRCDIR}/public/tier1/utlmemory.h"
	"${SRCDIR}/public/tier1/utlrbtree.h"
	"${SRCDIR}/public/tier1/utlsymbol.h"
	"${SRCDIR}/public/tier1/utlvector.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"
)

add_library(
	soundemittersystem SHARED
	${SOUNDEMITTERSYSTEM_SOURCE_FILES}
)

set_property(
	TARGET soundemittersystem PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	soundemittersystem
	""
	"soundemittersystem"
	"${GAMEDIR}/bin"
)

target_include_directories(
	soundemittersystem PRIVATE

	"${SRCDIR}/soundemittersystem"
	"${SRCDIR}/game/shared"
)

target_compile_definitions(
	soundemittersystem PRIVATE

	SOUNDEMITTERSYSTEM_EXPORTS
	_WINDOWS # SanyaSho: ???????????????????????????????
	PROTECTED_THINGS_ENABLE
	"$<${IS_WINDOWS}:fopen=dont_use_fopen>"
)
