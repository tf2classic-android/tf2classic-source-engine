#=============================================================================
#	launcher_main.cmake
#
#	Project Script
#=============================================================================

# CMAKETODO:
#	$Linker [$WIN32]
#	{
#		$EnableLargeAddresses		"Support Addresses Larger Than 2 Gigabytes (/LARGEADDRESSAWARE)" [$WIN32]
#		$FixedBaseAddress			"Generate a relocation section (/FIXED:NO)" [$WIN32]
#	}

# CMAKETODO:
#	$Folder	"Link Libraries" 
#	{
#		-$Lib tier0 [$WINDOWS]
#		-$Lib tier1 [$WINDOWS]
#		-$ImpLib vstdlib [$WINDOWS]
#	}

set( LAUNCHER_MAIN_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	LAUNCHER_MAIN_SOURCE_FILES

	# Source Files
	"${LAUNCHER_MAIN_DIR}/main.cpp"

	# Resources [$WIN32]
	"$<${IS_WINDOWS}:${LAUNCHER_MAIN_DIR}/launcher_main.rc>"
	"$<${IS_WINDOWS}:${SRCDIR}/launcher/res/launcher.ico>"
)

set(
	"launcher_main_exclude_source"
	"${SRCDIR}/public/tier0/memoverride.cpp"
)

set(
	"launcher_main_exclude_lib"
	tier0
	tier1
	vstdlib
)

add_executable(
	launcher_main WIN32
	${LAUNCHER_MAIN_SOURCE_FILES}
)

set_property(
	TARGET launcher_main PROPERTY FOLDER "Executables"
)

set_install_properties(
	launcher_main
	""
	"hl2${EXECUTABLE_SUBNAME}"
	"${GAMEDIR}"
)

target_compile_definitions(
	launcher_main PRIVATE

	fopen=dont_use_fopen
)

target_link_libraries(
	launcher_main

	"$<${IS_WINDOWS}:comctl32>"
)
