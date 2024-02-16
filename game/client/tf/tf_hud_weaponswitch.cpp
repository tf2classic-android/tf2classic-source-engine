//========= Copyright © 1996-2007, Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"
#include "hudelement.h"
#include "iclientmode.h"
#include <vgui/ILocalize.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/Label.h>
#include "controls/tf_advbuttonbase.h"

#include "tf_hud_weaponswitch.h"
#include "c_tf_player.h"
#include "tf_hud_freezepanel.h"

#include "tf_gamerules.h"
#include "tf_weapon_parse.h"
#include "c_tf_playerresource.h"

using namespace vgui;

DECLARE_HUDELEMENT( CTFHudWeaponSwitch );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CItemModelPanel::CItemModelPanel( Panel *parent, const char* name ) : EditablePanel( parent, name )
{
	m_pWeapon = NULL;
	m_pWeaponName = new vgui::Label( this, "WeaponName", "text" );
	m_pSlotID = new vgui::Label( this, "SlotID", "0" );
	m_pWeaponImage = new vgui::ImagePanel( this, "WeaponImage" );
	m_iBorderStyle = -1;
	m_ID = -1;
	m_bOldStyleIcon = false;
}

void CItemModelPanel::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	m_pDefaultFont = pScheme->GetFont( "ItemFontNameSmallest", true );
	m_pSelectedFont = pScheme->GetFont( "ItemFontNameSmall", true );
	m_pNumberDefaultFont = pScheme->GetFont( "FontStorePromotion", true );
	m_pNumberSelectedFont = pScheme->GetFont( "HudFontSmall", true );
	m_pDefaultBorder = pScheme->GetBorder( "TFFatLineBorder" );
	m_pSelectedRedBorder = pScheme->GetBorder( "TFFatLineBorderRedBG" );
	m_pSelectedBlueBorder = pScheme->GetBorder( "TFFatLineBorderBlueBG" );

	SetPaintBorderEnabled( false );

	m_pWeaponImage->SetShouldScaleImage( true );
	m_pWeaponImage->SetZPos( -1 );

	m_pWeaponName->SetFont( m_pDefaultFont );
	m_pWeaponName->SetContentAlignment( Label::a_south );
	m_pWeaponName->SetCenterWrap( true );

	m_pSlotID->SetFont( m_pNumberDefaultFont );
	m_pSlotID->SetContentAlignment( Label::a_northeast );
	m_pSlotID->SetFgColor( pScheme->GetColor( "TanLight", Color( 255, 255, 255, 255 ) ) );

	m_iBorderStyle = -1;
	m_ID = -1;
	m_bModelOnly = false;
	m_bShowQuality = false;
	m_pItemDef = NULL;
}

void CItemModelPanel::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );

	bool bModelOnly = inResourceData->GetBool( "model_only", false );
	m_bModelOnly = bModelOnly;
	m_pWeaponName->SetVisible( !bModelOnly );
}

void CItemModelPanel::PerformLayout( void )
{
	// Set border.
	if ( m_iBorderStyle == -1 )
	{
		SetPaintBorderEnabled( false );
	}
	else if ( m_iBorderStyle == 0 )
	{
		SetPaintBorderEnabled( true );
		SetBorder( m_pDefaultBorder );
	}
	else if ( m_iBorderStyle == 1 )
	{
		int iTeam = GetLocalPlayerTeam();

		if ( iTeam == TF_TEAM_RED )
		{
			SetBorder( m_pSelectedRedBorder );
		}
		else
		{
			SetBorder( m_pSelectedBlueBorder );
		}
	}

	// Position image.
	if ( m_bOldStyleIcon )
	{
		m_pWeaponImage->SetBounds( YRES( 5 ), -1 * ( GetTall() / 10.0 ) + YRES( 5 ), ( GetWide() * 1.5 ) - YRES( 10 ), ( GetWide() * 0.75 ) - YRES( 10 ) );
	}
	else
	{
		m_pWeaponImage->SetBounds( YRES( 5 ), -1 * ( GetTall() / 5.0 ) + YRES( 5 ), GetWide() - YRES( 10 ), GetWide() - YRES( 10 ) );
	}

	// Position weapon name.
	m_pWeaponName->SetBounds( YRES( 5 ), GetTall() - YRES( 25 ), GetWide() - YRES( 5 ), YRES( 20 ) );
	m_pWeaponName->SetFont( m_iBorderStyle ? m_pSelectedFont : m_pDefaultFont );

	if ( m_pWeapon && !m_pWeapon->HasAnyAmmo() )
	{
		m_pWeaponName->SetText( "#TF_OUT_OF_AMMO" );
		m_pWeaponName->SetFgColor( GETSCHEME()->GetColor( "RedSolid", Color( 255, 255, 255, 255 ) ) );
	}
	else
	{
		if( m_bShowQuality && (m_pItemDef != NULL) )
		{
			m_pWeaponName->SetFgColor( GETSCHEME()->GetColor( EconQuality_GetColorString( m_pItemDef->item_quality ), Color( 255, 255, 255, 255 ) ) );
		}
		else
		{
			m_pWeaponName->SetFgColor( GETSCHEME()->GetColor( "TanLight", Color( 255, 255, 255, 255 ) ) );
		}
	}

	// Position slot number.
	m_pSlotID->SetBounds( 0, YRES( 5 ), GetWide() - YRES( 5 ), YRES( 10 ) );
	m_pSlotID->SetFont( m_iBorderStyle ? m_pNumberSelectedFont : m_pNumberDefaultFont );
}

void CItemModelPanel::SetWeapon( C_BaseCombatWeapon *pWeapon, int iBorderStyle, int ID )
{
	m_pWeapon = pWeapon;
	int iItemID = m_pWeapon->GetItemID();
	CEconItemDefinition *pItemDefinition = GetItemSchema()->GetItemDefinition( iItemID );
	m_pItemDef = pItemDefinition;
	m_ID = ID;
	m_iBorderStyle = iBorderStyle;

	char szImage[128] = { '\0' };

	if ( pItemDefinition )
	{
		V_snprintf( szImage, sizeof( szImage ), "../%s_large", pItemDefinition->image_inventory );

		m_bOldStyleIcon = false;
		m_pWeaponName->SetText( pItemDefinition->GenerateLocalizedFullItemName() );
	}
	else
	{
		const CHudTexture *pTexture = m_pWeapon->GetSpriteInactive(); // red team
		if ( pTexture )
		{
			V_snprintf( szImage, sizeof( szImage ), "../%s", pTexture->szTextureFile );
		}

		m_bOldStyleIcon = true;
		m_pWeaponName->SetText( m_pWeapon->GetWpnData().szPrintName );
	}

	m_pWeaponImage->SetImage( szImage );

	if ( ID != -1 )
	{
		char szSlotID[8];
		V_snprintf( szSlotID, sizeof( szSlotID ), "%d", m_ID + 1 );
		m_pSlotID->SetText( szSlotID );
	}
	else
	{
		m_pSlotID->SetText( "" );
	}

	InvalidateLayout( true );
}

void CItemModelPanel::SetWeapon( CEconItemDefinition *pItemDefinition, int iBorderStyle, int ID )
{
	m_pWeapon = NULL;
	m_pItemDef = pItemDefinition;
	m_ID = ID;
	m_iBorderStyle = iBorderStyle;

	wchar_t *pText = NULL;
	if ( pItemDefinition )
	{
		pText = pItemDefinition->GenerateLocalizedFullItemName();
		char szImage[128];
		Q_snprintf( szImage, sizeof( szImage ), "../%s_large", pItemDefinition->image_inventory );
		m_pWeaponImage->SetImage( szImage );
		m_bOldStyleIcon = false;
	}

	m_pWeaponName->SetText( pText );
	if ( ID != -1 )
	{
		char szSlotID[8];
		Q_snprintf( szSlotID, sizeof( szSlotID ), "%d", m_ID + 1 );
		m_pSlotID->SetText( szSlotID );
	}
	else
	{
		m_pSlotID->SetText( "" );
	}

	InvalidateLayout( true );
}



//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFHudWeaponSwitch::CTFHudWeaponSwitch( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudWeaponSwitch" )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	m_pItemDefFrom = NULL;
	m_pItemDefTo = NULL;

	m_pWeaponFrom = new CItemModelPanel( this, "WeaponFrom" );
	m_pWeaponTo = new CItemModelPanel( this, "WeaponTo" );

	ivgui()->AddTickSignal( GetVPanel() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTFHudWeaponSwitch::ShouldDraw( void )
{
	if ( IsTakingAFreezecamScreenshot() )
		return false;

	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer || !pPlayer->IsAlive() )
		return false;

	int iWeaponTo = pPlayer->m_Shared.GetDesiredWeaponIndex();
	if ( iWeaponTo == -1 )
		return false;

	return CHudElement::ShouldDraw();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudWeaponSwitch::SetVisible( bool bVisible )
{
	if ( bVisible )
	{
		const char *key = engine->Key_LookupBinding( "+use" );
		if ( !key )
		{
			key = "< not bound >";
		}
		SetDialogVariable( "use", key );
	}

	BaseClass::SetVisible( bVisible );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudWeaponSwitch::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	// load control settings...
	LoadControlSettings( "resource/UI/HudWeaponSwitch.res" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudWeaponSwitch::OnTick()
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer )
		return;

	int iWeaponTo = pPlayer->m_Shared.GetDesiredWeaponIndex();
	if ( iWeaponTo == -1 )
		return;

	CEconItemDefinition *pItemTo = GetItemSchema()->GetItemDefinition( iWeaponTo );
	if ( !pItemTo )
		return;

	if ( pItemTo != m_pItemDefTo )
	{
		m_pItemDefTo = pItemTo;
		m_pWeaponTo->SetWeapon( m_pItemDefTo );
	}

	int iSlot = pItemTo->GetLoadoutSlot( pPlayer->GetPlayerClass()->GetClassIndex() );
	C_EconEntity *pWeaponFrom = pPlayer->GetEntityForLoadoutSlot( iSlot );
	if ( !pWeaponFrom )
		return;

	CEconItemDefinition *pItemFrom = pWeaponFrom->GetItem()->GetStaticData();
	if ( !pItemFrom )
		return;

	if ( pItemFrom != m_pItemDefFrom )
	{
		m_pItemDefFrom = pItemFrom;
		m_pWeaponFrom->SetWeapon( m_pItemDefFrom );
	}
}
