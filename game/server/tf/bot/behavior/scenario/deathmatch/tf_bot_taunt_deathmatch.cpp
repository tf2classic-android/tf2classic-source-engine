// tf_bot_taunt_deathmatch.cpp
// LEAKS INSIDE, GO WATCH YOU DUMDUM!
// SanyaSho, March 2024
// Used for TF2C Android port

#include "cbase.h"
#include "bot/tf_bot.h"
#include "bot/behavior/scenario/deathmatch/tf_bot_taunt_deathmatch.h"

#include "voice_gamemgr.h"

CON_COMMAND_F( tf_bot_reload_chat, "", FCVAR_CHEAT )
{
	g_TFBotDeathmatchReaction.LoadChatFile();
}

ConVar tf_bot_chat( "tf_bot_chat", "1", FCVAR_GAMEDLL, "If nonzero, TFBots will say something about your mother" );
ConVar tf_bot_chat_use_map_chat( "tf_bot_chat_use_map_chat", "0", FCVAR_GAMEDLL, "If nonzero, TFBots will use messages from maps/yourmapname_bot_chat.txt instaed of scripts/tf_bot_chat.txt" );
ConVar tf_bot_chat_chance( "tf_bot_chat_chance", "4.5", FCVAR_GAMEDLL, "Bot chat flooder chance (from 0% to 100%)", true, 0.f, true, 100.f );

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
	if( !tf_bot_chat.GetBool() )
	{
		return;
	}

	// SanyaSho: LATER
	//if( ( TFGameRules() && TFGameRules()->IsInfectionMode() ) && ( player1 && player1->GetTeamNumber() == TF_TEAM_BLUE ) )
	//{
	//	return;
	//}

	switch( ourevent )
	{
	case EVENT_GOT_KILLED:
		// player2 - our killer
		if( m_aGotKilledMessages.IsEmpty() )
			break;

		BOT_Say( player1, GetFormatedParabotMessage( m_aGotKilledMessages.Random(), player2, player2 ).Get() );
		break;
	case EVENT_KILLED_PLAYER:
		// player2 - our victim
		if( m_aKilledPlayerMessages.IsEmpty() )
			break;

		BOT_Say( player1, GetFormatedParabotMessage( m_aKilledPlayerMessages.Random(), player1, player2 ).Get() );
		break;
	case EVENT_GOT_WEAPON: // should be called when we've pickup a weapon_rpg, weapon_gauss, weapon_egon, weapon_crossbow
		// player2 - NULL
		if( m_aGotWeaponMessages.IsEmpty() )
			break;

		BOT_Say( player1, GetFormatedParabotMessage( m_aGotWeaponMessages.Random(), player1, player1 ).Get() );
		break;
	case EVENT_JOINED_SERVER:
		// player2 - NULL
		if( m_aJoinedServerMessages.IsEmpty() )
			break;

		BOT_Say( player1, GetFormatedParabotMessage( m_aJoinedServerMessages.Random(), player1, player1 ).Get() );
		break;
	case EVENT_LOOK_AT_ME:
		// player 2 - watcher
		if( m_aLookAtMeMessages.IsEmpty() )
			break;

		BOT_Say( player1, GetFormatedParabotMessage( m_aLookAtMeMessages.Random(), player2, player2 ).Get() );
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
	
	m_aWeaponReplacements.RemoveAll();

	m_aGotKilledMessages.RemoveAll();
	m_aKilledPlayerMessages.RemoveAll();
	m_aGotWeaponMessages.RemoveAll();
	m_aJoinedServerMessages.RemoveAll();
	m_aLookAtMeMessages.RemoveAll();
	
	const char *pszChatFile = "scripts/tf_bot_chat.txt";
	const char *pszMapChatFile = UTIL_VarArgs( "maps/%s_bot_chat.txt", gpGlobals->mapname );
	if( tf_bot_chat_use_map_chat.GetBool() && filesystem->FileExists( pszMapChatFile, "MOD" ) )
	{
		pszChatFile = pszMapChatFile;
	}

	if( pKV->LoadFromFile( filesystem, pszChatFile, "MOD" ) )
	{
		for( KeyValues *pData = pKV->GetFirstSubKey(); pData != NULL; pData = pData->GetNextKey() )
		{
			KeyValues *pWeaponReplacements = pKV->FindKey( "weapons" );
			if( pWeaponReplacements )
			{
				// aaa
				for( KeyValues *pData = pWeaponReplacements->GetFirstSubKey(); pData != NULL; pData = pData->GetNextKey() )
				{
					// aaaaaaa
					for( int i = 0; i < TF_WEAPON_COUNT; i++ )
					{
						// aaaaaaaaa
						if( !Q_stricmp( pData->GetName(), UTIL_VarArgs( "%d", i ) ) )
						{
							m_aWeaponReplacements.AddToTail( CUtlString( pData->GetString() ) );
						}
					}
				}
			}

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
			else if( !Q_stricmp( pData->GetName(), "LOOK_AT_ME" ) )
			{
				//Msg( "[LOOK_AT_ME] %s\n", pData->GetString() );
				m_aLookAtMeMessages.AddToTail( CUtlString( pData->GetString() ) );
			}
		}
	}
	else
	{
		for( int i = 0; i < TF_WEAPON_COUNT; i++ )
		{
			m_aWeaponReplacements.AddToTail( UTIL_VarArgs( "<WEAPON %d>", i ) );
		}

		m_aGotKilledMessages.AddToTail( "I WAS KILLED BY A PLAYER {PLAYERNAME} USING {WEAPONNAME}" );
		m_aKilledPlayerMessages.AddToTail( "I KILLED A PLAYER {PLAYERNAME} WITH {WEAPONNAME}" );
		m_aGotWeaponMessages.AddToTail( "I GOT A NEW WEAPON {WEAPONNAME}" );
		m_aJoinedServerMessages.AddToTail( "HELLO GUYS!" );
		m_aLookAtMeMessages.AddToTail( "HEY, {PLAYERNAME}! DON'T LOOK AT ME, YOU DUMDUM!" );
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

		if ( !pPlayer )
			continue;
		
		if ( pClient->edict() == pPlayer->edict() )
			continue;

		if ( !pClient->IsNetClient() )	// Not a client ? (should never be true)
			continue;

		if ( !pClient->CanHearAndReadChatFrom( pPlayer ) )
			continue;

		if ( GetVoiceGameMgr() && GetVoiceGameMgr()->IsPlayerIgnoringPlayer( pPlayer->entindex(), i ) )
			continue;

		CSingleUserRecipientFilter filter( pClient );
		filter.MakeReliable();

		if ( pszFormat )
		{
			UTIL_SayText2Filter( filter, pPlayer, true, pszFormat, pszPlayerName, p, pszLocation );
		}
		else
		{
			UTIL_SayTextFilter( filter, text, pPlayer, true );
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
		CUtlString weaponReplacement = m_aWeaponReplacements[pWeapon->GetWeaponID()];
		if( !weaponReplacement.IsEmpty() )
		{
			message = message.Replace( "{WEAPONNAME}", weaponReplacement.Get() );
		}
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
CTFBotTauntDeathmatch::CTFBotTauntDeathmatch( ETFBotDeathmatchReactionEvent nEvent, CTFPlayer *pAnotherPlayer )
{
	pAnotherTFPlayer = pAnotherPlayer;
	nOurEvent = nEvent;
}

ActionResult< CTFBot > CTFBotTauntDeathmatch::OnStart( CTFBot *me, Action< CTFBot > *priorAction )
{
	m_tauntTimer.Start( RandomFloat( 0.f, 1.f ) );
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
			g_TFBotDeathmatchReaction.ReactOnEvent( nOurEvent, me, pAnotherTFPlayer );
			m_didTaunt = true;
		}
	}

	return Continue();
}
