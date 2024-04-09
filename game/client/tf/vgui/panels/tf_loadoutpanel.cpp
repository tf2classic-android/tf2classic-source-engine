#include "cbase.h"
#include "tf_loadoutpanel.h"
#include "tf_mainmenu.h"
#include "controls/tf_advitembutton.h"
#include "controls/tf_advmodelpanel.h"
#include "tf_rgbpanel.h"
#include "basemodelpanel.h"
#include <vgui/ILocalize.h>
#include "script_parser.h"
#include "econ_item_view.h"

using namespace vgui;
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define PANEL_WIDE YRES(140)
#define PANEL_TALL YRES(70)
#define PANEL_X_OFFSET YRES(10)
#define PANEL_Y_OFFSET YRES(5)

static const char *pszClassModels[TF_CLASS_COUNT_ALL] =
{
	"",
	"models/player/scout.mdl",
	"models/player/sniper.mdl",
	"models/player/soldier.mdl",
	"models/player/demo.mdl",
	"models/player/medic.mdl",
	"models/player/heavy.mdl",
	"models/player/pyro.mdl",
	"models/player/spy.mdl",
	"models/player/engineer.mdl",
	"models/player/civilian.mdl",
	"models/player/merc_deathmatch.mdl"
};

static int g_aClassLoadoutSlots[TF_CLASS_COUNT_ALL][INVENTORY_ROWNUM] =
{
	// none
	{
		-1, -1, -1,
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
		-1,
		-1,
	},
	// mercenary
	{
		TF_LOADOUT_SLOT_MELEE,
		-1,
		-1,
	}
};

struct _WeaponData
{
	char szWorldModel[64];
	char iconActive[64];
	char iconInactive[64];
	char szPrintName[64];
	int iWeaponType;
	bool bHasTeamSkins;
};

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

class CTFWeaponScriptParser : public C_ScriptParser
{
public:
	DECLARE_CLASS_GAMEROOT( CTFWeaponScriptParser, C_ScriptParser );

	void Parse( KeyValues *pKeyValuesData, bool bWildcard, const char *szFileWithoutEXT )
	{
		_WeaponData sTemp;
		Q_strncpy( sTemp.szWorldModel, pKeyValuesData->GetString( "playermodel", "" ), sizeof( sTemp.szWorldModel ) );
		Q_strncpy( sTemp.szPrintName, pKeyValuesData->GetString( "printname", "" ), sizeof( sTemp.szPrintName ) );
		const char *pszWeaponType = pKeyValuesData->GetString( "WeaponType" );

		int iType = UTIL_StringFieldToInt( pszWeaponType, g_AnimSlots, TF_WPN_TYPE_COUNT );
		sTemp.iWeaponType = iType >= 0 ? iType : TF_WPN_TYPE_PRIMARY;

		sTemp.bHasTeamSkins = ( pKeyValuesData->GetBool( "HasTeamSkins_Worldmodel" ) );

		for ( KeyValues *pData = pKeyValuesData->GetFirstSubKey(); pData != NULL; pData = pData->GetNextKey() )
		{
			if ( !Q_stricmp( pData->GetName(), "TextureData" ) )
			{
				for ( KeyValues *pTextureData = pData->GetFirstSubKey(); pTextureData != NULL; pTextureData = pTextureData->GetNextKey() )
				{
					if ( !Q_stricmp( pTextureData->GetName(), "weapon" ) )
					{
						Q_strncpy( sTemp.iconInactive, pTextureData->GetString( "file", "" ), sizeof( sTemp.iconInactive ) );
					}
					if ( !Q_stricmp( pTextureData->GetName(), "weapon_s" ) )
					{
						Q_strncpy( sTemp.iconActive, pTextureData->GetString( "file", "" ), sizeof( sTemp.iconActive ) );
					}
				}
			}
		}
		m_WeaponInfoDatabase.Insert( szFileWithoutEXT, sTemp );
	};

	_WeaponData *GetTFWeaponInfo( const char *name )
	{
		int index = m_WeaponInfoDatabase.Find( name );

		if ( index != m_WeaponInfoDatabase.InvalidIndex() )
			return &m_WeaponInfoDatabase[index];

		return NULL;
	}

private:
	CUtlDict< _WeaponData, unsigned short > m_WeaponInfoDatabase;
};
CTFWeaponScriptParser g_TFWeaponScriptParser;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFLoadoutPanel::CTFLoadoutPanel( Panel *parent, const char *panelName ) : CTFDialogPanelBase( parent, panelName )
{
	m_pClassModelPanel = new CTFAdvModelPanel( this, "classmodelpanel" );
	m_pWeaponSetPanel = new CTFWeaponSetPanel( this, "weaponsetpanel" );
	m_pRGBPanel = new CTFRGBPanel( this, "rgbpanel" );

	// Make 5 weapon icons at first, we'll add more if we need to.
	for ( int i = 0; i < INVENTORY_ROWNUM; i++ )
	{
		for ( int j = 0; j < INVENTORY_COLNUM; j++ )
		{
			m_pWeaponIcons[i].AddToTail( new CTFItemButton( m_pWeaponSetPanel, "WeaponIcons", "DUK" ) );
		}
	}

	for ( int i = 0; i < INVENTORY_ROWNUM * 2; i++ )
	{
		m_pSlideButtons[i] = new CTFItemButton( m_pWeaponSetPanel, "SlideButton", "DUK" );
	}

	g_TFWeaponScriptParser.InitParser( "scripts/tf_weapon_*.txt", true, false );

	Init();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFLoadoutPanel::~CTFLoadoutPanel()
{
}

bool CTFLoadoutPanel::Init()
{
	BaseClass::Init();

	m_iCurrentClass = TF_CLASS_SCOUT;
	m_iCurrentSlot = TF_LOADOUT_SLOT_PRIMARY;

	memset( m_RawIDPos, 0, sizeof( m_RawIDPos ) );

	for ( int iClassIndex = 0; iClassIndex < TF_CLASS_COUNT_ALL; iClassIndex++ )
	{
		if ( pszClassModels[iClassIndex][0] != '\0' )
			modelinfo->FindOrLoadModel( pszClassModels[iClassIndex] );

		for ( int iSlot = 0; iSlot < TF_LOADOUT_SLOT_COUNT; iSlot++ )
		{
			for ( int iPreset = 0; iPreset < GetTFInventory()->NumWeapons( iClassIndex, (ETFLoadoutSlot)iSlot ); iPreset++ )
			{
				CEconItemView *pItem = GetTFInventory()->GetItem( iClassIndex, (ETFLoadoutSlot)iSlot, iPreset );

				if ( pItem )
				{
					const char *pszWeaponModel = GetWeaponModel( pItem->GetStaticData(), iClassIndex );

					if ( pszWeaponModel[0] != '\0' )
					{
						modelinfo->FindOrLoadModel( pszWeaponModel );
					}
				}
			}
		}
	}

	return true;
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
			SetupWeaponIcon( pWeaponButton, iSlot, iPreset );
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
}

void CTFLoadoutPanel::SetupWeaponIcon( CTFItemButton *pButton, int iSlot, int iPreset )
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
	m_iCurrentSlot = g_aClassLoadoutSlots[iClass][0];
	ResetRows();
	DefaultLayout();
};

void CTFLoadoutPanel::OnCommand( const char* command )
{
	if ( !Q_strcmp( command, "back" ) || ( !Q_strcmp( command, "vguicancel" ) ) )
	{
		Hide();
	}
	else if ( ( !Q_strcmp( command, "select_scout" ) ) || ( !Q_strcmp( command, "selectclass_scout" ) ) )
	{
		SetCurrentClass( TF_CLASS_SCOUT );
	}
	else if ( ( !Q_strcmp( command, "select_soldier" ) ) || ( !Q_strcmp( command, "selectclass_soldier" ) ) )
	{
		SetCurrentClass( TF_CLASS_SOLDIER );
	}
	else if ( ( !Q_strcmp( command, "select_pyro" ) ) || ( !Q_strcmp( command, "selectclass_pyro" ) ) )
	{
		SetCurrentClass( TF_CLASS_PYRO );
	}
	else if ( ( !Q_strcmp( command, "select_demoman" ) ) || ( !Q_strcmp( command, "selectclass_demoman") ) )
	{
		SetCurrentClass( TF_CLASS_DEMOMAN );
	}
	else if ( ( !Q_strcmp( command, "select_heavyweapons" ) ) || ( !Q_strcmp( command, "selectclass_heavy" ) ) )
	{
		SetCurrentClass( TF_CLASS_HEAVYWEAPONS );
	}
	else if ( ( !Q_strcmp( command, "select_engineer" ) ) || ( !Q_strcmp( command, "selectclass_engineer" ) ) )
	{
		SetCurrentClass( TF_CLASS_ENGINEER );
	}
	else if ( ( !Q_strcmp( command, "select_medic" ) ) || ( !Q_strcmp( command, "selectclass_medic" ) ))
	{
		SetCurrentClass( TF_CLASS_MEDIC );
	}
	else if ( ( !Q_strcmp( command, "select_sniper" ) ) || ( !Q_strcmp( command, "selectclass_sniper" ) ) )
	{
		SetCurrentClass( TF_CLASS_SNIPER );
	}
	else if ( ( !Q_strcmp( command, "select_spy" ) ) || ( !Q_strcmp( command, "selectclass_spy" ) ) )
	{
		SetCurrentClass( TF_CLASS_SPY );
	}
	else if ( ( !Q_strcmp( command, "select_civ" ) ) || ( !Q_strcmp( command, "selectclass_civilian" ) ) )
	{
		SetCurrentClass( TF_CLASS_CIVILIAN );
	}
	else if ( ( !Q_strcmp( command, "select_merc" ) ) || ( !Q_strcmp( command, "selectclass_mercenary" ) ) )
	{
		SetCurrentClass( TF_CLASS_MERCENARY );
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
					SetSlotAndPreset( iSlot, iWeapon );
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
				Q_snprintf( buffer, sizeof( buffer ), command + Q_strlen( szText ) );
				SideRow( atoi( buffer ), ( i == 0 ? -1 : 1 ) );
				return;
			}
		}

		BaseClass::OnCommand( command );
	}
}

void CTFLoadoutPanel::SetSlotAndPreset( int iSlot, int iPreset )
{
	SetCurrentSlot( iSlot );
	SetWeaponPreset( m_iCurrentClass, m_iCurrentSlot, iPreset );
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

	DefaultLayout();
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

int CTFLoadoutPanel::GetAnimSlot( CEconItemDefinition *pItemDef, int iClass )
{
	if ( !pItemDef )
		return -1;

	int iSlot = pItemDef->anim_slot;
	if ( iSlot == -1 )
	{
		// Fall back to script file data.
		const char *pszClassname = TranslateWeaponEntForClass( pItemDef->item_class, iClass );
		_WeaponData *pWeaponInfo = g_TFWeaponScriptParser.GetTFWeaponInfo( pszClassname );
		Assert( pWeaponInfo );

		iSlot = pWeaponInfo->iWeaponType;
	}

	return iSlot;
}

const char *CTFLoadoutPanel::GetWeaponModel( CEconItemDefinition *pItemDef, int iClass )
{
	if ( !pItemDef )
		return "";

	if ( pItemDef->act_as_wearable )
	{
		if ( pItemDef->model_player_per_class[iClass][0] != '\0' )
			return pItemDef->model_player_per_class[iClass];

		return pItemDef->model_player;
	}

	const char *pszModel = pItemDef->model_world;

	if ( pszModel[0] == '\0' && pItemDef->attach_to_hands == 1 )
	{
		pszModel = pItemDef->model_player;
	}

	return pszModel;
}

void CTFLoadoutPanel::UpdateModelWeapons( void )
{
	m_pClassModelPanel->ClearMergeMDLs();
	int iAnimationIndex = -1;

	// Get active weapon info.
	int iPreset = GetTFInventory()->GetWeaponPreset( m_iCurrentClass, (ETFLoadoutSlot)m_iCurrentSlot );
	CEconItemView *pActiveItem = GetTFInventory()->GetItem( m_iCurrentClass, (ETFLoadoutSlot)m_iCurrentSlot, iPreset );
	Assert( pActiveItem );

	if ( pActiveItem )
	{
		iAnimationIndex = GetAnimSlot( pActiveItem->GetStaticData(), m_iCurrentClass );

		// Can't be an active weapon without animation.
		if ( iAnimationIndex < 0 )
			pActiveItem = NULL;
	}

	for ( int iRow = 0; iRow < INVENTORY_ROWNUM; iRow++ )
	{
		int iSlot = g_aClassLoadoutSlots[m_iCurrentClass][iRow];
		if ( iSlot == -1 )
			continue;

		int iWeapon = GetTFInventory()->GetWeaponPreset( m_iCurrentClass, (ETFLoadoutSlot)iSlot );
		CEconItemView *pItem = GetTFInventory()->GetItem( m_iCurrentClass, (ETFLoadoutSlot)iSlot, iWeapon );
		CEconItemDefinition *pItemDef = pItem ? pItem->GetStaticData() : NULL;

		if ( !pItemDef )
			continue;

		if ( !pActiveItem )
		{
			// No active weapon, try this one.
			int iAnimSlot = GetAnimSlot( pItemDef, m_iCurrentClass );
			if ( iAnimSlot >= 0 )
			{
				pActiveItem = pItem;
				iAnimationIndex = iAnimSlot;
			}
		}

		// If this is the active weapon or it's a wearable, add its model.
		if ( pItem == pActiveItem || pItemDef->act_as_wearable )
		{
			const char *pszModel = GetWeaponModel( pItemDef, m_iCurrentClass );
			if ( pszModel[0] != '\0' )
			{
				int nSkin = 0;

				C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
				if ( pPlayer )
				{
					if ( pItem == pActiveItem )
					{
						const char *pszClassname = TranslateWeaponEntForClass( pItemDef->item_class, m_iCurrentClass );
						_WeaponData *pWeaponInfo = g_TFWeaponScriptParser.GetTFWeaponInfo( pszClassname );
						Assert( pWeaponInfo );

						if ( pWeaponInfo->bHasTeamSkins )
						{
							switch ( pPlayer->GetTeamNumber() )
							{
							case TF_TEAM_RED:
								nSkin = 0;
								break;
							case TF_TEAM_BLUE:
								nSkin = 1;
								break;
							case TF_TEAM_GREEN:
								nSkin = 2;
								break;
							case TF_TEAM_YELLOW:
								nSkin = 3;
								break;
							}
						}
					}
				}

				m_pClassModelPanel->SetMergeMDL( pszModel, NULL, nSkin );
			}
		}
	}

	// Set the animation.
	m_pClassModelPanel->SetAnimationIndex( iAnimationIndex >= 0 ? iAnimationIndex : TF_WPN_TYPE_PRIMARY );

	m_pClassModelPanel->Update();
}

void CTFLoadoutPanel::Show()
{
	BaseClass::Show();
	MAINMENU_ROOT->ShowPanel( SHADEBACKGROUND_MENU );

	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( pPlayer )
	{
		int iClass = pPlayer->m_Shared.GetDesiredPlayerClassIndex();
		if ( iClass >= TF_FIRST_NORMAL_CLASS && ( iClass <= TF_LAST_NORMAL_CLASS || iClass == TF_CLASS_MERCENARY ) )
			SetCurrentClass( pPlayer->m_Shared.GetDesiredPlayerClassIndex() );
	}
	DefaultLayout();
};

void CTFLoadoutPanel::Hide()
{
	BaseClass::Hide();
	MAINMENU_ROOT->HidePanel( SHADEBACKGROUND_MENU );
};

void CTFLoadoutPanel::SetModelClass( int iClass )
{
	int nSkin = 0;
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

	if ( iClass == TF_CLASS_MERCENARY )
	{
		nSkin = 8;
	}
	else if ( pPlayer )
	{
		switch ( pPlayer->GetTeamNumber() )
		{
		case TF_TEAM_RED:
			nSkin = 0;
			break;
		case TF_TEAM_BLUE:
			nSkin = 1;
			break;
		case TF_TEAM_GREEN:
			nSkin = 4;
			break;
		case TF_TEAM_YELLOW:
			nSkin = 5;
			break;
		}
	}

	m_pClassModelPanel->SetModelName( strdup( pszClassModels[iClass] ), nSkin );
}

void CTFLoadoutPanel::UpdateModelPanels()
{
	int iClassIndex = m_iCurrentClass;

	m_pClassModelPanel->SetVisible( true );
	m_pWeaponSetPanel->SetVisible( iClassIndex <= TF_LAST_NORMAL_CLASS );
	m_pRGBPanel->SetVisible( iClassIndex == TF_CLASS_MERCENARY );

	SetModelClass( iClassIndex );
	UpdateModelWeapons();
}

void CTFLoadoutPanel::DefaultLayout()
{
	BaseClass::DefaultLayout();

	UpdateModelPanels();

	int iClassIndex = m_iCurrentClass;
	SetDialogVariable( "classname", g_pVGuiLocalize->Find( g_aPlayerClassNames[iClassIndex] ) );

	// No point in updating all those buttons if they're not visible.
	if ( iClassIndex <= TF_LAST_NORMAL_CLASS )
	{
		for ( int iRow = 0; iRow < INVENTORY_ROWNUM; iRow++ )
		{
			int iColumnCount = 0;
			int iPos = m_RawIDPos[iRow];
			CTFButton *pSlideButtonL = m_pSlideButtons[iRow * 2];
			CTFButton *pSlideButtonR = m_pSlideButtons[( iRow * 2 ) + 1];
			int iSlot = g_aClassLoadoutSlots[iClassIndex][iRow];
			int iNumWeapons = GetTFInventory()->NumWeapons( iClassIndex, (ETFLoadoutSlot)iSlot );

			for ( int iColumn = 0; iColumn < m_pWeaponIcons[iRow].Count() || iColumn < iNumWeapons; iColumn++ )
			{
				if ( iColumn >= m_pWeaponIcons[iRow].Count() )
				{
					// Out of weapon icons, add a new one.
					CTFItemButton *pButton = new CTFItemButton( m_pWeaponSetPanel, "WeaponIcons", "DUK" );
					pButton->MakeReadyForUse();
					SetupWeaponIcon( pButton, iRow, iColumn );

					m_pWeaponIcons[iRow].AddToTail( pButton );
				}

				CTFItemButton *pWeaponButton = m_pWeaponIcons[iRow][iColumn];
				CEconItemView *pItem = NULL;

				if ( iSlot != -1 )
				{
					pItem = GetTFInventory()->GetItem( iClassIndex, (ETFLoadoutSlot)iSlot, iColumn );
				}

				CEconItemDefinition *pItemData = pItem ? pItem->GetStaticData() : NULL;

				if ( pItemData )
				{
					pWeaponButton->SetVisible( true );
					pWeaponButton->SetItemDefinition( pItemData );
					pWeaponButton->SetLoadoutSlot( iSlot, iColumn );

					int iWeaponPreset = GetTFInventory()->GetWeaponPreset( iClassIndex, (ETFLoadoutSlot)iSlot );

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

void CTFLoadoutPanel::GameLayout()
{
	BaseClass::GameLayout();

};

void CTFLoadoutPanel::SetWeaponPreset( int iClass, int iSlot, int iPreset )
{
	GetTFInventory()->SetWeaponPreset( iClass, (ETFLoadoutSlot)iSlot, iPreset );
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( pPlayer )
	{
		char szCmd[64];
		Q_snprintf( szCmd, sizeof( szCmd ), "weaponpresetclass %d %d %d", iClass, iSlot, iPreset ); //; tf2c_weaponset_show 0
		engine->ExecuteClientCmd( szCmd );
	}

	DefaultLayout();
}
