//====== Copyright © 1996-2003, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef C_TF_PLAYER_H
#define C_TF_PLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_playeranimstate.h"
#include "c_baseplayer.h"
#include "tf_shareddefs.h"
#include "baseparticleentity.h"
#include "tf_player_shared.h"
#include "c_tf_playerclass.h"
#include "tf_item.h"
#include "props_shared.h"
#include "hintsystem.h"
#include "c_playerattachedmodel.h"
#include "iinput.h"
#include "tf_weapon_medigun.h"
#include "ihasattributes.h"
#include "c_tf_spymask.h"
#include "c_tf_team.h"

class C_MuzzleFlashModel;
class C_BaseObject;

extern ConVar tf_medigun_autoheal;
extern ConVar cl_autorezoom;
extern ConVar cl_autoreload;
extern ConVar cl_flipviewmodels;

extern ConVar tf2c_zoom_hold;
extern ConVar tf2c_setmerccolor_r;
extern ConVar tf2c_setmerccolor_g;
extern ConVar tf2c_setmerccolor_b;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_TFPlayer : public C_BasePlayer, public IHasAttributes
{
public:

	DECLARE_CLASS( C_TFPlayer, C_BasePlayer );
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();

	C_TFPlayer();
	~C_TFPlayer();

	static C_TFPlayer* GetLocalTFPlayer();

	virtual void UpdateOnRemove( void );

	virtual const QAngle& GetRenderAngles();
	virtual void UpdateClientSideAnimation();
	virtual void SetDormant( bool bDormant );
	virtual void OnPreDataChanged( DataUpdateType_t updateType );
	virtual void OnDataChanged( DataUpdateType_t updateType );
	virtual void ProcessMuzzleFlashEvent();
	virtual void ValidateModelIndex( void );

	virtual Vector GetObserverCamOrigin( void );
	virtual int DrawModel( int flags );

	virtual bool CreateMove( float flInputSampleTime, CUserCmd *pCmd );

	virtual bool IsAllowedToSwitchWeapons( void );

	virtual void ClientThink();

	// Deal with recording
	virtual void GetToolRecordingState( KeyValues *msg );

	CTFWeaponBase *GetActiveTFWeapon( void ) const;
	bool		 IsActiveTFWeapon(int iWeaponID);

	virtual void Simulate( void );
	virtual void FireEvent( const Vector& origin, const QAngle& angles, int event, const char *options );
	C_TFTeam *GetTFTeam( void );
	virtual void PlayStepSound( Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force );

	bool IsVIP();

	void JumpSound();

	void LoadInventory(void);
	void EditInventory(int iSlot, int iWeapon);

	void FireBullet( const FireBulletsInfo_t &info, bool bDoEffects, int nDamageType, int nCustomDamageType = TF_DMG_CUSTOM_NONE );

	void ImpactWaterTrace( trace_t &trace, const Vector &vecStart );

	bool CanAttack( void );

	C_TFPlayerClass *GetPlayerClass( void )		{ return &m_PlayerClass; }
	bool IsPlayerClass( int iClass );
	virtual int GetMaxHealth( void ) const;

	virtual int GetRenderTeamNumber( void );

	bool IsWeaponLowered( void );

	void	AvoidPlayers( CUserCmd *pCmd );

	// Get the ID target entity index. The ID target is the player that is behind our crosshairs, used to
	// display the player's name.
	void UpdateIDTarget();
	int GetIDTarget() const;
	void SetForcedIDTarget( int iTarget );

	void SetAnimation( PLAYER_ANIM playerAnim );

	virtual float GetMinFOV() const;

	virtual const QAngle& EyeAngles();

	int GetBuildResources( void );

	// MATTTODO: object selection if necessary
	void SetSelectedObject( C_BaseObject *pObject ) {}

	void GetTeamColor( Color &color );

	virtual void ComputeFxBlend( void );

	// Taunts/VCDs
	virtual bool	StartSceneEvent( CSceneEventInfo *info, CChoreoScene *scene, CChoreoEvent *event, CChoreoActor *actor, C_BaseEntity *pTarget );
	virtual void	CalcView( Vector &eyeOrigin, QAngle &eyeAngles, float &zNear, float &zFar, float &fov );
	bool			StartGestureSceneEvent( CSceneEventInfo *info, CChoreoScene *scene, CChoreoEvent *event, CChoreoActor *actor, CBaseEntity *pTarget );
	void			TurnOnTauntCam( void );
	void			TurnOffTauntCam( void );
	void			TauntCamInterpolation( void );
	bool			InTauntCam( void ) { return m_bWasTaunting; }
	virtual void	ThirdPersonSwitch( bool bThirdperson );

	virtual void	InitPhonemeMappings();

	void			UpdateGlowEffect( void );
	void			DestroyGlowEffect( void );
	void			UpdateClientSideGlow( void );
	void			UpdateGlowColor( void );
	virtual void	GetGlowEffectColor( float *r, float *g, float *b );

	float GetDesaturationAmount();

	// Gibs.
	void InitPlayerGibs( void );
	void CreatePlayerGibs( const Vector &vecOrigin, const Vector &vecVelocity, float flImpactScale, bool bBurning = false );
	void DropPartyHat( breakablepropparams_t &breakParams, Vector &vecBreakVelocity );

	int	GetObjectCount( void );
	C_BaseObject *GetObject( int index );
	C_BaseObject *GetObjectOfType( int iObjectType, int iObjectMode );
	int GetNumObjects( int iObjectType, int iObjectMode );

	virtual bool ShouldCollide( int collisionGroup, int contentsMask ) const;

	float GetPercentInvisible( void );
	float GetEffectiveInvisibilityLevel( void );	// takes viewer into account

	virtual void AddDecal( const Vector& rayStart, const Vector& rayEnd,
		const Vector& decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t& tr, int maxLODToDecal = ADDDECAL_TO_ALL_LODS );

	virtual void CalcDeathCamView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov);
	virtual Vector GetChaseCamViewOffset( CBaseEntity *target );

	void CalcMinViewmodelOffset( void ); // Minimal Viewmodels

	void ClientPlayerRespawn( void );

	void CreateSaveMeEffect( void );

	virtual void	RemoveAmmo( int iCount, int iAmmoIndex );
	virtual int	GetAmmoCount( int iAmmoIndex ) const;

	bool		HasInfiniteAmmo() const;

	virtual bool	IsOverridingViewmodel( void );
	virtual int		DrawOverriddenViewmodel( C_BaseViewModel *pViewmodel, int flags );

	void			SetHealer( C_TFPlayer *pHealer, float flChargeLevel );
	void			GetHealer( C_TFPlayer **pHealer, float *flChargeLevel ) { *pHealer = m_hHealer; *flChargeLevel = m_flHealerChargeLevel; }
	float			MedicGetChargeLevel( void );
	CBaseEntity		*MedicGetHealTarget( void );

	void			StartBurningSound( void );
	void			StopBurningSound( void );
	void			UpdateRecentlyTeleportedEffect( void );

	bool			CanShowClassMenu( void );

	void			InitializePoseParams( void );
	void			UpdateLookAt( void );

	bool			IsEnemyPlayer( void );
	void			ShowNemesisIcon( bool bShow );

	CUtlVector<EHANDLE>		*GetSpawnedGibs( void ) { return &m_hSpawnedGibs; }

	Vector 			GetClassEyeHeight( void );

	void			ForceUpdateObjectHudState( void );

	bool			GetMedigunAutoHeal( void ){ return tf_medigun_autoheal.GetBool(); }
	bool			ShouldAutoRezoom( void ){ return cl_autorezoom.GetBool(); }
	bool			ShouldAutoReload( void ){ return cl_autoreload.GetBool(); }
	bool			ShouldFlipViewModel( void ) { return cl_flipviewmodels.GetBool(); }
	bool			ShouldHoldToZoom( void ) { return tf2c_zoom_hold.GetBool(); }

public:
	// Shared functions
	void			TeamFortress_SetSpeed();
	bool			HasItem( void ) const;					// Currently can have only one item at a time.
	void			SetItem( C_TFItem *pItem );
	C_TFItem		*GetItem( void ) const;
	bool			IsAllowedToPickUpFlag( void );
	bool			HasTheFlag( void ) const;
	float			GetCritMult( void ) { return m_Shared.GetCritMult(); }

	virtual void	ItemPostFrame( void );

	void			SetOffHandWeapon( CTFWeaponBase *pWeapon );
	void			HolsterOffHandWeapon( void );

	virtual int GetSkin();

	virtual bool		Weapon_ShouldSetLast( CBaseCombatWeapon *pOldWeapon, CBaseCombatWeapon *pNewWeapon );
	virtual	bool		Weapon_Switch( C_BaseCombatWeapon *pWeapon, int viewmodelindex = 0 );

	CWeaponMedigun		*GetMedigun( void );
	CTFWeaponBase		*Weapon_OwnsThisID( ETFWeaponID iWeaponID );
	CTFWeaponBase		*Weapon_GetWeaponByType( int iType );
	virtual bool		Weapon_SlotOccupied( CBaseCombatWeapon *pWeapon );
	virtual CBaseCombatWeapon *Weapon_GetSlot( int slot ) const;
	C_EconWearable		*GetEntityForLoadoutSlot( ETFLoadoutSlot iSlot );
	C_TFWeaponBase		*GetWeaponForLoadoutSlot( ETFLoadoutSlot iSlot );
	C_EconWearable		*GetWearableForLoadoutSlot( ETFLoadoutSlot iSlot );

	virtual void		GetStepSoundVelocities( float *velwalk, float *velrun );
	virtual void		SetStepSoundTime( stepsoundtimes_t iStepSoundTime, bool bWalking );

	bool	DoClassSpecialSkill( void );
	bool	CanGoInvisible( void );

	int		GetMaxAmmo( int iAmmoIndex, int iClassNumber = -1 );

	bool	CanPickupBuilding( C_BaseObject *pObject );

	virtual CAttributeManager *GetAttributeManager( void ) { return &m_AttributeManager; }
	virtual CAttributeContainer *GetAttributeContainer( void ) { return NULL; }
	virtual CBaseEntity *GetAttributeOwner( void ) { return NULL; }
	virtual void ReapplyProvision( void ) { /*Do nothing*/ };

public:
	// Ragdolls.
	virtual C_BaseAnimating *BecomeRagdollOnClient();
	virtual IRagdoll		*GetRepresentativeRagdoll() const;
	EHANDLE	m_hRagdoll;
	Vector m_vecRagdollVelocity;

	// Objects
	int CanBuild( int iObjectType, int iObjectMode );
	CUtlVector< CHandle<C_BaseObject> > m_aObjects;

	virtual CStudioHdr *OnNewModel( void );

	void				DisplaysHintsForTarget( C_BaseEntity *pTarget );

	// Shadows
	virtual ShadowType_t ShadowCastType( void ) ;
	virtual void GetShadowRenderBounds( Vector &mins, Vector &maxs, ShadowType_t shadowType );
	virtual void GetRenderBounds( Vector& theMins, Vector& theMaxs );
	virtual bool GetShadowCastDirection( Vector *pDirection, ShadowType_t shadowType ) const;

	CMaterialReference *GetInvulnMaterialRef( void ) { return &m_InvulnerableMaterial; }
	bool IsNemesisOfLocalPlayer();
	bool ShouldShowNemesisIcon();

	virtual	IMaterial *GetHeadLabelMaterial( void );

	virtual void FireGameEvent( IGameEvent *event );

	void UpdateSpyMask( void );
	void UpdateShieldEffect( void );

	void UpdateTypingBubble( void );
	void UpdateOverhealEffect( void );

	virtual const Vector &GetItemTintColor( void ) { return m_vecPlayerColor; }

protected:

	void ResetFlexWeights( CStudioHdr *pStudioHdr );

private:

	void HandleTaunting( void );

	void OnPlayerClassChange( void );
	void UpdatePartyHat( void );

	bool CanLightCigarette( void );

	void InitInvulnerableMaterial( void );

	bool				m_bWasTaunting;
	float				m_flTauntOffTime;
	CameraThirdData_t	m_TauntCameraData;

	QAngle				m_angTauntPredViewAngles;
	QAngle				m_angTauntEngViewAngles;

public:

	Vector				m_vecPlayerColor;

private:

	C_TFPlayerClass		m_PlayerClass;

	// ID Target
	int					m_iIDEntIndex;
	int					m_iForcedIDTarget;

	CNewParticleEffect	*m_pTeleporterEffect;
	bool				m_bToolRecordingVisibility;

	int					m_iOldState;
	int					m_iOldSpawnCounter;

	// Healer
	CHandle<C_TFPlayer>	m_hHealer;
	float				m_flHealerChargeLevel;
	int					m_iOldHealth;

	CNetworkVar( int, m_iPlayerModelIndex );

	// Look At
	/*
	int m_headYawPoseParam;
	int m_headPitchPoseParam;
	float m_headYawMin;
	float m_headYawMax;
	float m_headPitchMin;
	float m_headPitchMax;
	float m_flLastBodyYaw;
	float m_flCurrentHeadYaw;
	float m_flCurrentHeadPitch;
	*/

	// Spy cigarette smoke
	bool m_bCigaretteSmokeActive;

	// Medic callout particle effect
	CNewParticleEffect	*m_pSaveMeEffect;

	bool m_bUpdateObjectHudState;

public:

	CTFPlayerShared m_Shared;

// Called by shared code.
public:

	void DoAnimationEvent( PlayerAnimEvent_t event, int nData = 0 );

	CTFPlayerAnimState *m_PlayerAnimState;

	bool m_bIsPlayerADev;
	bool m_bIsPlayerChatProtected;

	QAngle	m_angEyeAngles;
	CInterpolatedVar< QAngle >	m_iv_angEyeAngles;

	CNetworkHandle( C_TFItem, m_hItem );

	CNetworkHandle( C_TFWeaponBase, m_hOffHandWeapon );

	int				m_iOldPlayerClass;	// Used to detect player class changes
	bool			m_bIsDisplayingNemesisIcon;

	int				m_iSpawnCounter;

	bool			m_bSaveMeParity;
	bool			m_bOldSaveMeParity;

	int				m_nOldWaterLevel;
	float			m_flWaterEntryTime;
	bool			m_bWaterExitEffectActive;

	CMaterialReference	m_InvulnerableMaterial;


	// Burning
	CSoundPatch			*m_pBurningSound;
	CNewParticleEffect	*m_pBurningEffect;
	float				m_flBurnEffectStartTime;
	float				m_flBurnEffectEndTime;

	CNewParticleEffect	*m_pDisguisingEffect;
	float m_flDisguiseEffectStartTime;
	float m_flDisguiseEndEffectStartTime;

	EHANDLE					m_hFirstGib;
	CUtlVector<EHANDLE>		m_hSpawnedGibs;

	int				m_iOldTeam;
	int				m_iOldClass;
	int				m_iOldDisguiseTeam;
	int				m_iOldDisguiseClass;

	bool			m_bDisguised;
	int				m_iPreviousMetal;

	EHANDLE			m_hOldActiveWeapon;

	int GetNumActivePipebombs( void );

	bool			m_bUpdatePartyHat;
	CHandle<C_PlayerAttachedModel>	m_hPartyHat;

	int				m_nForceTauntCam;
	float			m_flLastDamageTime;

	CHandle<C_TFSpyMask> m_hSpyMask;
	CHandle<C_PlayerAttachedModel> m_hPowerupShield;

	bool			m_bTyping;
	CNewParticleEffect	*m_pTypingEffect;

	CNewParticleEffect *m_pOverhealEffect;

	CAttributeManager m_AttributeManager;

private:

	float m_flWaterImpactTime;

	// Gibs.
	CUtlVector<breakmodel_t>	m_aGibs;

	C_TFPlayer( const C_TFPlayer & );
};

inline C_TFPlayer* ToTFPlayer( C_BaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	Assert( dynamic_cast<C_TFPlayer*>( pEntity ) != 0 );
	return static_cast< C_TFPlayer* >( pEntity );
}

#endif // C_TF_PLAYER_H
