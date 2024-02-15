#=============================================================================
# vpc_groups.cmake
#
# https://habr.com/ru/articles/754272/
#=============================================================================

include_guard( GLOBAL )

function( target_group_sources targetname ) 
	set( SOURCE_GROUP_DELIMITER "/" )
	set( last_dir "" )
	set( files "" )

	get_target_property( sources ${targetname} SOURCES )
	foreach( file ${sources} )                                            
		file( RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}" ${file} ) 
		get_filename_component( dir "${relative_file}" PATH )               
		if( NOT "${dir}" STREQUAL "${last_dir}" )
			if(files)
				source_group( "${last_dir}" FILES ${files} )
			endif()
			set( files "" )
		endif()
		set( files ${files} ${file} )
		set( last_dir "${dir}" )
	endforeach()

	if(files)
		source_group( "${last_dir}" FILES ${files} )
	endif()
endfunction()