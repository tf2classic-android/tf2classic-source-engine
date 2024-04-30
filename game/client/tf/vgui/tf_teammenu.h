//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef TF_TEAMMENU_H
#define TF_TEAMMENU_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <game/client/iviewport.h>
#include "tf_controls.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFTeamButton : public CExButton
{
private:
	DECLARE_CLASS_SIMPLE( CTFTeamButton, CExButton );

public:
	CTFTeamButton( vgui::Panel *parent, const char *panelName );

	virtual void ApplySettings( KeyValues *inResourceData );
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

	virtual void OnCursorExited();
	virtual void OnCursorEntered();

	virtual void OnTick( void );
	virtual void FireActionSignal( void );

	void SetDefaultAnimation( const char *pszName );

private:
	bool IsTeamFull();
	void SendAnimation( const char *pszAnimation );
	void SetMouseEnteredState( bool state );

private:
	char	m_szModelPanel[64];		// the panel we'll send messages to
	int		m_iTeam;				// the team we're associated with (if any)

	float	m_flHoverTimeToWait;	// length of time to wait before reporting a "hover" message (-1 = no hover)
	float	m_flHoverTime;			// when should a "hover" message be sent?
	bool	m_bMouseEntered;		// used to track when the mouse is over a button
	bool	m_bTeamDisabled;		// used to keep track of whether our team is a valid team for selection
};

//-----------------------------------------------------------------------------
// Purpose: Displays the team menu
//-----------------------------------------------------------------------------
class CTFTeamMenu : public vgui::Frame, public IViewPortPanel
{
public:
	DECLARE_CLASS_SIMPLE( CTFTeamMenu, vgui::Frame );

	CTFTeamMenu( IViewPort *pViewPort, const char *pName );
	~CTFTeamMenu();

	virtual const char *GetName( void ) { return BaseClass::GetName(); }
	virtual void SetData( KeyValues *data ) {}
	virtual void Reset() {}
	virtual void Update();
	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );
	virtual void CreateTeamButtons( void );

	virtual GameActionSet_t GetPreferredActionSet() { return GAME_ACTION_SET_NONE; }

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
	virtual bool IsVisible() { return BaseClass::IsVisible(); }
	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

#ifdef _X360
	CON_COMMAND_MEMBER_F( CTFTeamMenu, "join_team", Join_Team, "Send a jointeam command", 0 );
#endif

protected:
	virtual const char *GetResFilename( void ) { return "Resource/UI/Teammenu.res"; }
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void OnKeyCodePressed( vgui::KeyCode code );

	virtual void SetHighlanderTeamsFullPanels( bool bEnabled );

	// command callbacks
	virtual void OnCommand( const char *command );
	virtual void OnTick( void );
	virtual void OnThink( void );
	virtual void OnClose();

protected:
	CTFTeamButton	*m_pTeamButtons[TF_TEAM_COUNT];

private:
	CExLabel		*m_pSpecLabel;

	CExLabel		*m_pHighlanderLabel;
	CExLabel		*m_pHighlanderLabelShadow;
	CExLabel		*m_pTeamFullLabel;
	CExLabel		*m_pTeamFullLabelShadow;
	CTFImagePanel	*m_pTeamsFullArrow;

#ifdef _X360
	CTFFooter		*m_pFooter;
#else
	CExButton		*m_pCancelButton;
#endif

private:
	ButtonCode_t m_iScoreBoardKey;
	ButtonCode_t m_iTeamMenuKey;
};

//-----------------------------------------------------------------------------
// Purpose: Displays the four team menu
//-----------------------------------------------------------------------------
class CTFFourTeamMenu : public CTFTeamMenu
{
public:
	DECLARE_CLASS_SIMPLE( CTFFourTeamMenu, CTFTeamMenu );

	CTFFourTeamMenu( IViewPort *pViewPort, const char *pName );
	virtual const char *GetResFilename( void ) { return "Resource/UI/FourTeamMenu.res"; }
	virtual void CreateTeamButtons( void );
	
	virtual GameActionSet_t GetPreferredActionSet() { return GAME_ACTION_SET_NONE; }
};

//-----------------------------------------------------------------------------
// Purpose: Displays the FFA team menu
//-----------------------------------------------------------------------------
class CTFDeathmatchTeamMenu : public CTFTeamMenu
{
public:
	DECLARE_CLASS_SIMPLE( CTFDeathmatchTeamMenu, CTFTeamMenu );

	CTFDeathmatchTeamMenu( IViewPort *pViewPort, const char *pName );
	virtual const char *GetResFilename( void ) { return "Resource/UI/DeathmatchTeamMenu.res"; }
	virtual void CreateTeamButtons( void );
	
	virtual GameActionSet_t GetPreferredActionSet() { return GAME_ACTION_SET_NONE; }
};

#endif // TF_TEAMMENU_H
