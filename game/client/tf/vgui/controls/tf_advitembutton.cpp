#include "cbase.h"
#include "tf_advitembutton.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "econ_item_schema.h"
#include "tf_hud_weaponswitch.h"
#include "tf_shareddefs.h"
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
	m_pItemPanel = new CItemModelPanel( this, "ModelPanel" );
	m_pItem = NULL;
	m_iLoadoutSlot = TF_LOADOUT_SLOT_PRIMARY;
	
	// Set the borders.
	V_strncpy( m_szDefaultBG, ADVITEMBUTTON_DEFAULT_BG, sizeof( m_szDefaultBG ) );
	V_strncpy( m_szArmedBG, ADVITEMBUTTON_ARMED_BG, sizeof( m_szArmedBG ) );
	V_strncpy( m_szDepressedBG, ADVITEMBUTTON_DEPRESSED_BG, sizeof( m_szDepressedBG ) );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFItemButton::~CTFItemButton()
{
}

void CTFItemButton::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	// Don't want to darken weapon images.
	m_colorImageDefault = COLOR_WHITE;
	m_colorImageArmed = COLOR_WHITE;

	SetContentAlignment( Label::a_south );
	SetTextInset( 0, -10 );

	m_pItemPanel->SetMouseInputEnabled( false );
	m_pItemPanel->SetShowQuality( true );
}

void CTFItemButton::PerformLayout()
{
	BaseClass::PerformLayout();

	m_pItemPanel->SetBounds( 0, 0, GetWide(), GetTall() );
}

// ---------------------------------------------------------------------------- -
// Purpose: 
//-----------------------------------------------------------------------------
void CTFItemButton::ShowToolTip( bool bShow )
{
	// Using a custom tooltip.
	if ( m_pItem )
	{
		if ( bShow )
		{
			CEconItemDefinition *pItemDef = m_pItem->GetStaticData();
			if( !pItemDef )
				return;
			guiroot->ShowItemToolTip( pItemDef );
		}
		else
		{
			guiroot->HideItemToolTip();
		}
	}
}

void CTFItemButton::SetItem( CEconItemView *pItem )
{
	if( !pItem || !pItem->GetStaticData() )
		return;
		
	CEconItemDefinition *pItemDef = pItem->GetStaticData();
	if( !pItemDef )
		return;
	
	m_pItem = pItem;
	m_pItemPanel->SetWeapon( pItemDef );

	// Set the weapon sound from schema.
	if ( pItemDef->mouse_pressed_sound[0] != '\0' )
	{
		SetDepressedSound( pItemDef->mouse_pressed_sound );
	}
	else
	{
		SetDepressedSound( NULL );
	}

	SetReleasedSound( NULL );
}

void CTFItemButton::SetLoadoutSlot( ETFLoadoutSlot iSlot, int iPreset )
{
	m_iLoadoutSlot = iSlot;

	char szCommand[64];
	Q_snprintf( szCommand, sizeof( szCommand ), "loadout %d %d", iSlot, iPreset );
	SetCommand( szCommand );
}
