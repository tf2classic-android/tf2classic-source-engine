#ifndef TFMAINMENUPAUSEPANEL_H
#define TFMAINMENUPAUSEPANEL_H

#include "tf_menupanelbase.h"

class CTFButton;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFPauseMenuPanel : public CTFMenuPanelBase
{
	DECLARE_CLASS_SIMPLE( CTFPauseMenuPanel, CTFMenuPanelBase );

public:
	CTFPauseMenuPanel( vgui::Panel* parent, const char *panelName );
	virtual ~CTFPauseMenuPanel();

	virtual void PerformLayout();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void Show();
	virtual void Hide();
	virtual void OnCommand( const char* command );
	virtual void OnNotificationUpdate();

private:
	CTFButton	*m_pNotificationButton;
};

#endif // TFMAINMENUPAUSEPANEL_H
