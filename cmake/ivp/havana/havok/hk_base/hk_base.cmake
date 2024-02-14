#=============================================================================
#	hk_base.cmake
#
#	Project Script
#=============================================================================

#set( HK_BASE_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( HK_BASE_DIR "${SRCDIR}/ivp/havana/havok/hk_base" )

set(
	HK_BASE_SOURCE_FILES
	"${HK_BASE_DIR}/memory/memory.cpp"
	"${HK_BASE_DIR}/memory/memory_util.cpp"
	"${HK_BASE_DIR}/array/array.cpp"
	"${HK_BASE_DIR}/hash/hash.cpp"
	"${HK_BASE_DIR}/stopwatch/stopwatch.cpp"
	"${HK_BASE_DIR}/string/string.cpp"
	"${HK_BASE_DIR}/id_server/id_server.cpp"
	"${HK_BASE_DIR}/base_types.cpp"
	"${HK_BASE_DIR}/console.cpp"
)

add_library(
	hk_base STATIC
	${HK_BASE_SOURCE_FILES}
)

set_property(
	TARGET hk_base PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	hk_base PRIVATE
	"${SRCDIR}/ivp/havana/havok"
	"${SRCDIR}/ivp/ivp_utility"
)

target_compile_definitions(
	hk_base PRIVATE
	_LIB
)

