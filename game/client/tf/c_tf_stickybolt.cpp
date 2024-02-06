//=============================================================================//
//
// Purpose: TF Crossbow bolt effect
//
//=============================================================================//
#include "cbase.h"
#include "c_basetempentity.h"
#include "fx.h"
#include "decals.h"
#include "iefx.h"
#include "engine/IEngineSound.h"
#include "materialsystem/imaterialvar.h"
#include "IEffects.h"
#include "engine/IEngineTrace.h"
#include "vphysics/constraints.h"
#include "engine/ivmodelinfo.h"
#include "tempent.h"
#include "c_te_legacytempents.h"
#include "engine/ivdebugoverlay.h"
#include "c_te_effect_dispatch.h"
#include "c_tf_player.h"
#include "tf_shareddefs.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define TEMP_OBJECT_LIFETIME 10.0f

IPhysicsObject *GetWorldPhysObject( void );

const char *g_pszArrowModelClient[] =
{
        "models/weapons/w_models/w_arrow.mdl",
        "models/weapons/w_models/w_syringe_proj.mdl",
        "models/weapons/w_models/w_repair_claw.mdl",
        "models/weapons/w_models/w_arrow_xmas.mdl",
        "models/weapons/c_models/c_crusaders_crossbow/c_crusaders_crossbow_xmas_proj.mdl",
        "models/weapons/c_models/c_grapple_proj.mdl",
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CreateTFCrossbowBolt( const Vector &vecOrigin, const Vector &vecDirection, int iProjType, byte nSkin )
{
        Assert( iProjType > 0 && iProjType < TF_NUM_PROJECTILES );

        const char *pszModel; float flModelScale;
        switch ( iProjType )
        {
                case TF_PROJECTILE_ARROW:
                        pszModel = g_pszArrowModelClient[0];
                        flModelScale = 1.f;
                        break;
                case TF_PROJECTILE_HEALING_BOLT:
                        pszModel = g_pszArrowModelClient[1];
                        flModelScale = 1.6f;
                        break;
                case TF_PROJECTILE_BUILDING_REPAIR_BOLT:
                        pszModel = g_pszArrowModelClient[2];
                        flModelScale = 1.f;
                        break;
                case TF_PROJECTILE_FESTIVE_ARROW:
                        pszModel = g_pszArrowModelClient[3];
                        flModelScale = 1.f;
                        break;
                case TF_PROJECTILE_FESTIVE_HEALING_BOLT:
                        pszModel = g_pszArrowModelClient[4];
                        flModelScale = 1.6f;
                        break;
                case TF_PROJECTILE_GRAPPLINGHOOK:
                        pszModel = g_pszArrowModelClient[5];
                        flModelScale = 1.f;
                        break;
                default:
                        Warning( "Unsupported Projectile type in CreateTFCrossbowBolt - %d\n", iProjType );
                        return;
        }
        const model_t *pModel = engine->LoadModel( pszModel );

        QAngle vAngles;
        VectorAngles( vecDirection, vAngles );

        C_LocalTempEntity *pTemp = tempents->SpawnTempModel( pModel, vecOrigin - vecDirection * 5.f, vAngles, vec3_origin, TEMP_OBJECT_LIFETIME, FTENT_NONE );
        if ( pTemp )
        {
                pTemp->SetModelScale( flModelScale );
                pTemp->m_nSkin = nSkin;
        }
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void StickTFRagdollNow( const Vector &vecOrigin, const Vector &vecDirection, ClientEntityHandle_t const &pEntity, int iBone, int iPhysicsBone, int iOwner, int iHitGroup, int iVictim, int iProjType, byte nSkin )
{
        trace_t tr;
        UTIL_TraceLine( vecOrigin - vecDirection * 16.f, vecOrigin + vecDirection * 64.f, MASK_SOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &tr );

        if ( tr.surface.flags & SURF_SKY )
                return;

        C_BaseAnimating *pModel = dynamic_cast<C_BaseAnimating *>( ClientEntityList().GetBaseEntityFromHandle( pEntity ) );
        if ( pModel && pModel->m_pRagdoll )
        {
                IPhysicsObject *pPhysics = NULL;

                ragdoll_t *pRagdoll = pModel->m_pRagdoll->GetRagdoll();
                if ( iPhysicsBone < pRagdoll->listCount )
                {
                        pPhysics = pRagdoll->list[ iPhysicsBone ].pObject;
                }

                if ( GetWorldPhysObject() && pPhysics )
                {
                        Vector vecPhyOrigin; QAngle vecPhyAngle;
                        pPhysics->GetPosition( &vecPhyOrigin, &vecPhyAngle );

                        const float flRand = (float)( rand() / VALVE_RAND_MAX );
                        vecPhyOrigin = vecOrigin - ( vecDirection * flRand * 7.f + vecDirection * 7.f );
                        pPhysics->SetPosition( vecPhyOrigin, vecPhyAngle, true );

                        pPhysics->EnableMotion( false );

                        float flMaxMass = 0;
                        for ( int i = 0; i < pRagdoll->listCount; i++ )
                                flMaxMass = Max( flMaxMass, pRagdoll->list[i].pObject->GetMass() );

                        // normalize masses across all bones to prevent attempts at breaking constraint
                        int nIndex = pRagdoll->list[ iPhysicsBone ].parentIndex;
                        while ( nIndex >= 0 )
                        {
                                if ( pRagdoll->list[ nIndex ].pConstraint )
                                {
                                        const float flMass = Max( pRagdoll->list[ nIndex ].pObject->GetMass(), flMaxMass );
                                        pRagdoll->list[ nIndex ].pObject->SetMass( flMass );
                                }

                                nIndex = pRagdoll->list[ nIndex ].parentIndex;
                        }
                }
        }

        UTIL_ImpactTrace( &tr, DMG_GENERIC );
        CreateTFCrossbowBolt( vecOrigin, vecDirection, iProjType, nSkin );
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : &data -
//-----------------------------------------------------------------------------
void TFStickyBoltCallback( const CEffectData &data )
{
        // This is a mess
        StickTFRagdollNow( data.m_vOrigin,
                                           data.m_vNormal,
                                           data.m_hEntity,
                                           0,
                                           data.m_nMaterial,
                                           data.m_nHitBox,
                                           data.m_nDamageType,
                                           data.m_nSurfaceProp,
                                           data.m_fFlags,
                                           data.m_nColor );
}

DECLARE_CLIENT_EFFECT( "TFBoltImpact", TFStickyBoltCallback );
