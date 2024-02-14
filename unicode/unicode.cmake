#=============================================================================
#	unicode.cmake
#
#	Project Script
#=============================================================================

set( UNICODE_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	UNICODE_SOURCE_FILES

	# Source Files
	"${UNICODE_DIR}/unicode.cpp"
)

add_library(
	unicode SHARED
	${UNICODE_SOURCE_FILES}
)

set_property(
	TARGET unicode PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	unicode
	""
	"unicode"
	"${GAMEDIR}/bin"
)

target_compile_definitions(
	unicode PRIVATE

	UNICODE_EXPORTS
)

target_link_libraries(
	unicode PRIVATE

	# SanyaSho: unicows is unused
	#$<${IS_WINDOWS}:unicows>

	$<${IS_OSX}:iconv>
	$<${IS_OSX}:z>

	$<${IS_LINUX}:rt>
)
