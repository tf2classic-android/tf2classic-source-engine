//=============================================================================
// 
// Purpose: Team Fortress 2 Classic reverse-engineered music controller
// 
// Author: SanyaSho (2024)
// 
//=============================================================================

#include "cbase.h"
#include "GameEventListener.h"
#include "iclientnetworkable.h"
#include "igamesystem.h"
#include "shareddefs.h"
#include "soundenvelope.h"
#include "tf_gamerules.h"
#include "tf_music_controller.h"
#include "datamap.h"
#include "filesystem.h"

#if defined( CLIENT_DLL )
#include "client_thinklist.h"
#else
#endif

#include "tier0/memdbgon.h"

#if defined( CLIENT_DLL )
CTFMusicController *g_pMusicController;

void ToggleMusic( IConVar *pCVar, const char *pzsOld, float flOld )
{
	if( g_pMusicController )
	{
		ConVar *pCvar = (ConVar *)pCVar;
		g_pMusicController->ToggleMusicEnabled( pCvar->GetBool() );
	}
}
ConVar tf2c_music( "tf2c_music", "1", FCVAR_ARCHIVE, "Enable music in Deathmatch.", ToggleMusic ); // SanyaSho: 0 by default

ConVar tf2c_music_volume( "tf2c_music_volume", "1.0", FCVAR_ARCHIVE, "", true, 0.0f, true, 1.0f );
ConVar tf2c_music_volume_dead( "tf2c_music_volume_dead", "0.25", FCVAR_ARCHIVE, "", true, 0.0f, true, 1.0f );
#endif

const char *g_aMusicSegmentNames[TF_NUM_MUSIC_SEGMENTS] = 
{
	"loop",
	"waiting",
	"ending"
};

BEGIN_DATADESC( CTFMusicController )
#if defined( GAME_DLL )
	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputSetEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputSetDisable ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "SetTrack", InputSetTrack ),
	DEFINE_INPUTFUNC( FIELD_VOID, "SetRandomTrack", InputSetRandomTrack ),
	
	DEFINE_KEYFIELD( m_iTrack, FIELD_INTEGER, "tracknum" ),
#endif
END_DATADESC()

IMPLEMENT_NETWORKCLASS_ALIASED( TFMusicController, DT_TFMusicController )

BEGIN_NETWORK_TABLE( CTFMusicController, DT_TFMusicController )
#if defined( CLIENT_DLL )
	RecvPropInt( RECVINFO( m_iTrack ) ),
	RecvPropBool( RECVINFO( m_bShouldPlay ) ),
#else
	SendPropInt( SENDINFO( m_iTrack ) ),
	SendPropBool( SENDINFO( m_bShouldPlay ) ),
#endif
END_NETWORK_TABLE()

LINK_ENTITY_TO_CLASS( tf_music_controller, CTFMusicController );
PRECACHE_REGISTER( tf_music_controller );

CTFMusicController::CTFMusicController() : m_Tracks( 0, 0, DefLessFunc( int ) )
{
#if defined( CLIENT_DLL )
	m_iTrack = 1;
	m_bShouldPlay = false;
	m_iPlayingTrack = 0;
	m_bPlaying = false;
	m_bInWaiting = false;
#else
	m_iTrack = 0;
	m_bShouldPlay = false;
#endif
}

CTFMusicController::~CTFMusicController()
{

}

void CTFMusicController::Spawn()
{
	Precache();
#if defined( GAME_DLL )
	m_bShouldPlay = !HasSpawnFlags( SF_MUSICCONTROLLER_SILENT );
#endif
	BaseClass::Spawn();
}

void CTFMusicController::Precache()
{
	ParseMusicData( TF_MUSIC_CONTROLLER_FILE );
#if defined( GAME_DLL )
	PrecacheTrack( m_iTrack );
#endif
}

void CTFMusicController::ParseMusicData( const char *pszFile )
{
	KeyValues *pKV = new KeyValues( "Music" );
	if( !pKV )
	{
		// impossible
		return;
	}
	
	if( pKV->LoadFromFile( filesystem, pszFile, "MOD" ) )
	{
		for( KeyValues *pData = pKV->GetFirstSubKey(); pData != NULL; pData = pData->GetNextKey() )
		{
			int iTrackID = V_atoi( pData->GetName() );
			if( iTrackID == 0 )
			{
				continue;
			}
			
			MusicData_t musicData;
			musicData.szName[0] = '\0';
			musicData.szComposer[0] = '\0';
			musicData.szIcon[0] = '\0';
			memset( musicData.aSegments, 0, sizeof( musicData.aSegments ) );
			
			V_strncpy( musicData.szName, pData->GetString( "name" ), sizeof( musicData.szName ) );
			V_strncpy( musicData.szComposer, pData->GetString( "composer" ), sizeof( musicData.szComposer ) );
			V_strncpy( musicData.szIcon, pData->GetString( "icon" ), sizeof( musicData.szIcon ) );
			KeyValues *pSegmentsKey = pData->FindKey( "segments" );
			if( pSegmentsKey )
			{
				for( int x = 0; x < TF_NUM_MUSIC_SEGMENTS; x++ )
				{
					V_strncpy( musicData.aSegments[x], pSegmentsKey->GetString( g_aMusicSegmentNames[x] ), MAX_PATH );
				}
			}
			
			m_Tracks.Insert( iTrackID, musicData );
		}
	}
	else
	{
		Warning( "CTFMusicController: Failed to load music data from %s!\n", pszFile );
	}
	
	pKV->deleteThis();
}

#if defined( CLIENT_DLL )
// Client only
void CTFMusicController::ClientThink()
{
	CBasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if( !pPlayer )
	{
		return;
	}
	
	// no sound?
	if( !m_pSound )
	{
		return;
	}
	
	CSoundEnvelopeController &controller = CSoundEnvelopeController::GetController();
	
	float flOldVolume = controller.SoundGetVolume( m_pSound );
	float flVolume = tf2c_music_volume.GetFloat();

	// dead
	if( !pPlayer->IsAlive() )
	{
		flVolume = flVolume * tf2c_music_volume_dead.GetFloat();
	}
	
	if( flVolume != flOldVolume )
	{
		controller.SoundChangeVolume( m_pSound, flVolume, 0.5f );
	}
}

void CTFMusicController::OnPreDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnPreDataChanged( updateType );
	m_bOldShouldPlay = m_bShouldPlay;
}

void CTFMusicController::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );
	
	if( updateType != DATA_UPDATE_CREATED ) // probably something with datatable changed event
	{
		if( m_iTrack != m_iPlayingTrack )
		{
			RestartMusic();
		}
		
		if( m_bShouldPlay != m_bOldShouldPlay )
		{
			ToggleMusicEnabled( m_bShouldPlay );
		}	
	}
	else if( !g_pMusicController )
	{
		g_pMusicController = this;
		
		// for some reason listeners are here.
		ListenForGameEvent( "localplayer_changeteam" );
		ListenForGameEvent( "teamplay_update_timer" );
		ListenForGameEvent( "deathmatch_results" );
		ListenForGameEvent( "tf_game_over" );
		ListenForGameEvent( "teamplay_game_over" );
		ListenForGameEvent( "teamplay_round_win" );
	}
}
	
void CTFMusicController::StartMusic()
{
	// SanyaSho: don't try to play anything in non-dm
	if( !TFGameRules()->IsDeathmatch() )
	{
		return;
	}
	
	if( m_bPlaying )
	{
		return;
	}
	
	m_bInWaiting = ShouldPlayWaitingMusic();
	
	if( CanPlayMusic() && C_BasePlayer::GetLocalPlayer() )
	{
		m_iPlayingTrack = m_iTrack;
		
		CSoundEnvelopeController &controller = CSoundEnvelopeController::GetController();
		
		unsigned short nTrack = m_Tracks.Find( m_iPlayingTrack );
		
		if( nTrack == m_Tracks.InvalidIndex() )
		{
			Warning( "C_TFMusicController: Attempted to play unknown track %d!\n", m_iPlayingTrack );
		}
		else
		{
			MusicData_t musicData = m_Tracks.Element( nTrack );
			if( musicData.aSegments[m_bInWaiting][0] )
			{
				CLocalPlayerFilter filter;
				m_pSound = controller.SoundCreate( filter, entindex(), musicData.aSegments[m_bInWaiting] );
				
				controller.Play( m_pSound, tf2c_music_volume.GetFloat(), 100.f, 0.f );
				
				if( !m_bInWaiting )
				{
					IGameEvent *pEvent = gameeventmanager->CreateEvent( "song_started" );
					if( pEvent )
					{
						pEvent->SetString( "name", musicData.szName );
						pEvent->SetString( "composer", musicData.szComposer );
						pEvent->SetString( "icon", musicData.szIcon );
						gameeventmanager->FireEventClientSide( pEvent );
					}
				}
				
				m_bPlaying = true;
				SetNextClientThink( CLIENT_THINK_ALWAYS );
			}
		}
	}
}

void CTFMusicController::StopMusic( bool bPlayEnding )
{
	// SanyaSho: don't try to play anything in non-dm
	if( !TFGameRules()->IsDeathmatch() )
	{
		return;
	}

	if( !m_bPlaying )
	{
		return;
	}
	
	CSoundEnvelopeController &controller = CSoundEnvelopeController::GetController();
	
	if( m_pSound )
	{
		controller.SoundDestroy( m_pSound );
		m_pSound = NULL;
	}
	
	if( bPlayEnding )
	{
		if( m_bInWaiting )
		{
			m_bPlaying = false;
			SetNextClientThink( CLIENT_THINK_NEVER );
			return;
		}
		
		unsigned short nTrack = m_Tracks.Find( m_iPlayingTrack );
		
		if( nTrack != m_Tracks.InvalidIndex() )
		{
			MusicData_t musicData = m_Tracks.Element( nTrack );
			const char *pszEndingTrack = musicData.aSegments[TF_MUSIC_SEGMENT_ENDING];
			if( pszEndingTrack[0] )
			{
				EmitSound_t params;
				params.m_pSoundName = pszEndingTrack;
				CSoundParameters soundParameters;
				if( V_stristr( pszEndingTrack, ".wav" ) || V_stristr( pszEndingTrack, ".mp3" ) || !GetParametersForSound( pszEndingTrack, soundParameters, NULL ) )
				{
					params.m_nChannel = CHAN_STATIC;
					params.m_SoundLevel = SNDLVL_NONE;
					params.m_flVolume = tf2c_music_volume.GetFloat();
				}
				else
				{
					params.m_flVolume = tf2c_music_volume.GetFloat() * soundParameters.volume;
					params.m_nFlags |= SND_CHANGE_VOL;
				}
				
				CLocalPlayerFilter filter;
				EmitSound( filter, entindex(), params );
			}
		}
	}
	
	if( !m_bInWaiting )
	{
		IGameEvent *pEvent = gameeventmanager->CreateEvent( "song_stopped" );
		if( pEvent )
		{
			gameeventmanager->FireEventClientSide( pEvent );
		}
	}
	
	m_bPlaying = false;
	SetNextClientThink( CLIENT_THINK_NEVER );
}

void CTFMusicController::ToggleMusicEnabled( bool bEnable )
{
	if( bEnable )
	{
		StartMusic();
	}
	else
	{
		StopMusic( false );
	}
}

void CTFMusicController::RestartMusic()
{
	StopMusic( false );
	StartMusic();
}

void CTFMusicController::FireGameEvent( IGameEvent *pEvent )
{
	// SanyaSho: don't try to play anything in non-dm
	if( !TFGameRules()->IsDeathmatch() )
	{
		return;
	}
	
	CBasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if( !pPlayer )
	{
		return;
	}
	
	const char *pszEvent = pEvent->GetName();
	if( !Q_stricmp( pszEvent, "localplayer_changeteam" ) )
	{
		if( pPlayer->GetTeamNumber() == TEAM_SPECTATOR ) // should be <=
		{
			// player switched to spectator or TEAM_UNASSIGNED
			StopMusic( true );
			return;
		}
		StartMusic();
	}
	else if( !Q_stricmp( pszEvent, "deathmatch_results" ) || !Q_stricmp( pszEvent, "tf_game_over" ) || !Q_stricmp( pszEvent, "teamplay_game_over" ) || !Q_stricmp( pszEvent, "teamplay_round_win" ) )
	{
		StopMusic( true );
		m_bPlayingEnding = true; // FIXME(SanyaSho): should we move this directly to StopMusic()?
	}
	else if( !Q_stricmp( pszEvent, "teamplay_update_timer" ) )
	{
		if( m_bInWaiting != ShouldPlayWaitingMusic() )
		{
			RestartMusic();
		}
	}
}

void CTFMusicController::UpdateOnRemove()
{
	StopMusic( false );
	g_pMusicController = NULL;
	BaseClass::UpdateOnRemove();
}
#else
// Server only
void CTFMusicController::InputSetEnable( inputdata_t &inputdata )
{
	if( !m_bShouldPlay )
	{
		m_bShouldPlay = true;
	}
}

void CTFMusicController::InputSetDisable( inputdata_t &inputdata )
{
	if( m_bShouldPlay )
	{
		m_bShouldPlay = false;
	}
}

void CTFMusicController::InputSetTrack( inputdata_t &inputdata )
{
	int iTrack = inputdata.value.Int();
	SetTrack( iTrack );
}

void CTFMusicController::SetTrack( int iTrack )
{
	if( iTrack != m_iTrack )
	{
		m_iTrack = iTrack;
		PrecacheTrack( iTrack );
	}
}

void CTFMusicController::InputSetRandomTrack( inputdata_t &inputdata )
{
	SelectRandomTrack();
}

void CTFMusicController::SelectRandomTrack()
{
	unsigned short nTrack = m_Tracks.Find( m_Tracks.Key( RandomInt( 1, m_Tracks.Count() - 1 ) ) );

	//Msg( __FUNCTION__ ": Selected random track %d from m_Tracks list.\n", nTrack );

	if( nTrack != m_Tracks.InvalidIndex() )
	{
		SetTrack( nTrack );
	}
}

int CTFMusicController::UpdateTransmitState()
{
	return BaseClass::SetTransmitState( FL_EDICT_ALWAYS );
}

void CTFMusicController::PrecacheTrack( int iTrack )
{
	unsigned short nTrack = m_Tracks.Find( iTrack );
		
	if( nTrack != m_Tracks.InvalidIndex() )
	{
		MusicData_t musicData = m_Tracks.Element( nTrack );
		for( int i = 0; i < TF_NUM_MUSIC_SEGMENTS; i++ )
		{
			const char *pszElement = musicData.aSegments[i];
			if( !pszElement[0] )
				continue;
			
			PrecacheScriptSound( pszElement );
		}
	}
}

int CTFMusicController::DrawDebugTextOverlays()
{
	// SanyaSho: don't try to play anything in non-dm
	if( !TFGameRules()->IsDeathmatch() )
	{
		return BaseClass::DrawDebugTextOverlays();
	}

	int text_offset = BaseClass::DrawDebugTextOverlays();
	
	if( m_debugOverlays & OVERLAY_TEXT_BIT ) 
	{
		char tempstr[512];
		V_sprintf_safe( tempstr, "Track: %d", m_iTrack.Get() );
		EntityText( text_offset, tempstr, 0 );
		text_offset++;
		
		V_sprintf_safe( tempstr, "Should play: %s", m_bShouldPlay ? "Yes" : "No" );
		EntityText( text_offset, tempstr, 0 );
		text_offset++;
	}
	
	return text_offset;
}
#endif

