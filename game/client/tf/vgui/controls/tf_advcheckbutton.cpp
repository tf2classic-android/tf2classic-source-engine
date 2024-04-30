#include "cbase.h"
#include "tf_advcheckbutton.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;


#define ADVCHECKBUTTON_DEFAULT_BG		"AdvCheckButtonDefault"
#define ADVCHECKBUTTON_ARMED_BG			"AdvCheckButtonArmed"
#define ADVCHECKBUTTON_DEPRESSED_BG		"AdvCheckButtonDepressed"
#define DEFAULT_CHECKIMAGE				"main_menu/glyph_close_x"

DECLARE_BUILD_FACTORY_DEFAULT_TEXT( CTFCheckButton, CTFCheckButton );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFCheckButton::CTFCheckButton( Panel *parent, const char *panelName, const char *text ) : CTFButtonBase( parent, panelName, text )
{
	m_pCheckImage = new ImagePanel( this, "SubCheckImage" );
	m_pBGBorder = new EditablePanel( this, "BackgroundPanel" );

	m_bBorderVisible = false;
	m_bChecked = false;
	m_bInverted = false;

	// Set default border.
	V_strcpy_safe( m_szDefaultBG, ADVCHECKBUTTON_DEFAULT_BG );
	V_strcpy_safe( m_szArmedBG, ADVCHECKBUTTON_ARMED_BG );
	V_strcpy_safe( m_szDepressedBG, ADVCHECKBUTTON_DEPRESSED_BG );
}

CTFCheckButton::~CTFCheckButton()
{
	m_pCheckImage->DeletePanel();
	m_pBGBorder->DeletePanel();
}

void CTFCheckButton::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );

	m_bInverted = inResourceData->GetBool( "inverted" );

	KeyValues *pCheckImageKey = inResourceData->FindKey( "SubCheckImage" );

	if ( pCheckImageKey )
	{
		if ( m_pCheckImage )
			m_pCheckImage->ApplySettings( pCheckImageKey );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCheckButton::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetArmedSound( "#ui/buttonrollover.wav" );
	SetDepressedSound( "#ui/buttonclick.wav" );
	SetReleasedSound( "#ui/buttonclickrelease.wav" );

	// Show it above checkbox BG.
	m_pButtonImage->SetZPos( 1 );

	if ( m_pCheckImage->GetImage() == NULL )
	{
		// Set default check image if we're not overriding it.
		m_pCheckImage->SetImage( DEFAULT_CHECKIMAGE );
	}

	m_pCheckImage->SetMouseInputEnabled( false );
	m_pCheckImage->SetDrawColor( m_colorImageDefault );
	m_pCheckImage->SetShouldScaleImage( true );
	m_pCheckImage->SetScaleAmount( 0.0f );

	// Show check image above everything.
	m_pCheckImage->SetZPos( 2 );

	// The actual button takes up the whole panel width so it's easier to press but we want to only show the checkbox border.
	m_pBGBorder->SetMouseInputEnabled( false );
	m_pBGBorder->SetBorder( _defaultBorder );
	m_pBGBorder->SetVisible( true );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCheckButton::PerformLayout()
{
	BaseClass::PerformLayout();
	
	int wide, tall;
	GetSize( wide, tall );

	// Position check mark and box at the right side.
	m_pCheckImage->SetBounds( wide - tall, 0, tall, tall );
	m_pBGBorder->SetBounds( wide - tall, 0, tall, tall );

	// If it's inverted then we want to show the check mark when the box is off.
	m_pCheckImage->SetVisible( m_bInverted ? !IsChecked() : IsChecked() );

	// Set color and border based on our state.
	if ( IsDepressed() )
	{
		m_pButtonImage->SetDrawColor( m_colorImageDepressed );
		m_pCheckImage->SetDrawColor( m_colorImageDepressed );
		m_pBGBorder->SetBorder( _depressedBorder );
	}
	else if ( IsArmed() )
	{
		m_pButtonImage->SetDrawColor( m_colorImageArmed );
		m_pCheckImage->SetDrawColor( m_colorImageArmed );
		m_pBGBorder->SetBorder( _armedBorder );
	}
	else if ( IsSelected() )
	{
		m_pButtonImage->SetDrawColor( m_colorImageDepressed );
		m_pCheckImage->SetDrawColor( m_colorImageDepressed );
		m_pBGBorder->SetBorder( _depressedBorder );
	}
	else
	{
		m_pButtonImage->SetDrawColor( m_colorImageDefault );
		m_pCheckImage->SetDrawColor( m_colorImageDefault );
		m_pBGBorder->SetBorder( _defaultBorder );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCheckButton::DoClick( void )
{
	// Base class handling of "selected" flag is kind of finicky so we're using our own value.
	SetChecked( !IsChecked() );

	BaseClass::DoClick();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCheckButton::OnCheckButtonChecked( Panel *panel )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCheckButton::SetChecked( bool bState )
{
	m_bChecked = bState;

	// send a message saying we've been checked
	KeyValues *msg = new KeyValues( "CheckButtonChecked", "state", (int)bState );
	PostActionSignal( msg );

	InvalidateLayout();
}
