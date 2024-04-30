//========= Copyright © 1996-2007, Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"
#include "hudelement.h"
#include "iclientmode.h"
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/Label.h>

#include "tf_hud_condstatus.h"
#include "tf_hud_freezepanel.h"
#include "tf_gamerules.h"

using namespace vgui;

DECLARE_HUDELEMENT( CTFHudCondStatus );

//-----------------------------------------------------------------------------
// Purpose:  
//-----------------------------------------------------------------------------
CTFPowerupPanel::CTFPowerupPanel( Panel *parent, const char *name ) : EditablePanel( parent, name )
{
	m_pProgressBar = new CTFProgressBar( this, "TimePanelProgressBar" );
}

//-----------------------------------------------------------------------------
// Purpose:  
//-----------------------------------------------------------------------------
void CTFPowerupPanel::ApplySchemeSettings( IScheme *pScheme )
{
	LoadControlSettings( "resource/UI/PowerupPanel.res" );

	if ( m_pProgressBar )
	{
		for ( int i = 0; g_aPowerups[i].cond != TF_COND_LAST; i++ )
		{
			ETFCond nCond = g_aPowerups[i].cond;
			if ( nCond == m_nCond )
			{
				m_pProgressBar->SetIcon( g_aPowerups[i].hud_icon );
			}
		}
	}

	BaseClass::ApplySchemeSettings( pScheme );
}

//-----------------------------------------------------------------------------
// Purpose:  
//-----------------------------------------------------------------------------
void CTFPowerupPanel::UpdateStatus( void )
{
	// Update remaining power-up time.
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();

	if ( pPlayer )
	{
		m_flDuration = pPlayer->m_Shared.GetConditionDuration( m_nCond );
		m_flInitDuration = MAX( m_flDuration, m_flInitDuration );
	}

	if ( m_pProgressBar )
	{
		m_pProgressBar->SetPercentage( ( m_flInitDuration - m_flDuration ) / m_flInitDuration );
	}
}

//-----------------------------------------------------------------------------
// Purpose:  
//-----------------------------------------------------------------------------
void CTFPowerupPanel::SetData( ETFCond cond, float dur, float initdur )
{
	m_nCond = cond;
	m_flDuration = dur;
	m_flInitDuration = initdur;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFHudCondStatus::CTFHudCondStatus( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudCondStatus" )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	for ( int i = 0; g_aPowerups[i].cond != TF_COND_LAST; i++ )
	{
		CTFPowerupPanel *pPowerup = new CTFPowerupPanel( this, "PowerupPanel" );
		pPowerup->SetData( g_aPowerups[i].cond, 0.0f, 0.0f );
		m_pPowerups.AddToTail( pPowerup );
	}

	SetHiddenBits( HIDEHUD_MISCSTATUS );

	vgui::ivgui()->AddTickSignal( GetVPanel() );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFHudCondStatus::~CTFHudCondStatus()
{
	m_pPowerups.PurgeAndDeleteElements();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTFHudCondStatus::ShouldDraw( void )
{
	return CHudElement::ShouldDraw();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudCondStatus::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	// load control settings...
	LoadControlSettings( "resource/UI/HudCondStatus.res" );
}

void CTFHudCondStatus::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );
}

static int DurationSort( CTFPowerupPanel* const *a, CTFPowerupPanel* const *b )
{
	return ( ( *a )->m_flDuration < ( *b )->m_flDuration );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudCondStatus::PerformLayout()
{
	BaseClass::PerformLayout();

	int count = m_pPowerups.Count();

	int totalWide = 0;
	for ( int i = 0; i < count; i++ )
	{
		m_pPowerups[i]->SetPos( totalWide, 0 );

		// Skip inactive powerups.
		if ( m_pPowerups[i]->IsVisible() )
		{
			totalWide += m_pPowerups[i]->GetWide();
		}
	}

	SetWide( totalWide );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudCondStatus::OnTick( void )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();

	if ( pPlayer )
	{
		for ( int i = 0; i < m_pPowerups.Count(); i++ )
		{
			m_pPowerups[i]->UpdateStatus();
		}
	}

	bool bUpdateLayout = false;

	for ( int i = 0; i < m_pPowerups.Count(); i++ )
	{
		// Show indicators for active power-ups.
		bool bWasVisible = m_pPowerups[i]->IsVisible();
		bool bVisible = m_pPowerups[i]->m_flDuration != 0.0f;

		if ( bVisible != bWasVisible )
		{
			m_pPowerups[i]->SetVisible( bVisible );
			bUpdateLayout = true;
		}
	}

	if ( bUpdateLayout )
		InvalidateLayout( true );
}
