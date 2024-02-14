#=============================================================================
#	shaderapiempty.cmake
#
#	Project Script
#=============================================================================

set( SHADERAPIEMPTY_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	SHADERAPIEMPTY_SOURCE_FILES

	# Source Files
	"${SHADERAPIEMPTY_DIR}/shaderapiempty.cpp"

	# Header Files
	"${SRCDIR}/public/tier0/platform.h"
)

add_library(
	shaderapiempty SHARED
	${SHADERAPIEMPTY_SOURCE_FILES}
)

set_install_properties(
	shaderapiempty
	""
	"shaderapiempty"
	"${GAMEDIR}/bin"
)

set_property(
	TARGET shaderapiempty PROPERTY FOLDER "Shared Libs"
)

target_include_directories(
	shaderapiempty PRIVATE

	"${SRCDIR}/materialsystem"
)

target_compile_definitions(
	shaderapiempty PRIVATE

	SHADER_DLL_EXPORT
	PROTECTED_THINGS_ENABLE
)

target_link_libraries(
	shaderapiempty PRIVATE

	"$<${IS_WINDOWS}:odbc32>"
	"$<${IS_WINDOWS}:odbccp32>"
)
