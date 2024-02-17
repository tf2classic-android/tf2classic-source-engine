#include "cbase.h"
#include "tf_cues_manager.h"
#include "tf_gamerules.h"

#if defined( CLIENT_DLL )
void CuesChangedCallBack( IConVar *pCVar, const char *pzsOld, float flOld )
{
}
ConVar tf2c_music_cues( "tf2c_music_cues", "1", FCVAR_ARCHIVE, "Enable musical cues triggered by certain in-game events.", CuesChangedCallBack );
#endif

const char *g_aTFCueSounds[1] =
{
	"music.setup",
};

void CTFCuesManager::FireGameEvent( IGameEvent *pEvent )
{
#if defined( CLIENT_DLL )
	if( !tf2c_music_cues.GetBool() )
	{
		return;
	}
	
	if( !Q_stricmp( pEvent->GetName(), "teamplay_update_timer" ) )
	{
		if( !TFGameRules()->IsInWaitingForPlayers() )
		{
			CLocalPlayerFilter filter;
			C_BaseEntity::StopSound( -1, g_aTFCueSounds[0] );
		}
	}
#endif
}

void CTFCuesManager::LevelInitPreEntity()
{
#if defined( GAME_DLL )
	for( int i = 0; i < ARRAYSIZE( g_aTFCueSounds ); i++ )
	{
		CBaseEntity::PrecacheScriptSound( g_aTFCueSounds[i] );
	}
#endif
}

#if defined( CLIENT_DLL )
void CTFCuesManager::PlayCue( int id )
{
	if( !tf2c_music_cues.GetBool() )
	{
		return;
	}
	
	CLocalPlayerFilter filter;
	C_BaseEntity::EmitSound( filter, -1, g_aTFCueSounds[id] );
}

void CTFCuesManager::Update( float frametime )
{

}
#endif
