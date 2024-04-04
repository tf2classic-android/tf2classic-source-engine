//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Simple Inventory
// by MrModez
// $NoKeywords: $
//=============================================================================//
#ifndef TF_INVENTORY_H
#define TF_INVENTORY_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
//#include "server_class.h"
#include "igamesystem.h"
#include "tf_shareddefs.h"
#if defined( CLIENT_DLL )
#include "c_tf_player.h"
#include "filesystem.h" 
#endif

#define INVENTORY_WEAPONS		5
#define INVENTORY_WEAPONS_COUNT	500
#define INVENTORY_COLNUM		5
#define INVENTORY_ROWNUM		3
#define INVENTORY_VECTOR_NUM	INVENTORY_COLNUM * INVENTORY_ROWNUM

class CTFInventory : public CAutoGameSystemPerFrame
{
public:
	CTFInventory();
	~CTFInventory();

	virtual char const *Name() { return "CTFInventory"; }

	virtual bool	Init( void );
	virtual void	LevelInitPreEntity( void );

	int				GetWeapon( int iClass, ETFLoadoutSlot iSlot );
	CEconItemView *GetMercenaryItem( CBasePlayer *pPlayer, ETFLoadoutSlot iSlot );
	CEconItemView	*GetItem( int iClass, ETFLoadoutSlot iSlot, int iNum );
	bool			CheckValidSlot( int iClass, ETFLoadoutSlot iSlot );
	bool			CheckValidWeapon( int iClass, ETFLoadoutSlot iSlot, int iWeapon );
	int				NumWeapons( int iClass, ETFLoadoutSlot iSlot );

#if defined( CLIENT_DLL )
	int				GetWeaponPreset( int iClass, ETFLoadoutSlot iSlot );
	void			SetWeaponPreset( int iClass, ETFLoadoutSlot iSlot, int iPreset );
	const char		*GetSlotName( ETFLoadoutSlot iSlot );
#endif

private:
	static const int			Weapons[TF_CLASS_COUNT_ALL][TF_PLAYER_WEAPON_COUNT];
	CUtlVector<CEconItemView *>	m_Items[TF_CLASS_COUNT_ALL][TF_LOADOUT_SLOT_COUNT]; // should be CUtlVectorAutoPurge
	
	CEconItemView *m_DeathmatchItems[TF_LOADOUT_SLOT_COUNT];
	CEconItemView *m_CTFItems[TF_LOADOUT_SLOT_COUNT];
	//CEconItemView *m_InfectedHumanItems[TF_LOADOUT_SLOT_COUNT]; // SanyaSho: disable this for now
	//CEconItemView *m_InfectedZombieItems[TF_LOADOUT_SLOT_COUNT]; // SanyaSho: disable this for now
	CEconItemView *m_InstagibItems[TF_LOADOUT_SLOT_COUNT];
	
#if defined( CLIENT_DLL )
	void LoadInventory();
	void ResetInventory();
	void SaveInventory();
	KeyValues* m_pInventory;
#endif
};

CTFInventory *GetTFInventory();

#endif // TF_INVENTORY_H
