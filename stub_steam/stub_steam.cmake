#=============================================================================
#	stub_steam.cmake
#
#	хуй в жопе
#=============================================================================

set( STUB_STEAM_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	STUB_STEAM_SOURCE_FILES

	"${STUB_STEAM_DIR}/steam_api.cpp"
)

add_library(
	steam_api SHARED
	${STUB_STEAM_SOURCE_FILES}
)

set_install_properties(
	steam_api
	""
	"steam_api"
	"${GAMEDIR}/bin"
)

set_property(
	TARGET steam_api PROPERTY FOLDER "Shared Libs"
)