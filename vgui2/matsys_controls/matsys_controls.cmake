#=============================================================================
#	matsys_controls.cmake
#
#	Project Script
#=============================================================================

set( MATSYS_CONTROLS_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	MATSYS_CONTROLS_SOURCE_FILES

	# Source Files
	"${MATSYS_CONTROLS_DIR}/assetpicker.cpp"
	"${MATSYS_CONTROLS_DIR}/baseassetpicker.cpp"
	"${MATSYS_CONTROLS_DIR}/colorpickerpanel.cpp"
	"${MATSYS_CONTROLS_DIR}/curveeditorpanel.cpp"
	"${MATSYS_CONTROLS_DIR}/gamefiletreeview.cpp"
	"${MATSYS_CONTROLS_DIR}/manipulator.cpp"
	"${MATSYS_CONTROLS_DIR}/matsyscontrols.cpp"
	"${MATSYS_CONTROLS_DIR}/mdlpanel.cpp"
	"${MATSYS_CONTROLS_DIR}/mdlpicker.cpp"
	"${MATSYS_CONTROLS_DIR}/mdlsequencepicker.cpp"
	"${MATSYS_CONTROLS_DIR}/picker.cpp"
	"${MATSYS_CONTROLS_DIR}/potterywheelpanel.cpp"
	"${MATSYS_CONTROLS_DIR}/proceduraltexturepanel.cpp"
	"${MATSYS_CONTROLS_DIR}/QCGenerator.cpp"
	"${MATSYS_CONTROLS_DIR}/sequencepicker.cpp"
	"${MATSYS_CONTROLS_DIR}/tgapreviewpanel.cpp"
	"${MATSYS_CONTROLS_DIR}/vmtpicker.cpp"
	"${MATSYS_CONTROLS_DIR}/vmtpreviewpanel.cpp"
	"${MATSYS_CONTROLS_DIR}/vtfpicker.cpp"
	"${MATSYS_CONTROLS_DIR}/vtfpreviewpanel.cpp"
	"${MATSYS_CONTROLS_DIR}/vmtpanel.cpp"

	# Header Files
	"${SRCDIR}/public/matsys_controls/assetpicker.h"
	"${SRCDIR}/public/matsys_controls/baseassetpicker.h"
	"${SRCDIR}/public/matsys_controls/colorpickerpanel.h"
	"${SRCDIR}/public/matsys_controls/gamefiletreeview.h"
	"${SRCDIR}/public/matsys_controls/manipulator.h"
	"${SRCDIR}/public/matsys_controls/matsyscontrols.h"
	"${SRCDIR}/public/matsys_controls/mdlpanel.h"
	"${SRCDIR}/public/matsys_controls/mdlpicker.h"
	"${SRCDIR}/public/matsys_controls/mdlsequencepicker.h"
	"${SRCDIR}/public/matsys_controls/picker.h"
	"${SRCDIR}/public/matsys_controls/potterywheelpanel.h"
	"${SRCDIR}/public/matsys_controls/proceduraltexturepanel.h"
	"${SRCDIR}/public/matsys_controls/QCGenerator.h"
	"${SRCDIR}/public/matsys_controls/sequencepicker.h"
	"${SRCDIR}/public/matsys_controls/tgapreviewpanel.h"
	"${SRCDIR}/public/matsys_controls/vmtpicker.h"
	"${SRCDIR}/public/matsys_controls/vmtpreviewpanel.h"
	"${SRCDIR}/public/matsys_controls/vtfpicker.h"
	"${SRCDIR}/public/matsys_controls/vtfpreviewpanel.h"
	"${SRCDIR}/public/matsys_controls/vmtpanel.h"
)

add_library(
	matsys_controls STATIC
	${MATSYS_CONTROLS_SOURCE_FILES}
)

set_property(
	TARGET matsys_controls PROPERTY FOLDER "Static Libs"
)

target_include_directories(
	matsys_controls PRIVATE

	"${THIRDPARTYDIR}"
)

target_link_libraries(
	matsys_controls PRIVATE

	"$<${IS_WINDOWS}:bitmap>"
)
