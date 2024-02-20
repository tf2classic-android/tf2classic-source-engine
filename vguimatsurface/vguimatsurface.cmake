#=============================================================================
#	vguimatsurface.cmake
#
#	Project Script
#=============================================================================

# CMAKETODO:
#	$EnableC++Exceptions			"Yes (/EHsc)"

set( VGUIMATSURFACE_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VGUIMATSURFACE_SOURCE_FILES

	# Source Files
	"${VGUIMATSURFACE_DIR}/Clip2D.cpp"
	"${VGUIMATSURFACE_DIR}/Cursor.cpp"
	"${VGUIMATSURFACE_DIR}/FontTextureCache.cpp"
	"${VGUIMATSURFACE_DIR}/Input.cpp"
	"${VGUIMATSURFACE_DIR}/MatSystemSurface.cpp"
	"$<${IS_WINDOWS}:${VGUIMATSURFACE_DIR}/memorybitmap.cpp>"
	"${VGUIMATSURFACE_DIR}/TextureDictionary.cpp"
	"${SRCDIR}/vgui2/src/vgui_key_translation.cpp"
	"${SRCDIR}/public/vgui_controls/vgui_controls.cpp"
	"${SRCDIR}/public/filesystem_helpers.cpp"

	# Public Header Files
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/tier1/characterset.h"
	"${SRCDIR}/public/tier1/checksum_crc.h"
	"${SRCDIR}/public/tier0/commonmacros.h"
	"${SRCDIR}/public/filesystem.h"
	"${SRCDIR}/public/filesystem_helpers.h"
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/materialsystem/imaterial.h"
	"${SRCDIR}/public/materialsystem/imaterialsystem.h"
	"${SRCDIR}/public/materialsystem/imaterialvar.h"
	"${SRCDIR}/public/VGuiMatSurface/IMatSystemSurface.h"
	"${SRCDIR}/public/materialsystem/imesh.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/materialsystem/itexture.h"
	"${SRCDIR}/public/pixelwriter.h"
	"${SRCDIR}/public/tier1/strtools.h"
	"${SRCDIR}/public/tier1/utllinkedlist.h"
	"${SRCDIR}/public/tier1/utlmemory.h"
	"${SRCDIR}/public/tier1/utlrbtree.h"
	"${SRCDIR}/public/tier1/utlvector.h"
	"${SRCDIR}/public/mathlib/vector.h"
	"${SRCDIR}/public/mathlib/vector2d.h"
	"${SRCDIR}/public/mathlib/vector4d.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"
	"${SRCDIR}/public/vtf/vtf.h"

	# Common Header Files
	"$<${IS_WINDOWS}:${SRCDIR}/common/vgui_surfacelib/Win32Font.h>"
	"${SRCDIR}/common/vgui_surfacelib/FontAmalgam.h"
	"${SRCDIR}/common/vgui_surfacelib/FontManager.h"
)

add_library(
	vguimatsurface SHARED
	${VGUIMATSURFACE_SOURCE_FILES}
)

set_property(
	TARGET vguimatsurface PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	vguimatsurface
	""
	"vguimatsurface"
	"${GAMEDIR}/bin"
)

target_include_directories(
	vguimatsurface PRIVATE

	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
	"$<${IS_POSIX}:/usr/include/freetype2>"
	"$<${IS_OSX}:/usr/local/include>"
	"$<${IS_OSX}:/usr/local/include/freetype2>"
	"$<${IS_OSX}:/usr/local/include/fontconfig>"
	"$<${IS_WINDOWS}:${THIRDPARTYDIR}/freetype/include>"
	"$<${IS_WINDOWS}:${THIRDPARTYDIR}/fontconfig/fontconfig>"
)

target_compile_definitions(
	vguimatsurface PRIVATE

	BENCHMARK
	VGUIMATSURFACE_DLL_EXPORT
	GAMEUI_EXPORTS
	DONT_PROTECT_FILEIO_FUNCTIONS
	PROTECTED_THINGS_ENABLE
)

target_link_libraries(
	vguimatsurface PRIVATE

	"$<${IS_OSX}:-framework Carbon>"
	"$<${IS_X360}:vgui_surfacelib>"

	vgui_controls

	# OSX freetype2 depencencies
	"$<${IS_OSX}:${LIBCOMMON}/libbz2${STATIC_LIB_EXT}>"
	"$<${IS_OSX}:expat>"
	"$<${IS_OSX}:${LIBPUBLIC}/libz${STATIC_LIB_EXT}>" # png dep
	"$<${IS_OSX}:${LIBPUBLIC}/libpng${STATIC_LIB_EXT}>"

	bitmap
	mathlib
	tier2
	tier3
	vgui_surfacelib
	"$<$<AND:${IS_POSIX},$<NOT:${IS_ANDROID}>>:fontconfig>"
	"$<${IS_POSIX}:freetype>"
	"$<$<AND:${IS_ANDROID},$<NOT:${IS_64BIT}>>:expat>"
)

if( ${USE_SDL} )
	target_use_sdl2(
		vguimatsurface
	)
endif()
