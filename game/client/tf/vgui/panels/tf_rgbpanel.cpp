#include "KeyValues.h"
#include "cbase.h"
#include "tf_rgbpanel.h"
#include <vgui_controls/ComboBox.h>
#include "controls/tf_cvarslider.h"
#include <vgui/ILocalize.h>
#include "c_tf_player.h"
#include "tf_merc_customizations.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

extern ConVar tf2c_merc_particle;
extern ConVar tf2c_merc_winanim;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFRGBPanel::CTFRGBPanel( Panel *parent, const char *panelName ) : BaseClass( parent, panelName )
{
	m_pRedScrollBar = new CTFCvarSlider( this, "RedScrollBar" );
	m_pGrnScrollBar = new CTFCvarSlider( this, "GrnScrollBar" );
	m_pBluScrollBar = new CTFCvarSlider( this, "BluScrollBar" );
	m_pColorBG = new ImagePanel( this, "ColorBG" );
	m_pCombo = new ComboBox( this, "ParticleComboBox", 5, false );
	m_pAnimCombo = new ComboBox( this, "WinAnimComboBox", 5, false );
}

void CTFRGBPanel::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/UI/main_menu/RGBPanel.res" );

	m_pCombo->RemoveAll();
	m_pAnimCombo->RemoveAll();

	for( int i = 0; i < g_TFMercCustomizations.GetParticlesCount(); i++ )
	{
		respawn_particle_t *pRespawnParticle = g_TFMercCustomizations.GetParticle( i );
		if( !pRespawnParticle )
			continue;
		
		KeyValues *pKV = new KeyValues( "particle" );
		pKV->SetInt( "id", pRespawnParticle->id );
		pKV->SetString( "effect", pRespawnParticle->szEffect );
		int itemID = m_pCombo->AddItem( pRespawnParticle->szName, pKV );
		pKV->deleteThis();
		
		if( pRespawnParticle->id == tf2c_merc_particle.GetInt() )
		{
			m_pCombo->SilentActivateItem( itemID );
		}
	}
	
	for( int i = 0; i < g_TFMercCustomizations.GetWinAnimCount(); i++ )
	{
		win_anim_t *pWinAnim = g_TFMercCustomizations.GetAnimation( i );
		if( !pWinAnim )
			continue;
		
		KeyValues *pKV = new KeyValues( "animation" );
		pKV->SetInt( "id", pWinAnim->id );
		pKV->SetString( "sequence", pWinAnim->szSequence );
		int itemID = m_pAnimCombo->AddItem( pWinAnim->szName, pKV );
		pKV->deleteThis();
		
		if( pWinAnim->id == tf2c_merc_winanim.GetInt() )
		{
			m_pAnimCombo->SilentActivateItem( itemID );
		}
	}

	if ( tf2c_setmerccolor_r.GetFloat() == 0.0f && tf2c_setmerccolor_g.GetFloat() == 0.0f && tf2c_setmerccolor_b.GetFloat() == 0.0f )
	{
		// 0 0 0 is default value which disables the proxy. So that means this is the first game launch.
		// Let's generate a random color...
		tf2c_setmerccolor_r.SetValue( RandomInt( 0, 255 ) );
		tf2c_setmerccolor_g.SetValue( RandomInt( 0, 255 ) );
		tf2c_setmerccolor_b.SetValue( RandomInt( 0, 255 ) );
		m_pRedScrollBar->Reset();
		m_pGrnScrollBar->Reset();
		m_pBluScrollBar->Reset();
		
		OnControlModified( this );
	}
}

void CTFRGBPanel::PerformLayout()
{
	BaseClass::PerformLayout();

	OnControlModified( this );
}

const char *CTFRGBPanel::GetSpawnEffect()
{
	KeyValues *pKeys = m_pCombo->GetActiveItemUserData();
	if ( pKeys )
	{
		return pKeys->GetString( "effect" );
	}
		
	return NULL;
}

void CTFRGBPanel::OnControlModified( Panel *panel )
{
	PostActionSignal( new KeyValues( "ControlModified" ) );

	// Set the color on the panel.
	Color clr( m_pRedScrollBar->GetValue(), m_pGrnScrollBar->GetValue(), m_pBluScrollBar->GetValue(), 255 );
	m_pColorBG->SetFillColor( clr );
}

void CTFRGBPanel::OnTextChanged( Panel *panel )
{
	if( panel == m_pCombo )
	{
		KeyValues *pKeys = m_pCombo->GetActiveItemUserData();

		if ( pKeys )
		{
			tf2c_merc_particle.SetValue( pKeys->GetInt( "id" ) );
		}
		
		OnControlModified( this );
	}
	else if ( panel == m_pAnimCombo )
	{
		KeyValues *pKeys = m_pAnimCombo->GetActiveItemUserData();

		if ( pKeys )
		{
			tf2c_merc_winanim.SetValue( pKeys->GetInt( "id" ) );
		}
	}
}
