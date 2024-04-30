#include "cbase.h"
#include "tf_advslider.h"
#include <vgui/IInput.h>
#include <vgui_controls/AnimationController.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;


DECLARE_BUILD_FACTORY( CTFSlider );

#define ADVSLIDER_BG		"AdvSlider"

#define ADVSLIDER_DEFAULT_BORDER	"TFFatLineBorder"
#define ADVSLIDER_ARMED_BORDER		"TFFatLineBorderOpaque"
#define ADVSLIDER_DEPRESSED_BORDER	"TFFatLineBorderRedBGOpaque"
//#define ADVSLIDER_DEFAULT_BORDER		"AdvRoundedButtonDefault"
//#define ADVSLIDER_ARMED_BORDER		"AdvRoundedButtonArmed"
//#define ADVSLIDER_DEPRESSED_BORDER	"AdvRoundedButtonDepressed"

//=============================================================================//
// CTFSlider
//=============================================================================//
CTFSlider::CTFSlider( vgui::Panel *parent, const char *panelName ) : EditablePanel( parent, panelName )
{
	m_pValueLabel = new Label( this, "ValueLabel", "0" );
	m_pBGBorder = new EditablePanel( this, "BackgroundPanel" );
	m_pBGBorder->SetSize( 100, 50 );

	m_pButton = new CTFScrollButton( m_pBGBorder, "SubButton", "" );
	m_pButton->SetSliderPanel( this );

	Init();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFSlider::~CTFSlider()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::Init()
{
	m_szFont[0] = '\0';

	m_flMinValue = 0.0f;
	m_flMaxValue = 100.0f;
	m_flValue = 0.0f;

	m_bVertical = false;
	m_bValueVisible = true;
	m_bShowFrac = false;

	AddActionSignalTarget( this );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );

	V_strcpy_safe( m_szFont, inResourceData->GetString( "font" ) );

	m_bVertical = inResourceData->GetBool( "vertical", false );
	m_bValueVisible = inResourceData->GetBool( "value_visible", true );

	float flMin = inResourceData->GetFloat( "rangeMin", 0.0f );
	float flMax = inResourceData->GetFloat( "rangeMax", 100.0f );

	m_bShowFrac = inResourceData->GetBool( "showfraction", false );

	SetRange( flMin, flMax );

	SetValue( m_flMinValue );

	InvalidateLayout( false, true );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	m_pValueLabel->SetFgColor( pScheme->GetColor( "TanLight", Color( 255, 255, 255, 255 ) ) );
	m_pValueLabel->SetFont( m_pButton->GetFont() );

	if ( m_szFont[0] != '\0' )
	{
		SetFont( pScheme->GetFont( m_szFont, IsProportional() ) );
	}
	else
	{
		SetFont( pScheme->GetFont( "Default", IsProportional() ) );
	}

	// Show the button above everything.
	m_pButton->SetZPos( 3 );

	m_pBGBorder->SetBorder( pScheme->GetBorder( ADVSLIDER_BG ) );
	m_pBGBorder->SetVisible( true );
	m_pBGBorder->SetZPos( 1 );

	m_pValueLabel->SetVisible( m_bValueVisible );
	m_pValueLabel->SetZPos( 2 );
	m_pValueLabel->SetContentAlignment( Label::a_center );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::PerformLayout()
{
	BaseClass::PerformLayout();

	int wide, tall;
	GetSize( wide, tall );

	int iShift = YRES( 20 );

	if ( !m_bVertical )
		m_pButton->SetSize( YRES( 10 ), tall );  //scroll wide
	else
		m_pButton->SetSize( m_pBGBorder->GetWide(), YRES( 10 ) );  //scroll wide	

	m_pBGBorder->SetBounds( 0, 0, wide - iShift, tall );
	m_pValueLabel->SetBounds( wide - iShift, 0, iShift, tall );

	// Recalculate nob position.
	SetValue( m_flValue );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::SetFont( HFont font )
{
	// Set the font on all elements.
	m_pButton->SetFont( font );
	m_pValueLabel->SetFont( font );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::SetFont( const char *pszFont )
{
	V_strcpy_safe( m_szFont, pszFont );
	InvalidateLayout( false, true );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::OnThink( void )
{
	BaseClass::OnThink();

	if ( IsEnabled() && m_pButton && m_pButton->GetState() == MOUSE_PRESSED )
	{
		// Calculate percentage based on cursor position relative to the border.
		int x, y;
		input()->GetCursorPosition( x, y );
		m_pBGBorder->ScreenToLocal( x, y );

		int borderWide, borderTall;
		m_pBGBorder->GetSize( borderWide, borderTall ); //scroll local pos

		float flPerc;
		if ( !m_bVertical )
			flPerc =(float)x / (float)borderWide;
		else
			flPerc = (float)y / (float)borderTall;

		m_flValue = RemapValClamped( flPerc, 0.0f, 1.0f, m_flMinValue, m_flMaxValue );

		SetPercentage( flPerc );

		SendSliderMovedMessage();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
float CTFSlider::GetValue()
{
	return m_flValue;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CTFSlider::GetFinalValue()
{
	static char szValue[32];

	if ( m_bShowFrac || m_flMaxValue <= 1.0f )
	{
		// Only show fractional part if it's 0 to 1 slider.
		V_sprintf_safe( szValue, "%.02f", m_flValue );
	}
	else
	{
		V_sprintf_safe( szValue, "%.f", m_flValue );
	}

	return szValue;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::SetValue( float flValue )
{
	// UNDONE: Breaks inverted sliders.
	//m_flValue = clamp( flValue, m_flMinValue, m_flMaxValue );
	m_flValue = flValue;

	float fPerc = RemapValClamped( flValue, m_flMinValue, m_flMaxValue, 0.0f, 1.0f );
	SetPercentage( fPerc, true );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::SetRange( float flMin, float flMax )
{
	m_flMinValue = flMin;
	m_flMaxValue = flMax;

	// Update nob position.
	SetValue( m_flValue );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::SetPercentage( float flPerc, bool bInstant /*= false*/ )
{
	// Position the scroll button based on percentage.
	int borderWide, borderTall;
	m_pBGBorder->GetSize( borderWide, borderTall );

	int scrollX, scrollY, scrollWide, scrollTall;
	m_pButton->GetBounds( scrollX, scrollY, scrollWide, scrollTall ); //scroll local pos

	float flPos;
	if ( !m_bVertical )
		flPos = clamp( flPerc * borderWide, 0, borderWide - scrollWide );
	else
		flPos = clamp( flPerc * borderTall, 0, borderTall - scrollTall );

	m_pValueLabel->SetText( GetFinalValue() );

	if ( bInstant )
	{
		// Just set the position directly, no animation.
		if ( !m_bVertical )
		{
			m_pButton->SetPos( (int)flPos, scrollY );
		}
		else
		{
			m_pButton->SetPos( scrollX, (int)flPos );
		}
	}
	else
	{
		if ( !m_bVertical )
		{
			GetAnimationController()->RunAnimationCommand( m_pButton, "xpos", flPos, 0.0f, 0.05f, vgui::AnimationController::INTERPOLATOR_LINEAR, NULL );
		}
		else
		{
			GetAnimationController()->RunAnimationCommand( m_pButton, "ypos", flPos, 0.0f, 0.05f, vgui::AnimationController::INTERPOLATOR_LINEAR, NULL );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Send a message to interested parties when the slider moves
//-----------------------------------------------------------------------------
void CTFSlider::SendSliderMovedMessage()
{
	// send a changed message
	KeyValues *pParams = new KeyValues( "SliderMoved", "position", m_flValue );
	pParams->SetPtr( "panel", this );
	PostActionSignal( pParams );
}

//-----------------------------------------------------------------------------
// Purpose: Send a message to interested parties when the user begins dragging the slider
//-----------------------------------------------------------------------------
void CTFSlider::SendSliderDragStartMessage()
{
	// send a message
	KeyValues *pParams = new KeyValues( "SliderDragStart", "position", m_flValue );
	pParams->SetPtr( "panel", this );
	PostActionSignal( pParams );
}

//-----------------------------------------------------------------------------
// Purpose: Send a message to interested parties when the user ends dragging the slider
//-----------------------------------------------------------------------------
void CTFSlider::SendSliderDragEndMessage()
{
	// send a message
	KeyValues *pParams = new KeyValues( "SliderDragEnd", "position", m_flValue );
	pParams->SetPtr( "panel", this );
	PostActionSignal( pParams );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::SetToolTip( const char *pszText )
{
	if ( m_pButton )
	{
		m_pButton->SetToolTip( pszText );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFSlider::ClampValue()
{
	float flValue = m_flValue; 
	if (m_flMinValue < m_flMaxValue)
	{
		flValue = clamp( flValue, m_flMinValue, m_flMaxValue );
	}
	else
	{
		flValue = clamp( flValue, m_flMinValue, m_flMaxValue );
	}
	SetValue(flValue);
}


//=============================================================================//
// CTFScrollButton
//=============================================================================//
CTFScrollButton::CTFScrollButton( vgui::Panel *parent, const char *panelName, const char *text ) : CTFButtonBase( parent, panelName, text )
{
	m_pSliderPanel = NULL;

	// Set default border.
	V_strcpy_safe( m_szDefaultBG, ADVSLIDER_DEFAULT_BORDER );
	V_strcpy_safe( m_szArmedBG, ADVSLIDER_ARMED_BORDER );
	V_strcpy_safe( m_szDepressedBG, ADVSLIDER_DEPRESSED_BORDER );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFScrollButton::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFScrollButton::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetArmedSound( "#ui/buttonrollover.wav" );
	SetDepressedSound( "#ui/buttonclick.wav" );
	SetReleasedSound( "#ui/buttonclickrelease.wav" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFScrollButton::PerformLayout()
{
	BaseClass::PerformLayout();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFScrollButton::OnCursorEntered()
{
	Button::OnCursorEntered();

	if ( m_iMouseState != MOUSE_ENTERED && m_iMouseState != MOUSE_PRESSED )
	{
		SetMouseEnteredState( MOUSE_ENTERED );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFScrollButton::OnCursorExited()
{
	Button::OnCursorExited();

	if ( m_iMouseState != MOUSE_EXITED && m_iMouseState != MOUSE_PRESSED )
	{
		SetMouseEnteredState( MOUSE_EXITED );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFScrollButton::OnMousePressed( vgui::MouseCode code )
{
	Button::OnMousePressed( code );

	if ( code == MOUSE_LEFT && m_iMouseState != MOUSE_PRESSED )
	{
		SetMouseEnteredState( MOUSE_PRESSED );

		if ( m_pSliderPanel )
		{
			m_pSliderPanel->SendSliderDragStartMessage();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFScrollButton::OnMouseReleased( vgui::MouseCode code )
{
	Button::OnMouseReleased( code );

	if ( code == MOUSE_LEFT && m_iMouseState == MOUSE_ENTERED )
	{
		SetMouseEnteredState( MOUSE_ENTERED );
	}
	else
	{
		SetMouseEnteredState( MOUSE_EXITED );
	}

	if ( m_pSliderPanel )
	{
		m_pSliderPanel->SendSliderDragEndMessage();
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFScrollButton::SetMouseEnteredState( MouseState flag )
{
	BaseClass::SetMouseEnteredState( flag );
}
