//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef TF_VIEWRENDER_H
#define TF_VIEWRENDER_H
#ifdef _WIN32
#pragma once
#endif

#include "iviewrender.h"
#include "viewrender.h"

//-----------------------------------------------------------------------------
// Purpose: Implements the interview to view rendering for the client .dll
//-----------------------------------------------------------------------------
class CTFViewRender : public CViewRender
{
public:
	DECLARE_CLASS_GAMEROOT( CTFViewRender, CViewRender );

	CTFViewRender();

	virtual void Render2DEffectsPreHUD( const CViewSetup &view );
	virtual void Render2DEffectsPostHUD( const CViewSetup &view );
};

#endif //TF_VIEWRENDER_H
