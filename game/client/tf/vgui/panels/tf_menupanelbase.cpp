#include "cbase.h"
#include "tf_menupanelbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFMenuPanelBase::CTFMenuPanelBase( vgui::Panel *parent, const char *panelName ) : EditablePanel( parent, panelName, scheme()->GetScheme( "ClientScheme_tf2c" ) )
{
	SetProportional( true );
	SetVisible( true );

	int x, y, width, height;
	//surface()->GetScreenSize(width, height);
	GetParent()->GetBounds( x, y, width, height );
	SetBounds( 0, 0, width, height );
	m_bShowSingle = false;
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFMenuPanelBase::~CTFMenuPanelBase()
{
}

void CTFMenuPanelBase::SetShowSingle( bool ShowSingle )
{
	m_bShowSingle = ShowSingle;
}

void CTFMenuPanelBase::Show()
{
	SetVisible( true );
}

void CTFMenuPanelBase::Hide()
{
	SetVisible( false );
}
