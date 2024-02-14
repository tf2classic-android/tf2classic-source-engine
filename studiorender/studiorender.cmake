#=============================================================================
#	studiorender.cmake
#
#	Project Script
#=============================================================================

# CMAKETODO:
#	$Compiler [$WIN32]
#	{
#		$FloatingPointModel			"Precise (/fp:precise)"
#	}

set( STUDIORENDER_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	STUDIORENDER_SOURCE_FILES

	# Source Files
	"${STUDIORENDER_DIR}/studiorender.cpp"
	"${STUDIORENDER_DIR}/studiorendercontext.cpp"
	"${STUDIORENDER_DIR}/flexrenderdata.cpp"
	"${STUDIORENDER_DIR}/r_studio.cpp"
	"${STUDIORENDER_DIR}/r_studiodecal.cpp"
	"${STUDIORENDER_DIR}/r_studiodraw.cpp"
	"${STUDIORENDER_DIR}/r_studiodraw_computeflexedvertex.cpp"
	"${STUDIORENDER_DIR}/r_studioflex.cpp"
	"${STUDIORENDER_DIR}/r_studiogettriangles.cpp"
	"${STUDIORENDER_DIR}/r_studiolight.cpp"
	"${STUDIORENDER_DIR}/r_studiostats.cpp"

	# Header Files
	"${STUDIORENDER_DIR}/r_studiolight.h"
	"${STUDIORENDER_DIR}/studiorender.h"
	"${STUDIORENDER_DIR}/studiorendercontext.h"
	"${STUDIORENDER_DIR}/flexrenderdata.h"

	# Public Header Files
	"${SRCDIR}/public/basehandle.h"
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/bspflags.h"
	"${SRCDIR}/public/clientstats.h"
	"${SRCDIR}/public/cmodel.h"
	"${SRCDIR}/public/tier0/commonmacros.h"
	"${SRCDIR}/public/mathlib/compressed_vector.h"
	"${SRCDIR}/public/const.h"
	"${SRCDIR}/public/tier1/convar.h"
	"${SRCDIR}/public/tier0/dbg.h"
	"${SRCDIR}/public/tier0/fasttimer.h"
	"${SRCDIR}/public/gametrace.h"
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/tier0/icommandline.h"
	"${SRCDIR}/public/ihandleentity.h"
	"${SRCDIR}/public/materialsystem/imaterial.h"
	"${SRCDIR}/public/materialsystem/imaterialsystem.h"
	"${SRCDIR}/public/materialsystem/imaterialsystemhardwareconfig.h"
	"${SRCDIR}/public/materialsystem/imaterialvar.h"
	"${SRCDIR}/public/materialsystem/imesh.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/istudiorender.h"
	"${SRCDIR}/public/materialsystem/itexture.h"
	"${SRCDIR}/public/mathlib/mathlib.h"
	"${SRCDIR}/public/measure_section.h"
	"${SRCDIR}/public/tier0/mem.h"
	"${SRCDIR}/public/tier0/memalloc.h"
	"${SRCDIR}/public/tier0/memdbgoff.h"
	"${SRCDIR}/public/tier0/memdbgon.h"
	"${SRCDIR}/public/model_types.h"
	"${SRCDIR}/public/optimize.h"
	"${SRCDIR}/public/pixelwriter.h"
	"${SRCDIR}/public/tier0/platform.h"
	"${SRCDIR}/public/tier0/protected_things.h"
	"${SRCDIR}/public/string_t.h"
	"${SRCDIR}/public/tier1/strtools.h"
	"${SRCDIR}/public/studio.h"
	"${SRCDIR}/public/tier1/utlbuffer.h"
	"${SRCDIR}/public/tier1/utllinkedlist.h"
	"${SRCDIR}/public/tier1/utlmemory.h"
	"${SRCDIR}/public/tier1/utlvector.h"
	"${SRCDIR}/public/vcollide.h"
	"${SRCDIR}/public/mathlib/vector.h"
	"${SRCDIR}/public/mathlib/vector2d.h"
	"${SRCDIR}/public/mathlib/vector4d.h"
	"${SRCDIR}/public/mathlib/vmatrix.h"
	"${SRCDIR}/public/mathlib/vplane.h"
	"${SRCDIR}/public/tier0/vprof.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"
	"${SRCDIR}/public/vtf/vtf.h"
	"${SRCDIR}/public/tier1/UtlStringMap.h"
)

add_library(
	studiorender SHARED
	${STUDIORENDER_SOURCE_FILES}
)

set_property(
	TARGET studiorender PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	studiorender
	""
	"studiorender"
	"${GAMEDIR}/bin"
)

target_compile_definitions(
	studiorender PRIVATE

	STUDIORENDER_EXPORTS
	PROTECTED_THINGS_ENABLE
)

target_link_libraries(
	studiorender PRIVATE

	bitmap
	mathlib

	tier2
	tier3
)
