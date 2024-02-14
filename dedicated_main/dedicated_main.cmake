#=============================================================================
#	dedicated_main.cmake
#
#	Project Script
#=============================================================================

# CMAKETODO:
#		$EnableLargeAddresses		"Support Addresses Larger Than 2 Gigabytes (/LARGEADDRESSAWARE)"
#		$FixedBaseAddress			"Generate a relocation section (/FIXED:NO)"

set( DEDICATED_MAIN_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	DEDICATED_MAIN_SOURCE_FILES

	#$Folder	"Source Files"
	#{
		"${DEDICATED_MAIN_DIR}/main.cpp"
		"${SRCDIR}/common/SteamAppStartup.cpp"
		"${SRCDIR}/common/SteamAppStartup.h"
	#}

	#$Folder	"Resources"
	#{
		"${SRCDIR}/launcher/res/launcher.ico"
		"${DEDICATED_MAIN_DIR}/dedicated_main.rc"
	#}
)

set(
	"dedicated_main_exclude_source"
	"${SRCDIR}/public/tier0/memoverride.cpp"
)

set(
	"dedicated_main_exclude_lib"
	tier0
	$<${IS_WINDOWS}:tier1>
	$<${IS_WINDOWS}:vstdlib>
)

add_executable(
	dedicated_main WIN32
	${DEDICATED_MAIN_SOURCE_FILES}
)

set_property(
	TARGET dedicated_main PROPERTY FOLDER "Executables"
)

set_install_properties(
	dedicated_main
	""
	"srcds${EXECUTABLE_SUBNAME}"
	"${GAMEDIR}"
)

target_link_libraries(
	dedicated_main PRIVATE

	"$<${IS_WINDOWS}:Advapi32>"
)
