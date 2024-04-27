//=============================================================================//
//
// Purpose: HUD for drawing screen overlays.
//
//=============================================================================//
#include "cbase.h"
#include "tf_hud_screenoverlays.h"
#include "iclientmode.h"
#include "c_tf_player.h"
#include "view_scene.h"
#include "functionproxy.h"
#include "tf_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

CHudScreenOverlays g_ScreenOverlayManager;

struct ScreenOverlayData_t
{
	const char *pszName;
	bool bTeamColored;
};

ScreenOverlayData_t g_aScreenOverlays[TF_OVERLAY_COUNT] =
{
	{ "effects/shield_overlay_%s",		true	},
	{ "effects/berserk_overlay",		false	},
	{ "effects/dodge_overlay",		false	},
	{ "effects/imcookin",			false	},
	{ "effects/invuln_overlay_%s",		true	},
	{ "effects/desaturate",		false	},
};

CHudScreenOverlays::CHudScreenOverlays()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudScreenOverlays::LevelInitPreEntity( void )
{
	for ( int i = 0; i < TF_OVERLAY_COUNT; i++ )
	{
		if ( g_aScreenOverlays[i].bTeamColored )
		{
			// Precache all team colored versions including FFA.
			for ( int j = FIRST_GAME_TEAM; j < TF_TEAM_COUNT; j++ )
			{
				m_ScreenOverlays[i][j].Init( VarArgs( g_aScreenOverlays[i].pszName, g_aTeamLowerNames[j] ), TEXTURE_GROUP_CLIENT_EFFECTS );
			}

			m_ScreenOverlays[i][TEAM_SPECTATOR].Init( VarArgs( g_aScreenOverlays[i].pszName, "dm" ), TEXTURE_GROUP_CLIENT_EFFECTS );
		}
		else
		{
			m_ScreenOverlays[i][TEAM_UNASSIGNED].Init( g_aScreenOverlays[i].pszName, TEXTURE_GROUP_CLIENT_EFFECTS );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudScreenOverlays::LevelShutdownPostEntity( void )
{
	for ( int i = 0; i < TF_OVERLAY_COUNT; i++ )
	{
		for ( int j = 0; j < TF_TEAM_COUNT; j++ )
		{
			m_ScreenOverlays[i][j].Shutdown();
		}
	}
}

#define MAKEFLAG(x)	( 1 << x )

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudScreenOverlays::DrawOverlays( const CViewSetup &view )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer(); //GetLocalObservedPlayer( true );
	if ( !pPlayer )
		return;

	//if ( pPlayer->ShouldDrawThisPlayer() )
	//	return;

	// Check which overlays we should draw.
	int nOverlaysToDraw = 0;

	if ( pPlayer->IsAlive() )
	{
		if ( pPlayer->m_Shared.InCond( TF_COND_POWERUP_SHIELD ) )
		{
			nOverlaysToDraw |= MAKEFLAG( TF_OVERLAY_SHIELD );
		}

		if ( pPlayer->m_Shared.InCond( TF_COND_POWERUP_RAGEMODE ) )
		{
			nOverlaysToDraw |= MAKEFLAG( TF_OVERLAY_BERSERK );
		}

		if ( pPlayer->m_Shared.InCond( TF_COND_POWERUP_SPEEDBOOST ) )
		{
			nOverlaysToDraw |= MAKEFLAG( TF_OVERLAY_HASTE );
		}

		if ( pPlayer->m_Shared.InCond( TF_COND_BURNING ) )
		{
			nOverlaysToDraw |= MAKEFLAG( TF_OVERLAY_BURNING );
		}

		if ( pPlayer->m_Shared.InCond( TF_COND_INVULNERABLE_SPAWN_PROTECT ) && pPlayer->GetDesaturationAmount() <= 0.0f )
		{
			nOverlaysToDraw |= MAKEFLAG( TF_OVERLAY_INVULN );
		}

		if ( pPlayer->GetDesaturationAmount() != 0.0f )
		{
			nOverlaysToDraw |= MAKEFLAG( TF_OVERLAY_DESATURATE );
		}
	}

	int iTeam = pPlayer->GetTeamNumber();

	// Draw overlays, the order is important.
	for ( int i = 0; i < TF_OVERLAY_COUNT; i++ )
	{
		IMaterial *pMaterial;
		if ( g_aScreenOverlays[i].bTeamColored )
		{
			pMaterial = !TFGameRules()->IsDeathmatch() ? m_ScreenOverlays[i][iTeam] : m_ScreenOverlays[i][TEAM_SPECTATOR];
		}
		else
		{
			pMaterial = m_ScreenOverlays[i][TEAM_UNASSIGNED];
		}

		if ( ( nOverlaysToDraw & MAKEFLAG( i ) ) && pMaterial )
		{
			int x = view.x;
			int y = view.y;
			int w = view.width;
			int h = view.height;

			if ( pMaterial->NeedsFullFrameBufferTexture() )
			{
				// FIXME: check with multi/sub-rect renders. Should this be 0,0,w,h instead?
				DrawScreenEffectMaterial( pMaterial, x, y, w, h );
			}
			else if ( pMaterial->NeedsPowerOfTwoFrameBufferTexture() )
			{
				// First copy the FB off to the offscreen texture
				UpdateRefractTexture( x, y, w, h, true );

				// Now draw the entire screen using the material...
				CMatRenderContextPtr pRenderContext( materials );
				ITexture *pTexture = GetPowerOfTwoFrameBufferTexture();
				int sw = pTexture->GetActualWidth();
				int sh = pTexture->GetActualHeight();
				// Note - don't offset by x,y - already done by the viewport.
				pRenderContext->DrawScreenSpaceRectangle( pMaterial, 0, 0, w, h,
					0, 0, sw - 1, sh - 1, sw, sh );
			}
			else
			{
				byte color[4] = { 255, 255, 255, 255 };
				render->ViewDrawFade( color, pMaterial );
			}
		}
	}
}
