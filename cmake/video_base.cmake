#=============================================================================
# video_base.cmake
#
# $
#=============================================================================

set( USE_GL		0 )
set( USE_SDL		0 )

if( ${IS_POSIX} AND NOT ${IS_DEDICATED} )
	set( USE_GL 1 )
	set( USE_SDL 1 )
endif()

add_compile_definitions(
	"$<${IS_LINUX}:BINK_VIDEO>" # CMAKETODO(SanyaSho): Get rid of BINK_VIDEO and replace it with USE_SDL

	"$<${USE_GL}:GL_GLEXT_PROTOTYPES;DX_TO_GL_ABSTRACTION>"
	"$<${USE_SDL}:USE_SDL>"
)
