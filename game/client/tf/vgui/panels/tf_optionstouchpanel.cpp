//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"
#include "tf_optionstouchpanel.h"
#include "tf_mainmenu.h"
#include "controls/tf_scriptobject.h"
#include "controls/tf_cvartogglecheckbutton.h"
#include "controls/tf_cvarslider.h"
#include "controls/tf_advpanellistpanel.h"
#include "controls/tf_advbutton.h"
#include "vgui_controls/ComboBox.h"
#include <KeyValues.h>
#include <vgui/IScheme.h>
#include "tier1/convar.h"
#include <stdio.h>
#include <vgui_controls/TextEntry.h>
// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

CTFOptionsTouchPanel::CTFOptionsTouchPanel(vgui::Panel *parent, const char *panelName) : CTFDialogPanelBase(parent, panelName)
{
	Init();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFOptionsTouchPanel::~CTFOptionsTouchPanel()
{
}

bool CTFOptionsTouchPanel::Init()
{
	BaseClass::Init();

	m_pListPanel = new CPanelListPanel(this, "PanelListPanel");
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: sets background color & border
//-----------------------------------------------------------------------------
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

	m_pTouchSensitivitySlider = new CTFCvarSlider( this, "TouchSensitivity", "Touch sensitivity", 0.1f, 6.0f, "touch_sensitivity", true );
	m_pTouchAccelerationSlider = new CTFCvarSlider( this, "TouchAcceleration", "Touch acceleration", 1.0f, 1.5f, "touch_accel", true );
	m_pTouchHorizSensitSlider = new CTFCvarSlider( this, "TouchHorizSensit", "Horizontal sensitivity", 50.f, 300.f, "touch_yaw", true );
	m_pTouchVertSensitSlider = new CTFCvarSlider( this, "TouchVertSensit", "Vertical sensitivity", 50.f, 300.f, "touch_pitch", true );

	// Register controls
	AddControl( pTitleTouch, O_CATEGORY );

	AddControl( m_pEnableTouch, O_BOOL );
	AddControl( m_pDrawTouch, O_BOOL );
	AddControl( m_pReverseTouch, O_BOOL );
	AddControl( m_pTouchFilter, O_BOOL );
	AddControl( m_pTouchAcceleration, O_BOOL );

	AddControl( m_pTouchSensitivitySlider, O_SLIDER );
	AddControl( m_pTouchAccelerationSlider, O_SLIDER );
	AddControl( m_pTouchHorizSensitSlider, O_SLIDER );
	AddControl( m_pTouchVertSensitSlider, O_SLIDER );

	/*
	pTitleMouse = new Label( this, "DescTextTitle", "Touch options" );

	m_pReverseMouseCheckBox = new CTFCvarToggleCheckButton(this, "ReverseMouse", "#GameUI_ReverseMouse", "m_pitch");
	m_pRawInputCheckBox = new CTFCvarToggleCheckButton(this, "RawInput", "#GameUI_MouseRaw", "m_rawinput");
	m_pMouseFilterCheckBox = new CTFCvarToggleCheckButton(this, "MouseFilter", "#GameUI_MouseFilter", "m_filter");
	m_pMouseSensitivitySlider = new CTFCvarSlider(this, "MouseSensitivity", "#GameUI_MouseSensitivity", 1.0f, 20.0f, "sensitivity", true);

	m_pMouseAccelCheckBox = new CTFCvarToggleCheckButton(this, "MouseAccel", "#GameUI_MouseAcceleration_Hint", "m_customaccel");
	m_pMouseAccelSlider = new CTFCvarSlider(this, "MouseAccelSlider", "#GameUI_MouseAcceleration", 1.0f, 2.0f, "m_customaccel_exponent", true);

	///
	pTitleJoystick = new Label( this, "DescTextTitle", "Joystick options" );

	m_pJoystickCheckBox = new CTFCvarToggleCheckButton(this, "Joystick", "#GameUI_Joystick", "joystick");
	m_pJoystickSouthpawCheckBox = new CTFCvarToggleCheckButton(this, "JoystickSouthpaw", "#GameUI_JoystickSouthpaw", "joy_movement_stick");
	m_pReverseJoystickCheckBox = new CTFCvarToggleCheckButton(this, "ReverseJoystick", "#GameUI_ReverseJoystick", "joy_inverty");

	m_pJoyYawSensitivitySlider = new CTFCvarSlider(this, "JoystickYawSlider", "#GameUI_JoystickLookSpeedYaw", -0.5f, -7.0f, "joy_yawsensitivity", true);
	m_pJoyPitchSensitivitySlider = new CTFCvarSlider(this, "JoystickPitchSlider", "#GameUI_JoystickLookSpeedPitch", 0.5f, 7.0f, "joy_pitchsensitivity", true);

	AddControl(pTitleMouse, O_CATEGORY);
	AddControl(m_pReverseMouseCheckBox, O_BOOL);
	AddControl(m_pRawInputCheckBox, O_BOOL);
	AddControl(m_pMouseFilterCheckBox, O_BOOL);
	AddControl(m_pMouseSensitivitySlider, O_SLIDER);
	AddControl(m_pMouseAccelCheckBox, O_BOOL);
	AddControl(m_pMouseAccelSlider, O_SLIDER);

	AddControl(pTitleJoystick, O_CATEGORY);
	AddControl(m_pJoystickCheckBox, O_BOOL);
	AddControl(m_pJoystickSouthpawCheckBox, O_BOOL);
	AddControl(m_pReverseJoystickCheckBox, O_BOOL);
	AddControl(m_pJoyYawSensitivitySlider, O_SLIDER);
	AddControl(m_pJoyPitchSensitivitySlider, O_SLIDER);
	*/

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

	/*
	m_pReverseMouseCheckBox->Reset();
	m_pRawInputCheckBox->Reset();
	m_pMouseFilterCheckBox->Reset();
	m_pMouseSensitivitySlider->Reset();
	m_pMouseAccelCheckBox->Reset();
	m_pMouseAccelSlider->Reset();

	m_pJoystickCheckBox->Reset();
	m_pJoystickSouthpawCheckBox->Reset();
	m_pReverseJoystickCheckBox->Reset();
	m_pJoyYawSensitivitySlider->Reset();
	m_pJoyPitchSensitivitySlider->Reset();
	*/
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

	/*
	m_pReverseMouseCheckBox->ApplyChanges();
	m_pRawInputCheckBox->ApplyChanges();
	m_pMouseFilterCheckBox->ApplyChanges();
	m_pMouseSensitivitySlider->ApplyChanges();
	m_pMouseAccelCheckBox->ApplyChanges();
	m_pMouseAccelSlider->ApplyChanges();

	m_pJoystickCheckBox->ApplyChanges();
	m_pJoystickSouthpawCheckBox->ApplyChanges();
	m_pReverseJoystickCheckBox->ApplyChanges();
	m_pJoyYawSensitivitySlider->ApplyChanges();
	m_pJoyPitchSensitivitySlider->ApplyChanges();

	engine->ClientCmd_Unrestricted("joyadvancedupdate");
	*/
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
		//if ( panel == m_pJoystickCheckBox || panel == m_pMouseAccelCheckBox )
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
	m_pTouchAccelerationSlider->SetVisible( bEnabled );
	m_pTouchHorizSensitSlider->SetVisible( bEnabled );
	m_pTouchVertSensitSlider->SetVisible( bEnabled );

	bEnabled = m_pTouchAcceleration->IsChecked();
	m_pTouchAccelerationSlider->SetVisible( bEnabled );

	/*
	bool bEnabled = m_pMouseAccelCheckBox->IsChecked();
	m_pMouseAccelSlider->SetVisible(bEnabled);

	bEnabled = m_pJoystickCheckBox->IsChecked();
	m_pReverseJoystickCheckBox->SetVisible( bEnabled );
	m_pJoystickSouthpawCheckBox->SetVisible( bEnabled );
	m_pJoyYawSensitivitySlider->SetVisible( bEnabled );
	m_pJoyPitchSensitivitySlider->SetVisible( bEnabled );
	*/
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsTouchPanel::UpdateSensitivityLabel()
{

}
