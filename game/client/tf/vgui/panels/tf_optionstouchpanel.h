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
	void OnResetData();
	void OnApplyChanges();
	void OnCommand(const char *command);
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
	vgui::Label *m_pTouchSensitivityPreLabel;
	CTFCvarSlider *m_pTouchAccelerationSlider;
	vgui::Label *m_pTouchAccelerationPreLabel;
	CTFCvarSlider *m_pTouchHorizSensitSlider;
	vgui::Label *m_pTouchHorizSensitPreLabel;
	CTFCvarSlider *m_pTouchVertSensitSlider;
	vgui::Label *m_pTouchVertSensitPreLabel;
};



#endif // OPTIONS_SUB_TOUCH_H
