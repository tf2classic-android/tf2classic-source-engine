#=============================================================================
#	shaderapidx9.cmake
#
#	Project Script
#=============================================================================

# CMAKETODO:
#$Configuration "Debug"
#{
#	$Linker [$X360]
#	{
#		$AdditionalDependencies		"$BASE d3dx9d.lib xuirund.lib xuirenderd.lib xaudiod2.lib xmcored.lib"
#	}
#}
#
#$Configuration "Release"
#{
#	$Linker [$X360]
#	{
#		$AdditionalDependencies		"$BASE d3dx9.lib xuirun.lib xuirender.lib xaudio2.lib xmcore.lib"
#	}
#}

set( SHADERAPIDX9_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	SHADERAPIDX9_SOURCE_FILES

	# Source Files
	"${SHADERAPIDX9_DIR}/colorformatdx8.cpp"
	"${SRCDIR}/public/filesystem_helpers.cpp"
	"${SHADERAPIDX9_DIR}/hardwareconfig.cpp"
	"${SHADERAPIDX9_DIR}/meshbase.cpp"
	"${SHADERAPIDX9_DIR}/meshdx8.cpp"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${SHADERAPIDX9_DIR}/recording.cpp>"
	"${SHADERAPIDX9_DIR}/shaderapidx8.cpp"
	"${SHADERAPIDX9_DIR}/shaderdevicebase.cpp"
	"${SHADERAPIDX9_DIR}/shaderapibase.cpp"
	"${SHADERAPIDX9_DIR}/shaderdevicedx8.cpp"
	"${SHADERAPIDX9_DIR}/shadershadowdx8.cpp"
	"${SHADERAPIDX9_DIR}/texturedx8.cpp"
	"${SHADERAPIDX9_DIR}/TransitionTable.cpp"
	"${SHADERAPIDX9_DIR}/cvballoctracker.cpp"
	"${SHADERAPIDX9_DIR}/vertexdecl.cpp"
	"${SHADERAPIDX9_DIR}/vertexshaderdx8.cpp"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${SHADERAPIDX9_DIR}/wmi.cpp>"
	"$<${IS_X360}:${SHADERAPIDX9_DIR}/textureheap.cpp>"
	"$<${IS_POSIX}:${SHADERAPIDX9_DIR}/winutils.cpp>"

	# DirectX Header Files
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${DX9SDKDIR}/Include/d3dx9.h>"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${DX9SDKDIR}/Include/d3dx9anim.h>"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${DX9SDKDIR}/Include/d3dx9core.h>"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${DX9SDKDIR}/Include/d3dx9effect.h>"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${DX9SDKDIR}/Include/d3dx9math.h>"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${DX9SDKDIR}/Include/d3dx9math.inl>"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${DX9SDKDIR}/Include/d3dx9mesh.h>"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${DX9SDKDIR}/Include/d3dx9shader.h>"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${DX9SDKDIR}/Include/d3dx9shape.h>"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${DX9SDKDIR}/Include/d3dx9tex.h>"

	# Header Files
	"${SHADERAPIDX9_DIR}/TransitionTable.h"
	"${SHADERAPIDX9_DIR}/vertexdecl.h"
	"${SHADERAPIDX9_DIR}/colorformatdx8.h"
	"${SHADERAPIDX9_DIR}/dynamicib.h"
	"${SHADERAPIDX9_DIR}/dynamicvb.h"
	"${SHADERAPIDX9_DIR}/hardwareconfig.h"
	"${SHADERAPIDX9_DIR}/meshbase.h"
	"${SHADERAPIDX9_DIR}/imeshdx8.h"
	"${SHADERAPIDX9_DIR}/locald3dtypes.h"
	"${SHADERAPIDX9_DIR}/recording.h"
	"${SHADERAPIDX9_DIR}/shaderapidx8.h"
	"${SHADERAPIDX9_DIR}/shaderdevicebase.h"
	"${SHADERAPIDX9_DIR}/shaderapibase.h"
	"${SHADERAPIDX9_DIR}/shaderdevicedx8.h"
	"${SHADERAPIDX9_DIR}/shaderapidx8_global.h"
	"${SHADERAPIDX9_DIR}/shadershadowdx8.h"
	"${SHADERAPIDX9_DIR}/stubd3ddevice.h"
	"${SHADERAPIDX9_DIR}/texturedx8.h"
	"${SHADERAPIDX9_DIR}/vertexshaderdx8.h"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${SHADERAPIDX9_DIR}/wmi.h>"
	"$<${IS_X360}:${SHADERAPIDX9_DIR}/textureheap.h>"
	"$<${IS_POSIX}:${SHADERAPIDX9_DIR}/winutils.h>"

	# Public Header Files
	"${SRCDIR}/public/shaderapi/IShaderDevice.h"
	"${SRCDIR}/public/shaderapi/ishaderutil.h"
	"${SRCDIR}/public/shaderapi/ishaderapi.h"
	"${SRCDIR}/public/shaderapi/ishaderdynamic.h"
	"${SRCDIR}/public/shaderapi/ishadershadow.h"
	"${SRCDIR}/public/materialsystem/idebugtextureinfo.h"
	"${SRCDIR}/public/materialsystem/ivballoctracker.h"
	"${SRCDIR}/public/materialsystem/shader_vcs_version.h"
)

add_library(
	shaderapidx9 SHARED
	${SHADERAPIDX9_SOURCE_FILES}
)

set_property(
	TARGET shaderapidx9 PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	shaderapidx9
	""
	"shaderapidx9"
	"${GAMEDIR}/bin"
)

target_include_directories(
	shaderapidx9 PRIVATE

	"${SRCDIR}/materialsystem"
	"<$<${IS_WINDOWS}:${DX9SDKDIR}/Include>"
	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
)

target_compile_definitions(
	shaderapidx9 PRIVATE

	SHADERAPIDX9
	SHADER_DLL_EXPORT
	PROTECTED_THINGS_ENABLE
	strncpy=use_Q_strncpy_instead
	_snprintf=use_Q_snprintf_instead
	"$<$<AND:$<OR:${IS_WINDOWS},${IS_X360}>,$<NOT:${USE_GL}>>:USE_ACTUAL_DX>"
)

target_link_libraries(
	shaderapidx9 PRIVATE

	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:d3d9>"
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:${DX9SDK_LIBDIR}/d3dx9${STATIC_LIB_EXT}>"
	"$<${IS_WINDOWS}:ws2_32>"

	"$<${IS_OSX}:-framework Carbon>"
	"$<${IS_OSX}:-framework OpenGL>"
	"$<${IS_OSX}:-framework Quartz>"
	"$<${IS_OSX}:-framework Cocoa>"
	"$<${IS_OSX}:-framework IOKit>"

	bitmap
	mathlib
	tier2
	bz2
	#"${LIBCOMMON}/libbz2${STATIC_LIB_EXT}"
	"$<${USE_GL}:togl>" # [!$IS_LIB_PROJECT && $GL]
)
