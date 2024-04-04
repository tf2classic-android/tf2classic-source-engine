//=============================================================================
// 
// Purpose: Team Fortress 2 Classic reverse-engineered music controller
// 
// Author: SanyaSho (2024)
// 
//=============================================================================

#if !defined( TF_MUSIC_CONTROLLER_H )
#define TF_MUSIC_CONTROLLER_H

#if defined( CLIENT_DLL )
#include "c_baseentity.h"
#else
#include "baseentity.h"
#endif

#include "utllinkedlist.h"
#include "tf_gamerules.h"

enum
{
	TF_MUSIC_SEGMENT_LOOP = 0,
	TF_MUSIC_SEGMENT_WAITING,
	TF_MUSIC_SEGMENT_ENDING,
	
	TF_NUM_MUSIC_SEGMENTS
};

struct MusicData_t
{
	char szName[128];
	char szComposer[128];
	char szIcon[128];
	char aSegments[TF_NUM_MUSIC_SEGMENTS][MAX_PATH];
};

extern const char *g_aMusicSegmentNames[TF_NUM_MUSIC_SEGMENTS];

extern ConVar tf2c_music;

#if defined( CLIENT_DLL )
#define CTFMusicController C_TFMusicController
#endif

#define SF_MUSICCONTROLLER_SILENT 0x1

#define TF_MUSIC_CONTROLLER_FILE "scripts/deathmatch/music_tracks.txt"

class CTFMusicController;
extern CTFMusicController *g_pMusicController;

class CTFMusicController : public CBaseEntity
#if defined( CLIENT_DLL )
		, public CGameEventListener
#endif
{
	DECLARE_CLASS( CTFMusicController, CBaseEntity ); // BaseClass
public:
	DECLARE_NETWORKCLASS();

	CTFMusicController();
	virtual ~CTFMusicController();

	virtual void Spawn();
	virtual void Precache();
	
	void ParseMusicData( const char *pszFile );
	
#if defined( CLIENT_DLL )
	// Client only
	virtual void ClientThink();
	void OnPreDataChanged( DataUpdateType_t updateType );
	void OnDataChanged( DataUpdateType_t updateType );
	
	void StartMusic();
	void StopMusic( bool bPlayEnding );
	
	void ToggleMusicEnabled( bool bEnable );
	void RestartMusic();
	
	// CGameEventListener
	virtual void FireGameEvent( IGameEvent *pEvent );
	
	virtual void UpdateOnRemove();
#else
	// Server only
	
	void InputSetEnable( inputdata_t &inputdata );
	void InputSetDisable( inputdata_t &inputdata );
	void InputSetTrack( inputdata_t &inputdata );
	void InputSetRandomTrack( inputdata_t &inputdata );
	void SetTrack( int iTrack );
	void SelectRandomTrack();
	
	virtual int UpdateTransmitState();
	
	void PrecacheTrack( int iTrack );
	
	int DrawDebugTextOverlays();
#endif

#if defined( CLIENT_DLL )
	bool IsPlaying() const { return m_bPlaying; }
	bool IsPlayingEnding() const { return m_bPlayingEnding; }
	bool InWaiting() const { return m_bInWaiting; }
	bool ShouldPlayWaitingMusic()
	{
		return TFGameRules()->IsInWaitingForPlayers(); // idk
		//return TFGameRules() && ( TFGameRules()->IsInWaitingForPlayers() || TFGameRules()->InRoundRestart() );
	}
	bool CanPlayMusic()
	{
		// NOTE(SanyaSho): linux beta5 client says GetLocalPlayerTeam() < 1
		if( (g_pMusicController == this) && tf2c_music.GetBool() && m_bShouldPlay && (GetLocalPlayerTeam() >= FIRST_GAME_TEAM) )
			return (TFGameRules()->State_Get() != GR_STATE_GAME_OVER);
		return false;
	}
#endif

private:
#if defined( CLIENT_DLL )
	// Client only
	int m_iTrack; // RECV
	bool m_bShouldPlay; // RECV
	CSoundPatch *m_pSound;
	int m_iPlayingTrack;
	bool m_bPlaying;
	bool m_bPlayingEnding;
	bool m_bInWaiting;
	bool m_bOldShouldPlay;
#else
	// Server only
	CNetworkVar( int, m_iTrack ); // SEND
	CNetworkVar( bool, m_bShouldPlay ); // SEND
#endif

	// Shared only
	CUtlMap< int, MusicData_t > m_Tracks;

	DECLARE_DATADESC();
};

#endif // TF_MUSIC_CONTROLLER_H
