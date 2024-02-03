//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:  TF-specific things to vote on
//
//=============================================================================

#include "cbase.h"
#include "tf_voteissues.h"
#include "tf_player.h"

#include "vote_controller.h"
#include "fmtstr.h"
#include "eiface.h"
#include "tf_gamerules.h"
#include "inetchannelinfo.h"
#include "tf_gamestats.h"

#include "tf_objective_resource.h"
#include "player_resource.h"
#include "tf_player_resource.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar mp_autoteambalance;
extern ConVar tf_classlimit;
extern ConVar sv_vote_quorum_ratio;

// do not re-order, stored in DB
enum
{
	kVoteKickBanPlayerReason_Other,
	kVoteKickBanPlayerReason_Cheating,
	kVoteKickBanPlayerReason_Idle,
	kVoteKickBanPlayerReason_Scamming,
};

uint32 GetKickBanPlayerReason( const char *pReasonString )
{
        if ( Q_strncmp( pReasonString, "other", 5 ) == 0 )
        {
                return kVoteKickBanPlayerReason_Other;
        }
        else if ( Q_strncmp( pReasonString, "cheating", 8 ) == 0 )
        {
                return kVoteKickBanPlayerReason_Cheating;
        }
        else if ( Q_strncmp( pReasonString, "idle", 4 ) == 0 )
        {
                return kVoteKickBanPlayerReason_Idle;
        }
        else if ( Q_strncmp( pReasonString, "scamming", 8 ) == 0 )
        {
                return kVoteKickBanPlayerReason_Scamming;
        }
        return kVoteKickBanPlayerReason_Other;
}

static bool VotableMap( const char *pszMapName )
{
	char szCanonName[64] = { 0 };
	V_strncpy( szCanonName, pszMapName, sizeof( szCanonName ) );
	IVEngineServer::eFindMapResult eResult = engine->FindMap( szCanonName, sizeof( szCanonName ) );

	switch ( eResult )
	{
	case IVEngineServer::eFindMap_Found:
	case IVEngineServer::eFindMap_NonCanonical:
	case IVEngineServer::eFindMap_PossiblyAvailable:
	case IVEngineServer::eFindMap_FuzzyMatch:
		return true;
	case IVEngineServer::eFindMap_NotFound:
		return false;
	}

	AssertMsg( false, "Unhandled engine->FindMap return value\n" );
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Base TF Issue
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose: Restart Round Issue
//-----------------------------------------------------------------------------
ConVar sv_vote_issue_restart_game_allowed( "sv_vote_issue_restart_game_allowed", "0", FCVAR_NONE, "Can players call votes to restart the game?" );
ConVar sv_vote_issue_restart_game_cooldown( "sv_vote_issue_restart_game_cooldown", "300", FCVAR_NONE, "Minimum time before another restart vote can occur (in seconds)." );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CRestartGameIssue::ExecuteCommand( void )
{
	if ( sv_vote_issue_restart_game_cooldown.GetInt() )
	{
		SetIssueCooldownDuration( sv_vote_issue_restart_game_cooldown.GetFloat() );
	}

	engine->ServerCommand( "mp_restartgame 1;" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CRestartGameIssue::IsEnabled( void )
{
	return sv_vote_issue_restart_game_allowed.GetBool();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CRestartGameIssue::CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime )
{
	if( !CBaseTFIssue::CanCallVote( iEntIndex, pszDetails, nFailCode, nTime ) )
		return false;

	if( !IsEnabled() )
	{
		nFailCode = VOTE_FAILED_ISSUE_DISABLED;
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CRestartGameIssue::GetDisplayString( void )
{
	return "#TF_vote_restart_game";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CRestartGameIssue::GetVotePassedString( void )
{
	return "#TF_vote_passed_restart_game";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CRestartGameIssue::ListIssueDetails( CBasePlayer *pForWhom )
{
	if( !sv_vote_issue_restart_game_allowed.GetBool() )
		return;

	ListStandardNoArgCommand( pForWhom, GetTypeString() );
}


//-----------------------------------------------------------------------------
// Purpose: Kick Player Issue
//-----------------------------------------------------------------------------
ConVar sv_vote_issue_kick_allowed( "sv_vote_issue_kick_allowed", "0", FCVAR_NONE, "Can players call votes to kick players from the server?" );
ConVar sv_vote_kick_ban( "sv_vote_kick_ban", "0", FCVAR_NONE, "Ban player after kick. (0 = Disabled)" );
ConVar sv_vote_kick_ban_duration( "sv_vote_kick_ban_duration", "20", FCVAR_NONE, "The number of minutes a vote ban should last. (0 = Disabled)" );
ConVar sv_vote_issue_kick_namelock_duration( "sv_vote_issue_kick_namelock_duration", "120", FCVAR_NONE, "How long to prevent kick targets from changing their name (in seconds)." );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CKickIssue::Init( void )
{
	m_szTargetPlayerName[0] = 0;
	m_hPlayerTarget = NULL;
	m_unKickReason = kVoteKickBanPlayerReason_Other;
	m_KickCallerUserID = -1;
	m_KickTargetUserID = -1;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CKickIssue::ExecuteCommand( void )
{
	PrintLogData();

	engine->ServerCommand( CFmtStr( "kickid \"%d\" %s\n", m_KickTargetUserID, g_pszVoteKickString ) );

	if( !sv_vote_kick_ban.GetBool() )
	{
		return;
	}

	// If we're not in strict mode they might be here as or be able to rejoin as an adhoc player -- ban.
	// Technically the GC might send them back here for *new* match if they get kicked as ad-hoc, the current match ends, etc.
	engine->ServerCommand( CFmtStr( "banid %d \"%d\"\n", sv_vote_kick_ban_duration.GetInt(), m_KickTargetUserID ) );

	// Band-aid: Hacks are able to avoid kick+ban, and we're not yet sure how they're doing it.  This code checks to see
	//           if they come back.
	//
	// XXX(JohnS): We think the original cause behind this was fixed (connecting-but-not-active race condition)
	//g_voteController->AddPlayerToKickWatchList( m_steamIDVoteTarget, ( sv_vote_kick_ban_duration.GetFloat() * 60.f ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CKickIssue::IsEnabled( void )
{
	return sv_vote_issue_kick_allowed.GetBool();
}

//-----------------------------------------------------------------------------
// Purpose: This gets calle first.  If true, moves on to OnVoteStarted()
//-----------------------------------------------------------------------------
bool CKickIssue::CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime )
{
	if ( !CBaseTFIssue::CanCallVote( iEntIndex, pszDetails, nFailCode, nTime ) )
		return false;

	Init();

	if ( !IsEnabled() )
	{
		nFailCode = VOTE_FAILED_ISSUE_DISABLED;
		return false;
	}

	if ( !CreateVoteDataFromDetails( pszDetails ) )
	{
		nFailCode = VOTE_FAILED_PLAYERNOTFOUND;
		return false;
	}

	// Don't kick proxies
	if ( m_hPlayerTarget->IsReplay() || m_hPlayerTarget->IsHLTV() )
	{
		nFailCode = VOTE_FAILED_PLAYERNOTFOUND;
		return false;
	}

	// Don't kick the host or an admin
	if ( ( !engine->IsDedicatedServer() && m_hPlayerTarget->entindex() == 1 ) ||
		 m_hPlayerTarget->IsAutoKickDisabled() )
	{
		nFailCode = VOTE_FAILED_CANNOT_KICK_ADMIN;
		return false;
	}

	// Store caller steamID
	CTFPlayer *pTFVoteCaller = ToTFPlayer( UTIL_EntityByIndex( iEntIndex ) );
	if ( !pTFVoteCaller )
		return false;

	m_KickCallerUserID = pTFVoteCaller->GetUserID();
	if( ( m_KickCallerUserID == -1 ) || ( m_KickCallerUserID == 0 ) )
		return false;

	// Store target steamID - if they're not a bot
	bool bFakeClient = m_hPlayerTarget->IsFakeClient() || m_hPlayerTarget->IsBot();
	if ( !bFakeClient )
	{
		m_KickTargetUserID = m_hPlayerTarget->GetUserID();
		if( m_KickTargetUserID == -1 )
			return false;
	}
	
	// Don't kick players on other teams
	if ( pTFVoteCaller->GetTeamNumber() != m_hPlayerTarget->GetTeamNumber() )
		return false;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CKickIssue::OnVoteFailed( int iEntityHoldingVote )
{
	CBaseTFIssue::OnVoteFailed( iEntityHoldingVote );

	PrintLogData();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
 void CKickIssue::OnVoteStarted( void )
 {
	// CanCallVote() should have initialized this
	if ( !m_hPlayerTarget )
	{
		m_hPlayerTarget = UTIL_PlayerByUserId( m_KickTargetUserID );
	}

	if ( !m_hPlayerTarget )
		return;

	// Capture some data about the kick target now, so they can't avoid the 
	// result by doing things like drop, retry, stop sending commands, etc.
	if( m_KickTargetUserID != -1 )
	{
		Q_strncpy( m_szTargetPlayerName, m_hPlayerTarget->GetPlayerName(), sizeof( m_szTargetPlayerName ) );

		// Configured to block name changing when targeted for a kick?
		/*
		if ( sv_vote_issue_kick_namelock_duration.GetFloat() > 0 )
		{
			g_voteController->AddPlayerToNameLockedList( m_steamIDVoteTarget, sv_vote_issue_kick_namelock_duration.GetFloat(), m_hPlayerTarget->GetUserID() );
		}
		*/
	}

	// Auto vote 'No' for the person being kicked unless they are idle
	CTFPlayer *pTFVoteTarget = ToTFPlayer( m_hPlayerTarget );
	if ( pTFVoteTarget /* && !pTFVoteTarget->IsAwayFromKeyboard() */ && ( pTFVoteTarget->GetTeamNumber() != TEAM_SPECTATOR ) )
	{
		g_voteController->TryCastVote( pTFVoteTarget->entindex(), "Option2" );
	}
 }

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CKickIssue::GetDisplayString( void )
{
	switch ( m_unKickReason )
	{
	case kVoteKickBanPlayerReason_Other:	return "#TF_vote_kick_player_other";
	case kVoteKickBanPlayerReason_Cheating:	return "#TF_vote_kick_player_cheating";
	case kVoteKickBanPlayerReason_Idle:		return "#TF_vote_kick_player_idle";
	case kVoteKickBanPlayerReason_Scamming:	return "#TF_vote_kick_player_scamming";
	}
	return "#TF_vote_kick_player_other";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CKickIssue::GetVotePassedString( void )
{
	// Player left before we could finish, but still got banned
	if ( !m_hPlayerTarget && sv_vote_kick_ban_duration.GetInt() > 0 )
		return "#TF_vote_passed_ban_player";

	// Player is still here
	return "#TF_vote_passed_kick_player";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CKickIssue::GetDetailsString( void )
{
	if ( m_hPlayerTarget )
		return m_hPlayerTarget->GetPlayerName();

	// If they left, use name stored at creation
	if ( V_strlen( m_szTargetPlayerName ) )
		return m_szTargetPlayerName;
	
	return "Unnamed";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CKickIssue::PrintLogData( void )
{
	bool bFakeClient = m_hPlayerTarget && ( m_hPlayerTarget->IsFakeClient() || m_hPlayerTarget->IsHLTV() || m_hPlayerTarget->IsReplay() );

	UTIL_LogPrintf( "Kick Vote details:  VoteInitiatorUserID: %d  VoteTargetUserID: %d  Name: %s  Proxy: %i\n",
					m_KickCallerUserID,
					m_KickTargetUserID,
					m_hPlayerTarget ? m_szTargetPlayerName : "Disconnected",
					bFakeClient );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CKickIssue::CreateVoteDataFromDetails( const char *pszDetails )
{
	int iUserID = 0;
	const char *pReasonString = strstr( pszDetails, " " );
	if ( pReasonString != NULL )
	{
		pReasonString += 1;
		CUtlString userID;
		userID.SetDirect( pszDetails, pReasonString - pszDetails );
		iUserID = atoi( userID );
		m_unKickReason = GetKickBanPlayerReason( pReasonString );
	}
	else
	{
		iUserID = atoi( pszDetails );
	}

	// Try to use the steamID we stored in OnVoteStarted() (will fail for bots)
	for ( int i = 0; i < MAX_PLAYERS; i++ )
	{
		CTFPlayer *pPlayer = ToTFPlayer( UTIL_PlayerByIndex( i ) );
		if ( !pPlayer )
			continue;

		if( pPlayer->GetUserID() == m_KickTargetUserID )
		{
			m_hPlayerTarget = pPlayer;
			return true;
		}
	}
	
	// Otherwise rely on userID
	if ( iUserID )
	{
		m_hPlayerTarget = ToBasePlayer( UTIL_PlayerByUserId( iUserID ) );
	}

	return ( m_hPlayerTarget ) ? true : false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CKickIssue::ListIssueDetails( CBasePlayer *pForWhom )
{
	if( !sv_vote_issue_kick_allowed.GetBool() )
		return;

	char szBuffer[MAX_COMMAND_LENGTH];
	Q_snprintf( szBuffer, MAX_COMMAND_LENGTH, "callvote %s <userID>\n", GetTypeString() );
	ClientPrint( pForWhom, HUD_PRINTCONSOLE, szBuffer );
}


//-----------------------------------------------------------------------------
// Purpose: Changelevel
//-----------------------------------------------------------------------------
ConVar sv_vote_issue_changelevel_allowed( "sv_vote_issue_changelevel_allowed", "0", FCVAR_NONE, "Can players call votes to change levels?" );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CChangeLevelIssue::ExecuteCommand( void )
{
	engine->ChangeLevel( m_szDetailsString, NULL );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CChangeLevelIssue::CanTeamCallVote( int iTeam ) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CChangeLevelIssue::IsEnabled( void )
{
	return sv_vote_issue_changelevel_allowed.GetBool();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CChangeLevelIssue::CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime )
{
	if( !CBaseTFIssue::CanCallVote( iEntIndex, pszDetails, nFailCode, nTime ) )
		return false;

	if( !IsEnabled() )
	{
		nFailCode = VOTE_FAILED_ISSUE_DISABLED;
		return false;
	}

	if ( !Q_strcmp( pszDetails, "" ) )
	{
		nFailCode = VOTE_FAILED_MAP_NAME_REQUIRED;
		return false;
	}
	else
	{
		if ( !VotableMap( pszDetails ) )
		{
			nFailCode = VOTE_FAILED_MAP_NOT_FOUND;
			return false;
		}

		if ( MultiplayRules() && !MultiplayRules()->IsMapInMapCycle( pszDetails ) )
		{
			nFailCode = VOTE_FAILED_MAP_NOT_VALID;
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CChangeLevelIssue::GetDisplayString( void )
{
	return "#TF_vote_changelevel";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CChangeLevelIssue::GetVotePassedString( void )
{
	return "#TF_vote_passed_changelevel";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CChangeLevelIssue::GetDetailsString( void )
{
	return m_szDetailsString;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CChangeLevelIssue::ListIssueDetails( CBasePlayer *pForWhom )
{
	if( !sv_vote_issue_changelevel_allowed.GetBool() )
		return;

	char szBuffer[MAX_COMMAND_LENGTH];
	Q_snprintf( szBuffer, MAX_COMMAND_LENGTH, "callvote %s <mapname>\n", GetTypeString() );
	ClientPrint( pForWhom, HUD_PRINTCONSOLE, szBuffer );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CChangeLevelIssue::IsYesNoVote( void )
{
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Nextlevel
//-----------------------------------------------------------------------------
ConVar sv_vote_issue_nextlevel_allowed( "sv_vote_issue_nextlevel_allowed", "1", FCVAR_NONE, "Can players call votes to set the next level?" );
ConVar sv_vote_issue_nextlevel_choicesmode( "sv_vote_issue_nextlevel_choicesmode", "0", FCVAR_NONE, "Present players with a list of lowest playtime maps to choose from?" );
ConVar sv_vote_issue_nextlevel_allowextend( "sv_vote_issue_nextlevel_allowextend", "1", FCVAR_NONE, "Allow players to extend the current map?" );
ConVar sv_vote_issue_nextlevel_prevent_change( "sv_vote_issue_nextlevel_prevent_change", "1", FCVAR_NONE, "Not allowed to vote for a nextlevel if one has already been set." );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CNextLevelIssue::GetVoteOptions( CUtlVector <const char*> &vecNames )
{
	m_IssueOptions.RemoveAll();

	// Reserve the last option for "Extend current Map?"
	int nNumOptions = sv_vote_issue_nextlevel_allowextend.GetBool() ? GetNumberVoteOptions() - 1 : GetNumberVoteOptions();

	// Ask the stats system for playtime data
	/*
	if ( CTF_GameStats.GetVoteData( "NextLevel", nNumOptions, m_IssueOptions ) )
	{
		FOR_EACH_VEC( m_IssueOptions, iIndex )
		{
			vecNames.AddToTail( m_IssueOptions[iIndex] );
		}

		if ( sv_vote_issue_nextlevel_allowextend.GetBool() || m_IssueOptions.Count() == 1 )
		{
			vecNames.AddToTail( "Extend current Map" );
		}
	
		return true;
	}
	*/

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNextLevelIssue::ExecuteCommand( void )
{
	if ( Q_strcmp( m_szDetailsString, "Extend current Map" ) == 0 )
	{
		// Players want to extend the current map, so extend any existing limits
		if ( mp_timelimit.GetInt() > 0 )
		{
			engine->ServerCommand( CFmtStr( "mp_timelimit %d;", mp_timelimit.GetInt() + 20 ) );
		}

		if ( mp_maxrounds.GetInt() > 0 )
		{
			engine->ServerCommand( CFmtStr( "mp_maxrounds %d;", mp_maxrounds.GetInt() + 2 ) );
		}

		if ( mp_winlimit.GetInt() > 0 )
		{
			engine->ServerCommand( CFmtStr( "mp_winlimit %d;", mp_winlimit.GetInt() + 2 ) );
		}
	}
	else
	{
		nextlevel.SetValue( m_szDetailsString );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CNextLevelIssue::CanTeamCallVote( int iTeam ) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CNextLevelIssue::IsEnabled( void )
{
	return sv_vote_issue_nextlevel_allowed.GetBool();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CNextLevelIssue::CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime )
{
	if( !CBaseTFIssue::CanCallVote( iEntIndex, pszDetails, nFailCode, nTime ) )
		return false;

	// TFGameRules created vote
	if ( sv_vote_issue_nextlevel_choicesmode.GetBool() && iEntIndex == 99 )
	{
		// Invokes a UI down stream
		if ( Q_strcmp( pszDetails, "" ) == 0 )
		{
			return true;
		}

		return false;
	}
	
	if( !IsEnabled() )
	{
		nFailCode = VOTE_FAILED_ISSUE_DISABLED;
		return false;
	}

	if ( Q_strcmp( pszDetails, "" ) == 0 )
	{
		nFailCode = VOTE_FAILED_MAP_NAME_REQUIRED;
		return false;
	}
	else
	{
		if ( !VotableMap( pszDetails ) )
		{
			nFailCode = VOTE_FAILED_MAP_NOT_FOUND;
			return false;
		}

		if ( MultiplayRules() && !MultiplayRules()->IsMapInMapCycle( pszDetails ) )
		{
			nFailCode = VOTE_FAILED_MAP_NOT_VALID;
			return false;
		}
	}

	if ( sv_vote_issue_nextlevel_prevent_change.GetBool() )
	{
		if ( nextlevel.GetString() && *nextlevel.GetString() )
		{
			nFailCode = VOTE_FAILED_NEXTLEVEL_SET;
			return false;
		}
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CNextLevelIssue::GetDisplayString( void )
{
	// If we don't have a map passed in already...
	if ( Q_strcmp( m_szDetailsString, "" ) == 0 )
	{
		if ( sv_vote_issue_nextlevel_choicesmode.GetBool() )
		{
			return "#TF_vote_nextlevel_choices";
		}
	}

	return "#TF_vote_nextlevel";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CNextLevelIssue::GetVotePassedString( void )
{
	if ( sv_vote_issue_nextlevel_allowextend.GetBool() )
	{
		if ( Q_strcmp( m_szDetailsString, "Extend current Map" ) == 0 )
		{
			return "#TF_vote_passed_nextlevel_extend";
		}
	}
	
	return "#TF_vote_passed_nextlevel";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CNextLevelIssue::GetDetailsString( void )
{
	return m_szDetailsString;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNextLevelIssue::ListIssueDetails( CBasePlayer *pForWhom )
{
	if( !sv_vote_issue_nextlevel_allowed.GetBool() )
		return;

	if ( !sv_vote_issue_nextlevel_choicesmode.GetBool() )
	{
		char szBuffer[MAX_COMMAND_LENGTH];
		Q_snprintf( szBuffer, MAX_COMMAND_LENGTH, "callvote %s <mapname>\n", GetTypeString() );
		ClientPrint( pForWhom, HUD_PRINTCONSOLE, szBuffer );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CNextLevelIssue::IsYesNoVote( void )
{
	// If we don't have a map name already, this will trigger a list of choices
	if ( Q_strcmp( m_szDetailsString, "" ) == 0 )
	{
		if ( sv_vote_issue_nextlevel_choicesmode.GetBool() )
			return false;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CNextLevelIssue::GetNumberVoteOptions( void )
{
	// If we don't have a map name already, this will trigger a list of choices
	if ( Q_strcmp( m_szDetailsString, "" ) == 0 )
	{
		if ( sv_vote_issue_nextlevel_choicesmode.GetBool() )
			return MAX_VOTE_OPTIONS;
	}

	// Vote on a specific map - Yes, No
	return 2;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
float CNextLevelIssue::GetQuorumRatio( void )
{
	// We don't really care about a quorum in this case.  If a few
	// people have a preference on the next level, and no one else
	// bothers to vote, just let their choice pass.
	if ( sv_vote_issue_nextlevel_choicesmode.GetBool() )
		return 0.1f;

	// Default
	return sv_vote_quorum_ratio.GetFloat();
}

//-----------------------------------------------------------------------------
// Purpose: Extend the current level
//-----------------------------------------------------------------------------
ConVar sv_vote_issue_extendlevel_allowed( "sv_vote_issue_extendlevel_allowed", "1", FCVAR_NONE, "Can players call votes to set the next level?" );
ConVar sv_vote_issue_extendlevel_quorum( "sv_vote_issue_extendlevel_quorum", "0.6", FCVAR_NONE, "What is the ratio of voters needed to reach quorum?" );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CExtendLevelIssue::ExecuteCommand( void )
{
	// Players want to extend the current map, so extend any existing limits
	if ( mp_timelimit.GetInt() > 0 )
	{
		engine->ServerCommand( CFmtStr( "mp_timelimit %d;", mp_timelimit.GetInt() + 20 ) );
	}

	if ( mp_maxrounds.GetInt() > 0 )
	{
		engine->ServerCommand( CFmtStr( "mp_maxrounds %d;", mp_maxrounds.GetInt() + 2 ) );
	}

	if ( mp_winlimit.GetInt() > 0 )
	{
		engine->ServerCommand( CFmtStr( "mp_winlimit %d;", mp_winlimit.GetInt() + 2 ) );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CExtendLevelIssue::IsEnabled( void )
{
	return sv_vote_issue_extendlevel_allowed.GetBool();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CExtendLevelIssue::CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime )
{
	if ( !CBaseTFIssue::CanCallVote( iEntIndex, pszDetails, nFailCode, nTime ) )
		return false;

	if ( !IsEnabled() )
	{
		nFailCode = VOTE_FAILED_ISSUE_DISABLED;
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CExtendLevelIssue::GetDisplayString( void )
{
	return "#TF_vote_extendlevel";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CExtendLevelIssue::ListIssueDetails( CBasePlayer *pForWhom )
{
	if ( !sv_vote_issue_extendlevel_allowed.GetBool() ) 
		return;

	ListStandardNoArgCommand( pForWhom, GetTypeString() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CExtendLevelIssue::GetVotePassedString( void )
{
	// We already had a localized string for this, even though we never use it.
	return "#TF_vote_passed_nextlevel_extend";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
float CExtendLevelIssue::GetQuorumRatio( void )
{
	// Our own quorom 
	return sv_vote_issue_extendlevel_quorum.GetFloat();
}

//-----------------------------------------------------------------------------
// Purpose: Scramble Teams Issue
//-----------------------------------------------------------------------------
ConVar sv_vote_issue_scramble_teams_allowed( "sv_vote_issue_scramble_teams_allowed", "1", FCVAR_NONE, "Can players call votes to scramble the teams?" );
ConVar sv_vote_issue_scramble_teams_cooldown( "sv_vote_issue_scramble_teams_cooldown", "1200", FCVAR_NONE, "Minimum time before another scramble vote can occur (in seconds)." );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CScrambleTeams::ExecuteCommand( void )
{
	if ( sv_vote_issue_scramble_teams_cooldown.GetInt() )
	{
		SetIssueCooldownDuration( sv_vote_issue_scramble_teams_cooldown.GetFloat() );
	}

	engine->ServerCommand( "mp_scrambleteams 2;" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CScrambleTeams::IsEnabled( void )
{
	return sv_vote_issue_scramble_teams_allowed.GetBool();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CScrambleTeams::CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime )
{
	if( !CBaseTFIssue::CanCallVote( iEntIndex, pszDetails, nFailCode, nTime ) )
		return false;

	if( !IsEnabled() )
	{
		nFailCode = VOTE_FAILED_ISSUE_DISABLED;
		return false;
	}

	if ( TFGameRules() && TFGameRules()->ShouldScrambleTeams() )
	{
		nFailCode = VOTE_FAILED_SCRAMBLE_IN_PROGRESS;
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CScrambleTeams::GetDisplayString( void )
{
	return "#TF_vote_scramble_teams";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CScrambleTeams::GetVotePassedString( void )
{
	return "#TF_vote_passed_scramble_teams";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CScrambleTeams::ListIssueDetails( CBasePlayer *pForWhom )
{
	if( !sv_vote_issue_scramble_teams_allowed.GetBool() )
		return;

	ListStandardNoArgCommand( pForWhom, GetTypeString() );
}

//-----------------------------------------------------------------------------
// Purpose: Enable/Disable mp_autoteambalance
//-----------------------------------------------------------------------------
ConVar sv_vote_issue_autobalance_allowed( "sv_vote_issue_autobalance_allowed", "0", FCVAR_NONE, "Can players call votes to enable or disable auto team balance?" );
ConVar sv_vote_issue_autobalance_cooldown( "sv_vote_issue_autobalance_cooldown", "300", FCVAR_NONE, "Minimum time before another auto team balance vote can occur (in seconds)." );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CTeamAutoBalanceIssue::GetTypeStringLocalized( void )
{
	// Disabled
	if ( !mp_autoteambalance.GetInt() )
	{
		return "#Vote_TeamAutoBalance_Enable";
	}

	return "#Vote_TeamAutoBalance_Disable";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTeamAutoBalanceIssue::ExecuteCommand( void )
{
	if ( sv_vote_issue_autobalance_cooldown.GetInt() )
	{
		SetIssueCooldownDuration( sv_vote_issue_autobalance_cooldown.GetFloat() );
	}

	// Disable
	if ( mp_autoteambalance.GetInt() )
	{
		engine->ServerCommand( "mp_autoteambalance 0;" );
	}
	// Enable
	else
	{
		engine->ServerCommand( "mp_autoteambalance 1;" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTeamAutoBalanceIssue::IsEnabled( void )
{
	if( TFGameRules() && TFGameRules()->IsDeathmatch() )
	{
		return false;
	}

	return sv_vote_issue_autobalance_allowed.GetBool();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTeamAutoBalanceIssue::CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime )
{
	if ( !CBaseTFIssue::CanCallVote( iEntIndex, pszDetails, nFailCode, nTime ) )
		return false;

	if ( !IsEnabled() )
	{
		nFailCode = VOTE_FAILED_ISSUE_DISABLED;
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CTeamAutoBalanceIssue::GetDisplayString( void )
{
	// Disable
	if ( mp_autoteambalance.GetInt() )
		return "#TF_vote_autobalance_disable";

	// Enable
	return "#TF_vote_autobalance_enable";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CTeamAutoBalanceIssue::GetVotePassedString( void )
{
	// Disable
	if ( mp_autoteambalance.GetInt() )
		return "#TF_vote_passed_autobalance_disable";

	// Enable
	return "#TF_vote_passed_autobalance_enable";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTeamAutoBalanceIssue::ListIssueDetails( CBasePlayer *pForWhom )
{
	if ( !sv_vote_issue_autobalance_allowed.GetBool() )
		return;

	ListStandardNoArgCommand( pForWhom, GetTypeString() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
float CTeamAutoBalanceIssue::GetQuorumRatio( void )
{
	float flRatio = sv_vote_quorum_ratio.GetFloat();

	// Disable
	if ( mp_autoteambalance.GetInt() )
		return flRatio;

	// Enable
	return Max( 0.1f, flRatio * 0.7f );
}

//-----------------------------------------------------------------------------
// Purpose: Enable/Disable tf_classlimit
//-----------------------------------------------------------------------------
#ifdef STAGING_ONLY
ConVar sv_vote_issue_classlimits_allowed( "sv_vote_issue_classlimits_allowed", "1", FCVAR_NONE, "Can players call votes to enable or disable per-class limits?" );
#else
ConVar sv_vote_issue_classlimits_allowed( "sv_vote_issue_classlimits_allowed", "0", FCVAR_NONE, "Can players call votes to enable or disable per-class limits?" );
#endif
ConVar sv_vote_issue_classlimits_max( "sv_vote_issue_classlimits_max", "4", FCVAR_NONE, "Maximum number of players (per-team) that can be any one class.", true, 1.f, false, 16.f );
ConVar sv_vote_issue_classlimits_cooldown( "sv_vote_issue_classlimits_cooldown", "300", FCVAR_NONE, "Minimum time before another classlimits vote can occur (in seconds)." );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CClassLimitsIssue::GetTypeStringLocalized( void )
{
	// Disabled
	if ( !tf_classlimit.GetInt() )
	{
		return "#Vote_ClassLimit_Enable";
	}

	return "#Vote_ClassLimit_Disable";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClassLimitsIssue::ExecuteCommand( void )
{
	if ( sv_vote_issue_classlimits_cooldown.GetInt() )
	{
		SetIssueCooldownDuration( sv_vote_issue_classlimits_cooldown.GetFloat() );
	}

	// Disable
	if ( tf_classlimit.GetInt() )
	{
		engine->ServerCommand( "tf_classlimit 0;" );
	}
	// Enable
	else
	{
		int nLimit = sv_vote_issue_classlimits_max.GetInt();
		engine->ServerCommand( CFmtStr( "tf_classlimit %i;", nLimit ) );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CClassLimitsIssue::IsEnabled( void )
{
	return sv_vote_issue_classlimits_allowed.GetBool();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CClassLimitsIssue::CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime )
{
	if ( !CBaseTFIssue::CanCallVote( iEntIndex, pszDetails, nFailCode, nTime ) )
		return false;

	if ( !IsEnabled() )
	{
		nFailCode = VOTE_FAILED_ISSUE_DISABLED;
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CClassLimitsIssue::GetDisplayString( void )
{
	// Disable
	if ( tf_classlimit.GetInt() )
		return "#TF_vote_classlimits_disable";

	// Enable
	return "#TF_vote_classlimits_enable";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CClassLimitsIssue::GetVotePassedString( void )
{
	// Disable
	if ( tf_classlimit.GetInt() )
		return "#TF_vote_passed_classlimits_disable";

	// Enable
	return "#TF_vote_passed_classlimits_enable";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClassLimitsIssue::ListIssueDetails( CBasePlayer *pForWhom )
{
	if ( !sv_vote_issue_classlimits_allowed.GetBool() )
		return;

	ListStandardNoArgCommand( pForWhom, GetTypeString() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CClassLimitsIssue::GetDetailsString( void )
{
	int nLimit = sv_vote_issue_classlimits_max.GetInt();
	m_sRetString = CFmtStr( "%i", nLimit );
	return m_sRetString.String();
}


//-----------------------------------------------------------------------------
// Purpose: Pause Game
//-----------------------------------------------------------------------------
ConVar sv_vote_issue_pause_game_allowed( "sv_vote_issue_pause_game_allowed", "0", FCVAR_HIDDEN, "Can players call votes to pause the game?" );
ConVar sv_vote_issue_pause_game_timer( "sv_vote_issue_pause_game_timer", "120", FCVAR_HIDDEN, "How long to pause the game for when this vote passes (in seconds)." );
ConVar sv_vote_issue_pause_game_cooldown( "sv_vote_issue_pause_game_cooldown", "1200", FCVAR_HIDDEN, "Minimum time before another pause vote can occur (in seconds)." );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPauseGameIssue::ExecuteCommand( void )
{
	if ( sv_vote_issue_pause_game_cooldown.GetInt() )
	{
		SetIssueCooldownDuration( sv_vote_issue_pause_game_cooldown.GetFloat() );
	}

	engine->SetPausedForced( true, sv_vote_issue_pause_game_timer.GetFloat() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CPauseGameIssue::IsEnabled( void )
{
	if ( engine->IsPaused() )
		return false;

	return sv_vote_issue_pause_game_allowed.GetBool();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CPauseGameIssue::CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime )
{
	if ( !CBaseTFIssue::CanCallVote( iEntIndex, pszDetails, nFailCode, nTime ) )
		return false;

	if ( !IsEnabled() )
	{
		nFailCode = VOTE_FAILED_ISSUE_DISABLED;
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CPauseGameIssue::GetDisplayString( void )
{
	return "#TF_vote_pause_game";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CPauseGameIssue::GetVotePassedString( void )
{
	return "#TF_vote_passed_pause_game";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPauseGameIssue::ListIssueDetails( CBasePlayer *pForWhom )
{
	if ( !sv_vote_issue_pause_game_allowed.GetBool() )
		return;

	ListStandardNoArgCommand( pForWhom, GetTypeString() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CPauseGameIssue::GetDetailsString( void )
{
	m_sRetString = CFmtStr( "%i", sv_vote_issue_pause_game_timer.GetInt() );
	return (m_sRetString.String());
}
