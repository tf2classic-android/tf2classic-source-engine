#=============================================================================
# platform_dirs.cmake
#
# $
#=============================================================================

if( ${IS_64BIT} )
	set( PLATFORM_ARCH "64" )
else()
	set( PLATFORM_ARCH "32" )
endif()

if( UNIX )
	if( APPLE )
		set( EXECUTABLE_SUBNAME "_osx" )
		set( PLATFORM_SUBDIR "osx${PLATFORM_ARCH}" )
	else()
		set( EXECUTABLE_SUBNAME "_linux" )
		set( PLATFORM_SUBDIR "linux${PLATFORM_ARCH}" )
	endif()
endif()

if( WIN32 )
	set( EXECUTABLE_SUBNAME "" )
	set( PLATFORM_SUBDIR "win${PLATFORM_ARCH}" )
endif()

if( ${IS_64BIT} )
	set( DX9SDK_LIBDIR "${DX9SDKDIR}/lib/amd64" )
else()
	set( DX9SDK_LIBDIR "${DX9SDKDIR}/lib/x86" )
endif()
