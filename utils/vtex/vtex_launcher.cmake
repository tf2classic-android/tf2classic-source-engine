#=============================================================================
#	vtex_launcher.cmake
#
#	Project Script
#=============================================================================

# CMAKETODO: /LARGEADDRESSAWARE

set( VTEX_LAUNCHER_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VTEX_LAUNCHER_SOURCE_FILES

	#$Folder	"Source Files"
	#{
		"${VTEX_LAUNCHER_DIR}/vtex_launcher.cpp"
	#}

	#$Folder	"Header Files"
	#{
		"${SRCDIR}/public/ilaunchabledll.h"
		"${SRCDIR}/public/tier1/interface.h"
	#}
)

set(
	"vtex_launcher_exclude_source"
	"${SRCDIR}/public/tier0/memoverride.cpp"
)

add_executable(
	vtex_launcher
	${VTEX_LAUNCHER_SOURCE_FILES}
)

set_property(
	TARGET vtex_launcher PROPERTY FOLDER "Utils//VTF Utils//Executables"
)

set_install_properties(
	vtex_launcher
	""
	"vtex${EXECUTABLE_SUBNAME}"
	"${GAMEDIR}/bin"
)

target_include_directories(
	vtex_launcher PRIVATE

	"${SRCDIR}/utils/common"
)

target_compile_definitions(
	vtex_launcher PRIVATE

	PROTECTED_THINGS_DISABLE
)

target_link_libraries(
	vtex_launcher PRIVATE

	"$<${IS_WINDOWS}:odbc32>"
	"$<${IS_WINDOWS}:odbccp32>"

	tier2

	"$<${IS_POSIX}:tier0>"
	"$<${IS_POSIX}:tier1>"
	"$<${IS_POSIX}:vstdlib>"
)
