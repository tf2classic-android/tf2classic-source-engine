#include "cbase.h"
#include "tf_mainmenu.h"

#include "panels/tf_mainmenupanel.h"
#include "panels/tf_pausemenupanel.h"
#include "panels/tf_backgroundpanel.h"
#include "panels/tf_loadoutpanel.h"
#include "panels/tf_notificationpanel.h"
#include "panels/tf_shadebackgroundpanel.h"
#include "panels/tf_optionsdialog.h"
#include "panels/tf_quitdialogpanel.h"
#include "panels/tf_statsummarydialog.h"
#include "panels/tf_tooltippanel.h"
#include "panels/tf_createserverdialog.h"
#include "panels/tf_itemtooltippanel.h"
#include "engine/IEngineSound.h"
#include "tf_hud_statpanel.h"
#include "tf_notificationmanager.h"
#include "tf_hud_statpanel.h"
#include "ienginevgui.h"
#include <vgui_controls/AnimationController.h>
#include <vgui/ISurface.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

CTFMainMenu *guiroot = NULL;

CON_COMMAND( tf2c_mainmenu_reload, "Reload Main Menu" )
{
	if ( guiroot )
	{
		guiroot->InvalidatePanelsLayout( true, true );
	}
}

CON_COMMAND( showloadout, "Show loadout screen (new)" )
{
	if ( !guiroot )
		return;

	engine->ClientCmd( "gameui_activate" );
	guiroot->ShowPanel( LOADOUT_MENU, true );
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFMainMenu::CTFMainMenu() : EditablePanel( NULL, "MainMenu" )
{
	guiroot = this;

	SetParent( enginevgui->GetPanel( PANEL_GAMEUIDLL ) );
	SetScheme( scheme()->LoadSchemeFromFile( "resource/ClientScheme_tf2c.res", "ClientScheme_tf2c" ) );

	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );

	SetDragEnabled( false );
	SetShowDragHelper( false );
	SetProportional( true );
	SetVisible( true );

	int width, height;
	surface()->GetScreenSize( width, height );
	SetSize( width, height );
	SetPos( 0, 0 );

	memset( m_pPanels, 0, sizeof( m_pPanels ) );
	AddMenuPanel( new CTFMainMenuPanel( this, "CTFMainMenuPanel" ), MAIN_MENU );
	AddMenuPanel( new CTFPauseMenuPanel( this, "CTFPauseMenuPanel" ), PAUSE_MENU );
	AddMenuPanel( new CTFBackgroundPanel( this, "CTFBackgroundPanel" ), BACKGROUND_MENU );
	AddMenuPanel( new CTFLoadoutPanel( this, "CTFLoadoutPanel" ), LOADOUT_MENU );
	AddMenuPanel(new CTFNotificationPanel(this, "CTFNotificationPanel"), NOTIFICATION_MENU);
	AddMenuPanel( new CTFShadeBackgroundPanel( this, "CTFShadeBackgroundPanel" ), SHADEBACKGROUND_MENU );
	AddMenuPanel( new CTFQuitDialogPanel( this, "CTFQuitDialogPanel" ), QUIT_MENU );
	AddMenuPanel( new CTFOptionsDialog( this, "CTFOptionsDialog" ), OPTIONSDIALOG_MENU );
	AddMenuPanel( new CTFCreateServerDialog( this, "CTFCreateServerDialog" ), CREATESERVER_MENU );
	AddMenuPanel(new CTFStatsSummaryDialog(this, "CTFStatsSummaryDialog"), STATSUMMARY_MENU);
	AddMenuPanel( new CTFToolTipPanel( this, "CTFToolTipPanel" ), TOOLTIP_MENU );
	AddMenuPanel(new CTFItemToolTipPanel(this, "CTFItemToolTipPanel"), ITEMTOOLTIP_MENU);

	m_iMainMenuStatus = TFMAINMENU_STATUS_UNDEFINED;
	m_iUpdateLayout = 1;

	ivgui()->AddTickSignal( GetVPanel() );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFMainMenu::~CTFMainMenu()
{
	guiroot = NULL;
}

void CTFMainMenu::AddMenuPanel( CTFMenuPanelBase *pPanel, int iPanel )
{
	m_pPanels[iPanel] = pPanel;
	pPanel->SetZPos( iPanel );
	pPanel->SetVisible( false );
}

CTFMenuPanelBase* CTFMainMenu::GetMenuPanel( int iPanel )
{
	return m_pPanels[iPanel];
}

CTFMenuPanelBase* CTFMainMenu::GetMenuPanel( const char *name )
{
	for ( int i = FIRST_MENU; i < COUNT_MENU; i++ )
	{
		CTFMenuPanelBase* pMenu = GetMenuPanel( i );
		if ( pMenu && ( Q_strcmp( pMenu->GetName(), name ) == 0 ) )
		{
			return pMenu;
		}
	}
	return NULL;
}

void CTFMainMenu::ShowPanel( MenuPanel iPanel, bool m_bShowSingle /*= false*/ )
{
	GetMenuPanel( iPanel )->SetShowSingle( m_bShowSingle );
	GetMenuPanel( iPanel )->Show();
	if ( m_bShowSingle )
	{
		GetMenuPanel( guiroot->GetCurrentMainMenu() )->Hide();
	}
}

void CTFMainMenu::HidePanel( MenuPanel iPanel )
{
	GetMenuPanel( iPanel )->Hide();
}

void CTFMainMenu::InvalidatePanelsLayout( bool layoutNow, bool reloadScheme )
{
	for ( int i = FIRST_MENU; i < COUNT_MENU; i++ )
	{
		if ( GetMenuPanel( i ) )
		{
			bool bVisible = GetMenuPanel( i )->IsVisible();
			GetMenuPanel( i )->InvalidateLayout( layoutNow, reloadScheme );
			GetMenuPanel( i )->SetVisible( bVisible );
		}
	}
	
	UpdateCurrentMainMenu();
}

void CTFMainMenu::LaunchInvalidatePanelsLayout()
{
	m_iUpdateLayout = 4;
}

void CTFMainMenu::OnTick()
{
	// Don't draw during loading.
	SetVisible( !engine->IsDrawingLoadingImage() );

	int iStatus;

	// HACK to get rid of the main menu after changing the game resolution -DAN_H
	if (GetMenuPanel(OPTIONSDIALOG_MENU)->IsVisible())
	{
		HidePanel( MAIN_MENU );
		ShowPanel( SHADEBACKGROUND_MENU );
	}

	const char *levelName = engine->GetLevelName();
	if ( levelName && levelName[0] )
	{
		iStatus = engine->IsLevelMainMenuBackground() ? TFMAINMENU_STATUS_BACKGROUNDMAP : TFMAINMENU_STATUS_INGAME;
	}
	else
	{
		iStatus = TFMAINMENU_STATUS_MENU;
	}

	if ( iStatus != m_iMainMenuStatus )
	{
		m_iMainMenuStatus = iStatus;
		UpdateCurrentMainMenu();
	}

	if ( m_iUpdateLayout > 0 )
	{
		m_iUpdateLayout--;
		if ( !m_iUpdateLayout )
		{
			InvalidatePanelsLayout( true, true );
		}
	}
}

bool CTFMainMenu::IsInLevel()
{
	const char *levelName = engine->GetLevelName();
	if ( levelName && levelName[0] && !engine->IsLevelMainMenuBackground() )
	{
		return true;
	}
	return false;
}

bool CTFMainMenu::IsInBackgroundLevel()
{
	const char *levelName = engine->GetLevelName();
	if ( levelName && levelName[0] && engine->IsLevelMainMenuBackground() )
	{
		return true;
	}
	return false;
}

void CTFMainMenu::UpdateCurrentMainMenu()
{
	switch ( m_iMainMenuStatus )
	{
	case TFMAINMENU_STATUS_MENU:
		// Show Main Menu and Video BG.
		m_pPanels[MAIN_MENU]->SetVisible( true );
		m_pPanels[PAUSE_MENU]->SetVisible( false );
		m_pPanels[BACKGROUND_MENU]->SetVisible( true );
		break;
	case TFMAINMENU_STATUS_INGAME:
		// Show Pause Menu.
		m_pPanels[MAIN_MENU]->SetVisible( false );
		m_pPanels[PAUSE_MENU]->SetVisible( true );
		m_pPanels[BACKGROUND_MENU]->SetVisible( false );
		break;
	case TFMAINMENU_STATUS_BACKGROUNDMAP:
		// Show Main Menu without Video BG.
		m_pPanels[MAIN_MENU]->SetVisible( true );
		m_pPanels[PAUSE_MENU]->SetVisible( false );
		m_pPanels[BACKGROUND_MENU]->SetVisible( false );
		break;
	case TFMAINMENU_STATUS_UNDEFINED:
	default:
		Assert( false );
		m_pPanels[MAIN_MENU]->SetVisible( false );
		m_pPanels[PAUSE_MENU]->SetVisible( false );
		m_pPanels[BACKGROUND_MENU]->SetVisible( false );
		break;
	}

	m_pPanels[GetCurrentMainMenu()]->RequestFocus();
}

void CTFMainMenu::SetStats( CUtlVector<ClassStats_t> &vecClassStats )
{
	GET_MAINMENUPANEL( CTFStatsSummaryDialog )->SetStats( vecClassStats );
}

void CTFMainMenu::ShowToolTip( const char *pszText )
{
	GET_MAINMENUPANEL( CTFToolTipPanel )->ShowToolTip( pszText );
}

void CTFMainMenu::HideToolTip()
{
	GET_MAINMENUPANEL( CTFToolTipPanel )->HideToolTip();
}

void CTFMainMenu::ShowItemToolTip( CEconItemDefinition *pItemData )
{
	GET_MAINMENUPANEL( CTFItemToolTipPanel )->ShowToolTip( pItemData );
}

void CTFMainMenu::HideItemToolTip()
{
	GET_MAINMENUPANEL( CTFItemToolTipPanel )->HideToolTip();
}


void CTFMainMenu::OnNotificationUpdate()
{
	GET_MAINMENUPANEL( CTFNotificationPanel )->OnNotificationUpdate();
	GET_MAINMENUPANEL( CTFMainMenuPanel )->OnNotificationUpdate();
	GET_MAINMENUPANEL( CTFPauseMenuPanel )->OnNotificationUpdate();
}

void CTFMainMenu::FadeMainMenuIn( void )
{
	CTFMenuPanelBase *pMenu = m_pPanels[GetCurrentMainMenu()];
	pMenu->Show();
	pMenu->SetAlpha( 0 );
	GetAnimationController()->RunAnimationCommand( pMenu, "Alpha", 255, 0.05f, 0.5f, AnimationController::INTERPOLATOR_SIMPLESPLINE );
}
