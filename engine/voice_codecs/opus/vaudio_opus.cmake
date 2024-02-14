#=============================================================================
#	vaudio_opus.cmake
#
#	Project Script
#=============================================================================

set( ENGINE_SRC_DIR "${SRCDIR}/engine" )
set( VAUDIO_OPUS_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	VAUDIO_OPUS_SOURCE_FILES

	# Source Files
	"${SRCDIR}/tier1/interface.cpp"
	"${ENGINE_SRC_DIR}/voice_codecs/frame_encoder/voice_codec_frame.cpp"
	"${VAUDIO_OPUS_DIR}/voiceencoder_opus.cpp"

	# Header Files
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/vaudio/ivaudio.h"
)

add_library(
	vaudio_opus SHARED
	${VAUDIO_OPUS_SOURCE_FILES}
)

set_property(
	TARGET vaudio_opus PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	vaudio_opus
	""
	"vaudio_opus"
	"${GAMEDIR}/bin"
)

target_include_directories(
	vaudio_opus PRIVATE

	"${VAUDIO_OPUS_DIR}"
	"${THIRDPARTYDIR}/opus-1.4/include"
	"${ENGINE_SRC_DIR}/voice_codecs/frame_encoder"
	"${ENGINE_SRC_DIR}/audio"
	"${SRCDIR}/public/engine/audio"
	"${ENGINE_SRC_DIR}"
)

target_compile_definitions(
	vaudio_opus PRIVATE

	OPUS_EXPORTS
)

target_link_libraries(
	vaudio_opus PRIVATE

	"$<${IS_POSIX}:opus>"
	"$<${IS_WINDOWS}:${LIBCOMMON}/opus${STATIC_LIB_EXT}>"
)
