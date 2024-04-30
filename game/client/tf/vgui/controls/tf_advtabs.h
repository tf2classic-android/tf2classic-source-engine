#ifndef TF_MAINMENU_TABS_H
#define TF_MAINMENU_TABS_H

#ifdef _WIN32
#pragma once
#endif

class CTFButton;

class CAdvTabs : public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE( CAdvTabs, vgui::EditablePanel );

	CAdvTabs( vgui::Panel *parent, char const *panelName );
	~CAdvTabs();

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void ApplySettings( KeyValues *inResourceData );
	virtual void PerformLayout();
	virtual void OnCommand( const char *command );

	void SetSelectedButton( const char *pszName );

protected:
	MESSAGE_FUNC_PTR( OnButtonPressed, "ButtonPressed", panel );

	CUtlVector<CTFButton *> m_pButtons;
	CTFButton *m_pCurrentButton;
};

#endif // TF_MAINMENU_TABS_H
