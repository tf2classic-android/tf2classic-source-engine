#=============================================================================
#	engine.cmake
#
#	Project Script
#=============================================================================

if( ${IS_LINUX} AND NOT ${IS_ANDROID} )
	# Check for system cURL
	find_package( CURL REQUIRED )
endif()

# CMAKETODO(SanyaSho): stub
set( ENGINE_GPROFILER 0 )
set( ENGINE_ENABLE_RPT 0 )

set( ENGINE_DIR ${CMAKE_CURRENT_LIST_DIR} )

set(
	ENGINE_CLIENT_SOURCES

	#"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/client_pch.cpp>"
	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Create Precompiled Header (/Yc)"
	#				$Create/UsePCHThroughFile		"client_pch.h"
	#				$PrecompiledHeaderFile			"$(IntDir)/client_pch.pch"
	#			}
	#		}
	#	}
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_rcon.cpp>"
	"${ENGINE_DIR}/socketcreator.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/rpt_engine.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_steamauth.cpp>"
	"${ENGINE_DIR}/clientframe.cpp"
	"${ENGINE_DIR}/decal_clip.cpp"
	"${ENGINE_DIR}/demofile.cpp"
	"${ENGINE_DIR}/DevShotGenerator.cpp"
	"${ENGINE_DIR}/OcclusionSystem.cpp"
	"${ENGINE_DIR}/tmessage.cpp"
	#"${ENGINE_DIR}/logofile_shared.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/r_efx.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/view.cpp>"
	"${ENGINE_DIR}/baseclient.cpp"
	"${ENGINE_DIR}/baseclientstate.cpp"
	"${ENGINE_DIR}/cbenchmark.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cdll_engine_int.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_main.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_demo.cpp>"
	"$<${IS_DEDICATED}:${ENGINE_DIR}/cl_null.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_demoaction.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_demoaction_types.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_demoactioneditors.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_demoactionmanager.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_demoeditorpanel.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_demosmootherpanel.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_demouipanel.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_foguipanel.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_txviewpanel.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_entityreport.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_ents_parse.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_localnetworkbackdoor.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_parse_event.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_pluginhelpers.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_pred.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_texturelistpanel.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/client.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/colorcorrectionpanel.cpp>"
	"${ENGINE_DIR}/console.cpp"
  	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Use Precompiled Header (/Yu)"
	#				$Create/UsePCHThroughFile		"client_pch.h"
	#				$PrecompiledHeaderFile			"$(IntDir)/client_pch.pch"
	#			}
	#		}
	#	}
	#"${ENGINE_DIR}/render_pch.cpp"
	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Create Precompiled Header (/Yc)"
	#				$Create/UsePCHThroughFile		"render_pch.h"
	#				$PrecompiledHeaderFile			"$(IntDir)/render_pch.pch"
	#			}
	#		}
	#	}
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/buildcubemaps.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/debug_leafvis.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/debugoverlay.cpp>"
	"${ENGINE_DIR}/decals.cpp"
	"${ENGINE_DIR}/disp.cpp"
	"${ENGINE_DIR}/disp_interface.cpp"
	"${ENGINE_DIR}/disp_mapload.cpp"
	"${ENGINE_DIR}/gl_draw.cpp"
	"${ENGINE_DIR}/gl_rsurf.cpp"
	"${ENGINE_DIR}/gl_shader.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/gl_drawlights.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/gl_lightmap.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/gl_matsysiface.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/gl_rlight.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/gl_rmain.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/gl_rmisc.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/gl_screen.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/gl_warp.cpp>"
	"${ENGINE_DIR}/l_studio.cpp"
	"${ENGINE_DIR}/matsys_interface.cpp"
	"${ENGINE_DIR}/modelloader.cpp"
	"${ENGINE_DIR}/Overlay.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/r_areaportal.cpp>"
	"${ENGINE_DIR}/r_decal.cpp"
	"${ENGINE_DIR}/r_linefile.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/shadowmgr.cpp>"
	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Use Precompiled Header (/Yu)"
	#				$Create/UsePCHThroughFile		"render_pch.h"
	#				$PrecompiledHeaderFile			"$(IntDir)/render_pch.pch"
	#			}
	#		}
	#	}
)

set(
	ENGINE_SERVER_SOURCES

	#"${ENGINE_DIR}/server_pch.cpp"
	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Create Precompiled Header (/Yc)"
	#				$Create/UsePCHThroughFile		"server_pch.h"
	#				$PrecompiledHeaderFile			"$(IntDir)/server_pch.pch"
	#			}
	#		}
	#	}

	"${ENGINE_DIR}/sv_ipratelimit.cpp"
	"${ENGINE_DIR}/sv_rcon.cpp"
	"${ENGINE_DIR}/sv_steamauth.cpp"
	"${ENGINE_DIR}/sv_uploaddata.cpp"
	"${ENGINE_DIR}/sv_uploadgamestats.cpp"
	"${ENGINE_DIR}/vengineserver_impl.cpp"

	"${ENGINE_DIR}/sv_main.cpp"
	"${ENGINE_DIR}/sv_client.cpp"
	"${ENGINE_DIR}/sv_ents_write.cpp"
	"${ENGINE_DIR}/sv_filter.cpp"
	"${ENGINE_DIR}/sv_framesnapshot.cpp"
	"${ENGINE_DIR}/sv_log.cpp"
	"${ENGINE_DIR}/sv_packedentities.cpp"
	"${ENGINE_DIR}/sv_plugin.cpp"
	"${ENGINE_DIR}/sv_precache.cpp"
	"${ENGINE_DIR}/sv_redirect.cpp"
	"${ENGINE_DIR}/sv_remoteaccess.cpp"
  	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Use Precompiled Header (/Yu)"
	#				$Create/UsePCHThroughFile		"server_pch.h"
	#				$PrecompiledHeaderFile			"$(IntDir)/server_pch.pch"
	#			}
	#		}
	#	}
)

set(
	ENGINE_SOURCES

	# Source Files
	"${ENGINE_DIR}/baseautocompletefilelist.cpp"
	"${ENGINE_DIR}/baseserver.cpp"
	"${ENGINE_DIR}/bitbuf_errorhandler.cpp"
	"${SRCDIR}/public/blockingudpsocket.cpp"
	"${SRCDIR}/public/bsptreedata.cpp"
	"${SRCDIR}/public/builddisp.cpp"
	"${ENGINE_DIR}/changeframelist.cpp"
	"${ENGINE_DIR}/checksum_engine.cpp"
	"${ENGINE_DIR}/ccs.cpp"
	"${ENGINE_DIR}/clockdriftmgr.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/cl_bounded_cvars.cpp>"
	"${ENGINE_DIR}/cl_check_process.cpp"
	"${ENGINE_DIR}/cmd.cpp"
	"${ENGINE_DIR}/cmodel.cpp"
	"${ENGINE_DIR}/cmodel_bsp.cpp"
	"${ENGINE_DIR}/cmodel_disp.cpp"
	"${SRCDIR}/public/collisionutils.cpp"
	"${ENGINE_DIR}/common.cpp"
	"${SRCDIR}/public/crtmemdebug.cpp"
	"${ENGINE_DIR}/cvar.cpp"
	"${SRCDIR}/public/disp_common.cpp"
	"${ENGINE_DIR}/disp_defs.cpp"
	"${ENGINE_DIR}/disp_helpers.cpp"
	"${SRCDIR}/public/disp_powerinfo.cpp"
	"${SRCDIR}/public/dispcoll_common.cpp"
	"${ENGINE_DIR}/DownloadListGenerator.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/downloadthread.cpp>"
	"${ENGINE_DIR}/dt.cpp"
	"${ENGINE_DIR}/dt_common_eng.cpp"
	"${ENGINE_DIR}/dt_encode.cpp"
	"${ENGINE_DIR}/dt_instrumentation.cpp"
	"${ENGINE_DIR}/dt_instrumentation_server.cpp"
	"${ENGINE_DIR}/dt_localtransfer.cpp"
	"${SRCDIR}/public/dt_recv.cpp"
	"${ENGINE_DIR}/dt_recv_decoder.cpp"
	"${ENGINE_DIR}/dt_recv_eng.cpp"
	"${SRCDIR}/public/dt_send.cpp"
	"${ENGINE_DIR}/dt_send_eng.cpp"
	"${ENGINE_DIR}/dt_stack.cpp"
	"${ENGINE_DIR}/dt_test.cpp"
	"${SRCDIR}/public/dt_utlvector_common.cpp"
	"${SRCDIR}/public/dt_utlvector_recv.cpp"
	"${SRCDIR}/public/dt_utlvector_send.cpp"
	"${ENGINE_DIR}/enginesingleuserfilter.cpp"
	"${ENGINE_DIR}/enginestats.cpp"
	"${ENGINE_DIR}/enginethreads.cpp"
	"${ENGINE_DIR}/enginetrace.cpp"
	"${ENGINE_DIR}/filesystem_engine.cpp"
	"${SRCDIR}/public/filesystem_helpers.cpp"
	"$<$<NOT:${IS_CLANGCL}>:${SRCDIR}/public/filesystem_init.cpp>"
	"${ENGINE_DIR}/filetransfermgr.cpp"
	"${ENGINE_DIR}/GameEventManager.cpp"
	"${ENGINE_DIR}/GameEventManagerOld.cpp"
	"${ENGINE_DIR}/gametrace_engine.cpp"
	"${ENGINE_DIR}/hltvclient.cpp"
	"${ENGINE_DIR}/hltvclientstate.cpp"
	"${ENGINE_DIR}/hltvdemo.cpp" 
	"${ENGINE_DIR}/hltvserver.cpp"
	"${ENGINE_DIR}/hltvtest.cpp"
	"${ENGINE_DIR}/host.cpp"
	"${ENGINE_DIR}/host_cmd.cpp"
	"${ENGINE_DIR}/host_listmaps.cpp"
	"${ENGINE_DIR}/host_phonehome.cpp"
	"${ENGINE_DIR}/host_state.cpp"
	"${ENGINE_DIR}/initmathlib.cpp"
	"${SRCDIR}/common/language.cpp"
	"${ENGINE_DIR}/LocalNetworkBackdoor.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/LoadScreenUpdate.cpp>"
	"${SRCDIR}/public/lumpfiles.cpp"
	"${ENGINE_DIR}/MapReslistGenerator.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/matchmakinghost.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/matchmakingqos.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/matchmakingclient.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/matchmakingshared.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/matchmakingmigrate.cpp>"
	"${ENGINE_DIR}/replaydemoplayer.cpp"
	"${ENGINE_DIR}/materialproxyfactory.cpp"
	"${ENGINE_DIR}/mem_fgets.cpp"
	"${ENGINE_DIR}/mod_vis.cpp"
	"${ENGINE_DIR}/ModelInfo.cpp"
	"${ENGINE_DIR}/net_chan.cpp"
	"${ENGINE_DIR}/net_synctags.cpp"
	"${ENGINE_DIR}/net_ws.cpp"
	"${ENGINE_DIR}/net_ws_queued_packet_sender.cpp"
	"${SRCDIR}/common/netmessages.cpp"
	"${SRCDIR}/common/steamid.cpp"
	"${ENGINE_DIR}/networkstringtable.cpp"
	"${ENGINE_DIR}/NetworkStringTableItem.cpp"
	"${ENGINE_DIR}/networkstringtableserver.cpp"
	"${SRCDIR}/public/networkvar.cpp"
	"${ENGINE_DIR}/packed_entity.cpp"
	"${ENGINE_DIR}/pure_server.cpp"
	"${ENGINE_DIR}/pr_edict.cpp"
	"${ENGINE_DIR}/precache.cpp"
	"${ENGINE_DIR}/quakedef.cpp"
	"${ENGINE_DIR}/randomstream.cpp"
	"${SRCDIR}/common/randoverride.cpp"
	"${SRCDIR}/public/registry.cpp"
	"${ENGINE_DIR}/engine_replay_int.cpp"
	"${ENGINE_DIR}/replay_internal.cpp"
	"${ENGINE_DIR}/replaydemo.cpp"
	"${ENGINE_DIR}/replayserver.cpp"
	"${SRCDIR}/public/sentence.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/Session.cpp>"
	"${ENGINE_DIR}/sound_shared.cpp"
	"${ENGINE_DIR}/spatialpartition.cpp"
	"${ENGINE_DIR}/staticpropmgr.cpp"
	"${SRCDIR}/public/studio.cpp"
	"${ENGINE_DIR}/sys_dll.cpp"
	"${ENGINE_DIR}/sys_dll2.cpp"
	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				// This file uses _set_se_translator which requires C++ exceptions to be enabled,
	#				// which also means it can't use the precompiled header because it is built
	#				// without precompiled headers.
	#				$Create/UsePrecompiledHeader "Not Using Precompiled Headers"
	#				$EnableC++Exceptions "Yes with SEH Exceptions (/EHa)" [$WINDOWS]
	#			}
	#		}
	#	}
	"${ENGINE_DIR}/sys_engine.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/sys_mainwind.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/bugreporter.cpp>"
	"$<${IS_DEDICATED}:${ENGINE_DIR}/sys_linuxwind.cpp>" #Mr0maks: actually this is stubs for dedicated but IS_POSIX...
	"${ENGINE_DIR}/testscriptmgr.cpp"
	"${ENGINE_DIR}/traceinit.cpp"
	"${ENGINE_DIR}/voiceserver_impl.cpp"
	"${ENGINE_DIR}/vprof_engine.cpp"
	"${ENGINE_DIR}/vprof_record.cpp"
	"${ENGINE_DIR}/world.cpp"
	"${SRCDIR}/public/XZip.cpp"
	"${SRCDIR}/public/XUnzip.cpp"
	"${ENGINE_DIR}/zone.cpp"
	"${ENGINE_DIR}/cheatcodes.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/download.cpp>"
	"${SRCDIR}/public/editor_sendcommand.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/host_saverestore.cpp>"
	"${ENGINE_DIR}/keys.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/lightcache.cpp>"
	"${ENGINE_DIR}/networkstringtableclient.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/saverestore_filesystem.cpp>"
	"${SRCDIR}/public/scratchpad3d.cpp"
	"${ENGINE_DIR}/servermsghandler.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/sys_getmodes.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/vgui_askconnectpanel.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/xboxsystem.cpp>"
	"${SRCDIR}/common/SourceAppInfo.cpp"
	"${ENGINE_DIR}/masterserver.cpp"
)

set_source_files_properties(
	"${ENGINE_DIR}/sys_dll2.cpp"
	PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

set(
	ENGINE_AUDIO_SOURCES

	"${ENGINE_DIR}/snd_io.cpp"
	"${ENGINE_DIR}/EngineSoundServer.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/EngineSoundClient.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/engsoundservice.cpp>"
	"${ENGINE_DIR}/audio/voice_wavefile.cpp"
	"$<$<AND:$<NOT:${IS_DEDICATED}>,$<NOT:${IS_X360}>>:${ENGINE_DIR}/audio/MPAFile.cpp>"
	"$<$<AND:$<NOT:${IS_DEDICATED}>,$<NOT:${IS_X360}>>:${ENGINE_DIR}/audio/MPAHeader.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/circularbuffer.cpp>"
	"$<${IS_POSIX}:${ENGINE_DIR}/audio/snd_posix.cpp>"

	#"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/audio_pch.cpp>"
	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Create Precompiled Header (/Yc)"
	#				$Create/UsePCHThroughFile		"audio_pch.h" [!$POSIX]
	#				$Create/UsePCHThroughFile		"audio/audio_pch.h" [$POSIX]
	#				$PrecompiledHeaderFile			"$(IntDir)/audio_pch.pch"
	#				$AdditionalIncludeDirectories	"$BASE;$SRCDIR/engine/audio/public;$SRCDIR/engine/audio/private"
	#			}
	#		}
	#	}

	# precompiled header section: common audio files
	"${ENGINE_DIR}/audio/vox.cpp"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_dev_common.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_dma.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_dsp.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_mix.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_sentence_mixer.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_wave_data.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_wave_mixer.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_wave_mixer_adpcm.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_wave_source.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_wave_temp.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_win.cpp>"
	"${ENGINE_DIR}/audio/voice_gain.cpp"
	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Use Precompiled Header (/Yu)"
	#				$Create/UsePCHThroughFile		"audio_pch.h"
	#				$PrecompiledHeaderFile			"$(IntDir)/audio_pch.pch"
	#			}
	#		}
	#	}

	# WIN32 only audio files
	# ne, nihuya
	"$<$<AND:${IS_WINDOWS},$<NOT:${IS_DEDICATED}>>:${ENGINE_DIR}/audio/snd_dev_direct.cpp>"
	"$<$<AND:${IS_WINDOWS},$<NOT:${IS_DEDICATED}>>:${ENGINE_DIR}/audio/snd_dev_wave.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_mp3_source.cpp>"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/snd_wave_mixer_mp3.cpp>"
	"$<$<AND:$<NOT:${IS_DEDICATED}>,$<NOT:${IS_X360}>>:${ENGINE_DIR}/audio/VBRHeader.cpp>"
	"$<$<AND:$<NOT:${IS_DEDICATED}>,$<NOT:${IS_X360}>>:${ENGINE_DIR}/audio/voice.cpp>"
	"$<${IS_WINDOWS}:${ENGINE_DIR}/audio/voice_mixer_controls.cpp>"
	"$<${IS_WINDOWS}:${ENGINE_DIR}/audio/voice_record_dsound.cpp>"
	"$<$<AND:$<NOT:${IS_DEDICATED}>,$<NOT:${IS_X360}>>:${ENGINE_DIR}/audio/voice_sound_engine_interface.cpp>"
	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Use Precompiled Header (/Yu)"
	#				$Create/UsePCHThroughFile		"audio_pch.h"
	#				$PrecompiledHeaderFile			"$(IntDir)/audio_pch.pch"
	#			}
	#		}
	#	}

	# X360 only audio files
	#"${ENGINE_DIR}/audio/snd_dev_xaudio.cpp"
	#"${ENGINE_DIR}/audio/snd_wave_mixer_xma.cpp"
	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Use Precompiled Header (/Yu)"
	#				$Create/UsePCHThroughFile		"audio_pch.h"
	#				$PrecompiledHeaderFile			"$(IntDir)/audio_pch.pch"
	#			}
	#		}
	#	}

	#$File "audio/private/snd_stubs.cpp"
	"$<${USE_SDL}:${ENGINE_DIR}/audio/voice_record_sdl.cpp>"
	"$<$<AND:${USE_SDL},$<NOT:${IS_OSX}>>:${ENGINE_DIR}/audio/snd_dev_sdl.cpp>"
	"$<$<AND:${USE_SDL},$<NOT:${IS_OSX}>>:${ENGINE_DIR}/audio/snd_dev_sdl.h>"
	"$<${IS_OSX}:${ENGINE_DIR}/audio/snd_dev_openal.cpp>"
	"$<${IS_OSX}:${ENGINE_DIR}/audio/snd_dev_openal.h>"
	"$<$<AND:${IS_OSX},$<NOT:${IS_DEDICATED}>>:${ENGINE_DIR}/audio/snd_dev_mac_audioqueue.cpp>"
	"$<$<AND:${IS_OSX},$<NOT:${IS_DEDICATED}>>:${ENGINE_DIR}/audio/snd_dev_mac_audioqueue.h>"
	"$<$<OR:${IS_OSX},$<AND:${IS_LINUX},$<NOT:${IS_DEDICATED}>>>:${ENGINE_DIR}/audio/voice_mixer_controls_openal.cpp>"
	"$<$<OR:${IS_OSX},$<AND:${IS_LINUX},$<NOT:${IS_DEDICATED}>>>:${ENGINE_DIR}/audio/voice_record_openal.cpp>"
	"$<${IS_OSX}:${ENGINE_DIR}/audio/voice_record_mac_audioqueue.cpp>"

	# Header Files
	"${ENGINE_DIR}/audio/audio_pch.h"
	"${SRCDIR}/public/engine/audio/ivoicecodec.h"
	"${SRCDIR}/public/engine/audio/ivoicerecord.h"
	"${SRCDIR}/public/engine/audio/snd_audio_source.h"
	"${SRCDIR}/public/engine/audio/snd_device.h"
	"${SRCDIR}/public/engine/audio/snd_io.h"
	"${SRCDIR}/public/engine/audio/sound.h"
	"${SRCDIR}/public/engine/audio/soundservice.h"
	"${SRCDIR}/public/engine/audio/voice.h"
	"${SRCDIR}/public/engine/audio/vox.h"
	"$<${IS_WINDOWS}:${ENGINE_DIR}/audio/MPAFile.h>"
	"$<${IS_WINDOWS}:${ENGINE_DIR}/audio/MPAHeader.h>"
	"$<${IS_WINDOWS}:${ENGINE_DIR}/audio/VBRHeader.h>"
	"${ENGINE_DIR}/audio/circularbuffer.h"
	"${ENGINE_DIR}/audio/eax.h"
	"${ENGINE_DIR}/audio/snd_channels.h"
	"${ENGINE_DIR}/audio/snd_convars.h"
	"${ENGINE_DIR}/audio/snd_dev_common.h"
	"$<${IS_WINDOWS}:${ENGINE_DIR}/audio/snd_dev_direct.h>"
	"$<${IS_WINDOWS}:${ENGINE_DIR}/audio/snd_dev_wave.h>"
	"${ENGINE_DIR}/audio/snd_dev_xaudio.h"
	"${ENGINE_DIR}/audio/snd_dma.h"
	"${ENGINE_DIR}/audio/snd_env_fx.h"
	"${SRCDIR}/public/engine/audio/snd_fixedint.h"
	"${SRCDIR}/public/engine/audio/snd_mix_buf.h"
	"${ENGINE_DIR}/audio/snd_sfx.h"
	"${ENGINE_DIR}/audio/snd_wave_mixer.h"
	"${ENGINE_DIR}/audio/snd_wave_mixer_adpcm.h"
	"${ENGINE_DIR}/audio/snd_wave_mixer_xma.h"
	"${ENGINE_DIR}/audio/snd_wave_mixer_private.h"
	"${ENGINE_DIR}/audio/snd_wave_source.h"
	"${ENGINE_DIR}/audio/snd_wave_temp.h"
	"${ENGINE_DIR}/audio/sound_private.h"
	"${ENGINE_DIR}/audio/voice_gain.h"
	"${ENGINE_DIR}/audio/voice_mixer_controls.h"
	"${ENGINE_DIR}/audio/voice_sound_engine_interface.h"
	"${ENGINE_DIR}/audio/voice_wavefile.h"
	"${ENGINE_DIR}/audio/vox_private.h"
)

set(
	ENGINE_VGUI_PANELS_SOURCES # [!$DEDICATED]

	# public file, no pch
	"${SRCDIR}/public/vgui_controls/vgui_controls.cpp"
	"${SRCDIR}/common/vgui/vgui_basebudgetpanel.cpp"
	"${SRCDIR}/common/vgui/vgui_budgetbargraphpanel.cpp"
	"${SRCDIR}/common/vgui/vgui_budgethistorypanel.cpp"
	"${SRCDIR}/common/vgui/vgui_budgetpanelshared.cpp"

	"${ENGINE_DIR}/perfuipanel.cpp"
	"${ENGINE_DIR}/vgui_basepanel.cpp"
	"${ENGINE_DIR}/vgui_baseui_interface.cpp"
	"${ENGINE_DIR}/vgui_budgetpanel.cpp"
	"${ENGINE_DIR}/vgui_DebugSystemPanel.cpp"
	"${ENGINE_DIR}/vgui_drawtreepanel.cpp"
	"${ENGINE_DIR}/vgui_helpers.cpp"
	"${ENGINE_DIR}/vgui_texturebudgetpanel.cpp"
	"${ENGINE_DIR}/vgui_vprofgraphpanel.cpp"
	"${ENGINE_DIR}/vgui_vprofpanel.cpp"
	#	{
	#		$Configuration
	#		{
	#			$Compiler
	#			{
	#				$Create/UsePrecompiledHeader	"Use Precompiled Header (/Yu)"
	#				$Create/UsePCHThroughFile		"client_pch.h"
	#				$PrecompiledHeaderFile			"$(IntDir)/client_pch.pch"
	#			}
	#		}
	#	}

	"${ENGINE_DIR}/vgui_budgetpanel.h"
	"${SRCDIR}/common/vgui/vgui_budgetpanelshared.h"
	"${ENGINE_DIR}/vgui_texturebudgetpanel.h"
	"${ENGINE_DIR}/vgui_vprofgraphpanel.h"
	"${ENGINE_DIR}/vgui_vprofpanel.h"

	"${SRCDIR}/common/vgui/vgui_basebudgetpanel.h"
	"${ENGINE_DIR}/vgui_baseui_interface.h"
	"${SRCDIR}/common/vgui/vgui_budgetbargraphpanel.h"
	"${SRCDIR}/common/vgui/vgui_budgethistorypanel.h"
)

set(
	ENGINE_TOOLS_FRAMEWORK_SOURCES

	"${SRCDIR}/public/toolframework/iclientenginetools.h"
	"${SRCDIR}/public/toolframework/ienginetool.h"
	"${SRCDIR}/public/toolframework/iserverenginetools.h"
	"${SRCDIR}/public/toolframework/itooldictionary.h"
	"${SRCDIR}/public/toolframework/itoolentity.h"
	"${SRCDIR}/public/toolframework/itoolframework.h"
	"${SRCDIR}/public/toolframework/itoolsystem.h"
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/enginetool.cpp>"
	"${ENGINE_DIR}/toolframework.cpp"
)

set(
	ENGINE_HEADER_FILES

	# Header Files
	"${ENGINE_DIR}/baseclient.h"
	"${ENGINE_DIR}/baseclientstate.h"
	"${ENGINE_DIR}/baseserver.h"
	"${ENGINE_DIR}/bitbuf_errorhandler.h"
	"${SRCDIR}/public/bsptreedata.h"
	"${ENGINE_DIR}/cbenchmark.h"
	"${ENGINE_DIR}/cdll_engine_int.h"
	"${ENGINE_DIR}/cl_check_process.h"
	"${SRCDIR}/public/cdll_int.h"
	"${ENGINE_DIR}/changeframelist.h"
	"${ENGINE_DIR}/cheatcodes.h"
	"${ENGINE_DIR}/checksum_engine.h"
	"${ENGINE_DIR}/cl_demo.h"
	"${ENGINE_DIR}/cl_entityreport.h"
	"${ENGINE_DIR}/cl_ents_parse.h"
	"${ENGINE_DIR}/cl_localnetworkbackdoor.h"
	"${ENGINE_DIR}/cl_main.h"
	"${ENGINE_DIR}/cl_parse_event.h"
	"${ENGINE_DIR}/cl_pluginhelpers.h"
	"${ENGINE_DIR}/cl_pred.h"
	"${ENGINE_DIR}/cl_rcon.h"
	"${ENGINE_DIR}/socketcreator.h"
	"${ENGINE_DIR}/cl_steamauth.h"
	"${ENGINE_DIR}/cl_texturelistpanel.h"
	"${ENGINE_DIR}/client.h"
	"${SRCDIR}/public/client_render_handle.h"
	"${ENGINE_DIR}/clientframe.h"
	"${ENGINE_DIR}/clockdriftmgr.h"
	"${ENGINE_DIR}/cmd.h"
	"${ENGINE_DIR}/cmodel_engine.h"
	"${ENGINE_DIR}/cmodel_private.h"
	"${SRCDIR}/public/collisionutils.h"
	"${ENGINE_DIR}/common.h"
	"${SRCDIR}/public/mathlib/compressed_light_cube.h"
	"${ENGINE_DIR}/conprint.h"
	"${ENGINE_DIR}/console.h"
	"${ENGINE_DIR}/cvar.h"
	"${ENGINE_DIR}/debug_leafvis.h"
	"${ENGINE_DIR}/debugoverlay.h"
	"${ENGINE_DIR}/decal.h"
	"${ENGINE_DIR}/decal_clip.h"
	"${ENGINE_DIR}/decal_private.h"
	"${ENGINE_DIR}/demo.h"
	"${ENGINE_DIR}/demofile.h"
	"${ENGINE_DIR}/DevShotGenerator.h"
	"${ENGINE_DIR}/disp.h"
	"${SRCDIR}/public/disp_common.h"
	"${ENGINE_DIR}/disp_defs.h"
	"${ENGINE_DIR}/disp_helpers.h"
	"${SRCDIR}/public/disp_powerinfo.h"
	"${SRCDIR}/public/disp_tesselate.h"
	"${SRCDIR}/public/dispcoll_common.h"
	"${ENGINE_DIR}/dispnode.h"
	"${ENGINE_DIR}/download.h"
	"${ENGINE_DIR}/download_internal.h"
	"${ENGINE_DIR}/DownloadListGenerator.h"
	"${ENGINE_DIR}/draw.h"
	"${ENGINE_DIR}/dt.h"
	"${SRCDIR}/public/dt_common.h"
	"${ENGINE_DIR}/dt_encode.h"
	"${ENGINE_DIR}/dt_instrumentation.h"
	"${ENGINE_DIR}/dt_instrumentation_server.h"
	"${ENGINE_DIR}/dt_localtransfer.h"
	"${SRCDIR}/public/dt_recv.h"
	"${ENGINE_DIR}/dt_recv_decoder.h"
	"${ENGINE_DIR}/dt_recv_eng.h"
	"${SRCDIR}/public/dt_send.h"
	"${ENGINE_DIR}/dt_send_eng.h"
	"${ENGINE_DIR}/dt_stack.h"
	"${ENGINE_DIR}/dt_test.h"
	"${SRCDIR}/public/dt_utlvector_common.h"
	"${SRCDIR}/public/eifacev21.h"
	"${SRCDIR}/public/engine_hlds_api.h"
	"${SRCDIR}/common/engine_launcher_api.h"
	"${ENGINE_DIR}/enginesingleuserfilter.h"
	"${ENGINE_DIR}/EngineSoundInternal.h"
	"${ENGINE_DIR}/enginestats.h"
	"${ENGINE_DIR}/enginetrace.h"
	"${ENGINE_DIR}/ents_shared.h"
	"${SRCDIR}/public/filesystem.h"
	"${ENGINE_DIR}/filesystem_engine.h"
	"${ENGINE_DIR}/filetransfermgr.h"
	"${SRCDIR}/common/FindSteamServers.h"
	"${SRCDIR}/public/tier1/fmtstr.h"
	"${ENGINE_DIR}/framesnapshot.h"
	"${ENGINE_DIR}/GameEventManager.h"
	"${ENGINE_DIR}/getintersectingsurfaces_struct.h"
	"${ENGINE_DIR}/gl_cvars.h"
	"${ENGINE_DIR}/gl_drawlights.h"
	"${ENGINE_DIR}/gl_lightmap.h"
	"${ENGINE_DIR}/gl_matsysiface.h"
	"${ENGINE_DIR}/gl_model.h"
	"${ENGINE_DIR}/gl_model_private.h"
	"${ENGINE_DIR}/gl_rmain.h"
	"${ENGINE_DIR}/gl_rsurf.h"
	"${ENGINE_DIR}/gl_shader.h"
	"${ENGINE_DIR}/gl_water.h"
	"${ENGINE_DIR}/hltvclient.h"
	"${ENGINE_DIR}/hltvclientstate.h"
	"${ENGINE_DIR}/hltvdemo.h"
	"${ENGINE_DIR}/hltvserver.h"
	"${ENGINE_DIR}/hltvtest.h"
	"${ENGINE_DIR}/host.h"
	"${ENGINE_DIR}/host_cmd.h"
	"${ENGINE_DIR}/host_jmp.h"
	"${ENGINE_DIR}/host_saverestore.h"
	"${ENGINE_DIR}/host_state.h"
	"${SRCDIR}/public/engine/http.h"
	"${SRCDIR}/public/iclient.h"
	"${SRCDIR}/public/icliententity.h"
	"${ENGINE_DIR}/icolorcorrectiontools.h"
	"${SRCDIR}/public/materialsystem/idebugtextureinfo.h"
	"${SRCDIR}/public/idedicatedexports.h"
	"${ENGINE_DIR}/idispinfo.h"
	"$<$<NOT:${IS_DEDICATED}>:${SRCDIR}/common/engine/idownloadsystem.h>"
	"${SRCDIR}/public/engine/IEngineSound.h"
	"${ENGINE_DIR}/ienginetoolinternal.h"
	"${SRCDIR}/public/engine/IEngineTrace.h"
	"${SRCDIR}/common/igameserverdata.h"
	"${SRCDIR}/public/ihltvdirector.h"
	"${SRCDIR}/public/inetchannel.h"
	"${SRCDIR}/public/inetmsghandler.h"
	"${SRCDIR}/public/inetwork.h"
	"${SRCDIR}/public/iscratchpad3d.h"
	"${SRCDIR}/public/iserver.h"
	"${SRCDIR}/public/iserverentity.h"
	"${SRCDIR}/public/engine/iserverplugin.h"
	"${SRCDIR}/public/engine/ishadowmgr.h"
	"${SRCDIR}/public/ispatialpartition.h"
	"${ENGINE_DIR}/ispatialpartitioninternal.h"
	"${SRCDIR}/public/steam/isteamutils.h"
	"${SRCDIR}/public/istudiorender.h"
	"${SRCDIR}/public/ivoicetweak.h"
	"${ENGINE_DIR}/keys.h"
	"${SRCDIR}/public/tier0/l2cache.h"
	"${ENGINE_DIR}/l_studio.h"
	"${ENGINE_DIR}/lightcache.h"
	"${ENGINE_DIR}/LoadScreenUpdate.h"
	"${ENGINE_DIR}/LocalNetworkBackdoor.h"
	#"${ENGINE_DIR}/logofile_shared.h"
	"${ENGINE_DIR}/lowpassstream.h"
	"${ENGINE_DIR}/MapReslistGenerator.h"
	"${ENGINE_DIR}/matchmaking.h"
	"${ENGINE_DIR}/matchmakingqos.h"
	"${ENGINE_DIR}/MaterialBuckets.h"
	"${ENGINE_DIR}/materialproxyfactory.h"
	"${ENGINE_DIR}/mem.h"
	"${ENGINE_DIR}/mem_fgets.h"
	"${ENGINE_DIR}/mod_vis.h"
	"${ENGINE_DIR}/modelloader.h"
	"${SRCDIR}/public/modes.h"
	"${ENGINE_DIR}/net.h"
	"${ENGINE_DIR}/net_chan.h"
	"${ENGINE_DIR}/net_synctags.h"
	"${SRCDIR}/common/netmessages.h"
	"${ENGINE_DIR}/networkstringtable.h"
	"${SRCDIR}/public/networkstringtabledefs.h"
	"${ENGINE_DIR}/networkstringtableitem.h"
	"${ENGINE_DIR}/networkstringtableserver.h"
	"${SRCDIR}/public/nmatrix.h"
	"${SRCDIR}/public/nvector.h"
	"${SRCDIR}/public/optimize.h"
	"${ENGINE_DIR}/Overlay.h"
	"${ENGINE_DIR}/packed_entity.h"
	"${ENGINE_DIR}/pr_edict.h"
	"${ENGINE_DIR}/precache.h"
	"${ENGINE_DIR}/profile.h"
	"${ENGINE_DIR}/profiling.h"
	"${ENGINE_DIR}/progs.h"
	"${SRCDIR}/public/tier0/protected_things.h"
	"${SRCDIR}/common/protocol.h"
	"${ENGINE_DIR}/quakedef.h"
	"${ENGINE_DIR}/r_areaportal.h"
	"${ENGINE_DIR}/r_decal.h"
	"${ENGINE_DIR}/r_efxextern.h"
	"${ENGINE_DIR}/r_local.h"
	"${ENGINE_DIR}/randomnormals.h"
	"${ENGINE_DIR}/render.h"
	"$<$<NOT:${IS_DEDICATED}>:${SRCDIR}/common/engine/requestcontext.h>"
	"${SRCDIR}/public/savegame_version.h"
	"${ENGINE_DIR}/saverestore_filesystem.h"
	"${ENGINE_DIR}/sbar.h"
	"${SRCDIR}/public/scratchpad3d.h"
	"${ENGINE_DIR}/screen.h"
	"${ENGINE_DIR}/server.h"
	"${ENGINE_DIR}/Session.h"
	"${ENGINE_DIR}/shadowmgr.h"
	"${ENGINE_DIR}/audio/snd_mp3_source.h"
	"${ENGINE_DIR}/audio/snd_wave_data.h"
	"${SRCDIR}/public/engine/SndInfo.h"
	"${SRCDIR}/public/soundinfo.h"
	"${ENGINE_DIR}/staticpropmgr.h"
	"${SRCDIR}/public/steam/steam_api.h"
	"${SRCDIR}/public/steam/steam_gameserver.h"
	"${SRCDIR}/public/steam/steamtypes.h"
	"${ENGINE_DIR}/studio_internal.h"
	"${ENGINE_DIR}/surfacehandle.h"
	"${SRCDIR}/public/surfinfo.h"
	"${ENGINE_DIR}/sv_client.h"
	"${ENGINE_DIR}/sv_filter.h"
	"${ENGINE_DIR}/sv_ipratelimit.h"
	"${ENGINE_DIR}/sv_log.h"
	"${ENGINE_DIR}/sv_logofile.h"
	"${ENGINE_DIR}/sv_main.h"
	"${ENGINE_DIR}/sv_packedentities.h"
	"${ENGINE_DIR}/sv_plugin.h"
	"${ENGINE_DIR}/sv_precache.h"
	"${ENGINE_DIR}/sv_rcon.h"
	"${ENGINE_DIR}/sv_remoteaccess.h"
	"${ENGINE_DIR}/sv_steamauth.h"
	"${ENGINE_DIR}/sv_uploaddata.h"
	"${ENGINE_DIR}/sv_uploadgamestats.h"
	"${ENGINE_DIR}/sv_user.h"
	"${ENGINE_DIR}/sys.h"
	"${ENGINE_DIR}/sys_dll.h"
	"${ENGINE_DIR}/sysexternal.h"
	"${ENGINE_DIR}/testscriptmgr.h"
	"${SRCDIR}/public/texture_group_names.h"
	"${ENGINE_DIR}/tmessage.h"
	"${SRCDIR}/public/trace.h"
	"${ENGINE_DIR}/traceinit.h"
	"${SRCDIR}/common/userid.h"
	"${SRCDIR}/public/tier1/utlfixedmemory.h"
	"${SRCDIR}/public/tier1/utlsymbol.h"
	"${ENGINE_DIR}/vengineserver_impl.h"
	"${ENGINE_DIR}/vgui_basepanel.h"
	"${ENGINE_DIR}/vgui_DebugSystemPanel.h"
	"${ENGINE_DIR}/vgui_drawtreepanel.h"
	"${ENGINE_DIR}/vgui_helpers.h"
	"${ENGINE_DIR}/view.h"
	"${ENGINE_DIR}/vprof_engine.h"
	"${ENGINE_DIR}/vprof_record.h"
	"${ENGINE_DIR}/world.h"
	"${ENGINE_DIR}/zone.h"
	"${ENGINE_DIR}/baseautocompletefilelist.h"
	"${ENGINE_DIR}/dt_common_eng.h"
	"${ENGINE_DIR}/engineperftools.h"
	"${SRCDIR}/public/mathlib/IceKey.H"
	"${ENGINE_DIR}/IOcclusionSystem.h"
	"${ENGINE_DIR}/ithread.h"
	"${ENGINE_DIR}/ModelInfo.h"
	"${SRCDIR}/public/mathlib/polyhedron.h"
	"${SRCDIR}/common/proto_oob.h"
	"${SRCDIR}/common/proto_version.h"
	"${ENGINE_DIR}/sys_mainwind.h"
	"${ENGINE_DIR}/cl_demoaction.h"
	"${ENGINE_DIR}/cl_demoaction_types.h"
	"${ENGINE_DIR}/cl_demoactioneditors.h"
	"${ENGINE_DIR}/cl_demoactionmanager.h"
	"${ENGINE_DIR}/cl_demoeditorpanel.h"
	"${ENGINE_DIR}/cl_demosmootherpanel.h"
	"${ENGINE_DIR}/cl_demosmoothing.h"
	"${ENGINE_DIR}/cl_demouipanel.h"
	"${ENGINE_DIR}/cl_foguipanel.h"
	"${ENGINE_DIR}/cl_txviewpanel.h"

	# Public Header Files
	"${SRCDIR}/public/vgui_controls/AnalogBar.h"
	"${SRCDIR}/public/vgui_controls/AnimationController.h"
	"${SRCDIR}/public/mathlib/anorms.h"
	"${SRCDIR}/public/basehandle.h"
	"${SRCDIR}/public/tier0/basetypes.h"
	"${SRCDIR}/public/tier1/bitbuf.h"
	"${SRCDIR}/public/bitvec.h"
	"${SRCDIR}/public/blockingudpsocket.h"
	"${SRCDIR}/public/bspfile.h"
	"${SRCDIR}/public/bspflags.h"
	"${SRCDIR}/public/builddisp.h"
	"${SRCDIR}/public/vgui_controls/BuildGroup.h"
	"${SRCDIR}/public/mathlib/bumpvects.h"
	"${SRCDIR}/public/vgui_controls/Button.h"
	"${SRCDIR}/public/cdll_int.h"
	"${SRCDIR}/public/tier1/characterset.h"
	"${SRCDIR}/public/vgui_controls/CheckButton.h"
	"${SRCDIR}/public/tier1/checksum_crc.h"
	"${SRCDIR}/public/tier1/checksum_md5.h"
	"${SRCDIR}/public/client_class.h"
	"${SRCDIR}/public/client_textmessage.h"
	"${SRCDIR}/public/clientstats.h"
	"${SRCDIR}/public/cmodel.h"
	"${SRCDIR}/public/Color.h"
	"${SRCDIR}/public/vgui_controls/ComboBox.h"
	"${SRCDIR}/public/tier0/commonmacros.h"
	"${SRCDIR}/public/mathlib/compressed_vector.h"
	"${SRCDIR}/public/con_nprint.h"
	"${SRCDIR}/public/const.h"
	"${SRCDIR}/public/vgui_controls/Controls.h"
	"${SRCDIR}/public/tier1/convar.h"
	"${SRCDIR}/public/coordsize.h"
	"${SRCDIR}/public/crtmemdebug.h"
	"${SRCDIR}/common/cserserverprotocol_engine.h"
	"${SRCDIR}/public/vgui/Cursor.h"
	"${SRCDIR}/public/vgui/Dar.h"
	"${SRCDIR}/public/datamap.h"
	"${SRCDIR}/public/tier0/dbg.h"
	"${SRCDIR}/public/disp_vertindex.h"
	"${SRCDIR}/public/dlight.h"
	"${SRCDIR}/public/dt_utlvector_recv.h"
	"${SRCDIR}/public/dt_utlvector_send.h"
	"${SRCDIR}/public/edict.h"
	"${SRCDIR}/public/vgui_controls/EditablePanel.h"
	"${SRCDIR}/public/editor_sendcommand.h"
	"${SRCDIR}/public/eiface.h"
	"${SRCDIR}/public/event_flags.h"
	"${ENGINE_DIR}/event_system.h"
	"${SRCDIR}/public/tier0/fasttimer.h"
	"${SRCDIR}/public/vgui_controls/FileOpenDialog.h"
	"${SRCDIR}/public/filesystem_helpers.h"
	"${SRCDIR}/public/vgui_controls/FocusNavGroup.h"
	"${SRCDIR}/public/vgui_controls/Frame.h"
	"${SRCDIR}/public/gamebspfile.h"
	"${SRCDIR}/public/gametrace.h"
	"${SRCDIR}/public/globalvars_base.h"
	"${SRCDIR}/public/appframework/IAppSystem.h"
	"${SRCDIR}/public/ibsppack.h"
	"${SRCDIR}/public/icliententitylist.h"
	"${SRCDIR}/public/engine/IClientLeafSystem.h"
	"${SRCDIR}/public/iclientnetworkable.h"
	"${SRCDIR}/public/vgui/IClientPanel.h"
	"${SRCDIR}/public/iclientrenderable.h"
	"${SRCDIR}/public/iclientthinkable.h"
	"${SRCDIR}/public/iclientunknown.h"
	"${SRCDIR}/public/engine/ICollideable.h"
	"${SRCDIR}/public/tier0/icommandline.h"
	"${SRCDIR}/public/icvar.h"
	"${SRCDIR}/public/iefx.h"
	"${ENGINE_DIR}/iengine.h"
	"${SRCDIR}/public/ienginevgui.h"
	"${ENGINE_DIR}/igame.h"
	"${SRCDIR}/common/GameUI/IGameConsole.h"
	"${SRCDIR}/public/igameevents.h"
	"${SRCDIR}/common/GameUI/IGameUI.h"
	"${SRCDIR}/public/IGameUIFuncs.h"
	"${SRCDIR}/public/ihandleentity.h"
	"${SRCDIR}/public/ihltv.h"
	"${SRCDIR}/public/vgui/IHTML.h"
	"${SRCDIR}/public/vgui/IImage.h"
	"${SRCDIR}/public/vgui/IInput.h"
	"${SRCDIR}/public/vstdlib/IKeyValuesSystem.h"
	"${SRCDIR}/public/vgui/ILocalize.h"
	"${SRCDIR}/public/vgui_controls/Image.h"
	"${SRCDIR}/public/vgui_controls/ImageList.h"
	"${SRCDIR}/public/engine/imatchmaking.h"
	"${SRCDIR}/public/materialsystem/imaterial.h"
	"${SRCDIR}/public/materialsystem/imaterialproxy.h"
	"${SRCDIR}/public/materialsystem/imaterialproxyfactory.h"
	"${SRCDIR}/public/materialsystem/imaterialsystem.h"
	"${SRCDIR}/public/materialsystem/imaterialsystemhardwareconfig.h"
	"${SRCDIR}/public/materialsystem/imaterialvar.h"
	"${SRCDIR}/public/VGuiMatSurface/IMatSystemSurface.h"
	"${SRCDIR}/public/materialsystem/imesh.h"
	"${SRCDIR}/public/inetchannelinfo.h"
	"${SRCDIR}/public/inetmessage.h"
	"${SRCDIR}/public/tier1/interface.h"
	"${SRCDIR}/public/vgui/IPanel.h"
	"${SRCDIR}/public/iprediction.h"
	"${SRCDIR}/public/irecipientfilter.h"
	"${SRCDIR}/public/iregistry.h"
	"${SRCDIR}/public/isaverestore.h"
	"${SRCDIR}/public/vgui/IScheme.h"
	"${SRCDIR}/public/iservernetworkable.h"
	"${SRCDIR}/public/iserverunknown.h"
	"${SRCDIR}/public/engine/IStaticPropMgr.h"
	"${SRCDIR}/public/steam/isteamclient.h"
	"${SRCDIR}/public/steam/isteamgameserver.h"
	"${SRCDIR}/public/steam/isteamuser.h"
	"${SRCDIR}/public/steam/isteamuserstats.h"
	"${SRCDIR}/public/steam/isteamfriends.h"
	"${SRCDIR}/public/steam/isteammatchmaking.h"
	"${SRCDIR}/public/vgui/ISurface.h"
	"${SRCDIR}/public/vgui/ISystem.h"
	"${SRCDIR}/public/materialsystem/itexture.h"
	"${SRCDIR}/public/vaudio/ivaudio.h"
	"${SRCDIR}/public/engine/ivdebugoverlay.h"
	"${SRCDIR}/public/vgui/IVGui.h"
	"${SRCDIR}/public/ivguicenterprint.h"
	"${SRCDIR}/common/IVguiModule.h"
	"${ENGINE_DIR}/ivideomode.h"
	"${SRCDIR}/public/engine/ivmodelinfo.h"
	"${SRCDIR}/public/engine/ivmodelrender.h"
	"${SRCDIR}/public/ivoiceserver.h"
	"${SRCDIR}/common/ivprofexport.h"
	"${SRCDIR}/public/ivrenderview.h"
	"${SRCDIR}/public/ivtex.h"
	"$<${IS_X360}:${SRCDIR}/public/ixboxsystem.h>"
	"${SRCDIR}/public/vgui/KeyCode.h"
	"${SRCDIR}/public/tier1/KeyValues.h"
	"${SRCDIR}/public/vgui_controls/Label.h"
	"${SRCDIR}/public/vgui_controls/ListPanel.h"
	"${SRCDIR}/public/lumpfiles.h"
	"${SRCDIR}/public/materialsystem/materialsystem_config.h"
	"${SRCDIR}/public/mathlib/mathlib.h"
	"${SRCDIR}/public/measure_section.h"
	"${SRCDIR}/public/tier0/mem.h"
	"${SRCDIR}/public/tier0/memalloc.h"
	"${SRCDIR}/public/tier0/memdbgoff.h"
	"${SRCDIR}/public/tier0/memdbgon.h"
	"${SRCDIR}/public/tier1/mempool.h"
	"${SRCDIR}/public/tier1/memstack.h"
	"${SRCDIR}/public/vgui_controls/Menu.h"
	"${SRCDIR}/public/vgui_controls/MenuButton.h"
	"${SRCDIR}/public/vgui_controls/MenuItem.h"
	"${SRCDIR}/public/vgui_controls/MessageMap.h"
	"${SRCDIR}/public/model_types.h"
	"${SRCDIR}/public/vgui/MouseCode.h"
	"${SRCDIR}/public/mouthinfo.h"
	"${SRCDIR}/public/tier1/netadr.h"
	"${ENGINE_DIR}/networkstringtableclient.h"
	"${SRCDIR}/public/networkvar.h"
	"${SRCDIR}/public/overlaytext.h"
	"${SRCDIR}/public/vgui_controls/Panel.h"
	"${SRCDIR}/public/vgui_controls/PanelAnimationVar.h"
	"${SRCDIR}/public/vgui_controls/PHandle.h"
	"${SRCDIR}/public/phyfile.h"
	"${SRCDIR}/public/tier0/platform.h"
	"${SRCDIR}/public/PlayerState.h"
	"${SRCDIR}/public/vgui_controls/ProgressBar.h"
	"${SRCDIR}/public/vgui_controls/PropertyDialog.h"
	"${SRCDIR}/public/vgui_controls/PropertyPage.h"
	"${SRCDIR}/public/vgui_controls/PropertySheet.h"
	"${SRCDIR}/public/renamed_recvtable_compat.h"
	"${SRCDIR}/common/qlimits.h"
	"${SRCDIR}/public/r_efx.h"
	"${SRCDIR}/public/vstdlib/random.h"
	"${SRCDIR}/public/saverestoretypes.h"
	"${SRCDIR}/public/vgui_controls/ScrollBar.h"
	"${SRCDIR}/public/sentence.h"
	"${SRCDIR}/public/server_class.h"
	"${SRCDIR}/public/shake.h"
	"${SRCDIR}/public/vgui_controls/Slider.h"
	"${SRCDIR}/public/tier1/smartptr.h"
	"${SRCDIR}/public/smooth_average.h"
	"${SRCDIR}/public/soundchars.h"
	"${SRCDIR}/public/soundflags.h"
	"${SRCDIR}/public/stdstring.h"
	"${SRCDIR}/public/steam/steamclientpublic.h"
	"${SRCDIR}/common/SteamCommon.h"
	"${SRCDIR}/public/string_t.h"
	"${SRCDIR}/public/tier1/strtools.h"
	"${SRCDIR}/public/studio.h"
	"${SRCDIR}/public/vgui_controls/TextEntry.h"
	"${SRCDIR}/public/vgui_controls/TextImage.h"
	"${SRCDIR}/public/vgui_controls/ToggleButton.h"
	"${SRCDIR}/public/vgui_controls/Tooltip.h"
	"${SRCDIR}/public/vgui_controls/TreeView.h"
	"${SRCDIR}/public/vgui_controls/TreeViewListControl.h"
	"${SRCDIR}/public/unicode/unicode.h"
	"${SRCDIR}/public/tier1/utlbidirectionalset.h"
	"${SRCDIR}/public/tier1/utlbuffer.h"
	"${SRCDIR}/public/UtlCachedFileData.h"
	"${SRCDIR}/public/tier1/utldict.h"
	"${SRCDIR}/public/tier1/utlhash.h"
	"${SRCDIR}/public/tier1/utllinkedlist.h"
	"${SRCDIR}/public/tier1/utlmemory.h"
	"${SRCDIR}/public/tier1/utlmultilist.h"
	"${SRCDIR}/public/tier1/utlrbtree.h"
	"${SRCDIR}/public/tier1/UtlSortVector.h"
	"${SRCDIR}/public/tier1/utlvector.h"
	"${SRCDIR}/public/vcollide.h"
	"${SRCDIR}/public/vcollide_parse.h"
	"${SRCDIR}/public/mathlib/vector.h"
	"${SRCDIR}/public/mathlib/vector2d.h"
	"${SRCDIR}/public/mathlib/vector4d.h"
	"${SRCDIR}/public/vgui/VGUI.h"
	"${SRCDIR}/public/view_shared.h"
	"${SRCDIR}/public/mathlib/vmatrix.h"
	"${ENGINE_DIR}/vmodes.h"
	"${SRCDIR}/public/vphysics_interface.h"
	"${SRCDIR}/public/mathlib/vplane.h"
	"${SRCDIR}/public/tier0/vprof.h"
	"${SRCDIR}/public/vstdlib/vstdlib.h"
	"${SRCDIR}/public/vtf/vtf.h"
	"${SRCDIR}/public/worldsize.h"
	"${SRCDIR}/public/zip/XZip.h"
	"${SRCDIR}/public/zip_uncompressed.h"
	"${SRCDIR}/public/xwvfile.h"
)

add_library(
	engine SHARED
	${ENGINE_CLIENT_SOURCES}
	${ENGINE_SERVER_SOURCES}
	${ENGINE_SOURCES}
	${ENGINE_AUDIO_SOURCES}
	"$<$<NOT:${IS_DEDICATED}>:${ENGINE_VGUI_PANELS_SOURCES}>"
	${ENGINE_TOOLS_FRAMEWORK_SOURCES}
	${ENGINE_HEADERS}
)

set_property(
	TARGET engine PROPERTY FOLDER "Shared Libs"
)

set_install_properties(
	engine
	""
	"engine"
	"${GAMEDIR}/bin"
)

# CMAKETODO(SanyaSho): FIXME!
#target_precompile_headers(
#	engine
#	PRIVATE "$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/client_pch.h>"
#	PRIVATE "${ENGINE_DIR}/render_pch.h"
#	PRIVATE "${ENGINE_DIR}/server_pch.h"
#	PRIVATE "$<$<NOT:${IS_DEDICATED}>:${ENGINE_DIR}/audio/audio_pch.h>"
#)

target_include_directories(
	engine PRIVATE

	"${SRCDIR}/engine"
	"${SRCDIR}/engine/audio"
	"${SRCDIR}/public/engine/audio"
	"${SRCDIR}/vgui2/include"
	"${SRCDIR}/vgui2/controls"
	"$<${IS_WINDOWS}:${DX9SDKDIR}/Include>"
	"$<$<OR:${USE_SDL},${IS_DEDICATED}>:${THIRDPARTYDIR}/SDL>"
	"$<${ENGINE_GPROFILER}:${THIRDPARTYDIR}/gperftools-2.0/src>"
	"${THIRDPARTYDIR}/libjpeg-turbo-3.0.1"
)

# CMAKETODO: /fp:precise
target_compile_definitions(
	engine PRIVATE

	USE_CONVARS
	VOICE_OVER_IP
	BUMPMAP
	__USEA3D
	_ADD_EAX_
	ENGINE_DLL
	VERSION_SAFE_STEAM_API_INTERFACES
	USE_BREAKPAD_HANDLER
	"$<${IS_DEDICATED}:DEDICATED>"
	"$<${IS_DEDICATED}:SWDS>"
	"$<${IS_DEDICATED}:NO_STEAM>" # SanyaSho: required for nillerusr's engine
	"$<${IS_LINUX}:USE_WEBM_FOR_REPLAY>"
	"$<${ENGINE_GPROFILER}:GPROFILER>"
	"$<${ENGINE_ENABLE_RPT}:ENABLE_RPT>"
)

if( ${IS_CLANGCL} )
target_compile_options(
	engine PRIVATE

	/EHsc
)
endif()

target_link_libraries(
	engine PRIVATE

	"$<${IS_WINDOWS}:dinput8>"
	"$<${IS_WINDOWS}:winmm>"
	"$<${IS_WINDOWS}:wsock32>"
	"$<${IS_WINDOWS}:ws2_32>"
	"$<${IS_WINDOWS}:wininet>"
	"$<${IS_WINDOWS}:vfw32>"
	"$<${IS_WINDOWS}:Rpcrt4>"

	"$<${IS_OSX}:-framework Carbon>"
	"$<${IS_OSX}:-framework AppKit>"
	"$<${IS_OSX}:-framework OpenAL>"
	"$<${IS_OSX}:-framework CoreAudio>"
	"$<${IS_OSX}:-framework AudioToolbox>"
	"$<${IS_OSX}:-framework SystemConfiguration>"
	"$<${IS_OSX}:-framework AudioUnit>"

	#"$<${AND:${IS_WINDOWS},${STAGING_ONLY}}:socketlib>"

	appframework
	vtf
	bitmap
	bz2
	#"${LIBCOMMON}/libbz2${STATIC_LIB_EXT}"
	"$<$<AND:$<NOT:${IS_DEDICATED}>,${IS_WINDOWS}>:${LIBCOMMON}/libjpeg${STATIC_LIB_EXT}>"
	"$<$<AND:$<NOT:${IS_DEDICATED}>,${IS_POSIX}>:jpeg>"

	dmxloader
	mathlib
	"$<$<NOT:${IS_DEDICATED}>:matsys_controls>"
	tier2
	tier3
	"$<$<NOT:${IS_DEDICATED}>:vgui_controls>"

	"$<${IS_OSX}:${LIBCOMMON}/curl${IMPLIB_EXT}>"

	#libz${STATIC_LIB_EXT}
	z
	# libcrypto

	"$<${ENGINE_GPROFILER}:${THIRDPARTYDIR}/gperftools-2.0/.libs/libprofiler${SHARED_LIB_EXT}>"
	"$<${IS_WINDOWS}:dsound>"
	"$<${IS_WINDOWS}:dxguid>"

	"$<$<AND:${IS_LINUX},$<NOT:${IS_ANDROID}>>:openal>"
	"$<$<AND:${IS_LINUX},$<NOT:${IS_ANDROID}>>:${CURL_LIBRARIES}>"
	"$<$<AND:${IS_ANDROID},$<NOT:${IS_64BIT}>>:ssl>"
	"$<$<AND:${IS_ANDROID},$<NOT:${IS_64BIT}>>:crypto>"
	"$<${IS_ANDROID}:curl>"
)

target_include_crypto(
	engine
)

target_use_steamapi(
	engine
)

if( ${USE_SDL} )
	target_use_sdl2(
		engine
	)
endif()
