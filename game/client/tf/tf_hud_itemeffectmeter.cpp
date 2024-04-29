//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "c_tf_player.h"
#include "iclientmode.h"
#include "ienginevgui.h"
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ProgressBar.h>
#include "engine/IEngineSound.h"
#include "tf_controls.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CHudItemEffectMeter : public EditablePanel
{
	DECLARE_CLASS_SIMPLE( CHudItemEffectMeter, EditablePanel );

public:
	CHudItemEffectMeter( Panel *pParent, const char *pElementName );

	virtual void	ApplySchemeSettings( IScheme *scheme );
	virtual void	PerformLayout( void );
	virtual void	LevelInit( void );

	void			UpdateStatus( C_TFWeaponBase *pWeapon );

private:
	ContinuousProgressBar *m_pEffectMeter;
	CExLabel *m_pEffectMeterLabel;

	CHandle<C_TFWeaponBase> m_hWeapon;
	float m_flOldCharge;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudItemEffectMeter::CHudItemEffectMeter( Panel *pParent, const char *pElementName ) : EditablePanel( pParent, pElementName )
{
	m_pEffectMeter = new ContinuousProgressBar( this, "ItemEffectMeter" );
	m_pEffectMeterLabel = new CExLabel( this, "ItemEffectMeterLabel", "" );
	m_flOldCharge = 1.0f;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffectMeter::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
}

// ---------------------------------------------------------------------------- -
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffectMeter::LevelInit( void )
{
	m_hWeapon = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffectMeter::PerformLayout( void )
{
	if ( m_pEffectMeterLabel && m_hWeapon.Get() )
	{
		m_pEffectMeterLabel->SetText( m_hWeapon->GetEffectLabelText() );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffectMeter::UpdateStatus( C_TFWeaponBase *pWeapon )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();

	if ( !pPlayer || !pPlayer->IsAlive() )
	{
		m_flOldCharge = 1.0f;
		return;
	}

	// Update the weapon.
	if ( pWeapon )
	{
		if ( pWeapon != m_hWeapon.Get() )
		{
			// Weapon changed, reset the label and progress.
			m_hWeapon = pWeapon;
			m_flOldCharge = m_hWeapon->GetEffectBarProgress();

			InvalidateLayout();
		}
	}
	else
	{
		m_hWeapon = NULL;
	}

	if ( !m_hWeapon.Get() )
	{
		m_flOldCharge = 1.0f;
		SetVisible( false );
		return;
	}

	SetVisible( true );

	float flCharge = m_hWeapon->GetEffectBarProgress();
	m_pEffectMeter->SetProgress( flCharge );

	// Play a ding when full charged.
	if ( m_flOldCharge < 1.0f && flCharge == 1.0f && !m_hWeapon->IsWeapon( TF_WEAPON_INVIS ) )
	{
		CLocalPlayerFilter filter;
		C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "TFPlayer.Recharged" );
	}

	m_flOldCharge = flCharge;

	SetDialogVariable( "progresscount", pWeapon->GetCount() );
}

#define TF_NUM_EFFECT_METERS 3

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CHudItemEffects : public CHudElement, public EditablePanel
{
	DECLARE_CLASS_SIMPLE( CHudItemEffects, EditablePanel );

public:
	CHudItemEffects( const char *pElementName );
	~CHudItemEffects();

	virtual void ApplySchemeSettings( IScheme *pScheme );
	virtual bool ShouldDraw( void );
	virtual void OnTick( void );

private:
	CHudItemEffectMeter *m_pItemBars[TF_NUM_EFFECT_METERS];
	CHudItemEffectMeter *m_pItemCounters[TF_NUM_EFFECT_METERS];
	CHudItemEffectMeter *m_pLegacyMeter;
	bool m_bUsingLegacyMeter;
};

DECLARE_HUDELEMENT( CHudItemEffects );

CHudItemEffects::CHudItemEffects( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudItemEffects" )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	// Create effect bars.
	for ( int i = 0; i < TF_NUM_EFFECT_METERS; i++ )
	{
		m_pItemBars[i] = new CHudItemEffectMeter( this, VarArgs( "ItemMeter%d", i + 1 ) );
		m_pItemCounters[i] = new CHudItemEffectMeter( this, VarArgs( "ItemCounter%d", i + 1 ) );
	}

	m_pLegacyMeter = new CHudItemEffectMeter( this, "HudItemEffectMeter" );
	m_bUsingLegacyMeter = false;

	SetHiddenBits( HIDEHUD_MISCSTATUS );

	ivgui()->AddTickSignal( GetVPanel() );
}

CHudItemEffects::~CHudItemEffects()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CHudItemEffects::ShouldDraw( void )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer || !pPlayer->IsAlive() )
		return false;

	return CHudElement::ShouldDraw();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffects::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/UI/HudItemEffects.res" );

	// This is for compatibility with live TF2. Our RES file hides the element so we use the new HUD by default.
	m_pLegacyMeter->LoadControlSettings( "Resource/UI/HudItemEffectMeter.res" );
	m_bUsingLegacyMeter = m_pLegacyMeter->IsVisible();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffects::OnTick( void )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer )
		return;

	// Make the list of weapons.
	CUtlVector<C_TFWeaponBase *> aBarWeapons;
	CUtlVector<C_TFWeaponBase *> aCounterWeapons;

	// Traverse backwards since 1 is the rightmost icon and we want to show slots from left to right.
	for ( int i = TF_LOADOUT_SLOT_PDA2; i >= 0; i-- )
	{
		C_TFWeaponBase *pWeapon = pPlayer->GetWeaponForLoadoutSlot( (ETFLoadoutSlot)i );

		if ( pWeapon && pWeapon->HasEffectMeter() )
		{
			// GetCount() != -1 means we want to show a numeric counter above the normal ones.
			if ( pWeapon->GetCount() == -1 )
			{
				aBarWeapons.AddToTail( pWeapon );
			}
			else
			{
				aCounterWeapons.AddToTail( pWeapon );
			}
		}
	}

	if ( m_bUsingLegacyMeter )
	{
		if ( aBarWeapons.Count() != 0 )
		{
			m_pLegacyMeter->UpdateStatus( aBarWeapons[0] );
		}
		else
		{
			m_pLegacyMeter->UpdateStatus( NULL );
		}

		for ( int i = 0; i < TF_NUM_EFFECT_METERS; i++ )
		{
			m_pItemBars[i]->UpdateStatus( NULL );
		}

		for ( int i = 0; i < TF_NUM_EFFECT_METERS; i++ )
		{
			m_pItemCounters[i]->UpdateStatus( NULL );
		}
		return;
	}

	m_pLegacyMeter->UpdateStatus( NULL );

	// Update all meters.
	for ( int i = 0; i < TF_NUM_EFFECT_METERS; i++ )
	{
		CHudItemEffectMeter *pMeter = m_pItemBars[i];

		if ( i < aBarWeapons.Count() )
		{
			pMeter->UpdateStatus( aBarWeapons[i] );
		}
		else
		{
			pMeter->UpdateStatus( NULL );
		}
	}

	for ( int i = 0; i < TF_NUM_EFFECT_METERS; i++ )
	{
		CHudItemEffectMeter *pMeter = m_pItemCounters[i];

		if ( i < aCounterWeapons.Count() )
		{
			pMeter->UpdateStatus( aCounterWeapons[i] );
		}
		else
		{
			pMeter->UpdateStatus( NULL );
		}
	}
}
