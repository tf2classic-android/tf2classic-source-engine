#=============================================================================
#	ivp_physics.cmake
#
#	Project Script
#=============================================================================

#set( IVP_PHYSICS_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( IVP_PHYSICS_DIR "${SRCDIR}/ivp/ivp_physics" )

set(
	IVP_PHYSICS_SOURCE_FILES

	#IVP_PHYSICS_SRC_DIR
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_3d_solver.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_clustering_longrange.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_clustering_lrange_hash.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_clustering_visual_hash.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_clustering_visualizer.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_coll_del_root_mindist.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_collision_filter.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_compact_ledge.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_compact_ledge_solver.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_i_collision_vhash.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_mindist.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_mindist_debug.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_mindist_event.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_mindist_mcases.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_mindist_minimize.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_mindist_recursive.cxx"

	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_oo_watcher.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_range_manager.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_ray_solver.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_collision/ivp_sphere_solver.cxx"

	#IVP_INTERN_SRC_DIR
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_ball.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_calc_next_psi_solver.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_controller_phantom.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_core.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_environment.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_friction.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_friction_gaps.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_great_matrix.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_hull_manager.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_i_friction_hash.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_impact.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_i_object_vhash.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_merge_core.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_mindist_friction.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_object_attach.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_object.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_physic.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_physic_private.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_polygon.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_sim_unit.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_solver_core_reaction.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_intern/ivp_time.cxx"

	#IVP_UTILITY_SRC_DIR
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_active_value.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_bigvector.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_geometry.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_hash.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_linear.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_mapping.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_memory.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_min_hash.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_min_list.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_os_dep.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_quat.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_string.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_types.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_vector.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_utility/ivu_vhash.cxx"

	#IVP_PHYSICS_COL_SRC_DIR
	"${IVP_PHYSICS_DIR}/../ivp_physics/ivp_anomaly_manager.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_physics/ivp_betterdebugmanager.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_physics/ivp_betterstatisticsmanager.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_physics/ivp_cache_object.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_physics/ivp_liquid_surface_descript.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_physics/ivp_material.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_physics/ivp_performancecounter.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_physics/ivp_stat_manager_cback_con.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_physics/ivp_surface_manager.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_physics/ivp_templates.cxx"

	#IVP_SURFACE_MANAGER_SRC_DIR
	"${IVP_PHYSICS_DIR}/../ivp_surface_manager/ivp_compact_surface.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_surface_manager/ivp_gridbuild_array.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_surface_manager/ivp_surman_grid.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_surface_manager/ivp_surman_polygon.cxx"

	#IVP_CONTROLLER_SRC_DIR
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_actuator.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_actuator_spring.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_buoyancy_solver.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_car_system.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_constraint_car.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_constraint.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_constraint_fixed_keyed.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_constraint_local.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_controller_buoyancy.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_controller_floating.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_controller_motion.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_controller_raycast_car.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_controller_stiff_spring.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_controller_world_frict.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_forcefield.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_multidimensional_interp.cxx"
	"${IVP_PHYSICS_DIR}/../ivp_controller/ivp_template_constraint.cxx"
)

add_library(
	ivp_physics STATIC
	${IVP_PHYSICS_SOURCE_FILES}
)

set_property(
	TARGET ivp_physics PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	ivp_physics PRIVATE
	"${SRCDIR}/ivp/ivp_physics"
	"${SRCDIR}/ivp/ivp_physics/havok"
	"${SRCDIR}/ivp/ivp_utility"
	"${SRCDIR}/ivp/ivp_controller"
	"${SRCDIR}/ivp/ivp_compact_builder"
	"${SRCDIR}/ivp/ivp_collision"
	"${SRCDIR}/ivp/ivp_surface_manager"
	"${SRCDIR}/ivp/ivp_intern"
	"${SRCDIR}/ivp/havana/havok"
)

target_compile_definitions(
	ivp_physics PRIVATE
	HAVANA_CONSTRAINTS
	HAVOK_MOPP
	IVP_VERSION_SDK
)
