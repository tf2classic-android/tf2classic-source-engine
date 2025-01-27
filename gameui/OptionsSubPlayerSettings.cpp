//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//


#if defined( WIN32 ) && !defined( _X360 )
#include <windows.h> // SRC only!!
#endif

#include "OptionsSubPlayerSettings.h"
#include <stdio.h>

#include <vgui_controls/Button.h>
#include <vgui_controls/QueryBox.h>
#include <vgui_controls/CheckButton.h>
#include "tier1/KeyValues.h"
#include <vgui_controls/Label.h>
#include <vgui/ISystem.h>
#include <vgui/ISurface.h>
#include <vgui/Cursor.h>
#include <vgui_controls/RadioButton.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/FileOpenDialog.h>
#include <vgui_controls/MessageBox.h>
#include <vgui/IVGui.h>
#include <vgui/ILocalize.h>
#include <vgui/IPanel.h>

#include "CvarTextEntry.h"
#include "CvarToggleCheckButton.h"
#include "cvarslider.h"
#include "LabeledCommandComboBox.h"
#include "filesystem.h"
#include "EngineInterface.h"
#include "BitmapImagePanel.h"
#include "tier1/utlbuffer.h"
#include "ModInfo.h"
#include "tier1/convar.h"
#include "tier0/icommandline.h"

#include "materialsystem/imaterial.h"
#include "materialsystem/imesh.h"
#include "materialsystem/imaterialvar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
COptionsSubPlayerSettings::COptionsSubPlayerSettings(vgui::Panel *parent) : vgui::PropertyPage(parent, "OptionsSubPlayerSettings") 
{
	Button *ok = new Button( this, "OK", "#GameUI_OK" );
	ok->SetCommand( "Ok" );

	Button *cancel = new Button( this, "Cancel", "#GameUI_Cancel" );
	cancel->SetCommand( "Close" );

	Button *apply = new Button( this, "Apply", "#GameUI_Apply" );
	apply->SetCommand( "Apply" );

	
	// HACK: Temp file cuz we don't have these lines in the main script
	g_pVGuiLocalize->AddFile( "resource/playersettings_%language%.txt", "GAME", true );
	

	// player name
	m_pNameEntry = new CCvarTextEntry( this, "NameEntry", "name" );
	m_pNameEntry->AddActionSignalTarget( this );


	// avatar stuff
	Button *importAvatarImage = new Button( this, "ImportAvatarImage", "#GameUI_ImportAvatarEllipsis" );
	importAvatarImage->SetCommand( "ImportAvatarImage" );

	m_hImportAvatarDialog = NULL;

	m_pAvatarImage = new ImagePanel( this, "AvatarImage" );
	m_pAvatarImage->AddActionSignalTarget( this );

	m_pAvatarList = new CLabeledCommandComboBox( this, "AvatarList" );
	InitAvatarList( m_pAvatarList );

	m_AvatarName[0] = 0;


	LoadControlSettings( "Resource/OptionsSubPlayerSettings.res" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
COptionsSubPlayerSettings::~COptionsSubPlayerSettings()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPlayerSettings::OnCommand( const char *command )
{
	if ( !Q_stricmp( command, "ImportAvatarImage" ) )
	{
		if ( !m_hImportAvatarDialog )
		{
			m_hImportAvatarDialog = new FileOpenDialog( NULL, "#GameUI_ImportAvatarImage", true );
			m_hImportAvatarDialog->AddFilter( IsPosix() ? "*.tga,*.jpg,*.vtf" : "*.tga,*.jpg,*.bmp,*.vtf", "#GameUI_All_Images", true );
			m_hImportAvatarDialog->AddFilter( "*.tga", "#GameUI_TGA_Images", false );
			m_hImportAvatarDialog->AddFilter( "*.jpg", "#GameUI_JPEG_Images", false );
#ifdef WIN32
			m_hImportAvatarDialog->AddFilter( "*.bmp", "#GameUI_BMP_Images", false );
#endif
			m_hImportAvatarDialog->AddFilter( "*.vtf", "#GameUI_VTF_Images", false );
			m_hImportAvatarDialog->AddActionSignalTarget( this );
		}

		m_hImportAvatarDialog->DoModal( false );
		m_hImportAvatarDialog->Activate();
	}

	BaseClass::OnCommand( command );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPlayerSettings::ConversionError( ConversionErrorType nError )
{
	const char *pErrorText = NULL;

	switch ( nError )
	{
	case CE_MEMORY_ERROR:
		pErrorText = "#GameUI_Avatar_Import_Error_Memory";
		break;

	case CE_CANT_OPEN_SOURCE_FILE:
		pErrorText = "#GameUI_Avatar_Import_Error_Reading_Image";
		break;

	case CE_ERROR_PARSING_SOURCE:
		pErrorText = "#GameUI_Avatar_Import_Error_Image_File_Corrupt";
		break;

	case CE_SOURCE_FILE_SIZE_NOT_SUPPORTED:
		pErrorText = "#GameUI_Avatar_Import_Image_Wrong_Size";
		break;

	case CE_SOURCE_FILE_FORMAT_NOT_SUPPORTED:
		pErrorText = "#GameUI_Avatar_Import_Image_Wrong_Size";
		break;

	case CE_SOURCE_FILE_TGA_FORMAT_NOT_SUPPORTED:
		pErrorText = "#GameUI_Avatar_Import_Error_TGA_Format_Not_Supported";
		break;

	case CE_SOURCE_FILE_BMP_FORMAT_NOT_SUPPORTED:
		pErrorText = "#GameUI_Avatar_Import_Error_BMP_Format_Not_Supported";
		break;

	case CE_ERROR_WRITING_OUTPUT_FILE:
		pErrorText = "#GameUI_Avatar_Import_Error_Writing_Temp_Output";
		break;

	case CE_ERROR_LOADING_DLL:
		pErrorText = "#GameUI_Avatar_Import_Error_Cant_Load_VTEX_DLL";
		break;
	}

	if ( pErrorText )
	{
		// Create the dialog
		vgui::MessageBox *pErrorDlg = new vgui::MessageBox( "#GameUI_Avatar_Import_Error_Title", pErrorText );
		Assert( pErrorDlg );

		// Display
		if ( pErrorDlg )	// Check for a NULL just to be extra cautious...
		{
			pErrorDlg->DoModal();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPlayerSettings::OnFileSelected( const char *fullpath )
{
	// this can take a while, put up a waiting cursor
	surface()->SetCursor( dc_hourglass );

	ConversionErrorType nErrorCode = ImgUtl_ConvertToVTFAndDumpVMT( fullpath, IsPosix() ? "/vgui/avatars" : "\\vgui\\avatars", 256, 256 );
	if ( nErrorCode == CE_SUCCESS )
	{
		// refresh the avatar list so the new avatar shows up.
		InitAvatarList( m_pAvatarList );

		// Get the filename
		char szRootFilename[MAX_PATH];
		V_FileBase( fullpath, szRootFilename, sizeof( szRootFilename ) );

		// automatically select the avatar that was just imported.
		SelectAvatar( szRootFilename );
	}
	else
	{
		ConversionError( nErrorCode );
	}

	// change the cursor back to normal
	surface()->SetCursor( dc_user );
}

//-----------------------------------------------------------------------------
// Purpose: Builds the list of avatars
//-----------------------------------------------------------------------------
void COptionsSubPlayerSettings::InitAvatarList( CLabeledCommandComboBox *cb )
{
	// Find out images
	FileFindHandle_t fh;
	char directory[512];

	ConVarRef cl_avatarfile( "cl_avatarfile", true );
	if ( !cl_avatarfile.IsValid() )
		return;

	cb->DeleteAllItems();

	const char *avatarfile = cl_avatarfile.GetString();
	Q_snprintf( directory, sizeof( directory ), "materials/vgui/avatars/*.vtf" );
	const char *fn = g_pFullFileSystem->FindFirst( directory, &fh );
	int i = 0, initialItem = 0;
	while ( fn )
	{
		char filename[512];
		Q_snprintf( filename, sizeof( filename ), "materials/vgui/avatars/%s", fn );
		if ( strlen( filename ) >= 4 )
		{
			filename[strlen( filename ) - 4] = 0;
			Q_strncat( filename, ".vmt", sizeof( filename ), COPY_ALL_CHARACTERS );
			if ( g_pFullFileSystem->FileExists( filename ) )
			{
				// strip off the extension
				Q_strncpy( filename, fn, sizeof( filename ) );
				filename[strlen( filename ) - 4] = 0;
				cb->AddItem( filename, "" );

				// check to see if this is the one we have set
				Q_snprintf( filename, sizeof( filename ), "materials/vgui/avatars/%s", fn );
				if ( !Q_stricmp( filename, avatarfile ) )
				{
					initialItem = i;
				}

				++i;
			}
		}

		fn = g_pFullFileSystem->FindNext( fh );
	}

	g_pFullFileSystem->FindClose( fh );
	cb->SetInitialItem( initialItem );
}

//-----------------------------------------------------------------------------
// Purpose: Selects the given avatar in the avatar list.
//-----------------------------------------------------------------------------
void COptionsSubPlayerSettings::SelectAvatar( const char *avatarName )
{
	int numEntries = m_pAvatarList->GetItemCount();
	int index;
	wchar_t itemText[MAX_PATH];
	wchar_t itemToSelectText[MAX_PATH];

	// convert the avatar filename to unicode
	g_pVGuiLocalize->ConvertANSIToUnicode( avatarName, itemToSelectText, sizeof( itemToSelectText ) );

	// find the index of the avatar we want.
	for ( index = 0; index < numEntries; ++index )
	{
		m_pAvatarList->GetItemText( index, itemText, sizeof( itemText ) );
		if ( !wcscmp( itemText, itemToSelectText ) )
		{
			break;
		}
	}

	if ( index < numEntries )
	{
		// select the avatar.
		m_pAvatarList->ActivateItem( index );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPlayerSettings::RemapAvatar()
{
	char avatarname[256];
	m_pAvatarList->GetText( avatarname, sizeof( avatarname ) );

	if ( !avatarname[0] )
		return;

	// make sure there is a version with the proper shader
	g_pFullFileSystem->CreateDirHierarchy( "materials/vgui/avatars", "GAME" );

	char fullAvatarName[512];
	Q_snprintf( fullAvatarName, sizeof( fullAvatarName ), "materials/vgui/avatars/%s.vmt", avatarname );

	if ( !g_pFullFileSystem->FileExists( fullAvatarName ) )
	{
		FileHandle_t fp = g_pFullFileSystem->Open( fullAvatarName, "wb" );
		if ( !fp )
			return;

		char data[1024];
		Q_snprintf( data, sizeof( data ), "\"UnlitGeneric\"\n{\n\t\"$translucent\" 1\n\t\"$basetexture\" \"vgui/avatars/%s\"\n\t\"$vertexcolor\" 1\n\t\"$vertexalpha\" 1\n\t\"$no_fullbright\" 1\n\t\"$ignorez\" 1\n}\n", avatarname );

		g_pFullFileSystem->Write( data, strlen( data ), fp );
		g_pFullFileSystem->Close( fp );
	}

	Q_snprintf( fullAvatarName, sizeof( fullAvatarName ), "avatars/%s", avatarname );
	m_pAvatarImage->SetImage( fullAvatarName );
}

//-----------------------------------------------------------------------------
// Purpose: Called whenever model name changes
//-----------------------------------------------------------------------------
void COptionsSubPlayerSettings::OnTextChanged(vgui::Panel *panel)
{
	RemapAvatar();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPlayerSettings::OnControlModified()
{
	PostMessage( GetParent(), new KeyValues( "ApplyButtonEnable" ) );
	InvalidateLayout();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPlayerSettings::OnApplyChanges()
{
	// player name
	m_pNameEntry->ApplyChanges();

	// avatar stuff
	m_pAvatarList->ApplyChanges();
	m_pAvatarList->GetText( m_AvatarName, sizeof( m_AvatarName ) );

	char cmd[2048];
	// save the avatar name
	if ( m_AvatarName[0] )
	{
		Q_snprintf( cmd, sizeof( cmd ), "cl_avatarfile materials/vgui/avatars/%s.vtf\n", m_AvatarName );
	}
	else
	{
		Q_strncpy( cmd, "cl_avatarfile \"\"\n", sizeof( cmd ) );
	}
	engine->ClientCmd_Unrestricted( cmd );

#if defined( GAME_TF2CLASSIC )
	// HACK: Reload our avatar and player name
	engine->ClientCmd_Unrestricted( "tf2c_mainmenu_reload\n" );
#endif
}
