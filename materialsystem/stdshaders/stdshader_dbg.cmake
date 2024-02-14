#=============================================================================
#	stdshader_dbg.cmake
#
#	Project Script
#=============================================================================

set( STDSHADER_DBG_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	STDSHADER_DBG_SOURCE_FILES

	# Source Files
	"${STDSHADER_DBG_DIR}/BaseVSShader.cpp"
	"${STDSHADER_DBG_DIR}/debugdepth.cpp"
	"${STDSHADER_DBG_DIR}/debugdrawenvmapmask.cpp"
	"${STDSHADER_DBG_DIR}/debugluxel.cpp"
	"${STDSHADER_DBG_DIR}/debugnormalmap.cpp"
	"${STDSHADER_DBG_DIR}/debugtangentspace.cpp"
	"${STDSHADER_DBG_DIR}/fillrate.cpp"

	# Header Files
	"${STDSHADER_DBG_DIR}/BaseVSShader.h"
)

add_library(
	stdshader_dbg SHARED
	${STDSHADER_DBG_SOURCE_FILES}
)

set_property(
	TARGET stdshader_dbg PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	stdshader_dbg
	""
	"stdshader_dbg"
	"${GAMEDIR}/bin"
)

target_include_directories(
	stdshader_dbg PRIVATE

	"${SRCDIR}/materialsystem/stdshaders/fxctmp9"
	"${SRCDIR}/materialsystem/stdshaders/vshtmp9"
	"$<${IS_WINDOWS}:${DX9SDKDIR}/Include>"
	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL2>"
)

target_compile_definitions(
	stdshader_dbg PRIVATE

	STDSHADER_DBG_DLL_EXPORT
	FAST_MATERIALVAR_ACCESS
)

target_link_libraries(
	stdshader_dbg PRIVATE

	"$<${IS_WINDOWS}:${DX9SDK_LIBDIR}/d3dx9${STATIC_LIB_EXT}>"
	"$<${IS_WINDOWS}:version>"
	"$<${IS_WINDOWS}:winmm>"

	mathlib
	shaderlib
)

#	$File	"$SRCDIR\devtools\bin\vsh_prep.pl"
#	$File	"$SRCDIR\devtools\bin\psh_prep.pl"
#	$File	"$SRCDIR\devtools\bin\fxc_prep.pl"
#	$File	"$SRCDIR\devtools\bin\updateshaders.pl"
#
#	$Shaders	"stdshader_dx9_20b.txt"
#	$Shaders	"stdshader_dx9_30.txt"
#	//$Shaders	"stdshader_dx10.txt"
