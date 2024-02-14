#=============================================================================
# source_use_sdl2.cmake
#
# Link library with SDL2 (platform-dependent) (stripped out from baldimore's engine)
#=============================================================================

include_guard( GLOBAL )

function( target_use_sdl2 target )

	target_include_directories(
		${target} PRIVATE

		"${THIRDPARTYDIR}/SDL"
	)

	target_link_libraries(
		${target} PRIVATE

		# linux32/linux64 osx32/osx64
		"$<${IS_POSIX}:SDL2>"

		# win32/win64
		"$<${IS_WINDOWS}:${LIBPUBLIC}/SDL2${IMPLIB_EXT}>"
	)

endfunction()
