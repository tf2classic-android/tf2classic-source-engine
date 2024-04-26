#ifndef TF_ADVITEMBUTTON_H
#define TF_ADVITEMBUTTON_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_advbutton.h"
#include "tf_hud_weaponswitch.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFItemButton : public CTFButton
{
public:
	DECLARE_CLASS_SIMPLE( CTFItemButton, CTFButton );

	CTFItemButton( vgui::Panel *parent, const char *panelName, const char *text );
	~CTFItemButton();

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void PerformLayout();
	virtual void ShowToolTip( bool bShow );

	void SetItem( CEconItemView *pItem );
	void SetLoadoutSlot( ETFLoadoutSlot iSlot, int iPreset );

protected:
	CEconItemView *m_pItem;
	ETFLoadoutSlot m_iLoadoutSlot;
	CItemModelPanel *m_pItemPanel;
};

#endif // TF_ADVITEMBUTTON_H
