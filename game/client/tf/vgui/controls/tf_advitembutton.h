#ifndef TF_ADVITEMBUTTON_H
#define TF_ADVITEMBUTTON_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_advbutton.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFItemButton : public CTFButton
{
public:
	DECLARE_CLASS_SIMPLE( CTFItemButton, CTFButton );

	CTFItemButton( vgui::Panel *parent, const char *panelName, const char *text );
	~CTFItemButton();

	virtual void Init();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void PerformLayout();
	virtual void ShowToolTip( bool bShow );

	void SetItemDefinition( CEconItemDefinition *pItemData );
	void SetLoadoutSlot( int iSlot, int iPreset );

protected:
	CEconItemDefinition *m_pItemDefinition;
	int m_iLoadoutSlot;
};


#endif // TF_ADVITEMBUTTON_H
