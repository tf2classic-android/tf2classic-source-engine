//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: Shared player code.
//
//=============================================================================
#ifndef TF_PLAYER_SHARED_H
#define TF_PLAYER_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#include "networkvar.h"
#include "tf_shareddefs.h"
#include "tf_weaponbase.h"
#include "basegrenade_shared.h"

// Client specific.
#ifdef CLIENT_DLL
class C_TFPlayer;
// Server specific.
#else
class CTFPlayer;
#endif

//=============================================================================
//
// Tables.
//

// Client specific.
#ifdef CLIENT_DLL

	EXTERN_RECV_TABLE( DT_TFPlayerShared );

// Server specific.
#else

	EXTERN_SEND_TABLE( DT_TFPlayerShared );

#endif


//=============================================================================

#define PERMANENT_CONDITION		-1

// Damage storage for crit multiplier calculation
class CTFDamageEvent
{
	DECLARE_EMBEDDED_NETWORKVAR()
#ifdef CLIENT_DLL
	DECLARE_CLIENTCLASS_NOBASE();
#else
	DECLARE_SERVERCLASS_NOBASE();
#endif

public:
	float flDamage;
	float flTime;
	bool bKill;
};

enum
{
	STUN_PHASE_NONE,
	STUN_PHASE_LOOP,
	STUN_PHASE_END,
};

//=============================================================================
//
// Shared player class.
//
class CTFPlayerShared
{
public:

// Client specific.
#ifdef CLIENT_DLL

	friend class C_TFPlayer;
	typedef C_TFPlayer OuterClass;
	DECLARE_PREDICTABLE();

// Server specific.
#else

	friend class CTFPlayer;
	typedef CTFPlayer OuterClass;

#endif
	
	DECLARE_EMBEDDED_NETWORKVAR()
	DECLARE_CLASS_NOBASE( CTFPlayerShared );

	// Initialization.
	CTFPlayerShared();
	void Init( OuterClass *pOuter );

	// State (TF_STATE_*).
	int		GetState() const					{ return m_nPlayerState; }
	void	SetState( int nState )				{ m_nPlayerState = nState; }
	bool	InState( int nState )				{ return ( m_nPlayerState == nState ); }

	// Condition (TF_COND_*).
	int		GetCond() const						{ return m_nPlayerCond; }
	void	SetCond( ETFCond nCond )				{ m_nPlayerCond = nCond; }
	void	AddCond( ETFCond nCond, float flDuration = PERMANENT_CONDITION );
	void	RemoveCond( ETFCond nCond );
	bool	InCond( ETFCond nCond );
	bool	RemoveCondIfPresent( ETFCond nCond );
	void	RemoveAllCond( CTFPlayer *pPlayer );
	void	OnConditionAdded( ETFCond nCond );
	void	OnConditionRemoved( ETFCond nCond );
	void	ConditionThink( void );
	float	GetConditionDuration( ETFCond nCond );

	bool	IsCritBoosted( void );
	bool	IsMiniCritBoosted( void );
	bool	IsInvulnerable( void );
	bool	IsControlStunned( void );
	bool	IsLoserStateStunned( void ) const;
	bool	IsStealthed( void );

	int	GetPowerupFlags( void );
	
	void	ConditionGameRulesThink( void );

	void	InvisibilityThink( void );

	int		GetMaxBuffedHealth( void );

	bool	HealNegativeConds( void );

#ifdef CLIENT_DLL
	// This class only receives calls for these from C_TFPlayer, not
	// natively from the networking system
	virtual void OnPreDataChanged( void );
	virtual void OnDataChanged( void );

	// check the newly networked conditions for changes
	void	SyncConditions( int nCond, int nOldCond, int nUnused, int iOffset );
#endif

	void	Disguise( int nTeam, int nClass );
	void	CompleteDisguise( void );
	void	RemoveDisguise( void );
	void	FindDisguiseTarget( void );
	int		GetDisguiseTeam( void );
	int		GetDisguiseClass( void ) 			{ return InCond( TF_COND_DISGUISED_AS_DISPENSER ) ? (int)TF_CLASS_ENGINEER : m_nDisguiseClass;; }
	int		GetMaskClass( void )				{ return m_nMaskClass; }
	int		GetDesiredDisguiseClass( void )		{ return m_nDesiredDisguiseClass; }
	int		GetDesiredDisguiseTeam( void )		{ return m_nDesiredDisguiseTeam; }
	EHANDLE GetDisguiseTarget( void ) 	
	{
#ifdef CLIENT_DLL
		if ( m_iDisguiseTargetIndex == TF_DISGUISE_TARGET_INDEX_NONE )
			return NULL;
		return cl_entitylist->GetNetworkableHandle( m_iDisguiseTargetIndex );
#else
		return m_hDisguiseTarget.Get();
#endif
	}
	int		GetDisguiseHealth( void )			{ return m_iDisguiseHealth; }
	int		GetDisguiseTargetIndex( void )			{ return m_iDisguiseTargetIndex; }
	void		SetDisguiseHealth( int iDisguiseHealth );
	int		AddDisguiseHealth( int iHealthToAdd, bool bOverheal = false );
	int		GetDisguiseMaxHealth( void )			{ return m_iDisguiseMaxHealth; }
	int		GetDisguiseMaxBuffedHealth( void );

	CEconItemView	*GetDisguiseItem( void )			{ return &m_DisguiseItem; }
	void		RecalcDisguiseWeapon( bool bForcePrimary = false, int iSlot = 0 );

#ifdef CLIENT_DLL
	void	OnDisguiseChanged( void );
	int		GetDisguiseWeaponModelIndex( void ) { return m_iDisguiseWeaponModelIndex; }
	CTFWeaponInfo *GetDisguiseWeaponInfo( void );

	int	GetTeamTeleporterUsed( void ) { return m_nTeamTeleporterUsed; }
	void	UpdateCritBoostEffect( bool bForceHide = false );
	bool	SetParticleToMercColor( CNewParticleEffect *pParticle );
#endif

#ifdef GAME_DLL
	void	Heal( CTFPlayer *pPlayer, float flAmount, bool bDispenserHeal = false );
	void	StopHealing( CTFPlayer *pPlayer );
	void	RecalculateChargeEffects( bool bInstantRemove = false );
	//TF_MOD_BOT changes
	EHANDLE GetHealerByIndex(int index);
	int		FindHealerIndex( CTFPlayer *pPlayer );
	EHANDLE	GetFirstHealer();
	bool HealerIsDispenser(int index) const;
	void	HealthKitPickupEffects( int iAmount );
#endif
	int		GetNumHealers( void ) { return m_nNumHealers; }

	void	Burn( CTFPlayer *pAttacker, CTFWeaponBase *pWeapon = NULL, float flFlameDuration = -1.0f );
	void	StunPlayer( float flDuration, CTFPlayer *pStunner );

	void	RecalculatePlayerBodygroups( void );

	// Weapons.
	CTFWeaponBase *GetActiveTFWeapon() const;

	// Utility.
	bool	IsAlly( CBaseEntity *pEntity );

	bool	IsLoser( void );

	// Separation force
	bool	IsSeparationEnabled( void ) const	{ return m_bEnableSeparation; }
	void	SetSeparation( bool bEnable )		{ m_bEnableSeparation = bEnable; }
	const Vector &GetSeparationVelocity( void ) const { return m_vSeparationVelocity; }
	void	SetSeparationVelocity( const Vector &vSeparationVelocity ) { m_vSeparationVelocity = vSeparationVelocity; }

	void	FadeInvis( float flInvisFadeTime, bool bNoAttack = false );
	float	GetPercentInvisible( void );
	void	NoteLastDamageTime( int nDamage );
	void	OnSpyTouchedByEnemy( void );
	float	GetLastStealthExposedTime( void ) { return m_flLastStealthExposeTime; }

	int		GetDesiredPlayerClassIndex( void );

	int		GetDesiredWeaponIndex( void ) { return m_iDesiredWeaponID; }
	void	SetDesiredWeaponIndex( int iWeaponID ) { m_iDesiredWeaponID = iWeaponID; }

	float	GetSpyCloakMeter() const		{ return m_flCloakMeter; }
	void	SetSpyCloakMeter( float val ) { m_flCloakMeter = val; }

	bool	IsJumping( void ) { return m_bJumping; }
	void	SetJumping( bool bJumping );
	bool    IsAirDashing( void ) { return m_bAirDash; }
	void    SetAirDash( bool bAirDash );
	int		GetAirDucks( void ) { return m_nAirDucked; }
	void	IncrementAirDucks( void );
	void	ResetAirDucks( void );

	bool	IsHasting( void );
	bool	IsEnemy( CBaseEntity *pEntity );
	void	DebugPrintConditions( void );

	float	GetStealthNoAttackExpireTime( void );

	void	SetPlayerDominated( CTFPlayer *pPlayer, bool bDominated );
	bool	IsPlayerDominated( int iPlayerIndex );
	bool	IsPlayerDominatingMe( int iPlayerIndex );
	void	SetPlayerDominatingMe( CTFPlayer *pPlayer, bool bDominated );

	bool	IsCarryingObject( void ) { return m_bCarryingObject; }

#ifdef GAME_DLL
	void				SetCarriedObject( CBaseObject *pObj );
	CBaseObject*		GetCarriedObject( void );
#endif

	int		GetKillstreak( void ) { return m_nStreaks.Get( 0 ); }
	void	SetKillstreak( int iKillstreak ) { m_nStreaks.Set( 0, iKillstreak ); }
	void	IncKillstreak() { m_nStreaks.Set( 0, m_nStreaks.Get( 0 ) + 1 ); }

	int		GetStunPhase( void ) { return m_iStunPhase; }
	void	SetStunPhase( int iPhase ) { m_iStunPhase = iPhase; }
	float	GetStunExpireTime( void ) { return m_flStunExpireTime; }
	void	SetStunExpireTime( float flTime ) { m_flStunExpireTime = flTime; }

	int		GetTeleporterEffectColor( void ) { return m_nTeamTeleporterUsed; }
	void	SetTeleporterEffectColor( int iTeam ) { m_nTeamTeleporterUsed = iTeam; }
#ifdef CLIENT_DLL
	bool	ShouldShowRecentlyTeleported( void );
#endif

	int GetSequenceForDeath( CBaseAnimating *pAnim, int iDamageCustom );

	const Vector &GetAirblastPosition( void ) { return m_vecAirblastPos.Get(); }
	void SetAirblastPosition( Vector vecPosition ) { m_vecAirblastPos = vecPosition; }
private:

	void ImpactWaterTrace( trace_t &trace, const Vector &vecStart );

	void OnAddStealthed( void );
	void OnAddInvulnerable( void );
	void OnAddTeleported( void );
	void OnAddBurning( void );
	void OnAddDisguising( void );
	void OnAddDisguised( void );
	void OnAddTaunting( void );
	void OnAddSlowed( void );
	void OnAddCritboosted( void );
	void OnAddStunned( void );
	void OnAddHalloweenGiant( void );
	void OnAddHalloweenTiny( void );
	void OnAddRagemode( void );
	void OnAddShield( void );
	void OnAddSpeedBoost( void );
	void OnAddUrine( void );

	void OnRemoveZoomed( void );
	void OnRemoveBurning( void );
	void OnRemoveStealthed( void );
	void OnRemoveDisguised( void );
	void OnRemoveDisguising( void );
	void OnRemoveInvulnerable( void );
	void OnRemoveTeleported( void );
	void OnRemoveTaunting( void );
	void OnRemoveSlowed( void );
	void OnRemoveCritboosted( void );
	void OnRemoveStunned( void );
	void OnRemoveHalloweenGiant( void );
	void OnRemoveHalloweenTiny( void );
	void OnRemoveRagemode( void );
	void OnRemoveShield( void );
	void OnRemoveSpeedBoost( void );
	void OnRemoveUrine( void );

	float GetCritMult( void );

#ifdef GAME_DLL
	void  UpdateCritMult( void );
	void  RecordDamageEvent( const CTakeDamageInfo &info, bool bKill );
	void  ClearDamageEvents( void ) { m_DamageEvents.Purge(); }
	int	  GetNumKillsInTime( float flTime );

	// Invulnerable.
	medigun_charge_types  GetChargeEffectBeingProvided( CTFPlayer *pPlayer );
	void  SetChargeEffect( medigun_charge_types chargeType, bool bShouldCharge, bool bInstantRemove, const MedigunEffects_t &chargeEffect, float flRemoveTime, CTFPlayer *pProvider );
	void  TestAndExpireChargeEffect( medigun_charge_types chargeType );
#endif

private:

	// Vars that are networked.
	CNetworkVar( int, m_nPlayerState );			// Player state.
	CNetworkVar( int, m_nPlayerCond );			// Player condition flags.
	// Ugh...
	CNetworkVar( int, m_nPlayerCondEx );
	CNetworkVar( int, m_nPlayerCondEx2 );
	CNetworkVar( int, m_nPlayerCondEx3 );
	CNetworkArray( float, m_flCondExpireTimeLeft, TF_COND_LAST ); // Time until each condition expires

//TFTODO: What if the player we're disguised as leaves the server?
//...maybe store the name instead of the index?
	CNetworkVar( int, m_nDisguiseTeam );		// Team spy is disguised as.
	CNetworkVar( int, m_nDisguiseClass );		// Class spy is disguised as.
	CNetworkVar( int, m_nMaskClass );			// Fake disguise class.
	EHANDLE m_hDisguiseTarget;					// Playing the spy is using for name disguise.
	CNetworkVar( int, m_iDisguiseTargetIndex );
	CNetworkVar( int, m_iDisguiseHealth );		// Health to show our enemies in player id
	CNetworkVar( int, m_iDisguiseMaxHealth );
	CNetworkVar( float, m_flDisguiseChargeLevel );
	CNetworkVar( int, m_nDesiredDisguiseClass );
	CNetworkVar( int, m_nDesiredDisguiseTeam );
	CEconItemView m_DisguiseItem;

	bool m_bEnableSeparation;		// Keeps separation forces on when player stops moving, but still penetrating
	Vector m_vSeparationVelocity;	// Velocity used to keep player seperate from teammates

	float m_flInvisibility;
	CNetworkVar( float, m_flInvisChangeCompleteTime );		// when uncloaking, must be done by this time
	float m_flLastStealthExposeTime;

	CNetworkVar( int, m_nNumHealers );

	// Vars that are not networked.
	OuterClass			*m_pOuter;					// C_TFPlayer or CTFPlayer (client/server).

#ifdef GAME_DLL
	// Healer handling
	struct healers_t
	{
		EHANDLE	pPlayer;
		float	flAmount;
		bool	bDispenserHeal;
		float	iRecentAmount;
		float	flNextNofityTime;
	};
	CUtlVector< healers_t >	m_aHealers;	
	float					m_flHealFraction;	// Store fractional health amounts
	float					m_flDisguiseHealFraction;	// Same for disguised healing

	float		m_flChargeOffTime[TF_CHARGE_COUNT];
	bool		m_bChargeSounds[TF_CHARGE_COUNT];
#endif

	// Burn handling
	CHandle<CTFPlayer>		m_hBurnAttacker;
	CHandle<CTFWeaponBase>	m_hBurnWeapon;
	CNetworkVar( int,		m_nNumFlames );
	float					m_flFlameBurnTime;
	float					m_flFlameRemoveTime;
	float					m_flTauntRemoveTime;

public:
	// Jarate Player
	EHANDLE m_hUrineAttacker;
private:

	float m_flDisguiseCompleteTime;

	CNetworkVar( int, m_iDesiredPlayerClass );
	CNetworkVar( int, m_iDesiredWeaponID );

	float m_flNextBurningSound;

	CNetworkVar( float, m_flCloakMeter );	// [0,100]

	CNetworkVar( bool, m_bJumping );
	CNetworkVar( bool, m_bAirDash );
	CNetworkVar( int, m_nAirDucked );

	CNetworkVar( float, m_flStealthNoAttackExpire );
	CNetworkVar( float, m_flStealthNextChangeTime );

	CNetworkVar( int, m_iCritMult );

	CNetworkArray( int, m_nStreaks, 3 );

	CNetworkArray( bool, m_bPlayerDominated, MAX_PLAYERS+1 );		// array of state per other player whether player is dominating other players
	CNetworkArray( bool, m_bPlayerDominatingMe, MAX_PLAYERS+1 );	// array of state per other player whether other players are dominating this player

	CNetworkHandle( CTFPlayer, m_hStunner );
	CNetworkVar( float, m_flStunExpireTime );
	int m_iStunPhase;

	CNetworkVector( m_vecAirblastPos );

	CNetworkHandle( CBaseObject, m_hCarriedObject );
	CNetworkVar( bool, m_bCarryingObject );

	CNetworkVar( int, m_nTeamTeleporterUsed );

#ifdef GAME_DLL
	float	m_flNextCritUpdate;
	CUtlVector<CTFDamageEvent> m_DamageEvents;
#else
	int m_iDisguiseWeaponModelIndex;
	int m_iOldDisguiseWeaponModelIndex;
	CTFWeaponInfo *m_pDisguiseWeaponInfo;

	WEAPON_FILE_INFO_HANDLE	m_hDisguiseWeaponInfo;

	CNewParticleEffect *m_pCritEffect;
	CNewParticleEffect *m_pSpeedEffect;
	EHANDLE m_hCritEffectHost;
	CSoundPatch *m_pCritSound;

	int	m_nOldDisguiseClass;
	int m_nOldDisguiseTeam;

	int m_iOldDisguiseWeaponID;

	int	m_nOldConditions;
	int m_nOldConditionsEx;
	int m_nOldConditionsEx2;
	int m_nOldConditionsEx3;

	bool m_bWasCritBoosted;
#endif
};			   

#define TF_DEATH_DOMINATION				0x0001	// killer is dominating victim
#define TF_DEATH_ASSISTER_DOMINATION	0x0002	// assister is dominating victim
#define TF_DEATH_REVENGE				0x0004	// killer got revenge on victim
#define TF_DEATH_ASSISTER_REVENGE		0x0008	// assister got revenge on victim

extern const char *g_pszBDayGibs[22];

#endif // TF_PLAYER_SHARED_H
