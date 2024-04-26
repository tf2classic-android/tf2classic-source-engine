#ifndef TF_ROTATINGPANEL_H
#define TF_ROTATINGPANEL_H
#ifdef _WIN32
#pragma once
#endif

#include "vgui_controls/EditablePanel.h"

//-----------------------------------------------------------------------------
// Purpose:  Draws the rotated arrow panels
//-----------------------------------------------------------------------------
class CTFRotatingImagePanel : public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE( CTFRotatingImagePanel, vgui::EditablePanel );

	CTFRotatingImagePanel( vgui::Panel *parent, const char *name );
	virtual void Paint();
	virtual void ApplySettings( KeyValues *inResourceData );
	float GetAngleRotation( void );

private:
	float				m_flAngle;
	CMaterialReference	m_Material;
};

#endif // TF_ROTATINGPANEL_H
