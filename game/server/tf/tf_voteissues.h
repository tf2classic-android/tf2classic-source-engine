#ifndef TF_VOTEISSUES_H
#define TF_VOTEISSUES_H
#ifdef _WIN32
#pragma once
#endif

#include "vote_controller.h"

//-----------------------------------------------------------------------------
// Purpose: Kick Player Issue
//-----------------------------------------------------------------------------
class CKickIssue : public CBaseIssue
{
public:
	CKickIssue(const char *typeString);
	~CKickIssue();
	const char *GetDisplayString();
	const char * GetVotePassedString();
	void ListIssueDetails( CBasePlayer* pPlayer );
	bool IsEnabled();
	const char * GetDetailsString();
	void OnVoteStarted();
	void Init();
	void NotifyGC(bool a2);
	int PrintLogData();
	void OnVoteFailed(int a2);
	bool CreateVoteDataFromDetails(const char *s);
	bool CanCallVote( int iEntIndex, const char* pszDetails, vote_create_failed_t& nFailCode, int& nTime );
	void ExecuteCommand();
	bool IsTeamRestrictedVote();

private:
	char		 m_pzPlayerName[64];
	char		 m_pzReason[64];
	int			 m_iPlayerID;
};

//-----------------------------------------------------------------------------
// Purpose: Restart Round Issue
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CRestartGameIssue : public CBaseIssue
{
public:
	CRestartGameIssue( const char* typeString );
	~CRestartGameIssue();

	void		ExecuteCommand();
	bool		IsEnabled();
	void		Init();
	bool		CanCallVote( int iEntIndex, const char* pszDetails, vote_create_failed_t& nFailCode, int& nTime );
	const char* GetDisplayString();
	void		ListIssueDetails( CBasePlayer* pPlayer );
	const char* GetVotePassedString();
};


#endif // TF_INVENTORY_H
