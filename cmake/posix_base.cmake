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

	# Bald flags
	-fno-strict-aliasing # vphysics error *fix*

	"$<$<CONFIG:Release>:-funsafe-math-optimizations>"
	"$<$<CONFIG:Release>:-ftree-vectorize>"
	"$<$<CONFIG:Release>:-ffast-math>"
	"$<$<CONFIG:Release>:-Wno-narrowing>"

	"$<$<CONFIG:Debug>:-Wno-narrowing>"
)

if( ${IS_ANDROID} )

if( ${IS_ANDROID32} )
set( CMAKE_C_COMPILER_TARGET "armv7a-linux-androideabi24" )
set( CMAKE_CXX_COMPILER_TARGET "armv7a-linux-androideabi24" )
elseif( ${IS_ANDROID64} )
set( CMAKE_C_COMPILER_TARGET "aarch64-linux-android24" )
set( CMAKE_CXX_COMPILER_TARGET "aarch64-linux-android24" )
else()
message( FATAL_ERROR "Unsupported android arch abi" )
endif()

add_compile_definitions(
	"$<${IS_ANDROID32}:HAVE_EFFICIENT_UNALIGNED_ACCESS>"
	"$<${IS_ANDROID32}:VECTORIZE_SINCOS>"
	"$<${IS_ANDROID32}:_NDK_MATH_NO_SOFTFP=1>"
	"$<${IS_ANDROID32}:LOAD_HARDFP>"
	"$<${IS_ANDROID32}:SOFTFP_LINK>"

	"ANDROID"
	"_ANDROID"
	"__ANDROID__"
)

add_compile_options(
	"$<$<AND:${IS_CLANG},${IS_ANDROID32}>:--sysroot=${CMAKE_ANDROID_NDK}/platforms/android-24/arch-arm>"
	"$<$<AND:${IS_CLANG},${IS_ANDROID64}>:--sysroot=${CMAKE_ANDROID_NDK}/platforms/android-24/arch-arm64>"
	"$<$<AND:${IS_CLANG},${IS_ANDROID32}>:-isystem ${CMAKE_ANDROID_NDK}/platforms/android-24/arch-arm/usr/include>"
	"$<$<AND:${IS_CLANG},${IS_ANDROID64}>:-isystem ${CMAKE_ANDROID_NDK}/platforms/android-24/arch-arm64/usr/include>"

#	"$<$<AND:,${IS_CLANG},${IS_ANDROID}>:>"

	"$<${IS_ANDROID32}:-mfpu=neon-vfpv4>"
	"$<${IS_ANDROID32}:-mcpu=cortex-a7>"
	"$<${IS_ANDROID32}:-mtune=cortex-a7>"
	"$<${IS_ANDROID32}:-mfloat-abi=hard>"
	"$<${IS_ANDROID32}:-fno-builtin-strtod>"
	"$<${IS_ANDROID32}:-fno-builtin-strtof>"
	"$<${IS_ANDROID32}:-fno-builtin-strtold>"
#	"$<${IS_ANDROID32}:>"
#	"$<${IS_ANDROID32}:>"
#	"$<${IS_ANDROID32}:>"

	-funwind-tables
)

include_directories(
	"${CMAKE_ANDROID_NDK}/sources/android/support/include"
	"${CMAKE_ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.9/include"
	"$<${IS_ANDROID32}:${CMAKE_ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi-v7a-hard/include>"
	"$<${IS_ANDROID64}:${CMAKE_ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.9/libs/arm64-v8a/include>"

	"${THIRDPARTYDIR}/curl/include"
	"${THIRDPARTYDIR}/SDL"
	"${THIRDPARTYDIR}/openal-soft/include"
	"${THIRDPARTYDIR}/fontconfig"
	"${THIRDPARTYDIR}/freetype/include"
)

add_link_options(
	"--gcc-toolchain=\"${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64\""
	"$<$<AND:${IS_CLANG},${IS_ANDROID32}>:--sysroot=\"${CMAKE_ANDROID_NDK}/platforms/android-24/arch-arm\">"
	"$<$<AND:${IS_CLANG},${IS_ANDROID64}>:--sysroot=\"${CMAKE_ANDROID_NDK}/platforms/android-24/arch-arm64\">"

	-fuse-ld=lld

	-no-canonical-prefixes

	-stdlib=libstdc++
	"$<${IS_ANDROID32}:-march=armv7-a>"
	"$<${IS_ANDROID32}:-Wl,--no-warn-mismatch>"
	"$<${IS_ANDROID32}:-lm_hard>"

	-funwind-tables

	-nodefaultlibs
)

link_directories(
	"$<${IS_ANDROID32}:${CMAKE_ANDROID_NDK}/platforms/android-24/arch-arm/usr/lib>"
	"$<${IS_ANDROID64}:${CMAKE_ANDROID_NDK}/platforms/android-24/arch-arm64/usr/lib>"
)

link_libraries(
	log
	z
)
endif()

if( NOT ${IS_ANDROID} )

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
	$<${IS_LINUX}:-march=core2>
	-msse2
	-mfpmath=sse
	-mtune=core2
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
