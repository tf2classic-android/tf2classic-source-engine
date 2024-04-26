#ifndef tf_advbutton_H
#define tf_advbutton_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include "tf_imagepanel.h"
#include "tf_advbuttonbase.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFButton : public CTFButtonBase
{
public:
	DECLARE_CLASS_SIMPLE( CTFButton, CTFButtonBase );

	CTFButton( vgui::Panel *parent, const char *panelName, const char *text );

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void ApplySettings( KeyValues *inResourceData );
	virtual void PerformLayout();
	virtual void OnThink();
	virtual void SetArmed( bool bState );
	virtual void DoClick( void );

	void SetGlowing( bool Glowing );

private:
	int				m_iOrigX;
	int				m_iOrigY;
	CPanelAnimationVarAliasType( int, m_iXShift, "xshift", "0", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iYShift, "yshift", "0", "proportional_int" );

	bool			m_bGlowing;
	bool			m_bAnimationIn;

	float			m_flActionThink;
	float			m_flAnimationThink;
};


#endif // tf_advbutton_H
