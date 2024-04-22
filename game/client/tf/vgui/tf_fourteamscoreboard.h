#ifndef TF_FOURTEAMSCOREBOARD_H
#define TF_FOURTEAMSCOREBOARD_H

#ifdef _WIN32
#pragma once
#endif

#include "hud.h"
#include "hudelement.h"
#include "tf_hud_playerstatus.h"
#include "clientscoreboarddialog.h"
#include "tf_clientscoreboard.h"

class CTFFourTeamScoreBoardDialog : public CTFClientScoreBoardDialog
{
private:
	DECLARE_CLASS_SIMPLE( CTFFourTeamScoreBoardDialog, CTFClientScoreBoardDialog );

public:
	CTFFourTeamScoreBoardDialog( IViewPort *pViewPort, const char *pszName );
	
	virtual const char *GetName(void) { return PANEL_FOURTEAMSCOREBOARD; }
	
protected: // CTFClientScoreBoardDialog
	virtual const char *GetResFilename() { return "Resource/UI/FourTeamScoreBoard.res"; }
};

#endif // TF_FOURTEAMSCOREBOARD_H
