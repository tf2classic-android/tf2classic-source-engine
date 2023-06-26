//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef OPTIONS_SUB_MOUSE_H
#define OPTIONS_SUB_MOUSE_H
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
class CTFOptionsMousePanel : public CTFDialogPanelBase
{
	DECLARE_CLASS_SIMPLE(CTFOptionsMousePanel, CTFDialogPanelBase);

public:
	CTFOptionsMousePanel(vgui::Panel* parent, const char *panelName);
	~CTFOptionsMousePanel();
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
};



#endif // OPTIONS_SUB_MOUSE_H