#ifndef tf_advbuttonbase_H
#define tf_advbuttonbase_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_mainmenu.h"
#include "tf_controls.h"

class CTFButtonBase;
class CTFToolTipPanel;
class CTFDialogPanelBase;

enum MouseState
{
	MOUSE_DEFAULT,
	MOUSE_ENTERED,
	MOUSE_EXITED,
	MOUSE_PRESSED,
	MOUSE_RELEASED
};

#define GETSCHEME()			scheme()->GetIScheme(GetScheme())

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFButtonBase : public vgui::Button
{
public:
	DECLARE_CLASS_SIMPLE( CTFButtonBase, vgui::Button );

	CTFButtonBase( vgui::Panel *parent, const char *panelName, const char *text );
	virtual ~CTFButtonBase();

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void ApplySettings( KeyValues *inResourceData );

	// Set armed button border attributes.
	virtual void SetArmedBorder( vgui::IBorder *border );
	virtual void SetSelectedBorder( vgui::IBorder *border );
	virtual void SetDisabledBorder( vgui::IBorder *border );

	// Get button border attributes.
	virtual vgui::IBorder *GetBorder( bool depressed, bool armed, bool selected, bool keyfocus );

	virtual void OnCursorExited();
	virtual void OnCursorEntered();
	virtual void OnMousePressed( vgui::MouseCode code );
	virtual void OnMouseReleased( vgui::MouseCode code );
	virtual MouseState GetState() { return m_iMouseState; };
	virtual void SetArmed( bool bState );
	virtual void SetSelected( bool bState );
	virtual bool RequestInfo( KeyValues *data );
	virtual bool SetInfo( KeyValues *inputData );

	void SetBordersByName( const char *pszBorderDefault, const char *pszBorderArmed, const char *pszBorderDepressed );
	void SetFontByString( const char *sFont );

	virtual void SetImage( const char *sImage );
	virtual void SetImageInset( int iInsetX, int iInsetY );
	virtual void SetImageSize( int iWide, int iTall );

	virtual void ShowToolTip( bool bShow );
	virtual void SetToolTip( const char *pszText );

protected:
	virtual void	SetMouseEnteredState( MouseState flag );
	vgui::IBorder			*_armedBorder;
	vgui::IBorder			*_selectedBorder;
	vgui::IBorder			*_disabledBorder;
	bool			m_bBorderVisible;
	MouseState		m_iMouseState;

	vgui::ImagePanel *m_pButtonImage;

	char			m_szDefaultBG[64];
	char			m_szArmedBG[64];
	char			m_szDepressedBG[64];
	char			m_szDisabledBG[64];

	Color			m_colorImageDefault;
	Color			m_colorImageArmed;
	Color			m_colorImageDepressed;

	char			m_szToolTip[256];
};


#endif // tf_advbuttonbase_H
