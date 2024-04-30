//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: Client side C_TFTeam class
//
// $NoKeywords: $
//=============================================================================
#include "cbase.h"
#include "c_tf_team.h"
#include "tf_shareddefs.h"
#include <vgui_controls/Controls.h>
#include <vgui/ILocalize.h>
#include "c_baseobject.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: RecvProxy that converts the Player's object UtlVector to entindexes
//-----------------------------------------------------------------------------
void RecvProxy_TeamObjectList( const CRecvProxyData *pData, void *pStruct, void *pOut )
{
	C_TFTeam *pPlayer = (C_TFTeam*)pStruct;
	CBaseHandle *pHandle = (CBaseHandle*)(&(pPlayer->m_aObjects[pData->m_iElement])); 
	RecvProxy_IntToEHandle( pData, pStruct, pHandle );
}

void RecvProxyArrayLength_TeamObjects( void *pStruct, int objectID, int currentArrayLength )
{
	C_TFTeam *pPlayer = (C_TFTeam*)pStruct;
	
	if ( pPlayer->m_aObjects.Count() != currentArrayLength )
	{
		pPlayer->m_aObjects.SetSize( currentArrayLength );
	}
}

IMPLEMENT_CLIENTCLASS_DT( C_TFTeam, DT_TFTeam, CTFTeam )
	RecvPropInt( RECVINFO( m_nFlagCaptures ) ),
	RecvPropInt( RECVINFO( m_iRole ) ),
	RecvPropBool( RECVINFO( m_bEscorting ) ),
	RecvPropInt( RECVINFO( m_iRoundScore ) ),
	
	RecvPropArray2( RecvProxyArrayLength_TeamObjects, RecvPropInt( "team_object_array_element", 0, SIZEOF_IGNORE, 0, RecvProxy_TeamObjectList ), MAX_PLAYERS * MAX_OBJECTS_PER_PLAYER, 0, "team_object_array" ),
END_RECV_TABLE()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TFTeam::C_TFTeam()
{
	m_nFlagCaptures = 0;
	m_iRole = 0;
	m_bEscorting = false;
	m_iRoundScore = 0;
	m_wszLocalizedTeamName[0] = '\0';
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TFTeam::~C_TFTeam()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_TFTeam::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );

	if ( updateType == DATA_UPDATE_CREATED )
	{
		UpdateTeamName();
		SetNextClientThink( gpGlobals->curtime + 0.5f );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_TFTeam::ClientThink( void )
{
	UpdateTeamName();
	SetNextClientThink( gpGlobals->curtime + 0.5f );
}

int C_TFTeam::GetNumObjects( int iObjectType )
{
	// Asking for a count of a specific object type?
	if ( iObjectType > 0 )
	{
		int iCount = 0;
		for ( int i = 0; i < GetNumObjects(); i++ )
		{
			C_BaseObject *pObject = GetObject(i);
			if ( pObject && pObject->GetType() == iObjectType )
			{
				iCount++;
			}
		}
		return iCount;
	}
	
	return m_aObjects.Count();
}

C_BaseObject *C_TFTeam::GetObject( int num )
{
	Assert( num >= 0 && num < m_aObjects.Count() );
	return m_aObjects[ num ];
}
	
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_TFTeam::UpdateTeamName( void )
{
	// TODO: Add tournament mode team name handling here.
	const wchar_t *pwszLocalized = NULL;

	switch ( GetTeamNumber() )
	{
	case TF_TEAM_RED:
		pwszLocalized = g_pVGuiLocalize->Find( "#TF_RedTeam_Name" );
		break;
	case TF_TEAM_BLUE:
		pwszLocalized = g_pVGuiLocalize->Find( "#TF_BlueTeam_Name" );
		break;
	case TF_TEAM_GREEN:
		pwszLocalized = g_pVGuiLocalize->Find( "#TF_GreenTeam_Name" );
		break;
	case TF_TEAM_YELLOW:
		pwszLocalized = g_pVGuiLocalize->Find( "#TF_YellowTeam_Name" );
		break;
	case TEAM_SPECTATOR:
		pwszLocalized = g_pVGuiLocalize->Find( "#TF_Spectators" );
		break;
	}

	if ( pwszLocalized )
	{
		V_wcscpy_safe( m_wszLocalizedTeamName, pwszLocalized );
	}
	else
	{
		g_pVGuiLocalize->ConvertANSIToUnicode( g_aTeamNames[GetTeamNumber()], m_wszLocalizedTeamName, sizeof( m_wszLocalizedTeamName ) );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Get the C_TFTeam for the specified team number
//-----------------------------------------------------------------------------
C_TFTeam *GetGlobalTFTeam( int iTeamNumber )
{
	return assert_cast<C_TFTeam *>( GetGlobalTeam( iTeamNumber ) );
}

const wchar_t *GetLocalizedTeamName( int iTeamNumber )
{
	C_TFTeam *pTeam = GetGlobalTFTeam( iTeamNumber );
	if ( pTeam )
	{
		return pTeam->GetTeamName();
	}

	return L"";
}
