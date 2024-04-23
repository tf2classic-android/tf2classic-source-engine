#=============================================================================
# posix_base.cmake
#
# $
#=============================================================================

if( ${IS_LINUX} AND NOT ${IS_ANDROID} )
	# Check for system cURL
	find_package( CURL REQUIRED )
endif()

string( REPLACE "-O3" "-O2" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" )
string( REPLACE "-O3" "-O2" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" )

if( ${IS_OSX} )
	set( CMAKE_THREAD_LIBS_INIT "-lpthread" )
	set( CMAKE_HAVE_THREADS_LIBRARY 1 )
	set( CMAKE_USE_WIN32_THREADS_INIT 0 )
	set( CMAKE_USE_PTHREADS_INIT 1 )
	set( THREADS_PREFER_PTHREAD_FLAG ON )
	find_package( Threads REQUIRED )
else()
	find_package( Threads REQUIRED )
endif()

add_compile_options(
	-g
	$<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
	-fdiagnostics-color
	$<$<NOT:$<BOOL:${DISABLE_WARNINGS}>>:-Wno-narrowing>
	$<$<BOOL:${DISABLE_WARNINGS}>:-w>
	$<${IS_LINUX}:-U_FORTIFY_SOURCE>
	-Usprintf
	-Ustrncpy
	-UPROTECTED_THINGS_ENABLE
	$<${IS_CLANG}:-Wno-inconsistent-missing-override>
	$<$<AND:${IS_GCC},$<COMPILE_LANGUAGE:CXX>,$<NOT:$<BOOL:${DISABLE_WARNINGS}>>>:-Wno-invalid-offsetof> # Mr0maks: suppress invalid offsetof because of non standart classes. (All project work on it).
	$<$<AND:${IS_GCC},$<COMPILE_LANGUAGE:CXX>,$<NOT:$<BOOL:${DISABLE_WARNINGS}>>>:-Wno-ignored-attributes> # Mr0maks: suppress ignored attributes because of useing int128 in templates.
	#$<$<AND:${IS_LINUX},$<COMPILE_LANGUAGE:CXX>>:-fabi-compat-version=2> # FIXME(SanyaSho): I think we've don't need it anymore, because we're using GCC 10 from Sniper Steam Runtime

	# Bald flags
	-fno-strict-aliasing # vphysics error *fix*

	"$<$<CONFIG:Release>:-funsafe-math-optimizations>"
	"$<$<CONFIG:Release>:-ftree-vectorize>"
	"$<$<CONFIG:Release>:-ffast-math>"
	"$<$<CONFIG:Release>:-Wno-narrowing>"

	"$<$<CONFIG:Debug>:-Wno-narrowing>"

	"$<${IS_ANDROID}:-Wno-format-security>"
)

if( ${IS_ANDROID} )

add_compile_definitions(
	"ANDROID"
	"_ANDROID"

	HAVE_EFFICIENT_UNALIGNED_ACCESS
	VECTORIZE_SINCOS
	NO_MEMOVERRIDE_NEW_DELETE
)

link_libraries(
	log
	z
)

add_link_options(
	-static-libstdc++

	$<${IS_ANDROID32}:-march=armv7-a>
	$<${IS_ANDROID64}:-march=armv8-a>

	-mfpu=neon-vfpv4
	#-mcpu=cortex-a7
	-mtune=cortex-a53
	-fsigned-char
	-funwind-tables
)

include_directories(
	"${THIRDPARTYDIR}"
	"${THIRDPARTYDIR}/curl/include"
	"${THIRDPARTYDIR}/SDL"
	"${THIRDPARTYDIR}/openal-soft/include"
	"${THIRDPARTYDIR}/fontconfig"
	"${THIRDPARTYDIR}/freetype/include"
)

add_compile_options(
	$<${IS_ANDROID32}:-march=armv7-a>
	$<${IS_ANDROID64}:-march=armv8-a>

	-mfpu=neon-vfpv4
	#-mcpu=cortex-a7
	-mtune=cortex-a53
	-fsigned-char
	-funwind-tables
)

endif()

if( NOT (${IS_ANDROID} OR ${IS_ARM64}) )

if( ${IS_64BIT} )
	add_compile_options( -m64 )
	add_link_options( -m64 )
else()
	add_compile_options( -m32 )
	add_link_options( -m32 )
endif()

endif()

add_compile_definitions(
	_POSIX
	POSIX
	GNUC
	COMPILER_GCC
	NO_HOOK_MALLOC
	NO_MALLOC_OVERRIDE
	$<${IS_LINUX}:_LINUX>
	$<${IS_LINUX}:LINUX>
	$<${IS_OSX}:_OSX>
	$<${IS_OSX}:OSX>
	$<${IS_OSX}:_DARWIN_UNLIMITED_SELECT>
	$<${IS_OSX}:FD_SETSIZE=10240>
	$<${IS_TOGLES}:TOGLES>
)

if( ${IS_LINUX} )
	# Helps us catch any linker errors from out of order linking or in general
	list(
		APPEND ADDITIONAL_LINK_OPTIONS_DLL
		-Wl,--no-undefined
	)
endif()

link_libraries(
	Threads::Threads
	${CMAKE_DL_LIBS}
	$<$<AND:${IS_LINUX},$<NOT:${IS_ANDROID}>>:m>
)

if( NOT ${IS_ANDROID} )

if( ${IS_LINUX} )
	add_link_options(
		-static-libgcc
		-static-libstdc++
	)
endif()

add_compile_options(
	$<$<AND:${IS_LINUX},$<NOT:${IS_ARM64}>>:-march=x86-64-v2>
	$<$<NOT:${IS_ARM64}>:-mfpmath=sse>
	-mtune=generic

	$<${IS_ARM64}:-march=armv8.2-a>
)

add_link_options(
	$<${IS_ARM64}:-march=armv8.2-a>
)
endif()

list(
	APPEND ADDITIONAL_LINK_LIBRARIES_DLL
	tier0
	tier1
	vstdlib
)

if( ${IS_OSX} )
	set( CMAKE_SHARED_MODULE_SUFFIX ".dylib" )
endif()
