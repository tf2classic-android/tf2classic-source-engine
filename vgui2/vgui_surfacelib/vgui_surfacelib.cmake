#=============================================================================
#	vgui_surfacelib.cmake
#
#	Project Script
#=============================================================================

set( VGUI_SURFACELIB_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VGUI_SURFACELIB_SOURCE_FILES

	# Source Files
	"${VGUI_SURFACELIB_DIR}/BitmapFont.cpp"
	"${VGUI_SURFACELIB_DIR}/FontAmalgam.cpp"
	"${VGUI_SURFACELIB_DIR}/FontManager.cpp"
	"${VGUI_SURFACELIB_DIR}/FontEffects.cpp"
	"$<$<AND:${IS_WINDOWS},$<NOT:${IS_X360}>>:${VGUI_SURFACELIB_DIR}/Win32Font.cpp>"
	"$<${IS_POSIX}:${VGUI_SURFACELIB_DIR}/linuxfont.cpp>" # TODO(SanyaSho): rename me to freetypefont.cpp!

	# Header Files
	"${VGUI_SURFACELIB_DIR}/FontEffects.h"

	# Public Header Files
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/tier1/strtools.h"
	"${SRCDIR}/public/tier1/utlbuffer.h"
	"${SRCDIR}/public/tier1/utlmemory.h"
	"${SRCDIR}/public/tier1/utlvector.h"
	"${SRCDIR}/public/mathlib/vector2d.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"

	# Common Header Files
	"${SRCDIR}/common/vgui_surfacelib/vguifont.h"
	"$<${IS_WINDOWS}:${SRCDIR}/common/vgui_surfacelib/Win32Font.h>"
	"${SRCDIR}/common/vgui_surfacelib/BitmapFont.h"
	"${SRCDIR}/common/vgui_surfacelib/FontAmalgam.h"
	"${SRCDIR}/common/vgui_surfacelib/FontManager.h"
)

add_library(
	vgui_surfacelib STATIC
	${VGUI_SURFACELIB_SOURCE_FILES}
)

set_property(
	TARGET vgui_surfacelib PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	vgui_surfacelib PRIVATE

	"${THIRDPARTYDIR}"
	"$<${IS_POSIX}:/usr/include/freetype2>"
	"$<${IS_OSX}:/usr/local/include>"
	"$<${IS_OSX}:/usr/local/include/freetype2>"
	"$<${IS_OSX}:/usr/local/include/fontconfig>"
	"$<${IS_WINDOWS}:${THIRDPARTYDIR}/freetype/include>"
	"$<${IS_WINDOWS}:${THIRDPARTYDIR}/fontconfig/fontconfig>"
)
