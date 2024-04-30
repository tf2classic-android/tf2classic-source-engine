#include "cbase.h"
#include "tf_advbutton.h"
#include <vgui_controls/AnimationController.h>
#include "panels/tf_dialogpanelbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;


DECLARE_BUILD_FACTORY_DEFAULT_TEXT( CTFButton, CTFButton );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFButton::CTFButton( Panel *parent, const char *panelName, const char *text ) : CTFButtonBase( parent, panelName, text )
{
	m_iXShift = 0;
	m_iYShift = 0;

	m_bGlowing = false;
	m_bAnimationIn = false;
	m_flActionThink = -1.0f;
	m_flAnimationThink = -1.0f;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFButton::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	m_pButtonImage->SetDrawColor( m_colorImageDefault );

	// Bah, let's not bother with proper sound overriding for now.
	SetArmedSound( "#ui/buttonrollover.wav" );
	SetDepressedSound( "#ui/buttonclick.wav" );
	SetReleasedSound( "#ui/buttonclickrelease.wav" );

	// Save the original text position for animations.
	GetTextInset( &m_iOrigX, &m_iOrigY );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFButton::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFButton::PerformLayout()
{
	BaseClass::PerformLayout();

	if ( m_pButtonImage )
	{
		// Set image color based on our state.
		if ( IsDepressed() )
		{
			m_pButtonImage->SetDrawColor( m_colorImageDepressed );
		}
		else if ( IsArmed() || IsSelected() )
		{
			m_pButtonImage->SetDrawColor( m_colorImageArmed );
		}
		else
		{
			m_pButtonImage->SetDrawColor( m_colorImageDefault );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFButton::OnThink()
{
	BaseClass::OnThink();

	if ( m_bGlowing && m_flAnimationThink < gpGlobals->curtime )
	{
		float m_fAlpha = ( m_bAnimationIn ? 50.0f : 255.0f );
		float m_fDelay = ( m_bAnimationIn ? 0.75f : 0.0f );
		float m_fDuration = ( m_bAnimationIn ? 0.15f : 0.25f );
		vgui::GetAnimationController()->RunAnimationCommand( this, "Alpha", m_fAlpha, m_fDelay, m_fDuration, vgui::AnimationController::INTERPOLATOR_LINEAR );
		m_bAnimationIn = !m_bAnimationIn;
		m_flAnimationThink = gpGlobals->curtime + 1.0f;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFButton::SetArmed( bool bState )
{
	BaseClass::SetArmed( bState );

	// Do animation if applicable.
	if ( m_iXShift )
	{
		int x = bState ? m_iOrigX + m_iXShift : m_iOrigX;
		GetAnimationController()->RunAnimationCommand( this, "textinsetx", x, 0.0f, 0.1f, AnimationController::INTERPOLATOR_LINEAR );
	}

	if ( m_iYShift )
	{
		int y = bState ? m_iOrigY + m_iYShift : m_iOrigY;
		GetAnimationController()->RunAnimationCommand( this, "textinsety", y, 0.0f, 0.1f, AnimationController::INTERPOLATOR_LINEAR );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFButton::DoClick( void )
{
	BaseClass::DoClick();

	// Send message to the tabs manager.
	PostActionSignal( new KeyValues( "ButtonPressed" ) );
}

//-----------------------------------------------------------------------------
// Purpose: Enable glowing effect.
//-----------------------------------------------------------------------------
void CTFButton::SetGlowing( bool Glowing )
{
	m_bGlowing = Glowing;

	if ( !m_bGlowing )
	{
		float m_fAlpha = 255.0f;
		float m_fDelay = 0.0f;
		float m_fDuration = 0.0f;
		vgui::GetAnimationController()->RunAnimationCommand( this, "Alpha", m_fAlpha, m_fDelay, m_fDuration, AnimationController::INTERPOLATOR_LINEAR );
	}
}
