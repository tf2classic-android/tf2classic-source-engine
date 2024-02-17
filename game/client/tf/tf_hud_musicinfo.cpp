#include "c_baseplayer.h"
#include "cbase.h"
#include "cdll_util.h"
#include "tf_hud_musicinfo.h"
#include "util_shared.h"
#include "vgui/IScheme.h"
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
	// FIXME: ???????

	/*
	if( (m_flShowAt == 0.f) || gpGlobals->curtime < m_flShowAt )
	{
		if( (m_flHideAt != 0.f) && gpGlobals->curtime >= m_flHideAt )
		{
			vgui::GetAnimationController()->RunAnimationCommand( m_pInfoPanel, "alpha", 0.0f, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
			vgui::GetAnimationController()->RunAnimationCommand( m_pNameLabel, "alpha", 0.0f, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
			vgui::GetAnimationController()->RunAnimationCommand( m_pComposerLabel, "alpha", 0.0f, 0.5f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
			vgui::GetAnimationController()->RunAnimationCommand( this, "wide", GetTall(), 0.25f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		}
	}
	else
	{
		int nameWide, nameTall;
		m_pNameLabel->GetSize( nameWide, nameTall );
		int composerWide, composerTall;
		m_pComposerLabel->GetSize( composerWide, composerTall );

		int newWide = composerWide;
		if( nameWide > composerWide )
			newWide = nameWide;

		int height = (int)(ScreenHeight() * 0.002f * 10.f + newWide);

		vgui::GetAnimationController()->RunAnimationCommand( m_pInfoPanel, "alpha", 0.0f, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( m_pNameLabel, "alpha", 0.0f, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( m_pComposerLabel, "alpha", 0.0f, 0.5f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( this, "wide", GetTall(), 0.25f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
	}
	*/
	/*
	if( true )
	{
		vgui::GetAnimationController()->RunAnimationCommand( this, "alpha", 0.0f, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );

		vgui::GetAnimationController()->RunAnimationCommand( this, "alpha", 0.0f, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( this, "alpha", 0.0f, 0.5f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( this, "wide", GetTall(), 0.25f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
	}
	else
	{
		int nameWide, nameTall;
		m_pNameLabel->GetSize( nameWide, nameTall );
		int composerWide, composerTall;
		m_pComposerLabel->GetSize( composerWide, composerTall );

		int newWide = composerWide;
		if( nameWide > composerWide )
			newWide = nameWide;

		int height = (int)(ScreenHeight() * 0.002f * 10.f + newWide);

		vgui::GetAnimationController()->RunAnimationCommand( this, "alpha", 0.0f, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( m_pInfoPanel, "alpha", 0.0f, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( m_pNameLabel, "alpha", 0.0f, 0.5f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		vgui::GetAnimationController()->RunAnimationCommand( m_pComposerLabel, "wide", GetTall(), 0.25f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
	}
	*/
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
		const char *pszSongName = event->GetString( "name" );
		const char *pszSongCompositor = event->GetString( "composer" );

		m_pInfoPanel->SetDialogVariable( "songname", pszSongName );
		m_pInfoPanel->SetDialogVariable( "composername", pszSongCompositor );

		Msg( "Playing:\n\t%s by %s\n", pszSongName, pszSongCompositor );

		ResetAnimation();

		m_flShowAt = gpGlobals->curtime + 1.0f;
	}
	else if( !Q_stricmp( pszEventName, "song_stopped" ) )
	{
		ResetAnimation();
	}
}
