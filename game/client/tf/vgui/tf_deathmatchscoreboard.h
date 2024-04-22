#ifndef TF_DEATHMATCHSCOREBOARD_H
#define TF_DEATHMATCHSCOREBOARD_H

#ifdef _WIN32
#pragma once
#endif

#include "hud.h"
#include "hudelement.h"
#include "tf_hud_playerstatus.h"
#include "clientscoreboarddialog.h"
#include "tf_clientscoreboard.h"

class CTFDeathMatchScoreBoardDialog : public CTFClientScoreBoardDialog
{
private:
	DECLARE_CLASS_SIMPLE( CTFDeathMatchScoreBoardDialog, CTFClientScoreBoardDialog );
	
public:
	CTFDeathMatchScoreBoardDialog( IViewPort *pViewPort, const char *pszName );
	virtual void Update();

protected:
	virtual const char *GetName( void ) { return PANEL_DEATHMATCHSCOREBOARD; }
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

protected: // CTFClientScoreBoardDialog
	virtual void InitPlayerList( vgui::SectionedListPanel *pPlayerList );
	virtual void UpdatePlayerList();
	virtual int GetDefaultAvatar( int playerIndex );
	
	virtual const char *GetResFilename() { return "Resource/UI/DeathMatchScoreBoard.res"; }

private:
	void ResizeScoreboard();
	
	vgui::SectionedListPanel *m_pPlayerList;
	int m_iDefaultAvatar;
	int m_iDefaultTall;
	
	CPanelAnimationVarAliasType( int, m_iKillsWidth, "kills_width", "23", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iDeathsWidth, "deaths_width", "23", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iKillstreakWidth, "killstreak_width", "23", "proportional_int" );
};

#endif // TF_DEATHMATCHSCOREBOARD_H
