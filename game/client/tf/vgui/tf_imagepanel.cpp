//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"
#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui/IScheme.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui/ISurface.h>
#include <vgui/IImage.h>
#include <vgui_controls/Label.h>

#include "tf_imagepanel.h"
#include "c_tf_player.h"
#include "cdll_util.h"
#include "tf_gamerules.h"
#include "functionproxy.h"

using namespace vgui;

DECLARE_BUILD_FACTORY( CTFImagePanel );

extern ConVar tf2c_coloredhud;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFImagePanel::CTFImagePanel(Panel *parent, const char *name) : ScalableImagePanel(parent, name)
{
	for ( int i = 0; i < TF_TEAM_COUNT; i++ )
	{
		m_szTeamBG[i][0] = '\0';
	}

	m_bAlwaysColored = false;

	UpdateBGTeam();

	ListenForGameEvent( "localplayer_changeteam" );
	ListenForGameEvent( "server_spawn" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFImagePanel::ApplySettings(KeyValues *inResourceData)
{
	for ( int i = 0; i < TF_TEAM_COUNT; i++ )
	{
		Q_strncpy( m_szTeamBG[i], inResourceData->GetString( VarArgs("teambg_%d", i), "" ), sizeof( m_szTeamBG[i] ) );
	}
	
	m_bAlwaysColored = inResourceData->GetBool( "alwaysColored" );
	
	BaseClass::ApplySettings( inResourceData );

	UpdateBGImage();
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFImagePanel::UpdateBGImage( void )
{
	if ( m_iBGTeam >= 0 && m_iBGTeam < TF_TEAM_COUNT )
	{
		if ( m_szTeamBG[m_iBGTeam][0] != '\0' )
		{
			SetImage( m_szTeamBG[m_iBGTeam] );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFImagePanel::SetBGImage( int iTeamNum )
{
	m_iBGTeam = iTeamNum;
	UpdateBGImage();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFImagePanel::UpdateBGTeam( void )
{
	if( TFGameRules() && TFGameRules()->IsDeathmatch() )
		m_iBGTeam = tf2c_coloredhud.GetBool() || m_bAlwaysColored ? TEAM_UNASSIGNED : TEAM_SPECTATOR;
	else
		m_iBGTeam = GetLocalPlayerTeam();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFImagePanel::FireGameEvent(IGameEvent * event)
{
	if ( FStrEq( "localplayer_changeteam", event->GetName() ) )
	{
		UpdateBGTeam();
		UpdateBGImage();
	}
	else if ( FStrEq( "server_spawn", event->GetName() ) )
	{
		m_iBGTeam = TEAM_SPECTATOR;
		UpdateBGImage();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
Color CTFImagePanel::GetDrawColor(void)
{
	Color tempColor = GetFgColor();
	tempColor[3] = GetAlpha();

	return tempColor;
}

//-----------------------------------------------------------------------------
// Purpose: Same as PlayerTintColor but gets color of the local player. Should be used on HUD panels.
//-----------------------------------------------------------------------------
class CLocalPlayerTintColor : public CResultProxy
{
public:
	void OnBind( void *pC_BaseEntity )
	{
		C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();

		if ( pPlayer )
		{
			m_pResult->SetVecValue( pPlayer->m_vecPlayerColor.Base(), 3 );
		}
		else
		{
			m_pResult->SetVecValue( 0, 0, 0 );
		}
	}
};

EXPOSE_INTERFACE( CLocalPlayerTintColor, IMaterialProxy, "LocalPlayerTintColor" IMATERIAL_PROXY_INTERFACE_VERSION );
