//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"
#include "hudelement.h"
#include "iclientmode.h"
#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui/ISurface.h>
#include <vgui/IImage.h>
#include <vgui_controls/Label.h>

#include "c_tf_playerresource.h"
#include "shareddefs.h"
#include "tf_playermodelpanel.h"

#include "hud_numericdisplay.h"
#include "c_team.h"
#include "c_tf_player.h"
#include "tf_shareddefs.h"
#include "tf_hud_playerstatus.h"
#include "tf_gamerules.h"
#include "econ_entity.h"
#include "econ_wearable.h"

#include "fmtstr.h"

using namespace vgui;

//ConVar cl_hud_playerclass_use_playermodel( "cl_hud_playerclass_use_playermodel", "1", FCVAR_ARCHIVE, "Use player model in player class HUD." );
extern ConVar cl_hud_playerclass_use_playermodel;

extern ConVar tf_max_health_boost;


static const char *g_szClassImages[] =
{
	"",
	"../hud/class_scout",
	"../hud/class_sniper",
	"../hud/class_soldier",
	"../hud/class_demo",
	"../hud/class_medic",
	"../hud/class_heavy",
	"../hud/class_pyro",
	"../hud/class_spy",
	"../hud/class_engi",
	"../hud/class_civ",
	"../hud/class_merc"
};

const char *aStatusIcons[29] =
{
	"PlayerStatusBleedImage",
	"PlayerStatusHookBleedImage",
	"PlayerStatusMilkImage",
	"PlayerStatusMarkedForDeathImage",
	"PlayerStatusMarkedForDeathSilentImage",
	"PlayerStatus_MedicUberBulletResistImage",
	"PlayerStatus_MedicUberBlastResistImage",
	"PlayerStatus_MedicUberFireResistImage",
	"PlayerStatus_MedicSmallBulletResistImage",
	"PlayerStatus_MedicSmallBlastResistImage",
	"PlayerStatus_MedicSmallFireResistImage",
	"PlayerStatus_WheelOfDoom",
	"PlayerStatus_SoldierOffenseBuff",
	"PlayerStatus_SoldierDefenseBuff",
	"PlayerStatus_SoldierHealOnHitBuff",
	"PlayerStatus_SpyMarked",
	"PlayerStatus_Parachute",
	"PlayerStatus_RuneStrength",
	"PlayerStatus_RuneHaste",
	"PlayerStatus_RuneRegen",
	"PlayerStatus_RuneResist",
	"PlayerStatus_RuneVampire",
	"PlayerStatus_RuneReflect",
	"PlayerStatus_RunePrecision",
	"PlayerStatus_RuneAgility",
	"PlayerStatus_RuneKnockout",
	"PlayerStatus_RuneKing",
	"PlayerStatus_RunePlague",
	"PlayerStatus_RuneSupernova"
}; // idb

enum
{
	HUD_HEALTH_NO_ANIM = 0,
	HUD_HEALTH_BONUS_ANIM,
	HUD_HEALTH_DYING_ANIM,
};

DECLARE_BUILD_FACTORY( CTFClassImage );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFClassImage::SetClass( int iTeam, int iClass, int iCloakstate )
{
	char szImage[128];
	szImage[0] = '\0';

	if( iTeam >= FIRST_GAME_TEAM )
	{
		// copy base image name
		V_strncpy( szImage, g_szClassImages[iClass], sizeof( szImage ) );

		// if dethmatch - add "custom" suffix
		if( iClass == TF_CLASS_MERCENARY && (TFGameRules() && TFGameRules()->IsDeathmatch()) )
		{
			V_strncat( szImage, "custom", sizeof( szImage ), COPY_ALL_CHARACTERS );
		}
		else // if not deathmatch - add player team color
		{
			V_strncat( szImage, g_aTeamParticleNames[iTeam], sizeof( szImage ), COPY_ALL_CHARACTERS );
		}

		switch( iCloakstate )
		{
		case 1:
			V_strncat( szImage, "_halfcloak", sizeof(szImage), COPY_ALL_CHARACTERS );
			break;
		case 2:
			V_strncat( szImage, "_cloak", sizeof(szImage), COPY_ALL_CHARACTERS );
			break;
		default:
			break;
		}

		if ( Q_strlen( szImage ) > 0 )
		{
			SetImage( szImage );
		}
	}
}



///
/// CTFHudPlayerClass
///

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFHudPlayerClass::CTFHudPlayerClass( Panel *parent, const char *name ) : EditablePanel( parent, name )
{
	m_pClassImage = new CTFClassImage( this, "PlayerStatusClassImage" );
	m_pClassImageBG = new CTFImagePanel( this, "PlayerStatusClassImageBG" );
	m_pSpyImage = new CTFImagePanel( this, "PlayerStatusSpyImage" );
	m_pSpyOutlineImage = new CTFImagePanel( this, "PlayerStatusSpyOutlineImage" );
	m_pClassModelPanel = new CTFPlayerModelPanel( this, "classmodelpanel" );
	m_pClassModelPanelBG = new CTFImagePanel( this, "classmodelpanelBG" );
	m_pCarryingWeaponPanel = new vgui::EditablePanel( this, "CarryingWeapon" );

	m_nTeam = TEAM_UNASSIGNED;
	m_nClass = TF_CLASS_UNDEFINED;
	m_nDisguiseTeam = TEAM_UNASSIGNED;
	m_nDisguiseClass = TF_CLASS_UNDEFINED;
	m_flNextThink = 0.0f;
	m_iDisguiseItemIndex = -1;

	ListenForGameEvent( "localplayer_changedisguise" );
	ListenForGameEvent( "post_inventory_application" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerClass::Reset()
{
	m_flNextThink = gpGlobals->curtime + 0.05f;

	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "HudSpyDisguiseHide" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerClass::ApplySchemeSettings( IScheme *pScheme )
{
	// load control settings...
	LoadControlSettings( "resource/UI/HudPlayerClass.res" );

	m_nTeam = TEAM_UNASSIGNED;
	m_nClass = TF_CLASS_UNDEFINED;
	m_nDisguiseTeam = TEAM_UNASSIGNED;
	m_nDisguiseClass = TF_CLASS_UNDEFINED;
	m_flNextThink = 0.0f;
	m_nCloakLevel = 0;
	m_iDisguiseItemIndex = -1;
	
	// SanyaSho: hide annoying %carrying% panel
	m_pCarryingWeaponPanel->SetVisible( false );

	BaseClass::ApplySchemeSettings( pScheme );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerClass::OnThink()
{
	if ( m_flNextThink < gpGlobals->curtime )
	{
		C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
		bool bTeamChange = false;

		if ( pPlayer )
		{
			// set our background colors
			if ( m_nTeam != pPlayer->GetTeamNumber() )
			{
				bTeamChange = true;
				m_nTeam = pPlayer->GetTeamNumber();
			}

			int nCloakLevel = 0;
			bool bCloakChange = false;
			float flInvis = pPlayer->GetPercentInvisible();

			if ( flInvis > 0.9 )
			{
				nCloakLevel = 2;
			}
			else if ( flInvis > 0.1 )
			{
				nCloakLevel = 1;
			}

			if ( nCloakLevel != m_nCloakLevel )
			{
				m_nCloakLevel = nCloakLevel;
				bCloakChange = true;
			}

			// set our class image
			if ( m_nClass != pPlayer->GetPlayerClass()->GetClassIndex() || bTeamChange || bCloakChange || ( m_hWeapon != pPlayer->GetActiveTFWeapon() ) ||
				( m_nClass == TF_CLASS_SPY && m_nDisguiseClass != pPlayer->m_Shared.GetDisguiseClass() ) ||
				( m_nClass == TF_CLASS_SPY && m_nDisguiseTeam != pPlayer->m_Shared.GetDisguiseTeam() ) ||
				( m_nClass == TF_CLASS_SPY && m_iDisguiseItemIndex != pPlayer->m_Shared.GetDisguiseItem()->GetItemDefIndex() ) )
			{
				m_nClass = pPlayer->GetPlayerClass()->GetClassIndex();
				m_hWeapon = pPlayer->GetActiveTFWeapon() ? pPlayer->GetActiveTFWeapon() : NULL;
				m_iDisguiseItemIndex = pPlayer->m_Shared.GetDisguiseItem()->GetItemDefIndex();

				if ( m_nClass == TF_CLASS_SPY && pPlayer->m_Shared.InCond( TF_COND_DISGUISED ) )
				{
					if ( !pPlayer->m_Shared.InCond( TF_COND_DISGUISING ) )
					{
						m_nDisguiseTeam = pPlayer->m_Shared.GetDisguiseTeam();
						m_nDisguiseClass = pPlayer->m_Shared.GetDisguiseClass();
					}
				}
				else
				{
					m_nDisguiseTeam = TEAM_UNASSIGNED;
					m_nDisguiseClass = TF_CLASS_UNDEFINED;
				}
				
				if ( cl_hud_playerclass_use_playermodel.GetBool() )
				{
					m_pClassImage->SetVisible( false );
					m_pClassImageBG->SetVisible( false );
					m_pSpyImage->SetVisible( false );
					m_pClassModelPanel->SetVisible( true );
					m_pClassModelPanelBG->SetVisible( true );
					
					UpdateModelPanel();
				}
				else
				{
					m_pClassImage->SetVisible( true );
					m_pClassImageBG->SetVisible( true );
					m_pClassModelPanel->SetVisible( false );
					m_pClassModelPanelBG->SetVisible( false );

					int iCloakState = 0;
					if ( pPlayer->IsPlayerClass( TF_CLASS_SPY ) )
					{
						iCloakState = m_nCloakLevel;
					}

					if ( m_nDisguiseTeam != TEAM_UNASSIGNED || m_nDisguiseClass != TF_CLASS_UNDEFINED )
					{
						m_pSpyImage->SetVisible( true );
						m_pClassImage->SetClass( m_nDisguiseTeam, m_nDisguiseClass, iCloakState );
					}
					else
					{
						m_pSpyImage->SetVisible( false );
						m_pClassImage->SetClass( m_nTeam, m_nClass, iCloakState );
					}
				}
			}
		}

		m_flNextThink = gpGlobals->curtime + 0.05f;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerClass::UpdateModelPanel()
{
	if( !cl_hud_playerclass_use_playermodel.GetBool() )
	{
		return;
	}
	
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer )
	{
		return;
	}
	
	m_pClassModelPanel->ClearCarriedItems();
	
	ETFLoadoutSlot iSlot = TF_LOADOUT_SLOT_INVALID;
	
	if( m_nDisguiseTeam != TEAM_UNASSIGNED )
	{
		m_pClassModelPanel->SetToPlayerClass( m_nDisguiseClass );
		m_pClassModelPanel->SetTeam( m_nDisguiseTeam, true, false );
		m_pClassModelPanel->AddCarriedItem( pPlayer->m_Shared.GetDisguiseItem() );
		iSlot = pPlayer->m_Shared.GetDisguiseItem()->GetLoadoutSlot( m_nDisguiseClass );
	}
	else
	{
		m_pClassModelPanel->SetToPlayerClass( m_nClass );
		m_pClassModelPanel->SetTeam( m_nTeam, true, false );
		if( m_hWeapon.IsValid() )
		{
			CEconItemView *pItem = m_hWeapon.Get()->GetItem();
			if( !pItem )
				return;
			
			m_pClassModelPanel->AddCarriedItem( pItem );
			iSlot = pItem->GetLoadoutSlot( m_nClass );
		}
		
		for( int i = 0; i < pPlayer->GetNumWearables(); i++ )
		{
			C_EconWearable *pWearable = pPlayer->GetWearable( i );
			if( !pWearable )
			{
				continue;
			}
			
			m_pClassModelPanel->AddCarriedItem( pWearable->GetItem() );
		}
	}
	
	m_pClassModelPanel->SetModelTintColor( pPlayer->m_vecPlayerColor );
	
	if( iSlot != TF_LOADOUT_SLOT_INVALID )
		m_pClassModelPanel->HoldItemInSlot( iSlot );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerClass::FireGameEvent( IGameEvent * event )
{
	const char* pszEventName = event->GetName();

	if ( FStrEq( "localplayer_changedisguise", pszEventName ) )
	{
		if ( m_pSpyImage && m_pSpyOutlineImage )
		{
			bool bFadeIn = event->GetBool( "disguised", false );

			if ( bFadeIn )
			{
				m_pSpyImage->SetAlpha( 0 );
			}
			else
			{
				m_pSpyImage->SetAlpha( 255 );
			}

			m_pSpyOutlineImage->SetAlpha( 0 );
			
			m_pSpyImage->SetVisible( true );
			m_pSpyOutlineImage->SetVisible( true );

			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( bFadeIn ? "HudSpyDisguiseFadeIn" : "HudSpyDisguiseFadeOut" );
		}

		UpdateModelPanel();
	}
	/*
	// FIXME(SanyaSho): causes weird crash with 0xc18 address
	else if ( FStrEq( "post_inventory_application", pszEventName ) )
	{
		// Force a refresh. if this is for the local player
		int iUserID = event->GetInt( "userid" );
		C_TFPlayer* pPlayer = ToTFPlayer( C_TFPlayer::GetLocalPlayer() );
		if ( pPlayer && pPlayer->GetUserID() == iUserID )
		{
			UpdateModelPanel();
		}
	}
	*/
}



///
/// CTFHealthPanel
///
 
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFHealthPanel::CTFHealthPanel( Panel *parent, const char *name ) : vgui::Panel( parent, name )
{
	m_flHealth = 1.0f;

	m_iMaterialIndex = surface()->DrawGetTextureId( "hud/health_color" );
	if ( m_iMaterialIndex == -1 ) // we didn't find it, so create a new one
	{
		m_iMaterialIndex = surface()->CreateNewTextureID();	
		surface()->DrawSetTextureFile( m_iMaterialIndex, "hud/health_color", true, false );
	}

	m_iDeadMaterialIndex = surface()->DrawGetTextureId( "hud/health_dead" );
	if ( m_iDeadMaterialIndex == -1 ) // we didn't find it, so create a new one
	{
		m_iDeadMaterialIndex = surface()->CreateNewTextureID();	
		surface()->DrawSetTextureFile( m_iDeadMaterialIndex, "hud/health_dead", true, false );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHealthPanel::Paint()
{
	BaseClass::Paint();

	int x, y, w, h;
	GetBounds( x, y, w, h );

	Vertex_t vert[4];	
	float uv1 = 0.0f;
	float uv2 = 1.0f;
	int xpos = 0, ypos = 0;

	if ( m_flHealth <= 0 )
	{
		// Draw the dead material
		surface()->DrawSetTexture( m_iDeadMaterialIndex );
		
		vert[0].Init( Vector2D( xpos, ypos ), Vector2D( uv1, uv1 ) );
		vert[1].Init( Vector2D( xpos + w, ypos ), Vector2D( uv2, uv1 ) );
		vert[2].Init( Vector2D( xpos + w, ypos + h ), Vector2D( uv2, uv2 ) );				
		vert[3].Init( Vector2D( xpos, ypos + h ), Vector2D( uv1, uv2 ) );

		surface()->DrawSetColor( Color(255,255,255,255) );
	}
	else
	{
		float flDamageY = h * ( 1.0f - m_flHealth );

		// blend in the red "damage" part
		surface()->DrawSetTexture( m_iMaterialIndex );

		Vector2D uv11( uv1, uv2 - m_flHealth );
		Vector2D uv21( uv2, uv2 - m_flHealth );
		Vector2D uv22( uv2, uv2 );
		Vector2D uv12( uv1, uv2 );

		vert[0].Init( Vector2D( xpos, flDamageY ), uv11 );
		vert[1].Init( Vector2D( xpos + w, flDamageY ), uv21 );
		vert[2].Init( Vector2D( xpos + w, ypos + h ), uv22 );				
		vert[3].Init( Vector2D( xpos, ypos + h ), uv12 );

		surface()->DrawSetColor( GetFgColor() );
	}

	surface()->DrawTexturedPolygon( 4, vert );
}



///
/// CTFHudPlayerHealth
///

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFHudPlayerHealth::CTFHudPlayerHealth( Panel *parent, const char *name ) : EditablePanel( parent, name )
{
	m_pHealthImage = new CTFHealthPanel( this, "PlayerStatusHealthImage" );	
	m_pHealthImageBG = new ImagePanel( this, "PlayerStatusHealthImageBG" );
	m_pHealthBonusImage = new ImagePanel( this, "PlayerStatusHealthBonusImage" );
	m_pBuildingHealthImageBG = new ImagePanel( this, "BuildingStatusHealthImageBG" );

	m_flNextThink = 0.0f;

	m_nBonusHealthOrigX = -1;
	m_nBonusHealthOrigY = -1;
	m_nBonusHealthOrigW = -1;
	m_nBonusHealthOrigH = -1;

	m_iAnimState = HUD_HEALTH_NO_ANIM;
	m_bAnimate = true;
}

CTFHudPlayerHealth::~CTFHudPlayerHealth()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerHealth::Reset()
{
	m_flNextThink = gpGlobals->curtime + 0.05f;
	m_nHealth = -1;
	m_bBuilding = false;

	m_iAnimState = HUD_HEALTH_NO_ANIM;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerHealth::ApplySchemeSettings( IScheme *pScheme )
{
	// load control settings...
	LoadControlSettings( GetResFilename() );

	if ( m_pHealthBonusImage )
	{
		m_pHealthBonusImage->GetBounds( m_nBonusHealthOrigX, m_nBonusHealthOrigY, m_nBonusHealthOrigW, m_nBonusHealthOrigH );
	}

	m_flNextThink = 0.0f;

	BaseClass::ApplySchemeSettings( pScheme );

	m_pBuildingHealthImageBG->SetVisible( m_bBuilding );

	m_pPlayerLevelLabel = dynamic_cast<CExLabel*>( FindChildByName( "PlayerStatusPlayerLevel" ) );
	
	for( int i = 0; i < 29; i++ )
	{
		vgui::Panel *pChild = FindChildByName( aStatusIcons[i] );
		if( pChild )
			pChild->SetVisible( false );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerHealth::SetHealth( int iNewHealth, int iMaxHealth, int	iMaxBuffedHealth )
{
	// set our health
	m_nHealth = iNewHealth;
	m_nMaxHealth = iMaxHealth;
	m_pHealthImage->SetHealth( (float)(m_nHealth) / (float)(m_nMaxHealth) );

	if ( m_pHealthImage )
	{
		m_pHealthImage->SetFgColor( Color( 255, 255, 255, 255 ) );
	}

	if ( m_nHealth <= 0 )
	{
		if ( m_pHealthImageBG->IsVisible() )
		{
			m_pHealthImageBG->SetVisible( false );
		}
		if ( m_pBuildingHealthImageBG->IsVisible() )
		{
			m_pBuildingHealthImageBG->SetVisible( false );
		}
		HideHealthBonusImage();
	}
	else
	{
		if ( !m_pHealthImageBG->IsVisible() )
		{
			m_pHealthImageBG->SetVisible( true );
		}
		m_pBuildingHealthImageBG->SetVisible( m_bBuilding );

		// are we getting a health bonus?
		if ( m_nHealth > m_nMaxHealth )
		{
			if ( m_pHealthBonusImage && m_nBonusHealthOrigW != -1 )
			{
				if ( !m_pHealthBonusImage->IsVisible() )
				{
					m_pHealthBonusImage->SetVisible( true );
				}

				if ( m_bAnimate && m_iAnimState != HUD_HEALTH_BONUS_ANIM )
				{
					g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( this, "HudHealthDyingPulseStop" );
					g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( this, "HudHealthBonusPulse" );

					m_iAnimState = HUD_HEALTH_BONUS_ANIM;
				}

				m_pHealthBonusImage->SetDrawColor( Color( 255, 255, 255, 255 ) );

				// scale the flashing image based on how much health bonus we currently have
				float flBoostMaxAmount = ( iMaxBuffedHealth ) - m_nMaxHealth;
				float flPercent = MIN( ( m_nHealth - m_nMaxHealth ) / flBoostMaxAmount, 1.0f );

				int nPosAdj = RoundFloatToInt( flPercent * m_nHealthBonusPosAdj );
				int nSizeAdj = 2 * nPosAdj;

				m_pHealthBonusImage->SetBounds( m_nBonusHealthOrigX - nPosAdj, 
					m_nBonusHealthOrigY - nPosAdj, 
					m_nBonusHealthOrigW + nSizeAdj,
					m_nBonusHealthOrigH + nSizeAdj );
			}
		}
		// are we close to dying?
		else if ( m_nHealth < m_nMaxHealth * m_flHealthDeathWarning )
		{
			if ( m_pHealthBonusImage && m_nBonusHealthOrigW != -1 )
			{
				if ( !m_pHealthBonusImage->IsVisible() )
				{
					m_pHealthBonusImage->SetVisible( true );
				}

				if ( m_bAnimate && m_iAnimState != HUD_HEALTH_DYING_ANIM )
				{
					g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( this, "HudHealthBonusPulseStop" );
					g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( this, "HudHealthDyingPulse" );

					m_iAnimState = HUD_HEALTH_DYING_ANIM;
				}

				m_pHealthBonusImage->SetDrawColor( m_clrHealthDeathWarningColor );

				// scale the flashing image based on how much health bonus we currently have
				float flBoostMaxAmount = m_nMaxHealth * m_flHealthDeathWarning;
				float flPercent = ( flBoostMaxAmount - m_nHealth ) / flBoostMaxAmount;

				int nPosAdj = RoundFloatToInt( flPercent * m_nHealthBonusPosAdj );
				int nSizeAdj = 2 * nPosAdj;

				m_pHealthBonusImage->SetBounds( m_nBonusHealthOrigX - nPosAdj, 
					m_nBonusHealthOrigY - nPosAdj, 
					m_nBonusHealthOrigW + nSizeAdj,
					m_nBonusHealthOrigH + nSizeAdj );
			}

			if ( m_pHealthImage )
			{
				m_pHealthImage->SetFgColor( m_clrHealthDeathWarningColor );
			}
		}
		// turn it off
		else
		{
			HideHealthBonusImage();
		}
	}

	// set our health display value
	if ( m_nHealth > 0 )
	{
		SetDialogVariable( "Health", m_nHealth );
	}
	else
	{
		SetDialogVariable( "Health", "" );
	}	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerHealth::SetLevel( int nLevel )
{
	if ( m_pPlayerLevelLabel )
	{
		bool bVisible = ( nLevel >= 0 ) ? true : false;
		if ( bVisible )
		{
			m_pPlayerLevelLabel->SetText( CFmtStr( "%d", nLevel ) );
		}

		if ( m_pPlayerLevelLabel->IsVisible() != bVisible )
		{
			m_pPlayerLevelLabel->SetVisible( bVisible );
		}
	}
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerHealth::HideHealthBonusImage( void )
{
	if ( m_pHealthBonusImage && m_pHealthBonusImage->IsVisible() )
	{
		if ( m_nBonusHealthOrigW != -1 )
		{
			m_pHealthBonusImage->SetBounds( m_nBonusHealthOrigX, m_nBonusHealthOrigY, m_nBonusHealthOrigW, m_nBonusHealthOrigH );
		}
		m_pHealthBonusImage->SetVisible( false );
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( this, "HudHealthBonusPulseStop" );
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( this, "HudHealthDyingPulseStop" );

		m_iAnimState = HUD_HEALTH_NO_ANIM;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
static void SetPlayerHealthImagePanelVisibility( CTFPlayer *pPlayer, ETFCond eCond, vgui::ImagePanel *pImagePanel, int& nXOffset, const Color& colorIfVisible )
{
	Assert( pImagePanel != NULL );

	if ( pPlayer->m_Shared.InCond( eCond ) && !pImagePanel->IsVisible() )
	{
		pImagePanel->SetVisible( true );
		pImagePanel->SetDrawColor( colorIfVisible );
		
		// Reposition ourselves and increase the offset if we are active
		int x,y;
		pImagePanel->GetPos( x, y );
		pImagePanel->SetPos( nXOffset, y );
		nXOffset += 100.f;
	}
}

void CTFHudPlayerHealth::OnThink()
{
	if ( m_flNextThink < gpGlobals->curtime )
	{
		C_TFPlayer *pPlayer = ToTFPlayer( C_BasePlayer::GetLocalPlayer() );

		if ( pPlayer )
		{
			SetHealth( pPlayer->GetHealth(), pPlayer->GetMaxHealth(), pPlayer->m_Shared.GetMaxBuffedHealth() );
			
			/*if ( tf2c_low_health_sound.m_pParent->m_nValue )
			{
				if ( (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)v3 + 520))(v3) )
				{
					v6 = (float)this->m_nMaxHealth * tf2c_low_health_sound_threshold.m_pParent->m_fValue;
					if ( (float)m_nHealth >= v6 && v6 > (float)this->m_nHealth )
					{
						CLocalPlayerFilter::CLocalPlayerFilter(&filter);
						C_BaseEntity::EmitSound(&filter, -1, "TFPlayer.LowHealth", 0, 0.0, 0);
						C_RecipientFilter::~C_RecipientFilter(&filter);
					}
				}
			}*/
		}

		m_flNextThink = gpGlobals->curtime + 0.05f;
	}
}



///
/// CTFHudPlayerStatus
///

DECLARE_HUDELEMENT( CTFHudPlayerStatus );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFHudPlayerStatus::CTFHudPlayerStatus( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudPlayerStatus" ) 
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	m_pHudPlayerClass = new CTFHudPlayerClass( this, "HudPlayerClass" );
	m_pHudPlayerHealth = new CTFHudPlayerHealth( this, "HudPlayerHealth" );

	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerStatus::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	// HACK: Work around the scheme application order failing
	// to reload the player class hud element's scheme in minmode.
	static ConVarRef cl_hud_minmode( "cl_hud_minmode", true );
	if ( cl_hud_minmode.IsValid() && cl_hud_minmode.GetBool() )
	{
		m_pHudPlayerClass->InvalidateLayout( false, true );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTFHudPlayerStatus::ShouldDraw( void )
{
	CTFPlayer *pTFPlayer = CTFPlayer::GetLocalTFPlayer();
	if ( pTFPlayer && pTFPlayer->m_Shared.InCond( TF_COND_HALLOWEEN_GHOST_MODE ) )
		return false;

	return CHudElement::ShouldDraw();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudPlayerStatus::Reset()
{
	if ( m_pHudPlayerClass )
	{
		m_pHudPlayerClass->Reset();
	}

	if ( m_pHudPlayerHealth )
	{
		m_pHudPlayerHealth->Reset();
	}
}
