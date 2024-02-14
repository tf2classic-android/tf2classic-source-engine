#=============================================================================
#	particles.cmake
#
#	Project Script
#=============================================================================

set( PARTICLES_DIR ${CMAKE_CURRENT_LIST_DIR} )

# Source Files
set(
	PARTICLES_SOURCE_FILES

	# Source Files
	"${PARTICLES_DIR}/builtin_constraints.cpp"
	"${PARTICLES_DIR}/builtin_initializers.cpp"
	"${PARTICLES_DIR}/builtin_particle_emitters.cpp"
	"${PARTICLES_DIR}/builtin_particle_forces.cpp"
	"${PARTICLES_DIR}/addbuiltin_ops.cpp"
	"${PARTICLES_DIR}/builtin_particle_ops.cpp"
	"${PARTICLES_DIR}/builtin_particle_render_ops.cpp"
	"${PARTICLES_DIR}/particle_sort.cpp"
	"${PARTICLES_DIR}/particles.cpp"
	"${PARTICLES_DIR}/psheet.cpp"

	# Header Files
	"${PARTICLES_DIR}/random_floats.h"
	"${PARTICLES_DIR}/particles_internal.h"

	# Public Header Files
	"${SRCDIR}/public/particles/particles.h"
)

add_library(
	particles STATIC
	${PARTICLES_SOURCE_FILES}
)

set_property(
	TARGET particles PROPERTY FOLDER "Static Libs"
)
