//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: Game-specific explosion effects
//
//=============================================================================//
#include "cbase.h"
#include "c_te_effect_dispatch.h"
#include "cdll_util.h"
#include "dt_recv.h"
#include "effect_dispatch_data.h"
#include "iclientnetworkable.h"
#include "tempent.h"
#include "c_te_legacytempents.h"
#include "tf_shareddefs.h"
#include "c_basetempentity.h"
#include "tier0/vprof.h"
#include "util_shared.h"
#include "c_tf_player.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_TETFParticleEffect : public C_BaseTempEntity
{
public:
	DECLARE_CLASS( C_TETFParticleEffect, C_BaseTempEntity );
	DECLARE_CLIENTCLASS();

	C_TETFParticleEffect( void );

	virtual void	PostDataUpdate( DataUpdateType_t updateType );

public:
	Vector m_vecOrigin;
	Vector m_vecStart;
	QAngle m_vecAngles;

	int m_iParticleSystemIndex;

	ClientEntityHandle_t m_hEntity;

	int m_iAttachType;
	int m_iAttachmentPointIndex;

	bool m_bResetParticles;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TETFParticleEffect::C_TETFParticleEffect( void )
{
	m_vecOrigin.Init();
	m_vecStart.Init();
	m_vecAngles.Init();

	m_iParticleSystemIndex = -1;

	m_hEntity = INVALID_EHANDLE_INDEX;

	m_iAttachType = PATTACH_ABSORIGIN;
	m_iAttachmentPointIndex = 0;

	m_bResetParticles = false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_TETFParticleEffect::PostDataUpdate( DataUpdateType_t updateType )
{
	VPROF( "C_TETFParticleEffect::PostDataUpdate" );

	CEffectData	data;

	data.m_nHitBox = m_iParticleSystemIndex;

	data.m_vOrigin = m_vecOrigin;
	data.m_vStart = m_vecStart;
	data.m_vAngles = m_vecAngles;

	if ( m_hEntity != INVALID_EHANDLE_INDEX )
	{
		data.m_hEntity = m_hEntity;
		data.m_fFlags |= PARTICLE_DISPATCH_FROM_ENTITY;
	}
	else
	{
		data.m_hEntity = NULL;
	}

	data.m_nDamageType = m_iAttachType;
	data.m_nAttachmentIndex = m_iAttachmentPointIndex;

	if ( m_bResetParticles )
	{
		data.m_fFlags |= PARTICLE_DISPATCH_RESET_PARTICLES;
	}

	DispatchEffect( "ParticleEffect", data );
}

static void RecvProxy_ParticleSystemEntIndex( const CRecvProxyData *pData, void *pStruct, void *pOut )
{
	int nEntIndex = pData->m_Value.m_Int;
	((C_TETFParticleEffect*)pStruct)->m_hEntity = (nEntIndex < 0) ? INVALID_EHANDLE_INDEX : ClientEntityList().EntIndexToHandle( nEntIndex );
}

IMPLEMENT_CLIENTCLASS_EVENT_DT( C_TETFParticleEffect, DT_TETFParticleEffect, CTETFParticleEffect )
	RecvPropFloat( RECVINFO( m_vecOrigin[0] ) ),
	RecvPropFloat( RECVINFO( m_vecOrigin[1] ) ),
	RecvPropFloat( RECVINFO( m_vecOrigin[2] ) ),
	RecvPropFloat( RECVINFO( m_vecStart[0] ) ),
	RecvPropFloat( RECVINFO( m_vecStart[1] ) ),
	RecvPropFloat( RECVINFO( m_vecStart[2] ) ),
	RecvPropQAngles( RECVINFO( m_vecAngles ) ),
	RecvPropInt( RECVINFO( m_iParticleSystemIndex ) ),
	RecvPropInt( "entindex", 0, SIZEOF_IGNORE, 0, RecvProxy_ParticleSystemEntIndex ),
	RecvPropInt( RECVINFO( m_iAttachType ) ),
	RecvPropInt( RECVINFO( m_iAttachmentPointIndex ) ),
	RecvPropInt( RECVINFO( m_bResetParticles ) ),
END_RECV_TABLE()


// DM SPAWN PARTICLE
ConVar tf2c_dm_show_spawneffect_in_first_person( "tf2c_dm_show_spawneffect_in_first_person", "0", FCVAR_ARCHIVE | FCVAR_USERINFO ); // SanyaSho: small addition to 2017 code

class C_TETFSpawnEffect : public C_BaseTempEntity
{
public:
	DECLARE_CLASS( C_TETFSpawnEffect, C_BaseTempEntity );
	DECLARE_CLIENTCLASS();

	C_TETFSpawnEffect();

	virtual void PostDataUpdate( DataUpdateType_t updateType );
public:
	Vector m_vecOrigin;
	QAngle m_vecAngles;
	int m_iPlayerIndex;
	int m_iParticleSystemIndex;
	Vector m_vecColor;
};

IMPLEMENT_CLIENTCLASS_EVENT_DT( C_TETFSpawnEffect, DT_TETFSpawnEffect , CTETFSpawnEffect  )
	RecvPropVector( RECVINFO( m_vecOrigin ) ),
	RecvPropQAngles( RECVINFO( m_vecAngles ) ),
	RecvPropInt( RECVINFO( m_iPlayerIndex ) ),
	RecvPropInt( RECVINFO( m_iParticleSystemIndex ) ),
	RecvPropVector( RECVINFO( m_vecColor ) ),
END_RECV_TABLE()

C_TETFSpawnEffect::C_TETFSpawnEffect()
{

}

void C_TETFSpawnEffect::PostDataUpdate( DataUpdateType_t updateType )
{
	if( !IsPlayerIndex( m_iPlayerIndex ) )
	{
		return;
	}
	
	C_TFPlayer *pPlayer = ToTFPlayer( UTIL_PlayerByIndex( m_iPlayerIndex ) );
	if( !pPlayer )
	{
		return;
	}
	
	if( pPlayer->InFirstPersonView() && !tf2c_dm_show_spawneffect_in_first_person.GetBool() )
	{
		return;
	}
	
	CEffectData data;
	data.m_vOrigin = m_vecOrigin;
	data.m_vAngles = m_vecAngles;
	data.m_hEntity = pPlayer;
	data.m_nHitBox = m_iParticleSystemIndex;
	data.m_bCustomColors = true;
	data.m_CustomColors.m_vecColor1 = m_vecColor;
	
	DispatchEffect( "ParticleEffect", data );
}
