#if !defined( TF_MUSIC_CONTROLLER_H )
#define TF_MUSIC_CONTROLLER_H

#if defined( CLIENT_DLL )
#include "c_baseentity.h"
#else
#include "baseentity.h"
#endif

#include "utllinkedlist.h"

#define TF_NUM_MUSIC_SEGMENTS 3
struct MusicData_t
{
	char szName[128];
	char szComposer[128];
	char aSegments[TF_NUM_MUSIC_SEGMENTS][MAX_PATH];
};

extern const char *g_aMusicSegmentNames[TF_NUM_MUSIC_SEGMENTS];

#if defined( CLIENT_DLL )
#define CTFMusicController C_TFMusicController
#endif

#define SF_MUSICCONTROLLER_SHOULDPLAY 0x1

#define TF_MUSIC_CONTROLLER_FILE "scripts/deathmatch/music_tracks.txt"

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
	
	// CGameEventListener
	virtual void FireGameEvent( IGameEvent *pEvent );
	
	virtual void UpdateOnRemove();
#else
	// Server only
	
	void InputSetEnable( inputdata_t &inputdata );
	void InputSetDisable( inputdata_t &inputdata );
	void InputSetTrack( inputdata_t &inputdata );
	void SetTrack( int iTrack );
	
	virtual int UpdateTransmitState();
	
	void PrecacheTrack( int iTrack );
#endif

#if defined( CLIENT_DLL )
	bool IsPlaying() const { return m_bPlaying; }
	bool InWaiting() const { return m_bInWaiting; }
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

extern CTFMusicController *g_pMusicController;

#endif // TF_MUSIC_CONTROLLER_H
