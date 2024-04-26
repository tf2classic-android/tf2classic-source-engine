#ifndef TFMAINMENULOADOUTPANEL_H
#define TFMAINMENULOADOUTPANEL_H

#include "tf_dialogpanelbase.h"
#include "tf_inventory.h"
#include "tf_playermodelpanel.h"
#include "controls/tf_advtabs.h"

class CTFWeaponSetPanel;
class CModelPanel;
class CTFButton;
class CTFRGBPanel;
class CTFItemButton;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFLoadoutPanel : public CTFDialogPanelBase
{
	DECLARE_CLASS_SIMPLE( CTFLoadoutPanel, CTFDialogPanelBase );

public:
	CTFLoadoutPanel( vgui::Panel *parent, const char *panelName );
	virtual ~CTFLoadoutPanel();
	void ApplySchemeSettings( vgui::IScheme *pScheme );
	void PerformLayout();

	void SetupWeaponIcon( CTFItemButton *pButton, ETFLoadoutSlot iSlot, int iPreset );

	void OnCommand( const char* command );
	void Hide();
	void Show();
	void UpdateWeaponButtons();
	void SetWeaponPreset( int iClass, int iSlot, int iPreset );
	void SetCurrentClass( int iClass );
	void SideRow( int iRow, int iDir );
	void ResetRows();

private:
	MESSAGE_FUNC_PTR( OnControlModified, "ControlModified", panel );

	CTFPlayerModelPanel *m_pClassModelPanel;
	CTFWeaponSetPanel *m_pWeaponSetPanel;
	CTFRGBPanel *m_pRGBPanel;
	CUtlVector<CTFItemButton *> m_pWeaponIcons[INVENTORY_ROWNUM];
	CTFButton *m_pSlideButtons[INVENTORY_ROWNUM*2];
	CAdvTabs *m_pClassButtons;
	int m_RawIDPos[INVENTORY_ROWNUM];
	int m_iCurrentClass;
	bool m_bLoadoutChanged;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFWeaponSetPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CTFWeaponSetPanel, vgui::EditablePanel );

public:
	CTFWeaponSetPanel( vgui::Panel* parent, const char *panelName );
	void OnCommand( const char* command );
};

#endif // TFMAINMENULOADOUTPANEL_H
