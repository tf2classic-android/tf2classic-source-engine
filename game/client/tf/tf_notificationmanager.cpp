#if defined( POSIX )
#include <sys/utsname.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

#include <inttypes.h> // for PRId64
#include <curl/curl.h>

#include "cbase.h"
#include "tf_notificationmanager.h"
#include "tf_mainmenu.h"
#include "filesystem.h"
#include "script_parser.h"
#include "tf_gamerules.h"
#include "tf_hud_notification_panel.h"
#include "tier3/tier3.h"
#include "vgui/ISurface.h"
#include "vgui/ILocalize.h"
#include "fmtstr.h"



// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//#define USE_LOCAL_WEBSERVER

#define TF_CLASSIC_VERSION "2.0.0b4+"

const char *g_aRequestURLs[REQUEST_COUNT] =
{
#if 0 // legacy code(2016)
	"http://services.0x13.io/tf2c/version/?latest=1",
	"http://services.0x13.io/tf2c/motd/",
	"http://services.0x13.io/tf2c/servers/official/" // was removed with in-game official servers list
#elif 0 // new code (2017)
	"http://dev.tf2c.services.0x13.io/version.vdf",
	"http://dev.tf2c.services.0x13.io/notifications.vdf",
	"http://dev.tf2c.services.0x13.io/serverlist/official.vdf" // was removed with in-game official servers list
#else
#if defined( USE_LOCAL_WEBSERVER )
	"http://127.0.0.1/tf2c/version.vdf",
	"http://127.0.0.1/tf2c/notifications.vdf"
#else
	"http://147.78.67.219/tf2c/version.vdf",
	"http://147.78.67.219/tf2c/notifications.vdf"
#endif
#endif
};

MessageNotification::MessageNotification()
{
	timeStamp = 0;
	wszTitle[0] = '\0';
	wszMessage[0] = '\0';
	wszDate[0] = '\0';
	bUnread = true;
	bLocal = false;
};

MessageNotification::MessageNotification( const char *Title, const char *Message, time_t timeVal )
{
	g_pVGuiLocalize->ConvertANSIToUnicode( Title, wszTitle, sizeof( wszTitle ) );
	g_pVGuiLocalize->ConvertANSIToUnicode( Message, wszMessage, sizeof( wszMessage ) );
	bUnread = true;
	bLocal = false;
	SetTimeStamp( timeVal );
};

MessageNotification::MessageNotification( const wchar_t *Title, const wchar_t *Message, time_t timeVal )
{
	V_wcsncpy( wszTitle, Title, sizeof( wszTitle ) );
	V_wcsncpy( wszMessage, Message, sizeof( wszMessage ) );
	bUnread = true;
	bLocal = false;
	SetTimeStamp( timeVal );
};

void MessageNotification::SetTimeStamp( time_t timeVal )
{
	timeStamp = timeVal;

	char szDate[64];
	BGetLocalFormattedDate( timeStamp, szDate, sizeof( szDate ) );

	g_pVGuiLocalize->ConvertANSIToUnicode( szDate, wszDate, sizeof( wszDate ) );
}

static CTFNotificationManager g_TFNotificationManager;
CTFNotificationManager *GetNotificationManager()
{
	return &g_TFNotificationManager;
}

#if defined( PUBLIC_BUILD ) // hide funny cvars and commands from public builds
#define TF2C_CHECKCVARS_FLAGS FCVAR_DEVELOPMENTONLY
#define PrintIfDev( x )
#else
#define TF2C_CHECKCVARS_FLAGS FCVAR_NONE
#define PrintIfDev( x )  x
#endif

CON_COMMAND_F( tf2c_checkmessages, "Check for the messages", TF2C_CHECKCVARS_FLAGS )
{
	GetNotificationManager()->CheckVersionAndMessages();
}

ConVar tf2c_checkfrequency( "tf2c_checkfrequency", "900", TF2C_CHECKCVARS_FLAGS, "Messages check frequency (seconds)" );
ConVar tf2c_updatefrequency( "tf2c_updatefrequency", "15", TF2C_CHECKCVARS_FLAGS, "Updatelist update frequency (seconds)" );
ConVar tf2c_notification_useragent( "tf2c_notification_useragent", "Steam/3.0", TF2C_CHECKCVARS_FLAGS, "Set cURL user agent string" );
ConVar tf2c_latest_notification( "tf2c_latest_notification", "0", FCVAR_ARCHIVE );

//-----------------------------------------------------------------------------
// Purpose: constructor
//-----------------------------------------------------------------------------
CTFNotificationManager::CTFNotificationManager() : CAutoGameSystemPerFrame( "CTFNotificationManager" )
{
	curl_global_init( CURL_GLOBAL_ALL );

	m_Notifications.RemoveAll();
	m_bOutdated = false;
	m_bPlayedSound = false;
	m_flLastCheck = -1.f;
	m_response_data.Clear();

	Q_snprintf( m_szVersionName, sizeof( m_szVersionName ), TF_CLASSIC_VERSION );
	m_VersionTime = 0;
}

CTFNotificationManager::~CTFNotificationManager()
{
}

//-----------------------------------------------------------------------------
// Purpose: Initializer
//-----------------------------------------------------------------------------
bool CTFNotificationManager::Init()
{
	ParseVersionFile();
	return true;
}

void CTFNotificationManager::Update( float frametime )
{
	if ( !guiroot )
		return;

	float flCurTime = engine->Time(); // more accurate than gpGlobals->curtime
	if( (m_flLastCheck < 0.f) || (flCurTime - m_flLastCheck) > tf2c_checkfrequency.GetFloat() ) 
	{
		m_flLastCheck = flCurTime;
		CheckVersionAndMessages();
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFNotificationManager::CheckVersionAndMessages( void )
{
	AddRequest( REQUEST_VERSION );
	AddRequest( REQUEST_MESSAGE );
	m_bPlayedSound = false;
}

//-----------------------------------------------------------------------------
// Purpose: Event handler
//-----------------------------------------------------------------------------
void CTFNotificationManager::FireGameEvent( IGameEvent *event )
{
}

void CTFNotificationManager::AddRequest( RequestType type )
{
	CURL *curl_handle = curl_easy_init();
	if( !curl_handle )
		return;

	struct curl_slist *hs = NULL;
	hs = curl_slist_append( hs, "Content-Type: text/plain" );
	
	char username[256];
	Q_memset( username, 0, sizeof( username ) );

#if defined( POSIX )
#if !defined( ANDROID )
	if( getlogin_r( username, sizeof( username ) ) != 0 )
#endif
	{
		Q_snprintf( username, sizeof( username ), "empty" );
	}
	
	struct utsname buffer;
	if( uname( &buffer ) != 0 )
	{
		V_sprintf_safe( buffer.sysname, "empty" );
		V_sprintf_safe( buffer.nodename, "empty" );
		V_sprintf_safe( buffer.release, "empty" );
		V_sprintf_safe( buffer.version, "empty" );
		V_sprintf_safe( buffer.machine, "empty" );
	}
#else
	DWORD length = sizeof( username ) - 1;
	if ( !GetUserName( username, &length ) )
	{
		Q_snprintf( username, sizeof( username ), "empty" );
	}

	char sysname[256] = { 0 }, nodename[256] = { 0 }, release[256] = { 0 }, version[256] = { 0 }, machine[256] = { 0 };
	
	V_sprintf_safe( sysname, "Windows" );

	if( gethostname( nodename, sizeof( nodename ) ) != 0 )
	{
		V_sprintf_safe( nodename, "empty" );
	}
	
	OSVERSIONINFOEX osvi;
	ZeroMemory( &osvi, sizeof( OSVERSIONINFOEX ) );
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );
	
	if( GetVersionEx( (OSVERSIONINFO*)&osvi ) )
	{
		V_sprintf_safe( release, "%u.%u", osvi.dwMajorVersion, osvi.dwMinorVersion );
		
		V_sprintf_safe( version, (osvi.wProductType == VER_NT_WORKSTATION) ? "workstation" : "server" );

		SYSTEM_INFO si;
		ZeroMemory( &si, sizeof( si ) );
		GetNativeSystemInfo( &si );
		switch( si.wProcessorArchitecture )
		{
			case 9: //PROCESSOR_ARCHITECTURE_AMD64:
				V_sprintf_safe( machine, "amd64" );
				break;
			case 5: //PROCESSOR_ARCHITECTURE_ARM:
				V_sprintf_safe( machine, "arm" );
				break;
			case 12: //PROCESSOR_ARCHITECTURE_ARM64:
				V_sprintf_safe( machine, "arm64" );
				break;
			case 6: //PROCESSOR_ARCHITECTURE_IA64:
				V_sprintf_safe( machine, "ia64" );
				break;
			case 0: //PROCESSOR_ARCHITECTURE_INTEL:
				V_sprintf_safe( machine,"intel" );
				break;
			default:
				V_sprintf_safe( machine, "unknown" );
		};
	}
#endif

	const char *pszSource = g_aRequestURLs[type];

	curl_easy_setopt( curl_handle, CURLOPT_URL, pszSource );
#if defined( POSIX )
	curl_easy_setopt( curl_handle, CURLOPT_USERAGENT, CFmtStr( "%s %s/%s/%s/%s/%s/%s", tf2c_notification_useragent.GetString(), username, buffer.sysname, buffer.nodename, buffer.release, buffer.version, buffer.machine ).Get() );
#else
	curl_easy_setopt( curl_handle, CURLOPT_USERAGENT, CFmtStr( "%s %s/%s/%s/%s/%s/%s", tf2c_notification_useragent.GetString(), username, sysname, nodename, release, version, machine ).Get() );
#endif
	curl_easy_setopt( curl_handle, CURLOPT_FOLLOWLOCATION, 1L );
	curl_easy_setopt( curl_handle, CURLOPT_WRITEFUNCTION, &CTFNotificationManager::WriteCallback );
	curl_easy_setopt( curl_handle, CURLOPT_WRITEDATA, this );
	curl_easy_setopt( curl_handle, CURLOPT_HTTPHEADER, hs );

	CURLcode res = curl_easy_perform( curl_handle );

	if( res == CURLE_OK )
	{
		long response_code;
		curl_easy_getinfo( curl_handle, CURLINFO_RESPONSE_CODE, &response_code );

		if( response_code == 200 )
		{
			switch( type )
			{
				case REQUEST_VERSION:
					OnVersionCheckCompleted( m_response_data, pszSource );
					break;
				case REQUEST_MESSAGE:
					OnMessageCheckCompleted( m_response_data, pszSource );
					break;
				default:
					break;
			}

			m_response_data.Clear();
		}
	}

	curl_easy_cleanup( curl_handle );
}

size_t CTFNotificationManager::WriteCallback( void *contents, size_t size, size_t nmemb, void *userp )
{
	size_t realsize = size * nmemb;
	CTFNotificationManager *manager = static_cast< CTFNotificationManager * >( userp );
	manager->m_response_data.Append( static_cast<char*>(contents) );
	manager->m_response_data.SetLength( realsize );
	return realsize;
}

void CTFNotificationManager::OnVersionCheckCompleted( const char* pMessage, const char *pszSource )
{
	if ( pMessage[0] == '\0' )
		return;

	PrintIfDev( Msg( "OnVersionCheckCompleted( %s, %s )\n", pMessage, pszSource ); )

	//"tf2classic_version"
	//{
	//	"time" "0"
	//	"name" "2.0.0b4+"
	//}
	if( !Q_strncmp( pMessage, "\"tf2classic_version\"", strlen( "\"tf2classic_version\"" ) ) )
	{
		KeyValues *pMessages = new KeyValues( "tf2classic_version" );
		if( !pMessages )
			return;

		pMessages->LoadFromBuffer( pszSource, pMessage );

		time_t timeCurrent = GetVersionTimeStamp();
		time_t timeLatest = V_atoi64( pMessages->GetString( "time" ) );

		if( timeCurrent < timeLatest )
		{
			if ( m_bOutdated )
				return;

			m_bOutdated = true;

			MessageNotification Notification;
			const wchar_t *pszLocalizedTitle = g_pVGuiLocalize->Find( "#TF_GameOutdatedTitle" );
			if ( pszLocalizedTitle )
			{
				V_wcsncpy( Notification.wszTitle, pszLocalizedTitle, sizeof( Notification.wszTitle ) );
			}
			else
			{
				V_wcsncpy( Notification.wszTitle, L"#TF_GameOutdatedTitle", sizeof( Notification.wszTitle ) );
			}

			wchar_t wszVersion[16];
			g_pVGuiLocalize->ConvertANSIToUnicode( pMessages->GetString( "name" ), wszVersion, sizeof( wszVersion ) );

			char szDate[64];
			wchar_t wszDate[64];
			BGetLocalFormattedDate( timeLatest, szDate, sizeof( szDate ) );
			g_pVGuiLocalize->ConvertANSIToUnicode( szDate, wszDate, sizeof( wszDate ) );

			g_pVGuiLocalize->ConstructString( Notification.wszMessage, sizeof( Notification.wszMessage ), g_pVGuiLocalize->Find( "#TF_GameOutdated" ), 2, wszVersion, wszDate );

			// Urgent - set time to now.
			Notification.SetTimeStamp( time( NULL ) );
			Notification.bLocal = true;

			SendNotification( Notification );
		}
		else
		{
			m_bOutdated = false;
		}

		pMessages->deleteThis();
	}
	else
	{
		Warning( "Incorrect version message format.\n" );
	}
}

void CTFNotificationManager::OnMessageCheckCompleted( const char *pszPage, const char *pszSource )
{
	if ( pszPage[0] == '\0' )
		return;

	PrintIfDev( Msg( "OnMessageCheckCompleted( %s, %s )\n", pszPage, pszSource ); )

	//"tf2classic_messages"
	//{
	//	"0" // unix timestamp
	//	{
	//		"title" "msgtitle"
	//		"message" "msg"
	//	}
	//}
	if( !Q_strncmp( pszPage, "\"tf2classic_messages\"", strlen( "\"tf2classic_messages\"" ) ) )
	{
		KeyValues *pMessages = new KeyValues( "tf2classic_messages" );
		if( !pMessages )
			return;

		pMessages->UsesEscapeSequences( true );
		pMessages->LoadFromBuffer( pszSource, pszPage );
		
		for( KeyValues *pData = pMessages->GetFirstSubKey(); pData != NULL; pData = pData->GetNextKey() )
		{
			const char *pszName = pData->GetName();
			time_t timePrevious = V_atoi64( tf2c_latest_notification.GetString() );
			time_t timeNew = V_atoi64( pszName );

			if( timeNew > timePrevious )
			{
				tf2c_latest_notification.SetValue( pszName );

				const char *pszTitle = pData->GetString( "title" );
				const char *pszMessage = pData->GetString( "message" );

				MessageNotification Notification( pszTitle, pszMessage, timeNew );
				SendNotification( Notification );

				// this code was removed in 2017, but i think we keep it here.
				C_TFPlayer *pLocalPlayer = C_TFPlayer::GetLocalTFPlayer();
				if ( pLocalPlayer )
				{
					CHudNotificationPanel *pNotifyPanel = GET_HUDELEMENT( CHudNotificationPanel );
					if ( pNotifyPanel )
					{
						pNotifyPanel->SetupNotifyCustom( pszMessage, "ico_notify_flag_moving", pLocalPlayer->GetTeamNumber() );
					}
				}
			}
		}

		pMessages->deleteThis();
	}
	else
	{
		Warning( "Incorrect notification message format.\n" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Sort notifications by timestamp (latest first)
//-----------------------------------------------------------------------------
int NotificationsSort( const MessageNotification *pItem1, const MessageNotification *pItem2 )
{
	return ( pItem2->timeStamp - pItem1->timeStamp );
}

void CTFNotificationManager::SendNotification( MessageNotification &pMessage )
{
	m_Notifications.AddToTail( pMessage );
	m_Notifications.Sort( NotificationsSort );

	guiroot->OnNotificationUpdate();

	// Only play sound once per notification.
	if ( !m_bPlayedSound )
	{
		surface()->PlaySound( "ui/notification_alert.wav" );
		m_bPlayedSound = true;
	}
}

void CTFNotificationManager::RemoveNotification( int iIndex )
{
	m_Notifications.Remove( iIndex );
	m_Notifications.Sort( NotificationsSort );

	guiroot->OnNotificationUpdate();
};

int CTFNotificationManager::GetUnreadNotificationsCount()
{
	int iCount = 0;
	for ( int i = 0; i < m_Notifications.Count(); i++ )
	{
		if ( m_Notifications[i].bUnread )
			iCount++;
	}
	return iCount;
};

#define VERSION_NAME_KEY "VersionName="
#define VERSION_TIME_KEY "VersionTime="

void CTFNotificationManager::ParseVersionFile()
{
	if ( filesystem->FileExists( "version.txt" ) )
	{
		static int iNameKeyLen = V_strlen( VERSION_NAME_KEY );
		static int iTimeKeyLen = V_strlen( VERSION_TIME_KEY );

		FileHandle_t hVerFile = filesystem->Open( "version.txt", "r", "MOD" );
		if( !hVerFile )
			return;
		size_t fileSize = filesystem->Size( hVerFile );
		char *pMem = new char[fileSize];
		filesystem->Read( pMem, fileSize, hVerFile );
		pMem[fileSize - 1] = 0;

		char szToken[256];
		const char *pFile = engine->ParseFile( pMem, szToken, sizeof( szToken ) );
		while ( szToken[0] != '\0' )
		{
			if ( V_strnicmp( szToken, VERSION_NAME_KEY, iNameKeyLen ) == 0 )
			{
				V_strncpy( m_szVersionName, szToken + iNameKeyLen, sizeof( m_szVersionName ) );
				Msg( "[ParseVersionFile] Found version: %s\n", m_szVersionName );
			}
			else if ( V_strnicmp( szToken, VERSION_TIME_KEY, iTimeKeyLen ) == 0 )
			{
				m_VersionTime = V_atoi64( szToken + iTimeKeyLen );
				Msg( "[ParseVersionFile] Found time: %" PRId64 "\n", m_VersionTime );
			}

			pFile = engine->ParseFile( pFile, szToken, sizeof( szToken ) );
		}
	}
	else
	{
		Warning( "[ParseVersionFile] version.txt is missing!\n" );
	}
}
