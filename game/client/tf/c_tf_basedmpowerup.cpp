//====== Copyright © 1996-2013, Valve Corporation, All rights reserved. =======//
//
// Purpose: Base class for Deathmatch powerups 
//
//=============================================================================//
#include "cbase.h"
#include "c_tf_powerup.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar tf2c_powerup_respawn_timer( "tf2c_powerup_respawn_timer", "1", FCVAR_REPLICATED, "Show visual respawn timers for power-ups in Deathmatch." );
ConVar tf2c_powerup_respawn_size( "tf2c_powerup_respawn_size", "35", FCVAR_CHEAT ); // SanyaSho: was 40.0f

class C_TFBaseDMPowerup : public C_TFPowerup
{
public:
	DECLARE_CLASS( C_TFBaseDMPowerup, C_TFPowerup );
	DECLARE_CLIENTCLASS();

	C_TFBaseDMPowerup();
	~C_TFBaseDMPowerup();

	void	OnDataChanged( DataUpdateType_t type );

	void	Spawn( void );
	void	ClientThink( void );
	void	UpdateGlowEffect( void );

	virtual bool ShouldShowRespawnTimer( void ) { return tf2c_powerup_respawn_timer.GetBool(); }
	virtual float GetRespawnTimerSize( void ) { return tf2c_powerup_respawn_size.GetFloat(); }
	virtual Color GetRespawnTimerColor( void ) { return Color( 190, 190, 40, 192 ); }

private:
	QAngle		m_qAngle;

	CGlowObject		   *m_pGlowEffect;
	bool				m_bShouldGlow;
	bool				m_bShouldFlash;
	float				m_flAlpha;
	bool				m_bAlphaRising;
};

IMPLEMENT_CLIENTCLASS_DT( C_TFBaseDMPowerup, DT_TFBaseDMPowerup, CTFBaseDMPowerup )
END_RECV_TABLE()

C_TFBaseDMPowerup::C_TFBaseDMPowerup()
{
	m_qAngle.Init();

	m_pGlowEffect = NULL;
	m_bShouldGlow = false;
	m_flAlpha = 0.0f;
	m_bAlphaRising = false;
}

C_TFBaseDMPowerup::~C_TFBaseDMPowerup()
{
	delete m_pGlowEffect;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void C_TFBaseDMPowerup::OnDataChanged( DataUpdateType_t type )
{
	BaseClass::OnDataChanged( type );

	if ( type == DATA_UPDATE_CREATED )
	{
		SetNextClientThink( CLIENT_THINK_ALWAYS );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void C_TFBaseDMPowerup::Spawn( void )
{
	BaseClass::Spawn();
	m_qAngle = GetAbsAngles();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void C_TFBaseDMPowerup::ClientThink( void )
{
	m_qAngle.y += 90 * gpGlobals->frametime;
	if ( m_qAngle.y >= 360 )
		m_qAngle.y -= 360;

	SetAbsAngles( m_qAngle );

	bool bShouldGlow = false;
	bool bShouldFlash = false;

	// Powerups glow through walls.
	if ( !m_bDisabled )
	{
		if ( !m_bRespawning )
		{
			bShouldGlow = true;
			bShouldFlash = false;
		}
		else if ( m_flRespawnTime - gpGlobals->curtime < 15.0f )
		{
			// Start flashing 15 sseconds before respawning.
			bShouldGlow = true;
			bShouldFlash = true;
		}
	}
	
	if ( m_bShouldGlow != bShouldGlow ||
		bShouldFlash ||
		m_bShouldFlash != bShouldFlash )
	{
		m_bShouldGlow = bShouldGlow;
		m_bShouldFlash = bShouldFlash;
		UpdateGlowEffect();
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void C_TFBaseDMPowerup::UpdateGlowEffect( void )
{
	if ( !m_pGlowEffect )
	{
		m_pGlowEffect = new CGlowObject( this, Vector( 0.75f, 0.75f, 0.15f ), 1.0f, true, true );
	}

	if ( !m_bShouldGlow )
	{
		m_flAlpha = 0.0f;
		m_pGlowEffect->SetAlpha( m_flAlpha );
	}
	else
	{
		if ( m_bShouldFlash )
		{
			if ( m_bAlphaRising )
			{
				if ( m_flAlpha == 1.0f )
				{
					m_bAlphaRising = !m_bAlphaRising;
				}
			}
			else
			{
				if ( m_flAlpha <= 0.5f )
				{
					m_bAlphaRising = !m_bAlphaRising;
				}
			}

			m_flAlpha = Approach( m_bAlphaRising ? 1.0f : 0.5f, m_flAlpha, 0.5f * gpGlobals->frametime );
		}
		else
		{
			m_flAlpha = 1.0f;
		}

		m_pGlowEffect->SetAlpha( m_flAlpha );
	}
}
