#=============================================================================
#	tier0.cmake
#
#	Project Script
#=============================================================================

set( TIER0_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	TIER0_SOURCE_FILES

	#$Folder	"Source Files"
	#{
		"${TIER0_DIR}/assert_dialog.cpp"
		"$<${IS_WINDOWS}:${TIER0_DIR}/assert_dialog.rc>"		#[$WINDOWS]
		"${TIER0_DIR}/commandline.cpp"
		"${TIER0_DIR}/cpu.cpp"
		"${TIER0_DIR}/cpumonitoring.cpp"
		"$<${IS_POSIX}:${TIER0_DIR}/cpu_posix.cpp>"			#[$POSIX]
		"${TIER0_DIR}/cpu_usage.cpp"
		"${TIER0_DIR}/dbg.cpp"
		"${TIER0_DIR}/dynfunction.cpp"
		"$<${IS_WINDOWS}:${TIER0_DIR}/etwprof.cpp>"			#[$WINDOWS]
		"${TIER0_DIR}/fasttimer.cpp"
		"$<$<AND:${IS_WINDOWS},${IS_64BIT}>:${TIER0_DIR}/InterlockedCompareExchange128.masm>" #[$WIN64]
		#{
		#	$Configuration
		#	{
		#		$CustomBuildStep
		#		{
		#			// General
		#			$CommandLine                "$QUOTE$(VCInstallDir)bin/x86_amd64/ml64.exe$QUOTE /nologo /c /Fo$QUOTE$(IntDir)/$(InputName).obj$QUOTE $QUOTE$(InputPath)$QUOTE"
		#			$Description                "Compiling $(InputName).masm"
		#			$Outputs                    "$(IntDir)/$(InputName).obj"
		#		}
		#	}
		#}
		"${TIER0_DIR}/mem.cpp"
		"${TIER0_DIR}/mem_helpers.cpp"
		"${TIER0_DIR}/memdbg.cpp"
		"${TIER0_DIR}/memstd.cpp"
		"${TIER0_DIR}/memvalidate.cpp"
		"${TIER0_DIR}/minidump.cpp"
		#"${TIER0_DIR}/pch_tier0.cpp"
		#{
		#	$Configuration
		#	{
		#		$Compiler
		#		{
		#			$Create/UsePrecompiledHeader	"Create Precompiled Header (/Yc)"
		#		}
		#	}
		#}
		"$<$<OR:${IS_WINDOWS},${IS_X360}>:${TIER0_DIR}/platform.cpp>"		#[$WINDOWS||$X360]
		"$<${IS_POSIX}:${TIER0_DIR}/platform_posix.cpp>"		#[$POSIX]
		"$<${IS_X360}:${TIER0_DIR}/pmc360.cpp>"		#[$X360]
		"$<${IS_WINDOWS}:${TIER0_DIR}/pme.cpp>"			#[$WINDOWS]
		"$<${IS_POSIX}:${TIER0_DIR}/pme_posix.cpp>"		#[$POSIX]
		"$<$<OR:${IS_WINDOWS},${IS_POSIX}>:${TIER0_DIR}/PMELib.cpp>"		#[$WINDOWS||$POSIX]
		#{
		#	$Configuration
		#	{
		#		$Compiler
		#		{
		#			$Create/UsePrecompiledHeader	"Not Using Precompiled Headers"
		#		}
		#	}
		#}
		"${TIER0_DIR}/progressbar.cpp"
		"${TIER0_DIR}/systeminformation.cpp"
		"${TIER0_DIR}/stacktools.cpp"
		"$<$<OR:${IS_WINDOWS},${IS_POSIX}>:${TIER0_DIR}/thread.cpp>"		#[$WINDOWS||$POSIX]
		"${TIER0_DIR}/threadtools.cpp"
		"${TIER0_DIR}/tier0_strtools.cpp"
		"${TIER0_DIR}/tslist.cpp"
		"${TIER0_DIR}/vprof.cpp"
		"$<${IS_WINDOWS}:${TIER0_DIR}/vcrmode.cpp>"
		"$<${IS_POSIX}:${TIER0_DIR}/vcrmode_posix.cpp>"
		"$<${IS_WINDOWS}:${TIER0_DIR}/security.cpp>"
		"$<${IS_POSIX}:${TIER0_DIR}/security_linux.cpp>"
		"$<${IS_WINDOWS}:${TIER0_DIR}/win32consoleio.cpp>"	#[$WINDOWS]
		#"$<${IS_LINUX}:${SRCDIR}/tier1/pathmatch.cpp>" #[$LINUXALL] # SanyaSho: we dont need it anymore
	#}

	#$folder	"Header Files"
	#{
		"${SRCDIR}/public/tier0/basetypes.h"
		"${SRCDIR}/public/tier0/commonmacros.h"
		"${SRCDIR}/public/tier0/cpumonitoring.h"
		"${SRCDIR}/public/tier0/dbg.h"
		"${SRCDIR}/public/tier0/dbgflag.h"
		"${SRCDIR}/public/tier0/EventMasks.h"
		"${SRCDIR}/public/tier0/EventModes.h"
		"${SRCDIR}/public/tier0/etwprof.h"
		"${SRCDIR}/public/tier0/fasttimer.h"
		"${SRCDIR}/public/tier0/ia32detect.h"
		"${SRCDIR}/public/tier0/icommandline.h"
		"${SRCDIR}/public/tier0/IOCTLCodes.h"
		"${SRCDIR}/public/tier0/l2cache.h"
		"$<${IS_X360}:${SRCDIR}/public/tier0/pmc360.h>"		#[$X360]
		"${SRCDIR}/public/tier0/mem.h"
		"${SRCDIR}/public/tier0/memalloc.h"
		"${SRCDIR}/public/tier0/memdbgoff.h"
		"${SRCDIR}/public/tier0/memdbgon.h"
		"${SRCDIR}/public/tier0/minidump.h"
		"${SRCDIR}/public/tier0/P4PerformanceCounters.h"
		"${SRCDIR}/public/tier0/P5P6PerformanceCounters.h"
		"${TIER0_DIR}/pch_tier0.h"
		"${SRCDIR}/public/tier0/platform.h"
		"${SRCDIR}/public/tier0/PMELib.h"
		"${SRCDIR}/public/tier0/progressbar.h"
		"${SRCDIR}/public/tier0/protected_things.h"
		"${TIER0_DIR}/resource.h"
		"${SRCDIR}/public/tier0/systeminformation.h"
		"${SRCDIR}/public/tier0/threadtools.h"
		"${SRCDIR}/public/tier0/tslist.h"
		"${SRCDIR}/public/tier0/validator.h"
		"${SRCDIR}/public/tier0/valobject.h"
		"${SRCDIR}/public/tier0/vprof.h"
		"${SRCDIR}/public/tier0/wchartypes.h"
		"${TIER0_DIR}/mem_helpers.h"
	#}

	#$Folder	"DESKey" [$WINDOWS]
	#{
		"$<${IS_WINDOWS}:${TIER0_DIR}/DESKey/ALGO.H>"
		"$<${IS_WINDOWS}:${TIER0_DIR}/DESKey/ALGO32.LIB>"
		"$<${IS_WINDOWS}:${TIER0_DIR}/DESKey/DK2WIN32.H>"
		"$<${IS_WINDOWS}:${TIER0_DIR}/DESKey/DK2WIN32.LIB>"
	#}

	#$Folder "Xbox" [$X360]
	#{
		#$folder "Source Files"
		#{
 			"$<${IS_X360}:${TIER0_DIR}/xbox/xbox_console.cpp>"
			"$<${IS_X360}:${TIER0_DIR}/xbox/xbox_system.cpp>"
			"$<${IS_X360}:${TIER0_DIR}/xbox/xbox_win32stubs.cpp>"
		#}
		#$folder "Header Files"
		#{
			"$<${IS_X360}:${SRCDIR}/common/xbox/xbox_console.h>"
			"$<${IS_X360}:${SRCDIR}/common/xbox/xbox_core.h>"
			"$<${IS_X360}:${SRCDIR}/common/xbox/xbox_win32stubs.h>"
		#}
	#}
)

set_source_files_properties(
	"$<$<OR:${IS_WINDOWS},${IS_POSIX}>:${TIER0_DIR}/PMELib.cpp>"
	PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

if( ${IS_WINDOWS} OR ${IS_X360} )
set(
	"tier0_exclude_source"
	"${SRCDIR}/public/tier0/memoverride.cpp"
)
endif()

set(
	"tier0_exclude_lib"
	tier0
	tier1
	vstdlib
)

add_library(
	tier0 SHARED
	${TIER0_SOURCE_FILES}
)

set_property(
	TARGET tier0 PROPERTY FOLDER "Shared Libs"
)

if( ${IS_POSIX} )
	set( _LIB_PREFIX "lib" )
else()
	set( _LIB_PREFIX "" )
endif()

set_install_properties(
	tier0
	"${_LIB_PREFIX}"
	"tier0"
	"${GAMEDIR}/bin"
)

target_precompile_headers(
	tier0
	PRIVATE "${TIER0_DIR}/pch_tier0.h"
)

target_include_directories(
	tier0 PRIVATE

	"${SRCDIR}/tier0"
	"${SRCDIR}/public"
	"${SRCDIR}/public/tier0"
	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
)

target_compile_definitions(
	tier0 PRIVATE

	TIER0_DLL_EXPORT
)

if( ${IS_CLANGCL} )
target_compile_options(
	tier0 PRIVATE

	/EHsc
)
endif()

target_link_libraries(
	tier0 PRIVATE

	"$<${IS_WINDOWS}:ws2_32>"

	"$<$<AND:${IS_LINUX},$<NOT:${IS_ANDROID}>>:rt>"
)

if( ${USE_SDL} )
	target_use_sdl2(
		tier0
	)
endif()
