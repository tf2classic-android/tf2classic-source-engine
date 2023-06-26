#include "cbase.h"
#include "tf_advitembutton.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;


#define ADVITEMBUTTON_DEFAULT_BG "AdvRoundedButtonDisabled"
#define ADVITEMBUTTON_ARMED_BG "AdvRoundedButtonArmed"
#define ADVITEMBUTTON_DEPRESSED_BG "AdvRoundedButtonDepressed"

DECLARE_BUILD_FACTORY_DEFAULT_TEXT( CTFItemButton, CTFItemButton );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFItemButton::CTFItemButton( Panel *parent, const char *panelName, const char *text ) : CTFButton( parent, panelName, text )
{
	Init();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFItemButton::~CTFItemButton()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFItemButton::Init()
{
	m_pItemDefinition = NULL;
	m_iLoadoutSlot = TF_LOADOUT_SLOT_PRIMARY;

	// Set the borders.
	V_strncpy( m_szDefaultBG, ADVITEMBUTTON_DEFAULT_BG, sizeof( m_szDefaultBG ) );
	V_strncpy( m_szArmedBG, ADVITEMBUTTON_ARMED_BG, sizeof( m_szArmedBG ) );
	V_strncpy( m_szDepressedBG, ADVITEMBUTTON_DEPRESSED_BG, sizeof( m_szDepressedBG ) );
}

void CTFItemButton::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	// Don't want to darken weapon images.
	m_colorImageDefault = COLOR_WHITE;
	m_colorImageArmed = COLOR_WHITE;

	SetContentAlignment( Label::a_south );
	SetTextInset( 0, -10 );
}

void CTFItemButton::PerformLayout()
{
	BaseClass::PerformLayout();

	int inset = YRES( 45 );
	int wide = GetWide() - inset;

	SetImageSize( wide, wide );
	SetImageInset( inset / 2, -1 * wide / 5 );
}

// ---------------------------------------------------------------------------- -
// Purpose: 
//-----------------------------------------------------------------------------
void CTFItemButton::ShowToolTip( bool bShow )
{
	// Using a custom tooltip.
	if ( m_pItemDefinition )
	{
		if ( bShow )
		{
			MAINMENU_ROOT->ShowItemToolTip( m_pItemDefinition );
		}
		else
		{
			MAINMENU_ROOT->HideItemToolTip();
		}
	}
}

void CTFItemButton::SetItemDefinition( CEconItemDefinition *pItemData )
{
	m_pItemDefinition = pItemData;

	char szIcon[128];
	Q_snprintf( szIcon, sizeof( szIcon ), "../%s_large", pItemData->image_inventory );
	SetImage( szIcon );

	SetText( pItemData->GenerateLocalizedFullItemName() );

	// Set the weapon sound from schema.
	if ( pItemData->mouse_pressed_sound[0] != '\0' )
	{
		SetDepressedSound( pItemData->mouse_pressed_sound );
	}
	else
	{
		SetDepressedSound( NULL );
	}

	SetReleasedSound( NULL );
}

void CTFItemButton::SetLoadoutSlot( int iSlot, int iPreset )
{
	m_iLoadoutSlot = iSlot;

	char szCommand[64];
	Q_snprintf( szCommand, sizeof( szCommand ), "loadout %d %d", iSlot, iPreset );
	SetCommand( szCommand );
}
