#=============================================================================
#	choreoobjects.cmake
#
#	Project Script
#=============================================================================

set( CHOREOOBJECTS_DIR ${CMAKE_CUREENT_LIST_DIR} )

set(
	CHOREOOBJECTS_SOURCE_DIR

	# Source Files
	"${SRCDIR}/game/shared/choreoactor.cpp"
	"${SRCDIR}/game/shared/choreochannel.cpp"
	"${SRCDIR}/game/shared/choreoevent.cpp"
	"${SRCDIR}/game/shared/choreoscene.cpp"
	"${SRCDIR}/game/shared/sceneimage.cpp"

	# Header Files
	"${SRCDIR}/game/shared/choreoactor.h"
	"${SRCDIR}/game/shared/choreochannel.h"
	"${SRCDIR}/game/shared/choreoevent.h"
	"${SRCDIR}/game/shared/choreoscene.h"
	"${SRCDIR}/game/shared/ichoreoeventcallback.h"
	"${SRCDIR}/game/shared/sceneimage.h"
	"${SRCDIR}/public/mathlib/mathlib.h"
	"${SRCDIR}/public/mathlib/vector.h"
	"${SRCDIR}/public/mathlib/vector2d.h"
)

add_library(
	choreoobjects STATIC
	${CHOREOOBJECTS_SOURCE_DIR}
)

set_property(
	TARGET choreoobjects PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	choreoobjects PRIVATE
	"${SRCDIR}/game/shared"
	"${SRCDIR}/utils/common"
)
