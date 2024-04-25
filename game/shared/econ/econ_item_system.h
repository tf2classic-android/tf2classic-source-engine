#ifndef ECON_ITEM_SYSTEM_H
#define ECON_ITEM_SYSTEM_H

#ifdef _WIN32
#pragma once
#endif

#include "econ_item_schema.h"

#define GET_STRING(copyto, from, name)\
	if (from->GetString(#name, NULL))\
		V_strncpy(copyto->name, from->GetString(#name), sizeof(copyto->name))

#define GET_STRING_DEFAULT(copyto, from, name, defaultstring)\
	V_strncpy(copyto->name, from->GetString(#name, #defaultstring), sizeof(copyto->name))

#define GET_BOOL(copyto, from, name)\
	copyto->name = from->GetBool(#name, copyto->name)

#define GET_FLOAT(copyto, from, name)\
	copyto->name = from->GetFloat(#name, copyto->name)

#define GET_INT(copyto, from, name)\
	copyto->name = from->GetInt(#name, copyto->name)

#define GET_STRING_CONVERT(copyto, from, name)\
	if (from->GetString(#name, NULL))

#define FIND_ELEMENT(map, key, val)\
	unsigned int index = map.Find(key);\
	if (index != map.InvalidIndex())\
		val = map.Element(index)				

#define FIND_ELEMENT_STRING(map, key, val)\
	unsigned int index = map.Find(key);\
	if (index != map.InvalidIndex())\
		Q_snprintf(val, sizeof(val), map.Element(index))

#define IF_ELEMENT_FOUND(map, key)\
	unsigned int index = map.Find(key);\
	if (index != map.InvalidIndex())			

#define GET_VALUES_FAST_BOOL(dict, keys)\
	for (KeyValues *pKeyData = keys->GetFirstSubKey(); pKeyData != NULL; pKeyData = pKeyData->GetNextKey())\
	{\
		IF_ELEMENT_FOUND(dict, pKeyData->GetName())\
		{\
			dict.Element(index) = pKeyData->GetBool();\
		}\
		else\
		{\
			dict.Insert(pKeyData->GetName(), pKeyData->GetBool());\
		}\
	}


#define GET_VALUES_FAST_STRING(dict, keys)\
	for (KeyValues *pKeyData = keys->GetFirstSubKey(); pKeyData != NULL; pKeyData = pKeyData->GetNextKey())\
	{\
		IF_ELEMENT_FOUND(dict, pKeyData->GetName())\
		{\
			Q_snprintf((char*)dict.Element(index), sizeof(dict.Element(index)), "%s", pKeyData->GetString());\
		}\
		else\
		{\
			dict.Insert(pKeyData->GetName(), strdup(pKeyData->GetString()));\
		}\
	}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CEconItemSchema
{
	friend class CTFInventory;

public:
	CEconItemSchema();
	~CEconItemSchema();

	bool Init( void );
	void Precache( void );

	CEconItemDefinition* GetItemDefinition( int id );
	int GetItemIndex( const char *pszName );
	EconAttributeDefinition *GetAttributeDefinition( int id );
	EconAttributeDefinition *GetAttributeDefinitionByName( const char* name );
	EconAttributeDefinition *GetAttributeDefinitionByClass( const char* name );
	int GetAttributeIndex( const char *classname );
	
	bool ParseItemSchema( const char *pszFile );
	void ParseGameInfo( KeyValues *pKeyValuesData );
	void ParseQualities( KeyValues *pKeyValuesData );
	void ParseColors( KeyValues *pKeyValuesData );
	void ParsePrefabs( KeyValues *pKeyValuesData );
	void ParseItems( KeyValues *pKeyValuesData );
	void ParseAttributes( KeyValues *pKeyValuesData );
	bool ParseVisuals( KeyValues *pData, CEconItemDefinition* pItem, int iIndex );
	bool ParseItemRec( KeyValues *pData, CEconItemDefinition* pItem );

private:
	CUtlDict< int, unsigned short >					m_GameInfo;
	CUtlDict< EconQuality, unsigned short >			m_Qualities;
	CUtlDict< EconColor, unsigned short >			m_Colors;
	CUtlDict< KeyValues *, unsigned short >			m_PrefabsValues;
	CUtlMap< int, CEconItemDefinition * >			m_Items;
	CUtlMap< int, EconAttributeDefinition * >		m_Attributes;

	KeyValues *m_pSchemaData;
};

CEconItemSchema *GetItemSchema();

#endif // ECON_ITEM_SYSTEM_H
