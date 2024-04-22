//=============================================================================//
//
// Purpose:
//
//=============================================================================//
#ifndef TF_HUD_WINPANEL_DEATHMATCH_H
#define TF_HUD_WINPANEL_DEATHMATCH_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include "tf_playermodelpanel.h"
#include "hudelement.h"
#include "tf_controls.h"

class CTFWinPanelDeathmatch : public CHudElement, public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE( CTFWinPanelDeathmatch, vgui::EditablePanel );

	CTFWinPanelDeathmatch( const char *pszName );

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual bool ShouldDraw( void );
	virtual void LevelInit( void );
	virtual void FireGameEvent( IGameEvent *event );

private:
	float m_flShowAt;
	bool m_bHiddenScoreboard;

	CExLabel *m_pPlayerNames[3];
	CExLabel *m_pPlayerKills[3];
	CExLabel *m_pPlayerDeaths[3];
	CTFPlayerModelPanel *m_pPlayerModels[3];
	int m_iWinAnimation;
};

#endif // TF_HUD_WINPANEL_DEATHMATCH_H
