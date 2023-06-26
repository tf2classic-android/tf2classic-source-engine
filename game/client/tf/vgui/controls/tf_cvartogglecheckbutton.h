//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef tf_cvartogglecheckbutton_H
#define tf_cvartogglecheckbutton_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_advcheckbutton.h"

class CTFCvarToggleCheckButton : public CTFCheckButton
{
	DECLARE_CLASS_SIMPLE( CTFCvarToggleCheckButton, CTFCheckButton );

public:
	CTFCvarToggleCheckButton( vgui::Panel *parent, const char *panelName, const char *text = "",
		char const *cvarname = NULL, bool ignoreMissingCvar = false );
	~CTFCvarToggleCheckButton();

	virtual void	OnTick();

	void			Reset();
	void			ApplyChanges();
	bool			HasBeenModified();
	virtual void	ApplySettings( KeyValues *inResourceData );

	const char		*GetCvarName( void ) { return m_cvar.GetName(); }

private:
	MESSAGE_FUNC( OnButtonChecked, "CheckButtonChecked" );
	MESSAGE_FUNC( OnApplyChanges, "ApplyChanges" );

	ConVarRef		m_cvar;
	bool			m_bStartValue;
	bool			m_bIgnoreMissingCvar;
	bool			m_bAutoChange;
};

#endif // tf_cvartogglecheckbutton_H
