//=============================================================================
// 
// Purpose: NonSteam implementation of user avatars for TF2C
// 
// Author: SanyaSho (2025)
// 
//=============================================================================

#include "cbase.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/Image.h"
#include "materialsystem/itexture.h"
#include "filesystem.h"
#include "vgui/ISurface.h"
#include "vgui_avatarimage_nonsteam.h"

#include "tier0/memdbgon.h"

bool CopyAvatarFromDownloads( CRC32_t nAvatarCRC, char *texname, int nchars )
{
	char logohex[16];
	Q_binarytohex( (byte *)&nAvatarCRC, sizeof( nAvatarCRC ), logohex, sizeof( logohex ) );

	// See if logo has been downloaded.
	Q_snprintf( texname, nchars, "../temp/avatars/%s", logohex );

	char fulltexname[512];
	Q_snprintf( fulltexname, sizeof( fulltexname ), "materials/temp/avatars/%s.vtf", logohex );

	// copy VTF
	if ( !filesystem->FileExists( fulltexname ) )
	{
		//Warning( "vgui_avatarimage_nonsteam: CopyAvatarFromDownloads [VTF] Begin.\n" );
		char custname[512];
		Q_snprintf( custname, sizeof( custname ), "download/user_custom/%c%c/%s.dat", logohex[0], logohex[1], logohex );

		// it may have been downloaded but not copied under materials folder
		if ( !filesystem->FileExists( custname ) )
			return false; // not downloaded yet

		// copy from download folder to materials/temp folder
		// this is done since material system can access only materials/*.vtf files
		if ( !engine->CopyLocalFile( custname, fulltexname ) )
			return false;
		//Warning( "vgui_avatarimage_nonsteam: CopyAvatarFromDownloads [VTF] End.\n" );
	}

	Q_SetExtension( fulltexname, ".vmt", sizeof( fulltexname ) );

	// make VMT
	if ( !filesystem->FileExists( fulltexname ) )
	{
		//Warning( "vgui_avatarimage_nonsteam: CopyAvatarFromDownloads [VMT] Begin.\n" );
		FileHandle_t fp = g_pFullFileSystem->Open( fulltexname, "wb" );
		if ( !fp )
			return false;

		char data[1024];
		Q_snprintf( data, sizeof( data ), "\"UnlitGeneric\"\n{\n\t\"$translucent\" 1\n\t\"$basetexture\" \"temp/avatars/%s\"\n\t\"$vertexcolor\" 1\n\t\"$vertexalpha\" 1\n\t\"$no_fullbright\" 1\n\t\"$ignorez\" 1\n}\n", logohex );

		g_pFullFileSystem->Write( data, strlen( data ), fp );
		g_pFullFileSystem->Close( fp );
		//Warning( "vgui_avatarimage_nonsteam: CopyAvatarFromDownloads [VMT] End.\n" );
	}

	return true;
}

DECLARE_BUILD_FACTORY( CAvatarImagePanel );

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CAvatarImagePanel::CAvatarImagePanel( vgui::Panel *parent, const char *name ) : BaseClass( parent, name )
{
	m_pImage = new CAvatarImage();
	m_bScaleImage = false;
	m_bSizeDirty = true;
	m_bClickable = false;
}

//-----------------------------------------------------------------------------
// Purpose: Set the avatar by C_BasePlayer pointer
//-----------------------------------------------------------------------------
void CAvatarImagePanel::SetPlayer( C_BasePlayer *pPlayer, EAvatarSize avatarSize )
{
	if ( pPlayer )
	{
		int iIndex = pPlayer->entindex();
		SetPlayer( iIndex, avatarSize );
	}
	else
	{
		m_pImage->ClearAvatarSteamID();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Set the avatar by entity number
//-----------------------------------------------------------------------------
void CAvatarImagePanel::SetPlayer( int entindex, EAvatarSize avatarSize )
{
	m_pImage->ClearAvatarSteamID();

	// Used by MainMenu in TF2C
	if ( entindex == IP_LOCALPLAYER_IMAGE )
	{
		ConVarRef cl_avatarfile( "cl_avatarfile" );
		if ( cl_avatarfile.IsValid() )
		{
			char szAvatarFile[MAX_PATH];
			V_FileBase( cl_avatarfile.GetString(), szAvatarFile, sizeof( szAvatarFile ) );

			m_pImage->SetAvatar( vgui::scheme()->GetImage( CFmtStr( "avatars/%s", szAvatarFile ), true ) );
		}
		return;
	}

	player_info_t pi;
	if ( engine->GetPlayerInfo( entindex, &pi ) && !CRC_AVATAR_INVALID( CRC_AVATAR( pi ) ) )
	{
		m_pImage->SetAvatarFromPI( pi, avatarSize );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Set the avatar by SteamID
//-----------------------------------------------------------------------------
void CAvatarImagePanel::SetPlayer( CSteamID steamIDForPlayer, EAvatarSize avatarSize )
{
	m_pImage->ClearAvatarSteamID();

	IP_UNSUPPORTED
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CAvatarImagePanel::PaintBackground( void )
{
	if ( m_bSizeDirty )
		UpdateSize();

	m_pImage->Paint();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CAvatarImagePanel::ClearAvatar()
{
	m_pImage->ClearAvatarSteamID();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CAvatarImagePanel::SetDefaultAvatar( vgui::IImage *pDefaultAvatar )
{
	m_pImage->SetDefaultImage( pDefaultAvatar );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CAvatarImagePanel::SetAvatarSize( int width, int height )
{
	if ( m_bScaleImage )
	{
		// panel is charge of image size - setting avatar size this way not allowed
		Assert( false );
		return;
	}
	else
	{
		m_pImage->SetAvatarSize( width, height );
		m_bSizeDirty = true;
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CAvatarImagePanel::OnSizeChanged( int newWide, int newTall )
{
	BaseClass::OnSizeChanged( newWide, newTall );
	m_bSizeDirty = true;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CAvatarImagePanel::OnMousePressed( vgui::MouseCode code )
{
	if ( !m_bClickable || code != MOUSE_LEFT )
		return;

	PostActionSignal( new KeyValues( "AvatarMousePressed" ) );

	// audible feedback
	vgui::surface()->PlaySound( "ui/buttonclick.wav" );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CAvatarImagePanel::SetShouldScaleImage( bool bScaleImage )
{
	m_bScaleImage = bScaleImage;
	m_bSizeDirty = true;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CAvatarImagePanel::SetShouldDrawFriendIcon( bool bDrawFriend )
{
	m_pImage->SetDrawFriend( bDrawFriend );
	m_bSizeDirty = true;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CAvatarImagePanel::UpdateSize()
{
	if ( m_bScaleImage )
	{
		// the panel is in charge of the image size
		m_pImage->SetAvatarSize( GetWide(), GetTall() );
	}
	else
	{
		// the image is in charge of the panel size
		SetSize( m_pImage->GetAvatarWide(), m_pImage->GetAvatarTall() );
	}

	m_bSizeDirty = false;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CAvatarImagePanel::ApplySettings( KeyValues *inResourceData )
{
	m_bScaleImage = inResourceData->GetInt( "scaleImage", 0 );

	BaseClass::ApplySettings( inResourceData );
}