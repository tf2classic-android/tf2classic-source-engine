#=============================================================================
# posix_base.cmake
#
# $
#=============================================================================

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
)

if( ${IS_64BIT} )
	add_compile_options( -m64 )
	add_link_options( -m64 )
else()
	add_compile_options( -m32 )
	add_link_options( -m32 )
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
	$<${IS_LINUX}:m>
)

if( ${IS_LINUX} )
	add_link_options(
		-static-libgcc
		-static-libstdc++
	)
endif()

add_compile_options(
	$<${IS_LINUX}:-march=core2>
	-msse2
	-mfpmath=sse
	-mtune=core2
)

list(
	APPEND ADDITIONAL_LINK_LIBRARIES_DLL
	tier0
	tier1
	vstdlib
)

if( ${IS_OSX} )
	set( CMAKE_SHARED_MODULE_SUFFIX ".dylib" )
endif()
