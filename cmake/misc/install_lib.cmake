#=============================================================================
# install_lib.cmake
#
# $
#=============================================================================

include_guard( GLOBAL )

set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )

function( set_install_properties target outputnameprefix outputname install_dir )

	set( _LIB_PREFIX "${outputnameprefix}" )
	if( ${IS_ANDROID} )
	set( _LIB_PREFIX "lib" ) # force "lib-" prefix for android only
	endif()

	set_target_properties(
		${target} PROPERTIES
		PREFIX "${_LIB_PREFIX}"
		OUTPUT_NAME "${outputname}"
	)


	if( NOT MSVC )
		install(
			TARGETS ${target}

			DESTINATION ${install_dir}

			PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE	#
				    GROUP_READ GROUP_EXECUTE			# chmod 755
				    WORLD_READ WORLD_EXECUTE			#
		)

		if( ${IS_LINUX} AND NOT ${IS_ANDROID} )

			# Check for executable
			get_target_property( mytarget_type ${target} TYPE )
			if( ${mytarget_type} STREQUAL "EXECUTABLE" )
				set( _ext "" )
			else()
				set( _ext ".so" )
			endif()

			set( _library_external_dbg "${outputnameprefix}${outputname}${_ext}.dbg" )

			# Cleanup .dbg files
			set_property(
				TARGET ${target}
				APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${_library_external_dbg}
			)

			install(
				FILES ${CMAKE_BINARY_DIR}/${_library_external_dbg}

				DESTINATION ${install_dir}

				PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE	#
					    GROUP_READ GROUP_EXECUTE			# chmod 755
					    WORLD_READ WORLD_EXECUTE			#
			)

		endif()
	else()
		install(
			TARGETS ${target}
			RUNTIME DESTINATION ${install_dir}
			LIBRARY DESTINATION ${install_dir}
		)

		# PDB
		install(
			FILES $<TARGET_PDB_FILE:${target}>
			DESTINATION ${install_dir}
		)
	endif()

endfunction()

# install some precompiled libs
if( ${IS_WINDOWS} )

	# SDL2
	install(
		FILES ${LIBPUBLIC}/SDL2.dll

		DESTINATION ${GAMEDIR}/bin
	)

	# libcurl (TF2Classic)
	install(
		FILES ${LIBPUBLIC}/curl/RelWithDebInfo/libcurl.dll

		DETINATION ${GAMEDIR}/${BUILD_FOLDER}/bin
	)

endif()
