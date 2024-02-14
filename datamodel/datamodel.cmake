#=============================================================================
#	datamodel.cmake
#
#	Project Script
#=============================================================================

set( DATAMODEL_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	DATAMODEL_SOURCE_FILES

	# Source Files
	"${DATAMODEL_DIR}/clipboardmanager.cpp"
	"${DATAMODEL_DIR}/datamodel.cpp"
	"${DATAMODEL_DIR}/dependencygraph.cpp"
	"${DATAMODEL_DIR}/dmattribute.cpp"
	"${DATAMODEL_DIR}/dmelement.cpp"
	"${DATAMODEL_DIR}/dmelementdictionary.cpp"
	"${DATAMODEL_DIR}/dmelementfactoryhelper.cpp"
	"${DATAMODEL_DIR}/DmElementFramework.cpp"
	"${DATAMODEL_DIR}/dmserializerbinary.cpp"
	"${DATAMODEL_DIR}/dmserializerkeyvalues.cpp"
	"${DATAMODEL_DIR}/dmserializerkeyvalues2.cpp"
	"${DATAMODEL_DIR}/undomanager.cpp"

	# Header Files
	"${DATAMODEL_DIR}/clipboardmanager.h"
	"${DATAMODEL_DIR}/datamodel.h"
	"${DATAMODEL_DIR}/dependencygraph.h"
	"${DATAMODEL_DIR}/dmattributeinternal.h"
	"${DATAMODEL_DIR}/dmelementdictionary.h"
	"${SRCDIR}/public/datamodel/dmelementfactoryhelper.h"
	"${DATAMODEL_DIR}/DmElementFramework.h"
	"${SRCDIR}/public/datamodel/dmelementhandle.h"
	"${DATAMODEL_DIR}/dmserializerbinary.h"
	"${DATAMODEL_DIR}/dmserializerkeyvalues.h"
	"${DATAMODEL_DIR}/dmserializerkeyvalues2.h"
	"${DATAMODEL_DIR}/undomanager.h"

	# External
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/tier0/commonmacros.h"
	"${SRCDIR}/public/tier0/dbg.h"
	"${SRCDIR}/public/tier0/fasttimer.h"
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/tier0/platform.h"
	"${SRCDIR}/public/tier0/protected_things.h"
	"${SRCDIR}/public/string_t.h"

	# Interface
	"${SRCDIR}/public/datamodel/attributeflags.h"
	"${SRCDIR}/public/datamodel/dmattributetypes.h"
	"${SRCDIR}/public/datamodel/dmattributevar.h"
	"${SRCDIR}/public/datamodel/dmelement.h"
	"${SRCDIR}/public/datamodel/dmehandle.h"
	"${SRCDIR}/public/datamodel/idatamodel.h"
)

add_library(
	datamodel STATIC
	${DATAMODEL_SOURCE_FILES}
)

set_property(
	TARGET datamodel PROPERTY FOLDER "Static Libs"
)

target_compile_definitions(
	datamodel PRIVATE

	DATAMODEL_LIB
)
