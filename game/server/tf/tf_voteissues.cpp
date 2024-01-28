#include "cbase.h"
#include "tf_shareddefs.h"
#include "tf_voteissues.h"
#include "tf_gamerules.h"

extern ConVar sv_vote_timer_duration;

//-----------------------------------------------------------------------------
// Purpose: Kick Player Issue
//-----------------------------------------------------------------------------
ConVar sv_vote_issue_kick_allowed( "sv_vote_issue_kick_allowed", "1", FCVAR_REPLICATED, "Can players call vote to kick players from the server?" );
ConVar sv_vote_issue_kick_cooldown( "sv_vote_issue_kick_cooldown", "30", FCVAR_NONE, "Minimum time before another kick vote can occur (in seconds)" );

CKickIssue::CKickIssue(const char *typeString) : CBaseIssue(typeString)
{
	Q_snprintf(m_pzPlayerName, sizeof(m_pzPlayerName), "");
	Q_snprintf(m_pzReason, sizeof(m_pzReason), "");
	m_iPlayerID = 0;
}

CKickIssue::~CKickIssue()
{
}

const char *CKickIssue::GetDisplayString()
{
	char result[64];
	Q_snprintf(result, sizeof(result), "#TF_vote_kick_player_%s", m_pzReason);
	char *szResult = (char*)malloc(sizeof(result));
	Q_strncpy(szResult, result, sizeof(result));
	return szResult;
}

const char *CKickIssue::GetVotePassedString()
{
	return "#TF_vote_passed_kick_player";
}

void CKickIssue::ListIssueDetails( CBasePlayer* pPlayer )
{
	if ( !sv_vote_issue_kick_allowed.GetBool() )
		return;

	char szBuffer[MAX_COMMAND_LENGTH];
	Q_snprintf( szBuffer, MAX_COMMAND_LENGTH, "callvote %s <userID>\n", GetTypeString() );
	ClientPrint( pPlayer, HUD_PRINTCONSOLE, szBuffer );
}

bool CKickIssue::IsEnabled()
{
	return sv_vote_issue_kick_allowed.GetBool();
}

const char * CKickIssue::GetDetailsString()
{
	if (m_iPlayerID > 0 && UTIL_PlayerByIndex(m_iPlayerID - 1))
	{
		return m_pzPlayerName;
	}
	else
	{
		return "Unnamed";
	}
}

void CKickIssue::OnVoteStarted()
{
	const char *pDetails = CBaseIssue::GetDetailsString();
	const char *pch;
	pch = strrchr(pDetails, ' ');
	if (!pch)
		return;

	int i = pch - pDetails + 1;
	Q_snprintf(m_pzReason, sizeof(m_pzReason), pDetails + i);

	char m_PlayerID[64];
	Q_snprintf(m_PlayerID, i, pDetails);

	m_iPlayerID = atoi(m_PlayerID);
	CBasePlayer *pVoteCaller = UTIL_PlayerByIndex(m_iPlayerID - 1);
	if (!pVoteCaller)
		return;

	Q_snprintf(m_pzPlayerName, sizeof(m_pzPlayerName), pVoteCaller->GetPlayerName());

	g_voteController->TryCastVote(pVoteCaller->entindex(), "Option2");
	CSteamID pSteamID;
	pVoteCaller->GetSteamID(&pSteamID);
	g_voteController->AddPlayerToNameLockedList(pSteamID, sv_vote_timer_duration.GetFloat(), m_iPlayerID);
}

void CKickIssue::Init()
{

}

void CKickIssue::NotifyGC(bool a2)
{
	return;
}

int CKickIssue::PrintLogData()
{
	return 0;
}

void CKickIssue::OnVoteFailed(int a2)
{
	CBaseIssue::OnVoteFailed(a2);
	PrintLogData();
}

bool CKickIssue::CreateVoteDataFromDetails(const char *s)
{
	return 0;
}

bool CKickIssue::CanCallVote( int iEntIndex, const char* pszDetails, vote_create_failed_t& nFailCode, int& nTime )
{
	return 0;
}

void CKickIssue::ExecuteCommand()
{
	if ( sv_vote_issue_kick_cooldown.GetInt() )
	{
		SetIssueCooldownDuration( sv_vote_issue_kick_cooldown.GetFloat() );
	}

	// Currently doesnt kick
}

bool CKickIssue::IsTeamRestrictedVote()
{
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Restart Round Issue
//-----------------------------------------------------------------------------
ConVar sv_vote_issue_restart_game_allowed( "sv_vote_issue_restart_game_allowed", "1", FCVAR_REPLICATED, "Can players call vote to restart the game?" );
ConVar sv_vote_issue_restart_game_cooldown( "sv_vote_issue_restart_game_cooldown", "300", FCVAR_NONE, "Minimum time before another restart vote can occur (in seconds) ");

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CRestartGameIssue::CRestartGameIssue(const char* typeString) : CBaseIssue( typeString )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CRestartGameIssue::~CRestartGameIssue()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CRestartGameIssue::ExecuteCommand()
{
	if ( sv_vote_issue_restart_game_cooldown.GetInt() )
	{
		SetIssueCooldownDuration( sv_vote_issue_restart_game_cooldown.GetFloat() );
	}

	engine->ServerCommand( "mp_restartgame 2;" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CRestartGameIssue::IsEnabled()
{
	return sv_vote_issue_restart_game_allowed.GetBool();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CRestartGameIssue::Init()
{

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CRestartGameIssue::CanCallVote( int iEntIndex, const char* pszDetails, vote_create_failed_t& nFailCode, int& nTime )
{
	if ( !CBaseIssue::CanCallVote ( iEntIndex, pszDetails, nFailCode, nTime ) )
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
const char* CRestartGameIssue::GetDisplayString()
{
	return "#TF_vote_restart_game";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char* CRestartGameIssue::GetVotePassedString()
{
	return "#TF_vote_passed_restart_game";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CRestartGameIssue::ListIssueDetails(CBasePlayer* pForWhom)
{
	if ( !sv_vote_issue_restart_game_allowed.GetBool() )
		return;

	ListStandardNoArgCommand(pForWhom, GetTypeString());
}