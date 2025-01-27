//=========== (C) Copyright 1999 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// Purpose: The TF Game rules object
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================

#ifndef TF_GAMERULES_H
#define TF_GAMERULES_H

#ifdef _WIN32
#pragma once
#endif


#include "teamplayroundbased_gamerules.h"
#include "convar.h"
#include "gamevars_shared.h"
#include "GameEventListener.h"
#include "tf_gamestats_shared.h"

#ifdef CLIENT_DLL
#include "c_tf_player.h"
#else
#include "tf_player.h"
#include "team_train_watcher.h"
#endif

#ifdef CLIENT_DLL
	
	#define CTFGameRules C_TFGameRules
	#define CTFGameRulesProxy C_TFGameRulesProxy

#else

	#include "bot/map_entities/tf_bot_roster.h"

	extern BOOL no_cease_fire_text;
	extern BOOL cease_fire;

	class CHealthKit;
	class CTFBotRoster;
	class CMedievalLogic;

#endif

extern ConVar	tf_avoidteammates;
extern ConVar	tf_avoidteammates_pushaway;

extern ConVar	fraglimit;

extern Vector g_TFClassViewVectors[];

extern ConVar mp_humans_must_join_team;

#ifdef GAME_DLL
class CTFRadiusDamageInfo
{
public:
	CTFRadiusDamageInfo();

	bool	ApplyToEntity( CBaseEntity *pEntity );

public:
	CTakeDamageInfo info;
	Vector m_vecSrc;
	float m_flRadius;
	float m_flSelfDamageRadius;
	int m_iClassIgnore;
	CBaseEntity *m_pEntityIgnore;
	bool m_bStockSelfDamage;
};
#endif

class CTFGameRulesProxy : public CTeamplayRoundBasedRulesProxy
{
public:
	DECLARE_CLASS( CTFGameRulesProxy, CTeamplayRoundBasedRulesProxy );
	DECLARE_NETWORKCLASS();

#ifdef GAME_DLL
	DECLARE_DATADESC();
	void	InputSetRedTeamRespawnWaveTime( inputdata_t &inputdata );
	void	InputSetBlueTeamRespawnWaveTime( inputdata_t &inputdata );
	void	InputSetGreenTeamRespawnWaveTime(inputdata_t &inputdata);
	void	InputSetYellowTeamRespawnWaveTime(inputdata_t &inputdata);
	void	InputAddRedTeamRespawnWaveTime( inputdata_t &inputdata );
	void	InputAddBlueTeamRespawnWaveTime( inputdata_t &inputdata );
	void	InputAddGreenTeamRespawnWaveTime(inputdata_t &inputdata);
	void	InputAddYellowTeamRespawnWaveTime(inputdata_t &inputdata);
	void	InputSetRedTeamGoalString( inputdata_t &inputdata );
	void	InputSetBlueTeamGoalString( inputdata_t &inputdata );
	void	InputSetGreenTeamGoalString(inputdata_t &inputdata);
	void	InputSetYellowTeamGoalString(inputdata_t &inputdata);
	void	InputSetRedTeamRole( inputdata_t &inputdata );
	void	InputSetBlueTeamRole( inputdata_t &inputdata );
	void	InputSetGreenTeamRole( inputdata_t &inputdata );
	void	InputSetYellowTeamRole( inputdata_t &inputdata );
	void	InputAddRedTeamScore( inputdata_t &inputdata );
	void	InputAddBlueTeamScore( inputdata_t &inputdata );
	void	InputAddGreenTeamScore( inputdata_t &inputdata );
	void	InputAddYellowTeamScore( inputdata_t &inputdata );

	void	InputSetRedKothClockActive( inputdata_t &inputdata );
	void	InputSetBlueKothClockActive( inputdata_t &inputdata );
	void	InputSetGreenKothClockActive( inputdata_t &inputdata );
	void	InputSetYellowKothClockActive( inputdata_t &inputdata );

	void	InputSetCTFCaptureBonusTime( inputdata_t &inputdata );

	void	InputPlayVO( inputdata_t &inputdata );
	void	InputPlayVORed( inputdata_t &inputdata );
	void	InputPlayVOBlue( inputdata_t &inputdata );
	void	InputPlayVOGreen( inputdata_t &inputdata );
	void	InputPlayVOYellow( inputdata_t &inputdata );

	virtual void Activate();

	int		m_iHud_Type;
	bool	m_bFourTeamMode;
	bool	m_bCTF_Overtime;

#endif
};

struct PlayerRoundScore_t
{
	int iPlayerIndex;	// player index
	int iRoundScore;	// how many points scored this round
	int	iTotalScore;	// total points scored across all rounds
	int	iKills;
	int iDeaths;
};

#define MAX_TEAMGOAL_STRING		256

class CTFGameRules : public CTeamplayRoundBasedRules
{
public:
	DECLARE_CLASS( CTFGameRules, CTeamplayRoundBasedRules );

	CTFGameRules();

	enum
	{
		HALLOWEEN_SCENARIO_DOOMSDAY
	};

	// Damage Queries.
	virtual bool	Damage_IsTimeBased( int iDmgType );			// Damage types that are time-based.
	virtual bool	Damage_ShowOnHUD( int iDmgType );				// Damage types that have client HUD art.
	virtual bool	Damage_ShouldNotBleed( int iDmgType );			// Damage types that don't make the player bleed.
	// TEMP:
	virtual int		Damage_GetTimeBased( void );		
	virtual int		Damage_GetShowOnHud( void );
	virtual int		Damage_GetShouldNotBleed( void );

	int				GetFarthestOwnedControlPoint( int iTeam, bool bWithSpawnpoints );
	virtual bool	TeamMayCapturePoint( int iTeam, int iPointIndex );
	virtual bool	PlayerMayCapturePoint( CBasePlayer *pPlayer, int iPointIndex, char *pszReason = NULL, int iMaxReasonLength = 0 );
	virtual bool	PlayerMayBlockPoint( CBasePlayer *pPlayer, int iPointIndex, char *pszReason = NULL, int iMaxReasonLength = 0 );
	
	static int		CalcPlayerScore( RoundStats_t *pRoundStats );

	bool			IsBirthday( void );

	virtual const unsigned char *GetEncryptionKey( void ) { return (unsigned char *)"E2NcUkG2"; }

	virtual bool	AllowThirdPersonCamera( void );

	virtual float	GetRespawnWaveMaxLength( int iTeam, bool bScaleWithNumPlayers = true );

	virtual bool	ShouldBalanceTeams( void );

	CTeamRoundTimer *GetKothTeamTimer( int iTeam )
	{
		if( !IsInKothMode() )
			return NULL;

		switch( iTeam )
		{
			case TF_TEAM_RED:
				return m_hRedKothTimer.Get();
				break;

			case TF_TEAM_BLUE:
				return m_hBlueKothTimer.Get();
				break;

			case TF_TEAM_GREEN:
				return m_hGreenKothTimer.Get();
				break;

			case TF_TEAM_YELLOW:
				return m_hYellowKothTimer.Get();
				break;

			default:
				Assert( 0 );
		};

		return NULL;
	}

#ifdef GAME_DLL
public:
	// Override this to prevent removal of game specific entities that need to persist
	virtual bool	RoundCleanupShouldIgnore( CBaseEntity *pEnt );
	virtual bool	ShouldCreateEntity( const char *pszClassName );
	virtual void	CleanUpMap( void );

	virtual void	FrameUpdatePostEntityThink();

	// Called when a new round is being initialized
	virtual void	SetupOnRoundStart( void );

	// Called when a new round is off and running
	virtual void	SetupOnRoundRunning( void );

	virtual void	PreRound_End( void );

	// Called before a new round is started (so the previous round can end)
	virtual void	PreviousRoundEnd( void );

	// Send the team scores down to the client
	virtual void	SendTeamScoresEvent( void ) { return; }

	// Send the end of round info displayed in the win panel
	virtual void	SendDeathmatchResults( void );
	
	// Send the end of round info displayed in the win panel
	virtual void	SendWinPanelInfo( void );

	// Setup spawn points for the current round before it starts
	virtual void	SetupSpawnPointsForRound( void );

	// Called when a round has entered stalemate mode (timer has run out)
	virtual void	SetupOnStalemateStart( void );
	virtual void	SetupOnStalemateEnd( void );

	void			RecalculateControlPointState( void );

	virtual void	HandleSwitchTeams( void );
	virtual void	HandleScrambleTeams( void );
	bool			CanChangeClassInStalemate( void );

	virtual void	SetRoundOverlayDetails( void );	
	virtual void	ShowRoundInfoPanel( CTFPlayer *pPlayer = NULL ); // NULL pPlayer means show the panel to everyone

	virtual bool	TimerMayExpire( void );

	void			HandleCTFCaptureBonus( int iTeam );

	virtual void	Arena_NotifyTeamSizeChange( void );
	virtual int		Arena_PlayersNeededForMatch( void );
	virtual void	Arena_ResetLosersScore( bool bUnknown );
	virtual void	Arena_RunTeamLogic( void );

	virtual void	Activate();

	virtual void	SetHudType(int iHudType){ m_nHudType = iHudType; };

	virtual bool	AllowDamage( CBaseEntity *pVictim, const CTakeDamageInfo &info );

	virtual int		GetClassLimit( int iDesiredClassIndex, int iTeam );
	virtual bool	CanPlayerChooseClass( CBasePlayer *pPlayer, int iDesiredClassIndex );
	bool			CanBotChangeClass( CBasePlayer *pPlayer );
	virtual bool	CanBotChooseClass( CBasePlayer *pPlayer, int iDesiredClassIndex );

	void			SetTeamGoalString( int iTeam, const char *pszGoal );

	// Speaking, vcds, voice commands.
	virtual void	InitCustomResponseRulesDicts();
	virtual void	ShutdownCustomResponseRulesDicts();

	virtual bool	HasPassedMinRespawnTime( CBasePlayer *pPlayer );

	bool			ShouldScorePerRound( void );

	virtual bool	IsValveMap( void );

	virtual int		PlayerRelationship(CBaseEntity *pPlayer, CBaseEntity *pTarget);

	virtual void	PlayTrainCaptureAlert( CTeamControlPoint *pPoint, bool bFinalPointInMap );

	// populate vector with set of control points the player needs to capture
	virtual void CollectCapturePoints( CBasePlayer *player, CUtlVector< CTeamControlPoint * > *captureVector ) const;

	// populate vector with set of control points the player needs to defend from capture
	virtual void CollectDefendPoints( CBasePlayer *player, CUtlVector< CTeamControlPoint * > *defendVector ) const;

	CObjectSentrygun *FindSentryGunWithMostKills( int team = TEAM_ANY ) const;

	virtual bool	ClientConnected(edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen);

	void SetKothTeamTimer( int iTeam, CTeamRoundTimer *pTimer )
	{
		switch( iTeam )
		{
			case TF_TEAM_RED:
				m_hRedKothTimer.Set( pTimer );
				break;

			case TF_TEAM_BLUE:
				m_hBlueKothTimer.Set( pTimer );
				break;

			case TF_TEAM_GREEN:
				m_hGreenKothTimer.Set( pTimer );
				break;

			case TF_TEAM_YELLOW:
				m_hYellowKothTimer.Set( pTimer );
				break;

			default:
				Assert( 0 );
		};
	}

protected:
	virtual void	InitTeams( void );

	virtual void	RoundRespawn( void );

	virtual void	InternalHandleTeamWin( int iWinningTeam );
	
	static int		PlayerRoundScoreSortFunc( const PlayerRoundScore_t *pRoundScore1, const PlayerRoundScore_t *pRoundScore2 );

	virtual void	FillOutTeamplayRoundWinEvent( IGameEvent *event );

	virtual bool	CanChangelevelBecauseOfTimeLimit( void );
	virtual bool	CanGoToStalemate( void );
#endif // GAME_DLL

public:
	// Return the value of this player towards capturing a point
	virtual int		GetCaptureValueForPlayer( CBasePlayer *pPlayer );

	// Collision and Damage rules.
	virtual bool	ShouldCollide( int collisionGroup0, int collisionGroup1 );
	
	int GetTimeLeft( void );

	// Get the view vectors for this mod.
	virtual const CViewVectors *GetViewVectors() const;

	virtual void FireGameEvent( IGameEvent *event );

	virtual const char *GetGameTypeName( void ){ return g_aGameTypeNames[m_nGameType]; }
	virtual int GetGameType( void ){ return m_nGameType; }

	virtual bool FlagsMayBeCapped( void );

	void	RunPlayerConditionThink ( void );

	const char *GetTeamGoalString( int iTeam );

	virtual int		GetHudType( void ){ return m_nHudType; };

	virtual bool	IsMultiplayer( void ){ return true; };

	virtual bool	IsConnectedUserInfoChangeAllowed( CBasePlayer *pPlayer );

	virtual bool	IsFourTeamGame( void ){ return m_bFourTeamMode; };
	virtual bool	IsDeathmatch( void ){ return m_nGameType == TF_GAMETYPE_DM; };
	virtual bool    IsMannVsMachineMode( void ) { return false; };
	virtual bool	IsInArenaMode( void ) const { return m_nGameType == TF_GAMETYPE_ARENA; }
	virtual bool	IsInKothMode( void ) const { return m_bPlayingKoth; }
	virtual bool IsInMedievalMode( void ) const { return m_bPlayingMedieval; }
	virtual bool    IsHalloweenScenario( int iEventType ) { return false; };
	virtual bool	IsPVEModeActive( void ) { return false; };
	virtual bool	IsCompetitiveMode( void ){ return m_bCompetitiveMode; };
	virtual bool	IsInHybridCTF_CPMode( void ){ return m_bPlayingHybrid_CTF_CP; };
	virtual bool	IsInDomination( void ) { return m_bPlayingDomination; }
	virtual bool	IsInSpecialDeliveryMode( void ){ return m_bPlayingSpecialDeliveryMode; };

	virtual bool	IsInstagib( void ) { return m_bInstagib; }

	int		GetFragLimit();

#ifdef CLIENT_DLL

	DECLARE_CLIENTCLASS_NOBASE(); // This makes data tables able to access our private vars.

	virtual void	OnDataChanged( DataUpdateType_t updateType );
	virtual void	HandleOvertimeBegin();
	virtual void	ModifySentChat( char *pBuf, int iBufSize );
	virtual void	GetTeamGlowColor( int nTeam, float &r, float &g, float &b );


	bool			ShouldShowTeamGoal( void );

	const char *GetVideoFileForMap( bool bWithExtension = true );

#else

	DECLARE_SERVERCLASS_NOBASE(); // This makes data tables able to access our private vars.
	
	virtual ~CTFGameRules();

	virtual bool ClientCommand( CBaseEntity *pEdict, const CCommand &args );
	virtual void LevelShutdown();
	virtual void Think();

	bool CheckWinLimit();
	bool CheckFragLimit();
	bool CheckCapsPerRound();

	virtual bool FPlayerCanTakeDamage( CBasePlayer *pPlayer, CBaseEntity *pAttacker, const CTakeDamageInfo &info );

	// Spawing rules.
	CBaseEntity *GetPlayerSpawnSpot( CBasePlayer *pPlayer );
	bool IsSpawnPointValid( CBaseEntity *pSpot, CBasePlayer *pPlayer, bool bIgnorePlayers );

	virtual float FlItemRespawnTime( CItem *pItem );
	virtual Vector VecItemRespawnSpot( CItem *pItem );
	virtual QAngle VecItemRespawnAngles( CItem *pItem );
	
	virtual bool CanHaveItem( CBasePlayer *pPlayer, CItem *pItem );
	virtual void PlayerGotItem( CBasePlayer *pPlayer, CItem *pItem );

	virtual const char *GetChatFormat( bool bTeamOnly, CBasePlayer *pPlayer );
	void ClientSettingsChanged( CBasePlayer *pPlayer );
	virtual void GetTaggedConVarList( KeyValues *pCvarTagList );
	void ChangePlayerName( CTFPlayer *pPlayer, const char *pszNewName );

	virtual VoiceCommandMenuItem_t *VoiceCommand( CBaseMultiplayerPlayer *pPlayer, int iMenu, int iItem ); 

	bool IsInPreMatch() const;
	float GetPreMatchEndTime() const;	// Returns the time at which the prematch will be over.
	void GoToIntermission( void );

	virtual int GetAutoAimMode()	{ return AUTOAIM_NONE; }

	bool CanHaveAmmo( CBaseCombatCharacter *pPlayer, int iAmmoIndex );

	virtual const char *GetGameDescription( void );
	
	const CUtlVector< CHandle< CBaseEntity > > &GetHealthEntityVector( void ); // return vector of health entities
	const CUtlVector< CHandle< CBaseEntity > > &GetAmmoEntityVector( void ); // return vector of ammo entities

	CHandle< CTeamTrainWatcher > GetPayloadToPush( int pushingTeam ) const;			// return the train watcher for the Payload cart the given team needs to push to win, or NULL if none currently exists
	CHandle< CTeamTrainWatcher > GetPayloadToBlock( int blockingTeam ) const;		// return the train watcher for the Payload cart the given team needs to block from advancing, or NULL if none currently exists

	// Sets up g_pPlayerResource.
	virtual void CreateStandardEntities();

	virtual void PlayerKilled( CBasePlayer *pVictim, const CTakeDamageInfo &info );
	virtual void DeathNotice( CBasePlayer *pVictim, const CTakeDamageInfo &info );
	virtual CBasePlayer *GetDeathScorer( CBaseEntity *pKiller, CBaseEntity *pInflictor, CBaseEntity *pVictim );

	void CalcDominationAndRevenge( CTFPlayer *pAttacker, CTFPlayer *pVictim, bool bIsAssist, int *piDeathFlags );

	const char *GetKillingWeaponName( const CTakeDamageInfo &info, CTFPlayer *pVictim, ETFWeaponID &iWeaponID );
	CBasePlayer *GetAssister( CBasePlayer *pVictim, CBasePlayer *pScorer, CBaseEntity *pInflictor );
	CTFPlayer *GetRecentDamager( CTFPlayer *pVictim, int iDamager, float flMaxElapsed );

	void	ManageServerSideVoteCreation( void );

	virtual void ClientDisconnected( edict_t *pClient );

	void	RadiusDamage( CTFRadiusDamageInfo &radiusInfo );
	bool	RadiusJarEffect( CTFRadiusDamageInfo &radiusInfo, ETFCond iCond );
	virtual void  RadiusDamage( const CTakeDamageInfo &info, const Vector &vecSrc, float flRadius, int iClassIgnore, CBaseEntity *pEntityIgnore );

	virtual float FlPlayerFallDamage( CBasePlayer *pPlayer );

	virtual bool  FlPlayerFallDeathDoesScreenFade( CBasePlayer *pl ) { return false; }

	virtual bool UseSuicidePenalty() { return false; }

	int		GetPreviousRoundWinners( void ) { return m_iPreviousRoundWinners; }

	void	SendHudNotification( IRecipientFilter &filter, HudNotification_t iType );
	void	SendHudNotification( IRecipientFilter &filter, const char *pszText, const char *pszIcon, int iTeam = TEAM_UNASSIGNED );

	void	OnNavMeshLoad( void );
	
	void OnDispenserBuilt( CBaseEntity *dispenser );
	void OnDispenserDestroyed( CBaseEntity *dispenser );

	bool	Domination_RunLogic();

	//-----------------------------------------------------------------------------
	// Purpose: Restrict team human players can join
	//-----------------------------------------------------------------------------
	int GetAssignedHumanTeam( void )
	{
		if( FStrEq( "blue", mp_humans_must_join_team.GetString() ) )
		{
			return TF_TEAM_BLUE;
		}
		else if( FStrEq( "red", mp_humans_must_join_team.GetString() ) )
		{
			return TF_TEAM_RED;
		}
		else if( FStrEq( "green", mp_humans_must_join_team.GetString() ) && IsFourTeamGame() )
		{
			return TF_TEAM_GREEN;
		}
		else if( FStrEq( "yellow", mp_humans_must_join_team.GetString() ) && IsFourTeamGame() )
		{
			return TF_TEAM_YELLOW;
		}
		else if( FStrEq( "spectator", mp_humans_must_join_team.GetString() ) )
		{
			return TEAM_SPECTATOR;
		}
		else
		{
			return TEAM_ANY;
		}
	}

private:

	int DefaultFOV( void ) { return 75; }

#endif

private:

	void ComputeHealthAndAmmoVectors( void ); // compute internal vectors of health and ammo locations
	bool m_areHealthAndAmmoVectorsReady;
	
#ifdef GAME_DLL
	mutable CHandle< CTeamTrainWatcher > m_redPayloadToPush;
	mutable CHandle< CTeamTrainWatcher > m_bluePayloadToPush;
	mutable CHandle< CTeamTrainWatcher > m_redPayloadToBlock;
	mutable CHandle< CTeamTrainWatcher > m_bluePayloadToBlock;

	Vector2D	m_vecPlayerPositions[MAX_PLAYERS];

	CUtlVector<CHandle<CHealthKit> > m_hDisabledHealthKits;	
	
	char	m_szMostRecentCappers[MAX_PLAYERS+1];	// list of players who made most recent capture.  Stored as string so it can be passed in events.
	int		m_iNumCaps[TF_TEAM_COUNT];				// # of captures ever by each team during a round

	int SetCurrentRoundStateBitString();
	void SetMiniRoundBitMask( int iMask );
	int m_iPrevRoundState;	// bit string representing the state of the points at the start of the previous miniround
	int m_iCurrentRoundState;
	int m_iCurrentMiniRoundMask;
	float m_flTimerMayExpireAt;

	CountdownTimer m_botCountTimer;
	
	CUtlVector< CHandle< CBaseEntity > > m_ammoVector; // vector of active ammo entities
	bool m_isAmmoVectorReady; // for lazy evaluation
	
	CUtlVector< CHandle< CBaseEntity > > m_healthVector; // vector of active health entities
	bool m_isHealthVectorReady; // for lazy evaluation

	bool m_bFirstBlood;
	int	m_iArenaTeamCount;
#endif

	float m_flNextDominationThink;

	CNetworkVar( int, m_nGameType ); // Type of game this map is (CTF, CP)
	CNetworkString( m_pszTeamGoalStringRed, MAX_TEAMGOAL_STRING );
	CNetworkString( m_pszTeamGoalStringBlue, MAX_TEAMGOAL_STRING );
	CNetworkString( m_pszTeamGoalStringGreen, MAX_TEAMGOAL_STRING );
	CNetworkString( m_pszTeamGoalStringYellow, MAX_TEAMGOAL_STRING );
	CNetworkVar( float, m_flCapturePointEnableTime );
	CNetworkVar( int, m_nHudType );
	CNetworkVar( bool, m_bPlayingKoth );
	CNetworkVar( bool, m_bPlayingMedieval );
	CNetworkVar( bool, m_bPlayingSpecialDeliveryMode );
	CNetworkVar( bool, m_bPlayingRobotDestructionMode );
	CNetworkVar( bool, m_bPlayingMannVsMachine );
	CNetworkVar( bool, m_bPlayingDomination );
	CNetworkVar( bool, m_bPlayingHybrid_CTF_CP );
	CNetworkVar( bool, m_bCompetitiveMode );
	CNetworkVar( bool, m_bPowerupMode );
	CNetworkVar( CHandle<CTeamRoundTimer>, m_hBlueKothTimer );
	CNetworkVar( CHandle<CTeamRoundTimer>, m_hRedKothTimer );
	CNetworkVar( CHandle<CTeamRoundTimer>, m_hGreenKothTimer );
	CNetworkVar( CHandle<CTeamRoundTimer>, m_hYellowKothTimer );
	CNetworkVar( bool, m_bInstagib );

public:

	bool m_bControlSpawnsPerTeam[ MAX_TEAMS ][ MAX_CONTROL_POINTS ];
	int	 m_iPreviousRoundWinners;

	int		m_iBirthdayMode;

	CNetworkVar( bool, m_bFourTeamMode );

#ifdef GAME_DLL
	float	m_flCTFBonusTime;

	// for bot rosters
	CHandle<CTFBotRoster> m_hBlueBotRoster;
	CHandle<CTFBotRoster> m_hRedBotRoster;
	CHandle<CTFBotRoster> m_hGreenBotRoster;
	CHandle<CTFBotRoster> m_hYellowBotRoster;

	// Automatic vote called near the end of a map
	bool    m_bVoteCalled;
	bool    m_bServerVoteOnReset;
	float   m_flVoteCheckThrottle;
#endif

};

//-----------------------------------------------------------------------------
// Gets us at the team fortress game rules
//-----------------------------------------------------------------------------

inline CTFGameRules* TFGameRules()
{
	return static_cast<CTFGameRules*>(g_pGameRules);
}

#ifdef GAME_DLL
	bool EntityPlacementTest( CBaseEntity *pMainEnt, const Vector &vOrigin, Vector &outPos, bool bDropToGround );
#endif

#ifdef CLIENT_DLL
	void AddSubKeyNamed( KeyValues *pKeys, const char *pszName );
#endif

#endif // TF_GAMERULES_H
