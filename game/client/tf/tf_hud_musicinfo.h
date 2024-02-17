#if !defined( TF_HUD_MUSICINFO_H )
#define TF_HUD_MUSICINFO_H

#if defined( _WIN32 )
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include "hudelement.h"
#include "tf_controls.h"
#include <vgui_controls/AnimationController.h>
#include "GameEventListener.h"

class CTFHudMusicInfo : public CHudElement, public vgui::EditablePanel
{
private:
	DECLARE_CLASS_SIMPLE( CTFHudMusicInfo, vgui::EditablePanel );

public:
	CTFHudMusicInfo( const char *pElementName );
	virtual ~CTFHudMusicInfo();

public:
	void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void LevelInit();
	void OnTick();
	void ResetAnimation();
	bool ShouldDraw();

public:
	virtual void FireGameEvent( IGameEvent *event );

private:
	vgui::EditablePanel *m_pInfoPanel;
	CExLabel *m_pNameLabel;
	CExLabel *m_pComposerLabel;

	float m_flHideAt;
	float m_flShowAt;
};

#endif
