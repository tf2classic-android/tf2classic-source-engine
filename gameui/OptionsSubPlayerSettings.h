//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef OPTIONSSUBPLAYERSETTINGS_H
#define OPTIONSSUBPLAYERSETTINGS_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/PropertyPage.h>
#include <vgui_controls/ImagePanel.h>
#include "imageutils.h"

class CLabeledCommandComboBox;
class CCvarTextEntry;

//-----------------------------------------------------------------------------
// Purpose: multiplayer options property page
//-----------------------------------------------------------------------------
class COptionsSubPlayerSettings : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE( COptionsSubPlayerSettings, vgui::PropertyPage );

public:
	COptionsSubPlayerSettings( vgui::Panel *parent );
	virtual ~COptionsSubPlayerSettings();

	MESSAGE_FUNC( OnControlModified, "ControlModified" );

protected:
	// Called when the OK / Apply button is pressed.  Changed data should be written into document.
	virtual void OnApplyChanges();

	virtual void OnCommand( const char *command );

private:
	void InitAvatarList(CLabeledCommandComboBox *cb);

	void RemapAvatar();

	void ConversionError( ConversionErrorType nError );

	MESSAGE_FUNC_PTR( OnTextChanged, "TextChanged", panel );
	MESSAGE_FUNC_CHARPTR( OnFileSelected, "FileSelected", fullpath );

	CCvarTextEntry *m_pNameEntry;

	vgui::ImagePanel *m_pAvatarImage;
	CLabeledCommandComboBox *m_pAvatarList;
	char m_AvatarName[128];

	// Begin Avatar Import Functions
	void SelectAvatar( const char *avatarName );
	// End Avatar Import Functions

	vgui::FileOpenDialog *m_hImportAvatarDialog;
};

#endif // OPTIONSSUBPLAYERSETTINGS_H
