#=============================================================================
#	stdshader_dx9.cmake
#
#	Project Script
#=============================================================================

set( STDSHADER_DX9_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	STDSHADER_DX9_SOURCE_FILES

	# Source Files
	"${STDSHADER_DX9_DIR}/aftershock.cpp"
	"${STDSHADER_DX9_DIR}/aftershock_helper.cpp"
	"${STDSHADER_DX9_DIR}/AccumBuff4Sample.cpp"
	"${STDSHADER_DX9_DIR}/accumbuff5sample.cpp"
	"${STDSHADER_DX9_DIR}/BaseVSShader.cpp"
	"${STDSHADER_DX9_DIR}/bik_dx90.cpp"
	"${STDSHADER_DX9_DIR}/Bloom.cpp"
	"${STDSHADER_DX9_DIR}/BlurFilterX.cpp"
	"${STDSHADER_DX9_DIR}/BlurFilterY.cpp"
	"${STDSHADER_DX9_DIR}/BufferClearObeyStencil_dx9.cpp"
	"${STDSHADER_DX9_DIR}/cable_dx9.cpp"
	"${STDSHADER_DX9_DIR}/cloak.cpp"
	"${STDSHADER_DX9_DIR}/cloak_blended_pass_helper.cpp"
	"${STDSHADER_DX9_DIR}/cloak_dx9_helper.cpp"
	"${STDSHADER_DX9_DIR}/cloud_dx9.cpp"
	"${STDSHADER_DX9_DIR}/colorcorrection.cpp"
	"${STDSHADER_DX9_DIR}/compositor.cpp"
	"${STDSHADER_DX9_DIR}/core_dx9.cpp"
	"${STDSHADER_DX9_DIR}/color_projection.cpp"
	"${STDSHADER_DX9_DIR}/debugmrttexture.cpp"
	"$<${IS_WINDOWS}:${STDSHADER_DX9_DIR}/debugmorphaccumulator_dx9.cpp>"
	"${STDSHADER_DX9_DIR}/DebugTextureView.cpp"
	"${STDSHADER_DX9_DIR}/DecalBaseTimesLightmapAlphaBlendSelfIllum_dx9.cpp"
	"${STDSHADER_DX9_DIR}/DecalModulate_dx9.cpp"
	"${STDSHADER_DX9_DIR}/depthwrite.cpp"
	"${STDSHADER_DX9_DIR}/Downsample.cpp"
	"${STDSHADER_DX9_DIR}/downsample_nohdr.cpp"
	"${STDSHADER_DX9_DIR}/Engine_Post_dx9.cpp"
	"${STDSHADER_DX9_DIR}/emissive_scroll_blended_pass_helper.cpp"
	"${STDSHADER_DX9_DIR}/eye_refract.cpp"
	"${STDSHADER_DX9_DIR}/eye_refract_helper.cpp"
	"${STDSHADER_DX9_DIR}/eyes_dx8_dx9_helper.cpp"
	"${STDSHADER_DX9_DIR}/eyes_dx9.cpp"
	"${STDSHADER_DX9_DIR}/eyeglint_dx9.cpp"
	"${STDSHADER_DX9_DIR}/filmdust_dx8_dx9.cpp"
	"${STDSHADER_DX9_DIR}/filmgrain_dx8_dx9.cpp"
	"${STDSHADER_DX9_DIR}/flesh_interior_blended_pass_helper.cpp"
	"${STDSHADER_DX9_DIR}/floatcombine.cpp"
	"${STDSHADER_DX9_DIR}/floatcombine_autoexpose.cpp"
	"${STDSHADER_DX9_DIR}/floattoscreen.cpp"
	"${STDSHADER_DX9_DIR}/floattoscreen_vanilla.cpp"
	"${STDSHADER_DX9_DIR}/HDRCombineTo16Bit.cpp"
	"${STDSHADER_DX9_DIR}/HDRSelectRange.cpp"
	"${STDSHADER_DX9_DIR}/hsl_filmgrain_pass1.cpp"
	"${STDSHADER_DX9_DIR}/hsl_filmgrain_pass2.cpp"
	"${STDSHADER_DX9_DIR}/hsv.cpp"
	"${STDSHADER_DX9_DIR}/introscreenspaceeffect.cpp"
	"${STDSHADER_DX9_DIR}/lightmappedgeneric_dx9.cpp"
	"${STDSHADER_DX9_DIR}/lightmappedgeneric_dx9_helper.cpp"
	"${STDSHADER_DX9_DIR}/lightmappedreflective.cpp"
	"${STDSHADER_DX9_DIR}/modulate_dx9.cpp"
	"${STDSHADER_DX9_DIR}/MonitorScreen_dx9.cpp"
	"$<${IS_WINDOWS}:${STDSHADER_DX9_DIR}/morphaccumulate_dx9.cpp>"
	"$<${IS_WINDOWS}:${STDSHADER_DX9_DIR}/morphweight_dx9.cpp>"
	"${STDSHADER_DX9_DIR}/motion_blur_dx9.cpp"
	"${STDSHADER_DX9_DIR}/occlusion_dx9.cpp"
	"${STDSHADER_DX9_DIR}/particlelitgeneric_dx9.cpp"
	"${STDSHADER_DX9_DIR}/particlelitgeneric_dx9_helper.cpp"
	"${STDSHADER_DX9_DIR}/particlesphere_dx9.cpp"
	"${STDSHADER_DX9_DIR}/portal.cpp"
	"${STDSHADER_DX9_DIR}/portalstaticoverlay.cpp"
	"${STDSHADER_DX9_DIR}/portal_refract.cpp"
	"${STDSHADER_DX9_DIR}/portal_refract_helper.cpp"
	"${STDSHADER_DX9_DIR}/pyro_vision.cpp"
	"${STDSHADER_DX9_DIR}/refract.cpp"
	"${STDSHADER_DX9_DIR}/refract_dx9_helper.cpp"
	"$<${IS_X360}:${STDSHADER_DX9_DIR}/rendertargetblit_x360.cpp>"
	"${STDSHADER_DX9_DIR}/sample4x4.cpp"
	"${STDSHADER_DX9_DIR}/sample4x4_blend.cpp"
	"${STDSHADER_DX9_DIR}/screenspace_general.cpp"
	"${STDSHADER_DX9_DIR}/sfm_blurfilterx.cpp"
	"${STDSHADER_DX9_DIR}/sfm_blurfiltery.cpp"
	"${STDSHADER_DX9_DIR}/sfm_downsample.cpp"
	"${STDSHADER_DX9_DIR}/sfm_integercombine.cpp"
	"${STDSHADER_DX9_DIR}/shadow.cpp"
	"${STDSHADER_DX9_DIR}/shadowbuild_dx9.cpp"
	"${STDSHADER_DX9_DIR}/shadowmodel_dx9.cpp"
	"${STDSHADER_DX9_DIR}/shatteredglass.cpp"
	"${STDSHADER_DX9_DIR}/showz.cpp"
	"${STDSHADER_DX9_DIR}/skin_dx9_helper.cpp"
	"${STDSHADER_DX9_DIR}/sky_dx9.cpp"
	"${STDSHADER_DX9_DIR}/sky_hdr_dx9.cpp"
	"${STDSHADER_DX9_DIR}/sprite_dx9.cpp"
	"${STDSHADER_DX9_DIR}/spritecard.cpp"
	"${STDSHADER_DX9_DIR}/teeth.cpp"
	"${STDSHADER_DX9_DIR}/TreeLeaf.cpp"
	"${STDSHADER_DX9_DIR}/unlitgeneric_dx9.cpp"
	"${STDSHADER_DX9_DIR}/unlittwotexture_dx9.cpp"
	"${STDSHADER_DX9_DIR}/vertexlitgeneric_dx9.cpp"
	"${STDSHADER_DX9_DIR}/vertexlitgeneric_dx9_helper.cpp"
	"${STDSHADER_DX9_DIR}/volume_clouds.cpp"
	"${STDSHADER_DX9_DIR}/volume_clouds_helper.cpp"
	"${STDSHADER_DX9_DIR}/vortwarp_dx9.cpp"
	"${STDSHADER_DX9_DIR}/vr_distort_hud.cpp"
	"${STDSHADER_DX9_DIR}/vr_distort_texture.cpp"
	"${STDSHADER_DX9_DIR}/warp.cpp"
	"${STDSHADER_DX9_DIR}/water.cpp"
	"${STDSHADER_DX9_DIR}/weapon_sheen_pass_helper.cpp"
	"${STDSHADER_DX9_DIR}/windowimposter_dx90.cpp"
	"${STDSHADER_DX9_DIR}/wireframe_dx9.cpp"
	"${STDSHADER_DX9_DIR}/worldtwotextureblend.cpp"
	"${STDSHADER_DX9_DIR}/worldvertexalpha.cpp"
	"${STDSHADER_DX9_DIR}/worldvertextransition.cpp"
	"${STDSHADER_DX9_DIR}/worldvertextransition_dx8_helper.cpp"
	"${STDSHADER_DX9_DIR}/writez_dx9.cpp"
	"${STDSHADER_DX9_DIR}/writestencil_dx9.cpp"

	"$<${IS_WINDOWS}:${SRCDIR}/tier1/checksum_crc.cpp>"
	"$<${IS_WINDOWS}:${SRCDIR}/tier1/checksum_md5.cpp>"

	# 360 collapes stdshader_dbg.dll into stdshader_dx9.dll
	"$<${IS_X360}:${STDSHADER_DX9_DIR}/debugdepth.cpp>"
	"$<${IS_X360}:${STDSHADER_DX9_DIR}/DebugDrawEnvmapMask.cpp>"
	"$<${IS_X360}:${STDSHADER_DX9_DIR}/debugluxel.cpp>"
	"$<${IS_X360}:${STDSHADER_DX9_DIR}/debugnormalmap.cpp>"
	"$<${IS_X360}:${STDSHADER_DX9_DIR}/debugtangentspace.cpp>"
	"$<${IS_X360}:${STDSHADER_DX9_DIR}/fillrate.cpp>"

	# dead shader, but hl2 models have it
	"$<${IS_X360}:${STDSHADER_DX9_DIR}/eyeball.cpp>"

	# Header Files
	"${STDSHADER_DX9_DIR}/BaseVSShader.h"
	"${STDSHADER_DX9_DIR}/cloak_dx9_helper.h"
	"${STDSHADER_DX9_DIR}/common_fxc.h"
	"${STDSHADER_DX9_DIR}/common_hlsl_cpp_consts.h"
	"${STDSHADER_DX9_DIR}/common_ps_fxc.h"
	"${STDSHADER_DX9_DIR}/common_vertexlitgeneric_dx9.h"
	"${STDSHADER_DX9_DIR}/common_vs_fxc.h"
	"${STDSHADER_DX9_DIR}/lightmappedgeneric_dx9_helper.h"
	"${STDSHADER_DX9_DIR}/lightmappedgeneric_ps2_3_x.h"
	"${STDSHADER_DX9_DIR}/refract_dx9_helper.h"
	"${STDSHADER_DX9_DIR}/shader_constant_register_map.h"
	"${STDSHADER_DX9_DIR}/skin_dx9_helper.h"
	"${STDSHADER_DX9_DIR}/particlelitgeneric_dx9_helper.h"
	"${STDSHADER_DX9_DIR}/vertexlitgeneric_dx95_helper.h"
	"${STDSHADER_DX9_DIR}/vortwarp_vs20_helper.h"
	"${STDSHADER_DX9_DIR}/worldvertextransition_dx8_helper.h"
	"${STDSHADER_DX9_DIR}/water_ps2x_helper.h"
	"${STDSHADER_DX9_DIR}/commandbuilder.h"
)

add_library(
	stdshader_dx9 SHARED
	${STDSHADER_DX9_SOURCE_FILES}
)

set_property(
	TARGET stdshader_dx9 PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	stdshader_dx9
	""
	"stdshader_dx9"
	"${GAMEDIR}/bin"
)

target_compile_definitions(
	stdshader_dx9 PRIVATE

	STDSHADER_DX9_DLL_EXPORT
	FAST_MATERIALVAR_ACCESS
	"$<$<AND:${IS_WINDOWS},$<NOT:${USE_GL}>>:USE_ACTUAL_DX>"
)

target_include_directories(
	stdshader_dx9 PRIVATE

	"${SRCDIR}/materialsystem/stdshaders/fxctmp9"
	"${SRCDIR}/materialsystem/stdshaders/vshtmp9"
	"$<${IS_WINDOWS}:${DX9SDKDIR}/Include>"
	"$<${IS_X360}:${SRCDIR}/materialsystem/stdshaders/fxctmp9_360>"
	"$<${IS_X360}:${SRCDIR}/materialsystem/stdshaders/vshtmp9_360>"
	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL2>"
)

target_link_libraries(
	stdshader_dx9 PRIVATE

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
#	$File	"$SRCDIR\devtools\bin\copyshaders.pl"
#	$File	"$SRCDIR\devtools\bin\valve_perl_helpers.pl"
#	$File	"$SRCDIR\devtools\bin\checkshaderchecksums.pl"
#	$File	"buildallshaders.bat"
#	$File	"buildshaders.bat"
#
#	$Shaders	"stdshader_dx9_20b.txt"
#	$Shaders	"stdshader_dx9_30.txt"
#	//$Shaders	"stdshader_dx10.txt"
