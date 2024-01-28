//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef OPTIONS_SUB_TOUCH_H
#define OPTIONS_SUB_TOUCH_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_dialogpanelbase.h"

class CPanelListPanel;
class CTFButton;
class CTFCvarToggleCheckButton;
class CTFCvarSlider;

namespace vgui
{
    class Label;
    class Panel;
}

//-----------------------------------------------------------------------------
// Purpose: Mouse Details, Part of OptionsDialog
//-----------------------------------------------------------------------------
class CTFOptionsTouchPanel : public CTFDialogPanelBase
{
	DECLARE_CLASS_SIMPLE(CTFOptionsTouchPanel, CTFDialogPanelBase);

public:
	CTFOptionsTouchPanel(vgui::Panel* parent, const char *panelName);
	~CTFOptionsTouchPanel();
	virtual bool Init();
	virtual void OnResetData();
	virtual void OnApplyChanges();
	virtual void OnCommand(const char *command);
protected:
    void ApplySchemeSettings(vgui::IScheme *pScheme);
	void CreateControls();
	void DestroyControls();
	void UpdateSensitivityLabel();
	void UpdatePanels();

private:
	MESSAGE_FUNC( OnControlModified, "ControlModified" );
	MESSAGE_FUNC_PTR( OnCheckButtonChecked, "CheckButtonChecked", panel );

	vgui::Label *pTitleTouch;
	CTFCvarToggleCheckButton *m_pEnableTouch;
	CTFCvarToggleCheckButton *m_pDrawTouch;
	CTFCvarToggleCheckButton *m_pReverseTouch;
	CTFCvarToggleCheckButton *m_pTouchFilter;
	CTFCvarToggleCheckButton *m_pTouchAcceleration;

	CTFCvarSlider *m_pTouchSensitivitySlider;
	CTFCvarSlider *m_pTouchAccelerationSlider;
	CTFCvarSlider *m_pTouchHorizSensitSlider;
	CTFCvarSlider *m_pTouchVertSensitSlider;

	/*
	vgui::Label					*pTitleMouse;
	vgui::Label					*pTitleJoystick;
	CTFCvarToggleCheckButton		*m_pReverseMouseCheckBox;
	CTFCvarToggleCheckButton		*m_pRawInputCheckBox;
	CTFCvarToggleCheckButton		*m_pMouseFilterCheckBox;

	CTFCvarSlider					*m_pMouseSensitivitySlider;
    vgui::TextEntry             *m_pMouseSensitivityLabel;

	CTFCvarToggleCheckButton		*m_pMouseAccelCheckBox;
	CTFCvarSlider					*m_pMouseAccelSlider;

	CTFCvarToggleCheckButton		*m_pJoystickCheckBox;
	CTFCvarToggleCheckButton		*m_pJoystickSouthpawCheckBox;
	CTFCvarToggleCheckButton		*m_pQuickInfoCheckBox;
	CTFCvarToggleCheckButton		*m_pReverseJoystickCheckBox;

	CTFCvarSlider					*m_pJoyYawSensitivitySlider;
	vgui::Label					*m_pJoyYawSensitivityPreLabel;
	CTFCvarSlider					*m_pJoyPitchSensitivitySlider;
	vgui::Label					*m_pJoyPitchSensitivityPreLabel;
	*/
};



#endif // OPTIONS_SUB_TOUCH_H