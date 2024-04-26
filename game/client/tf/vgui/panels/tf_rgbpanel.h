#ifndef TFMAINMENURGBPANEL_H
#define TFMAINMENURGBPANEL_H

#include <vgui_controls/EditablePanel.h>
#include "tf_shareddefs.h"

class CTFCvarSlider;
class CCvarComboBox;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFRGBPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CTFRGBPanel, vgui::EditablePanel );

public:
	CTFRGBPanel( vgui::Panel *parent, const char *panelName );

	virtual void PerformLayout();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	const char *GetSpawnEffect();

private:
	MESSAGE_FUNC_PTR( OnControlModified, "ControlModified", panel );
	MESSAGE_FUNC_PTR( OnTextChanged, "TextChanged", panel );

	CTFCvarSlider *m_pRedScrollBar;
	CTFCvarSlider *m_pGrnScrollBar;
	CTFCvarSlider *m_pBluScrollBar;
	vgui::ImagePanel *m_pColorBG;
	vgui::ComboBox *m_pCombo;
	vgui::ComboBox *m_pAnimCombo;
};

#endif // TFMAINMENURGBPANEL_H
