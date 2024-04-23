#=============================================================================
#	game_shader_dx9_tf2classic.cmake
#
#	Project Script
#=============================================================================

set( GAME_SHADER_DX9_TF2CLASSIC_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	GAME_SHADER_DX9_TF2CLASSIC_SOURCE_FILES

	# Source Files
	"${GAME_SHADER_DX9_TF2CLASSIC_DIR}/BaseVSShader.cpp"
	"${GAME_SHADER_DX9_TF2CLASSIC_DIR}/tf2classic/desaturate.cpp"

	# Header Files
	"${GAME_SHADER_DX9_TF2CLASSIC_DIR}/BaseVSShader.h"
)

add_library(
	game_shader_dx9 SHARED
	${GAME_SHADER_DX9_TF2CLASSIC_SOURCE_FILES}
)

set_property(
	TARGET game_shader_dx9 PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	game_shader_dx9
	""
	"game_shader_dx9"
	"${GAMEDIR}/tf2classic/bin"
)

target_include_directories(
	game_shader_dx9 PRIVATE

	"${GAME_SHADER_DX9_TF2CLASSIC_DIR}"
	"${SRCDIR}/materialsystem/stdshaders/fxctmp9"
	"${SRCDIR}/materialsystem/stdshaders/vshtmp9"
	"$<${IS_WINDOWS}:${DX9SDKDIR}/Include>"
	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
)

target_compile_definitions(
	game_shader_dx9 PRIVATE

	STDSHADER_DBG_DLL_EXPORT
	FAST_MATERIALVAR_ACCESS
	GAME_SHADER_DLL
)

target_link_libraries(
	game_shader_dx9 PRIVATE

	"$<${IS_WINDOWS}:${DX9SDK_LIBDIR}/d3dx9${STATIC_LIB_EXT}>"
	"$<${IS_WINDOWS}:version>"
	"$<${IS_WINDOWS}:winmm>"

	shaderlib
	mathlib
)
