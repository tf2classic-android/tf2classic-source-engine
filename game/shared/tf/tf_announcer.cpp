//=============================================================================//
//
// Purpose: 
//
//=============================================================================//
#include "cbase.h"
#include "tf_announcer.h"
#include "tf_gamerules.h"

#ifdef CLIENT_DLL
#include <engine/IEngineSound.h>
#include "hud_macros.h"
#endif

ConVar tf2c_announcer_manual_override( "tf2c_announcer_manual_override", "-1", FCVAR_NOTIFY | FCVAR_REPLICATED, "Override announcer sounds\n-1 - Disable; 0 - TF2; 1 - VOX" );

#if defined( GAME_DLL )
CON_COMMAND_F( tf2c_announcer_speak, "", FCVAR_CHEAT )
{
	if(args.ArgC() < 2 )
	{
		Msg( "Usage: %s <sentence number>\n", args[0] );
		return;
	}
	
	int iSentence = atoi( args[1] );
	if( iSentence >= TF_ANNOUNCER_MESSAGE_COUNT )
		return;

	Msg( "[TFANNOUNCER]: Playing sentence %d\n", iSentence );
	g_TFAnnouncer.Speak( iSentence );
}
#endif

const char *CTFAnnouncer::m_aAnnouncerSounds[TF_ANNOUNCERTYPE_COUNT][TF_ANNOUNCER_MESSAGE_COUNT] =
{
	// Default TF2 announcer
	{
		"Game.Stalemate",
		"Game.SuddenDeath",
		"Game.YourTeamWon",
		"Game.YourTeamLost",
		"Game.Overtime",
		
		"Announcer.RoundBegins60Seconds",
		"Announcer.RoundBegins30Seconds",
		"Announcer.RoundBegins10Seconds",
		"Announcer.RoundBegins5Seconds",
		"Announcer.RoundBegins4Seconds",
		"Announcer.RoundBegins3Seconds",
		"Announcer.RoundBegins2Seconds",
		"Announcer.RoundBegins1Seconds",
		"Announcer.RoundEnds5minutes",
		"Announcer.RoundEnds60seconds",
		"Announcer.RoundEnds30seconds",
		"Announcer.RoundEnds10seconds",
		"Announcer.RoundEnds5seconds",
		"Announcer.RoundEnds4seconds",
		"Announcer.RoundEnds3seconds",
		"Announcer.RoundEnds2seconds",
		"Announcer.RoundEnds1seconds",
		
		"Announcer.TimeAdded",
		"Announcer.TimeAwardedForTeam",
		"Announcer.TimeAddedForEnemy",
		
		// CTF
		"CaptureFlag.EnemyStolen",
		"CaptureFlag.EnemyDropped",
		"CaptureFlag.EnemyCaptured",
		"CaptureFlag.EnemyReturned",
		"CaptureFlag.TeamStolen",
		"CaptureFlag.TeamDropped",
		"CaptureFlag.TeamCaptured",
		"CaptureFlag.TeamReturned",
		
		// AD
		"AttackDefend.EnemyStolen",
		"AttackDefend.EnemyDropped",
		"AttackDefend.EnemyCaptured",
		"AttackDefend.EnemyReturned",
		"AttackDefend.TeamStolen",
		"AttackDefend.TeamDropped",
		"AttackDefend.TeamCaptured",
		"AttackDefend.TeamReturned",
		
		// INVADE
		"Invade.EnemyStolen",
		"Invade.EnemyDropped",
		"Invade.EnemyCaptured",
		"Invade.TeamStolen",
		"Invade.TeamDropped",
		"Invade.TeamCaptured",
		"Invade.FlagReturned",
		
		// SD
		"Announcer.SD_TheirTeamHasFlag",
		"Announcer.SD_TheirTeamDroppedFlag",
		"Announcer.SD_TheirTeamCapped",
		"Announcer.SD_OurTeamHasFlag",
		"Announcer.SD_OurTeamDroppedFlag",
		"Announcer.SD_OurTeamCapped",
		"Announcer.SD_FlagReturned",
		"Announcer.SD_RoundStart",
		
		// CP
		"Announcer.ControlPointContested",
		"Announcer.ControlPointContested_Neutral",
		
		// Payload
		"Announcer.Cart.WarningAttacker",
		"Announcer.Cart.WarningDefender",
		"Announcer.Cart.FinalWarningAttacker",
		"Announcer.Cart.FinalWarningDefender",
		
		// Arena
		"Announcer.AM_RoundStartRandom",
		"Announcer.AM_FirstBloodRandom",
		"Announcer.AM_FirstBloodFast",
		"Announcer.AM_FirstBloodFinally",
		"Announcer.AM_CapEnabledRandom",
		"Announcer.AM_FlawlessVictory01",
		"Announcer.AM_FlawlessVictoryRandom",
		"Announcer.AM_FlawlessDefeatRandom",
		"Announcer.AM_TeamScrambleRandom",
		"Announcer.AM_LastManAlive",
		
		// MvM
		"Announcer.SecurityAlert",
		NULL,
		
		// DEATHMATCH
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		
		// INFECTION
		NULL,
		NULL
	},
	// TF2C VOX announcer
	{
		"Announcer_VOX.Stalemate",
		"Announcer_VOX.SuddenDeath",
		"Announcer_VOX.YourTeamWon",
		"Announcer_VOX.YourTeamLost",
		"Announcer_VOX.Overtime",
		
		"Announcer_VOX.RoundBegins60Seconds",
		"Announcer_VOX.RoundBegins30Seconds",
		"Announcer_VOX.RoundBegins10Seconds",
		"Announcer_VOX.RoundBegins5Seconds",
		"Announcer_VOX.RoundBegins4Seconds",
		"Announcer_VOX.RoundBegins3Seconds",
		"Announcer_VOX.RoundBegins2Seconds",
		"Announcer_VOX.RoundBegins1Seconds",
		"Announcer_VOX.RoundEnds5minutes",
		"Announcer_VOX.RoundEnds60seconds",
		"Announcer_VOX.RoundEnds30seconds",
		"Announcer_VOX.RoundEnds10seconds",
		"Announcer_VOX.RoundEnds5seconds",
		"Announcer_VOX.RoundEnds4seconds",
		"Announcer_VOX.RoundEnds3seconds",
		"Announcer_VOX.RoundEnds2seconds",
		"Announcer_VOX.RoundEnds1seconds",
		
		"Announcer_VOX.TimeAdded",
		"Announcer_VOX.TimeAwardedForTeam",
		"Announcer_VOX.TimeAddedForEnemy",
		
		// CTF
		"Announcer_VOX.CaptureFlag_EnemyStolen",
		"Announcer_VOX.CaptureFlag_EnemyDropped",
		"Announcer_VOX.CaptureFlag_EnemyCaptured",
		"Announcer_VOX.CaptureFlag_EnemyReturned",
		"Announcer_VOX.CaptureFlag_TeamStolen",
		"Announcer_VOX.CaptureFlag_TeamDropped",
		"Announcer_VOX.CaptureFlag_TeamCaptured",
		"Announcer_VOX.CaptureFlag_TeamReturned",
		
		// AD
		"Announcer_VOX.AttackDefend_EnemyStolen",
		"Announcer_VOX.AttackDefend_EnemyDropped",
		"Announcer_VOX.AttackDefend_EnemyCaptured",
		"Announcer_VOX.AttackDefend_EnemyReturned",
		"Announcer_VOX.AttackDefend_TeamStolen",
		"Announcer_VOX.AttackDefend_TeamDropped",
		"Announcer_VOX.AttackDefend_TeamCaptured",
		"Announcer_VOX.AttackDefend_TeamReturned",
		
		// INVADE
		"Announcer_VOX.Invade_EnemyStolen",
		"Announcer_VOX.Invade_EnemyDropped",
		"Announcer_VOX.Invade_EnemyCaptured",
		"Announcer_VOX.Invade_TeamStolen",
		"Announcer_VOX.Invade_TeamDropped",
		"Announcer_VOX.Invade_TeamCaptured",
		"Announcer_VOX.Invade_FlagReturned",
		
		// SD
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		
		// CP
		"Announcer_VOX.ControlPointContested",
		"Announcer_VOX.ControlPointContested_Neutral",
		
		// Payload
		"Announcer_VOX.Cart.WarningAttacker",
		"Announcer_VOX.Cart.WarningDefender",
		"Announcer_VOX.Cart.FinalWarningAttacker",
		"Announcer_VOX.Cart.FinalWarningDefender",
		
		// AM
		"Announcer_VOX.AM_RoundStartRandom",
		"Announcer_VOX.AM_FirstBloodRandom",
		"Announcer_VOX.AM_FirstBloodFast",
		"Announcer_VOX.AM_FirstBloodFinally",
		"Announcer_VOX.AM_CapEnabledRandom",
		"Announcer_VOX.AM_FlawlessVictory01",
		"Announcer_VOX.AM_FlawlessVictoryRandom",
		"Announcer_VOX.AM_FlawlessDefeatRandom",
		"Announcer_VOX.AM_TeamScrambleRandom",
		"Announcer_VOX.AM_LastManAlive",
		
		// MvM
		"Announcer_VOX.SecurityAlert",
		NULL,
		
		// DEATHMATCH
		"Announcer_VOX.DM_CritIncoming",
		"Announcer_VOX.DM_ShieldIncoming",
		"Announcer_VOX.DM_HasteIncoming",
		"Announcer_VOX.DM_BerserkIncoming",
		"Announcer_VOX.DM_CloakIncoming",
		"Announcer_VOX.DM_MegahealthIncoming",
		"Announcer_VOX.DM_DisplacerIncoming",
		"Announcer_VOX.DM_CritSpawn",
		"Announcer_VOX.DM_ShieldSpawn",
		"Announcer_VOX.DM_HasteSpawn",
		"Announcer_VOX.DM_BerserkSpawn",
		"Announcer_VOX.DM_CloakSpawn",
		"Announcer_VOX.DM_MegahealthSpawn",
		"Announcer_VOX.DM_DisplacerSpawn",
		"Announcer_VOX.DM_CritTeamPickup",
		"Announcer_VOX.DM_ShieldTeamPickup",
		"Announcer_VOX.DM_HasteTeamPickup",
		"Announcer_VOX.DM_BerserkTeamPickup",
		"Announcer_VOX.DM_CloakTeamPickup",
		"Announcer_VOX.DM_MegahealthTeamPickup",
		"Announcer_VOX.DM_DisplacerTeamPickup",
		"Announcer_VOX.DM_CritEnemyPickup",
		"Announcer_VOX.DM_ShieldEnemyPickup",
		"Announcer_VOX.DM_HasteEnemyPickup",
		"Announcer_VOX.DM_BerserkEnemyPickup",
		"Announcer_VOX.DM_CloakEnemyPickup",
		"Announcer_VOX.DM_MegahealthEnemyPickup",
		"Announcer_VOX.DM_DisplacerEnemyPickup",
		
		// INFECTION
		"Announcer_VOX.HumansWin",
		"Announcer_VOX.ZombiesWin"
	}
};

CTFAnnouncer g_TFAnnouncer( "Announcer" );

#ifdef CLIENT_DLL
static void __MsgFunc_AnnouncerSpeak( bf_read &msg )
{
	int iMessage = msg.ReadShort();
	g_TFAnnouncer.Speak( iMessage );
}
#endif

CTFAnnouncer::CTFAnnouncer( const char *pszName ) : CAutoGameSystem( pszName )
{
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CTFAnnouncer::Init( void )
{
#ifdef CLIENT_DLL
	HOOK_MESSAGE( AnnouncerSpeak );
#endif

	return true;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFAnnouncer::LevelInitPreEntity( void )
{
#ifdef GAME_DLL
	for ( int i = 0; i < TF_ANNOUNCERTYPE_COUNT; i++ )
	{
		for ( int j = 0; j < TF_ANNOUNCER_MESSAGE_COUNT; j++ )
		{
			const char *pszSound = m_aAnnouncerSounds[i][j];
			if ( !pszSound || !pszSound[0] )
				continue;

			CBaseEntity::PrecacheScriptSound( pszSound );
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
const char *CTFAnnouncer::GetSoundForMessage( int iMessage )
{
	int iType = TF_ANNOUNCERTYPE_TF2;
	if( TFGameRules() && TFGameRules()->IsDeathmatch() )
	{
		iType = TF_ANNOUNCERTYPE_VOX;
	}
	
	// SanyaSho: manual override
	if( tf2c_announcer_manual_override.GetInt() != -1 )
	{
		iType = clamp( tf2c_announcer_manual_override.GetInt(), 0, TF_ANNOUNCERTYPE_COUNT - 1 );
	}
	
	return m_aAnnouncerSounds[iType][iMessage];
}

#ifdef GAME_DLL
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFAnnouncer::Speak( IRecipientFilter &filter, int iMessage )
{
	UserMessageBegin( filter, "AnnouncerSpeak" );
	WRITE_SHORT( iMessage );
	MessageEnd();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFAnnouncer::Speak( int iMessage )
{
	CReliableBroadcastRecipientFilter filter;
	Speak( filter, iMessage );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFAnnouncer::Speak( CBasePlayer *pPlayer, int iMessage )
{
	CSingleUserRecipientFilter filter( pPlayer );
	filter.MakeReliable();
	Speak( filter, iMessage );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFAnnouncer::Speak( int iTeam, int iMessage )
{
	CTeamRecipientFilter filter( iTeam, true );
	Speak( filter, iMessage );
}
#else
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFAnnouncer::Speak( int iMessage )
{
	const char *pszSound = GetSoundForMessage( iMessage );
	if ( !pszSound || !pszSound[0] )
		return;

	CLocalPlayerFilter filter;
	C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, pszSound );
}
#endif
