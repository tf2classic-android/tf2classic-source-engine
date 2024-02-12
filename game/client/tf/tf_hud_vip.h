#if !defined( TF_HUD_VIP_H )
#define TF_HUD_VIP_H

#if defined( _WIN32 )
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include "hudelement.h"
#include "tf_controls.h"
#include <vgui_controls/AnimationController.h>

class CTFHudVIP : public vgui::EditablePanel
{
private:
	DECLARE_CLASS_SIMPLE( CTFHudVIP, vgui::EditablePanel );

public:
	CTFHudVIP( vgui::Panel *pParent, const char *pszName );
	virtual ~CTFHudVIP();

public:
	void ApplySchemeSettings( vgui::IScheme *pScheme );
	void OnTick();
	bool IsVisible();

private:
	float m_flPreviousProgress;
	vgui::ImagePanel *m_pLevelBar;
	vgui::EditablePanel *m_pEscortItemPanel;
};

#endif
