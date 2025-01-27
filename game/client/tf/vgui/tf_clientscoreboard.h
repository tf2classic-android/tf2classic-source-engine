//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef TF_SCOREBOARD_H
#define TF_SCOREBOARD_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include <game/client/iviewport.h>
#include "GameEventListener.h"
#include "tf_controls.h"
#include "tf_shareddefs.h"

#define TF_SCOREBOARD_MAX_DOMINATIONS 16

class CTFClassImage;
class CTFPlayerModelPanel;

enum
{
	DEVICE_COMPUTER = 0,
	DEVICE_PHONE,
	DEVICE_BOT,
	DEVICE_MAX
};

//-----------------------------------------------------------------------------
// Purpose: displays the MapInfo menu
//-----------------------------------------------------------------------------

class CTFClientScoreBoardDialog : public vgui::EditablePanel, public IViewPortPanel, public CGameEventListener
{
public:
	DECLARE_CLASS_SIMPLE( CTFClientScoreBoardDialog, vgui::EditablePanel );

	CTFClientScoreBoardDialog( IViewPort *pViewPort, const char *pszName, int iTeamCount = 4 ); // default is 4 (unassign,spec,red,blue)
	virtual ~CTFClientScoreBoardDialog();

	virtual void PerformLayout();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

	virtual const char *GetName( void ) { return PANEL_SCOREBOARD; }
	virtual void SetData( KeyValues *data ) {}
	virtual void Reset();
	virtual void Update();
	virtual bool NeedsUpdate( void );
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );
	virtual void OnCommand( const char *command );
	virtual void OnThink( void );

	virtual GameActionSet_t GetPreferredActionSet() { return GAME_ACTION_SET_NONE; }

	// both vgui::EditablePanel and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
	virtual bool IsVisible() { return BaseClass::IsVisible(); }
	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

	void UpdatePlayerAvatar( int playerIndex, KeyValues *kv );
	virtual int GetDefaultAvatar( int playerIndex );

	int	HudElementKeyInput( int down, ButtonCode_t keynum, const char *pszCurrentBinding );
	
protected:
	virtual void InitPlayerList( vgui::SectionedListPanel *pPlayerList );
	void SetPlayerListImages( vgui::SectionedListPanel *pPlayerList );
	virtual void UpdateTeamInfo();
	virtual void UpdatePlayerList();
	virtual void UpdateSpectatorList();
	virtual void UpdatePlayerDetails( void );
	virtual void UpdateArenaWaitingToPlayList();
	void MoveToCenterOfScreen();
	
	virtual const char *GetResFilename()
	{
		return "Resource/UI/Scoreboard.res";
	}

	virtual void FireGameEvent( IGameEvent *event );

	vgui::SectionedListPanel *GetSelectedPlayerList( void );

	static bool TFPlayerSortFunc( vgui::SectionedListPanel *list, int itemID1, int itemID2 );

	vgui::SectionedListPanel	*m_pPlayerLists[TF_TEAM_COUNT];

	vgui::ImageList				*m_pImageList;
#if defined( ENABLE_STEAM_AVATARS )
	CUtlMap<CSteamID, int>		m_mapAvatarsToImageList;
#else
	CUtlMap< int, int >			m_mapAvatarsToImageList;
#endif

	CPanelAnimationVar( int, m_iDeviceWidth, "device_width", "34" );		// Avatar width doesn't scale with resolution
	CPanelAnimationVar( int, m_iAvatarWidth, "avatar_width", "34" );		// Avatar width doesn't scale with resolution
	CPanelAnimationVarAliasType( int, m_iNameWidth, "name_width", "102", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iClassWidth, "class_width", "35", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iScoreWidth, "score_width", "35", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iPingWidth, "ping_width", "23", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iStatusWidth, "status_width", "15", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iNemesisWidth, "nemesis_width", "15", "proportional_int" );

	int							m_iImageDead;
	int							m_iImageDominated;
	int							m_iImageNemesis;
	int							m_iClassEmblem[TF_CLASS_COUNT_ALL];
	int							m_iClassEmblemDead[TF_CLASS_COUNT_ALL];
	int							m_iImageDominations[TF_SCOREBOARD_MAX_DOMINATIONS];
	int							m_iDefaultAvatars[TF_TEAM_COUNT];
	int							m_iDeviceIcons[DEVICE_MAX];

private:
	MESSAGE_FUNC_INT( OnPollHideCode, "PollHideCode", code );
	MESSAGE_FUNC_PARAMS( ShowContextMenu, "ItemContextMenu", data );
	
	int m_iTeamCount;
	
	CExLabel					*m_pLabelPlayerName;
	CExLabel					*m_pLabelMapName;
	vgui::ImagePanel				*m_pImagePanelHorizLine;
	CTFClassImage				*m_pClassImage;
	CTFPlayerModelPanel			*m_pClassModelPanel;
	vgui::EditablePanel				*m_pLocalPlayerStatsPanel;
	vgui::EditablePanel				*m_pLocalPlayerDuelStatsPanel;
	CExLabel					*m_pSpectatorsInQueue;
	CExLabel					*m_pServerTimeLeftValue;
	vgui::HFont				m_hTimeLeftFont;
	vgui::HFont				m_hTimeLeftNotSetFont;
	vgui::Menu				*m_pContextMenu;

	float					m_flNextUpdateTime;
	ButtonCode_t				m_nCloseKey;
	int					m_iSelectedPlayerIndex;
};

#endif // TF_SCOREBOARD_H
