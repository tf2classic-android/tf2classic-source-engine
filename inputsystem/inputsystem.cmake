#=============================================================================
#	inputsystem.cmake
#
#	Project Script
#=============================================================================

set( INPUTSYSTEM_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	INPUTSYSTEM_SOURCE_FILES

	# Source Files
	"${INPUTSYSTEM_DIR}/inputsystem.cpp"
	"${INPUTSYSTEM_DIR}/inputsystem.h"
	"${INPUTSYSTEM_DIR}/joystick_sdl.cpp"
	"${INPUTSYSTEM_DIR}/key_translation.cpp"
	"${INPUTSYSTEM_DIR}/key_translation.h"
	"${INPUTSYSTEM_DIR}/steamcontroller.cpp"
	"${INPUTSYSTEM_DIR}/touch_sdl.cpp"

	# Header Files
	"${SRCDIR}/public/inputsystem/AnalogCode.h"
	"${SRCDIR}/public/inputsystem/ButtonCode.h"
	"${SRCDIR}/public/inputsystem/iinputsystem.h"
	"${SRCDIR}/public/inputsystem/InputEnums.h"
)

add_library(
	inputsystem SHARED
	${INPUTSYSTEM_SOURCE_FILES}
)

set_property(
	TARGET inputsystem PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	inputsystem
	""
	"inputsystem"
	"${GAMEDIR}/bin"
)

target_include_directories(
	inputsystem PRIVATE

	"${THIRDPARTYDIR}/SDL"
)

target_compile_definitions(
	inputsystem PRIVATE

	VERSION_SAFE_STEAM_API_INTERFACES
)

target_link_libraries(
	inputsystem PRIVATE

	"$<${IS_OSX}:-framework IOKit>"
	"$<${IS_OSX}:-framework Carbon>"
	"$<${IS_OSX}:-framework ForceFeedback>"

	tier2
)

target_use_steamapi(
	inputsystem
)

target_use_sdl2(
	inputsystem
)
