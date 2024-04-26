#ifndef TFMAINMENUBACKGROUNDPANEL_H
#define TFMAINMENUBACKGROUNDPANEL_H

#include "vgui_controls/Panel.h"
#include "tf_menupanelbase.h"
#include "tf_vgui_video.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFBackgroundPanel : public CTFMenuPanelBase
{
	DECLARE_CLASS_SIMPLE( CTFBackgroundPanel, CTFMenuPanelBase );

public:
	CTFBackgroundPanel( vgui::Panel* parent, const char *panelName );
	virtual ~CTFBackgroundPanel();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void SetVisible( bool bVisible );

	MESSAGE_FUNC( VideoReplay, "IntroFinished" );

private:
	CTFVideoPanel		*m_pVideo;
	char				m_szVideoFile[MAX_PATH];
	void				GetRandomVideo( char *pszBuf, int iBufLength, bool bWidescreen );
};

#endif // TFMAINMENUBACKGROUNDPANEL_H
