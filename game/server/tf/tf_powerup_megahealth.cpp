//====== Copyright © 1996-2013, Valve Corporation, All rights reserved. =======//
//
// Purpose: FILL IN
//
//=============================================================================//

#include "cbase.h"
#include "tf_powerup_megahealth.h"
#include "tf_player.h"
#include "tf_gamerules.h"
#include "tf_team.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//=============================================================================

BEGIN_DATADESC( CTFPowerupMegaHealth )
END_DATADESC()

LINK_ENTITY_TO_CLASS( item_powerup_megahealth, CTFPowerupMegaHealth );

//-----------------------------------------------------------------------------
// Purpose: Constructor 
//-----------------------------------------------------------------------------
CTFPowerupMegaHealth::CTFPowerupMegaHealth()
{
	m_flEffectDuration = 15.0f;
}

//-----------------------------------------------------------------------------
// Purpose: Touch function
//-----------------------------------------------------------------------------
bool CTFPowerupMegaHealth::MyTouch( CBasePlayer *pPlayer )
{
	CTFPlayer *pTFPlayer = ToTFPlayer( pPlayer );

	if ( pTFPlayer && ValidTouch( pPlayer ) )
	{
		// Give 200% health
		pTFPlayer->TakeHealth( pTFPlayer->GetMaxHealth() * 2, HEAL_NOTIFY | HEAL_IGNORE_MAXHEALTH | HEAL_MAXBUFFCAP );
		pTFPlayer->m_Shared.HealNegativeConds();
		pTFPlayer->EmitSound( GetPickupSound() );

		if ( !TFGameRules()->IsDeathmatch() && TFGameRules()->IsTeamplay() )
		{
			for ( int i = FIRST_GAME_TEAM; i < GetNumberOfTeams(); i++ )
			{
				if ( i != pPlayer->GetTeamNumber() )
				{
					CTeamRecipientFilter filter( i, true );
					g_TFAnnouncer.Speak( filter, GetEnemyPickupAnnouncement() );
				}
				else
				{
					CTeamRecipientFilter filter( i, true );
					filter.RemoveRecipient( pPlayer );
					g_TFAnnouncer.Speak( filter, GetTeamPickupAnnouncement() );
				}
			}
		}
		else
		{
			CTeamRecipientFilter filter( FIRST_GAME_TEAM, true );
			filter.RemoveRecipient( pPlayer );
			g_TFAnnouncer.Speak( filter, GetEnemyPickupAnnouncement() );
		}

		return true;
	}

	return false;
}
