#=============================================================================
#	vgui_dll.cmake
#
#	Project Script
#=============================================================================

set( VGUI_DLL_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VGUI_DLL_SOURCE_FILES

	# Source Files
	"${VGUI_DLL_DIR}/Bitmap.cpp"
	"${VGUI_DLL_DIR}/Border.cpp"
	"${VGUI_DLL_DIR}/ScalableImageBorder.cpp"
	"${VGUI_DLL_DIR}/ImageBorder.cpp"
	"${VGUI_DLL_DIR}/fileimage.cpp"
	"${VGUI_DLL_DIR}/InputWin32.cpp"
	"${VGUI_DLL_DIR}/LocalizedStringTable.cpp"
	"${VGUI_DLL_DIR}/MemoryBitmap.cpp"
	"${VGUI_DLL_DIR}/Memorybitmap.h"
	"${VGUI_DLL_DIR}/MessageListener.cpp"
	"${VGUI_DLL_DIR}/Scheme.cpp"
	"$<${IS_WINDOWS}:${VGUI_DLL_DIR}/Surface.cpp>"
	"$<${IS_WINDOWS}:${VGUI_DLL_DIR}/System.cpp>"
	"$<${IS_POSIX}:${VGUI_DLL_DIR}/system_posix.cpp>"
	"${VGUI_DLL_DIR}/vgui.cpp"
	"${VGUI_DLL_DIR}/vgui_internal.cpp"
	"${VGUI_DLL_DIR}/vgui_key_translation.cpp"
	"${VGUI_DLL_DIR}/VPanel.cpp"
	"${VGUI_DLL_DIR}/VPanelWrapper.cpp"
	"${VGUI_DLL_DIR}/keyrepeat.cpp"
	"${SRCDIR}/public/filesystem_helpers.cpp"
	"${SRCDIR}/public/filesystem_init.cpp"
	"${SRCDIR}/public/UnicodeFileHelpers.cpp"

	# Header Files
	"${VGUI_DLL_DIR}/bitmap.h"
	"${VGUI_DLL_DIR}/fileimage.h"
	"${VGUI_DLL_DIR}/IMessageListener.h"
	"${VGUI_DLL_DIR}/vgui_internal.h"
	"${VGUI_DLL_DIR}/vgui_key_translation.h"
	"${VGUI_DLL_DIR}/VPanel.h"

	# Public Header Files
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/Color.h"
	"${SRCDIR}/public/vgui/Cursor.h"
	"${SRCDIR}/public/filesystem.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/vgui/KeyCode.h"
	"${SRCDIR}/public/tier1/strtools.h"
	"${SRCDIR}/public/UnicodeFileHelpers.h"
	"${SRCDIR}/public/tier1/utlbuffer.h"
	"${SRCDIR}/public/tier1/utllinkedlist.h"
	"${SRCDIR}/public/tier1/utlmemory.h"
	"${SRCDIR}/public/tier1/utlpriorityqueue.h"
	"${SRCDIR}/public/tier1/utlrbtree.h"
	"${SRCDIR}/public/tier1/utlvector.h"
	"${SRCDIR}/public/mathlib/vector2d.h"
	"${SRCDIR}/public/vgui/VGUI.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"
	"${SRCDIR}/public/vgui/keyrepeat.h"

	# Common Header Files
	"$<${IS_WINDOWS}:${SRCDIR}/common/vgui_surfacelib/Win32Font.h>"
	"${SRCDIR}/common/vgui_surfacelib/FontAmalgam.h"
	"${SRCDIR}/common/vgui_surfacelib/FontManager.h"
	"${SRCDIR}/common/SteamBootStrapper.h"

	# Interfacs
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/vgui/IBorder.h"
	"${SRCDIR}/public/vgui/IClientPanel.h"
	"${SRCDIR}/public/vgui/IHTML.h"
	"${SRCDIR}/public/vgui/IImage.h"
	"${SRCDIR}/public/vgui/IInput.h"
	"${SRCDIR}/public/vgui/ILocalize.h"
	"${SRCDIR}/public/vgui/IPanel.h"
	"${SRCDIR}/public/vgui/IScheme.h"
	"${SRCDIR}/public/vgui/ISurface.h"
	"${SRCDIR}/public/vgui/ISystem.h"
	"${SRCDIR}/public/vgui/IVGui.h"
	"${SRCDIR}/public/vgui/IVguiMatInfo.h"
	"${SRCDIR}/public/vgui/IVguiMatInfoVar.h"
	"${VGUI_DLL_DIR}/VGUI_Border.h"
	"${VGUI_DLL_DIR}/ScalableImageBorder.h"
	"${VGUI_DLL_DIR}/ImageBorder.h"
)

add_library(
	vgui_dll MODULE
	${VGUI_DLL_SOURCE_FILES}
)

set_property(
	TARGET vgui_dll PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	vgui_dll
	""
	"vgui2"
	"${GAMEDIR}/bin"
)

target_include_directories(
	vgui_dll PRIVATE

	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
	"$<${IS_WINDOWS}:${THIRDPARTYDIR}/freetype-2.13.2/include>"
	"$<${IS_WINDOWS}:${THIRDPARTYDIR}/fontconfig-2.15.0/include>"
)

target_compile_definitions(
	vgui_dll PRIVATE

	DONT_PROTECT_FILEIO_FUNCTIONS
)

target_link_libraries(
	vgui_dll PRIVATE

	"$<${IS_WINDOWS}:imm32>"
	"$<${IS_WINDOWS}:shlwapi>"
	"$<${IS_WINDOWS}:odbc32>"
	"$<${IS_WINDOWS}:odbccp32>"
	"$<${IS_WINDOWS}:winmm>"

	"$<${IS_OSX}:-framework Carbon>"

	vgui_surfacelib
	tier2
	tier3
)

if( ${USE_SDL} )
	target_use_sdl2(
		vgui_dll
	)
endif()
