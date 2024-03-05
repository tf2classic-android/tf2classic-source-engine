#include "cbase.h"
#include "bot/tf_bot.h"
#include "bot/behavior/scenario/deathmatch/tf_bot_taunt_deathmatch.h"

#include "voice_gamemgr.h"

CON_COMMAND_F( tf2c_bot_deathmatch_reload_chat, "", FCVAR_CHEAT )
{
	g_TFBotDeathmatchReaction.LoadChatFile();
}

extern char *CheckChatText( CBasePlayer *pPlayer, char *text );

CTFBotDeathmatchReaction g_TFBotDeathmatchReaction( "TFBotDeathmatchReaction" );

CTFBotDeathmatchReaction::CTFBotDeathmatchReaction( const char *pszName ) : CAutoGameSystem( pszName )
{
}

bool CTFBotDeathmatchReaction::Init( void )
{
	return true;
}

void CTFBotDeathmatchReaction::LevelInitPreEntity( void )
{
	LoadChatFile();
}

void CTFBotDeathmatchReaction::ReactOnEvent( ETFBotDeathmatchReactionEvent ourevent, CTFPlayer *player1, CTFPlayer *player2 )
{
	if( !TFGameRules() || !TFGameRules()->IsDeathmatch() )
	{
		return;
	}
	
	switch( ourevent )
	{
	case EVENT_GOT_KILLED:
		// player2 - our killer
		BOT_Say( player1, GetFormatedParabotMessage( m_aGotKilledMessages.Random(), player2, player2 ).Get() );
		break;
	case EVENT_KILLED_PLAYER:
		// player 2 - our victim
		BOT_Say( player1, GetFormatedParabotMessage( m_aKilledPlayerMessages.Random(), player1, player2 ).Get() );
		break;
	case EVENT_GOT_WEAPON: // should be called when we've pickup a weapon_rpg, weapon_gauss, weapon_egon, weapon_crossbow
		// player2 - NULL
		BOT_Say( player1, GetFormatedParabotMessage( m_aGotWeaponMessages.Random(), player1, player1 ).Get() );
		break;
	case EVENT_JOINED_SERVER:
		// player2 - NULL
		BOT_Say( player1, GetFormatedParabotMessage( m_aJoinedServerMessages.Random(), player1, player1 ).Get() );
		break;
	default:
		Assert( 0 );
		break;
	};
}

void CTFBotDeathmatchReaction::LoadChatFile()
{
	KeyValues *pKV = new KeyValues( "ParabotChat" );
	if( !pKV )
	{
		// impossible
		return;
	}
	
	m_aGotKilledMessages.RemoveAll();
	m_aKilledPlayerMessages.RemoveAll();
	m_aGotWeaponMessages.RemoveAll();
	m_aJoinedServerMessages.RemoveAll();
	
	if( pKV->LoadFromFile( filesystem, "scripts/tf_bot_chat.txt", "MOD" ) )
	{
		for( KeyValues *pData = pKV->GetFirstSubKey(); pData != NULL; pData = pData->GetNextKey() )
		{
			if( !Q_stricmp( pData->GetName(), "GOT_KILLED" ) )
			{
				//Msg( "[GOT_KILLED] %s\n", pData->GetString() );
				m_aGotKilledMessages.AddToTail( CUtlString( pData->GetString() ) );
			}
			else if( !Q_stricmp( pData->GetName(), "KILLED_PLAYER" ) )
			{
				//Msg( "[KILLED_PLAYER] %s\n", pData->GetString() );
				m_aKilledPlayerMessages.AddToTail( CUtlString( pData->GetString() ) );
			}
			else if( !Q_stricmp( pData->GetName(), "GOT_WEAPON" ) )
			{
				//Msg( "[GOT_WEAPON] %s\n", pData->GetString() );
				m_aGotWeaponMessages.AddToTail( CUtlString( pData->GetString() ) );
			}
			else if( !Q_stricmp( pData->GetName(), "JOINED_SERVER" ) )
			{
				//Msg( "[JOINED_SERVER] %s\n", pData->GetString() );
				m_aJoinedServerMessages.AddToTail( CUtlString( pData->GetString() ) );
			}
		}
	}
	else
	{
		m_aGotKilledMessages.AddToTail( "KILLED BY A PLAYER {PLAYERNAME} USING {WEAPONNAME}" );
		m_aKilledPlayerMessages.AddToTail( "KILLED A PLAYER {PLAYERNAME} WITH {WEAPONNAME}" );
		m_aGotWeaponMessages.AddToTail( "GOT A NEW WEAPON {WEAPONNAME}" );
		m_aJoinedServerMessages.AddToTail( "HELLO GUYS!" );
	}
	
	pKV->deleteThis();
}

void CTFBotDeathmatchReaction::BOT_Say( CTFPlayer *pPlayer, const char *szMessage )
{
	if( !pPlayer )
	{
		return;
	}
	
	char szTemp[256];
	Q_snprintf( szTemp, sizeof( szTemp ), "%s", szMessage );
	
	char *p = CheckChatText( pPlayer, szTemp );

	if ( !p )
		return;
	
	if ( !pPlayer->CanSpeak() )
		return;
	
	// See if the player wants to modify of check the text
	pPlayer->CheckChatText( p, 127 ); // though the buffer szTemp that p points to is 256, 
	// chat text is capped to 127 in CheckChatText above
	
	Assert( strlen( pPlayer->GetPlayerName() ) > 0 );
	
	const char *pszFormat = NULL;
	const char *pszPrefix = NULL;
	const char *pszLocation = NULL;
	if ( g_pGameRules )
	{
		pszFormat = g_pGameRules->GetChatFormat( false, pPlayer );
		pszPrefix = g_pGameRules->GetChatPrefix( false, pPlayer );	
		pszLocation = g_pGameRules->GetChatLocation( false, pPlayer );
	}

	const char *pszPlayerName = pPlayer ? pPlayer->GetPlayerName() : "Console";

	char text[256] = { 0 };
	if ( pszPrefix && strlen( pszPrefix ) > 0 )
	{
		if ( pszLocation && strlen( pszLocation ) )
		{
			Q_snprintf( text, sizeof(text), "%s %s @ %s: ", pszPrefix, pszPlayerName, pszLocation );
		}
		else
		{
			Q_snprintf( text, sizeof(text), "%s %s: ", pszPrefix, pszPlayerName );
		}
	}
	else
	{
		Q_snprintf( text, sizeof(text), "%s: ", pszPlayerName );
	}

	int j = sizeof(text) - 2 - strlen(text);  // -2 for /n and null terminator
	if ( (int)strlen(p) > j )
		p[j] = 0;

	Q_strncat( text, p, sizeof( text ), COPY_ALL_CHARACTERS );
	Q_strncat( text, "\n", sizeof( text ), COPY_ALL_CHARACTERS );
 
	// loop through all players
	// Start with the first player.
	// This may return the world in single player if the client types something between levels or during spawn
	// so check it, or it will infinite loop

	CTFPlayer *pClient = NULL;
	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		pClient = ToTFPlayer( UTIL_PlayerByIndex( i ) );
		if ( !pClient || !pClient->edict() )
			continue;
		
		if ( pClient->edict() == pPlayer->edict() )
			continue;

		if ( !(pClient->IsNetClient()) )	// Not a client ? (should never be true)
			continue;

		if ( pPlayer && !pClient->CanHearAndReadChatFrom( pPlayer ) )
			continue;

		if ( pPlayer && GetVoiceGameMgr() && GetVoiceGameMgr()->IsPlayerIgnoringPlayer( pPlayer->entindex(), i ) )
			continue;

		CSingleUserRecipientFilter user( pClient );
		user.MakeReliable();

		if ( pszFormat )
		{
			UTIL_SayText2Filter( user, pPlayer, true, pszFormat, pszPlayerName, p, pszLocation );
		}
		else
		{
			UTIL_SayTextFilter( user, text, pPlayer, true );
		}
	}

	// echo to server console
	// Adrian: Only do this if we're running a dedicated server since we already print to console on the client.
	if ( engine->IsDedicatedServer() )
		 Msg( "%s", text );

	Assert( p );

	IGameEvent * event = gameeventmanager->CreateEvent( "player_say", true );

	if ( event )
	{
		event->SetInt("userid", pPlayer->GetUserID() );
		event->SetString("text", p );
		event->SetInt("priority", 1 );	// HLTV event priority, not transmitted
		gameeventmanager->FireEvent( event, true );
	}
}

CUtlString CTFBotDeathmatchReaction::GetFormatedParabotMessage( CUtlString message, CTFPlayer *player1, CTFPlayer *player2 )
{
	if( !player1 )
	{
		return NULL;
	}
	
	CTFWeaponBase *pWeapon = player1->GetActiveTFWeapon();
	if( pWeapon )
	{
		CUtlString weapon( pWeapon->GetClassname() );
		weapon = weapon.Replace( "tf_weapon_grenade_", "" );
		weapon = weapon.Replace( "tf_weapon_", "" );
		message = message.Replace( "{WEAPONNAME}", weapon.Get() );
	}
	else
	{
		Assert( 0 );
		message = message.Replace( "{WEAPONNAME}", "<no active weapon>" );
	}
	
	if( player2 )
	{
		message = message.Replace( "{PLAYERNAME}", player2->GetPlayerName() );
	}

	return message;
}


// STUB TAUNT
CTFBotTauntDeathmatch::CTFBotTauntDeathmatch( CTFPlayer *pAnotherPlayer )
{
	pAnotherTFPlayer = pAnotherPlayer;
}

ActionResult< CTFBot > CTFBotTauntDeathmatch::OnStart( CTFBot *me, Action< CTFBot > *priorAction )
{
	m_tauntTimer.Start( RandomFloat( 0, 1.0f ) );
	m_didTaunt = false;

	return Continue();
}

ActionResult< CTFBot > CTFBotTauntDeathmatch::Update( CTFBot *me, float interval )
{
	if ( m_tauntTimer.IsElapsed() )
	{
		if ( m_didTaunt )
		{
			return Done( "Taunt finished" );			
		}
		else
		{
			g_TFBotDeathmatchReaction.ReactOnEvent( EVENT_KILLED_PLAYER, me, pAnotherTFPlayer );
			m_didTaunt = true;
		}
	}

	return Continue();
}
