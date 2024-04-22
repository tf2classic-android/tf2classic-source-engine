#include "cbase.h"

#include <tier1/fmtstr.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/RichText.h>
#include <vgui_controls/Frame.h>
#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui_controls/SectionedListPanel.h>
#include <vgui_controls/ImageList.h>
#include <vgui_controls/Menu.h>
#include <vgui_controls/MenuItem.h>
#include <game/client/iviewport.h>
#include <KeyValues.h>
#include <filesystem.h>

#include "tf_clientscoreboard.h"
#include "tf_shareddefs.h"
#include "c_tf_player.h"
#include "c_tf_playerresource.h"
#include "tf_deathmatchscoreboard.h"
#include "tier3/tier3.h"
#include "vgui_avatarimage.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFDeathMatchScoreBoardDialog::CTFDeathMatchScoreBoardDialog( IViewPort *pViewPort, const char *pszName ) : CTFClientScoreBoardDialog( pViewPort, pszName, 3 )
{
	m_pPlayerList = m_pPlayerLists[TF_TEAM_RED];
	m_iDefaultAvatar = 0;
}

void CTFDeathMatchScoreBoardDialog::Update()
{
	BaseClass::Update();
	ResizeScoreboard();
	BaseClass::MoveToCenterOfScreen();
}

void CTFDeathMatchScoreBoardDialog::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
	
	CAvatarImage *m_pDefaultImage = new CAvatarImage();
	m_pDefaultImage->SetAvatarSize( 32, 32 );
	m_pDefaultImage->SetDefaultImage( scheme()->GetImage( "../vgui/avatar_default_merc", true ) );
	m_iDefaultAvatar = m_pImageList->AddImage( m_pDefaultImage );
	m_iDefaultTall = m_pPlayerList->GetTall();
}

void CTFDeathMatchScoreBoardDialog::InitPlayerList( vgui::SectionedListPanel *pPlayerList )
{
	pPlayerList->RemoveAll();
	pPlayerList->RemoveAllSections();
	pPlayerList->AddSection( 0, "Players", BaseClass::TFPlayerSortFunc );
	pPlayerList->SetSectionAlwaysVisible( 0 );
	pPlayerList->SetSectionFgColor( 0, Color( 255, 255, 255, 255 ) );
	pPlayerList->SetBgColor( Color( 0, 0, 0, 0 ) );
	pPlayerList->SetBorder( NULL );
	
	pPlayerList->AddColumnToSection( 0, "avatar", "", 2, this->m_iAvatarWidth, 0 );
	pPlayerList->AddColumnToSection( 0, "name", "#TF_Scoreboard_Name", 0, this->m_iNameWidth, 0 );
	pPlayerList->AddColumnToSection( 0, "status", "", 2, this->m_iStatusWidth, 0 );
	pPlayerList->AddColumnToSection( 0, "domination", "", 2, this->m_iNemesisWidth, 0 );
	pPlayerList->AddColumnToSection( 0, "nemesis", "", 2, this->m_iNemesisWidth, 0 );
	pPlayerList->AddColumnToSection( 0, "score", "#TF_Scoreboard_Score", 16, this->m_iScoreWidth, 0 );
	pPlayerList->AddColumnToSection( 0, "kills", "#TF_ScoreBoard_Kills", 16, this->m_iKillsWidth, 0 );
	pPlayerList->AddColumnToSection( 0, "deaths", "#TF_ScoreBoard_Deaths", 16, this->m_iDeathsWidth, 0 );
	pPlayerList->AddColumnToSection( 0, "streak", "#TF_ScoreBoard_KillStreak", 16, this->m_iKillstreakWidth, 0 );
	pPlayerList->AddColumnToSection( 0, "ping", "#TF_Scoreboard_Ping", 16, this->m_iPingWidth, 0 );
}

void CTFDeathMatchScoreBoardDialog::UpdatePlayerList()
{
	int iSelectedPlayerIndex = GetLocalPlayerIndex();
	
	// Save off which player we had selected
	int itemID = m_pPlayerList->GetSelectedItem();
	
	if ( itemID >= 0 )
	{
		KeyValues *pInfo = m_pPlayerList->GetItemData( itemID );
		if ( pInfo )
		{
			iSelectedPlayerIndex = m_pPlayerList->GetItemData( itemID )->GetInt( "playerIndex" );
		}
	}

	m_pPlayerList->RemoveAll();

	if ( !g_TF_PR )
		return;

	for ( int playerIndex = 1; playerIndex <= MAX_PLAYERS; playerIndex++ )
	{
		if ( !g_TF_PR->IsConnected( playerIndex ) )
			continue;

		int iTeam = g_PR->GetTeam( playerIndex );

		if ( iTeam < FIRST_GAME_TEAM )
			continue;
			
		C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
		if( !pPlayer )
			continue;

		KeyValues *pKeyValues = new KeyValues( "data" );

		pKeyValues->SetInt( "playerIndex", playerIndex );
		pKeyValues->SetString( "name", g_TF_PR->GetPlayerName( playerIndex ) );
		pKeyValues->SetInt( "score", g_TF_PR->GetTotalScore( playerIndex ) );
		pKeyValues->SetInt( "kills", g_TF_PR->GetPlayerScore( playerIndex ) );
		pKeyValues->SetInt( "deaths", g_TF_PR->GetDeaths( playerIndex ) );
		pKeyValues->SetInt( "streak", g_TF_PR->GetKillstreak( playerIndex ) );

		if ( g_TF_PR->IsPlayerDominating( playerIndex ) )
		{
			// if local player is dominated by this player, show a nemesis icon
			pKeyValues->SetInt( "nemesis", m_iImageNemesis );
			//pKeyValues->SetString( "class", "#TF_Nemesis" );
		}
		else if ( g_TF_PR->IsPlayerDominated( playerIndex ) )
		{
			// if this player is dominated by the local player, show the domination icon
			pKeyValues->SetInt( "nemesis", m_iImageDominated );
			//pKeyValues->SetString( "class", "#TF_Dominated" );
		}

		// display whether player is alive or dead (all players see this for all other players on both teams)
		pKeyValues->SetInt( "status", g_TF_PR->IsAlive( playerIndex ) ? 0 : m_iImageDead );

		// Show number of dominations.
		int iDominations = Min( g_TF_PR->GetNumberOfDominations( playerIndex ), TF_SCOREBOARD_MAX_DOMINATIONS );
		pKeyValues->SetInt( "domination", iDominations > 0 ? m_iImageDominations[iDominations - 1] : 0 );
		
		int iPing = g_PR->GetPing( playerIndex );
		if ( iPing < 1 )
		{
			if ( g_PR->IsFakePlayer( playerIndex ) )
			{
				pKeyValues->SetString( "ping", "#TF_Scoreboard_Bot" );
			}
			else
			{
				pKeyValues->SetString( "ping", "" );
			}
		}
		else
		{
			pKeyValues->SetInt( "ping", iPing );
		}

		BaseClass::UpdatePlayerAvatar( playerIndex, pKeyValues );

		int itemID = m_pPlayerLists[iTeam]->AddItem( 0, pKeyValues );

		Color clr = g_TF_PR->GetPlayerColor( playerIndex );
		clr[3] = g_TF_PR->IsAlive( playerIndex ) ? 204 : 51;

		m_pPlayerLists[iTeam]->SetItemFgColor( itemID, clr );
		
		if( iSelectedPlayerIndex == playerIndex )
		{
			m_pPlayerList->SetSelectedItem( itemID );
		}

		pKeyValues->deleteThis();
	}
}

int CTFDeathMatchScoreBoardDialog::GetDefaultAvatar( int playerIndex )
{
	return m_iDefaultAvatar;
}

void CTFDeathMatchScoreBoardDialog::ResizeScoreboard()
{
	// almost 1in1 from 2016
	int _wide, _tall;
	int wide, tall;
	int x, y;
	surface()->GetScreenSize( _wide, _tall );
	m_pPlayerList->GetContentSize( wide, tall );
	m_pPlayerList->GetPos( x, y );
	int yshift = y + scheme()->GetProportionalScaledValue( 10 );
	if ( tall > m_iDefaultTall  && tall + yshift < _tall )
	{
		m_pPlayerList->SetSize( wide, tall );
		tall += yshift;
		wide = GetWide();
		SetSize( wide, tall );
	}
}
