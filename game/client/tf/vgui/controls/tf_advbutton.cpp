#include "cbase.h"
#include "tf_advbutton.h"
#include "vgui_controls/Frame.h"
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui/IInput.h>
#include "vgui_controls/Button.h"
#include "vgui_controls/ImagePanel.h"
#include "tf_controls.h"
#include <filesystem.h>
#include <vgui_controls/AnimationController.h>
#include "basemodelpanel.h"
#include "panels/tf_dialogpanelbase.h"
#include "inputsystem/iinputsystem.h"

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_BUILD_FACTORY_DEFAULT_TEXT( CTFButton, CTFButton );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFButton::CTFButton( Panel *parent, const char *panelName, const char *text ) : CTFButtonBase( parent, panelName, text )
{
	m_flXShift = 0.0;
	m_flYShift = 0.0;

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

	// Set default colors.
	SetDefaultColor( pScheme->GetColor( ADVBUTTON_DEFAULT_COLOR, COLOR_WHITE ), Color( 0, 0, 0, 0 ) );
	SetArmedColor( pScheme->GetColor( ADVBUTTON_ARMED_COLOR, COLOR_WHITE ), Color( 0, 0, 0, 0 ) );
	SetDepressedColor( pScheme->GetColor( ADVBUTTON_DEPRESSED_COLOR, COLOR_WHITE ), Color( 0, 0, 0, 0 ) );
	SetSelectedColor( pScheme->GetColor( ADVBUTTON_DEPRESSED_COLOR, COLOR_WHITE ), Color( 0, 0, 0, 0 ) );

	m_pButtonImage->SetDrawColor( m_colorImageDefault );

	// Bah, let's not bother with proper sound overriding for now.
	SetArmedSound( "ui/buttonrollover.wav" );
	SetDepressedSound( "ui/buttonclick.wav" );
	SetReleasedSound( "ui/buttonclickrelease.wav" );

	// Save the original position for animations.
	GetPos( m_iOrigX, m_iOrigY );

	// Add keyboard shortcut if it's contained in the string... We should really come up with a better way.
	if ( GetCommand() )
	{
		const char *pszCommand = GetCommand()->GetString( "command" );

		CTFDialogPanelBase *pParent = dynamic_cast<CTFDialogPanelBase *>( GetParent() );

		if ( pParent )
		{
			char sText[64];
			GetText( sText, sizeof( sText ) );
			if ( Q_strcmp( sText, "" ) )
			{
				char * pch;
				pch = strchr( sText, '&' );
				if ( pch != NULL )
				{
					int id = pch - sText + 1;
					//pch = strchr(pch + 1, '&');
					char* cTest = &sText[id];
					cTest[1] = '\0';
					pParent->AddShortcut( cTest, pszCommand );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFButton::ApplySettings( KeyValues *inResourceData )
{
	m_flXShift = inResourceData->GetFloat( "xshift", 0.0f );
	m_flYShift = inResourceData->GetFloat( "yshift", 0.0f );

	BaseClass::ApplySettings( inResourceData );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFButton::PerformLayout()
{
	BaseClass::PerformLayout();
	
	// Offset the text if required by image.
	if ( m_iImageWidth != 0 )
	{
		int iWidth = YRES( m_iImageWidth );
		int iHeight = iWidth;
		int iShift = ( GetTall() - iWidth ) / 2;
		int x = iShift * 2 + iWidth;

		SetTextInset( x, 0 );

		m_pButtonImage->SetPos( iShift, iShift );
		m_pButtonImage->SetWide( iWidth );
		m_pButtonImage->SetTall( iHeight );
	}

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
	bool bAnimation = ( ( m_flXShift == 0 && m_flYShift == 0 ) ? false : true );

	if ( bAnimation )
	{
		AnimationController::PublicValue_t p_AnimRest( m_iOrigX, m_iOrigY );
		AnimationController::PublicValue_t p_AnimHover( m_iOrigX + m_flXShift, m_iOrigY + m_flYShift );

		GetAnimationController()->RunAnimationCommand( this, "Position", bState ? p_AnimHover : p_AnimRest, 0.0f, 0.1f, AnimationController::INTERPOLATOR_LINEAR, NULL );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFButton::DoClick( void )
{
	BaseClass::DoClick();

	// Send message to the tabs manager.
	KeyValues *msg = new KeyValues( "ButtonPressed" );
	PostActionSignal( msg );
}

//-----------------------------------------------------------------------------
// Purpose: 
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
