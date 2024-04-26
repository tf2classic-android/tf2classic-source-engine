//=========== (C) Copyright 1999 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// $Header: $
// $NoKeywords: $
//
//=============================================================================
#include "cbase.h"
#include "hud.h"
#include "clientmode_tf.h"
#include "cdll_client_int.h"
#include "iinput.h"
#include "vgui/ISurface.h"
#include "vgui/IPanel.h"
#include "GameUI/IGameUI.h"
#include <vgui_controls/AnimationController.h>
#include "ivmodemanager.h"
#include "buymenu.h"
#include "filesystem.h"
#include "vgui/IVGui.h"
#include "view_shared.h"
#include "view.h"
#include "ivrenderview.h"
#include "model_types.h"
#include "iefx.h"
#include "dlight.h"
#include <imapoverview.h>
#include "c_playerresource.h"
#include <KeyValues.h>
#include "text_message.h"
#include "panelmetaclassmgr.h"
#include "c_tf_player.h"
#include "ienginevgui.h"
#include "in_buttons.h"
#include "voice_status.h"
#include "tf_hud_menu_engy_build.h"
#include "tf_hud_menu_engy_destroy.h"
#include "tf_hud_menu_spy_disguise.h"
#include "tf_statsummary.h"
#include "tf_hud_freezepanel.h"
#include "clienteffectprecachesystem.h"
#include "glow_outline_effect.h"
#include "cam_thirdperson.h"
#include "tf_gamerules.h"
#include "tf_hud_chat.h"
#include "c_tf_team.h"
#include "c_tf_playerresource.h"
#include "tf_clientscoreboard.h"
#include "tf_mainmenu.h"
#include "tier0/icommandline.h"

#if defined( _X360 )
#include "tf_clientscoreboard.h"
#endif

ConVar default_fov( "default_fov", "75", FCVAR_CHEAT );
ConVar fov_desired( "fov_desired", "90", FCVAR_ARCHIVE | FCVAR_USERINFO, "Sets the base field-of-view.", true, 75.0, true, MAX_FOV );

void HUDMinModeChangedCallBack( IConVar *var, const char *pOldString, float flOldValue )
{
	engine->ExecuteClientCmd( "hud_reloadscheme" );
}

ConVar cl_hud_minmode( "cl_hud_minmode", "0", FCVAR_ARCHIVE, "Set to 1 to turn on the advanced minimalist HUD mode.", HUDMinModeChangedCallBack );
ConVar tf2c_coloredhud("tf2c_coloredhud", "0", FCVAR_ARCHIVE, "Set to 1 to turn on panel coloring.", HUDMinModeChangedCallBack);

#if defined( DEBUG )
#define TF2C_ITEMS_CRC32_FLAGS	FCVAR_HIDDEN | FCVAR_DONTRECORD | FCVAR_USERINFO
#else
#define TF2C_ITEMS_CRC32_FLAGS	FCVAR_DEVELOPMENTONLY | FCVAR_DONTRECORD | FCVAR_USERINFO
#endif
ConVar tf2c_cl_items_crc32( "tf2c_cl_items_crc32", "uninitialized", TF2C_ITEMS_CRC32_FLAGS );

extern ConVar tf_scoreboard_mouse_mode;

#if defined( ANDROID )
#define IS_MOBILE "1"
#else
#define IS_MOBILE "0"
#endif

#define FCVAR_TF2C_IS_MOBILE FCVAR_HIDDEN | FCVAR_USERINFO | FCVAR_DEVELOPMENTONLY
ConVar tf2c_is_mobile_build( "tf2c_is_mobile_build", IS_MOBILE, FCVAR_TF2C_IS_MOBILE, "Determine player's platform" );

IClientMode *g_pClientMode = NULL;
// --------------------------------------------------------------------------------- //
// CTFModeManager.
// --------------------------------------------------------------------------------- //

class CTFModeManager : public IVModeManager
{
public:
	virtual void	Init();
	virtual void	SwitchMode( bool commander, bool force ) {}
	virtual void	LevelInit( const char *newmap );
	virtual void	LevelShutdown( void );
	virtual void	ActivateMouse( bool isactive ) {}
};

static CTFModeManager g_ModeManager;
IVModeManager *modemanager = ( IVModeManager * )&g_ModeManager;

CLIENTEFFECT_REGISTER_BEGIN( PrecachePostProcessingEffectsGlow )
	CLIENTEFFECT_MATERIAL( "dev/glow_blur_x" )
	CLIENTEFFECT_MATERIAL( "dev/glow_blur_y" )
	CLIENTEFFECT_MATERIAL( "dev/glow_color" )
	CLIENTEFFECT_MATERIAL( "dev/glow_downsample" )
	CLIENTEFFECT_MATERIAL( "dev/halo_add_to_screen" )
CLIENTEFFECT_REGISTER_END_CONDITIONAL(	engine->GetDXSupportLevel() >= 90 )

// --------------------------------------------------------------------------------- //
// CTFModeManager implementation.
// --------------------------------------------------------------------------------- //

#define SCREEN_FILE		"scripts/vgui_screens.txt"

void CTFModeManager::Init()
{
	g_pClientMode = GetClientModeNormal();
	
	PanelMetaClassMgr()->LoadMetaClassDefinitionFile( SCREEN_FILE );

	// Load the objects.txt file.
	LoadObjectInfos( ::filesystem );

	GetClientVoiceMgr()->SetHeadLabelOffset( 40 );

	// ITEMS CRC32
	CRC32_t file_crc32 = 0xFFFFFFF;
	char buffer[8192] = {};

	FileHandle_t file = filesystem->Open( "scripts/items/items_game.txt", "r", "MOD" );

	if( !file )
	{
#ifdef DEBUG
		DevMsg( "[CLIENT] CRC32 of items_game.txt is failed\n" );
#endif
		filesystem->Close( file );
		return;
	}

	CRC32_Init( &file_crc32 );
	while( !filesystem->EndOfFile( file ) )
	{
		int readed_real = filesystem->Read( &buffer, sizeof( buffer ), file );
		CRC32_ProcessBuffer( &file_crc32, &buffer, readed_real );
	}
	CRC32_Final( &file_crc32 );

#ifdef DEBUG
	DevMsg( "[CLIENT] CRC32 of items_game.txt is 0x%x\n", file_crc32 );
#endif

	filesystem->Close( file );

	tf2c_cl_items_crc32.SetValue( VarArgs( "%x", file_crc32 ) );
}

void CTFModeManager::LevelInit( const char *newmap )
{
	g_pClientMode->LevelInit( newmap );

	ConVarRef voice_steal( "voice_steal" );

	if ( voice_steal.IsValid() )
	{
		voice_steal.SetValue( 1 );
	}

	g_ThirdPersonManager.Init();
}

void CTFModeManager::LevelShutdown( void )
{
	g_pClientMode->LevelShutdown();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
ClientModeTFNormal::ClientModeTFNormal()
{
	m_pMenuEngyBuild = NULL;
	m_pMenuEngyDestroy = NULL;
	m_pMenuSpyDisguise = NULL;
	m_pGameUI = NULL;
	m_pFreezePanel = NULL;

	m_pScoreboard = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: If you don't know what a destructor is by now, you are probably going to get fired
//-----------------------------------------------------------------------------
ClientModeTFNormal::~ClientModeTFNormal()
{
}

// See interface.h/.cpp for specifics:  basically this ensures that we actually Sys_UnloadModule the dll and that we don't call Sys_LoadModule 
//  over and over again.
static CDllDemandLoader g_GameUI( "GameUI" );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ClientModeTFNormal::Init()
{
	m_pMenuEngyBuild = ( CHudMenuEngyBuild * )GET_HUDELEMENT( CHudMenuEngyBuild );
	Assert( m_pMenuEngyBuild );

	m_pMenuEngyDestroy = ( CHudMenuEngyDestroy * )GET_HUDELEMENT( CHudMenuEngyDestroy );
	Assert( m_pMenuEngyDestroy );

	m_pMenuSpyDisguise = ( CHudMenuSpyDisguise * )GET_HUDELEMENT( CHudMenuSpyDisguise );
	Assert( m_pMenuSpyDisguise );

	m_pFreezePanel = ( CTFFreezePanel * )GET_HUDELEMENT( CTFFreezePanel );
	Assert( m_pFreezePanel );

	CreateInterfaceFn gameUIFactory = g_GameUI.GetFactory();
	if ( gameUIFactory )
	{
		m_pGameUI = (IGameUI *) gameUIFactory(GAMEUI_INTERFACE_VERSION, NULL );
		if ( m_pGameUI )
		{
			// insert stats summary panel as the loading background dialog
			CTFStatsSummaryPanel *pPanel = GStatsSummaryPanel();
			pPanel->InvalidateLayout( false, true );
			pPanel->SetVisible( false );
			pPanel->MakePopup( false );
			m_pGameUI->SetLoadingBackgroundDialog( pPanel->GetVPanel() );

			// Create the new main menu.
			if ( CommandLine()->CheckParm( "-nonewmenu" ) == NULL )
			{
				// Disable normal BG music played in GameUI.
				CommandLine()->AppendParm( "-nostartupsound", NULL );
				CTFMainMenu *pMenu = new CTFMainMenu();
				m_pGameUI->SetMainMenuOverride( pMenu->GetVPanel() );
			}
		}
	}

#if defined( _X360 )
	m_pScoreboard = (CTFClientScoreBoardDialog *)( gViewPortInterface->FindPanelByName( PANEL_SCOREBOARD ) );
	Assert( m_pScoreboard );
#endif

	BaseClass::Init();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ClientModeTFNormal::Shutdown()
{
	DestroyStatsSummaryPanel();
}

void ClientModeTFNormal::InitViewport()
{
	m_pViewport = new TFViewport();
	m_pViewport->Start( gameuifuncs, gameeventmanager );
}

ClientModeTFNormal g_ClientModeNormal;

IClientMode *GetClientModeNormal()
{
	return &g_ClientModeNormal;
}


ClientModeTFNormal* GetClientModeTFNormal()
{
	Assert( dynamic_cast< ClientModeTFNormal* >( GetClientModeNormal() ) );

	return static_cast< ClientModeTFNormal* >( GetClientModeNormal() );
}

//-----------------------------------------------------------------------------
// Purpose: Fixes some bugs from base class.
//-----------------------------------------------------------------------------
void ClientModeTFNormal::OverrideView( CViewSetup *pSetup )
{
	QAngle camAngles;

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	// Let the player override the view.
	pPlayer->OverrideView( pSetup );

	if ( ::input->CAM_IsThirdPerson() && !pPlayer->IsObserver() )
	{
		const Vector& cam_ofs = g_ThirdPersonManager.GetCameraOffsetAngles();
		Vector cam_ofs_distance;

		if ( g_ThirdPersonManager.IsOverridingThirdPerson() )
		{
			cam_ofs_distance = g_ThirdPersonManager.GetDesiredCameraOffset();
		}
		else
		{
			cam_ofs_distance = g_ThirdPersonManager.GetFinalCameraOffset();
		}

		cam_ofs_distance *= g_ThirdPersonManager.GetDistanceFraction();

		camAngles[PITCH] = cam_ofs[PITCH];
		camAngles[YAW] = cam_ofs[YAW];
		camAngles[ROLL] = 0;

		Vector camForward, camRight, camUp;

		if ( g_ThirdPersonManager.IsOverridingThirdPerson() == false )
		{
			engine->GetViewAngles( camAngles );
		}

		// get the forward vector
		AngleVectors( camAngles, &camForward, &camRight, &camUp );

		VectorMA( pSetup->origin, -cam_ofs_distance[0], camForward, pSetup->origin );
		VectorMA( pSetup->origin, cam_ofs_distance[1], camRight, pSetup->origin );
		VectorMA( pSetup->origin, cam_ofs_distance[2], camUp, pSetup->origin );

		// Override angles from third person camera
		VectorCopy( camAngles, pSetup->angles );
	}
	else if ( ::input->CAM_IsOrthographic() )
	{
		pSetup->m_bOrtho = true;
		float w, h;
		::input->CAM_OrthographicSize( w, h );
		w *= 0.5f;
		h *= 0.5f;
		pSetup->m_OrthoLeft = -w;
		pSetup->m_OrthoTop = -h;
		pSetup->m_OrthoRight = w;
		pSetup->m_OrthoBottom = h;
	}
}

extern ConVar v_viewmodel_fov;
float g_flViewModelFOV = 75;
float ClientModeTFNormal::GetViewModelFOV( void )
{
	return v_viewmodel_fov.GetFloat();
//	return g_flViewModelFOV;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool ClientModeTFNormal::ShouldDrawViewModel()
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( pPlayer )
	{
		if ( pPlayer->m_Shared.InCond( TF_COND_ZOOMED ) )
			return false;
	}

	return true;
}

ConVar tf_hud_no_crosshair_on_scope_zoom( "tf_hud_no_crosshair_on_scope_zoom", "0", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool ClientModeTFNormal::ShouldDrawCrosshair()
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if( !pPlayer )
	{
		return false;
	}

	if( pPlayer->GetPlayerClass() &&
		(pPlayer->IsPlayerClass( TF_CLASS_SNIPER ) || pPlayer->IsPlayerClass( TF_CLASS_MERCENARY )) &&
		pPlayer->m_Shared.InCond( TF_COND_ZOOMED ) &&
		tf_hud_no_crosshair_on_scope_zoom.GetBool()
	)
	{
		return false;
	}

	return ClientModeShared::ShouldDrawCrosshair();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool ClientModeTFNormal::DoPostScreenSpaceEffects( const CViewSetup *pSetup )
{
	if ( !IsInFreezeCam() )
		g_GlowObjectManager.RenderGlowEffects( pSetup, 0 );

	return BaseClass::DoPostScreenSpaceEffects( pSetup );
}

int ClientModeTFNormal::GetDeathMessageStartHeight( void )
{
	return m_pViewport->GetDeathMessageStartHeight();
}


bool IsInCommentaryMode( void );
bool PlayerNameNotSetYet( const char *pszName );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ClientModeTFNormal::FireGameEvent( IGameEvent *event )
{
	const char *eventname = event->GetName();

	if ( !eventname || !eventname[0] )
		return;

	if ( V_strcmp( "player_changename", eventname ) == 0 )
	{
		return; // server sends a colorized text string for this
	}
	else if ( V_strcmp( "player_team", eventname ) == 0 )
	{
		// Using our own strings here.
		int iPlayerIndex = engine->GetPlayerForUserID( event->GetInt( "userid" ) );

		CHudChat *pChat = GetTFChatHud();
		if ( !pChat )
			return;

		bool bDisconnected = event->GetBool( "disconnect" );

		if ( bDisconnected )
			return;

		int iTeam = event->GetInt( "team" );
		bool bAutoTeamed = event->GetBool( "autoteam" );
		bool bSilent = event->GetBool( "silent" );

		const char *pszName = event->GetString( "name" );
		if ( PlayerNameNotSetYet( pszName ) )
			return;

		if ( !bSilent )
		{
			wchar_t wszPlayerName[MAX_PLAYER_NAME_LENGTH];
			g_pVGuiLocalize->ConvertANSIToUnicode( pszName, wszPlayerName, sizeof( wszPlayerName ) );

			wchar_t wszTeam[64];
			C_Team *pTeam = GetGlobalTeam( iTeam );
			if ( pTeam )
			{
				g_pVGuiLocalize->ConvertANSIToUnicode( pTeam->Get_Name(), wszTeam, sizeof( wszTeam ) );
			}
			else
			{
				_snwprintf( wszTeam, sizeof( wszTeam ) / sizeof( wchar_t ), L"%d", iTeam );
			}

			if ( !IsInCommentaryMode() )
			{
				// Client isn't going to catch up on team change so we have to set the color manually here.
				Color col = pChat->GetTeamColor( iTeam );

				wchar_t wszLocalized[100];
				if ( bAutoTeamed )
				{
					g_pVGuiLocalize->ConstructString( wszLocalized, sizeof( wszLocalized ), g_pVGuiLocalize->Find( "#TF_Joined_AutoTeam" ), 2, wszPlayerName, wszTeam );
				}
				else if ( TFGameRules() && TFGameRules()->IsDeathmatch() )
				{
					if ( iTeam >= FIRST_GAME_TEAM )
					{
						g_pVGuiLocalize->ConstructString( wszLocalized, sizeof( wszLocalized ), g_pVGuiLocalize->Find( "#TF_Joined_Deathmatch" ), 1, wszPlayerName );
					}
					else
					{
						g_pVGuiLocalize->ConstructString( wszLocalized, sizeof( wszLocalized ), g_pVGuiLocalize->Find( "#TF_Joined_Deathmatch_Spectator" ), 1, wszPlayerName );
					}

					if( g_TF_PR )
					{
						col = g_TF_PR->GetPlayerColor( iPlayerIndex );
					}
				}
				else
				{
					g_pVGuiLocalize->ConstructString( wszLocalized, sizeof( wszLocalized ), g_pVGuiLocalize->Find( "#TF_Joined_Team" ), 2, wszPlayerName, wszTeam );
				}

				char szLocalized[100];
				g_pVGuiLocalize->ConvertUnicodeToANSI( wszLocalized, szLocalized, sizeof( szLocalized ) );

				pChat->SetCustomColor( col );
				pChat->ChatPrintf( iPlayerIndex, CHAT_FILTER_TEAMCHANGE, "%s", szLocalized );
			}
		}

		C_BasePlayer *pPlayer = UTIL_PlayerByIndex( iPlayerIndex );

		if ( pPlayer && pPlayer->IsLocalPlayer() )
		{
			// that's me
			pPlayer->TeamChange( iTeam );
		}

		return;
	}
	else if ( V_strcmp( "game_maploaded", eventname ) == 0 )
	{
		// Remember which scoreboard should be used on this map.
		m_pScoreboard = (CTFClientScoreBoardDialog *)GetTFViewPort()->FindPanelByName( GetTFViewPort()->GetModeSpecificScoreboardName() );
	}

	BaseClass::FireGameEvent( event );
}


void ClientModeTFNormal::PostRenderVGui()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool ClientModeTFNormal::CreateMove( float flInputSampleTime, CUserCmd *cmd )
{
	return BaseClass::CreateMove( flInputSampleTime, cmd );
}

//-----------------------------------------------------------------------------
// Purpose: See if hud elements want key input. Return 0 if the key is swallowed
//-----------------------------------------------------------------------------
int ClientModeTFNormal::HudElementKeyInput( int down, ButtonCode_t keynum, const char *pszCurrentBinding )
{
	if ( tf_scoreboard_mouse_mode.GetInt() == 2 )
	{
		if ( m_pScoreboard && !m_pScoreboard->HudElementKeyInput( down, keynum, pszCurrentBinding ) )
		{
			return 0;
		}
	}

	// check for hud menus
	if ( m_pMenuEngyBuild )
	{
		if ( !m_pMenuEngyBuild->HudElementKeyInput( down, keynum, pszCurrentBinding ) )
		{
			return 0;
		}
	}

	if ( m_pMenuEngyDestroy )
	{
		if ( !m_pMenuEngyDestroy->HudElementKeyInput( down, keynum, pszCurrentBinding ) )
		{
			return 0;
		}
	}

	if ( m_pMenuSpyDisguise )
	{
		if ( !m_pMenuSpyDisguise->HudElementKeyInput( down, keynum, pszCurrentBinding ) )
		{
			return 0;
		}
	}

	if ( m_pFreezePanel )
	{
		m_pFreezePanel->HudElementKeyInput( down, keynum, pszCurrentBinding );
	}

	return BaseClass::HudElementKeyInput( down, keynum, pszCurrentBinding );
}

//-----------------------------------------------------------------------------
// Purpose: See if spectator input occurred. Return 0 if the key is swallowed.
//-----------------------------------------------------------------------------
int ClientModeTFNormal::HandleSpectatorKeyInput( int down, ButtonCode_t keynum, const char *pszCurrentBinding )
{
	if ( tf_scoreboard_mouse_mode.GetInt() == 2 )
	{
		// Scoreboard allows enabling mouse input with right click so don't steal input from it.
		if ( m_pScoreboard && m_pScoreboard->IsVisible() )
		{
			return 1;
		}
	}

	return BaseClass::HandleSpectatorKeyInput( down, keynum, pszCurrentBinding );
}

// Sent when when map is *actually* completely loaded on the client, i.e. all the entities have been spawned.
class CPostEntityHudInitSystem : public CAutoGameSystem
{
public:
	CPostEntityHudInitSystem() : CAutoGameSystem( "PostEntityHudInit" )
	{
	}

	virtual void LevelInitPostEntity()
	{
		IGameEvent *event = gameeventmanager->CreateEvent( "game_maploaded" );
		if( event )
		{
			gameeventmanager->FireEventClientSide( event );
		}
	}
};

static CPostEntityHudInitSystem g_pPostEntityHudInit;
