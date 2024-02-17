cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

set( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE )

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

if( UNIX )
	set( CMAKE_SKIP_BUILD_RPATH FALSE )
	set( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE )
	if( APPLE )
		set( CMAKE_MACOSX_RPATH ON )
		set( CMAKE_INSTALL_RPATH "@loader_path" )
	else()
		set( CMAKE_INSTALL_RPATH "$\{ORIGIN\}" )
	endif()
endif()

if( APPLE )
	set( CMAKE_OSX_ARCHITECTURES x86_64 )
	set( CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym" )
endif()

project( TF2Classic CXX C )

if( WIN32 )
	enable_language(
		ASM_MASM
	)
endif()

# For some reason, checking if CMAKE_BUILD_TYPE is defined is unreliable
# So simply check if it's empty instead
if( "${CMAKE_BUILD_TYPE}" STREQUAL "" )
	set( CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE )
endif()

message( STATUS "Build type: ${CMAKE_BUILD_TYPE}" )

if( NOT ${CMAKE_BUILD_TYPE} MATCHES "Release" AND NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug" )
	message( FATAL_ERROR "Unknown build type!" )
endif()

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_VISIBILITY_PRESET hidden )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

# This is a way to emulate groups.vgc
set( BUILD_GROUP "game" CACHE STRING "Build Group" )

# For the CMake GUIs that support a combobox list
set_property( CACHE BUILD_GROUP PROPERTY STRINGS
	"dedicated"		# server-only
	"game"			# client-only
)

# SanyaSho: Add stubs here for template source groups
set(
	CMAKE_DLL_SOURCE_FILES
	"FIXME__Hey_Some_Of_Your_Projects_Is_Using_CMAKE_DLL_SOURCE_FILES__FIXME"
)

set(
	CMAKE_EXE_SOURCE_FILES
	"FIXME__Hey_Some_Of_Your_Projects_Is_Using_CMAKE_EXE_SOURCE_FILES__FIXME"
)

set(
	CMAKE_LIB_SOURCE_FILES
	"FIXME__Hey_Some_Of_Your_Projects_Is_Using_CMAKE_LIB_SOURCE_FILES__FIXME"
)

# The game build whe are putting the binaries
set( BUILD_FOLDER "tf2classic" CACHE STRING "Build Game" )

set( SRCDIR "${CMAKE_CURRENT_LIST_DIR}" )
set( GAMEDIR "${CMAKE_CURRENT_LIST_DIR}/game_tf2classic" CACHE STRING "Build Folder" )
set( THIRDPARTYDIR "${SRCDIR}/thirdparty" )
set( DX9SDKDIR "${SRCDIR}/dx9sdk/" )

# Compile options that are populated and set for each target depending on their type
set( ADDITIONAL_COMPILE_OPTIONS_EXE )
set( ADDITIONAL_COMPILE_OPTIONS_DLL )
set( ADDITIONAL_COMPILE_OPTIONS_LIB )

# Libraries that are linked to for each target depending on their type
set( ADDITIONAL_LINK_LIBRARIES_EXE )
set( ADDITIONAL_LINK_LIBRARIES_DLL )

# Linker options that are populated and set for each target depending on their type
set( ADDITIONAL_LINK_OPTIONS_EXE )
set( ADDITIONAL_LINK_OPTIONS_DLL )
set( ADDITIONAL_LINK_OPTIONS_LIB )

# Sources that are added to each target depending on their type
set( ADDITIONAL_SOURCES_EXE )
set( ADDITIONAL_SOURCES_DLL )
set( ADDITIONAL_SOURCES_LIB )

# Compile definitions that are added to each target depending on their type
set( ADDITIONAL_COMPILE_DEFINITIONS_EXE )
set( ADDITIONAL_COMPILE_DEFINITIONS_DLL )
set( ADDITIONAL_COMPILE_DEFINITIONS_LIB )

set( ADDITIONAL_INCLUDES_EXE )
set( ADDITIONAL_INCLUDES_DLL )
set( ADDITIONAL_INCLUDES_LIB )

# SanyaSho: moved from pch_skip.cmake
set_source_files_properties(
	"${SRCDIR}/public/tier0/memoverride.cpp"
	PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

include( "cmake/base.cmake" )
include( "cmake/platform_dirs.cmake" )
include( "cmake/video_base.cmake" )
include( "cmake/postbuild.cmake" )

# SanyaSho: include cryptlib
include( "cmake/misc/source_cryptlib_include.cmake" )

include( "cmake/misc/source_use_steamapi.cmake" )
include( "cmake/misc/source_use_sdl2.cmake" )

#set( LIBPUBLIC "${SRCDIR}/lib/public/${PLATFORM_SUBDIR}" )
#set( LIBCOMMON "${SRCDIR}/lib/common/${PLATFORM_SUBDIR}" )

if( WIN32 )
set( _INTERNAL_PLATFORM "win32" )
elseif( ${IS_ANDROID} )
set( _INTERNAL_PLATFORM "android" )
else()
set( _INTERNAL_PLATFORM "linux" )
endif()

if( ${IS_64BIT} )
if( ${IS_ANDROID} )
set( _INTERNAL_ARCH "aarch64" )
else()
set( _INTERNAL_ARCH "amd64" )
endif()
else()
if( ${IS_ANDROID} )
set( _INTERNAL_ARCH "arm" )
else()
set( _INTERNAL_ARCH "x86" )
endif()
endif()

set( LIBPUBLIC "${SRCDIR}/lib/${_INTERNAL_PLATFORM}/${_INTERNAL_ARCH}" )
set( LIBCOMMON ${LIBPUBLIC} )

include( "cmake/misc/install_lib.cmake" )

link_directories(
	${LIBPUBLIC}
	${LIBCOMMON}
)

add_compile_definitions( $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Debug>:_DEBUG> )
add_compile_definitions( $<$<CONFIG:Release>:NDEBUG> )

if( ${IS_WINDOWS} )
	include( "cmake/windows_base.cmake" )
elseif( ${IS_LINUX} OR ${IS_OSX} )
	include( "cmake/posix_base.cmake" )
endif()

include( "cmake/groups.cmake" )

# Store all targets in a variable name ( See: https://stackoverflow.com/questions/37434946/how-do-i-iterate-over-all-cmake-targets-programmatically/62311397#62311397 )
function( get_all_targets var )
    set( targets )
    get_all_targets_recursive( targets ${CMAKE_CURRENT_SOURCE_DIR} )
    set( ${var} ${targets} PARENT_SCOPE )
endfunction()

macro( get_all_targets_recursive targets dir )
    get_property( subdirectories DIRECTORY ${dir} PROPERTY SUBDIRECTORIES )

    foreach( subdir ${subdirectories} )
        get_all_targets_recursive( ${targets} ${subdir} )
    endforeach()

    get_property( current_targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS )
    list( APPEND ${targets} ${current_targets} )
endmacro()

get_all_targets( ALL_TARGETS )

# Set of helper functions to add defintions/options/libs for each target in a filtered way
function( add_compile_definitions_filtered target definitions )
	foreach( additional_definition IN LISTS ${definitions} )
		set( SHOULD_EXCLUDE 0 )

		# Exclude the compile definition if target defines an exclude list
		foreach( exclude IN LISTS "${target}_exclude_compile_definitions" )
			if( ${additional_definition} STREQUAL ${exclude} )
				set( SHOULD_EXCLUDE 1 )
				break()
			endif()
		endforeach()

		if( NOT ${SHOULD_EXCLUDE} )
			target_compile_definitions( ${target} PRIVATE ${additional_definition} )
		endif()
	endforeach()
endfunction()


function( add_compile_options_filtered target options )
	foreach( additional_option IN LISTS ${options} )
		set( SHOULD_EXCLUDE 0 )

		# Exclude the compile options if target defines an exclude list
		foreach( exclude IN LISTS "${target}_exclude_compile_options" )
			if( ${additional_option} STREQUAL ${exclude} )
				set( SHOULD_EXCLUDE 1 )
				break()
			endif()
		endforeach()

		if( NOT ${SHOULD_EXCLUDE})
			target_compile_options( ${target} PRIVATE "$<$<COMPILE_LANGUAGE:C,CXX>:${additional_option}>" )
		endif()
	endforeach()
endfunction()

function( add_sources_filtered target sources )
	foreach( additional_source IN LISTS ${sources} )
		set( SHOULD_EXCLUDE 0 )

		# Exclude the source if target defines an exclude list
		foreach( exclude IN LISTS "${target}_exclude_source" )
			if( ${additional_source} STREQUAL ${exclude} )
				set( SHOULD_EXCLUDE 1 )
				break()
			endif()
		endforeach()

		if( NOT ${SHOULD_EXCLUDE} )
			target_sources( ${target} PRIVATE ${additional_source} )
		endif()
	endforeach()
endfunction()

function( add_include_dirs_filtered target includes )
	foreach( additional_include_dir IN LISTS ${includes} )
		set( SHOULD_EXCLUDE 0 )

		# Exclude the source if target defines an exclude list
		foreach( exclude IN LISTS "${target}_exclude_include_dirs" )
			if( ${additional_include_dir} STREQUAL ${exclude} )
				set( SHOULD_EXCLUDE 1 )
				break()
			endif()
		endforeach()

		if( NOT ${SHOULD_EXCLUDE} )
			target_include_directories( ${target} PRIVATE ${additional_include_dir} )
		endif()
	endforeach()
endfunction()

function( add_libraries_filtered target libraries)
	foreach( additional_lib IN LISTS ${libraries} )
		set( SHOULD_EXCLUDE 0 )

		# Exclude the lib if target defines an exclude list
		foreach( exclude IN LISTS "${target}_exclude_lib" )
			if( ${additional_lib} STREQUAL ${exclude} )
				set( SHOULD_EXCLUDE 1 )
				break()
			endif()
		endforeach()

		if( NOT ${SHOULD_EXCLUDE} )
			get_target_property( libraries ${target} LINK_LIBRARIES )
			# Don't bother adding it if the target already links it manually
			foreach( lib IN LISTS libraries )
				if( ${additional_lib} STREQUAL ${lib} )
					set( SHOULD_EXCLUDE 1 )
					break()
				endif()
			endforeach()
		endif()

		if( NOT ${SHOULD_EXCLUDE} )
			target_link_libraries( ${target} PRIVATE ${additional_lib} )
		endif()
	endforeach()
endfunction()

message( "--- Targets for \"${BUILD_GROUP}\":" )

# Iterates over all the targets and add necessary definitions/options/libs
# This is an incredible hack, but it allows for targets to specify exclude lists
# This allows us to emulate -$File and such from VPC
foreach( target ${ALL_TARGETS} )
	get_target_property( target_type ${target} TYPE )
	message( "---- ${target} (${target_type})" )

	# Define an empty exclude list if one isn't defined
	if( NOT DEFINED "${target}_exclude_compile_options" )
		set( "${target}_exclude_compile_options" )
	endif()

	# Define an empty exclude list if one isn't defined
	if( NOT DEFINED "${target}_exclude_lib" )
		set( "${target}_exclude_lib" )
	endif()

	# Define an empty exclude list if one isn't defined
	if( NOT DEFINED "${target}_exclude_source" )
		set( "${target}_exclude_source" )
	endif()

	if( NOT DEFINED "${target}_exclude_include_dirs" )
		set( "${target}_exclude_include_dirs" )
	endif()

	if( ${target_type} STREQUAL "EXECUTABLE" )
		add_compile_options_filtered( ${target} ADDITIONAL_COMPILE_OPTIONS_EXE )
		add_libraries_filtered( ${target} ADDITIONAL_LINK_LIBRARIES_EXE )
		add_sources_filtered( ${target} ADDITIONAL_SOURCES_EXE )
		add_include_dirs_filtered( ${target} ADDITIONAL_INCLUDES_EXE )
		target_link_options( ${target} PRIVATE ${ADDITIONAL_LINK_OPTIONS_EXE} )
		target_compile_definitions( ${target} PRIVATE MEMOVERRIDE_MODULE=$<TARGET_NAME_IF_EXISTS:${target}> )
		add_compile_definitions_filtered( ${target} ADDITIONAL_COMPILE_DEFINITIONS_EXE )

		# Only applies to Linux and OSX
		target_strip_symbols( ${target} )
	elseif( ( ${target_type} STREQUAL "SHARED_LIBRARY" ) OR ( ${target_type} STREQUAL "MODULE_LIBRARY" ) )
		add_compile_options_filtered( ${target} ADDITIONAL_COMPILE_OPTIONS_DLL )
		add_libraries_filtered( ${target} ADDITIONAL_LINK_LIBRARIES_DLL )
		add_sources_filtered( ${target} ADDITIONAL_SOURCES_DLL )
		add_include_dirs_filtered( ${target} ADDITIONAL_INCLUDES_DLL )
		target_link_options( ${target} PRIVATE ${ADDITIONAL_LINK_OPTIONS_DLL} )
		target_compile_definitions( ${target} PRIVATE MEMOVERRIDE_MODULE=$<TARGET_NAME_IF_EXISTS:${target}> DLLNAME=$<TARGET_NAME_IF_EXISTS:${target}> )
		add_compile_definitions_filtered( ${target} ADDITIONAL_COMPILE_DEFINITIONS_DLL )

		# Only applies to Linux and OSX
		target_strip_symbols( ${target} )
	elseif( ${target_type} STREQUAL "STATIC_LIBRARY" )
		add_compile_options_filtered( ${target} ADDITIONAL_COMPILE_OPTIONS_LIB )
		add_sources_filtered( ${target} ADDITIONAL_SOURCES_LIB )
		add_include_dirs_filtered( ${target} ADDITIONAL_INCLUDES_LIB )
		target_link_options( ${target} PRIVATE ${ADDITIONAL_LINK_OPTIONS_LIB} )
		target_compile_definitions( ${target} PRIVATE LIBNAME=$<TARGET_NAME_IF_EXISTS:${target}> )
		add_compile_definitions_filtered( ${target} ADDITIONAL_COMPILE_DEFINITIONS_LIB )
	endif()
endforeach()
