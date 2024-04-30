//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_dialogpanelbase.h"

class CTFButton;

//-----------------------------------------------------------------------------
// Purpose: Holds all the game option pages
//-----------------------------------------------------------------------------
class CTFOptionsDialog : public CTFDialogPanelBase
{
	DECLARE_CLASS_SIMPLE(CTFOptionsDialog, CTFDialogPanelBase);

public:
	CTFOptionsDialog(vgui::Panel *parent, const char *panelName);
	~CTFOptionsDialog();
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual void OnCommand(const char *command);
	virtual void Show();
	virtual void Hide();

	MESSAGE_FUNC(OnCancelPressed, "CancelPressed");
	MESSAGE_FUNC(OnOkPressed, "OkPressed");
	MESSAGE_FUNC(OnApplyPressed, "OnApplyPressed");	
	MESSAGE_FUNC(OnDefaultPressed, "OnDefaultPressed");
	MESSAGE_FUNC(OnGameUIHidden, "GameUIHidden");	// called when the GameUI is hidden
	MESSAGE_FUNC(OnApplyButtonEnabled, "ApplyButtonEnable" );

private:
	enum OptionPanel
	{
		PANEL_ADV,
		PANEL_MOUSE,
		PANEL_TOUCH,
		PANEL_KEYBOARD,
		PANEL_AUDIO,
		PANEL_VIDEO,

		PANEL_COUNT
	};

	void SetCurrentPanel(OptionPanel pCurrentPanel);
	void AddPanel(CTFDialogPanelBase *m_pPanel, int iPanel);
	CTFDialogPanelBase* GetPanel(int iPanel);
	CUtlVector<CTFDialogPanelBase*>	m_pPanels;
	OptionPanel	m_pOptionsCurrent;

	CTFButton *m_pApplyButton;
	CTFButton *m_pDefaultsButton;
};


#define OPTIONS_MAX_NUM_ITEMS 15

struct OptionData_t;


#endif // OPTIONSDIALOG_H
