#include "cbase.h"
#include "iclientmode.h"
#include "tf_hud_vip.h"
#include "vgui/IScheme.h"
#include "tf_gamerules.h"
#include "c_tf_team.h"
#include "c_team_objectiveresource.h"

CTFHudVIP::CTFHudVIP( vgui::Panel *pParent, const char *pszName ) : vgui::EditablePanel( pParent, pszName )
{
	vgui::Panel *pViewport = g_pClientMode->GetViewport();
	SetParent( pViewport );
	
	m_flPreviousProgress = 0.f;

	m_pLevelBar = new vgui::ImagePanel( this, "LevelBar" );
	m_pEscortItemPanel = new vgui::EditablePanel( this, "EscortItemPanel" );

	vgui::ivgui()->AddTickSignal( GetVPanel() );
}

CTFHudVIP::~CTFHudVIP()
{
}

void CTFHudVIP::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/ui/HudObjectiveVIP.res" );
}

void CTFHudVIP::OnTick()
{
	// TODO: VIP MODE
#if 0
	if( !ObjectiveResource() )
	{
		return;
	}

	float flProgress = ObjectiveResource()->GetVIPProgress( TF_TEAM_BLUE ); // TODO: HARDCODED VIP TEAM (3)
	float flNewProgress = 0.001f;

	if( (flProgress - m_flPreviousProgress) > 0.001f )
	{
		if( flProgress <= m_flPreviousProgress )
		{
			flNewProgress = 0.001f;
		}

		flProgress = m_flPreviousProgress + flNewProgress;
	}

	m_flPreviousProgress = flProgress;

	int x, y, wide, tall;
	m_pLevelBar->GetBounds( x, y, wide, tall );

	int nNewX = (((float)wide * flProgress) - m_pEscortItemPanel->GetWide()) / 2;

	m_pEscortItemPanel->SetPos( nNewX + x, m_pEscortItemPanel->GetYPos() );
#endif
}

bool CTFHudVIP::IsVisible()
{
	return !IsInFreezeCam() && BaseClass::IsVisible();
}
