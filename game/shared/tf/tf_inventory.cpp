//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Simple Inventory
// by MrModez
// $NoKeywords: $
//=============================================================================//


#include "cbase.h"
#include "econ_item_view.h"
#include "tf_gamerules.h"
#include "tf_shareddefs.h"
#include "tf_inventory.h"
#include "econ_item_system.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define TF_INVENTORY_FILE "tf_inventory.txt"

static CTFInventory g_TFInventory;

CTFInventory *GetTFInventory()
{
	return &g_TFInventory;
}

CTFInventory::CTFInventory() : CAutoGameSystemPerFrame( "CTFInventory" )
{
#ifdef CLIENT_DLL
	m_pInventory = NULL;
#endif

	// Generate dummy base items.
	for ( int iClass = 0; iClass < TF_CLASS_COUNT_ALL; iClass++ )
	{
		for ( int iSlot = 0; iSlot < TF_LOADOUT_SLOT_COUNT; iSlot++ )
		{
			m_Items[iClass][iSlot].AddToTail( NULL );
		}
	}
	
	// FIXME(SanyaSho): this should look like this
	for ( int iSlot = 0; iSlot < TF_LOADOUT_SLOT_COUNT; iSlot++ )
	{
		m_DeathmatchItems[iSlot] = NULL;
		m_CTFItems[iSlot] = NULL;
		//m_InfectedHumanItems[iSlot] = NULL; // SanyaSho: disable this for now
		//m_InfectedZombieItems[iSlot] = NULL; // SanyaSho: disable this for now
		m_InstagibItems[iSlot] = NULL;
	}
};

CTFInventory::~CTFInventory()
{
#if defined( CLIENT_DLL )
	m_pInventory->deleteThis();
#endif
}

const char *g_aDeathmatchLoadout[TF_LOADOUT_SLOT_COUNT] =
{
	NULL,
	"Pistol (Merc)",
	"TF_WEAPON_CROWBAR",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	//NULL,
	//NULL
}; // idb
const char *g_aCTFLoadout[TF_LOADOUT_SLOT_COUNT] =
{
	"Assault Rifle",
	"Sten Gun",
	"TF_WEAPON_CROWBAR",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	//NULL,
	//NULL
}; // idb
/* // SanyaSho: disable this for now
const char *g_aInfectedHumanLoadout[TF_LOADOUT_SLOT_COUNT] =
{
	"Hunting Shotgun",
	"Pistol (Merc)",
	"TF_WEAPON_CROWBAR",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	//NULL,
	//NULL
}; // idb
const char *g_aInfectedZombieLoadout[TF_LOADOUT_SLOT_COUNT] =
{
	NULL,
	NULL,
	"Claws",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	//NULL,
	//NULL
}; // idb
*/
const char *g_aInstagibLoadout[TF_LOADOUT_SLOT_COUNT] =
{
	"Lever Rifle",
	NULL,
	"TF_WEAPON_CROWBAR",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	//NULL,
	//NULL
}; // idb

//-----------------------------------------------------------------------------
// Purpose: Fill the item arrays with data from item schema.
//-----------------------------------------------------------------------------
bool CTFInventory::Init( void )
{
	GetItemSchema()->Init();

	// Generate item list.
	FOR_EACH_MAP( GetItemSchema()->m_Items, i )
	{
		int iItemID = GetItemSchema()->m_Items.Key( i );
		CEconItemDefinition *pItemDef = GetItemSchema()->m_Items.Element( i );

		if ( pItemDef->item_slot == TF_LOADOUT_SLOT_INVALID )
			continue;

		// Add it to each class that uses it.
		for ( int iClass = 0; iClass < TF_CLASS_COUNT_ALL; iClass++ )
		{
			if ( pItemDef->used_by_classes & ( 1 << iClass ) )
			{
				// Show it if it's either base item or has show_in_armory flag.
				int iSlot = pItemDef->GetLoadoutSlot( iClass );

				if ( pItemDef->baseitem )
				{
					CEconItemView *pBaseItem = m_Items[iClass][iSlot][0];
					if ( pBaseItem != NULL )
					{
						Warning( "Duplicate base item %d for class %s in slot %s!\n", iItemID, g_aPlayerClassNames_NonLocalized[iClass], g_LoadoutSlots[iSlot] );
						delete pBaseItem;
					}

					m_Items[iClass][iSlot][0] = new CEconItemView( iItemID );
				}
				else if ( pItemDef->show_in_armory )
				{
					CEconItemView *pNewItem = new CEconItemView( iItemID );
					m_Items[iClass][iSlot].AddToTail( pNewItem );
				}
			}
			
			// FIXME(SanyaSho): should be called if something wrong with m_Items(probably)
			for( int i = 0; i < TF_LOADOUT_SLOT_COUNT; i++ )
			{
				m_DeathmatchItems[i] = new CEconItemView( g_aDeathmatchLoadout[i] );
				m_CTFItems[i] = new CEconItemView(  g_aCTFLoadout[i] );
				//m_InfectedHumanItems[i] = new CEconItemView(  g_aInfectedHumanLoadout[i] ); // SanyaSho: disable this for now
				//m_InfectedZombieItems[i] = new CEconItemView( g_aInfectedZombieLoadout[i] ); // SanyaSho: disable this for now
				m_InstagibItems[i] = new CEconItemView( g_aInstagibLoadout[i] );
			}
		}
	}


#if defined( CLIENT_DLL )
	LoadInventory();
#endif

	return true;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFInventory::LevelInitPreEntity( void )
{
	GetItemSchema()->Precache();
}

//-----------------------------------------------------------------------------
// Purpose: LEGACY
//-----------------------------------------------------------------------------
int CTFInventory::GetWeapon( int iClass, ETFLoadoutSlot iSlot )
{
	return Weapons[iClass][iSlot];
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CEconItemView *CTFInventory::GetMercenaryItem( CBasePlayer *pPlayer, ETFLoadoutSlot iSlot )
{
	CEconItemView *m_pItem = NULL;

	// ADDITION(SanyaSho): apply CTF item list if we're playing CTF in non-dm.
	if( !TFGameRules()->IsDeathmatch() )
	{
		m_pItem = m_CTFItems[iSlot];
	}
	else
	{
#if SOONSOON
		switch( TFGameRules()->GetRetroModeType() )
		{
			case TF_GAMESUBTYPE_CTF:
			case TF_GAMESUBTYPE_AD:
			case TF_GAMESUBTYPE_INVADE:
				m_pItem = m_CTFItems[iSlot];
				break;
			case TF_GAMESUBTYPE_INFECTION:
				if( pPlayer->GetTeamNumber() == TF_TEAM_BLUE ) // zimbabwe ABOBA
				{
					m_pItem = m_InfectedZombieItems[iSlot];
				}
				else
				{
					m_pItem = m_InfectedHumanItems[iSlot];
				}
				break;
			default:
				m_pItem = m_DeathmatchItems[iSlot];
				
				if( TFGameRules()->IsInstagib() )
				{
					m_pItem = m_InstagibItems[iSlot];
				}
				break;
		}
#else
		switch( TFGameRules()->GetGameType() )
		{
			case TF_GAMETYPE_CTF:
			{
				m_pItem = m_CTFItems[iSlot];
				break;
			}
			
			default:
			{
				m_pItem = m_DeathmatchItems[iSlot];
				
				if( TFGameRules()->IsInstagib() )
				{
					m_pItem = m_InstagibItems[iSlot];
				}
				break;
			}
		}
#endif
	}
	
	// missing item
	if( !m_pItem || (m_pItem->GetItemDefIndex() < 0) )
		return NULL;

	return m_pItem;
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CEconItemView *CTFInventory::GetItem( int iClass, ETFLoadoutSlot iSlot, int iNum )
{
	if ( CheckValidWeapon( iClass, iSlot, iNum ) == false )
		return NULL;

	//if( iClass > TF_CLASS_COUNT )
	//	return NULL;

	return m_Items[iClass][iSlot][iNum];
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CTFInventory::CheckValidSlot( int iClass, ETFLoadoutSlot iSlot )
{
	if ( iClass < TF_CLASS_UNDEFINED || iClass > TF_CLASS_COUNT )
		return false;

	int iCount = ( TF_LOADOUT_SLOT_TAUNT );

	// Array bounds check.
	if ( iSlot >= iCount || iSlot < 0 )
		return false;

	// Slot must contain a base item.
	if ( m_Items[iClass][iSlot][0] == NULL )
		return false;

	return true;
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CTFInventory::CheckValidWeapon( int iClass, ETFLoadoutSlot iSlot, int iWeapon )
{
	if ( iClass < TF_CLASS_UNDEFINED || iClass > TF_CLASS_COUNT )
		return false;

	int iCount = ( m_Items[iClass][iSlot].Count() );

	// Array bounds check.
	if ( iWeapon >= iCount || iWeapon < 0 )
		return false;

	// Don't allow switching if this class has no weapon at this position.
	if ( m_Items[iClass][iSlot][iWeapon] == NULL )
		return false;

	return true;
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int CTFInventory::NumWeapons( int iClass, ETFLoadoutSlot iSlot )
{
	// Slot must contain a base item.
	if ( m_Items[iClass][iSlot][0] == NULL )
		return 0;

	return m_Items[iClass][iSlot].Count();
}

#if defined( CLIENT_DLL )
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFInventory::LoadInventory()
{
	bool bExist = filesystem->FileExists( TF_INVENTORY_FILE, "MOD" );
	if ( bExist )
	{
		if ( !m_pInventory )
		{
			m_pInventory = new KeyValues( "Inventory" );
		}
		m_pInventory->LoadFromFile( filesystem, TF_INVENTORY_FILE );
	}
	else
	{
		ResetInventory();
	}
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFInventory::SaveInventory()
{
	m_pInventory->SaveToFile( filesystem, TF_INVENTORY_FILE );
};

//-----------------------------------------------------------------------------
// Purpose: Create a default inventory file.
//-----------------------------------------------------------------------------
void CTFInventory::ResetInventory()
{
	if ( m_pInventory )
	{
		m_pInventory->deleteThis();
	}

	m_pInventory = new KeyValues( "Inventory" );

	for ( int i = TF_CLASS_UNDEFINED; i < TF_CLASS_COUNT_ALL; i++ )
	{
		KeyValues *pClassInv = new KeyValues( g_aPlayerClassNames_NonLocalized[i] );
		for ( int j = 0; j < TF_LOADOUT_SLOT_COUNT; j++ )
		{
			pClassInv->SetInt( g_LoadoutSlots[j], 0 );
		}
		m_pInventory->AddSubKey( pClassInv );
	}

	SaveInventory();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int CTFInventory::GetWeaponPreset( int iClass, ETFLoadoutSlot iSlot )
{
	KeyValues *pClass = m_pInventory->FindKey( g_aPlayerClassNames_NonLocalized[iClass] );
	if ( !pClass )	//cannot find class node
	{
		ResetInventory();
		return 0;
	}
	int iPreset = pClass->GetInt( g_LoadoutSlots[iSlot], TF_LOADOUT_SLOT_INVALID );
	if ( iPreset == TF_LOADOUT_SLOT_INVALID )	//cannot find slot node
	{
		ResetInventory();
		return 0;
	}

	if ( CheckValidWeapon( iClass, iSlot, iPreset ) == false )
		return 0;

	return iPreset;
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFInventory::SetWeaponPreset( int iClass, ETFLoadoutSlot iSlot, int iPreset )
{
	KeyValues* pClass = m_pInventory->FindKey( g_aPlayerClassNames_NonLocalized[iClass] );
	if ( !pClass )	//cannot find class node
	{
		ResetInventory();
		pClass = m_pInventory->FindKey( g_aPlayerClassNames_NonLocalized[iClass] );
	}
	pClass->SetInt( GetSlotName( iSlot ), iPreset );
	SaveInventory();
}

const char* CTFInventory::GetSlotName( ETFLoadoutSlot iSlot )
{
	return g_LoadoutSlots[iSlot];
};

#endif

//-----------------------------------------------------------------------------
// Purpose: Legacy array, used when we're forced to use old method of giving out weapons.
//-----------------------------------------------------------------------------
const int CTFInventory::Weapons[TF_CLASS_COUNT_ALL][TF_PLAYER_WEAPON_COUNT] =
{
	{

	},
	{
		TF_WEAPON_SCATTERGUN,
		TF_WEAPON_PISTOL_SCOUT,
		TF_WEAPON_BAT
	},
	{
		TF_WEAPON_SNIPERRIFLE,
		TF_WEAPON_SMG,
		TF_WEAPON_CLUB
	},
	{
		TF_WEAPON_ROCKETLAUNCHER,
		TF_WEAPON_SHOTGUN_SOLDIER,
		TF_WEAPON_SHOVEL
	},
	{
		TF_WEAPON_GRENADELAUNCHER,
		TF_WEAPON_PIPEBOMBLAUNCHER,
		TF_WEAPON_BOTTLE
	},
	{
		TF_WEAPON_SYRINGEGUN_MEDIC,
		TF_WEAPON_MEDIGUN,
		TF_WEAPON_BONESAW
	},
	{
		TF_WEAPON_MINIGUN,
		TF_WEAPON_SHOTGUN_HWG,
		TF_WEAPON_FISTS
	},
	{
		TF_WEAPON_FLAMETHROWER,
		TF_WEAPON_SHOTGUN_PYRO,
		TF_WEAPON_FIREAXE
	},
	{
		TF_WEAPON_REVOLVER,
		TF_WEAPON_NONE,
		TF_WEAPON_KNIFE,
		TF_WEAPON_PDA_SPY,
		TF_WEAPON_INVIS
	},
	{
		TF_WEAPON_SHOTGUN_PRIMARY,
		TF_WEAPON_PISTOL,
		TF_WEAPON_WRENCH,
		TF_WEAPON_PDA_ENGINEER_BUILD,
		TF_WEAPON_PDA_ENGINEER_DESTROY
	},
	{
		TF_WEAPON_NONE,
		TF_WEAPON_NONE,
		TF_WEAPON_UMBRELLA
	},
	{
		TF_WEAPON_NONE,
		TF_WEAPON_PISTOL,
		TF_WEAPON_CROWBAR
	}
};
