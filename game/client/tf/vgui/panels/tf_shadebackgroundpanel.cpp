#include "cbase.h"
#include "tf_shadebackgroundpanel.h"
#include "tf_mainmenu.h"
#include <vgui_controls/AnimationController.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFShadeBackgroundPanel::CTFShadeBackgroundPanel( Panel *parent, const char *panelName ) : CTFMenuPanelBase( parent, panelName )
{
	m_pShadedBG = new ImagePanel( this, "ShadedBG" );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFShadeBackgroundPanel::~CTFShadeBackgroundPanel()
{
}

void CTFShadeBackgroundPanel::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/UI/main_menu/ShadeBackgroundPanel.res" );
}

void CTFShadeBackgroundPanel::Show()
{
	BaseClass::Show();
	GetAnimationController()->RunAnimationCommand( m_pShadedBG, "Alpha", 220, 0.0f, 0.3f, AnimationController::INTERPOLATOR_LINEAR );
}

void CTFShadeBackgroundPanel::Hide()
{
	BaseClass::Hide();
	GetAnimationController()->RunAnimationCommand( m_pShadedBG, "Alpha", 0, 0.0f, 0.1f, AnimationController::INTERPOLATOR_LINEAR );
}
