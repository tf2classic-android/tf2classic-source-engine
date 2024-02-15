#=============================================================================
#	video_services.cmake
#
#	Project Script
#=============================================================================

set( VIDEO_SERVICES_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VIDEO_SERVICES_SOURCE_FILES

	# Source Files
	"${VIDEO_SERVICES_DIR}/videoservices.cpp"

	# Header Files
	"${VIDEO_SERVICES_DIR}/video_macros.h"
	"${VIDEO_SERVICES_DIR}/videoservices.h"
	"${VIDEO_SERVICES_DIR}/videosubsystem.h"

	# Interface
	"${SRCDIR}/public/video/ivideoservices.h"
)

add_library(
	video_services SHARED
	${VIDEO_SERVICES_SOURCE_FILES}
)

set_property(
	TARGET video_services PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	video_services
	""
	"video_services"
	"${GAMEDIR}/bin"
)

target_include_directories(
	video_services PRIVATE

	"${SRCDIR}/public/video"
	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
)

target_link_libraries(
	video_services PRIVATE

	"$<${IS_OSX}:-framework Carbon>"

	"$<$<AND:${IS_WINDOWS},${USE_SDL}>:appframework>"
	tier2
	tier3
)

if( ${IS_WINDOWS} AND ${USE_SDL} )
	target_use_sdl2(
		video_services
	)
endif()
