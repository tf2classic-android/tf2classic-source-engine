//========= Copyright Valve Corporation, All rights reserved. ============//
// tf_bot_dead.cpp
// Push up daisies
// Michael Booth, May 2009

#include "cbase.h"
#include "tf_player.h"
#include "tf_gamerules.h"
#include "bot/tf_bot.h"
#include "bot/behavior/tf_bot_dead.h"
#include "bot/behavior/tf_bot_behavior.h"
#include "bot/behavior/scenario/deathmatch/tf_bot_taunt_deathmatch.h"

#include "nav_mesh.h"


CTFBotDead::CTFBotDead( CTFPlayer *pAttacker )
{
	pTFAttacker = pAttacker;
	m_bSaidSomethingAboutYourMother = false;
}

//---------------------------------------------------------------------------------------------
ActionResult< CTFBot >	CTFBotDead::OnStart( CTFBot *me, Action< CTFBot > *priorAction )
{
	m_deadTimer.Start();

	return Continue();
}


//---------------------------------------------------------------------------------------------
ActionResult< CTFBot >	CTFBotDead::Update( CTFBot *me, float interval )
{
	if ( me->IsAlive() )
	{
		// how did this happen?
		return ChangeTo( new CTFBotMainAction, "This should not happen!" );
	}

	// print a message after a 1-1.5 second after ded
	if( (TFGameRules() && TFGameRules()->IsDeathmatch()) && m_deadTimer.IsGreaterThen( RandomFloat( 1.f, 1.5f ) ) && ( RandomFloat( 0.f, 100.f ) <= tf_bot_chat_chance.GetFloat() ) )
	{
		if( pTFAttacker )
		{
			if( !m_bSaidSomethingAboutYourMother )
			{
				g_TFBotDeathmatchReaction.ReactOnEvent( EVENT_GOT_KILLED, me, pTFAttacker );
				m_bSaidSomethingAboutYourMother = true;
			}
		}
	}

	if ( m_deadTimer.IsGreaterThen( 5.0f ) )
	{
		if ( me->HasAttribute( CTFBot::REMOVE_ON_DEATH ) )
		{
			// remove dead bots
			engine->ServerCommand( UTIL_VarArgs( "kickid %d\n", me->GetUserID() ) );
		}
		else if ( me->HasAttribute( CTFBot::BECOME_SPECTATOR_ON_DEATH ) )
		{
			me->ChangeTeam( TEAM_SPECTATOR /*, false, true*/ );
			return Done();
		}
	}

#ifdef TF_RAID_MODE
	if ( TFGameRules()->IsRaidMode() && me->GetTeamNumber() == TF_TEAM_RED )
	{
		// dead defenders go to spectator for recycling
		me->ChangeTeam( TEAM_SPECTATOR, false, true );
	}
#endif // TF_RAID_MODE

	return Continue();
}

