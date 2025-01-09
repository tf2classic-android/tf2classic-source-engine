#include "c_baseplayer.h"
#include "cbase.h"
#include "cdll_util.h"
#include "tf_hud_musicinfo.h"
#include "util_shared.h"
#include "vgui/IScheme.h"
#include "vgui_controls/TextImage.h"
#include "iclientmode.h"
#include "tf_music_controller.h"

DECLARE_HUDELEMENT( CTFHudMusicInfo );

CTFHudMusicInfo::CTFHudMusicInfo( const char *pElementName ) : CHudElement( pElementName ), vgui::EditablePanel( NULL, "HudMusicInfo" )
{
	vgui::Panel *pViewport = g_pClientMode->GetViewport();
	SetParent( pViewport );
	
	m_pInfoPanel = new vgui::EditablePanel( this, "InfoPanel" );
	m_pNameLabel = new CExLabel( m_pInfoPanel, "NameLabel", "" );
	m_pComposerLabel = new CExLabel( m_pInfoPanel, "ComposerLabel", "" );

	m_flHideAt = 0.f;
	m_flShowAt = 0.f;

	vgui::ivgui()->AddTickSignal( GetVPanel() );

	ListenForGameEvent( "song_started" );
	ListenForGameEvent( "song_stopped" );
}

CTFHudMusicInfo::~CTFHudMusicInfo()
{
}

void CTFHudMusicInfo::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
	LoadControlSettings( "resource/ui/HudMusicInfo.res" );
}

void CTFHudMusicInfo::LevelInit()
{
	ResetAnimation();
}

void CTFHudMusicInfo::OnTick()
{
	// Can't put this into HUD animations script file since we have to set the width through the code.
	if ( m_flShowAt != 0.0f && gpGlobals->curtime >= m_flShowAt )
	{
		// Calculate width.
		int nameWide, nameTall, composerWide, composerTall;
		m_pNameLabel->GetTextImage()->GetContentSize( nameWide, nameTall );
		m_pComposerLabel->GetTextImage()->GetContentSize( composerWide, composerTall );

		int iPanelWidth = Max( nameWide, composerWide ) + YRES( 10 );

		vgui::GetAnimationController()->RunAnimationCommand( m_pInfoPanel, "alpha", 255.0f, 0.0f, 0.5f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( m_pInfoPanel, "wide", iPanelWidth, 0.5f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );

		vgui::GetAnimationController()->RunAnimationCommand( m_pNameLabel, "alpha", 255.0f, 0.65f, 0.1f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( m_pComposerLabel, "alpha", 255.0f, 0.65f, 0.1f, vgui::AnimationController::INTERPOLATOR_LINEAR );

		// Show it for 3 seconds.
		m_flHideAt = gpGlobals->curtime + 4.0f;
		m_flShowAt = 0.0f;
	}
	else if ( m_flHideAt != 0.0f && gpGlobals->curtime >= m_flHideAt )
	{
		vgui::GetAnimationController()->RunAnimationCommand( m_pNameLabel, "alpha", 0.0f, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( m_pComposerLabel, "alpha", 0.0f, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );

		vgui::GetAnimationController()->RunAnimationCommand( m_pInfoPanel, "alpha", 0.0f, 0.5f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( m_pInfoPanel, "wide", GetTall(), 0.25f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );

		m_flHideAt = 0.0f;
	}
}

void CTFHudMusicInfo::ResetAnimation()
{
	m_flHideAt = 0.f;
	m_flShowAt = 0.f;
	vgui::GetAnimationController()->CancelAnimationsForPanel( this );
	m_pInfoPanel->SetAlpha( 0.f );
	m_pInfoPanel->SetWide( GetTall() );
	m_pNameLabel->SetAlpha( 0.f );
	m_pComposerLabel->SetAlpha( 0.f );
}

bool CTFHudMusicInfo::ShouldDraw()
{
	return (g_pMusicController && g_pMusicController->IsPlaying() && !g_pMusicController->InWaiting()) && !IsInFreezeCam() && CHudElement::ShouldDraw();
}

void CTFHudMusicInfo::FireGameEvent( IGameEvent *event )
{
	const char *pszEventName = event->GetName();
	if( !Q_stricmp( pszEventName, "song_started" ) )
	{
		m_pInfoPanel->SetDialogVariable( "songname", event->GetString( "name" ) );
		m_pInfoPanel->SetDialogVariable( "composername", event->GetString( "composer" ) );

		ResetAnimation();

		m_flShowAt = gpGlobals->curtime + 1.0f;
	}
	else if( !Q_stricmp( pszEventName, "song_stopped" ) )
	{
		ResetAnimation();
	}
}
