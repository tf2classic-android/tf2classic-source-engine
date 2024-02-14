#=============================================================================
# source_use_steamapi.cmake
#
# Link library with SteamAPI (platform-dependent) (stripped out from baldimore's engine)
#=============================================================================

include_guard( GLOBAL )

function( target_use_steamapi target )

	target_link_libraries(
		${target} PRIVATE

		steam_api
	)

endfunction()
