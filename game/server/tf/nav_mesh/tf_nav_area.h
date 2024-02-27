//========= Copyright Valve Corporation, All rights reserved. ============//
// tf_nav_area.h
// TF specific nav area
// Michael Booth, February 2009

#ifndef TF_NAV_AREA_H
#define TF_NAV_AREA_H

#include "nav_area.h"
#include "tf_shareddefs.h"

enum TFNavAttributeType : uint64
{
	TF_NAV_INVALID						= 0x000000000,

	// Also look for NAV_MESH_NAV_BLOCKER (w/ nav_debug_blocked ConVar).
	TF_NAV_BLOCKED						= ( 1ull << 0 ),			// blocked for some TF-specific reason
	TF_NAV_SPAWN_ROOM_RED				= ( 1ull << 1 ),
	TF_NAV_SPAWN_ROOM_BLUE				= ( 1ull << 2 ),
	TF_NAV_SPAWN_ROOM_EXIT				= ( 1ull << 3 ),
	TF_NAV_HAS_AMMO						= ( 1ull << 4 ),
	TF_NAV_HAS_HEALTH					= ( 1ull << 5 ),
	TF_NAV_CONTROL_POINT				= ( 1ull << 6 ),

	TF_NAV_BLUE_SENTRY_DANGER			= ( 1ull << 7 ),			// sentry can potentially fire upon enemies in this area
	TF_NAV_RED_SENTRY_DANGER			= ( 1ull << 8 ),

	TF_NAV_BLUE_SETUP_GATE				= ( 1ull << 9 ),			// this area is blocked until the setup period is over
	TF_NAV_RED_SETUP_GATE				= ( 1ull << 10 ),			// this area is blocked until the setup period is over
	TF_NAV_BLOCKED_AFTER_POINT_CAPTURE	= ( 1ull << 11 ),			// this area becomes blocked after the first point is capped
	TF_NAV_BLOCKED_UNTIL_POINT_CAPTURE  = ( 1ull << 12 ),			// this area is blocked until the first point is capped, then is unblocked
	TF_NAV_BLUE_ONE_WAY_DOOR			= ( 1ull << 13 ),
	TF_NAV_RED_ONE_WAY_DOOR				= ( 1ull << 14 ),

 	TF_NAV_WITH_SECOND_POINT			= ( 1ull << 15 ),			// modifier for BLOCKED_*_POINT_CAPTURE
 	TF_NAV_WITH_THIRD_POINT				= ( 1ull << 16 ),			// modifier for BLOCKED_*_POINT_CAPTURE
  	TF_NAV_WITH_FOURTH_POINT			= ( 1ull << 17 ),			// modifier for BLOCKED_*_POINT_CAPTURE
 	TF_NAV_WITH_FIFTH_POINT				= ( 1ull << 18 ),			// modifier for BLOCKED_*_POINT_CAPTURE

	TF_NAV_SNIPER_SPOT					= ( 1ull << 19 ),			// this is a good place for a sniper to lurk
	TF_NAV_SENTRY_SPOT					= ( 1ull << 20 ),			// this is a good place to build a sentry

	TF_NAV_ESCAPE_ROUTE					= ( 1ull << 21 ),			// for Raid mode
	TF_NAV_ESCAPE_ROUTE_VISIBLE			= ( 1ull << 22 ),			// all areas that have visibility to the escape route

	TF_NAV_NO_SPAWNING					= ( 1ull << 23 ),			// don't spawn bots in this area

 	TF_NAV_RESCUE_CLOSET				= ( 1ull << 24 ),			// for respawning friends in Raid mode

 	TF_NAV_BOMB_CAN_DROP_HERE			= ( 1ull << 25 ),			// the bomb can be dropped here and reached by the invaders in MvM

	TF_NAV_DOOR_NEVER_BLOCKS			= ( 1ull << 26 ),
	TF_NAV_DOOR_ALWAYS_BLOCKS			= ( 1ull << 27 ),

	TF_NAV_UNBLOCKABLE					= ( 1ull << 28 ),			// this area cannot be blocked

	// TF2C
	TF_NAV_SPAWN_ROOM_GREEN				= ( 1ull << 29 ),
	TF_NAV_SPAWN_ROOM_YELLOW			= ( 1ull << 30 ),
	TF_NAV_GREEN_SENTRY_DANGER			= ( 1ull << 31 ),
	TF_NAV_YELLOW_SENTRY_DANGER			= ( 1ull << 32 ),
	TF_NAV_GREEN_SETUP_GATE				= ( 1ull << 33 ),
	TF_NAV_YELLOW_SETUP_GATE			= ( 1ull << 34 ),
	TF_NAV_GREEN_ONE_WAY_DOOR			= ( 1ull << 35 ),
	TF_NAV_YELLOW_ONE_WAY_DOOR			= ( 1ull << 36 ),

	TF_NAV_HAS_DM_WEAPON				= ( 1ull << 37 ),
	TF_NAV_HAS_DM_POWERUP				= ( 1ull << 38 ),

	TF_NAV_MASK_SPAWN_ROOM				= ( TF_NAV_SPAWN_ROOM_RED | TF_NAV_SPAWN_ROOM_BLUE | TF_NAV_SPAWN_ROOM_GREEN | TF_NAV_SPAWN_ROOM_YELLOW ),
	TF_NAV_MASK_SENTRY_DANGER			= ( TF_NAV_RED_SENTRY_DANGER | TF_NAV_BLUE_SENTRY_DANGER | TF_NAV_GREEN_SENTRY_DANGER | TF_NAV_YELLOW_SENTRY_DANGER ),
	TF_NAV_MASK_SETUP_GATE				= ( TF_NAV_BLUE_SETUP_GATE | TF_NAV_RED_SETUP_GATE | TF_NAV_GREEN_SETUP_GATE | TF_NAV_YELLOW_SETUP_GATE ),

	// save/load these manually set flags, and don't clear them between rounds
	TF_NAV_PERSISTENT_ATTRIBUTES
		= TF_NAV_SNIPER_SPOT | TF_NAV_SENTRY_SPOT | TF_NAV_NO_SPAWNING
		| TF_NAV_BLUE_SETUP_GATE | TF_NAV_RED_SETUP_GATE
		| TF_NAV_BLOCKED_AFTER_POINT_CAPTURE | TF_NAV_BLOCKED_UNTIL_POINT_CAPTURE
		| TF_NAV_BLUE_ONE_WAY_DOOR | TF_NAV_RED_ONE_WAY_DOOR
		| TF_NAV_DOOR_NEVER_BLOCKS | TF_NAV_DOOR_ALWAYS_BLOCKS
		| TF_NAV_UNBLOCKABLE
		| TF_NAV_WITH_SECOND_POINT | TF_NAV_WITH_THIRD_POINT
		| TF_NAV_WITH_FOURTH_POINT | TF_NAV_WITH_FIFTH_POINT
		| TF_NAV_RESCUE_CLOSET
		// TF2C
		| TF_NAV_GREEN_SETUP_GATE | TF_NAV_YELLOW_SETUP_GATE
		| TF_NAV_GREEN_ONE_WAY_DOOR | TF_NAV_YELLOW_ONE_WAY_DOOR
};



class CTFNavArea : public CNavArea
{
public:
	DECLARE_CLASS( CTFNavArea, CNavArea );

	CTFNavArea( void );

	virtual void OnServerActivate( void );						// (EXTEND) invoked when map is initially loaded
	virtual void OnRoundRestart( void );						// (EXTEND) invoked for each area when the round restarts

	virtual void CustomAnalysis( bool isIncremental = false );	// for game-specific analysis
	virtual void Draw( void ) const;							// draw area for debugging & editing

	virtual void UpdateBlocked( bool force = false, int teamID = TEAM_ANY )		{ }		// we'll handle managing blocked status directly
	virtual bool IsBlocked( int teamID, bool ignoreNavBlockers = false ) const;

	virtual void Save( CUtlBuffer &fileBuffer, unsigned int version ) const;								// (EXTEND)
	virtual NavErrorType Load( CUtlBuffer &fileBuffer, unsigned int version, unsigned int subVersion );		// (EXTEND)

	float GetIncursionDistance( int team ) const;				// return travel distance from the team's active spawn room to this area, -1 for invalid
	CTFNavArea *GetNextIncursionArea( int team ) const;			// return adjacent area with largest increase in incursion distance
	bool IsReachableByTeam( int team ) const;					// return true if the given team can reach this area
	void CollectPriorIncursionAreas( int team, CUtlVector< CTFNavArea * > *priorVector );	// populate 'priorVector' with a collection of adjacent areas that have a lower incursion distance that this area
	void CollectNextIncursionAreas( int team, CUtlVector< CTFNavArea * > *priorVector );	// populate 'priorVector' with a collection of adjacent areas that have a higher incursion distance that this area

	const CUtlVector< CTFNavArea * > &GetEnemyInvasionAreaVector( int myTeam ) const;	// given OUR team index, return list of areas the enemy is invading from
	bool IsAwayFromInvasionAreas( int myTeam, float safetyRange = 1000.0f ) const;		// return true if this area is at least safetyRange units away from all invasion areas

	void ComputeInvasionAreaVectors( void );
	void SetInvasionSearchMarker( unsigned int marker );
	bool IsInvasionSearchMarked( unsigned int marker ) const;

	void SetAttributeTF( uint64 flags );
	void ClearAttributeTF( uint64 flags );
	bool HasAttributeTF( uint64 flags ) const;

	void AddPotentiallyVisibleActor( CBaseCombatCharacter *who );
	void RemovePotentiallyVisibleActor( CBaseCombatCharacter *who );
	void ClearAllPotentiallyVisibleActors( void );
	bool IsPotentiallyVisibleToActor( CBaseCombatCharacter *who ) const;	// return true if the given actor has potential visibility to this area

	virtual bool IsPotentiallyVisibleToTeam( int team ) const;				// return true if any portion of this area is visible to anyone on the given team (very fast)

	class IForEachPotentiallyVisibleActor
	{
	public:
		virtual bool Inspect( CBaseCombatCharacter *who ) = 0;
	};
	bool ForEachPotentiallyVisibleActor( IForEachPotentiallyVisibleActor &func, int team = TEAM_ANY );

	void OnCombat( void );										// invoked when combat happens in/near this area
	bool IsInCombat( void ) const;								// return true if this area has seen combat recently
	float GetCombatIntensity( void ) const;						// 1 = in active combat, 0 = quiet

	uint64 GetAttr_SpawnRoom( int iTeam ) const;
	uint64 GetAttr_Sentry( int iTeam ) const;

	static void MakeNewTFMarker( void );
	static void ResetTFMarker( void );
	bool IsTFMarked( void ) const;
	void TFMark( void );

	// Raid mode -------------------------------------------------
	void AddToWanderCount( int count );
	void SetWanderCount( int count );
	int GetWanderCount( void ) const;

	bool IsValidForWanderingPopulation( void ) const;
	// Raid mode -------------------------------------------------

	// Distance for MvM bomb delivery
	float GetTravelDistanceToBombTarget( void ) const;

private:
	friend class CTFNavMesh;

	float m_distanceFromSpawnRoom[ TF_TEAM_COUNT ];
	CUtlVector< CTFNavArea * > m_invasionAreaVector[ TF_TEAM_COUNT ];	// use our team as index to get list of areas the enemy is invading from
	unsigned int m_invasionSearchMarker;

	uint64 m_attributeFlags;

	CUtlVector< CHandle< CBaseCombatCharacter > > m_potentiallyVisibleActor[ TF_TEAM_COUNT ];

	float m_combatIntensity;
	IntervalTimer m_combatTimer;

	static unsigned int m_masterTFMark;
	unsigned int m_TFMark;					// this area's mark

	// Raid mode -------------------------------------------------
	int m_wanderCount;						// how many wandering defenders to populate here
	// Raid mode -------------------------------------------------

	float m_distanceToBombTarget;
};


inline float CTFNavArea::GetTravelDistanceToBombTarget( void ) const
{
	return m_distanceToBombTarget;
}

inline void CTFNavArea::AddToWanderCount( int count )
{
	m_wanderCount += count;
}

inline void CTFNavArea::SetWanderCount( int count )
{
	m_wanderCount = count;
}

inline int CTFNavArea::GetWanderCount( void ) const
{
	return m_wanderCount;
}



inline bool CTFNavArea::IsPotentiallyVisibleToActor( CBaseCombatCharacter *who ) const
{
	if ( who == NULL )
		return false;

	int team = who->GetTeamNumber();
	if ( team < 0 || team >= TF_TEAM_COUNT )
		return false;

	return m_potentiallyVisibleActor[ team ].Find( who ) != m_potentiallyVisibleActor[ team ].InvalidIndex();
}


inline bool CTFNavArea::IsPotentiallyVisibleToTeam( int team ) const
{
	return team >= 0 && team < TF_TEAM_COUNT && m_potentiallyVisibleActor[ team ].Count() > 0;
}


inline bool CTFNavArea::ForEachPotentiallyVisibleActor( CTFNavArea::IForEachPotentiallyVisibleActor &func, int team )
{
	if ( team == TEAM_ANY )
	{
		for( int t=0; t<TF_TEAM_COUNT; ++t )
		{
			for( int i=0; i<m_potentiallyVisibleActor[ t ].Count(); ++i )
			{
				CBaseCombatCharacter *who = m_potentiallyVisibleActor[ t ][ i ];

				if ( who && func.Inspect( who ) == false )
				{
					return false;
				}
			}
		}
	}
	else if ( team >= 0 && team < TF_TEAM_COUNT )
	{
		for( int i=0; i<m_potentiallyVisibleActor[ team ].Count(); ++i )
		{
			CBaseCombatCharacter *who = m_potentiallyVisibleActor[ team ][ i ];

			if ( who && func.Inspect( who ) == false )
			{
				return false;
			}
		}
	}

	return true;
}

inline void CTFNavArea::RemovePotentiallyVisibleActor( CBaseCombatCharacter *who )
{
	for( int i=0; i<TF_TEAM_COUNT; ++i )
		m_potentiallyVisibleActor[i].FindAndFastRemove( who );
}

inline void CTFNavArea::ClearAllPotentiallyVisibleActors( void )
{
	for( int i=0; i<TF_TEAM_COUNT; ++i )
		m_potentiallyVisibleActor[i].RemoveAll();
}

inline float CTFNavArea::GetIncursionDistance( int team ) const
{
	if ( team < 0 || team >= TF_TEAM_COUNT )
	{
		return -1.0f;
	}

	return m_distanceFromSpawnRoom[ team ];
}

inline bool CTFNavArea::IsReachableByTeam( int team ) const
{
	if ( team < 0 || team >= TF_TEAM_COUNT )
	{
		return false;
	}

	return m_distanceFromSpawnRoom[ team ] >= 0.0f;
}

inline const CUtlVector< CTFNavArea * > &CTFNavArea::GetEnemyInvasionAreaVector( int myTeam ) const
{
	if ( myTeam < 0 || myTeam >= TF_TEAM_COUNT )
	{
		myTeam = 0.0f;
	}

	return m_invasionAreaVector[ myTeam ];
}

inline void CTFNavArea::SetInvasionSearchMarker( unsigned int marker )
{
	m_invasionSearchMarker = marker;
}

inline bool CTFNavArea::IsInvasionSearchMarked( unsigned int marker ) const
{
	return marker == m_invasionSearchMarker;
}

inline void CTFNavArea::SetAttributeTF( uint64 flags )
{
	m_attributeFlags |= flags;
}

inline void CTFNavArea::ClearAttributeTF( uint64 flags )
{
	m_attributeFlags &= ~flags;
}

inline bool CTFNavArea::HasAttributeTF( uint64 flags ) const
{
	return ( m_attributeFlags & flags ) ? true : false;
}

#endif // TF_NAV_AREA_H