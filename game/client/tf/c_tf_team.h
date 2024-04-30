//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: Client side CTFTeam class
//
// $NoKeywords: $
//=============================================================================

#ifndef C_TF_TEAM_H
#define C_TF_TEAM_H
#ifdef _WIN32
#pragma once
#endif

#include "c_team.h"
#include "shareddefs.h"

class C_BaseEntity;
class C_BaseObject;
class CBaseTechnology;

//-----------------------------------------------------------------------------
// Purpose: TF's Team manager
//-----------------------------------------------------------------------------
class C_TFTeam : public C_Team
{
	DECLARE_CLASS( C_TFTeam, C_Team );
	DECLARE_CLIENTCLASS();
public:
	C_TFTeam();
	virtual ~C_TFTeam();
	
	int GetFlagCaptures( void ) { return m_nFlagCaptures; }
	int GetRole( void ) { return m_iRole; }
	
	virtual void OnDataChanged( DataUpdateType_t updateType );
	virtual void ClientThink( void );

	int GetNumObjects( int iObjectType = -1 );
	C_BaseObject *GetObject( int num );
	
	int GetRoundScore( void ) { return m_iRoundScore; }
	void UpdateTeamName( void );
	const wchar_t *GetTeamName( void ) { return m_wszLocalizedTeamName; }

	CUtlVector< CHandle<C_BaseObject> > m_aObjects;
	
	bool IsEscorting() const { return m_bEscorting; }
private:
	int m_nFlagCaptures;
	int m_iRole;
	bool m_bEscorting;
	int m_iRoundScore;
	wchar_t m_wszLocalizedTeamName[128];
};

C_TFTeam *GetGlobalTFTeam( int iTeamNumber );
const wchar_t *GetLocalizedTeamName( int iTeamNumber );

#endif // C_TF_TEAM_H
