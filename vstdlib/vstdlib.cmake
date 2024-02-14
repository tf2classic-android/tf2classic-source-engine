#=============================================================================
#	vstdlib.cmake
#
#	Project Script
#=============================================================================

set( VSTDLIB_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VSTDLIB_SOURCE_FILES

	#$Folder	"Source Files"
	#{
		"$<${IS_X360}:${VSTDLIB_DIR}/xbox/___FirstModule.cpp>"	#[$X360]
		"$<$<AND:${IS_WINDOWS},${IS_64BIT}>:${VSTDLIB_DIR}/GetStackPtr64.asm>" #[$WIN64]
		#{
		#    $Configuration
		#    {
		#        $CustomBuildStep
		#        {
		#            // General
		#            $CommandLine                "$QUOTE$(VCInstallDir)bin/x86_amd64/ml64.exe$QUOTE /nologo /c /Fo$QUOTE$(IntDir)/$(InputName).obj$QUOTE $QUOTE$(InputPath)$QUOTE"
		#            $Description                "Compiling GetStackPtr64.masm"
		#            $Outputs                    "$(IntDir)/$(InputName).obj"
		#        }
		#    }
		#}
		"$<$<AND:${IS_WINDOWS},${IS_64BIT}>:${VSTDLIB_DIR}/coroutine_win64.asm>" #[$WIN64]
		#{
		#	$Configuration
		#	{
		#		$CustomBuildStep
		#		{
		#			// General
		#			$CommandLine                "$QUOTE$(VCInstallDir)bin/x86_amd64/ml64.exe$QUOTE /c /Fo$QUOTE$(IntDir)/$(InputName).obj$QUOTE $QUOTE$(InputPath)$QUOTE"
		#			$Description                "Compiling coroutine_win64.masm"
		#			$Outputs                    "$(IntDir)/$(InputName).obj"
		#		}
		#	}
		#}

		"$<$<AND:$<NOT:${IS_X360}>,$<NOT:${IS_OSX}>>:${VSTDLIB_DIR}/coroutine.cpp>"				#[!$X360 && !$OSXALL]
		#{
		#	$Configuration
		#	{
		#		$Compiler
		#		{
		#			$BasicRuntimeChecks			"Default"
		#		}
		#	}
		#}
		"${VSTDLIB_DIR}/cvar.cpp"
		"${VSTDLIB_DIR}/jobthread.cpp"
		"${VSTDLIB_DIR}/KeyValuesSystem.cpp"
		"$<${IS_WINDOWS}:${VSTDLIB_DIR}/processutils.cpp>"			#[$WINDOWS]
		"${VSTDLIB_DIR}/random.cpp"
		"${VSTDLIB_DIR}/vcover.cpp"
	#}

	#$Folder	"Public Header Files"
	#{
		"${SRCDIR}/public/vstdlib/cvar.h"
		"${SRCDIR}/public/vstdlib/coroutine.h"
		"${SRCDIR}/public/vstdlib/jobthread.h"
		"${SRCDIR}/public/vstdlib/IKeyValuesSystem.h"
		"${SRCDIR}/public/vstdlib/iprocessutils.h"
		"${SRCDIR}/public/tier1/mempool.h"
		"${SRCDIR}/public/vstdlib/random.h"
		"${SRCDIR}/public/vstdlib/vcover.h"
		"${SRCDIR}/public/vstdlib/vstdlib.h"
	#}
)

set(
	"vstdlib_exclude_lib"
	vstdlib
)

add_library(
	vstdlib SHARED
	${VSTDLIB_SOURCE_FILES}
)

set_property(
	TARGET vstdlib PROPERTY FOLDER "Shared Libs"
)

if( ${IS_POSIX} )
	set( _LIB_PREFIX "lib" )
else()
	set( _LIB_PREFIX "" )
endif()

set_install_properties(
	vstdlib
	"${_LIB_PREFIX}"
	"vstdlib"
	"${GAMEDIR}/bin"
)

target_compile_definitions(
	vstdlib PRIVATE
	VSTDLIB_DLL_EXPORT
)

target_link_libraries(
	vstdlib PRIVATE
	$<${IS_OSX}:coroutine_osx>
)

target_link_libraries(
	vstdlib PRIVATE

	"$<${IS_WINDOWS}:odbc32>"
	"$<${IS_WINDOWS}:odbccp32>"

	"$<${IS_OSX}:-framework CoreServices>"
	"$<${IS_OSX}:coroutine_osx>"
)

if( ${IS_OSX} )
target_link_options(
	vstdlib PRIVATE

	-all_load
)
endif()
