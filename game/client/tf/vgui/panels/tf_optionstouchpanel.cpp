//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"
#include "tf_optionstouchpanel.h"
#include "controls/tf_scriptobject.h"
#include "controls/tf_cvartogglecheckbutton.h"
#include "controls/tf_cvarslider.h"
#include "controls/tf_advpanellistpanel.h"
#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <vgui_controls/TextEntry.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

CTFOptionsTouchPanel::CTFOptionsTouchPanel( vgui::Panel *parent, const char *panelName ) : CTFDialogPanelBase( parent, panelName )
{
	m_pListPanel = new CPanelListPanel( this, "PanelListPanel" );
}

CTFOptionsTouchPanel::~CTFOptionsTouchPanel()
{
}

void CTFOptionsTouchPanel::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	LoadControlSettings("resource/UI/main_menu/OptionsTouch.res");
}

void CTFOptionsTouchPanel::OnCommand(const char* command)
{
	BaseClass::OnCommand(command);
}

void CTFOptionsTouchPanel::CreateControls()
{
	BaseClass::CreateControls();

	pTitleTouch = new Label( this, "DescTextTitle", "Touch options" );

	m_pEnableTouch = new CTFCvarToggleCheckButton( this, "EnableTouch", "Enable touch", "touch_enable" );
	m_pDrawTouch = new CTFCvarToggleCheckButton( this, "DrawTouch", "Draw touch", "touch_draw" );
	m_pReverseTouch = new CTFCvarToggleCheckButton( this, "ReverseTouch", "Reverse touch", "touch_reverse" );
	m_pTouchFilter = new CTFCvarToggleCheckButton( this, "TouchFilter", "Touch filter", "touch_filter" );
	m_pTouchAcceleration = new CTFCvarToggleCheckButton( this, "TouchFilter", "Touch acceleration", "touch_filter" );
	
	m_pTouchSensitivitySlider = new CTFCvarSlider( this, "TouchSensitivity", 0.1f, 6.0f, "touch_sensitivity", true );
	m_pTouchAccelerationSlider = new CTFCvarSlider( this, "TouchAcceleration", 1.0f, 1.5f, "touch_accel", true );
	m_pTouchHorizSensitSlider = new CTFCvarSlider( this, "TouchHorizSensit", 50.f, 300.f, "touch_yaw", true );
	m_pTouchVertSensitSlider = new CTFCvarSlider( this, "TouchVertSensit", 50.f, 300.f, "touch_pitch", true );

	// Register controls
	AddControl( pTitleTouch, O_CATEGORY );

	AddControl( m_pEnableTouch, O_BOOL );
	AddControl( m_pDrawTouch, O_BOOL );
	AddControl( m_pReverseTouch, O_BOOL );
	AddControl( m_pTouchFilter, O_BOOL );
	AddControl( m_pTouchAcceleration, O_BOOL );

	AddControl( m_pTouchSensitivitySlider, O_SLIDER, "Touch sensitivity", "", &m_pTouchSensitivityPreLabel );
	AddControl( m_pTouchAccelerationSlider, O_SLIDER, "Touch acceleration", "", &m_pTouchAccelerationPreLabel );
	AddControl( m_pTouchHorizSensitSlider, O_SLIDER, "Horizontal sensitivity", "", &m_pTouchHorizSensitPreLabel );
	AddControl( m_pTouchVertSensitSlider, O_SLIDER, "Vertical Sensitivity", "", &m_pTouchVertSensitPreLabel );

	UpdatePanels();
}

void CTFOptionsTouchPanel::DestroyControls()
{
	BaseClass::DestroyControls();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsTouchPanel::OnResetData()
{
	BaseClass::OnResetData();

	m_pEnableTouch->Reset();
	m_pDrawTouch->Reset();
	m_pReverseTouch->Reset();
	m_pTouchFilter->Reset();
	m_pTouchAcceleration->Reset();

	m_pTouchSensitivitySlider->Reset();
	m_pTouchAccelerationSlider->Reset();
	m_pTouchHorizSensitSlider->Reset();
	m_pTouchVertSensitSlider->Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsTouchPanel::OnApplyChanges()
{
	BaseClass::OnApplyChanges();

	m_pEnableTouch->ApplyChanges();
	m_pDrawTouch->ApplyChanges();
	m_pReverseTouch->ApplyChanges();
	m_pTouchFilter->ApplyChanges();
	m_pTouchAcceleration->ApplyChanges();

	m_pTouchSensitivitySlider->ApplyChanges();
	m_pTouchAccelerationSlider->ApplyChanges();
	m_pTouchHorizSensitSlider->ApplyChanges();
	m_pTouchVertSensitSlider->ApplyChanges();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsTouchPanel::OnControlModified( void )
{
	PostActionSignal( new KeyValues( "ApplyButtonEnable" ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsTouchPanel::OnCheckButtonChecked( Panel *panel )
{
	if ( IsVisible() )
	{
		if( panel == m_pEnableTouch || panel == m_pTouchAcceleration )
			UpdatePanels();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsTouchPanel::UpdatePanels()
{
	bool bEnabled = m_pEnableTouch->IsChecked();
	m_pDrawTouch->SetVisible( bEnabled );
	m_pReverseTouch->SetVisible( bEnabled );
	m_pTouchFilter->SetVisible( bEnabled );
	m_pTouchAcceleration->SetVisible( bEnabled );

	m_pTouchSensitivitySlider->SetVisible( bEnabled );
	m_pTouchSensitivityPreLabel->SetVisible( bEnabled );
	m_pTouchAccelerationSlider->SetVisible( bEnabled );
	m_pTouchAccelerationPreLabel->SetVisible( bEnabled );
	m_pTouchHorizSensitSlider->SetVisible( bEnabled );
	m_pTouchHorizSensitPreLabel->SetVisible( bEnabled );
	m_pTouchVertSensitSlider->SetVisible( bEnabled );
	m_pTouchVertSensitPreLabel->SetVisible( bEnabled );

	bEnabled = m_pTouchAcceleration->IsChecked();
	m_pTouchAccelerationSlider->SetVisible( bEnabled );
	m_pTouchAccelerationPreLabel->SetVisible( bEnabled );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsTouchPanel::UpdateSensitivityLabel()
{

}
