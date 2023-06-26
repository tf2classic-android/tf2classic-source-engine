#ifndef TF_MAINMENU_SCROLLBAR_H
#define TF_MAINMENU_SCROLLBAR_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_advbuttonbase.h"
#include "tf_imagepanel.h"

class CTFScrollButton;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFSlider : public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE( CTFSlider, vgui::EditablePanel );

	CTFSlider( vgui::Panel *parent, const char *panelName, const char *text );
	~CTFSlider();

	virtual void Init();
	virtual void ApplySettings( KeyValues *inResourceData );
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void PerformLayout();
	virtual void OnThink( void );

	void SetFont( vgui::HFont font );
	float GetValue();
	const char *GetFinalValue();
	void SetPercentage( float fPerc, bool bInstant = false );
	void SetValue( float flValue );
	void SetRange( float flMin, float flMax );
	bool IsVertical() { return m_bVertical; }

	virtual void SendSliderMovedMessage();
	virtual void SendSliderDragStartMessage();
	virtual void SendSliderDragEndMessage();

	CTFScrollButton *GetButton() { return m_pButton; };

protected:
	CTFScrollButton	*m_pButton;
	vgui::Label			*m_pTitleLabel;
	vgui::Label			*m_pValueLabel;
	vgui::EditablePanel *m_pBGBorder;

	float			m_flMinValue;
	float			m_flMaxValue;
	int			m_iLabelWidth;
	float			m_flValue;
	bool			m_bValueVisible;
	bool			m_bVertical;
	bool			m_bShowFrac;
};


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFScrollButton : public CTFButtonBase
{
public:
	DECLARE_CLASS_SIMPLE( CTFScrollButton, CTFButtonBase );

	CTFScrollButton( vgui::Panel *parent, const char *panelName, const char *text );

	void ApplySettings( KeyValues *inResourceData );
	void ApplySchemeSettings( vgui::IScheme *pScheme );
	void PerformLayout();
	void OnCursorExited();
	void OnCursorEntered();
	void OnMousePressed( vgui::MouseCode code );
	void OnMouseReleased( vgui::MouseCode code );
	void SetMouseEnteredState( MouseState flag );
	void SetSliderPanel( CTFSlider *m_pButton ) { m_pSliderPanel = m_pButton; };

private:
	CTFSlider *m_pSliderPanel;
};


#endif // TF_MAINMENU_SCROLLBAR_H
