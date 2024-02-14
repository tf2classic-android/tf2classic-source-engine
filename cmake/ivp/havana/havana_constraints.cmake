#=============================================================================
#	havana_constraints.cmake
#
#	Project Script
#=============================================================================

#set( HAVANA_CONSTRAINTS_DIR "${CMAKE_CURRENT_LIST_DIR}/havok/hk_physics/constraint" )
set( HAVANA_CONSTRAINTS_DIR "${SRCDIR}/ivp/havana/havok/hk_physics/constraint" )

set(
	HAVANA_CONSTRAINTS_SOURCE_FILES
	"${HAVANA_CONSTRAINTS_DIR}/constraint.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/ball_socket/ball_socket_constraint.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/limited_ball_socket/limited_ball_socket_constraint.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/ragdoll/ragdoll_constraint.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/ragdoll/ragdoll_constraint_bp_builder.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/hinge/hinge_constraint.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/hinge/hinge_bp_builder.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/breakable_constraint/breakable_constraint.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/fixed/fixed_constraint.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/prismatic/prismatic_constraint.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/pulley/pulley_constraint.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/stiff_spring/stiff_spring_constraint.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/local_constraint_system/local_constraint_system.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/../core/rigid_body_core.cpp"
	"${HAVANA_CONSTRAINTS_DIR}/../effector/rigid_body_binary_effector.cpp"
)

add_library(
	havana_constraints STATIC
	${HAVANA_CONSTRAINTS_SOURCE_FILES}
)

set_property(
	TARGET havana_constraints PROPERTY FOLDER "Static Libs"
)

target_compile_definitions(
	havana_constraints PRIVATE
	HAVANA_CONSTRAINTS
	HAVOK_MOPP
	IVP_VERSION_SDK
)

target_include_directories(
	havana_constraints PRIVATE
	"${SRCDIR}/ivp/ivp_physics"
	"${SRCDIR}/ivp/havana/havok"
	"${SRCDIR}/ivp"
	"${SRCDIR}/ivp/ivp_utility"
	"${SRCDIR}/ivp/ivp_controller"
)

