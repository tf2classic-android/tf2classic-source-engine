#ifndef TFMAINMENUPANEL_H
#define TFMAINMENUPANEL_H

#include "tf_menupanelbase.h"
#include "steam/steam_api.h"

class CAvatarImagePanel;
class CTFButton;
class CTFSlider;

enum MusicStatus
{
	MUSIC_STOP,
	MUSIC_FIND,
	MUSIC_PLAY,
	MUSIC_STOP_FIND,
	MUSIC_STOP_PLAY,
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFMainMenuPanel : public CTFMenuPanelBase
{
	DECLARE_CLASS_SIMPLE( CTFMainMenuPanel, CTFMenuPanelBase );

public:
	CTFMainMenuPanel( vgui::Panel* parent, const char *panelName );
	virtual ~CTFMainMenuPanel();

	virtual void PerformLayout();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void OnTick();
	virtual void OnThink();
	virtual void Show();
	virtual void Hide();
	virtual void OnCommand( const char* command );

	void HideFakeIntro( void );

	void SetVersionLabel();
	void PlayMusic();
	void OnNotificationUpdate();

private:
	void GetRandomMusic(char *pszBuf, int iBufLength);

	CExLabel			*m_pVersionLabel;
	CTFButton		*m_pNotificationButton;
	CAvatarImagePanel	*m_pProfileAvatar;
	vgui::ImagePanel	*m_pFakeBGImage;

	int					m_iShowFakeIntro;

	char				m_pzMusicLink[64];	
	int					m_nSongGuid;
	MusicStatus			m_psMusicStatus;

	CSteamID			m_SteamID;
};

#endif // TFMAINMENUPANEL_H
