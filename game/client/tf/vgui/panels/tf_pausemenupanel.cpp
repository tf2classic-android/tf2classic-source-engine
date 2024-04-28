#include "cbase.h"
#include "tf_pausemenupanel.h"
#include "controls/tf_advbutton.h"
#include "tf_notificationmanager.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFPauseMenuPanel::CTFPauseMenuPanel( vgui::Panel* parent, const char *panelName ) : CTFMenuPanelBase( parent, panelName )
{
	m_pNotificationButton = NULL;
	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFPauseMenuPanel::~CTFPauseMenuPanel()
{

}

void CTFPauseMenuPanel::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/UI/main_menu/PauseMenuPanel.res" );
	m_pNotificationButton = dynamic_cast<CTFButton*>(FindChildByName("NotificationButton"));
}

void CTFPauseMenuPanel::PerformLayout()
{
	BaseClass::PerformLayout();
}

void CTFPauseMenuPanel::OnCommand( const char* command )
{
	if ( !V_stricmp( command, "newquit" ) )
	{
		guiroot->ShowPanel( QUIT_MENU );
	}
	else if ( !V_stricmp( command, "newoptionsdialog" ) )
	{
		guiroot->ShowPanel( OPTIONSDIALOG_MENU );
	}
	else if ( !V_stricmp( command, "newloadout" ) )
	{
		guiroot->ShowPanel( LOADOUT_MENU );
	}
	else if ( !V_stricmp( command, "newcreateserver" ) )
	{
		guiroot->ShowPanel( CREATESERVER_MENU );
	}
	else if ( !V_stricmp( command, "newstats" ) )
	{
		guiroot->ShowPanel( STATSUMMARY_MENU );
	}
	else if ( !V_stricmp( command, "checkversion" ) )
	{
		//MAINMENU_ROOT->CheckVersion();
	}
	else if ( !V_stricmp( command, "shownotification") )
	{
		if( m_pNotificationButton )
		{
			m_pNotificationButton->SetGlowing( false );
		}
		guiroot->ShowPanel( NOTIFICATION_MENU );
	}
	else if ( !V_stricmp( command, "testnotification" ) )
	{
		wchar_t resultString[128];
		V_snwprintf( resultString, sizeof( resultString ), L"test %d", GetNotificationManager()->GetNotificationsCount() );
		MessageNotification Notification( L"Yoyo", resultString, time( NULL ) );
		GetNotificationManager()->SendNotification( Notification );
	}
	else if ( !V_stricmp( command, "callvote" ) )
	{
		engine->ClientCmd( "gameui_hide" );
		engine->ClientCmd( command );
	}
	else if ( V_stristr( command, "gamemenucommand " ) )
	{
		engine->ClientCmd( command );
	}
	else if ( V_stristr( command, "engine " ) )
	{
		const char *engineCMD = strstr( command, "engine " ) + strlen( "engine " );
		if ( strlen( engineCMD ) > 0 )
		{
			engine->ClientCmd_Unrestricted( const_cast<char *>( engineCMD ) );
		}
	}
	else
	{
		BaseClass::OnCommand( command );
	}
}

void CTFPauseMenuPanel::OnNotificationUpdate()
{
	if( m_pNotificationButton )
	{
		if( GetNotificationManager()->GetNotificationsCount() > 0 )
		{
			m_pNotificationButton->SetVisible( true );
		}
		else
		{
			m_pNotificationButton->SetVisible( false );
		}

		if( GetNotificationManager()->GetUnreadNotificationsCount() > 0 )
		{
			m_pNotificationButton->SetGlowing( true );
		}
		else
		{
			m_pNotificationButton->SetGlowing( false );
		}
	}
}

void CTFPauseMenuPanel::Show()
{
	BaseClass::Show();

	RequestFocus();
}

void CTFPauseMenuPanel::Hide()
{
	BaseClass::Hide();
}
