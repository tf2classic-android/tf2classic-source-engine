// TF2C 2017
// SanyaSho (2024)

#include "cbase.h"
#include "tf_shareddefs.h"
#include "tf_gamerules.h"
#include "tf_player.h"
#include "func_regenerate.h"
#include "func_restock_zone.h"
#include "eventqueue.h"

LINK_ENTITY_TO_CLASS( func_restock, CRestockZone );

BEGIN_DATADESC( CRestockZone )

	DEFINE_KEYFIELD( m_bRestoreHealth,	FIELD_BOOLEAN,	"RestoreHealth"	),
	DEFINE_KEYFIELD( m_bRestoreAmmo,	FIELD_BOOLEAN,	"RestoreAmmo"	),

END_DATADESC()

CRestockZone::CRestockZone()
{
	m_bRestoreHealth = false;
	m_bRestoreAmmo = false;
}

void CRestockZone::Regenerate( CTFPlayer *pPlayer )
{
	pPlayer->Restock( m_bRestoreHealth, m_bRestoreAmmo );
	pPlayer->SetNextRegenTime( gpGlobals->curtime + TF_REGENERATE_NEXT_USE_TIME );

	CSingleUserRecipientFilter filter( pPlayer );
	EmitSound( filter, pPlayer->entindex(), TF_REGENERATE_SOUND );
	
	if( m_hAssociatedModel )
	{
		variant_t tmpVar;
		tmpVar.SetString( MAKE_STRING( "open" ) );
		m_hAssociatedModel->AcceptInput( "SetAnimation", this, this, tmpVar, 0 );

		tmpVar.SetString( MAKE_STRING( "close" ) );
		g_EventQueue.AddEvent( m_hAssociatedModel, "SetAnimation", tmpVar, TF_REGENERATE_NEXT_USE_TIME - 1.0, this, this );
	}
}
