//=============================================================================//
//
// Purpose: HUD for drawing screen overlays.
//
//=============================================================================//
#ifndef TF_HUD_SCREENOVERLAYS_H
#define TF_HUD_SCREENOVERLAYS_H

#ifdef _WIN32
#pragma once
#endif

#include "igamesystem.h"
#include "GameEventListener.h"
#include "clienteffectprecachesystem.h"
#include "tf_shareddefs.h"

enum
{
	TF_OVERLAY_SHIELD,
	TF_OVERLAY_BERSERK,
	TF_OVERLAY_HASTE,
	TF_OVERLAY_BURNING,
	TF_OVERLAY_INVULN,

	TF_OVERLAY_COUNT
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CHudScreenOverlays : public CAutoGameSystem
{
public:
	CHudScreenOverlays();

	virtual void LevelInitPreEntity( void );
	virtual void LevelShutdownPostEntity( void );

	void DrawOverlays( const CViewSetup &view );

private:
	CMaterialReference m_ScreenOverlays[TF_OVERLAY_COUNT][TF_TEAM_COUNT];
};

extern CHudScreenOverlays g_ScreenOverlayManager;

#endif // TF_HUD_SCREENOVERLAYS_H
