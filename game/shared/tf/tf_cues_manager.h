#if !defined( TF_CUES_MANAGER_H )
#define TF_CUES_MANAGER_H

extern const char *g_aTFCueSounds[1];

#define _stub_ Msg( "[STUB]: %s\n", __FUNCTION__ );

#include "GameEventListener.h"

class CTFCuesManager : public CAutoGameSystemPerFrame, public CGameEventListener
{
public:
	CTFCuesManager() : CAutoGameSystemPerFrame( "TFCuesManager" )
	{
#if defined( CLIENT_DLL )
		ListenForGameEvent( "teamplay_update_timer" );
#endif
	}
	
public:
	virtual void FireGameEvent( IGameEvent *pEvent );

	virtual void LevelInitPreEntity();
	
#if defined( CLIENT_DLL )
	void PlayCue( int id );
	virtual void Update( float frametime );
#endif
};

static CTFCuesManager g_TFCuesManager;

#endif
