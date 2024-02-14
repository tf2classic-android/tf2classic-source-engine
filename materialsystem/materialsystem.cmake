#=============================================================================
#	materialsystem.cmake
#
#	Project Script
#=============================================================================

set( MATERIALSYSTEM_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	MATERIALSYSTEM_SOURCE_FILES

	# Source Files
	"${MATERIALSYSTEM_DIR}/CColorCorrection.cpp"
	"${MATERIALSYSTEM_DIR}/cmaterial.cpp"
	"${MATERIALSYSTEM_DIR}/cmaterial_queuefriendly.cpp"
	"${MATERIALSYSTEM_DIR}/CMaterialSubRect.cpp"
	"${MATERIALSYSTEM_DIR}/cmaterialvar.cpp"
	"${MATERIALSYSTEM_DIR}/cmatnullrendercontext.cpp"
	"${MATERIALSYSTEM_DIR}/colorspace.cpp"
	"${MATERIALSYSTEM_DIR}/ctexture.cpp"
	"${SRCDIR}/public/filesystem_helpers.cpp"
	"${MATERIALSYSTEM_DIR}/imagepacker.cpp"
	"${MATERIALSYSTEM_DIR}/mat_stub.cpp"
	"${MATERIALSYSTEM_DIR}/materialsystem_global.cpp"
	"${MATERIALSYSTEM_DIR}/morph.cpp"
	"${MATERIALSYSTEM_DIR}/occlusionquerymgr.cpp"
	"${MATERIALSYSTEM_DIR}/shadersystem.cpp"
	"${MATERIALSYSTEM_DIR}/texturemanager.cpp"

	#"${MATERIALSYSTEM_DIR}/pch_materialsystem.cpp"
	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Create Precompiled Header (/Yc)"
	#			}
	#		}
	#	}

	"${MATERIALSYSTEM_DIR}/cmaterialdict.cpp"
	"${MATERIALSYSTEM_DIR}/cmaterialsystem.cpp"
	"${MATERIALSYSTEM_DIR}/cmatlightmaps.cpp"
	"${MATERIALSYSTEM_DIR}/cmatrendercontext.cpp"
	"${MATERIALSYSTEM_DIR}/cmatqueuedrendercontext.cpp"
	"${MATERIALSYSTEM_DIR}/ctexturecompositor.cpp"
	#	{
	#			$Configuration
	#			{
	#				$Compiler
	#				{
	#					$Create/UsePrecompiledHeader	"Use Precompiled Header (/Yu)"
	#				}
	#			}
	#	}

	# Header Files
	"${MATERIALSYSTEM_DIR}/cmaterial_queuefriendly.h"
	"${MATERIALSYSTEM_DIR}/cmaterialdict.h"
	"${MATERIALSYSTEM_DIR}/cmaterialsystem.h"
	"${MATERIALSYSTEM_DIR}/cmatlightmaps.h"
	"${MATERIALSYSTEM_DIR}/cmatnullrendercontext.h"
	"${MATERIALSYSTEM_DIR}/cmatqueuedrendercontext.h"
	"${MATERIALSYSTEM_DIR}/cmatrendercontext.h"
	"${MATERIALSYSTEM_DIR}/colorspace.h"
	"${MATERIALSYSTEM_DIR}/ctexturecompositor.h"
	"${MATERIALSYSTEM_DIR}/IHardwareConfigInternal.h"
	"${MATERIALSYSTEM_DIR}/imagepacker.h"
	"${MATERIALSYSTEM_DIR}/imaterialinternal.h"
	"${MATERIALSYSTEM_DIR}/imaterialsysteminternal.h"
	"${MATERIALSYSTEM_DIR}/imatrendercontextinternal.h"
	"${MATERIALSYSTEM_DIR}/imorphinternal.h"
	"${MATERIALSYSTEM_DIR}/IShaderSystem.h"
	"${SRCDIR}/public/shaderapi/ishaderutil.h"
	"${MATERIALSYSTEM_DIR}/itextureinternal.h"
	"${MATERIALSYSTEM_DIR}/materialsystem_global.h"
	"${MATERIALSYSTEM_DIR}/occlusionquerymgr.h"
	"${MATERIALSYSTEM_DIR}/shadersystem.h"
	"${MATERIALSYSTEM_DIR}/texturemanager.h"

	# Public Header Files
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/mathlib/bumpvects.h"
	"${SRCDIR}/public/const.h"
	"${SRCDIR}/public/tier1/convar.h"
	"${SRCDIR}/public/crtmemdebug.h"
	"${SRCDIR}/common/cstringhash.h"
	"${SRCDIR}/public/filesystem.h"
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/materialsystem/combineoperations.h"
	"${SRCDIR}/public/materialsystem/IColorCorrection.h"
	"${SRCDIR}/public/materialsystem/imaterial.h"
	"${SRCDIR}/public/materialsystem/imaterialproxy.h"
	"${SRCDIR}/public/materialsystem/imaterialproxyfactory.h"
	"${SRCDIR}/public/materialsystem/imaterialsystem.h"
	"${SRCDIR}/public/materialsystem/imaterialsystemhardwareconfig.h"
	"${SRCDIR}/public/materialsystem/imaterialsystemstub.h"
	"${SRCDIR}/public/materialsystem/imaterialvar.h"
	"${SRCDIR}/public/materialsystem/imesh.h"
	"${SRCDIR}/public/materialsystem/imorph.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/materialsystem/IShader.h"
	"${SRCDIR}/public/materialsystem/ishaderapi.h"
	"${SRCDIR}/public/materialsystem/itexture.h"
	"${SRCDIR}/public/materialsystem/itexturecompositor.h"
	"${SRCDIR}/public/tier1/KeyValues.h"
	"${SRCDIR}/public/materialsystem/materialsystem_config.h"
	"${SRCDIR}/public/mathlib/mathlib.h"
	"${SRCDIR}/public/tier1/mempool.h"
	"${SRCDIR}/public/pixelwriter.h"
	"${SRCDIR}/public/renderparm.h"
	"${SRCDIR}/public/tier1/strtools.h"
	"${SRCDIR}/public/tier1/utlbuffer.h"
	"${SRCDIR}/public/tier1/utlmemory.h"
	"${SRCDIR}/public/tier1/utlrbtree.h"
	"${SRCDIR}/public/tier1/utlsymbol.h"
	"${SRCDIR}/public/tier1/utlvector.h"
	"${SRCDIR}/public/mathlib/vector.h"
	"${SRCDIR}/public/mathlib/vector2d.h"
	"${SRCDIR}/public/mathlib/vector4d.h"
	"${SRCDIR}/public/mathlib/vmatrix.h"
	"${SRCDIR}/public/mathlib/vplane.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"
	"${SRCDIR}/public/vtf/vtf.h"
)

add_library(
	materialsystem SHARED
	${MATERIALSYSTEM_SOURCE_FILES}
)

set_property(
	TARGET materialsystem PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	materialsystem
	""
	"materialsystem"
	"${GAMEDIR}/bin"
)

target_precompile_headers(
	materialsystem

	PRIVATE "${MATERIALSYSTEM_DIR}/pch_materialsystem.h"
)

target_include_directories(
	materialsystem PRIVATE

	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL2>"
)

target_compile_definitions(
	materialsystem PRIVATE

	DEFINE_MATERIALSYSTEM_INTERFACE
	MATERIALSYSTEM_EXPORTS
	PROTECTED_THINGS_ENABLE
	strncpy=use_Q_strncpy_instead
	_snprintf=use_Q_snprintf_instead
	"$<${IS_WINDOWS}:fopen=dont_use_fopen>"
)

target_link_libraries(
	materialsystem PRIVATE

	vtf
	bitmap
	mathlib
	shaderlib
	tier2
)
