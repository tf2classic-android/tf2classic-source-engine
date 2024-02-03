//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:  TF-specific things to vote on
//
//=============================================================================

#ifndef TF_VOTEISSUES_H
#define TF_VOTEISSUES_H

#ifdef _WIN32
#pragma once
#endif

#include "vote_controller.h"

class CTFPlayer;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CBaseTFIssue : public CBaseIssue
{
	// Overrides to BaseIssue standard to this mod.
public:
	CBaseTFIssue(const char *typeString) : CBaseIssue(typeString)
	{
	}
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CRestartGameIssue : public CBaseTFIssue
{
public:
	CRestartGameIssue() : CBaseTFIssue( "RestartGame" ) { } // This string will have "Vote_" glued onto the front for localization (i.e. "#Vote_RestartGame")
	
	virtual void		ExecuteCommand( void ) OVERRIDE;
	virtual bool		IsEnabled( void ) OVERRIDE;
	virtual bool		CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime ) OVERRIDE;
	virtual const char *GetDisplayString( void ) OVERRIDE;
	virtual void		ListIssueDetails( CBasePlayer *forWhom ) OVERRIDE;
	virtual const char *GetVotePassedString( void ) OVERRIDE;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CKickIssue : public CBaseTFIssue
{
public:
	CKickIssue() : CBaseTFIssue( "Kick" ) { } // This string will have "Vote_" glued onto the front for localization (i.e. "#Vote_Kick")
	
	virtual void		ExecuteCommand( void ) OVERRIDE;
	virtual bool		IsEnabled( void ) OVERRIDE;
	virtual bool		CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime ) OVERRIDE;
	virtual const char *GetDisplayString( void ) OVERRIDE;
	virtual void		ListIssueDetails( CBasePlayer *pForWhom ) OVERRIDE;
	virtual const char *GetVotePassedString( void ) OVERRIDE;
	virtual bool		IsTeamRestrictedVote( void ) OVERRIDE				{ return true; }
	virtual void		OnVoteFailed( int iEntityHoldingVote ) OVERRIDE;
	virtual void		OnVoteStarted( void ) OVERRIDE;
	virtual const char *GetDetailsString( void ) OVERRIDE;

private:
	void				Init( void );
	bool				CreateVoteDataFromDetails( const char *pszDetails );
	void				PrintLogData( void );

	int 				m_KickCallerUserID;
	int 				m_KickTargetUserID;

	CSteamID			m_steamIDVoteCaller;
	CSteamID			m_steamIDVoteTarget;
	char				m_szTargetPlayerName[MAX_PLAYER_NAME_LENGTH];
	uint32				m_unKickReason;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CChangeLevelIssue : public CBaseTFIssue
{
public:
	CChangeLevelIssue() : CBaseTFIssue( "ChangeLevel" ) { } // This string will have "Vote_" glued onto the front for localization (i.e. "#Vote_ChangeLevel")
	
	virtual void		ExecuteCommand( void ) OVERRIDE;
	virtual bool		IsEnabled( void ) OVERRIDE;
	virtual bool		CanTeamCallVote( int iTeam ) const OVERRIDE;
	virtual bool		CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime ) OVERRIDE;
	virtual const char *GetDisplayString( void ) OVERRIDE;
	virtual void		ListIssueDetails( CBasePlayer *pForWhom ) OVERRIDE;
	virtual const char *GetVotePassedString( void ) OVERRIDE;
	virtual const char *GetDetailsString( void ) OVERRIDE;
	virtual bool		IsYesNoVote( void ) OVERRIDE;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CNextLevelIssue : public CBaseTFIssue
{
public:
	CNextLevelIssue() : CBaseTFIssue( "NextLevel" ) { } // This string will have "Vote_" glued onto the front for localization (i.e. "#Vote_NextLevel")
	
	virtual void		ExecuteCommand( void ) OVERRIDE;
	virtual bool		IsEnabled( void ) OVERRIDE;
	virtual bool		CanTeamCallVote( int iTeam ) const OVERRIDE;
	virtual bool		CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime ) OVERRIDE;
	virtual const char *GetDisplayString( void ) OVERRIDE;
	virtual void		ListIssueDetails( CBasePlayer *pForWhom ) OVERRIDE;
	virtual const char *GetVotePassedString( void ) OVERRIDE;
	virtual const char *GetDetailsString( void ) OVERRIDE;
	virtual bool		IsYesNoVote( void ) OVERRIDE;
	virtual int			GetNumberVoteOptions( void ) OVERRIDE;
	virtual bool		GetVoteOptions( CUtlVector <const char*> &vecNames ) OVERRIDE;
	virtual float		GetQuorumRatio( void ) OVERRIDE;

private:
	CUtlVector <const char *> m_IssueOptions;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CExtendLevelIssue : public CBaseTFIssue
{
public:
	CExtendLevelIssue() : CBaseTFIssue( "ExtendLevel" ) { } // This string will have "Vote_" glued onto the front for localization (i.e. "#Vote_ExtendLevel")

	virtual void		ExecuteCommand( void ) OVERRIDE;
	virtual bool		IsEnabled( void ) OVERRIDE;
	virtual bool		CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime ) OVERRIDE;
	virtual const char *GetDisplayString( void ) OVERRIDE;
	virtual void		ListIssueDetails( CBasePlayer *pForWhom ) OVERRIDE;
	virtual const char *GetVotePassedString( void ) OVERRIDE;
	virtual float		GetQuorumRatio( void ) OVERRIDE;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CScrambleTeams : public CBaseTFIssue
{
public:
	CScrambleTeams() : CBaseTFIssue( "ScrambleTeams" ) { } // This string will have "Vote_" glued onto the front for localization (i.e. "#Vote_ScrambleTeams")

	virtual void		ExecuteCommand( void ) OVERRIDE;
	virtual bool		IsEnabled( void ) OVERRIDE;
	virtual bool		CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime ) OVERRIDE;
	virtual const char *GetDisplayString( void ) OVERRIDE;
	virtual void		ListIssueDetails( CBasePlayer *pForWhom ) OVERRIDE;
	virtual const char *GetVotePassedString( void ) OVERRIDE;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTeamAutoBalanceIssue : public CBaseTFIssue
{
public:
	CTeamAutoBalanceIssue() : CBaseTFIssue( "TeamAutoBalance" ) { } // This string will have "Vote_" glued onto the front for localization (i.e. "#Vote_TeamAutoBalance")
	
	virtual const char	*GetTypeStringLocalized( void ) OVERRIDE;
	virtual void		ExecuteCommand( void ) OVERRIDE;
	virtual bool		IsEnabled( void ) OVERRIDE;
	virtual bool		CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime ) OVERRIDE;
	virtual const char *GetDisplayString( void ) OVERRIDE;
	virtual void		ListIssueDetails( CBasePlayer *forWhom ) OVERRIDE;
	virtual const char *GetVotePassedString( void ) OVERRIDE;
	virtual float		GetQuorumRatio( void ) OVERRIDE;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CClassLimitsIssue : public CBaseTFIssue
{
public:
	CClassLimitsIssue() : CBaseTFIssue( "ClassLimits" ) { } // This string will have "Vote_" glued onto the front for localization (i.e. "#Vote_ClassLimits")

	virtual const char	*GetTypeStringLocalized( void ) OVERRIDE;
	virtual void		ExecuteCommand( void ) OVERRIDE;
	virtual bool		IsEnabled( void ) OVERRIDE;
	virtual bool		CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime ) OVERRIDE;
	virtual const char *GetDisplayString( void ) OVERRIDE;
	virtual void		ListIssueDetails( CBasePlayer *forWhom ) OVERRIDE;
	virtual const char *GetVotePassedString( void ) OVERRIDE;
	virtual const char *GetDetailsString( void ) OVERRIDE;
private:
	CUtlString m_sRetString;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CPauseGameIssue : public CBaseTFIssue
{
public:
	CPauseGameIssue() : CBaseTFIssue( "PauseGame" ) {}	// This string will have "Vote_" glued onto the front for localization (i.e. "#Vote_PauseGame")

	virtual void		ExecuteCommand( void ) OVERRIDE;
	virtual bool		IsEnabled( void ) OVERRIDE;
	virtual bool		CanCallVote( int iEntIndex, const char *pszDetails, vote_create_failed_t &nFailCode, int &nTime ) OVERRIDE;
	virtual const char *GetDisplayString( void ) OVERRIDE;
	virtual void		ListIssueDetails( CBasePlayer *forWhom ) OVERRIDE;
	virtual const char *GetVotePassedString( void ) OVERRIDE;
	virtual const char *GetDetailsString( void ) OVERRIDE;
private:
	CUtlString m_sRetString;
};


static const char* g_pszVoteKickString = "#TF_Vote_kicked";

#endif // TF_VOTEISSUES_H
