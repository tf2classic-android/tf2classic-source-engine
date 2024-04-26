//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"
#include "tf_optionsmousepanel.h"
#include "tf_mainmenu.h"
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

CTFOptionsMousePanel::CTFOptionsMousePanel( vgui::Panel *parent, const char *panelName ) : CTFDialogPanelBase( parent, panelName )
{
	m_pListPanel = new CPanelListPanel( this, "PanelListPanel" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFOptionsMousePanel::~CTFOptionsMousePanel()
{
}

//-----------------------------------------------------------------------------
// Purpose: sets background color & border
//-----------------------------------------------------------------------------
void CTFOptionsMousePanel::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/UI/main_menu/OptionsMouse.res" );
}

void CTFOptionsMousePanel::OnCommand( const char* command )
{
	BaseClass::OnCommand( command );
}

void CTFOptionsMousePanel::CreateControls()
{
	BaseClass::CreateControls();

	pTitleMouse = new Label( this, "DescTextTitle", "Mouse options" );

	m_pReverseMouseCheckBox = new CTFCvarToggleCheckButton( this, "ReverseMouse", "#GameUI_ReverseMouse", "m_pitch" );

	m_pRawInputCheckBox = new CTFCvarToggleCheckButton( this, "RawInput", "#GameUI_MouseRaw", "m_rawinput" );
	m_pMouseFilterCheckBox = new CTFCvarToggleCheckButton( this, "MouseFilter", "#GameUI_MouseFilter", "m_filter" );
	m_pMouseSensitivitySlider = new CTFCvarSlider( this, "MouseSensitivity", 1.0f, 20.0f, "sensitivity", true );

	m_pMouseAccelCheckBox = new CTFCvarToggleCheckButton( this, "MouseAccel", "#GameUI_MouseAcceleration", "m_customaccel" );
	m_pMouseAccelSlider = new CTFCvarSlider( this, "MouseAccelSlider", 1.0f, 2.0f, "m_customaccel_exponent", true );

	///
	pTitleJoystick = new Label( this, "DescTextTitle", "Joystick options" );

	m_pJoystickCheckBox = new CTFCvarToggleCheckButton( this, "Joystick", "#GameUI_Joystick", "joystick" );
	m_pJoystickCheckBox->SetToolTip( "#GameUI_JoystickLabel" );
	m_pJoystickSouthpawCheckBox = new CTFCvarToggleCheckButton( this, "JoystickSouthpaw", "#GameUI_JoystickSouthpaw", "joy_movement_stick" );
	m_pReverseJoystickCheckBox = new CTFCvarToggleCheckButton( this, "ReverseJoystick", "#GameUI_ReverseJoystick", "joy_inverty" );

	m_pJoyYawSensitivitySlider = new CTFCvarSlider( this, "JoystickYawSlider", -0.5f, -7.0f, "joy_yawsensitivity", true );
	m_pJoyPitchSensitivitySlider = new CTFCvarSlider( this, "JoystickPitchSlider", 0.5f, 7.0f, "joy_pitchsensitivity", true );

	AddControl( pTitleMouse, O_CATEGORY );
	AddControl( m_pReverseMouseCheckBox, O_BOOL, "", "#GameUI_ReverseMouseLabel" );
	AddControl( m_pRawInputCheckBox, O_BOOL, "", "#GameUI_MouseRaw_Hint" );
	AddControl( m_pMouseFilterCheckBox, O_BOOL, "", "#GameUI_MouseFilterLabel" );
	AddControl( m_pMouseSensitivitySlider, O_SLIDER, "#GameUI_MouseSensitivity" );
	AddControl( m_pMouseAccelCheckBox, O_BOOL, "", "#GameUI_MouseAcceleration_Hint" );
	AddControl( m_pMouseAccelSlider, O_SLIDER, "", "#GameUI_MouseAccelerationAmount_Hint" );

	AddControl( pTitleJoystick, O_CATEGORY );
	AddControl( m_pJoystickCheckBox, O_BOOL, "", "#GameUI_JoystickLabel" );
	AddControl( m_pJoystickSouthpawCheckBox, O_BOOL );
	AddControl( m_pReverseJoystickCheckBox, O_BOOL );
	AddControl( m_pJoyYawSensitivitySlider, O_SLIDER, "#GameUI_JoystickLookSpeedYaw", "", &m_pJoyYawSensitivityPreLabel );
	AddControl( m_pJoyPitchSensitivitySlider, O_SLIDER, "#GameUI_JoystickLookSpeedPitch", "", &m_pJoyPitchSensitivityPreLabel );

	UpdatePanels();
}

void CTFOptionsMousePanel::DestroyControls()
{
	BaseClass::DestroyControls();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsMousePanel::OnResetData()
{
	BaseClass::OnResetData();

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
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsMousePanel::OnApplyChanges()
{
	BaseClass::OnApplyChanges();

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

	engine->ClientCmd_Unrestricted( "joyadvancedupdate" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsMousePanel::OnControlModified( void )
{
	PostActionSignal( new KeyValues( "ApplyButtonEnable" ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsMousePanel::OnCheckButtonChecked( Panel *panel )
{
	if ( IsVisible() )
	{
		if ( panel == m_pJoystickCheckBox || panel == m_pMouseAccelCheckBox )
			UpdatePanels();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsMousePanel::UpdatePanels()
{
	bool bEnabled = m_pMouseAccelCheckBox->IsChecked();
	m_pMouseAccelSlider->SetVisible( bEnabled );

	bEnabled = m_pJoystickCheckBox->IsChecked();
	m_pReverseJoystickCheckBox->SetVisible( bEnabled );
	m_pJoystickSouthpawCheckBox->SetVisible( bEnabled );
	m_pJoyYawSensitivitySlider->SetVisible( bEnabled );
	m_pJoyYawSensitivityPreLabel->SetVisible( bEnabled );
	m_pJoyPitchSensitivitySlider->SetVisible( bEnabled );
	m_pJoyPitchSensitivityPreLabel->SetVisible( bEnabled );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFOptionsMousePanel::UpdateSensitivityLabel()
{

}
