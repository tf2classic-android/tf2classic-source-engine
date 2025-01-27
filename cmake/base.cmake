#=============================================================================
# base.cmake
#
# $
#=============================================================================

# NOTE: We use 0 or 1 so we can use these more easily in generator expressions
# Initialize them with default values that we then set later

# SanyaSho: ADD YOUR GAME HERE!
set( IS_GAME_TF2CLASSIC	0 )

set( IS_64BIT		0 )
set( IS_ARM64		0 )
set( IS_DEDICATED	0 )
set( IS_TOGLES		0 )
set( STEAM_AVATARS	0 )

# Platforms
set( IS_WINDOWS		0 )
set( IS_POSIX		0 )
set( IS_LINUX		0 )
set( IS_OSX		0 )
set( IS_X360		0 ) # SanyaSho: never
set( IS_XCODE 		0 )
set( IS_ANDROID		0 )
set( IS_ANDROID32	0 )
set( IS_ANDROID64	0 )

if( WIN32 )
	set( IS_WINDOWS	1 )
endif()

if( UNIX )
	set( IS_POSIX 1 )

	if( ${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64" )
		set( IS_ARM64 1 )
	endif()

	if( NOT APPLE )
		set( IS_LINUX 1 )

		if( ${CMAKE_SYSTEM_NAME} STREQUAL "Android" )
			set( IS_ANDROID 1 )

			if( ${CMAKE_ANDROID_ARCH_ABI} STREQUAL "arm64-v8a" )
				set( IS_ANDROID64 1 )
			elseif( ${CMAKE_ANDROID_ARCH_ABI} STREQUAL "armeabi-v7a" )
				set( IS_ANDROID32 1 )
			else()
				message( FATAL_ERROR "Unsupported android arch abi" )
			endif()
		endif()
	elseif( APPLE )
		set( IS_OSX 1 )
		if( ${CMAKE_GENERATOR} STREQUAL "Xcode" )
			set( IS_XCODE 1 )
		endif()
	else()
		message( FATAL_ERROR "Failed to determine UNIX platform!" )
	endif()
endif()

# Compilers
set( IS_CLANG		0 )
set( IS_CLANGCL		0 )
set( IS_APPLECLANG	0 )
set( IS_GCC		0 )
set( IS_INTELC		0 )
set( IS_MSVC		0 )

if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
	# using Clang
	set( IS_CLANG 1 )
	if( MSVC )
		# using ClangCL
		set( IS_CLANGCL 1 )
	endif()
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" )
	# using Apple Clang
	set( IS_CLANG 1 )
	set( IS_APPLECLANG 1 )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
	# using GCC
	set( IS_GCC 1 )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "Intel" )
	# using Intel C++
	set( IS_INTELC 1 )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
	# using Visual Studio C++
	set( IS_MSVC 1 )
endif()

if( ${IS_WINDOWS} )
	set( _DLL_EXT ".dll" )
	set( STATIC_LIB_EXT ".lib" )
	set( IMPLIB_EXT ".lib" )
elseif( ${IS_LINUX} )
	set( _DLL_EXT ".so" )
	set( STATIC_LIB_EXT ".a" )
	set( IMPLIB_EXT ".so" )
elseif( ${IS_OSX} )
	set( _DLL_EXT ".dylib" )
	set( STATIC_LIB_EXT ".a" )
	set( IMPLIB_EXT ".dylib" )
endif()

message( "=============================================================================" )

# Set this to ON to supress all warnings
option( DISABLE_WARNINGS "Disable warnings" OFF )
message( STATUS "DISABLE_WARNINGS: ${DISABLE_WARNINGS}" )

# Stock source engine options
option( RETAIL "Build in retail mode" OFF )
message( STATUS "RETAIL: ${RETAIL}" )

option( STAGING_ONLY "Staging only" OFF )
message( STATUS "STAGING_ONLY: ${STAGING_ONLY}" )

option( DEDICATED "Build dedicated" OFF )
message( STATUS "DEDICATED: ${DEDICATED}" )
if( DEDICATED )
	set( IS_DEDICATED 1 )
	set( BUILD_GROUP "dedicated" ) # HACK(SanyaSho): I need to leave it here because you know
endif()

option( RELEASEASSERTS "Build engine with enabled asserts in release" OFF )
message( STATUS "RELEASEASSERTS: ${RELEASEASSERTS}" )

option( BUILD_VPHYSICS "Build engine with custom VPhysics. Use VPHYSICS_TYPE if you need to change physics engine" OFF )
message( STATUS "BUILD_VPHYSICS: ${BUILD_VPHYSICS}" )

set( BUILD_VPHYSICS_TYPE "IVP" CACHE STRING "Select VPhysics type. Available: \"IVP\", \"JOLT\"" )

set_property( CACHE BUILD_VPHYSICS_TYPE PROPERTY STRINGS
	"IVP"
	"JOLT"
)

if( BUILD_VPHYSICS )
	message( "--- Selected VPhysics: ${BUILD_VPHYSICS_TYPE}" )
endif()

if( ${IS_WINDOWS} AND BUILD_VPHYSICS AND ${BUILD_VPHYSICS_TYPE} STREQUAL "JOLT" ) # SanyaSho: don't need this for IVP and POSIX
	set( VPHYSICS_JOLT_TOOLSET_OVERRIDE "v142" CACHE STRING "Override vphysics_jolt VS toolset" )

	set_property( CACHE VPHYSICS_JOLT_TOOLSET_OVERRIDE PROPERTY STRINGS
		"v141"	# VS 2017 (unsupported)
		"v142"	# VS 2019
		"v143"	# VS 2022
	)

	message( "--- Jolt VS toolset: " ${VPHYSICS_JOLT_TOOLSET_OVERRIDE} )
endif()

option( BUILD_64BIT "64 bit build of engine" OFF )
if( BUILD_64BIT )
	if( IS_ARM64 )
		message( STATUS "Build arch: aarch64/arm64" )
	else()
		message( STATUS "Build arch: x86_64" )
	endif()

	set( IS_64BIT 1 )

	add_compile_definitions(
		PLATFORM_64BITS
	)
else()
	message( STATUS "Build arch: x86" )
endif()

option( ENABLE_TOGLES "Enable ToGLES" OFF )
message( STATUS "ENABLE_TOGLES: ${ENABLE_TOGLES}" )
if( ENABLE_TOGLES )
	set( IS_TOGLES 1 )
endif()

option( BUILD_FOR_PUBLIC_USE "Disables some funny stuff that we have in game/" OFF )
message( STATUS "BUILD_FOR_PUBLIC_USE: ${BUILD_FOR_PUBLIC_USE}" )
if( BUILD_FOR_PUBLIC_USE )
	add_compile_definitions(
		PUBLIC_BUILD
	)
endif()

option( ENABLE_STEAM_AVATARS "Enables old avatar behavior" OFF )
message( STATUS "ENABLE_STEAM_AVATARS: ${ENABLE_STEAM_AVATARS}" )
if( ENABLE_STEAM_AVATARS )
	set( STEAM_AVATARS 1 )

	add_compile_definitions(
		ENABLE_STEAM_AVATARS
	)
endif()

message( "=============================================================================" )

# CMAKETODO(SanyaSho): windows support
if( ${IS_POSIX} )
	find_program( CCACHE ccache )

	if( NOT CCACHE MATCHES "NOTFOUND" )
		# https://github.com/llvm/llvm-project/blob/main/llvm/CMakeLists.txt#L239-L267
		set( CCACHE_COMMAND "CCACHE_DIR=\"${SRCDIR}/../.ccache\" ${CCACHE}" )

		set_property(
			GLOBAL
			PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_COMMAND}"
		)

		# CMAKETODO(SanyaSho): 'Error running link command: No such file or directory'
		#set_property(
		#	GLOBAL
		#	PROPERTY RULE_LAUNCH_LINK "${CCACHE_COMMAND}"
		#)
	else()
		message( STATUS "Could not find ccache executable!" )
	endif()
endif()

list(
	APPEND ADDITIONAL_INCLUDES_EXE
	"${SRCDIR}/common"
	"${SRCDIR}/public"
	"${SRCDIR}/public/tier0"
	"${SRCDIR}/public/tier1"
)

list(
	APPEND ADDITIONAL_INCLUDES_DLL
	"${SRCDIR}/common"
	"${SRCDIR}/public"
	"${SRCDIR}/public/tier0"
	"${SRCDIR}/public/tier1"
)

list(
	APPEND ADDITIONAL_INCLUDES_LIB
	"${SRCDIR}/common"
	"${SRCDIR}/public"
	"${SRCDIR}/public/tier0"
	"${SRCDIR}/public/tier1"
)

# GIT_COMMIT_START
# https://jonathanhamberg.com/post/cmake-embedding-git-hash/
execute_process( COMMAND git describe --dirty --always WORKING_DIRECTORY ${SRCDIR} OUTPUT_VARIABLE GIT_COMMIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE )
string( TIMESTAMP BUILD_START_DATE "%Y-%m-%d" )

configure_file( "${SRCDIR}/common/git_info.h.in" "${SRCDIR}/common/git_info.h" @ONLY )
configure_file( "${SRCDIR}/common/git_info.cpp.in" "${SRCDIR}/common/git_info.cpp" @ONLY )

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "common/git_info.h.in" "common/git_info.cpp.in" )

list(
	APPEND ADDITIONAL_SOURCES_DLL
	"${SRCDIR}/common/git_info.cpp"
)
# GIT_COMMIT_END

add_compile_definitions(
	# Some shared values
	VPROF_LEVEL=4
	SOURCE1=1
	DEV_BUILD

	_DLL_EXT=${_DLL_EXT}
	$<$<BOOL:${RETAIL}>:_RETAIL>
	$<$<BOOL:${STAGING_ONLY}>:STAGING_ONLY>
	$<$<BOOL:${RELEASEASSERTS}>:RELEASEASSERTS>
	RAD_TELEMETRY_DISABLED
	FRAME_POINTER_OMISSION_DISABLED

	WAF_CFLAGS="UNSUPPORTED"
	WAF_LDFLAGS="UNSUPPORTED"
)

add_compile_definitions(
	HAVE_JPEG
	HAVE_PNG
	HAVE_CURL
	HAVE_ZLIB
	$<$<NOT:${IS_ANDROID}>:HAVE_FC>

	$<${IS_WINDOWS}:USE_ZLIB>
)

string( TOUPPER ${BUILD_FOLDER} GAME_DEFINITION )

if( NOT DEFINED IS_GAME_${GAME_DEFINITION} )
	message( FATAL_ERROR "Could not find IS_GAME_${GAME_DEFINITION} predef var." )
endif()

set( IS_GAME_${GAME_DEFINITION} 1 )

add_compile_definitions(
	GAME_${GAME_DEFINITION}
)

message( STATUS "Current game prepocessor definition: \"GAME_${GAME_DEFINITION}\"" )
message( STATUS "Current game CMAKE definiton: \"IS_GAME_${GAME_DEFINITION}\"" )
