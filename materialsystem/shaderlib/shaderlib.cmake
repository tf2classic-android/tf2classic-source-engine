#=============================================================================
#	shaderlib.cmake
#
#	Project Script
#=============================================================================

set( SHADERLIB_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	SHADERLIB_SOURCE_FILES

	# Source Files
	"${SHADERLIB_DIR}/BaseShader.cpp"
	"${SHADERLIB_DIR}/ShaderDLL.cpp"
	"${SHADERLIB_DIR}/shaderlib_cvar.cpp"

	# Header Files
	"${SHADERLIB_DIR}/shaderDLL_Global.h"
	"${SHADERLIB_DIR}/shaderlib_cvar.h"
	"${SRCDIR}/materialsystem/IShaderSystem.h"

	# Public Header Files
	"${SRCDIR}/public/shaderlib/BaseShader.h"
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/tier0/commonmacros.h"
	"${SRCDIR}/public/shaderlib/cshader.h"
	"${SRCDIR}/public/tier0/dbg.h"
	"${SRCDIR}/public/tier0/fasttimer.h"
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/tier0/icommandline.h"
	"${SRCDIR}/public/icvar.h"
	"${SRCDIR}/public/materialsystem/imaterial.h"
	"${SRCDIR}/public/materialsystem/imaterialsystem.h"
	"${SRCDIR}/public/materialsystem/imaterialsystemhardwareconfig.h"
	"${SRCDIR}/public/materialsystem/imaterialvar.h"
	"${SRCDIR}/public/materialsystem/imesh.h"
	"${SRCDIR}/public/materialsystem/IShader.h"
	"${SRCDIR}/public/materialsystem/ishaderapi.h"
	"${SRCDIR}/public/materialsystem/itexture.h"
	"${SRCDIR}/public/materialsystem/materialsystem_config.h"
	"${SRCDIR}/public/mathlib/mathlib.h"
	"${SRCDIR}/public/tier0/memdbgoff.h"
	"${SRCDIR}/public/tier0/memdbgon.h"
	"${SRCDIR}/public/tier0/platform.h"
	"${SRCDIR}/public/tier0/protected_things.h"
	"${SRCDIR}/public/shaderlib/ShaderDLL.h"
	"${SRCDIR}/public/string_t.h"
	"${SRCDIR}/public/tier1/strtools.h"
	"${SRCDIR}/public/tier1/utlmemory.h"
	"${SRCDIR}/public/tier1/utlvector.h"
	"${SRCDIR}/public/mathlib/vector.h"
	"${SRCDIR}/public/mathlib/vector2d.h"
	"${SRCDIR}/public/mathlib/vector4d.h"
	"${SRCDIR}/public/mathlib/vmatrix.h"
	"${SRCDIR}/public/mathlib/vplane.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"
)

add_library(
	shaderlib STATIC
	${SHADERLIB_SOURCE_FILES}
)

set_property(
	TARGET shaderlib PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	shaderlib PRIVATE

	"${SRCDIR}/materialsystem/"
)

target_compile_definitions(
	shaderlib PRIVATE

	FAST_MATERIALVAR_ACCESS
	"$<${IS_WINDOWS}:fopen=dont_use_fopen>"
)
