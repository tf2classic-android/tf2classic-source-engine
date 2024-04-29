#include "cbase.h"
#include "econ_item_schema.h"
#include "econ_item_system.h"
#include "activitylist.h"
#include "filesystem.h"
#include "util_shared.h"
#if defined( CLIENT_DLL )
#include "hud.h"
#endif
#include "tf_shareddefs.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void InitPerClassStringVectorArray( KeyValues *pKeys, CUtlVector<char const *> *pVector )
{
	if( !pKeys )
		return;
	
	for( KeyValues *pData = pKeys->GetFirstSubKey(); pData != NULL; pData = pData->GetNextKey() )
	{
		int iClass = UTIL_StringFieldToInt( pData->GetName(), g_aPlayerClassNames_NonLocalized, TF_CLASS_COUNT_ALL );
		if( iClass != -1 )
		{
			const char *pszString = pData->GetString();
			if( pszString && pszString[0] )
			{
				pVector[iClass].AddToTail( pszString );
			}
			
			// check multi line case
			FOR_EACH_SUBKEY( pData, pValueKey )
			{
				const char *pszValue = pValueKey->GetString();
				if ( pszValue && *pszValue )
				{
					pVector[iClass].AddToTail( pszValue );
				}
			}
		}
	}
}

void InitPerClassStringArray( KeyValues *pKeys, const char **pArray )
{
	if( !pKeys )
		return;
		
	for( KeyValues *pData = pKeys->GetFirstSubKey(); pData != NULL; pData = pData->GetNextKey() )
	{
		int iClass = UTIL_StringFieldToInt( pData->GetName(), g_aPlayerClassNames_NonLocalized, TF_CLASS_COUNT_ALL );
		if( iClass != -1 )
		{
			const char *pszString = pData->GetString();
			if( pszString && pszString[0] )
			{
				pArray[iClass] = pszString;
			}
		}
	}
}

#define TF_TAUNTATK_COUNT 32
const char *taunt_attack_name[TF_TAUNTATK_COUNT] =
{
	"TAUNTATK_NONE",
	"TAUNTATK_PYRO_HADOUKEN",
	"TAUNTATK_HEAVY_EAT",
	"TAUNTATK_HEAVY_RADIAL_BUFF",
	"TAUNTATK_HEAVY_HIGH_NOON",
	"TAUNTATK_SCOUT_DRINK",
	"TAUNTATK_SCOUT_GRAND_SLAM",
	"TAUNTATK_MEDIC_INHALE",
	"TAUNTATK_SPY_FENCING_SLASH_A",
	"TAUNTATK_SPY_FENCING_SLASH_B",
	"TAUNTATK_SPY_FENCING_STAB",
	"TAUNTATK_RPS_KILL",
	"TAUNTATK_SNIPER_ARROW_STAB_IMPALE",
	"TAUNTATK_SNIPER_ARROW_STAB_KILL",
	"TAUNTATK_SOLDIER_GRENADE_KILL",
	"TAUNTATK_DEMOMAN_BARBARIAN_SWING",
	"TAUNTATK_MEDIC_UBERSLICE_IMPALE",
	"TAUNTATK_MEDIC_UBERSLICE_KILL",
	"TAUNTATK_FLIP_LAND_PARTICLE",
	"TAUNTATK_RPS_PARTICLE",
	"TAUNTATK_HIGHFIVE_PARTICLE",
	"TAUNTATK_ENGINEER_GUITAR_SMASH",
	"TAUNTATK_ENGINEER_ARM_IMPALE",
	"TAUNTATK_ENGINEER_ARM_KILL",
	"TAUNTATK_ENGINEER_ARM_BLEND",
	"TAUNTATK_SOLDIER_GRENADE_KILL_WORMSIGN",
	"TAUNTATK_SHOW_ITEM",
	"TAUNTATK_MEDIC_RELEASE_DOVES",
	"TAUNTATK_PYRO_ARMAGEDDON",
	"TAUNTATK_PYRO_SCORCHSHOT",
	"TAUNTATK_ALLCLASS_GUITAR_RIFF",
	"TAUNTATK_MEDIC_HEROIC_TAUNT"
}; // idb

const char *g_TeamVisualSections[TF_TEAM_COUNT] =
{
	"visuals",			// TEAM_UNASSIGNED
	"",					// TEAM_SPECTATOR
	"visuals_red",		// TEAM_RED
	"visuals_blu",		// TEAM_BLUE
	"visuals_grn",		// TEAM_GREEN
	"visuals_ylw",		// TEAM_YELLOW
	//"visuals_mvm_boss"	// ???
};

const char *g_AttributeDescriptionFormats[] =
{
	"value_is_percentage",
	"value_is_inverted_percentage",
	"value_is_additive",
	"value_is_additive_percentage",
	"value_is_or",
	"value_is_date",
	"value_is_account_id",
	"value_is_particle_index",
	"value_is_killstreakeffect_index",
	"value_is_killstreak_idleeffect_index",
	"value_is_item_def",
	"value_is_from_lookup_table"
};

const char *g_EffectTypes[] =
{
	"unusual",
	"strange",
	"neutral",
	"positive",
	"negative"
};

const char *g_szQualityStrings[] =
{
	"normal",
	"rarity1",
	"rarity2",
	"vintage",
	"rarity3",
	"rarity4",
	"unique",
	"community",
	"developer",
	"selfmade",
	"customized",
	"strange",
	"completed",
	"haunted",
	"collectors",
	"paintkitWeapon",
};

const char *g_szQualityColorStrings[] =
{
	"QualityColorNormal",
	"QualityColorrarity1",
	"QualityColorrarity2",
	"QualityColorVintage",
	"QualityColorrarity3",
	"QualityColorrarity4",
	"QualityColorUnique",
	"QualityColorCommunity",
	"QualityColorDeveloper",
	"QualityColorSelfMade",
	"QualityColorSelfMadeCustomized",
	"QualityColorStrange",
	"QualityColorCompleted",
	"QualityColorHaunted",
	"QualityColorCollectors",
	"QualityColorPaintkitWeapon",
};

const char *g_szQualityLocalizationStrings[] =
{
	"#Normal",
	"#rarity1",
	"#rarity2",
	"#vintage",
	"#rarity3",
	"#rarity4",
	"#unique",
	"#community",
	"#developer",
	"#selfmade",
	"#customized",
	"#strange",
	"#completed",
	"#haunted",
	"#collectors",
	"#paintkitWeapon",
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
static CEconItemSchema g_EconItemSchema;
CEconItemSchema *GetItemSchema()
{
	return &g_EconItemSchema;
}

//-----------------------------------------------------------------------------
// Purpose: constructor
//-----------------------------------------------------------------------------
CEconItemSchema::CEconItemSchema()
{
	SetDefLessFunc( m_Items );
	SetDefLessFunc( m_Attributes );

}

CEconItemSchema::~CEconItemSchema()
{
}

//-----------------------------------------------------------------------------
// Purpose: Initializer
//-----------------------------------------------------------------------------
bool CEconItemSchema::Init( void )
{
	// Must register activities early so we can parse animation replacements.
	ActivityList_Free();
	ActivityList_RegisterSharedActivities();
	
	float flStartTime = engine->Time();
	ParseItemSchema( "scripts/items/items_game.txt" );
	float flEndTime = engine->Time();
	Msg( "[%s] Processing item schema took %.02fms. Parsed %d items and %d attributes.\n",
	     CBaseEntity::IsServer() ? "SERVER" : "CLIENT",
	     ( flEndTime - flStartTime ) * 1000.0f,
	     m_Items.Count(),
	     m_Attributes.Count() );
	

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Runs on level start, precaches models and sounds from schema.
//-----------------------------------------------------------------------------
void CEconItemSchema::Precache( void )
{
	// rewriteme

	string_t strPrecacheAttribute = AllocPooledString( "custom_projectile_model" );

	// Precache everything from schema.
	FOR_EACH_MAP( m_Items, i )
	{
		CEconItemDefinition *pItem = m_Items[i];

		// Precache models.
		if ( pItem->model_world[0] != '\0' )
			CBaseEntity::PrecacheModel( pItem->model_world );

		if ( pItem->model_player[0] != '\0' )
			CBaseEntity::PrecacheModel( pItem->model_player );

		for ( int iClass = 0; iClass < TF_CLASS_COUNT_ALL; iClass++ )
		{
			const char *pszModel = pItem->model_player_per_class[iClass];
			if ( pszModel[0] != '\0' )
				CBaseEntity::PrecacheModel( pszModel );
		}

		// Precache visuals.
		for ( int i = 0; i < TF_TEAM_COUNT; i++ )
		{
			if ( i == TEAM_SPECTATOR )
				continue;

			EconItemVisuals *pVisuals = &pItem->visual[i];

			// Precache sounds.
			for ( int i = 0; i < NUM_SHOOT_SOUND_TYPES; i++ )
			{
				if ( pVisuals->aWeaponSounds[i][0] != '\0' )
					CBaseEntity::PrecacheScriptSound( pVisuals->aWeaponSounds[i] );
			}
		}

		// Cache all attrbute names.
		for ( int i = 0; i < pItem->attributes.Count(); i++ )
		{
			CEconItemAttribute *pAttribute = &pItem->attributes[i];
			pAttribute->m_strAttributeClass = AllocPooledString( pAttribute->attribute_class );

			// Special case for custom_projectile_model attribute.
			if ( pAttribute->m_strAttributeClass == strPrecacheAttribute )
			{
				CBaseEntity::PrecacheModel( pAttribute->value_string.Get() );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CEconItemDefinition* CEconItemSchema::GetItemDefinition( int id )
{
	if ( id < 0 )
		return NULL;

	CEconItemDefinition *itemdef = NULL;
	FIND_ELEMENT( m_Items, id, itemdef );
	return itemdef;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CEconItemSchema::GetItemIndex( const char *pszName )
{
	if( !pszName || !pszName[0] )
		return -1;
	
	FOR_EACH_MAP_FAST( m_Items, i )
	{
		CEconItemDefinition *pItem = m_Items.Element( i );
		if( !pItem )
			continue;
		
		if( !Q_stricmp( pItem->name, pszName ) )
		{
			return m_Items.Key( i );
		}
	}
	
	return -1;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
EconAttributeDefinition *CEconItemSchema::GetAttributeDefinition( int id )
{
	if ( id < 0 )
		return NULL;

	EconAttributeDefinition *attribdef = NULL;
	FIND_ELEMENT( m_Attributes, id, attribdef );
	return attribdef;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
EconAttributeDefinition *CEconItemSchema::GetAttributeDefinitionByName( const char *name )
{
	//unsigned int index = m_Attributes.Find(name);
	//if (index < m_Attributes.Count())
	//{
	//	return &m_Attributes[index];
	//}
	FOR_EACH_MAP_FAST( m_Attributes, i )
	{
		if ( !V_stricmp( m_Attributes[i]->name, name ) )
		{
			return m_Attributes[i];
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
EconAttributeDefinition *CEconItemSchema::GetAttributeDefinitionByClass( const char *classname )
{
	//unsigned int index = m_Attributes.Find(name);
	//if (index < m_Attributes.Count())
	//{
	//	return &m_Attributes[index];
	//}
	FOR_EACH_MAP_FAST( m_Attributes, i )
	{
		if ( !V_stricmp( m_Attributes[i]->attribute_class, classname ) )
		{
			return m_Attributes[i];
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CEconItemSchema::GetAttributeIndex( const char *name )
{
	if ( !name )
		return -1;

	FOR_EACH_MAP_FAST( m_Attributes, i )
	{
		if ( !V_stricmp( m_Attributes[i]->name, name ) )
		{
			return m_Attributes.Key( i );
		}
	}

	return -1;
}

bool CEconItemSchema::ParseItemSchema( const char *pszFile )
{
	m_pSchemaData = new KeyValues( "ItemData" );
	if( !m_pSchemaData )
	{
		Error( "Failed to allocate memory for %s\n", pszFile );
		return false;
	}
	
	if( !m_pSchemaData->LoadFromFile( filesystem, pszFile, "MOD" ) )
	{
		Error( "%s missing.\n", pszFile );
		return false;
	}
	
	KeyValues *pPrefabs = m_pSchemaData->FindKey( "prefabs" );
	if ( pPrefabs )
	{
		ParsePrefabs( pPrefabs );
	}
	
	KeyValues *pGameInfo = m_pSchemaData->FindKey( "game_info" );
	if ( pGameInfo )
	{
		ParseGameInfo( pGameInfo );
	}
	
	KeyValues *pQualities = m_pSchemaData->FindKey( "qualities" );
	if ( pQualities )
	{
		ParseQualities( pQualities );
	}
	
	KeyValues *pColors = m_pSchemaData->FindKey( "colors" );
	if ( pColors )
	{
		ParseColors( pColors );
	}
	
	KeyValues *pAttributes = m_pSchemaData->FindKey( "attributes" );
	if ( pAttributes )
	{
		ParseAttributes( pAttributes );
	}
	
	KeyValues *pItems = m_pSchemaData->FindKey( "items" );
	if ( pItems )
	{
		ParseItems( pItems );
	}
	
	return true;
}
	
void CEconItemSchema::ParseGameInfo( KeyValues *pKeyValuesData )
{
	for ( KeyValues *pSubData = pKeyValuesData->GetFirstSubKey(); pSubData != NULL; pSubData = pSubData->GetNextKey() )
	{
		m_GameInfo.Insert( pSubData->GetName(), pSubData->GetFloat() );
	}
}

void CEconItemSchema::ParseQualities( KeyValues *pKeyValuesData )
{
	for ( KeyValues *pSubData = pKeyValuesData->GetFirstSubKey(); pSubData != NULL; pSubData = pSubData->GetNextKey() )
	{
		EconQuality Quality;
		GET_INT( ( &Quality ), pSubData, value );
		m_Qualities.Insert( pSubData->GetName(), Quality );
	}
}

void CEconItemSchema::ParseColors( KeyValues *pKeyValuesData )
{
	for ( KeyValues *pSubData = pKeyValuesData->GetFirstSubKey(); pSubData != NULL; pSubData = pSubData->GetNextKey() )
	{
		EconColor ColorDesc;
		GET_STRING( ( &ColorDesc ), pSubData, color_name );
		m_Colors.Insert( pSubData->GetName(), ColorDesc );
	}
}

void CEconItemSchema::ParsePrefabs( KeyValues *pKeyValuesData )
{
	for ( KeyValues *pSubData = pKeyValuesData->GetFirstSubKey(); pSubData != NULL; pSubData = pSubData->GetNextKey() )
	{
		KeyValues *Values = pSubData->MakeCopy();
		m_PrefabsValues.Insert( pSubData->GetName(), Values );
	}
}

void CEconItemSchema::ParseItems( KeyValues *pKeyValuesData )
{
	for ( KeyValues *pSubData = pKeyValuesData->GetFirstSubKey(); pSubData != NULL; pSubData = pSubData->GetNextKey() )
	{
		// Skip over default item, not sure why it's there.
		if ( !V_stricmp( pSubData->GetName(), "default" ) )
			continue;
		
		CEconItemDefinition *Item = new CEconItemDefinition;
		int index = atoi( pSubData->GetName() );
		
		if ( ParseItemRec( pSubData, Item ) )
		{
			m_Items.Insert( index, Item );
		}
		else
		{
			delete Item;
		}
	}
	for ( unsigned int i = 0; i < m_PrefabsValues.Count(); i++ )
	{
		m_PrefabsValues[i]->deleteThis();
	}
	m_PrefabsValues.RemoveAll();
}

void CEconItemSchema::ParseAttributes( KeyValues *pKeyValuesData )
{
	for ( KeyValues *pSubData = pKeyValuesData->GetFirstSubKey(); pSubData != NULL; pSubData = pSubData->GetNextKey() )
	{
		EconAttributeDefinition *pAttribute = new EconAttributeDefinition;
		int index = atoi( pSubData->GetName() );
		
		GET_STRING_DEFAULT( pAttribute, pSubData, name, ( unnamed ) );
		GET_STRING( pAttribute, pSubData, attribute_class );
		GET_STRING( pAttribute, pSubData, description_string );
		pAttribute->string_attribute = ( V_stricmp( pSubData->GetString( "attribute_type" ), "string" ) == 0 );
		
		const char *pszFormat = pSubData->GetString( "description_format" );
		pAttribute->description_format = UTIL_StringFieldToInt( pszFormat, g_AttributeDescriptionFormats, ARRAYSIZE( g_AttributeDescriptionFormats ) );
		
		const char *pszEffect = pSubData->GetString( "effect_type" );
		pAttribute->effect_type = UTIL_StringFieldToInt( pszEffect, g_EffectTypes, ARRAYSIZE( g_EffectTypes ) );
		
		GET_BOOL( pAttribute, pSubData, hidden );
		GET_BOOL( pAttribute, pSubData, stored_as_integer );
		
		m_Attributes.Insert( index, pAttribute );
	}
};

bool CEconItemSchema::ParseVisuals( KeyValues *pData, CEconItemDefinition* pItem, int iIndex )
{
	EconItemVisuals *pVisuals = &pItem->visual[iIndex];
	
	for ( KeyValues *pVisualData = pData->GetFirstSubKey(); pVisualData != NULL; pVisualData = pVisualData->GetNextKey() )
	{
		if ( !V_stricmp( pVisualData->GetName(), "player_bodygroups" ) )
		{
			GET_VALUES_FAST_BOOL( pVisuals->player_bodygroups, pVisualData );
		}
		else if ( !V_stricmp( pVisualData->GetName(), "attached_models" ) )
		{
			// TODO
		}
		else if ( !V_stricmp( pVisualData->GetName(), "animation_replacement" ) )
		{
			for ( KeyValues *pKeyData = pVisualData->GetFirstSubKey(); pKeyData != NULL; pKeyData = pKeyData->GetNextKey() )
			{
				int key = ActivityList_IndexForName( pKeyData->GetName() );
				int value = ActivityList_IndexForName( pKeyData->GetString() );
				
				if ( key != kActivityLookup_Missing && value != kActivityLookup_Missing )
				{
					pVisuals->animation_replacement.Insert( key, value );
				}
			}
		}
		else if ( !V_stricmp( pVisualData->GetName(), "playback_activity" ) )
		{
			GET_VALUES_FAST_STRING( pVisuals->playback_activity, pVisualData );
		}
		else if ( !V_strnicmp( pVisualData->GetName(), "sound_", 6 ) )
		{
			// Fetching this similar to weapon script file parsing.
			// Advancing pointer past sound_ prefix... why couldn't they just make a subsection for sounds?
			int iSound = GetWeaponSoundFromString( pVisualData->GetName() + 6 );
			
			if ( iSound != -1 )
			{
				V_strncpy( pVisuals->aWeaponSounds[iSound], pVisualData->GetString(), MAX_WEAPON_STRING );
			}
		}
		else if ( !V_stricmp( pVisualData->GetName(), "styles" ) )
		{
		/*
			for (KeyValues *pStyleData = pVisualData->GetFirstSubKey(); pStyleData != NULL; pStyleData = pStyleData->GetNextKey())
			{
				EconItemStyle *style;
				IF_ELEMENT_FOUND(visual->styles, pStyleData->GetName())
				{
					style = visual->styles.Element(index);
				}
				else
				{
					style = new EconItemStyle();
					visual->styles.Insert(pStyleData->GetName(), style);
				}
				
				GET_STRING(style, pStyleData, name);
				GET_STRING(style, pStyleData, model_player);
				GET_STRING(style, pStyleData, image_inventory);
				GET_BOOL(style, pStyleData, selectable);
				GET_INT(style, pStyleData, skin_red);
				GET_INT(style, pStyleData, skin_blu);
				
				for (KeyValues *pStyleModelData = pStyleData->GetFirstSubKey(); pStyleModelData != NULL; pStyleModelData = pStyleModelData->GetNextKey())
				{
					if (!V_stricmp(pStyleModelData->GetName(), "model_player_per_class"))
					{
						GET_VALUES_FAST_STRING(style->model_player_per_class, pStyleModelData);
					}
				}
			}
		*/
		}
		else
		{
			GET_VALUES_FAST_STRING( pVisuals->misc_info, pVisualData );
		}
	}
	
	return true;
}

bool CEconItemSchema::ParseItemRec( KeyValues *pData, CEconItemDefinition* pItem )
{
	char prefab[64];
	Q_snprintf( prefab, sizeof( prefab ), "%s", pData->GetString( "prefab" ) );	//check if there's prefab for prefab.. PREFABSEPTION
	
	if ( prefab[0] != '\0' )
	{
		char * pch;
		pch = strtok( prefab, " " );
		while ( pch != NULL )
		{
			KeyValues *pPrefabValues = NULL;
			FIND_ELEMENT( m_PrefabsValues, pch, pPrefabValues );
			if ( pPrefabValues )
			{
				ParseItemRec( pPrefabValues, pItem );
			}
			pch = strtok( NULL, " " );
		}
	}
	
	GET_STRING( pItem, pData, name );
	GET_BOOL( pItem, pData, show_in_armory );
	
	GET_STRING( pItem, pData, item_class );
	GET_STRING( pItem, pData, item_name );
	GET_STRING( pItem, pData, item_description );
	GET_STRING( pItem, pData, item_type_name );
	
	const char *pszQuality = pData->GetString( "item_quality" );
	if ( pszQuality[0] )
	{
		int iQuality = UTIL_StringFieldToInt( pszQuality, g_szQualityStrings, ARRAYSIZE( g_szQualityStrings ) );
		if ( iQuality != -1 )
		{
			pItem->item_quality = (EEconItemQuality)iQuality;
		}
	}
	
	GET_STRING( pItem, pData, item_logname );
	GET_STRING( pItem, pData, item_iconname );
	GET_BOOL( pItem, pData, propername );
	
	const char *pszLoadoutSlot = pData->GetString( "item_slot" );
	
	if ( pszLoadoutSlot[0] )
	{
		pItem->item_slot = (ETFLoadoutSlot)UTIL_StringFieldToInt( pszLoadoutSlot, g_LoadoutSlots, TF_LOADOUT_SLOT_COUNT );
	}
	
	// SanyaSho: one new check from 2017 build // this is a very weird check which brokes all types of shotgun, but in 2017 build it works fine
	//if ( pItem->item_slot == TF_LOADOUT_SLOT_INVALID )
	//	return false;
	
	const char *pszAnimSlot = pData->GetString( "anim_slot" );
	if ( pszAnimSlot[0] )
	{
		if ( V_strcmp( pszAnimSlot, "FORCE_NOT_USED" ) != 0 )
		{
			pItem->anim_slot = (ETFWeaponType)UTIL_StringFieldToInt( pszAnimSlot, g_AnimSlots, TF_WPN_TYPE_COUNT );
		}
		else
		{
			pItem->anim_slot = TF_WPN_TYPE_NOT_USED;
		}
	}
	
	GET_BOOL( pItem, pData, baseitem );
	GET_INT( pItem, pData, min_ilevel );
	GET_INT( pItem, pData, max_ilevel );
	
	GET_STRING( pItem, pData, image_inventory );
	GET_INT( pItem, pData, image_inventory_size_w );
	GET_INT( pItem, pData, image_inventory_size_h );
	
#if defined( CLIENT_DLL )
	if( pItem->image_inventory[0] )
	{
		CHudTexture *texture = new CHudTexture();
		texture->bRenderUsingFont = false;
		texture->rc.left = 0;
		texture->rc.top = 0;
		texture->rc.right = 512;
		texture->rc.bottom = 512;
		
		Q_snprintf( texture->szShortName, sizeof( texture->szShortName ), "econ_item_%d", V_atoi( pData->GetName() ) );
		Q_snprintf( texture->szTextureFile, sizeof( texture->szTextureFile ), "%s_large", pItem->image_inventory );
		CHudTexture *pTexture = gHUD.AddUnsearchableHudIconToList( *texture );
		pItem->pIcon = pTexture;
		if( pTexture )
			pTexture->Precache();
	}
#endif
	
	GET_STRING( pItem, pData, model_player );
	GET_STRING( pItem, pData, model_world );
	
	GET_INT( pItem, pData, attach_to_hands );
	GET_BOOL( pItem, pData, act_as_wearable );
	
	GET_STRING( pItem, pData, mouse_pressed_sound );
	GET_STRING( pItem, pData, drop_sound );
	
	for ( KeyValues *pSubData = pData->GetFirstSubKey(); pSubData != NULL; pSubData = pSubData->GetNextKey() )
	{
		if ( !V_stricmp( pSubData->GetName(), "capabilities" ) )
		{
			GET_VALUES_FAST_BOOL( pItem->capabilities, pSubData );
		}
		else if ( !V_stricmp( pSubData->GetName(), "tags" ) )
		{
			GET_VALUES_FAST_BOOL( pItem->tags, pSubData );
		}
		else if ( !V_stricmp( pSubData->GetName(), "model_player_per_class" ) )
		{
			for ( KeyValues *pClassData = pSubData->GetFirstSubKey(); pClassData != NULL; pClassData = pClassData->GetNextKey() )
			{
				const char *pszClass = pClassData->GetName();
				int iClass = UTIL_StringFieldToInt( pszClass, g_aPlayerClassNames_NonLocalized, TF_CLASS_COUNT_ALL );
				
				if ( iClass != -1 )
				{
					V_strncpy( pItem->model_player_per_class[iClass], pClassData->GetString(), 128 );
				}
			}
		}
		else if ( !V_stricmp( pSubData->GetName(), "used_by_classes" ) )
		{
			for ( KeyValues *pClassData = pSubData->GetFirstSubKey(); pClassData != NULL; pClassData = pClassData->GetNextKey() )
			{
				const char *pszClass = pClassData->GetName();
				int iClass = UTIL_StringFieldToInt( pszClass, g_aPlayerClassNames_NonLocalized, TF_CLASS_COUNT_ALL );
				
				if ( iClass != -1 )
				{
					pItem->used_by_classes |= ( 1 << iClass );
					const char *pszSlotname = pClassData->GetString();
					
					if ( pszSlotname[0] != '1' )
					{
						pItem->item_slot_per_class[iClass] = (ETFLoadoutSlot)UTIL_StringFieldToInt( pszSlotname, g_LoadoutSlots, TF_LOADOUT_SLOT_COUNT );
					}
				}
			}
		}
		else if ( !V_stricmp( pSubData->GetName(), "attributes" ) )
		{
			for ( KeyValues *pAttribData = pSubData->GetFirstSubKey(); pAttribData != NULL; pAttribData = pAttribData->GetNextKey() )
			{
				int iAttributeID = GetAttributeIndex( pAttribData->GetName() );
				
				if ( iAttributeID == -1 )
					continue;
				
				EconAttributeDefinition *pAttribDef = GetAttributeDefinition( iAttributeID );
				
				if ( pAttribDef->string_attribute )
				{
					CEconItemAttribute attribute( iAttributeID, pAttribData->GetString( "value" ), pAttribData->GetString( "attribute_class" ) );
					pItem->attributes.AddToTail( attribute );
				}
				else
				{
					CEconItemAttribute attribute( iAttributeID, pAttribData->GetFloat( "value" ), pAttribData->GetString( "attribute_class" ) );
					pItem->attributes.AddToTail( attribute );
				}
			}
		}
		else if ( !V_stricmp( pSubData->GetName(), "visuals_mvm_boss" ) )
		{
			// Deliberately skipping this.
		}
		else if ( !V_strnicmp( pSubData->GetName(), "visuals", 7 ) )
		{
			// Figure out what team is this meant for.
			int iVisuals = UTIL_StringFieldToInt( pSubData->GetName(), g_TeamVisualSections, TF_TEAM_COUNT );
			
			if ( iVisuals != -1 )
			{
				if ( iVisuals == TEAM_UNASSIGNED )
				{
					// Hacky: for standard visuals block, assign it to all teams at once.
					for ( int i = 0; i < TF_TEAM_COUNT; i++ )
					{
						if ( i == TEAM_SPECTATOR )
							continue;
						
						ParseVisuals( pSubData, pItem, i );
					}
				}
				else
				{
					ParseVisuals( pSubData, pItem, iVisuals );
				}
			}
		}
		else if( !V_strnicmp( pSubData->GetName(), "taunt", 5 ) )
		{
			InitPerClassStringVectorArray( pSubData->FindKey( "custom_taunt_scene_per_class" ), pItem->taunt.custom_taunt_scene_per_class );
			KeyValues *pKV = pSubData->FindKey( "custom_partner_taunt_per_class" );
			if( pKV )
			{
				InitPerClassStringVectorArray( pKV, pItem->taunt.custom_partner_taunt_initiator_per_class );
				InitPerClassStringVectorArray( pKV, pItem->taunt.custom_partner_taunt_receiver_per_class );
			}
			InitPerClassStringVectorArray( pSubData->FindKey( "custom_partner_taunt_initiator_per_class" ), pItem->taunt.custom_partner_taunt_initiator_per_class );
			InitPerClassStringVectorArray( pSubData->FindKey( "custom_partner_taunt_receiver_per_class" ), pItem->taunt.custom_partner_taunt_receiver_per_class );
			InitPerClassStringVectorArray( pSubData->FindKey( "custom_taunt_outro_scene_per_class" ), pItem->taunt.custom_taunt_outro_scene_per_class );
			InitPerClassStringArray( pSubData->FindKey( "custom_taunt_prop_per_class" ), pItem->taunt.custom_taunt_prop_per_class );
			InitPerClassStringArray( pSubData->FindKey( "custom_taunt_prop_scene_per_class" ), pItem->taunt.custom_taunt_prop_scene_per_class );
			InitPerClassStringArray( pSubData->FindKey( "custom_taunt_prop_outro_scene_per_class" ), pItem->taunt.custom_taunt_prop_outro_scene_per_class );
			const char *pszTauntAttack = pSubData->GetString( "taunt_attack_name" );
			if( pszTauntAttack && pszTauntAttack[0] )
			{
				int iData = UTIL_StringFieldToInt( pszTauntAttack, taunt_attack_name, TF_TAUNTATK_COUNT );
				if( iData != -1 )
				{
					pItem->taunt.taunt_attack = iData;
				}
			}
			pItem->taunt.is_hold_taunt = pSubData->GetBool( "is_hold_taunt", pItem->taunt.is_hold_taunt );
			pItem->taunt.is_partner_taunt = pSubData->GetBool( "is_partner_taunt", pItem->taunt.is_partner_taunt );
			pItem->taunt.taunt_attack_time = pSubData->GetFloat( "taunt_attack_time", pItem->taunt.taunt_attack_time );
			pItem->taunt.taunt_separation_forward_distance = pSubData->GetFloat( "taunt_separation_forward_distance", pItem->taunt.taunt_separation_forward_distance );
			pItem->taunt.stop_taunt_if_moved = pSubData->GetBool( "stop_taunt_if_moved", pItem->taunt.stop_taunt_if_moved );
			pItem->taunt.taunt_success_sound = pSubData->GetString( "taunt_success_sound", pItem->taunt.taunt_success_sound );
			pItem->taunt.taunt_success_sound_loop = pSubData->GetString( "taunt_success_sound_loop", pItem->taunt.taunt_success_sound_loop );
			pItem->taunt.taunt_move_speed = pSubData->GetFloat( "taunt_move_speed", pItem->taunt.taunt_move_speed );
			pItem->taunt.taunt_turn_speed = pSubData->GetFloat( "taunt_turn_speed", pItem->taunt.taunt_turn_speed );
			pItem->taunt.taunt_force_move_forward = pSubData->GetBool( "taunt_force_move_forward", pItem->taunt.taunt_force_move_forward );
			pItem->taunt.taunt_mimic = pSubData->GetBool( "taunt_mimic", pItem->taunt.taunt_mimic );
			const char *pszWeaponSlot = pSubData->GetString( "taunt_force_weapon_slot" );
			if( pszWeaponSlot && pszWeaponSlot[0] )
			{
				ETFLoadoutSlot iSlot = (ETFLoadoutSlot)UTIL_StringFieldToInt( pszWeaponSlot, g_LoadoutSlots, TF_LOADOUT_SLOT_COUNT );
				if( iSlot != TF_LOADOUT_SLOT_INVALID )
				{
					pItem->taunt.taunt_force_weapon_slot = iSlot;
				}
			}
		}
	}
	
	return true;
}
