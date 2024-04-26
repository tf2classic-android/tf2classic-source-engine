#ifndef TFMAINMENUSHADEBACKGROUNDPANEL_H
#define TFMAINMENUSHADEBACKGROUNDPANEL_H

#include "tf_menupanelbase.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFShadeBackgroundPanel : public CTFMenuPanelBase
{
	DECLARE_CLASS_SIMPLE( CTFShadeBackgroundPanel, CTFMenuPanelBase );

public:
	CTFShadeBackgroundPanel( vgui::Panel *parent, const char *panelName );
	virtual ~CTFShadeBackgroundPanel();
	virtual void Show();
	virtual void Hide();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

private:
	vgui::ImagePanel *m_pShadedBG;
};

#endif // TFMAINMENUSHADEBACKGROUNDPANEL_H