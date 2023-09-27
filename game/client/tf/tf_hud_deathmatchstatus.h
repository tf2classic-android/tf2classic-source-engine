//=============================================================================//
//
// Purpose: Deathmatch HUD
//
//=============================================================================//

#ifndef TF_HUD_DEATHMATCHSTATUS_H
#define TF_HUD_DEATHMATCHSTATUS_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include "tf_controls.h"
#include "vgui_avatarimage.h"

//-----------------------------------------------------------------------------
// Purpose:  
//-----------------------------------------------------------------------------
class CTFHudDeathMatchObjectives : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CTFHudDeathMatchObjectives, vgui::EditablePanel );

public:

	CTFHudDeathMatchObjectives( vgui::Panel *parent, const char *name );

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual bool IsVisible( void );
	virtual void LevelInit( void );
	virtual void Reset( void );
	virtual void OnTick( void );

private:
	void UpdateStatus( void );
	void SetPlayingToLabelVisible( bool bVisible );

private:
	vgui::EditablePanel *m_pLocalPlayerPanel;
	vgui::EditablePanel *m_pBestPlayerPanel;
	CAvatarImagePanel *m_pPlayerAvatar;
	CAvatarImagePanel *m_pRivalAvatar;

	CExLabel *m_pPlayingTo;
	CTFImagePanel *m_pPlayingToBG;

	int m_iRivalPlayer;

	CPanelAnimationVar( Color, m_DeltaPositiveColor, "PositiveColor", "0 255 0 255" );
	CPanelAnimationVar( Color, m_DeltaNegativeColor, "NegativeColor", "255 0 0 255" );
};

#endif	// TF_HUD_DEATHMATCHSTATUS_H
