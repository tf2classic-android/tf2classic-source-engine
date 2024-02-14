#=============================================================================
#	ivp_compactbuilder.cmake
#
#	Project Script
#=============================================================================

#set( IVP_COMPACTBUILDER_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( IVP_COMPACTBUILDER_DIR "${SRCDIR}/ivp/ivp_compact_builder" )

set(
	IVP_COMPACTBUILDER_SOURCE_FILES
	"${IVP_COMPACTBUILDER_DIR}/ivp_compact_ledge_gen.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_compact_modify.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_compact_recursive.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_convex_decompositor.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_halfspacesoup.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_i_fpoint_vhash.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_i_point_vhash.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_object_polygon_tetra.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_rot_inertia_solver.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_surbuild_halfspacesoup.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_surbuild_ledge_soup.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_surbuild_pointsoup.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_surbuild_polygon_convex.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_surbuild_polyhdrn_cncv.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_surbuild_q12.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_templates_intern.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivp_tetra_intrude.cxx"
	"${IVP_COMPACTBUILDER_DIR}/ivv_cluster_min_hash.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull_geom.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull_geom2.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull_global.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull_io.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull_mem.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull_merge.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull_poly.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull_poly2.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull_qset.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull_stat.cxx"
	"${IVP_COMPACTBUILDER_DIR}/qhull_user.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_cutfac.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_cvdec3.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_drdec3.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_dsphdc.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_edght.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_initcb.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_insed3.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_insfac.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_insvr3.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_prime.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_ptpolg.cxx"
	"${IVP_COMPACTBUILDER_DIR}/geompack_resedg.cxx"
	"${IVP_COMPACTBUILDER_DIR}/3dsimport_co.cxx"
)

add_library(
	ivp_compactbuilder STATIC
	${IVP_COMPACTBUILDER_SOURCE_FILES}
)

set_property(
	TARGET ivp_compactbuilder PROPERTY FOLDER "Static Libs"
)

target_compile_definitions(
	ivp_compactbuilder PRIVATE
	IVP_VERSION_SDK
	HAVOK_MOOP
)

target_include_directories(
	ivp_compactbuilder PRIVATE
	"${SRCDIR}/ivp/ivp_physics"
	"${SRCDIR}/ivp/ivp_utility"
	"${SRCDIR}/ivp/ivp_compact_builder"
	"${SRCDIR}/ivp/ivp_collision"
	"${SRCDIR}/ivp/ivp_controller"
	"${SRCDIR}/ivp/ivp_surface_manager"
	"${SRCDIR}/ivp/havana/havok/"
)
