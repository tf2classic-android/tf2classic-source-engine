//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Team Fortress specific special triggers
//
//===========================================================================//

#include "cbase.h"
#include "player.h"
#include "gamerules.h"
#include "entityapi.h"
#include "entitylist.h"
#include "saverestore_utlvector.h"
#include "tf_player.h"
#include "triggers.h"
#include "tf_triggers.h"
#include "tf_weapon_compound_bow.h"
#include "doors.h"
#include "bot/tf_bot.h"
#include "trigger_area_capture.h"
#include "particle_parse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


//-----------------------------------------------------------------------------
// Purpose: Ignites the arrows of any bow carried by a player who touches this trigger
//-----------------------------------------------------------------------------
class CTriggerIgniteArrows : public CBaseTrigger
{
public:
	DECLARE_CLASS( CTriggerIgniteArrows, CBaseTrigger );

	void Spawn( void );
	void Touch( CBaseEntity *pOther );

	DECLARE_DATADESC();
};

BEGIN_DATADESC( CTriggerIgniteArrows )
END_DATADESC()

LINK_ENTITY_TO_CLASS( trigger_ignite_arrows, CTriggerIgniteArrows );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTriggerIgniteArrows::Spawn( void )
{
	BaseClass::Spawn();
	InitTrigger();
}

//-----------------------------------------------------------------------------
// Purpose: Ignites the arrows of any bow carried by a player who touches this trigger
//-----------------------------------------------------------------------------
void CTriggerIgniteArrows::Touch( CBaseEntity *pOther )
{
	if (!PassesTriggerFilters(pOther))
		return;

	if ( !pOther->IsPlayer() )
		return;

	CTFPlayer *pPlayer = ToTFPlayer( pOther );

	// Ignore non-snipers
	if ( !pPlayer || !pPlayer->IsPlayerClass(TF_CLASS_SNIPER) )
		return;

	// Make sure they're looking at the origin
	Vector vecPos, vecForward, vecUp, vecRight;
	pPlayer->EyePositionAndVectors( &vecPos, &vecForward, &vecUp, &vecRight );
	Vector vTargetDir = GetAbsOrigin() - vecPos;
	VectorNormalize(vTargetDir);
	float fDotPr = DotProduct(vecForward,vTargetDir);
	if (fDotPr < 0.95)
		return;

	// Does he have the bow?
	CTFWeaponBase *pWpn = pPlayer->GetActiveTFWeapon();
	if ( pWpn && pWpn->GetWeaponID() == TF_WEAPON_COMPOUND_BOW )
	{
		CTFCompoundBow *pBow = static_cast<CTFCompoundBow*>( pWpn );
		//pBow->SetArrowAlight( true );
		//pBow->Extinguish();
		pBow->LightArrow();
	}
}


//-----------------------------------------------------------------------------
// Purpose: Trigger that adds tag to bots
//-----------------------------------------------------------------------------
class CTriggerBotTag : public CBaseTrigger
{
public:
	DECLARE_DATADESC();
	DECLARE_CLASS( CTriggerBotTag, CBaseTrigger );

	virtual void Spawn( void );

	virtual void Touch( CBaseEntity *pOther );

private:
	string_t m_iszTags;

	bool m_bAdd;
	CUtlStringList m_tags;
};

BEGIN_DATADESC( CTriggerBotTag )
	DEFINE_KEYFIELD( m_iszTags, FIELD_STRING, "tags" ),
	DEFINE_KEYFIELD( m_bAdd, FIELD_BOOLEAN, "add" ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( trigger_bot_tag, CTriggerBotTag );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTriggerBotTag::Spawn()
{
	BaseClass::Spawn();
	InitTrigger();

	m_tags.RemoveAll();
	// chop space-delimited string into individual tokens
	const char *tags = STRING( m_iszTags );
	if ( tags )
	{
		CSplitString splitStrings( tags, " " );
		for( int i=0; i<splitStrings.Count(); ++i )
		{
			m_tags.CopyAndAddToTail( splitStrings.Element( i ) );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTriggerBotTag::Touch( CBaseEntity *pOther )
{
	if ( m_bDisabled )
	{
		return;
	}

	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CTFBot *pBot = ToTFBot( pOther );
	if ( !pBot )
	{
		return;
	}

	if ( m_bAdd )
	{
		for ( int i=0; i<m_tags.Count(); ++i )
		{
			pBot->AddTag( m_tags[i] );
		}
	}
	else
	{
		for ( int i=0; i<m_tags.Count(); ++i )
		{
			pBot->RemoveTag( m_tags[i] );
		}
	}
}
