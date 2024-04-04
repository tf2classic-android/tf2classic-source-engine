// tf_bot_taunt_deathmatch.h
// LEAKS INSIDE, GO WATCH YOU DUMDUM!
// SanyaSho, March 2024
// Used for TF2C Android port

#ifndef TF_BOT_TAUNT_DEATHMATCH_H
#define TF_BOT_TAUNT_DEATHMATCH_H

enum ETFBotDeathmatchReactionEvent
{
	EVENT_NONE = 0,
	EVENT_GOT_KILLED,
	EVENT_KILLED_PLAYER,
	EVENT_GOT_WEAPON,
	EVENT_JOINED_SERVER,
	EVENT_LOOK_AT_ME // monkey
};

extern ConVar tf_bot_chat;
extern ConVar tf_bot_chat_chance;

class CTFPlayer;
class CTFBotDeathmatchReaction : public CAutoGameSystem
{
public:
	CTFBotDeathmatchReaction( const char *pszName );

	virtual bool Init( void );
	virtual void LevelInitPreEntity( void );

	void ReactOnEvent( ETFBotDeathmatchReactionEvent ourevent, CTFPlayer *player1, CTFPlayer *player2 = NULL );

	void LoadChatFile();
private:
	void BOT_Say( CTFPlayer *pPlayer, const char *szMessage );
	CUtlString GetFormatedParabotMessage( CUtlString message, CTFPlayer *player1, CTFPlayer *player2 = NULL );

	CUtlVector< CUtlString > m_aWeaponReplacements;

	CUtlVector< CUtlString > m_aGotKilledMessages;
	CUtlVector< CUtlString > m_aKilledPlayerMessages;
	CUtlVector< CUtlString > m_aGotWeaponMessages;
	CUtlVector< CUtlString > m_aJoinedServerMessages;
	CUtlVector< CUtlString > m_aLookAtMeMessages;
};

class CTFBotTauntDeathmatch : public Action< CTFBot >
{
public:
	CTFBotTauntDeathmatch( ETFBotDeathmatchReactionEvent nEvent = EVENT_KILLED_PLAYER, CTFPlayer *pAnotherPlayer = NULL );

	virtual ActionResult< CTFBot > OnStart( CTFBot *me, Action< CTFBot > *priorAction );
	virtual ActionResult< CTFBot > Update( CTFBot *me, float interval );

	virtual const char *GetName( void ) const { return "Deathmatch Taunt"; };

private:
	CountdownTimer m_tauntTimer;
	bool m_didTaunt;
	
	CTFPlayer *pAnotherTFPlayer;
	ETFBotDeathmatchReactionEvent nOurEvent;
};

extern CTFBotDeathmatchReaction g_TFBotDeathmatchReaction;

#endif // TF_BOT_TAUNT_DEATHMATCH_H
