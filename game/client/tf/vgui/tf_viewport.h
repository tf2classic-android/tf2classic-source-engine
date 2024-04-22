//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef TF_VIEWPORT_H
#define TF_VIEWPORT_H


#include "tf_shareddefs.h"
#include "baseviewport.h"


using namespace vgui;

namespace vgui 
{
	class Panel;
	class Label;
	class CBitmapImagePanel;
}

//==============================================================================
class TFViewport : public CBaseViewport
{

private:
	DECLARE_CLASS_SIMPLE( TFViewport, CBaseViewport );

public:
	TFViewport();

	IViewPortPanel* CreatePanelByName( const char *szPanelName );
	void CreateDefaultPanels( void );

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void Start( IGameUIFuncs *pGameUIFuncs, IGameEventManager2 * pGameEventManager );
		
	int GetDeathMessageStartHeight( void );

	const char *GetModeSpecificScoreboardName( void );
	void ShowScoreboard( bool bState, int nPollHideCode = BUTTON_CODE_INVALID );
	
	CON_COMMAND_MEMBER_F( TFViewport, "+showscores", CC_ScoresDown, "Toggles score panel", FCVAR_NONE );
	CON_COMMAND_MEMBER_F( TFViewport, "-showscores", CC_ScoresUp, "Toggles score panel", FCVAR_NONE );
	
	CON_COMMAND_MEMBER_F( TFViewport, "togglescores", CC_ToggleScores, "Toggles score panel", FCVAR_NONE );

	void ShowTeamMenu( bool bState );
	CON_COMMAND_MEMBER_F( TFViewport, "changeteam", CC_ChangeTeam, "Choose a new team", FCVAR_NONE );

	void ShowClassMenu( bool bState );
	CON_COMMAND_MEMBER_F( TFViewport, "changeclass", CC_ChangeClass, "Choose a new class", FCVAR_NONE );

	virtual void OnScreenSizeChanged( int iOldWide, int iOldTall );

private:
	void CenterWindow( vgui::Frame *win );
};

inline TFViewport *GetTFViewPort( void )
{
	return static_cast<TFViewport *>( gViewPortInterface );
}

#endif // TF_Viewport_H
