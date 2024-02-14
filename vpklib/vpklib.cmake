#=============================================================================
#	vpklib.cmake
#
#	Project Script
#=============================================================================

set( VPKLIB_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VPKLIB_SOURCE_FILES

	#$Folder	"Source Files"
	#{
		"${VPKLIB_DIR}/packedstore.cpp"
		#$Folder	"Crypto"
		#{
			"${SRCDIR}/common/simplebitstring.cpp"
		#}
	#}

	#$Folder	"Header Files"
	#{
		"${SRCDIR}/public/vpklib/packedstore.h"
		"${VPKLIB_DIR}/packedstore_internal.h"
		#$Folder	"Crypto"
		#{
			"${SRCDIR}/common/simplebitstring.h"
			#"${SRCDIR}/common/crypto.h"
		#}
	#}
)

add_library(
	vpklib STATIC
	${VPKLIB_SOURCE_FILES}
)

set_property(
	TARGET vpklib PROPERTY FOLDER "Static Libs"
)

if( ${IS_WINDOWS} )
target_compile_options(
	vpklib PRIVATE

	/EHsc
)
endif()
