#ifndef TF_NOTIFICATIONMANAGER_H
#define TF_NOTIFICATIONMANAGER_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_shareddefs.h"
#include "igamesystem.h"
#include "GameEventListener.h"
#include "time.h"
#include "git_info.h"

#define TF_NOTIFICATION_TITLE_SIZE 64
#define TF_NOTIFICATION_MESSAGE_SIZE 256

class CTFNotificationManager;

enum RequestType
{
	REQUEST_VERSION = 0,
	REQUEST_MESSAGE,

	REQUEST_COUNT
};

struct MessageNotification
{
	MessageNotification();
	MessageNotification( const char *Title, const char *Message, time_t timeVal );
	MessageNotification( const wchar_t *Title, const wchar_t *Message, time_t timeVal );

	void SetTimeStamp( time_t timeVal );

	time_t timeStamp;
	wchar_t wszTitle[TF_NOTIFICATION_TITLE_SIZE];
	wchar_t wszDate[32];
	wchar_t wszMessage[TF_NOTIFICATION_MESSAGE_SIZE];
	bool bUnread;
	bool bLocal;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFNotificationManager : public CAutoGameSystemPerFrame, public CGameEventListener
{
public:
	CTFNotificationManager();
	~CTFNotificationManager();

	// Methods of IGameSystem
	virtual bool Init();
	virtual char const *Name() { return "CTFNotificationManager"; }
	// Gets called each frame
	virtual void Update( float frametime );

	void CheckVersionAndMessages( void );

	// Methods of CGameEventListener
	virtual void FireGameEvent( IGameEvent *event );

	virtual void AddRequest( RequestType type );
	virtual void SendNotification( MessageNotification &pMessage );
	virtual MessageNotification *GetNotification( int iIndex ) { return &m_Notifications[iIndex]; };
	virtual int GetNotificationsCount() { return m_Notifications.Count(); };
	virtual int GetUnreadNotificationsCount();
	virtual void RemoveNotification( int iIndex );
	virtual bool IsOutdated() { return m_bOutdated; };
	
	const char *GetVersionName( void ) { return m_szVersionName; }
	time_t GetVersionTimeStamp( void ) { return m_VersionTime; }
	const char *GetVersionCommit( void )
	{
#if defined( GIT_COMMIT_HASH )
		return GIT_COMMIT_HASH;
#else
		return "undefined";
#endif
	}
	
	void ParseVersionFile();

private:
	CUtlVector<MessageNotification>	m_Notifications;

	bool				m_bOutdated;
	bool				m_bPlayedSound;

	float				m_flLastCheck;

	void				OnMessageCheckCompleted( const char* pMessage, const char *pszSource );
	void				OnVersionCheckCompleted( const char* pMessage, const char *pszSource );

	static size_t			WriteCallback( void *contents, size_t size, size_t nmemb, void *userp );
	CUtlString m_response_data;
	
	time_t m_VersionTime;
	char m_szVersionName[32];
};

CTFNotificationManager *GetNotificationManager();
#endif // TF_NOTIFICATIONMANAGER_H
