//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef tf_cvarslider_H
#define tf_cvarslider_H
#ifdef _WIN32
#pragma once
#endif

//#include <vgui_controls/Slider.h>
#include "tf_advslider.h"

class CTFCvarSlider : public CTFSlider
{
	DECLARE_CLASS_SIMPLE(CTFCvarSlider, CTFSlider);

public:

	CTFCvarSlider(vgui::Panel *parent, const char *panelName, const char *name);
	CTFCvarSlider( vgui::Panel *parent, const char *panelName, char const *caption,
		float minValue, float maxValue, char const *cvarname, bool bShowFrac = false, bool bAutoChange = false );
	~CTFCvarSlider();

	void SetupSlider( float minValue, float maxValue, bool m_bShowFrac, bool bAutoChange );

	virtual void	ApplySettings( KeyValues *inResourceData );

	virtual void	OnTick( void );

	void            Reset( void );
	bool			HasBeenModified( void );
	void			ApplyChanges( void );

private:
	MESSAGE_FUNC( OnSliderMoved, "SliderDragEnd" );
	MESSAGE_FUNC( OnApplyChanges, "ApplyChanges" );

	ConVarRef		m_cvar;
	float			m_flStartValue;
	bool			m_bAutoChange;
};

#endif // tf_cvarslider_H
