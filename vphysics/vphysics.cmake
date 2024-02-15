#=============================================================================
#	vphysics.cmake
#
#	Project Script
#=============================================================================

set( VPHYSICS_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VPHYSICS_SOURCE_FILES

	# Source Files
	#"${VPHYSICS_DIR}/stdafx.cpp"
	#{
	#	$Configuration
	#	{
	#		$Compiler
	#		{
	#			$Create/UsePrecompiledHeader	"Create Precompiled Header (/Yc)"
	#		}
	#	}
	#}

	"${VPHYSICS_DIR}/convert.cpp"
	"${SRCDIR}/public/filesystem_helpers.cpp"
	#{
	#	$Configuration
	#	{
	#		$Compiler
	#		{
	#			$Create/UsePrecompiledHeader	"Not Using Precompiled Headers"
	#		}
	#	}
	#}

	"${VPHYSICS_DIR}/ledgewriter.cpp"
	"${VPHYSICS_DIR}/main.cpp"
	"${VPHYSICS_DIR}/physics_airboat.cpp"
	"${VPHYSICS_DIR}/physics_collide.cpp"
	"${VPHYSICS_DIR}/physics_constraint.cpp"
	"${VPHYSICS_DIR}/physics_controller_raycast_vehicle.cpp"
	"${VPHYSICS_DIR}/physics_environment.cpp"
	"${VPHYSICS_DIR}/physics_fluid.cpp"
	"${VPHYSICS_DIR}/physics_friction.cpp"
	"${VPHYSICS_DIR}/physics_material.cpp"
	"${VPHYSICS_DIR}/physics_motioncontroller.cpp"
	"${VPHYSICS_DIR}/physics_object.cpp"
	"${VPHYSICS_DIR}/physics_shadow.cpp"
	"${VPHYSICS_DIR}/physics_spring.cpp"
	"${VPHYSICS_DIR}/physics_vehicle.cpp"
	"${VPHYSICS_DIR}/physics_virtualmesh.cpp"
	"${VPHYSICS_DIR}/trace.cpp"
	"${VPHYSICS_DIR}/vcollide_parse.cpp"
	"${VPHYSICS_DIR}/vphysics_saverestore.cpp"

	# Header Files
	"${VPHYSICS_DIR}/cbase.h"
	"${VPHYSICS_DIR}/convert.h"
	"${VPHYSICS_DIR}/linear_solver.h"
	"${VPHYSICS_DIR}/physics_airboat.h"
	"${VPHYSICS_DIR}/physics_constraint.h"
	"${VPHYSICS_DIR}/physics_controller_raycast_vehicle.h"
	"${VPHYSICS_DIR}/physics_environment.h"
	"${VPHYSICS_DIR}/physics_fluid.h"
	"${VPHYSICS_DIR}/physics_friction.h"
	"${VPHYSICS_DIR}/physics_material.h"
	"${VPHYSICS_DIR}/physics_motioncontroller.h"
	"${VPHYSICS_DIR}/physics_object.h"
	"${VPHYSICS_DIR}/physics_shadow.h"
	"${VPHYSICS_DIR}/physics_spring.h"
	"${VPHYSICS_DIR}/physics_trace.h"
	"${VPHYSICS_DIR}/physics_vehicle.h"
	"${VPHYSICS_DIR}/vcollide_parse_private.h"
	"${VPHYSICS_DIR}/vphysics_internal.h"
	"${VPHYSICS_DIR}/vphysics_saverestore.h"

	# Public Header Files
	"${SRCDIR}/public/vphysics/collision_set.h"
	"${SRCDIR}/public/vphysics/constraints.h"
	"${SRCDIR}/public/datamap.h"
	"${SRCDIR}/public/filesystem_helpers.h"
	"${SRCDIR}/public/vphysics/friction.h"
	"${SRCDIR}/public/vphysics/object_hash.h"
	"${SRCDIR}/public/vphysics/performance.h"
	"${SRCDIR}/public/vphysics/player_controller.h"
	"${SRCDIR}/public/vphysics/stats.h"
	"${SRCDIR}/public/vcollide.h"
	"${SRCDIR}/public/vcollide_parse.h"
	"${SRCDIR}/public/vphysics/vehicles.h"
	"${SRCDIR}/public/vphysics_interface.h"
	"${SRCDIR}/public/vphysics_interfaceV30.h"
)

set_source_files_properties(
	"${VPHYSICS_DIR}/convert.cpp"
	"${SRCDIR}/public/filesystem_helpers.cpp"
	PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

add_library(
	vphysics SHARED
	${VPHYSICS_SOURCE_FILES}
)

set_property(
	TARGET vphysics PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	vphysics
	""
	"vphysics"
	"${GAMEDIR}/bin"
)

target_precompile_headers(
	vphysics PRIVATE
	"${VPHYSICS_DIR}/cbase.h"
)

target_include_directories(
	vphysics PRIVATE

	"${SRCDIR}/ivp/ivp_intern"
	"${SRCDIR}/ivp/ivp_collision"
	"${SRCDIR}/ivp/ivp_physics"
	"${SRCDIR}/ivp/ivp_surface_manager"
	"${SRCDIR}/ivp/ivp_utility"
	"${SRCDIR}/ivp/ivp_controller"
	"${SRCDIR}/ivp/ivp_compact_builder"
	"${SRCDIR}/ivp/havana/havok"
	"${SRCDIR}/ivp/havana"
)

target_compile_definitions(
	vphysics PRIVATE

	VPHYSICS_EXPORTS
	HAVANA_CONSTRAINTS
	HAVOK_MOPP
)

target_link_libraries(
	vphysics PRIVATE

	"$<${IS_WINDOWS}:odbc32>"
	"$<${IS_WINDOWS}:odbccp32>"

	"$<${IS_OSX}:iconv>"

	havana_constraints
	hk_math
	hk_base
	ivp_compactbuilder
	ivp_physics

	mathlib
	tier2
)
