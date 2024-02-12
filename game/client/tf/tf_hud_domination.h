#if !defined( TF_HUD_DOMINATION_H )
#define TF_HUD_DOMINATION_H

#if defined( _WIN32 )
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include "hudelement.h"
#include "tf_controls.h"
#include <vgui_controls/AnimationController.h>
#include "GameEventListener.h"

class CTFHudDomination : public vgui::EditablePanel, public CGameEventListener
{
private:
	DECLARE_CLASS_SIMPLE( CTFHudDomination, vgui::EditablePanel );

public:
	CTFHudDomination( vgui::Panel *pParent, const char *pszName );
	virtual ~CTFHudDomination();

public:
	void ApplySchemeSettings( vgui::IScheme *pScheme );
	void OnTick();
	bool IsVisible();

public:
	virtual void FireGameEvent( IGameEvent *event );
};

#endif
