//=============================================================================
//
// Purpose: 
//
//=============================================================================
#ifndef TF_PLAYERMODELPANEL_H
#define TF_PLAYERMODELPANEL_H

#ifdef _WIN32
#pragma once
#endif

#include "basemodel_panel.h"
#include "ichoreoeventcallback.h"
#include "choreoscene.h"
#include "tf_shareddefs.h"
//#include "tf_merc_customizations.h"

class CTFPlayerModelPanel : public CBaseModelPanel, public CDefaultClientRenderable, public IChoreoEventCallback, public IHasLocalToGlobalFlexSettings
{
public:
	DECLARE_CLASS_SIMPLE( CTFPlayerModelPanel, CBaseModelPanel );

	CTFPlayerModelPanel( vgui::Panel *pParent, const char *pName );
	~CTFPlayerModelPanel();

	virtual void ApplySettings( KeyValues *inResourceData ) OVERRIDE;
	virtual void PerformLayout( void ) OVERRIDE;
	virtual void RequestFocus( int direction = 0 ) OVERRIDE {}

	void LockStudioHdr();
	void UnlockStudioHdr();
	CStudioHdr *GetModelPtr( void ) { return m_pStudioHdr; }
	int LookupSequence( const char *pszName );
	float GetSequenceFrameRate( int nSequence );

	virtual void PrePaint3D( IMatRenderContext *pRenderContext ) OVERRIDE;
	virtual void PostPaint3D( IMatRenderContext *pRenderContext ) OVERRIDE;
	virtual void SetupFlexWeights( void ) OVERRIDE;
	virtual void FireEvent( const char *pszEventName, const char *pszEventOptions ) OVERRIDE;

	void SetToRandomClass( int iTeam );
	void SetToPlayerClass( int iClass );
	void SetTeam( int iTeam, bool bInGame, bool bForceFFA );
	void LoadItems( ETFLoadoutSlot iSlot = TF_LOADOUT_SLOT_INVALID );
	CEconItemView *GetItemInSlot( ETFLoadoutSlot iSlot );
	void HoldFirstValidItem( void );
	bool HoldItemInSlot( ETFLoadoutSlot iSlot );
	void AddCarriedItem( CEconItemView *pItem );
	void ClearCarriedItems( void );

	void InitPhonemeMappings( void );
	void SetFlexWeight( LocalFlexController_t index, float value );
	float GetFlexWeight( LocalFlexController_t index );
	void ResetFlexWeights( void );
	int FlexControllerLocalToGlobal( const flexsettinghdr_t *pSettinghdr, int key );
	LocalFlexController_t FindFlexController( const char *szName );

	void ProcessVisemes( Emphasized_Phoneme *classes );
	void AddVisemesForSentence( Emphasized_Phoneme *classes, float emphasis_intensity, CSentence *sentence, float t, float dt, bool juststarted );
	void AddViseme( Emphasized_Phoneme *classes, float emphasis_intensity, int phoneme, float scale, bool newexpression );
	bool SetupEmphasisBlend( Emphasized_Phoneme *classes, int phoneme );
	void ComputeBlendedSetting( Emphasized_Phoneme *classes, float emphasis_intensity );
	
	// IHasLocalToGlobalFlexSettings
	virtual void EnsureTranslations( const flexsettinghdr_t *pSettinghdr ) OVERRIDE;

	CChoreoScene *LoadScene( const char *filename );
	void PlayVCD( const char *pszFile );
	void StopVCD();
	void ProcessLoop( CChoreoScene *scene, CChoreoEvent *event );
	void ProcessSequence( CChoreoScene *scene, CChoreoEvent *event );
	void ProcessFlexAnimation( CChoreoScene *scene, CChoreoEvent *event );
	void ProcessFlexSettingSceneEvent( CChoreoScene *scene, CChoreoEvent *event );
	void AddFlexSetting( const char *expr, float scale, const flexsettinghdr_t *pSettinghdr );

	// IChoreoEventCallback
	virtual void StartEvent( float currenttime, CChoreoScene *scene, CChoreoEvent *event ) OVERRIDE;
	virtual void EndEvent( float currenttime, CChoreoScene *scene, CChoreoEvent *event ) OVERRIDE;
	virtual void ProcessEvent( float currenttime, CChoreoScene *scene, CChoreoEvent *event ) OVERRIDE;
	virtual bool CheckEvent( float currenttime, CChoreoScene *scene, CChoreoEvent *event ) OVERRIDE;

	// IClientRenderable
	virtual const Vector&			GetRenderOrigin( void ) OVERRIDE { return vec3_origin; }
	virtual const QAngle&			GetRenderAngles( void ) OVERRIDE { return vec3_angle; }
	virtual void					GetRenderBounds( Vector& mins, Vector& maxs ) OVERRIDE
	{
		GetBoundingBox( mins, maxs );
	}
	virtual const matrix3x4_t &		RenderableToWorldTransform() OVERRIDE
	{
		static matrix3x4_t mat;
		SetIdentityMatrix( mat );
		return mat;
	}
	virtual bool					ShouldDraw( void ) OVERRIDE { return false; }
	virtual bool					IsTransparent( void ) OVERRIDE { return false; }
	virtual bool					ShouldReceiveProjectedTextures( int flags ) OVERRIDE { return false; }

	const Vector &GetModelTintColor( void );
	void SetModelTintColor( const Vector &vecColor );
	void UseCvarsForTintColor( bool bUseCvars ) { m_bUseMercCvars = bUseCvars; }
	void EmitSpawnEffect( const char *pszName );
private:
	CStudioHdr *m_pStudioHdr;
	CThreadFastMutex m_StudioHdrInitLock;

	int m_iTeamNum;
	int m_iClass;
	bool m_bCustomClassData[TF_CLASS_COUNT_ALL];
	BMPResData_t m_ClassResData[TF_CLASS_COUNT_ALL];
	CUtlVector<CEconItemView *> m_Items;

	int m_aMergeMDLMap[TF_PLAYER_WEAPON_COUNT];
	int m_iActiveWpnMDLIndex;
	int m_iTauntMDLIndex;

	Vector m_vecModelTintColor;
	bool m_bUseMercCvars;
	CBaseModelPanel::particle_data_t *m_pSpawnEffectData;

	CChoreoScene *m_pScene;
	float m_flCurrentTime;
	bool m_bFlexEvents;

	CUtlRBTree<FS_LocalToGlobal_t, unsigned short> m_LocalToGlobal;
	Emphasized_Phoneme m_PhonemeClasses[NUM_PHONEME_CLASSES];
	float m_flexWeight[MAXSTUDIOFLEXCTRL];
};

#endif // TF_PLAYERMODELPANEL_H
