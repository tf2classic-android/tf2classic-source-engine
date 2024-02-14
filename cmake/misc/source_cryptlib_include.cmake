#=============================================================================
# source_cryptlib_include.cmake
#
# Includes needed to link cryptopp (stripped out from baldimore's engine)
#=============================================================================

include_guard( GLOBAL )

function( target_include_crypto target )

	target_link_libraries(
		${target} PRIVATE

		vpklib
	)

endfunction()
