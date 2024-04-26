#ifndef TFMAINMENU_H
#define TFMAINMENU_H

#include <vgui_controls/EditablePanel.h>

struct ClassStats_t;

enum MenuPanel //position in this enum = zpos on the screen
{
	NONE_MENU,
	BACKGROUND_MENU,
	MAIN_MENU,
	PAUSE_MENU,
	SHADEBACKGROUND_MENU, //add popup/additional menus below:		
	LOADOUT_MENU,
	STATSUMMARY_MENU,
	NOTIFICATION_MENU,
	OPTIONSDIALOG_MENU,
	CREATESERVER_MENU,
	QUIT_MENU,
	TOOLTIP_MENU,
	ITEMTOOLTIP_MENU,
	COUNT_MENU,

	FIRST_MENU = NONE_MENU + 1
};

#define GET_MAINMENUPANEL( className ) assert_cast<className*>(guiroot->GetMenuPanel(#className))

class CTFMenuPanelBase;
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFMainMenu : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CTFMainMenu, vgui::EditablePanel );

public:
	CTFMainMenu();
	virtual ~CTFMainMenu();

	enum
	{
		TFMAINMENU_STATUS_UNDEFINED = 0,
		TFMAINMENU_STATUS_MENU,
		TFMAINMENU_STATUS_BACKGROUNDMAP,
		TFMAINMENU_STATUS_INGAME,
	};

	CTFMenuPanelBase* GetMenuPanel( int iPanel );
	CTFMenuPanelBase* GetMenuPanel( const char *name );
	MenuPanel GetCurrentMainMenu( void ) { return ( !IsInLevel() ? MAIN_MENU : PAUSE_MENU ); }
	int GetMainMenuStatus( void ) { return m_iMainMenuStatus; }

	virtual void OnTick();

	void ShowPanel( MenuPanel iPanel, bool m_bShowSingle = false );
	void HidePanel( MenuPanel iPanel );
	void InvalidatePanelsLayout( bool layoutNow = false, bool reloadScheme = false );
	void LaunchInvalidatePanelsLayout();
	bool IsInLevel();
	bool IsInBackgroundLevel();
	void UpdateCurrentMainMenu();
	void SetStats( CUtlVector<ClassStats_t> &vecClassStats );
	void ShowToolTip( const char *pszText );
	void HideToolTip();
	void ShowItemToolTip( CEconItemDefinition *pItemData );
	void HideItemToolTip();
	void OnNotificationUpdate();
	void FadeMainMenuIn();

private:
	CTFMenuPanelBase	*m_pPanels[COUNT_MENU];
	void				AddMenuPanel( CTFMenuPanelBase *m_pPanel, int iPanel );

	int					m_iMainMenuStatus;
	int					m_iUpdateLayout;
};

extern CTFMainMenu *guiroot;

#endif // TFMAINMENU_H
