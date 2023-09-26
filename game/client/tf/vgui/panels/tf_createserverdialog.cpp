//=============================================================================//
//
// Purpose: 
//
//=============================================================================//
#include "cbase.h"
#include "tf_createserverdialog.h"
#include <vgui_controls/ComboBox.h>
#include "controls/tf_advpanellistpanel.h"
#include "controls/tf_advcheckbutton.h"
#include "controls/tf_advslider.h"
#include "tier0/vcrmode.h"
#include <filesystem.h>
#include <ctime>

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

#define OPTIONS_DIR "cfg"
#define DEFAULT_OPTIONS_FILE OPTIONS_DIR "/settings_default.scr"
#define OPTIONS_FILE OPTIONS_DIR "/settings.scr"

#define RANDOM_MAP "#GameUI_RandomMap"

//-----------------------------------------------------------------------------
// Purpose: class for loading/saving server config file
//-----------------------------------------------------------------------------
class CServerDescription : public CDescription
{
public:
	CServerDescription( CPanelListPanel *panel );

	void WriteScriptHeader( FileHandle_t fp );
	void WriteFileHeader( FileHandle_t fp );
};

CTFCreateServerDialog::CTFCreateServerDialog( Panel *pParent, const char *pszName ) : CTFDialogPanelBase( pParent, pszName )
{
	m_bBotsEnabled = false;

	m_pListPanel = new CPanelListPanel( this, "PanelListPanel" );
	m_pList = NULL;

	// create KeyValues object to load/save config options
	m_pSavedData = new KeyValues( "ServerConfig" );
	m_pSavedData->LoadFromFile( filesystem, "ServerConfig.vdf", "GAME" ); // this is game-specific data, so it should live in GAME, not CONFIG

	m_pDescription = new CServerDescription( m_pListPanel );
	m_pDescription->InitFromFile( DEFAULT_OPTIONS_FILE );
	m_pDescription->InitFromFile( OPTIONS_FILE );
}

CTFCreateServerDialog::~CTFCreateServerDialog()
{
	if ( m_pSavedData )
	{
		m_pSavedData->deleteThis();
		m_pSavedData = NULL;
	}

	delete m_pDescription;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCreateServerDialog::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/ui/main_menu/CreateServerDialog.res" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCreateServerDialog::Show( void )
{
	BaseClass::Show();

	OnCreateControls();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCreateServerDialog::Hide( void )
{
	OnDestroyControls();

	BaseClass::Hide();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCreateServerDialog::CreateControls( void )
{
	BaseClass::CreateControls();

	m_pMapList = new ComboBox( this, "MapList", 12, false );
	AddControl( m_pMapList, O_LIST, "#GameUI_Map" );

	LoadMapList();

	const char *pszStartMap = m_pSavedData->GetString( "map", "" );

	if ( pszStartMap[0] != '\0' )
	{
		SetMap( pszStartMap );
	}

	LoadGameOptionsList();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCreateServerDialog::DestroyControls( void )
{
	BaseClass::DestroyControls();
	m_pList = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCreateServerDialog::OnApplyChanges( void )
{
	// reset server enforced cvars
	g_pCVar->RevertFlaggedConVars( FCVAR_REPLICATED );

	// Cheats were disabled; revert all cheat cvars to their default values.
	// This must be done heading into multiplayer games because people can play
	// demos etc and set cheat cvars with sv_cheats 0.
	g_pCVar->RevertFlaggedConVars( FCVAR_CHEAT );

	DevMsg( "FCVAR_CHEAT cvars reverted to defaults.\n" );

	// Get the values from the controls
	GatherCurrentValues();

	// Create the game.cfg file
	if ( m_pDescription )
	{
		FileHandle_t fp;

		// Add settings to config.cfg
		m_pDescription->WriteToConfig();

		// save out in the settings file
		filesystem->CreateDirHierarchy( OPTIONS_DIR, "GAME" );
		fp = filesystem->Open( OPTIONS_FILE, "wb", "GAME" );
		if ( fp )
		{
			m_pDescription->WriteToScriptFile( fp );
			filesystem->Close( fp );
		}
	}

	char szMapName[64], szHostName[64], szPassword[64];
	V_strcpy_safe( szMapName, GetMapName() );
	V_strcpy_safe( szHostName, GetHostName() );
	V_strcpy_safe( szPassword, GetPassword() );

	// save the config data
	if ( m_pSavedData )
	{
		if ( IsRandomMapSelected() )
		{
			// it's set to random map, just save an
			m_pSavedData->SetString( "map", "" );
		}
		else
		{
			m_pSavedData->SetString( "map", szMapName );
		}

		// save config to a file
		m_pSavedData->SaveToFile( filesystem, "ServerConfig.vdf", "GAME" );
	}

	char szMapCommand[1024];

	// create the command to execute
	V_sprintf_safe( szMapCommand, "disconnect\nwait\nwait\nsetmaster enable\nmaxplayers %i\nsv_password \"%s\"\nhostname \"%s\"\nprogress_enable\nmap %s\n",
		GetMaxPlayers(),
		szPassword,
		szHostName,
		szMapName
		);

	// exec
	engine->ClientCmd_Unrestricted( szMapCommand );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTFCreateServerDialog::IsRandomMapSelected()
{
	const char *mapname = m_pMapList->GetActiveItemUserData()->GetString( "mapname" );

	if ( !V_strcmp( mapname, RANDOM_MAP ) )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CTFCreateServerDialog::GetMapName()
{
	int count = m_pMapList->GetItemCount();

	// if there is only one entry it's the special "select random map" entry
	if ( count <= 1 )
		return NULL;

	const char *mapname = m_pMapList->GetActiveItemUserData()->GetString( "mapname" );
	if ( !V_strcmp( mapname, RANDOM_MAP ) )
	{
		int which = RandomInt( 1, count - 1 );
		mapname = m_pMapList->GetItemUserData( which )->GetString( "mapname" );
	}

	return mapname;
}

//-----------------------------------------------------------------------------
// Purpose: Sets currently selected map in the map combobox
//-----------------------------------------------------------------------------
void CTFCreateServerDialog::SetMap( const char *mapName )
{
	for ( int i = 0; i < m_pMapList->GetItemCount(); i++ )
	{
		if ( !m_pMapList->IsItemIDValid( i ) )
			continue;

		if ( !V_stricmp( m_pMapList->GetItemUserData( i )->GetString( "mapname" ), mapName ) )
		{
			m_pMapList->ActivateItem( i );
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCreateServerDialog::LoadMaps( const char *pszPathID )
{
	FileFindHandle_t findHandle = NULL;

	const char *pszFilename = filesystem->FindFirst( "maps/*.bsp", &findHandle );
	do
	{
		char mapname[256];

		// FindFirst ignores the pszPathID, so check it here
		V_sprintf_safe( mapname, "maps/%s", pszFilename );
		if ( !filesystem->FileExists( mapname, pszPathID ) )
			continue;

		// remove the text 'maps/' and '.bsp' from the file name to get the map name
		const char *str = Q_strstr( pszFilename, "maps" );
		if ( str )
		{
			V_strcpy_safe( mapname, str + 5 );	// maps + \\ = 5
		}
		else
		{
			V_strcpy_safe( mapname, pszFilename );
		}
		char *ext = V_strstr( mapname, ".bsp" );
		if ( ext )
		{
			*ext = 0;
		}

		// add to the map list
		m_pMapList->AddItem( mapname, new KeyValues( "data", "mapname", mapname ) );

	} while ( ( pszFilename = filesystem->FindNext( findHandle ) ) != NULL );

	filesystem->FindClose( findHandle );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCreateServerDialog::LoadMapList( void )
{
	// clear the current list (if any)
	m_pMapList->DeleteAllItems();

	// add special "name" to represent loading a randomly selected map
	m_pMapList->AddItem( RANDOM_MAP, new KeyValues( "data", "mapname", RANDOM_MAP ) );

	// Load the GameDir maps
	LoadMaps( "MOD" );

	// set the first item to be selected
	m_pMapList->ActivateItem( 0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CTFCreateServerDialog::GetMaxPlayers( void )
{
	return atoi( GetValue( "maxplayers", "32" ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CTFCreateServerDialog::GetPassword( void )
{
	return GetValue( "sv_password", "" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CTFCreateServerDialog::GetHostName( void )
{
	return GetValue( "hostname", "Half-Life" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CTFCreateServerDialog::GetValue( const char *cvarName, const char *defaultValue )
{
	for ( mpcontrol_t *mp = m_pList; mp != NULL; mp = mp->next )
	{
		Panel *control = mp->pControl;
		if ( control && !stricmp( mp->GetName(), cvarName ) )
		{
			KeyValues *data = new KeyValues( "GetText" );
			static char buf[128];
			if ( control && control->RequestInfo( data ) )
			{
				V_strcpy_safe( buf, data->GetString( "text", defaultValue ) );
			}
			else
			{
				// no value found, copy in default text
				V_strcpy_safe( buf, defaultValue );
			}

			// free
			data->deleteThis();
			return buf;
		}

	}

	return defaultValue;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCreateServerDialog::LoadGameOptionsList( void )
{
	// Go through desciption creating controls
	CScriptObject *pObj;
	pObj = m_pDescription->pObjList;

	mpcontrol_t	*pCtrl;
	CTFCheckButton *pBox;
	TextEntry *pEdit;
	ComboBox *pCombo;
	CScriptListItem *pListItem;

	HFont hFont = GETSCHEME()->GetFont( "TF2CMenuNormal", true );

	Panel *objParent = m_pListPanel;
	while ( pObj )
	{
		//Msg("\nAdded: %s %s %f %f %i\n", pObj->prompt, pObj->cvarname, pObj->fcurValue, pObj->fcurValue, pObj->type);

		if ( pObj->type == O_OBSOLETE )
		{
			pObj = pObj->pNext;
			continue;
		}

		pCtrl = new mpcontrol_t( objParent, pObj->cvarname );
		pCtrl->type = pObj->type;

		switch ( pCtrl->type )
		{
		case O_BOOL:
			pBox = new CTFCheckButton( pCtrl, "DescCheckButton", pObj->prompt );
			pBox->MakeReadyForUse();

			pBox->SetFont( hFont );
			pBox->SetToolTip( pObj->tooltip );
			pBox->SetChecked( pObj->fdefValue != 0.0f ? true : false );

			pCtrl->pControl = pBox;
			break;
		case O_STRING:
		case O_NUMBER:
			pEdit = new TextEntry( pCtrl, "DescTextEntry" );
			pEdit->MakeReadyForUse();

			pEdit->SetFont( hFont );
			pEdit->InsertString( pObj->curValue );

			pCtrl->pControl = pEdit;
			break;
		case O_LIST:
			pCombo = new ComboBox( pCtrl, "DescComboBox", 5, false );
			pCombo->MakeReadyForUse();

			pCombo->SetFont( hFont );

			pListItem = pObj->pListItems;
			while ( pListItem )
			{
				int iItemID = pCombo->AddItem( pListItem->szItemText, NULL );

				if ( V_strcmp( pObj->curValue, pListItem->szValue ) == 0 )
				{
					pCombo->ActivateItem( iItemID );
				}

				pListItem = pListItem->pNext;
			}

			pCtrl->pControl = pCombo;
			break;
		default:
			break;
		}

		if ( pCtrl->type != O_BOOL )
		{
			pCtrl->pPrompt = new Label( pCtrl, "DescLabel", "" );
			pCtrl->pPrompt->MakeReadyForUse();

			pCtrl->pPrompt->SetFont( hFont );
			pCtrl->pPrompt->SetContentAlignment( vgui::Label::a_west );
			pCtrl->pPrompt->SetTextInset( 5, 0 );
			pCtrl->pPrompt->SetText( pObj->prompt );
		}

		pCtrl->pScrObj = pObj;
		int h = m_pListPanel->GetTall() / 13.0; //(float)GetParent()->GetTall() / 15.0;
		pCtrl->SetSize( 800, h );
		//pCtrl->SetBorder( scheme()->GetBorder(1, "DepressedButtonBorder") );
		m_pListPanel->AddItem( pCtrl );

		// Link it in
		if ( !m_pList )
		{
			m_pList = pCtrl;
			pCtrl->next = NULL;
		}
		else
		{
			mpcontrol_t *p;
			p = m_pList;
			while ( p )
			{
				if ( !p->next )
				{
					p->next = pCtrl;
					pCtrl->next = NULL;
					break;
				}
				p = p->next;
			}
		}

		pObj = pObj->pNext;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCreateServerDialog::GatherCurrentValues( void )
{
	if ( !m_pDescription )
		return;

	// OK
	CTFCheckButton *pBox;
	TextEntry *pEdit;
	ComboBox *pCombo;

	mpcontrol_t *pList;

	CScriptObject *pObj;
	CScriptListItem *pItem;

	char szValue[256];

	pList = m_pList;
	while ( pList )
	{
		pObj = pList->pScrObj;

		if ( !pList->pControl )
		{
			pObj->SetCurValue( pObj->curValue );
			pList = pList->next;
			continue;
		}

		switch ( pObj->type )
		{
		case O_BOOL:
			pBox = (CTFCheckButton *)pList->pControl;
			V_sprintf_safe( szValue, "%d", pBox->IsChecked() ? 1 : 0 );
			break;
		case O_NUMBER:
			pEdit = (TextEntry *)pList->pControl;
			pEdit->GetText( szValue, sizeof( szValue ) );
			break;
		case O_STRING:
			pEdit = (TextEntry *)pList->pControl;
			pEdit->GetText( szValue, sizeof( szValue ) );
			break;
		case O_LIST:
			pCombo = (ComboBox *)pList->pControl;
			pCombo->GetText( szValue, sizeof( szValue ) );
			int activeItem = pCombo->GetActiveItem();

			pItem = pObj->pListItems;
			//			int n = (int)pObj->fcurValue;

			while ( pItem )
			{
				if ( !activeItem-- )
					break;

				pItem = pItem->pNext;
			}

			if ( pItem )
			{
				V_sprintf_safe( szValue, "%s", pItem->szValue );
			}
			else  // Couln't find index
			{
				//assert(!("Couldn't find string in list, using default value"));
				V_sprintf_safe( szValue, "%s", pObj->curValue );
			}
			break;
		}

		// Remove double quotes and % characters
		UTIL_StripInvalidCharacters( szValue, sizeof( szValue ) );

		pObj->SetCurValue( szValue );

		pList = pList->next;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Constructor, load/save server settings object
//-----------------------------------------------------------------------------
CServerDescription::CServerDescription( CPanelListPanel *panel ) : CDescription( panel )
{
	setHint( "// NOTE:  THIS FILE IS AUTOMATICALLY REGENERATED, \r\n"
		"//DO NOT EDIT THIS HEADER, YOUR COMMENTS WILL BE LOST IF YOU DO\r\n"
		"// Multiplayer options script\r\n"
		"//\r\n"
		"// Format:\r\n"
		"//  Version [float]\r\n"
		"//  Options description followed by \r\n"
		"//  Options defaults\r\n"
		"//\r\n"
		"// Option description syntax:\r\n"
		"//\r\n"
		"//  \"cvar\" { \"Prompt\" { type [ type info ] } { default } }\r\n"
		"//\r\n"
		"//  type = \r\n"
		"//   BOOL   (a yes/no toggle)\r\n"
		"//   STRING\r\n"
		"//   NUMBER\r\n"
		"//   LIST\r\n"
		"//\r\n"
		"// type info:\r\n"
		"// BOOL                 no type info\r\n"
		"// NUMBER       min max range, use -1 -1 for no limits\r\n"
		"// STRING       no type info\r\n"
		"// LIST         "" delimited list of options value pairs\r\n"
		"//\r\n"
		"//\r\n"
		"// default depends on type\r\n"
		"// BOOL is \"0\" or \"1\"\r\n"
		"// NUMBER is \"value\"\r\n"
		"// STRING is \"value\"\r\n"
		"// LIST is \"index\", where index \"0\" is the first element of the list\r\n\r\n\r\n" );

	setDescription( "SERVER_OPTIONS" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CServerDescription::WriteScriptHeader( FileHandle_t fp )
{
	char am_pm[] = "AM";
	tm newtime;
	VCRHook_LocalTime( &newtime );

	if ( newtime.tm_hour > 12 )        /* Set up extension. */
		V_strcpy_safe( am_pm, "PM" );
	if ( newtime.tm_hour > 12 )        /* Convert from 24-hour */
		newtime.tm_hour -= 12;    /*   to 12-hour clock.  */
	if ( newtime.tm_hour == 0 )        /*Set hour to 12 if midnight. */
		newtime.tm_hour = 12;

	filesystem->FPrintf( fp, (char *)getHint() );

	// Write out the comment and Cvar Info:
	filesystem->FPrintf( fp, "// Half-Life Server Configuration Layout Script (stores last settings chosen, too)\r\n" );
	filesystem->FPrintf( fp, "// File generated:  %.19s %s\r\n", asctime( &newtime ), am_pm );
	filesystem->FPrintf( fp, "//\r\n//\r\n// Cvar\t-\tSetting\r\n\r\n" );

	filesystem->FPrintf( fp, "VERSION %.1f\r\n\r\n", SCRIPT_VERSION );

	filesystem->FPrintf( fp, "DESCRIPTION SERVER_OPTIONS\r\n{\r\n" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CServerDescription::WriteFileHeader( FileHandle_t fp )
{
	char am_pm[] = "AM";
	tm newtime;
	VCRHook_LocalTime( &newtime );

	if ( newtime.tm_hour > 12 )        /* Set up extension. */
		V_strcpy_safe( am_pm, "PM" );
	if ( newtime.tm_hour > 12 )        /* Convert from 24-hour */
		newtime.tm_hour -= 12;    /*   to 12-hour clock.  */
	if ( newtime.tm_hour == 0 )        /*Set hour to 12 if midnight. */
		newtime.tm_hour = 12;

	filesystem->FPrintf( fp, "// Half-Life Server Configuration Settings\r\n" );
	filesystem->FPrintf( fp, "// DO NOT EDIT, GENERATED BY HALF-LIFE\r\n" );
	filesystem->FPrintf( fp, "// File generated:  %.19s %s\r\n", asctime( &newtime ), am_pm );
	filesystem->FPrintf( fp, "//\r\n//\r\n// Cvar\t-\tSetting\r\n\r\n" );
}

