#include "c_baseplayer.h"
#include "cbase.h"
#include "tf_loadoutpanel.h"
#include "cdll_util.h"
#include "controls/tf_advtabs.h"
#include "tf_inventory.h"
#include "tf_mainmenu.h"
#include "controls/tf_advitembutton.h"
#include "tf_rgbpanel.h"
#include "basemodelpanel.h"
#include <vgui/ILocalize.h>
#include "script_parser.h"
#include "econ_item_view.h"
#include "tf_shareddefs.h"
#include "tier3/tier3.h"
#include "util_shared.h"
#include <vgui_controls/AnimationController.h>

using namespace vgui;
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar tf_respawn_on_loadoutchanges( "tf_respawn_on_loadoutchanges", "1", FCVAR_ARCHIVE, "When set to 1, you will automatically respawn whenever you change loadouts inside a respawn zone." );

#define PANEL_WIDE YRES(140)
#define PANEL_TALL YRES(70)
#define PANEL_X_OFFSET YRES(10)
#define PANEL_Y_OFFSET YRES(5)

static ETFLoadoutSlot g_aClassLoadoutSlots[TF_CLASS_COUNT_ALL][INVENTORY_ROWNUM] =
{
	// none
	{
		TF_LOADOUT_SLOT_INVALID,
		TF_LOADOUT_SLOT_INVALID,
		TF_LOADOUT_SLOT_INVALID,
	},
	// scout
	{
		TF_LOADOUT_SLOT_PRIMARY,
		TF_LOADOUT_SLOT_SECONDARY,
		TF_LOADOUT_SLOT_MELEE,
	},
	// sniper
	{
		TF_LOADOUT_SLOT_PRIMARY,
		TF_LOADOUT_SLOT_SECONDARY,
		TF_LOADOUT_SLOT_MELEE,
	},
	// soldier
	{
		TF_LOADOUT_SLOT_PRIMARY,
		TF_LOADOUT_SLOT_SECONDARY,
		TF_LOADOUT_SLOT_MELEE,
	},
	// demo
	{
		TF_LOADOUT_SLOT_PRIMARY,
		TF_LOADOUT_SLOT_SECONDARY,
		TF_LOADOUT_SLOT_MELEE,
	},
	// medic
	{
		TF_LOADOUT_SLOT_PRIMARY,
		TF_LOADOUT_SLOT_SECONDARY,
		TF_LOADOUT_SLOT_MELEE,
	},
	// heavy
	{
		TF_LOADOUT_SLOT_PRIMARY,
		TF_LOADOUT_SLOT_SECONDARY,
		TF_LOADOUT_SLOT_MELEE,
	},
	// pyro
	{
		TF_LOADOUT_SLOT_PRIMARY,
		TF_LOADOUT_SLOT_SECONDARY,
		TF_LOADOUT_SLOT_MELEE,
	},
	// spy
	{
		TF_LOADOUT_SLOT_SECONDARY,
		TF_LOADOUT_SLOT_BUILDING,
		TF_LOADOUT_SLOT_MELEE
	},
	// engineer
	{
		TF_LOADOUT_SLOT_PRIMARY,
		TF_LOADOUT_SLOT_SECONDARY,
		TF_LOADOUT_SLOT_MELEE,
	},
	// civilian
	{
		TF_LOADOUT_SLOT_MELEE,
		TF_LOADOUT_SLOT_INVALID,
		TF_LOADOUT_SLOT_INVALID,
	},
	// mercenary
	{
		TF_LOADOUT_SLOT_MELEE,
		TF_LOADOUT_SLOT_INVALID,
		TF_LOADOUT_SLOT_INVALID,
	}
};

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFLoadoutPanel::CTFLoadoutPanel( Panel *parent, const char *panelName ) : CTFDialogPanelBase( parent, panelName )
{
	m_pClassModelPanel = new CTFPlayerModelPanel( this, "classmodelpanel" );
	m_pWeaponSetPanel = new CTFWeaponSetPanel( this, "weaponsetpanel" );
	m_pRGBPanel = new CTFRGBPanel( this, "rgbpanel" );
	m_pClassButtons = new CAdvTabs( this, "classselection" );

	// Make 5 weapon icons at first, we'll add more if we need to.
	for ( int i = 0; i < INVENTORY_ROWNUM; i++ )
	{
		for ( int j = 0; j < INVENTORY_COLNUM; j++ )
		{
			m_pWeaponIcons[i].AddToTail( new CTFItemButton( m_pWeaponSetPanel, "WeaponIcons", "" ) );
		}
	}

	for ( int i = 0; i < INVENTORY_ROWNUM * 2; i++ )
	{
		m_pSlideButtons[i] = new CTFItemButton( m_pWeaponSetPanel, "SlideButton", "" );
	}

	AddActionSignalTarget( this );
	
	m_iCurrentClass = TF_CLASS_SCOUT;
	
	memset( m_RawIDPos, 0, sizeof( m_RawIDPos ) );
	
	m_bLoadoutChanged = false;
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFLoadoutPanel::~CTFLoadoutPanel()
{
}

void CTFLoadoutPanel::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/UI/main_menu/LoadoutPanel.res" );

	for ( int iSlot = 0; iSlot < INVENTORY_ROWNUM; iSlot++ )
	{
		for ( int iPreset = 0; iPreset < m_pWeaponIcons[iSlot].Count(); iPreset++ )
		{
			CTFItemButton *pWeaponButton = m_pWeaponIcons[iSlot][iPreset];
			SetupWeaponIcon( pWeaponButton, (ETFLoadoutSlot)iSlot, iPreset );
		}

		CTFButton *pSlideButtonL = m_pSlideButtons[iSlot * 2];
		CTFButton *pSlideButtonR = m_pSlideButtons[( iSlot * 2 ) + 1];

		pSlideButtonL->SetSize( YRES( 10 ), PANEL_TALL );
		pSlideButtonL->SetPos( 0, iSlot * ( PANEL_TALL + PANEL_Y_OFFSET ) );
		pSlideButtonL->SetZPos( 1 );
		pSlideButtonL->SetText( "<" );
		pSlideButtonL->SetBordersByName( "AdvLeftButtonDefault", "AdvLeftButtonArmed", "AdvLeftButtonDepressed" );
		char szCommand[64];
		Q_snprintf( szCommand, sizeof( szCommand ), "SlideL%i", iSlot );
		pSlideButtonL->SetCommand( szCommand );

		pSlideButtonR->SetSize( YRES( 10 ), PANEL_TALL );
		pSlideButtonR->SetPos( m_pWeaponSetPanel->GetWide() - YRES( 10 ), iSlot * ( PANEL_TALL + PANEL_Y_OFFSET ) );
		pSlideButtonR->SetText( ">" );
		pSlideButtonR->SetZPos( 1 );
		pSlideButtonR->SetBordersByName( "AdvRightButtonDefault", "AdvRightButtonArmed", "AdvRightButtonDepressed" );
		Q_snprintf( szCommand, sizeof( szCommand ), "SlideR%i", iSlot );
		pSlideButtonR->SetCommand( szCommand );
	}
}

void CTFLoadoutPanel::PerformLayout()
{
	BaseClass::PerformLayout();
	
	int iClassIndex = m_iCurrentClass;
	SetDialogVariable( "classname", g_pVGuiLocalize->Find( g_aPlayerClassNames[iClassIndex] ) );
	
	m_pWeaponSetPanel->SetVisible( iClassIndex <= TF_CLASS_ENGINEER );
	m_pRGBPanel->SetVisible( iClassIndex == TF_CLASS_MERCENARY );
	
	m_pClassModelPanel->SetToPlayerClass( iClassIndex );
	m_pClassModelPanel->SetTeam( GetLocalPlayerTeam(), false, iClassIndex == TF_CLASS_MERCENARY );
	m_pClassModelPanel->LoadItems();
	m_pClassModelPanel->UseCvarsForTintColor( true );
	if( iClassIndex == TF_CLASS_MERCENARY )
		m_pClassModelPanel->HoldItemInSlot( TF_LOADOUT_SLOT_MELEE );
	
	UpdateWeaponButtons();
}

void CTFLoadoutPanel::SetupWeaponIcon( CTFItemButton *pButton, ETFLoadoutSlot iSlot, int iPreset )
{
	pButton->SetSize( PANEL_WIDE, PANEL_TALL );
	pButton->SetPos( ( iPreset - m_RawIDPos[iSlot] ) * ( PANEL_WIDE + PANEL_X_OFFSET ), iSlot * ( PANEL_TALL + PANEL_Y_OFFSET ) );
	pButton->SetLoadoutSlot( iSlot, iPreset );
}

void CTFLoadoutPanel::SetCurrentClass( int iClass )
{
	if ( m_iCurrentClass == iClass )
		return;

	m_iCurrentClass = iClass;
	ResetRows();
	InvalidateLayout( false, false );
};

void CTFLoadoutPanel::OnCommand( const char* command )
{
	if ( !Q_strcmp( command, "back" ) || ( !Q_strcmp( command, "vguicancel" ) ) )
	{
		Hide();
	}
	else if ( !V_strnicmp( command, "selectclass_", 12 ) )
	{
		int iClass = UTIL_StringFieldToInt( command + 12, g_aPlayerClassNames_NonLocalized, TF_CLASS_COUNT_ALL );
		if( iClass != -1 && iClass != m_iCurrentClass )
		{
			SetCurrentClass( iClass );
		}
	}
	else
	{
		char buffer[64];
		const char* szText;
		char strText[40];

		if ( !Q_strncmp( command, "loadout", 7 ) )
		{
			const char *sChar = strchr( command, ' ' );
			if ( sChar )
			{
				int iSlot = atoi( sChar + 1 );
				sChar = strchr( sChar + 1, ' ' );
				if ( sChar )
				{
					int iWeapon = atoi( sChar + 1 );
					SetWeaponPreset( m_iCurrentClass, iSlot, iWeapon );
				}
			}
			return;
		}

		for ( int i = 0; i < 2; i++ )
		{
			szText = ( i == 0 ? "SlideL" : "SlideR" );
			Q_strncpy( strText, command, Q_strlen( szText ) + 1 );
			if ( !Q_strcmp( strText, szText ) )
			{
				Q_snprintf( buffer, sizeof( buffer ), "%s", command + Q_strlen( szText ) );
				SideRow( atoi( buffer ), ( i == 0 ? -1 : 1 ) );
				return;
			}
		}

		BaseClass::OnCommand( command );
	}
}

void CTFLoadoutPanel::SideRow( int iRow, int iDir )
{
	m_RawIDPos[iRow] += iDir;

	for ( int iPreset = 0; iPreset < m_pWeaponIcons[iRow].Count(); iPreset++ )
	{
		CTFItemButton *m_pWeaponButton = m_pWeaponIcons[iRow][iPreset];
		int _x, _y;
		m_pWeaponButton->GetPos( _x, _y );
		int x = ( iPreset - m_RawIDPos[iRow] ) * ( PANEL_WIDE + PANEL_X_OFFSET );
		AnimationController::PublicValue_t p_AnimHover( x, _y );
		vgui::GetAnimationController()->RunAnimationCommand( m_pWeaponButton, "Position", p_AnimHover, 0.0f, 0.1f, vgui::AnimationController::INTERPOLATOR_LINEAR, NULL );
	}

	UpdateWeaponButtons();
}

void CTFLoadoutPanel::ResetRows()
{
	for ( int iSlot = 0; iSlot < INVENTORY_ROWNUM; iSlot++ )
	{
		m_RawIDPos[iSlot] = 0;
		for ( int iPreset = 0; iPreset < m_pWeaponIcons[iSlot].Count(); iPreset++ )
		{
			CTFItemButton *m_pWeaponButton = m_pWeaponIcons[iSlot][iPreset];
			m_pWeaponButton->SetPos( iPreset * ( PANEL_WIDE + PANEL_X_OFFSET ), iSlot * ( PANEL_TALL + PANEL_Y_OFFSET ) );
		}
	}
}

void CTFLoadoutPanel::Show()
{
	BaseClass::Show();
	m_bLoadoutChanged = false;

	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( pPlayer )
	{
		int iClass = pPlayer->m_Shared.GetDesiredPlayerClassIndex();
		if ( iClass >= TF_FIRST_NORMAL_CLASS && ( iClass <= TF_LAST_NORMAL_CLASS || iClass == TF_CLASS_MERCENARY ) )
			SetCurrentClass( pPlayer->m_Shared.GetDesiredPlayerClassIndex() );
	}
	
	InvalidateLayout( false, false );
}

void CTFLoadoutPanel::Hide()
{
	if( m_bLoadoutChanged )
	{
		m_bLoadoutChanged = false;
		if( C_BasePlayer::GetLocalPlayer() )
		{
			if( tf_respawn_on_loadoutchanges.GetBool() )
			{
				engine->ExecuteClientCmd( "loadoutchanged" );
			}
		}
	}
	// TODO: send event to panel_class
	BaseClass::Hide();
}

void CTFLoadoutPanel::UpdateWeaponButtons()
{
	// No point in updating all those buttons if they're not visible.
	if ( m_iCurrentClass <= TF_LAST_NORMAL_CLASS )
	{
		for ( int iRow = 0; iRow < INVENTORY_ROWNUM; iRow++ )
		{
			int iColumnCount = 0;
			int iPos = m_RawIDPos[iRow];
			CTFButton *pSlideButtonL = m_pSlideButtons[iRow * 2];
			CTFButton *pSlideButtonR = m_pSlideButtons[( iRow * 2 ) + 1];
			ETFLoadoutSlot iSlot = g_aClassLoadoutSlots[m_iCurrentClass][iRow];
			int iNumWeapons = GetTFInventory()->NumWeapons( m_iCurrentClass, (ETFLoadoutSlot)iSlot );

			for ( int iColumn = 0; iColumn < m_pWeaponIcons[iRow].Count() || iColumn < iNumWeapons; iColumn++ )
			{
				if ( iColumn >= m_pWeaponIcons[iRow].Count() )
				{
					// Out of weapon icons, add a new one.
					CTFItemButton *pButton = new CTFItemButton( m_pWeaponSetPanel, "WeaponIcons", "" );
					pButton->MakeReadyForUse();
					SetupWeaponIcon( pButton, (ETFLoadoutSlot)iRow, iColumn );

					m_pWeaponIcons[iRow].AddToTail( pButton );
				}

				CTFItemButton *pWeaponButton = m_pWeaponIcons[iRow][iColumn];
				CEconItemView *pItem = NULL;

				if ( iSlot != -1 )
				{
					pItem = GetTFInventory()->GetItem( m_iCurrentClass, (ETFLoadoutSlot)iSlot, iColumn );
				}

				CEconItemDefinition *pItemData = pItem ? pItem->GetStaticData() : NULL;

				if ( pItemData )
				{
					pWeaponButton->SetVisible( true );
					pWeaponButton->SetItem( pItem );
					pWeaponButton->SetLoadoutSlot( iSlot, iColumn );

					int iWeaponPreset = GetTFInventory()->GetWeaponPreset( m_iCurrentClass, (ETFLoadoutSlot)iSlot );

					pWeaponButton->SetSelected( ( iColumn == iWeaponPreset ) );

					iColumnCount++;
				}
				else
				{
					pWeaponButton->SetVisible( false );
				}
			}

			if ( iColumnCount > 2 )
			{
				if ( iPos == 0 )	//left
				{
					pSlideButtonL->SetVisible( false );
					pSlideButtonR->SetVisible( true );
				}
				else if ( iPos == iColumnCount - 2 )	//right
				{
					pSlideButtonL->SetVisible( true );
					pSlideButtonR->SetVisible( false );
				}
				else  //middle
				{
					pSlideButtonL->SetVisible( true );
					pSlideButtonR->SetVisible( true );
				}
			}
			else
			{
				pSlideButtonL->SetVisible( false );
				pSlideButtonR->SetVisible( false );
			}
		}
	}
};

void CTFLoadoutPanel::SetWeaponPreset( int iClass, int iSlot, int iPreset )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( pPlayer )
	{
		if( pPlayer->IsPlayerClass( iClass ) )
		{
			int iDefaultPreset = GetTFInventory()->GetWeaponPreset( iClass, (ETFLoadoutSlot)iSlot );
			if( iPreset != iDefaultPreset )
				m_bLoadoutChanged = true;
		}
		
		char szCmd[64];
		Q_snprintf( szCmd, sizeof( szCmd ), "weaponpresetclass %d %d %d", iClass, iSlot, iPreset ); //; tf2c_weaponset_show 0
		engine->ExecuteClientCmd( szCmd );
	}
	
	GetTFInventory()->SetWeaponPreset( iClass, (ETFLoadoutSlot)iSlot, iPreset );
	
	m_pClassModelPanel->LoadItems( (ETFLoadoutSlot)iSlot );

	UpdateWeaponButtons();
}

void CTFLoadoutPanel::OnControlModified( vgui::Panel *panel )
{
	if( panel == m_pRGBPanel && panel->IsVisible() )
	{
		m_pClassModelPanel->EmitSpawnEffect( m_pRGBPanel->GetSpawnEffect() );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFWeaponSetPanel::CTFWeaponSetPanel( vgui::Panel* parent, const char *panelName ) : EditablePanel( parent, panelName )
{
}

void CTFWeaponSetPanel::OnCommand( const char* command )
{
	GetParent()->OnCommand( command );
}
