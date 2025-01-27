//=============================================================================
// 
// Purpose: NonSteam implementation of user avatars for TF2C
// 
// Author: SanyaSho (2025)
// 
//=============================================================================

#if !defined( VGUI_AVATARIMAGE_NONSTEAM_H )
#define VGUI_AVATARIMAGE_NONSTEAM_H

#include <vgui/IImage.h>
#include "vgui_controls/Panel.h"

#include "fmtstr.h"

#include "steam/steam_api.h" // for CSteamID

// size of the standard avatar icon (unless override by SetAvatarSize)
#define DEFAULT_AVATAR_SIZE		(32)

#define CRC_AVATAR( pi ) pi.customFiles[2]
#define CRC_AVATAR_INVALID( crc ) ((crc == 0x00000000) || (crc == 0xFFFFFFFF))

#define IP_LOCALPLAYER_IMAGE 1337

#if defined( DEBUG )
#define IP_UNSUPPORTED Warning( "vgui_avatarimage_nonsteam: %s called!\n", __FUNCTION__ );
#else
#define IP_UNSUPPORTED
#endif

//-----------------------------------------------------------------------------
// Purpose: avatar sizes, formerly used in ISteamFriends, but now only used in game code
//-----------------------------------------------------------------------------
enum EAvatarSize
{
	k_EAvatarSize32x32 = 0,
	k_EAvatarSize64x64 = 1,
	k_EAvatarSize184x184 = 2,
};

bool CopyAvatarFromDownloads( CRC32_t nAvatarCRC, char *texname, int nchars );

class CAvatarImage : public vgui::IImage
{
	DECLARE_CLASS_SIMPLE( CAvatarImage, vgui::IImage );

public:
	CAvatarImage()
	{
		m_nX = m_nY = 0;
		m_wide = m_tall = 0;
		m_avatarWide = m_avatarTall = 0;
		m_Color = Color( 255, 255, 255, 255 );

		m_pImage = m_pDefaultImage = NULL;

		SetAvatarSize( DEFAULT_AVATAR_SIZE, DEFAULT_AVATAR_SIZE );
	}
	virtual ~CAvatarImage() {}

	virtual void Paint()
	{
		if ( m_pImage )
		{
			m_pImage->SetSize( m_avatarWide, m_avatarTall );
			m_pImage->SetPos( m_nX, m_nY );
			m_pImage->SetColor( m_Color );
			m_pImage->Paint();
		}
		else if ( m_pDefaultImage )
		{
			// draw default
			m_pDefaultImage->SetSize( m_avatarWide, m_avatarTall );
			m_pDefaultImage->SetPos( m_nX, m_nY );
			m_pDefaultImage->SetColor( m_Color );
			m_pDefaultImage->Paint();
		}
	}

	virtual void SetPos( int x, int y ) { m_nX = x; m_nY = y; }

	virtual void GetContentSize( int &wide, int &tall ) { wide = m_wide; tall = m_tall; }

	virtual void GetSize( int &wide, int &tall ) { GetContentSize( wide, tall ); }

	virtual void SetSize( int wide, int tall ) { m_wide = wide; m_tall = tall; m_avatarWide = m_wide; m_avatarTall = m_tall; }

	virtual void SetColor( Color col ) { m_Color = col; }

	virtual bool Evict() { return false; }

	virtual int GetNumFrames() { return 0; }

	virtual void SetFrame( int nFrame ) {}

	virtual vgui::HTexture GetID() { return 0; }

	virtual void SetRotation( int iRotation ) {}

	void SetDrawFriend( bool bDraw ) { IP_UNSUPPORTED }
	void SetAvatarSteamID( CSteamID steamID, EAvatarSize nAvatarSize = k_EAvatarSize32x32 ) { IP_UNSUPPORTED }

	void SetAvatarSize( int width, int height )
	{
		m_avatarWide = width;
		m_avatarTall = height;

		m_wide = m_avatarWide;
		m_tall = m_avatarTall;
	}

	void SetDefaultImage( vgui::IImage *pImage ) { m_pDefaultImage = pImage; }

	void UpdateFriendStatus() { IP_UNSUPPORTED }
	void ClearAvatarSteamID() { SetAvatar( NULL ); }

	void SetAvatar( vgui::IImage *pImage ) { m_pImage = pImage; }

	int GetWide() { return m_wide; }
	int	GetTall() { return m_tall; }
	int	GetAvatarWide() { return m_avatarWide; }
	int	GetAvatarTall() { return m_avatarTall; }

	void SetAvatarFromPI( const player_info_t &pi, EAvatarSize nAvatarSize = k_EAvatarSize32x32 )
	{
		CRC32_t avatarCRC = CRC_AVATAR( pi );
		if ( CRC_AVATAR_INVALID( avatarCRC ) )
		{
			Warning( "CAvatarImage: Invalid image CRC32! Grab a programmer!\n" );
			return;
		}

		char avatarname[MAX_PATH];
		if ( !CopyAvatarFromDownloads( avatarCRC, avatarname, sizeof( avatarname ) ) )
		{
			Warning( "CAvatarImage: Failed to copy avatar 0x%x for %s!\n", avatarCRC, pi.name );
			SetAvatar( NULL ); // drop avatar (don't render it anymore, try to switch to default one)
			return;
		}

		SetAvatar( vgui::scheme()->GetImage( avatarname, true ) );

		// SanyaSho: This is not what EAvatarSize mean. Avatar is scaled dynamicly depending on the parent panel size or user values.
		// EAvatarSize is sent to Steam to get sized avatar RGBA for user. We don't need to do this cuz we don't have Steam.
		// Having this code enabled causes issues with Deathmatch Objectives panel in TF2C.
#if 0
		switch ( nAvatarSize )
		{
		case k_EAvatarSize32x32:
			SetAvatarSize( 32, 32 );
			break;
		case k_EAvatarSize64x64:
			SetAvatarSize( 64, 64 );
			break;
		case k_EAvatarSize184x184:
			SetAvatarSize( 184, 184 );
			break;
		};
#endif
	}

private:
	int m_nX, m_nY;
	int m_wide, m_tall;
	int	m_avatarWide, m_avatarTall;
	Color m_Color;

	vgui::IImage *m_pImage;
	vgui::IImage *m_pDefaultImage;
};

//-----------------------------------------------------------------------------
// Purpose: Modified version of OG CAvatarImagePanel
//-----------------------------------------------------------------------------
class CAvatarImagePanel : public vgui::Panel
{
public:
	DECLARE_CLASS_SIMPLE( CAvatarImagePanel, vgui::Panel );

	CAvatarImagePanel( vgui::Panel *parent, const char *name );

	// Set the player that this Avatar should display for
	//=============================================================================
	// HPE_BEGIN:
	// [menglish] Added default variable of scalable to allow the avatar to be drawn at sizes other than 32, 32
	// [tj] added a parameter for drawing the friend icon. Defaulted to true to maintain backward compatibility.
	// [menglish] Added parameter to specify a default avatar
	// [menglish] Added a function to set the avatar size of the AvatarImage
	//=============================================================================

	// reset the image to its default value, clearing any info retrieved from Steam
	void ClearAvatar();

	// Set the avatar by C_BasePlayer pointer (NOTE: avatarSize is ignored!)
	void SetPlayer( C_BasePlayer *pPlayer, EAvatarSize avatarSize = k_EAvatarSize32x32 );

	// [tj] Overloaded function to go straight to entity index (NOTE: avatarSize is ignored!)
	void SetPlayer( int entityIndex, EAvatarSize avatarSize = k_EAvatarSize32x32 );

	// [tj] lower level function that expects a steam ID instead of a player (NOTE: avatarSize is ignored!)
	void SetPlayer( CSteamID steamIDForPlayer, EAvatarSize avatarSize );

	// sets whether or not the image should scale to fit the size of the ImagePanel (defaults to false)
	void SetShouldScaleImage( bool bScaleImage );

	// sets whether to automatically draw the friend icon behind the avatar for Steam friends
	void SetShouldDrawFriendIcon( bool bDrawFriend );

	// specify the size of the avatar portion of the image (the actual image may be larger than this
	// when it incorporates the friend icon)
	void SetAvatarSize( int width, int height );

	// specify a fallback image to use
	void SetDefaultAvatar( vgui::IImage *pDefaultAvatar );

	virtual void OnSizeChanged( int newWide, int newTall );

	//=============================================================================
	// HPE_END
	//=============================================================================

	virtual void OnMousePressed( vgui::MouseCode code );

	virtual void PaintBackground();

	bool IsValid() { return true; }

	void SetClickable( bool bClickable ) { m_bClickable = bClickable; }

protected:
	virtual void ApplySettings( KeyValues *inResourceData );

	void UpdateSize();

private:
	CAvatarImage *m_pImage;
	bool m_bScaleImage;
	bool m_bSizeDirty;
	bool m_bClickable;
};

#endif // VGUI_AVATARIMAGE_NONSTEAM_H