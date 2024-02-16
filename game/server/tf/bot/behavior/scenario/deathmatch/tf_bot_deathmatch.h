// tf_bot_deathmatch.h
// Run. Think. Shoot. Live
// SanyaSho, February 2024


#ifndef TF_BOT_DEATHMATCH_H
#define TF_BOT_DEATHMATCH_H

#include "Path/NextBotChasePath.h"

//
// Roam around the map attacking enemies
//
class CTFBotDeathmatch : public Action< CTFBot >
{
public:
	CTFBotDeathmatch();

	virtual ActionResult< CTFBot >	OnStart( CTFBot *me, Action< CTFBot > *priorAction );
	virtual ActionResult< CTFBot >	Update( CTFBot *me, float interval );

	virtual ActionResult< CTFBot >	OnResume( CTFBot *me, Action< CTFBot > *interruptingAction );

	virtual EventDesiredResult< CTFBot > OnStuck( CTFBot *me );
	virtual EventDesiredResult< CTFBot > OnMoveToSuccess( CTFBot *me, const Path *path );
	virtual EventDesiredResult< CTFBot > OnMoveToFailure( CTFBot *me, const Path *path, MoveToFailureType reason );

	virtual QueryResultType	ShouldRetreat( const INextBot *me ) const;	// is it time to retreat?
	virtual QueryResultType ShouldHurry( const INextBot *me ) const;		// are we in a hurry?

	virtual EventDesiredResult< CTFBot > OnTerritoryCaptured( CTFBot *me, int territoryID );
	virtual EventDesiredResult< CTFBot > OnTerritoryLost( CTFBot *me, int territoryID );
	virtual EventDesiredResult< CTFBot > OnTerritoryContested( CTFBot *me, int territoryID );

	virtual const char *GetName( void ) const
	{
		return "Deathmatch";
	};

private:
	CBaseEntity *m_goalEntity;
	CBaseEntity *ChooseGoalEntity( CTFBot *me );

	PathFollower m_PathFollower;
	CountdownTimer m_ctRecomputePath;
	void RecomputeSeekPath( CTFBot *me );
};


#endif // TF_BOT_DEATHMATCH_H
