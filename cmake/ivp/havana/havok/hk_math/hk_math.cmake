#=============================================================================
#	hk_math.cmake
#
#	Project Script
#=============================================================================

#set( HK_MATH_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( HK_MATH_DIR "${SRCDIR}/ivp/havana/havok/hk_math" )

set(
	HK_MATH_SOURCE_FILES
	"${HK_MATH_DIR}/lcp/lcp_solver.cpp"
	"${HK_MATH_DIR}/incr_lu/incr_lu.cpp"
	"${HK_MATH_DIR}/gauss_elimination/gauss_elimination.cpp"
	"${HK_MATH_DIR}/quaternion/quaternion.cpp"
	"${HK_MATH_DIR}/quaternion/quaternion_util.cpp"
	"${HK_MATH_DIR}/vector3/vector3.cpp"
	"${HK_MATH_DIR}/vector3/vector3_util.cpp"
	"${HK_MATH_DIR}/densematrix.cpp"
	"${HK_MATH_DIR}/densematrix_util.cpp"
	"${HK_MATH_DIR}/eulerangles.cpp"
	"${HK_MATH_DIR}/math.cpp"
	"${HK_MATH_DIR}/matrix3.cpp"
	"${HK_MATH_DIR}/odesolve.cpp"
	"${HK_MATH_DIR}/plane.cpp"
	"${HK_MATH_DIR}/rotation.cpp"
	"${HK_MATH_DIR}/spatial_matrix.cpp"
	"${HK_MATH_DIR}/spatial_vector.cpp"
	"${HK_MATH_DIR}/transform.cpp"
)

add_library(
	hk_math STATIC
	${HK_MATH_SOURCE_FILES}
)

set_property(
	TARGET hk_math PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	hk_math PRIVATE
	"${SRCDIR}/ivp/havana/havok"
	"${SRCDIR}/ivp/ivp_utility"
)

target_compile_definitions(
	hk_math PRIVATE
	_LIB
)
