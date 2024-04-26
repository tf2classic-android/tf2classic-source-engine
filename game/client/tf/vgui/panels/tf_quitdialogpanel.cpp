#include "cbase.h"
#include "tf_quitdialogpanel.h"
#include "tf_mainmenu.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFQuitDialogPanel::CTFQuitDialogPanel( vgui::Panel* parent, const char *panelName ) : CTFDialogPanelBase( parent, panelName )
{

}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFQuitDialogPanel::~CTFQuitDialogPanel()
{

}

void CTFQuitDialogPanel::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/UI/main_menu/QuitDialogPanel.res" );
}

void CTFQuitDialogPanel::OnCommand( const char* command )
{
	if ( !V_stricmp( command, "quitconfirm" ) )
	{
		engine->ClientCmd( "quit" );
	}
	else
	{
		BaseClass::OnCommand( command );
	}
}
