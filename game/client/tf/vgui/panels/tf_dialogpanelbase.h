#ifndef TFDIALOGPANELBASE_H
#define TFDIALOGPANELBASE_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_menupanelbase.h"
#include "controls/tf_scriptobject.h"

class CPanelListPanel;

class CTFDialogPanelBase : public CTFMenuPanelBase
{
	DECLARE_CLASS_SIMPLE( CTFDialogPanelBase, CTFMenuPanelBase );

public:
	CTFDialogPanelBase( vgui::Panel* parent, const char *panelName );
	~CTFDialogPanelBase();
	virtual void OnResetData();
	virtual void OnApplyChanges();
	virtual void OnSetDefaults();
	virtual void OnCommand( const char *command );
	virtual void OnCreateControls() { CreateControls(); OnResetData(); }
	virtual void OnDestroyControls() { DestroyControls(); }
	virtual void Show();
	virtual void Hide();
	virtual void SetEmbedded( bool bState ) { m_bEmbedded = bState; }
	virtual void OnKeyCodeTyped( vgui::KeyCode code );
	virtual void OnKeyTyped( wchar_t key );

protected:
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void ApplySettings( KeyValues *inResourceData );
	virtual void CreateControls();
	virtual void DestroyControls();
	virtual void AddControl( vgui::Panel* panel, objtype_t iType, const char* text = "", const char *pszToolTip = "", vgui::Label **pLabel = NULL );


	bool			m_bEmbedded;
	CPanelListPanel *m_pListPanel;
	bool			m_bHideMainMenu;
};



#endif // TFDIALOGPANELBASE_H