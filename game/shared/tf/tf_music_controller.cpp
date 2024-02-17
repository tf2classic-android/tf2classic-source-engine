#include "cbase.h"
#include "GameEventListener.h"
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
		if( pCvar->GetBool() )
		{
			g_pMusicController->StartMusic();
		}
		else
		{
			g_pMusicController->StopMusic( false );
		}
	}
}
ConVar tf2c_music( "tf2c_music", "0", FCVAR_ARCHIVE, "Enable music in Deathmatch.", ToggleMusic );

ConVar tf2c_music_volume( "tf2c_music_volume", "1.0", FCVAR_ARCHIVE, "", true, 0.0f, true, 1.0f );
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
	m_iTrack = 1; // default: 0
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
	m_bShouldPlay = HasSpawnFlags( SF_MUSICCONTROLLER_SHOULDPLAY );
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
			memset( musicData.aSegments, 0, sizeof( musicData.aSegments ) );
			
			V_strncpy( musicData.szName, pData->GetString( "name" ), sizeof( musicData.szName ) );
			V_strncpy( musicData.szComposer, pData->GetString( "composer" ), sizeof( musicData.szComposer ) );
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
		Warning( "C_TFMusicController: Failed to load music data from %s!\n", pszFile );
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
	float flNewVolume = 0.f;
	
	// dead
	if( !pPlayer->IsAlive() )
	{
		flNewVolume = flVolume * 0.25f;
		flVolume = flVolume * 0.25f;
	}
	
	if( flNewVolume != flOldVolume )
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
	
	if( !g_pMusicController )
	{
		g_pMusicController = this;
		
		// for some reason listeners are here.
		ListenForGameEvent( "localplayer_changeteam" );
		ListenForGameEvent( "teamplay_update_timer" );
		ListenForGameEvent( "deathmatch_results" );
	}
	
	StopMusic( false );
	StartMusic();
}
	
void CTFMusicController::StartMusic()
{
	if( m_bPlaying )
	{
		return;
	}
	
	m_bInWaiting = TFGameRules() && ( TFGameRules()->IsInWaitingForPlayers() || TFGameRules()->InRoundRestart() );
	
	if( (g_pMusicController == this) && tf2c_music.GetBool() && m_bShouldPlay && (GetLocalPlayerTeam() >= FIRST_GAME_TEAM) &&
	    (TFGameRules()->State_Get() != GR_STATE_GAME_OVER) && C_BasePlayer::GetLocalPlayer() )
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
			const char *pszEndingTrack = musicData.aSegments[2];
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
					params.m_nFlags |= 1u;
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

void CTFMusicController::FireGameEvent( IGameEvent *pEvent )
{
	CBasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if( !pPlayer )
	{
		return;
	}
	
	const char *pszEvent = pEvent->GetName();
	if( !Q_stricmp( pszEvent, "localplayer_changeteam" ) )
	{
		int nTeamNumber = pPlayer->GetTeamNumber();
		if( nTeamNumber < FIRST_GAME_TEAM )
		{
			// ded
			StopMusic( true );
			return;
		}
		StartMusic();
	}
	else if( !Q_stricmp( pszEvent, "teamplay_update_timer" ) || !Q_stricmp( pszEvent, "deathmatch_results" ) )
	{
		StopMusic( true );
		// BYTE5(this->m_pfnThink) = 1; // probably SetNextClientThink
	}
	else
	{
		/*
		v8 = LOBYTE(g_pGameRules[5].m_pNext) || BYTE1(g_pGameRules[4].m_pNext);
		if ( BYTE6(this->m_pfnThink) != v8 )
		{
			C_TFMusicController::StopMusic((C_TFMusicController *)((char *)this - 1320), 0);
			pMusicController = (C_TFMusicController *)((char *)this - 1320);
LABEL_16:
			C_TFMusicController::StartMusic(pMusicController);
		}
		*/
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
	if( iTrack != m_iTrack )
	{
		m_iTrack = iTrack;
		PrecacheTrack( iTrack );
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
#endif

