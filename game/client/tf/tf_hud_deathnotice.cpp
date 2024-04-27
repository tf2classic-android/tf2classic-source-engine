//=============================================================================//
//
// Purpose: TF2 Killfeed HUD
//
//=============================================================================//
#include "cbase.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "c_playerresource.h"
#include "iclientmode.h"
#include <vgui_controls/Controls.h>
#include <vgui_controls/Panel.h>
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include <KeyValues.h>
#include "c_baseplayer.h"
#include "c_team.h"

#include "tf_shareddefs.h"
#include "clientmode_tf.h"
#include "c_tf_player.h"
#include "c_tf_playerresource.h"
#include "tf_hud_freezepanel.h"
#include "engine/IEngineSound.h"
#include "tf_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

const char *szLocalizedObjectNames[OBJ_LAST] =
{
        "#TF_Object_Dispenser",
        "#TF_Object_Tele",
        "#TF_Object_Sentry",
        "#TF_object_Sapper"
};

// Player entries in a death notice
struct DeathNoticePlayer
{
	DeathNoticePlayer()
	{
		szName[0] = 0;
		iTeam = TEAM_UNASSIGNED;
		playerColor = COLOR_WHITE;
	}

	char		szName[MAX_PLAYER_NAME_LENGTH * 2];	// big enough for player name and additional information
	int			iTeam;								// team #	
	Color		playerColor;						// Custom color of this player (FFA)
};

// Contents of each entry in our list of death notices
struct DeathNoticeItem
{
	DeathNoticeItem()
	{
		szIcon[0] = 0;
		wzInfoText[0] = 0;
		iconDeath = NULL;
		iconCritDeath = NULL;
		iconHeadshot = NULL;
		bSelfInflicted = false;
		flCreationTime = 0;
		bLocalPlayerInvolved = false;
		bCrit = false;
	}

	float GetExpiryTime();

	DeathNoticePlayer	Killer;
	DeathNoticePlayer   Victim;
	DeathNoticePlayer   Assister;
	char		szIcon[32];		// name of icon to display
	wchar_t		wzInfoText[32];	// any additional text to display next to icon
	CHudTexture *iconDeath;
	CHudTexture *iconCritDeath;	// crit background icon
	CHudTexture *iconHeadshot;

	bool		bSelfInflicted;
	float		flCreationTime;
	bool		bLocalPlayerInvolved;
	bool		bCrit;
};

#define NUM_CORNER_COORD 10
#define NUM_BACKGROUND_COORD NUM_CORNER_COORD*4

class CTFHudDeathNotice : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CTFHudDeathNotice, vgui::Panel );
public:
	CTFHudDeathNotice( const char *pElementName );

	void VidInit( void );
	virtual void Init( void );
	virtual void ApplySchemeSettings( vgui::IScheme *scheme );
	virtual bool IsVisible( void );
	virtual bool ShouldDraw( void );
	virtual void Paint( void );

	void RetireExpiredDeathNotices( void );

	virtual void FireGameEvent( IGameEvent *event );

	void PlayRivalrySounds( int iKillerIndex, int iVictimIndex, int iType );
	virtual Color GetInfoTextColor( int iDeathNoticeMsg, bool bLocalPlayerInvolved ) { return bLocalPlayerInvolved ? COLOR_BLACK : COLOR_WHITE; }

protected:
	virtual Color GetPlayerColor( const DeathNoticePlayer &playerItem, bool bLocalPlayerInvolved = false );
	void DrawText( int x, int y, vgui::HFont hFont, Color clr, const wchar_t *szText );
	int AddDeathNoticeItem();
	void GetBackgroundPolygonVerts( int x0, int y0, int x1, int y1, int iVerts, vgui::Vertex_t vert[] );
	void CalcRoundedCorners();
	CHudTexture *GetIcon( const char *szIcon, bool bInvert );

	void GetLocalizedControlPointName( IGameEvent *event, char *namebuf, int namelen );

private:
	void AddAdditionalMsg( int iKillerID, int iVictimID, const char *pMsgKey );

	CHudTexture		*m_iconDomination;

	CPanelAnimationVar( Color, m_clrBlueText, "TeamBlue", "153 204 255 255" );
	CPanelAnimationVar( Color, m_clrRedText, "TeamRed", "255 64 64 255" );
	CPanelAnimationVar( Color, m_clrGreenText, "TeamGreen", "8 174 0 255" );
	CPanelAnimationVar( Color, m_clrYellowText, "TeamYellow", "255 160 0 255" );

	CPanelAnimationVarAliasType( float, m_flLineHeight, "LineHeight", "16", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flLineSpacing, "LineSpacing", "4", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flCornerRadius, "CornerRadius", "3", "proportional_float" );
	CPanelAnimationVar( float, m_flMaxDeathNotices, "MaxDeathNotices", "4" );
	CPanelAnimationVar( bool, m_bRightJustify, "RightJustify", "1" );
	CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "Default" );
	CPanelAnimationVar( Color, m_clrIcon, "IconColor", "255 80 0 255" );
	CPanelAnimationVar( Color, m_clrBaseBGColor, "BaseBackgroundColor", "46 43 42 220" );
	CPanelAnimationVar( Color, m_clrLocalBGColor, "LocalBackgroundColor", "245 229 196 200" );

	CUtlVector<DeathNoticeItem> m_DeathNotices;

	Vector2D	m_CornerCoord[NUM_CORNER_COORD];
};

using namespace vgui;

static ConVar hud_deathnotice_time( "hud_deathnotice_time", "6" );

DECLARE_HUDELEMENT( CTFHudDeathNotice );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFHudDeathNotice::CTFHudDeathNotice( const char *pElementName ) :
CHudElement( pElementName ), BaseClass( NULL, "HudDeathNotice" )
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	// This is needed for custom colors.
	SetScheme( scheme()->LoadSchemeFromFile( "Resource/ClientScheme_tf2c.res", "ClientScheme_tf2c" ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudDeathNotice::Init( void )
{
	ListenForGameEvent( "player_death" );
	ListenForGameEvent( "object_destroyed" );
	ListenForGameEvent( "teamplay_flag_event" );
	//ListenForGameEvent( "rd_robot_killed" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudDeathNotice::VidInit( void )
{
	m_DeathNotices.RemoveAll();
}

void CTFHudDeathNotice::ApplySchemeSettings( vgui::IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );
	SetPaintBackgroundEnabled( false );

	CalcRoundedCorners();

	m_iconDomination = gHUD.GetIcon( "leaderboard_dominated" );
}

bool CTFHudDeathNotice::IsVisible( void )
{
	if ( IsTakingAFreezecamScreenshot() )
		return false;

	return BaseClass::IsVisible();
}

//-----------------------------------------------------------------------------
// Purpose: Draw if we've got at least one death notice in the queue
//-----------------------------------------------------------------------------
bool CTFHudDeathNotice::ShouldDraw( void )
{
	return ( CHudElement::ShouldDraw() && ( m_DeathNotices.Count() ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFHudDeathNotice::Paint()
{
	// Retire any death notices that have expired
	RetireExpiredDeathNotices();

	CBaseViewport *pViewport = dynamic_cast<CBaseViewport *>( GetClientModeNormal()->GetViewport() );
	int yStart = pViewport->GetDeathMessageStartHeight();

	surface()->DrawSetTextFont( m_hTextFont );

	int xMargin = YRES( 10 );
	int xSpacing = UTIL_ComputeStringWidth( m_hTextFont, L" " );

	int iCount = m_DeathNotices.Count();
	for ( int i = 0; i < iCount; i++ )
	{
		const DeathNoticeItem &msg = m_DeathNotices[i];

		CHudTexture *icon = msg.iconDeath;

		wchar_t victim[256] = L"";
		wchar_t killer[256] = L"";
		wchar_t assister[256] = L"";

		// TEMP - print the death icon name if we don't have a material for it

		g_pVGuiLocalize->ConvertANSIToUnicode( msg.Victim.szName, victim, sizeof( victim ) );
		g_pVGuiLocalize->ConvertANSIToUnicode( msg.Killer.szName, killer, sizeof( killer ) );
		g_pVGuiLocalize->ConvertANSIToUnicode( msg.Assister.szName, assister, sizeof( assister ) );

		int iVictimTextWide = UTIL_ComputeStringWidth( m_hTextFont, victim ) + xSpacing;
		int iDeathInfoTextWide = msg.wzInfoText[0] ? UTIL_ComputeStringWidth( m_hTextFont, msg.wzInfoText ) + xSpacing : 0;
		int iKillerTextWide = killer[0] ? UTIL_ComputeStringWidth( m_hTextFont, killer ) + xSpacing : 0;
		int iPlusIconWide = assister[0] ? UTIL_ComputeStringWidth( m_hTextFont, "+" ) + xSpacing : 0;
		int iAssisterTextWide = assister[0] ? UTIL_ComputeStringWidth( m_hTextFont, assister ) + xSpacing : 0;
		int iLineTall = m_flLineHeight;
		int iTextTall = surface()->GetFontTall( m_hTextFont );
		int iconWide = 0, iconTall = 0, iDeathInfoOffset = 0, iVictimTextOffset = 0, iconActualWide = 0;
		int headshotWide = 0, headshotActualWide = 0;

		// Get the local position for this notice
		if ( icon )
		{
			iconActualWide = icon->Width();
			iconWide = iconActualWide + xSpacing;
			iconTall = icon->Height();

			int iconTallDesired = iLineTall - YRES( 2 );
			Assert( 0 != iconTallDesired );
			float flScale = (float)iconTallDesired / (float)iconTall;

			iconActualWide *= flScale;
			iconTall *= flScale;
			iconWide *= flScale;

			if ( msg.iconHeadshot )
			{
				headshotActualWide = msg.iconHeadshot->Width();
				headshotWide = headshotActualWide + xSpacing;

				flScale = (float)iconTallDesired / (float)msg.iconHeadshot->Height();
				headshotActualWide *= flScale;
				headshotWide *= flScale;
			}
		}

		int iTotalWide = iKillerTextWide + iPlusIconWide + iAssisterTextWide + iconWide + headshotWide + iVictimTextWide + iDeathInfoTextWide + ( xMargin * 2 );
		int y = yStart + ( ( iLineTall + m_flLineSpacing ) * i );
		int yText = y + ( ( iLineTall - iTextTall ) / 2 );
		int yIcon = y + ( ( iLineTall - iconTall ) / 2 );

		int x = 0;
		if ( m_bRightJustify )
		{
			x = GetWide() - iTotalWide;
		}

		// draw a background panel for the message
		Vertex_t vert[NUM_BACKGROUND_COORD];
		GetBackgroundPolygonVerts( x, y + 1, x + iTotalWide, y + iLineTall - 1, ARRAYSIZE( vert ), vert );
		surface()->DrawSetTexture( -1 );
		surface()->DrawSetColor( msg.bLocalPlayerInvolved ? m_clrLocalBGColor : m_clrBaseBGColor );
		surface()->DrawTexturedPolygon( ARRAYSIZE( vert ), vert );

		x += xMargin;

		if ( killer[0] )
		{
			// Draw killer's name
			DrawText( x, yText, m_hTextFont, GetPlayerColor( msg.Killer, msg.bLocalPlayerInvolved ), killer );
			x += iKillerTextWide;
		}

		if ( assister[0] )
		{
			// Draw a + between the names
			DrawText( x, yText, m_hTextFont, GetInfoTextColor( i, msg.bLocalPlayerInvolved ), L"+" );
			x += iPlusIconWide;

			// Draw assister's name
			DrawText( x, yText, m_hTextFont, GetPlayerColor( msg.Assister, msg.bLocalPlayerInvolved ), assister );
			x += iAssisterTextWide;
		}

		// Draw glow behind weapon icon to show it was a crit death
		if ( msg.bCrit && msg.iconCritDeath )
		{
			msg.iconCritDeath->DrawSelf( x, yIcon, iconActualWide, iconTall, m_clrIcon );
		}

		// Draw death icon
		if ( icon )
		{
			icon->DrawSelf( x, yIcon, iconActualWide, iconTall, m_clrIcon );
			x += iconWide;

			if ( msg.iconHeadshot )
			{
				msg.iconHeadshot->DrawSelf( x, yIcon, headshotActualWide, iconTall, m_clrIcon );
				x += headshotWide;
			}
		}

		// Draw additional info text next to death icon 
		if ( msg.wzInfoText[0] )
		{
			if ( msg.bSelfInflicted )
			{
				iDeathInfoOffset += iVictimTextWide;
				iVictimTextOffset -= iDeathInfoTextWide;
			}

			DrawText( x + iDeathInfoOffset, yText, m_hTextFont, GetInfoTextColor( i, msg.bLocalPlayerInvolved ), msg.wzInfoText );
			x += iDeathInfoTextWide;
		}

		// Draw victims name
		DrawText( x + iVictimTextOffset, yText, m_hTextFont, GetPlayerColor( msg.Victim, msg.bLocalPlayerInvolved ), victim );
		x += iVictimTextWide;
	}
}

//-----------------------------------------------------------------------------
// Purpose: This message handler may be better off elsewhere
//-----------------------------------------------------------------------------
void CTFHudDeathNotice::RetireExpiredDeathNotices()
{
	// Remove any expired death notices.  Loop backwards because we might remove one
	int iCount = m_DeathNotices.Count();
	for ( int i = iCount - 1; i >= 0; i-- )
	{
		if ( gpGlobals->curtime > m_DeathNotices[i].GetExpiryTime() )
		{
			m_DeathNotices.Remove( i );
		}
	}

	// Do we have too many death messages in the queue?
	if ( m_DeathNotices.Count() > 0 &&
		m_DeathNotices.Count() > (int)m_flMaxDeathNotices )
	{
		// First, remove any notices not involving the local player, since they are lower priority.		
		iCount = m_DeathNotices.Count();
		int iNeedToRemove = iCount - (int)m_flMaxDeathNotices;
		// loop condition is iCount-1 because we won't remove the most recent death notice, otherwise
		// new non-local-player-involved messages would not appear if the queue was full of messages involving the local player
		for ( int i = 0; i < iCount - 1 && iNeedToRemove > 0; i++ )
		{
			if ( !m_DeathNotices[i].bLocalPlayerInvolved )
			{
				m_DeathNotices.Remove( i );
				iCount--;
				iNeedToRemove--;
			}
		}

		// Now that we've culled any non-local-player-involved messages up to the amount we needed to remove, see
		// if we've removed enough
		iCount = m_DeathNotices.Count();
		iNeedToRemove = iCount - (int)m_flMaxDeathNotices;
		if ( iNeedToRemove > 0 )
		{
			// if we still have too many messages, then just remove however many we need, oldest first
			for ( int i = 0; i < iNeedToRemove; i++ )
			{
				m_DeathNotices.Remove( 0 );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Server's told us that someone's died
//-----------------------------------------------------------------------------
void CTFHudDeathNotice::FireGameEvent( IGameEvent *event )
{
	if ( !g_TF_PR )
	{
		return;
	}

	if ( hud_deathnotice_time.GetFloat() == 0 )
	{
		return;
	}

	const char *pszEventName = event->GetName();

	// Add a new death message.  Note we always look it up by index rather than create a reference or pointer to it;
	// additional messages may get added during this function that cause the underlying array to get realloced, so don't
	// ever keep a pointer to memory here.
	int iMsg = AddDeathNoticeItem();
	int iLocalPlayerIndex = GetLocalPlayerIndex();

	if ( FStrEq( pszEventName, "player_death" ) || FStrEq( pszEventName, "object_destroyed" ) )
	{
		bool bIsObjectDestroyed = FStrEq( pszEventName, "object_destroyed" );
		int victim = engine->GetPlayerForUserID( event->GetInt( "userid" ) );
		int killer = engine->GetPlayerForUserID( event->GetInt( "attacker" ) );
		const char *killedwith = event->GetString( "weapon" );
		const char *killedwithweaponlog = event->GetString( "weapon_logclassname" );
		int nPowerupFlags = event->GetInt( "powerup_flags" );

		// Get the names of the players
		const char *killer_name = ( killer > 0 ) ? g_PR->GetPlayerName( killer ) : "";
		const char *victim_name = g_PR->GetPlayerName( victim );

		if ( !killer_name )
		{
			killer_name = "";
		}

		if ( !victim_name )
		{
			victim_name = "";
		}

		// Make a new death notice
		bool bLocalPlayerInvolved = false;
		if ( iLocalPlayerIndex == killer || iLocalPlayerIndex == victim )
		{
			bLocalPlayerInvolved = true;
		}

		for ( int i = 0; g_aPowerups[i].cond != TF_COND_LAST; i++ )
		{
			if ( ( nPowerupFlags & ( 1 << i ) ) && g_aPowerups[i].kill_bg && g_aPowerups[i].kill_bg[0] )
			{
				m_DeathNotices[iMsg].bCrit = true;
				m_DeathNotices[iMsg].iconCritDeath = GetIcon( g_aPowerups[i].kill_bg, bLocalPlayerInvolved );
				break;
			}
		}

		if ( !m_DeathNotices[iMsg].bCrit )
		{
			if ( event->GetInt( "damagebits" ) & DMG_CRITICAL )
			{
				m_DeathNotices[iMsg].bCrit = true;
				m_DeathNotices[iMsg].iconCritDeath = GetIcon( "d_crit", bLocalPlayerInvolved );
			}
			else
			{
				m_DeathNotices[iMsg].bCrit = false;
				m_DeathNotices[iMsg].iconCritDeath = NULL;
			}
		}

		m_DeathNotices[iMsg].bLocalPlayerInvolved = bLocalPlayerInvolved;
		m_DeathNotices[iMsg].Killer.iTeam = ( killer > 0 ) ? g_PR->GetTeam( killer ) : 0;
		m_DeathNotices[iMsg].Victim.iTeam = g_PR->GetTeam( victim );
		m_DeathNotices[iMsg].Killer.playerColor = ( killer > 0 ) ? g_TF_PR->GetPlayerColor( killer ) : COLOR_WHITE;
		m_DeathNotices[iMsg].Victim.playerColor = g_TF_PR->GetPlayerColor( victim );
		V_strcpy_safe( m_DeathNotices[iMsg].Killer.szName, killer_name );
		V_strcpy_safe( m_DeathNotices[iMsg].Victim.szName, victim_name );

		if ( killedwith && *killedwith )
		{
			V_sprintf_safe( m_DeathNotices[iMsg].szIcon, "d_%s", killedwith );
		}
		if ( !killer || killer == victim )
		{
			m_DeathNotices[iMsg].bSelfInflicted = true;
			m_DeathNotices[iMsg].Killer.szName[0] = 0;

			if ( event->GetInt( "damagebits" ) & DMG_FALL )
			{
				// special case text for falling death
				V_wcscpy_safe( m_DeathNotices[iMsg].wzInfoText, g_pVGuiLocalize->Find( "#DeathMsg_Fall" ) );
			}
			else if ( /*( event->GetInt( "damagebits" ) & DMG_TRAIN ) ||*/ ( 0 == V_stricmp( m_DeathNotices[iMsg].szIcon, "d_tracktrain" ) ) )
			{
				// special case icon for hit-by-vehicle death
				V_strcpy_safe( m_DeathNotices[iMsg].szIcon, "d_vehicle" );
			}
			/*else if ( event->GetInt( "damagebits" ) & DMG_SAWBLADE )
			{
				V_strcpy_safe( m_DeathNotices[iMsg].szIcon, "d_saw_kill" );
			}*/
		}

		char sDeathMsg[512];

		// Record the death notice in the console
		if ( m_DeathNotices[iMsg].bSelfInflicted )
		{
			if ( !strcmp( m_DeathNotices[iMsg].szIcon, "d_worldspawn" ) )
			{
				V_sprintf_safe( sDeathMsg, "%s died.", m_DeathNotices[iMsg].Victim.szName );
			}
			else	// d_world
			{
				V_sprintf_safe( sDeathMsg, "%s suicided.", m_DeathNotices[iMsg].Victim.szName );
			}
		}
		else
		{
			V_sprintf_safe( sDeathMsg, "%s killed %s", m_DeathNotices[iMsg].Killer.szName, m_DeathNotices[iMsg].Victim.szName );

			if ( killedwithweaponlog && killedwithweaponlog[0] && ( killedwithweaponlog[0] > 13 ) )
			{
				V_strcat_safe( sDeathMsg, VarArgs( " with %s.", killedwithweaponlog ) );
			}
			else if ( m_DeathNotices[iMsg].szIcon[0] && ( m_DeathNotices[iMsg].szIcon[0] > 13 ) )
			{
				V_strcat_safe( sDeathMsg, VarArgs( " with %s.", &m_DeathNotices[iMsg].szIcon[2] ) );
			}
		}

		if ( FStrEq( pszEventName, "player_death" ) )
		{
			if ( m_DeathNotices[iMsg].bCrit )
			{
				Msg( "%s (crit)\n", sDeathMsg );
			}
			else
			{
				Msg( "%s\n", sDeathMsg );
			}
		}

		int iAssisterID = engine->GetPlayerForUserID( event->GetInt( "assister" ) );
		const char *assister_name = ( iAssisterID > 0 ? g_PR->GetPlayerName( iAssisterID ) : NULL );

		if ( assister_name )
		{
#if 1
			// Base TF2 assumes that the assister and the killer are on the same team, thus it 
			// writes both in the same string, which in turn gives them both the killer's team color
			// whether the assister is on the killer's team or not. -danielmm8888
			m_DeathNotices[iMsg].Assister.iTeam = ( iAssisterID > 0 ) ? g_TF_PR->GetTeam( iAssisterID ) : 0;
			m_DeathNotices[iMsg].Assister.playerColor = g_TF_PR->GetPlayerColor( iAssisterID );

			V_strcpy_safe( m_DeathNotices[iMsg].Assister.szName, assister_name );
#else
			// This is the old code used for assister handling.
			char szKillerBuf[MAX_PLAYER_NAME_LENGTH * 2];
			V_sprintf_safe( szKillerBuf, "%s + %s", m_DeathNotices[iDeathNoticeMsg].Killer.szName, assister_name );
			V_strcpy_safe( m_DeathNotices[iDeathNoticeMsg].Killer.szName, szKillerBuf );
#endif

			if ( iLocalPlayerIndex == iAssisterID )
			{
				m_DeathNotices[iMsg].bLocalPlayerInvolved = true;
			}
		}

		// if this death involved a player dominating another player or getting revenge on another player, add an additional message
		// mentioning that
		int iKillerID = engine->GetPlayerForUserID( event->GetInt( "attacker" ) );
		int iVictimID = engine->GetPlayerForUserID( event->GetInt( "userid" ) );
		int nDeathFlags = event->GetInt( "death_flags" );

		if ( !bIsObjectDestroyed )
		{
			if ( nDeathFlags & TF_DEATH_DOMINATION )
			{
				AddAdditionalMsg( iKillerID, iVictimID, "#Msg_Dominating" );
				PlayRivalrySounds( iKillerID, iVictimID, TF_DEATH_DOMINATION );
			}
			if ( ( nDeathFlags & TF_DEATH_ASSISTER_DOMINATION ) && ( iAssisterID > 0 ) )
			{
				AddAdditionalMsg( iAssisterID, iVictimID, "#Msg_Dominating" );
				PlayRivalrySounds( iAssisterID, iVictimID, TF_DEATH_DOMINATION );
			}
			if ( nDeathFlags & TF_DEATH_REVENGE )
			{
				AddAdditionalMsg( iKillerID, iVictimID, "#Msg_Revenge" );
				PlayRivalrySounds( iKillerID, iVictimID, TF_DEATH_REVENGE );
			}
			if ( ( nDeathFlags & TF_DEATH_ASSISTER_REVENGE ) && ( iAssisterID > 0 ) )
			{
				AddAdditionalMsg( iAssisterID, iVictimID, "#Msg_Revenge" );
				PlayRivalrySounds( iAssisterID, iVictimID, TF_DEATH_REVENGE );
			}
		}
		else
		{
			// if this is an object destroyed message, set the victim name to "<object type> (<owner>)"
			int iObjectType = event->GetInt( "objecttype" );
			if ( iObjectType >= 0 && iObjectType < OBJ_LAST )
			{
				// get the localized name for the object
				char szLocalizedObjectName[MAX_PLAYER_NAME_LENGTH];
				szLocalizedObjectName[ 0 ] = 0;
				const wchar_t* wszLocalizedObjectName = g_pVGuiLocalize->Find( szLocalizedObjectNames[iObjectType] );
				if ( wszLocalizedObjectName )
				{
					g_pVGuiLocalize->ConvertUnicodeToANSI( wszLocalizedObjectName, szLocalizedObjectName, ARRAYSIZE( szLocalizedObjectName ) );
				}
				else
				{
					Warning( "Couldn't find localized object name for '%s'\n", szLocalizedObjectNames[iObjectType] );
					Q_strncpy( szLocalizedObjectName, szLocalizedObjectNames[iObjectType], sizeof( szLocalizedObjectName ) );
				}

				// compose the string
				DeathNoticeItem& msg = m_DeathNotices[ iMsg ];
				if ( msg.Victim.szName[0] )
				{
					char szVictimBuf[MAX_PLAYER_NAME_LENGTH * 2];
					Q_snprintf( szVictimBuf, ARRAYSIZE( szVictimBuf ), "%s (%s)", szLocalizedObjectName, msg.Victim.szName );
					Q_strncpy( msg.Victim.szName, szVictimBuf, ARRAYSIZE( msg.Victim.szName ) );
				}
				else
				{
					Q_strncpy( msg.Victim.szName, szLocalizedObjectName, ARRAYSIZE( msg.Victim.szName ) );
				}

			}
			else
			{
				Assert( false ); // invalid object type
			}
		}

		int iCustomDamage = event->GetInt( "customkill" );

		switch ( iCustomDamage )
		{
		case TF_DMG_CUSTOM_BACKSTAB:
			V_strcpy_safe( m_DeathNotices[iMsg].szIcon, "d_backstab" );
			break;
		case TF_DMG_CUSTOM_HEADSHOT:
		{
			const char *pszIcon = "d_headshot";

			switch( event->GetInt( "weaponid" ) )
			{
				case TF_WEAPON_COMPOUND_BOW:
					pszIcon = "d_headshot_huntsman";
					break;
				case TF_WEAPON_LEVERRIFLE:
					pszIcon = "d_headshot_leverrifle";
					break;
			};

			m_DeathNotices[iMsg].iconHeadshot = GetIcon( pszIcon, bLocalPlayerInvolved );
			break;
		}
		case TF_DMG_CUSTOM_SUICIDE:
		{
			// display a different message if this was suicide, or assisted suicide (suicide w/recent damage, kill awarded to damager)
			bool bAssistedSuicide = event->GetInt( "userid" ) != event->GetInt( "attacker" );
			const wchar_t *pMsg = g_pVGuiLocalize->Find( bAssistedSuicide ? "#DeathMsg_AssistedSuicide" : "#DeathMsg_Suicide" );
			if ( pMsg )
			{
				V_wcscpy_safe( m_DeathNotices[iMsg].wzInfoText, pMsg );
			}
			break;
		}
		/*case TF_DMG_CHAINSAW_BACKSTAB:
			V_strcpy_safe( m_DeathNotices[iMsg].szIcon, "d_chainsaw_backstab" );
			break;*/

		default:
			break;
		}
	}
	else if ( FStrEq( "teamplay_flag_event", pszEventName ) )
	{
		const char *pszMsgKey = NULL;
		int iEventType = event->GetInt( "eventtype" );
		switch ( iEventType )
		{
		case TF_FLAGEVENT_PICKUP:
			pszMsgKey = "#Msg_PickedUpFlag";
			break;
		case TF_FLAGEVENT_CAPTURE:
			pszMsgKey = "#Msg_CapturedFlag";
			break;
		case TF_FLAGEVENT_DEFEND:
			pszMsgKey = "#Msg_DefendedFlag";
			break;

			// Add this when we can get localization for it
			//case TF_FLAGEVENT_DROPPED: 
			//	pszMsgKey = "#Msg_DroppedFlag"; 
			//	break;

		default:
			// unsupported, don't put anything up			
			m_DeathNotices.Remove( iMsg );
			return;
		}

		wchar_t *pwzEventText = g_pVGuiLocalize->Find( pszMsgKey );
		Assert( pwzEventText );
		if ( pwzEventText )
		{
			V_wcscpy_safe( m_DeathNotices[iMsg].wzInfoText, pwzEventText );
		}

		int iPlayerIndex = event->GetInt( "player" );
		const char *szPlayerName = g_PR->GetPlayerName( iPlayerIndex );
		V_strcpy_safe( m_DeathNotices[iMsg].Killer.szName, szPlayerName );
		m_DeathNotices[iMsg].Killer.iTeam = g_PR->GetTeam( iPlayerIndex );
		if ( iLocalPlayerIndex == iPlayerIndex )
			m_DeathNotices[iMsg].bLocalPlayerInvolved = true;

		// set the icon
		int iTeam = m_DeathNotices[iMsg].Killer.iTeam;
		Assert( iTeam >= FIRST_GAME_TEAM && iTeam < TF_TEAM_COUNT );
		if ( iTeam >= FIRST_GAME_TEAM && iTeam < TF_TEAM_COUNT )
		{
			V_sprintf_safe( m_DeathNotices[iMsg].szIcon, "d_%scapture", g_aTeamLowerNames[iTeam] );
		}
	}

	//OnGameEvent( event, iMsg );

	if ( !m_DeathNotices[iMsg].iconDeath && m_DeathNotices[iMsg].szIcon )
	{
		// Try and find the death identifier in the icon list
		// On consoles, we flip usage of the inverted icon to make it more visible
		bool bInverted = m_DeathNotices[iMsg].bLocalPlayerInvolved;
		if ( IsConsole() )
		{
			bInverted = !bInverted;
		}
		m_DeathNotices[iMsg].iconDeath = GetIcon( m_DeathNotices[iMsg].szIcon, bInverted );
		if ( !m_DeathNotices[iMsg].iconDeath )
		{
			// Can't find it, so use the default skull & crossbones icon
			m_DeathNotices[iMsg].iconDeath = GetIcon( "d_skull_tf", m_DeathNotices[iMsg].bLocalPlayerInvolved );
		}
	}
}

void CTFHudDeathNotice::PlayRivalrySounds( int iKillerIndex, int iVictimIndex, int iType )
{
	int iLocalPlayerIndex = GetLocalPlayerIndex();

	//We're not involved in this kill
	if ( iKillerIndex != iLocalPlayerIndex && iVictimIndex != iLocalPlayerIndex )
		return;

	// Stop any sounds that are already playing to avoid ear rape in case of
	// multiple dominations at once.
	C_BaseEntity::StopSound( SOUND_FROM_LOCAL_PLAYER, "Game.Domination" );
	C_BaseEntity::StopSound( SOUND_FROM_LOCAL_PLAYER, "Game.Nemesis" );
	C_BaseEntity::StopSound( SOUND_FROM_LOCAL_PLAYER, "Game.Revenge" );

	const char *pszSoundName = NULL;

	if ( iType == TF_DEATH_DOMINATION )
	{
		if ( iKillerIndex == iLocalPlayerIndex )
		{
			pszSoundName = "Game.Domination";
		}
		else if ( iVictimIndex == iLocalPlayerIndex )
		{
			pszSoundName = "Game.Nemesis";
		}
	}
	else if ( iType == TF_DEATH_REVENGE )
	{
		pszSoundName = "Game.Revenge";
	}

	CLocalPlayerFilter filter;
	C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, pszSoundName );
}

//-----------------------------------------------------------------------------
// Purpose: Gets the localized name of the control point sent in the event
//-----------------------------------------------------------------------------
void CTFHudDeathNotice::GetLocalizedControlPointName( IGameEvent *event, char *namebuf, int namelen )
{
	// Cap point name ( MATTTODO: can't we find this from the point index ? )
	const char *pName = event->GetString( "cpname", "Unnamed Control Point" );
	const wchar_t *pLocalizedName = g_pVGuiLocalize->Find( pName );

	if ( pLocalizedName )
	{
		g_pVGuiLocalize->ConvertUnicodeToANSI( pLocalizedName, namebuf, namelen );
	}
	else
	{
		V_strncpy( namebuf, pName, namelen );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Adds a new death notice to the queue
//-----------------------------------------------------------------------------
int CTFHudDeathNotice::AddDeathNoticeItem()
{
	int iMsg = m_DeathNotices.AddToTail();
	DeathNoticeItem &msg = m_DeathNotices[iMsg];
	msg.flCreationTime = gpGlobals->curtime;
	return iMsg;
}

//-----------------------------------------------------------------------------
// Purpose: draw text helper
//-----------------------------------------------------------------------------
void CTFHudDeathNotice::DrawText( int x, int y, HFont hFont, Color clr, const wchar_t *szText )
{
	surface()->DrawSetTextPos( x, y );
	surface()->DrawSetTextColor( clr );
	surface()->DrawSetTextFont( hFont );	//reset the font, draw icon can change it
	surface()->DrawUnicodeString( szText, vgui::FONT_DRAW_NONADDITIVE );
}

//-----------------------------------------------------------------------------
// Purpose: Creates a rounded-corner polygon that fits in the specified bounds
//-----------------------------------------------------------------------------
void CTFHudDeathNotice::GetBackgroundPolygonVerts( int x0, int y0, int x1, int y1, int iVerts, vgui::Vertex_t vert[] )
{
	Assert( iVerts == NUM_BACKGROUND_COORD );
	// use the offsets we generated for one corner and apply those to the passed-in dimensions to create verts for the poly
	for ( int i = 0; i < NUM_CORNER_COORD; i++ )
	{
		int j = ( NUM_CORNER_COORD - 1 ) - i;
		// upper left corner
		vert[i].Init( Vector2D( x0 + m_CornerCoord[i].x, y0 + m_CornerCoord[i].y ) );
		// upper right corner
		vert[i + NUM_CORNER_COORD].Init( Vector2D( x1 - m_CornerCoord[j].x, y0 + m_CornerCoord[j].y ) );
		// lower right corner
		vert[i + ( NUM_CORNER_COORD * 2 )].Init( Vector2D( x1 - m_CornerCoord[i].x, y1 - m_CornerCoord[i].y ) );
		// lower left corner
		vert[i + ( NUM_CORNER_COORD * 3 )].Init( Vector2D( x0 + m_CornerCoord[j].x, y1 - m_CornerCoord[j].y ) );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Gets specified icon
//-----------------------------------------------------------------------------
CHudTexture *CTFHudDeathNotice::GetIcon( const char *szIcon, bool bInvert )
{
	// get the inverted version if specified
	if ( bInvert && 0 == V_strncmp( "d_", szIcon, 2 ) )
	{
		// change prefix from d_ to dneg_
		char szIconTmp[255] = "dneg_";
		V_strcat( szIconTmp, szIcon + 2, ARRAYSIZE( szIconTmp ) );
		CHudTexture *pIcon = gHUD.GetIcon( szIconTmp );
		// return inverted version if found
		if ( pIcon )
			return pIcon;
		// if we didn't find the inverted version, keep going and try the normal version
	}
	return gHUD.GetIcon( szIcon );
}

//-----------------------------------------------------------------------------
// Purpose: Creates the offsets for rounded corners based on current screen res
//-----------------------------------------------------------------------------
void CTFHudDeathNotice::CalcRoundedCorners()
{
	// generate the offset geometry for upper left corner
	int iMax = ARRAYSIZE( m_CornerCoord );
	for ( int i = 0; i < iMax; i++ )
	{
		m_CornerCoord[i].x = m_flCornerRadius * ( 1 - cos( ( (float)i / (float)( iMax - 1 ) ) * ( M_PI / 2 ) ) );
		m_CornerCoord[i].y = m_flCornerRadius * ( 1 - sin( ( (float)i / (float)( iMax - 1 ) ) * ( M_PI / 2 ) ) );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Adds an additional death message
//-----------------------------------------------------------------------------
void CTFHudDeathNotice::AddAdditionalMsg( int iKillerID, int iVictimID, const char *pMsgKey )
{
	DeathNoticeItem &msg2 = m_DeathNotices[AddDeathNoticeItem()];
	V_strcpy_safe( msg2.Killer.szName, g_PR->GetPlayerName( iKillerID ) );
	V_strcpy_safe( msg2.Victim.szName, g_PR->GetPlayerName( iVictimID ) );

	msg2.Killer.iTeam = g_PR->GetTeam( iKillerID );
	msg2.Killer.playerColor = g_TF_PR->GetPlayerColor( iKillerID );
	msg2.Victim.iTeam = g_PR->GetTeam( iVictimID );
	msg2.Victim.playerColor = g_TF_PR->GetPlayerColor( iVictimID );

	const wchar_t *wzMsg = g_pVGuiLocalize->Find( pMsgKey );
	if ( wzMsg )
	{
		V_wcscpy_safe( msg2.wzInfoText, wzMsg );
	}
	msg2.iconDeath = m_iconDomination;
	int iLocalPlayerIndex = GetLocalPlayerIndex();
	if ( iLocalPlayerIndex == iVictimID || iLocalPlayerIndex == iKillerID )
	{
		msg2.bLocalPlayerInvolved = true;
	}
}

//-----------------------------------------------------------------------------
// Purpose: returns the color to draw text in for this team.  
//-----------------------------------------------------------------------------
Color CTFHudDeathNotice::GetPlayerColor( const DeathNoticePlayer &playerItem, bool bLocalPlayerInvolved /*= false*/ )
{
	if ( TFGameRules() && TFGameRules()->IsDeathmatch() )
	{
		return playerItem.playerColor;
	}

	switch ( playerItem.iTeam )
	{
	case TF_TEAM_RED:
		return m_clrRedText;
		break;
	case TF_TEAM_BLUE:
		return m_clrBlueText;
		break;
	case TF_TEAM_GREEN:
		return m_clrGreenText;
		break;
	case TF_TEAM_YELLOW:
		return m_clrYellowText;
		break;
	case TEAM_UNASSIGNED:
		return bLocalPlayerInvolved ? Color( 0, 0, 0, 255 ) : Color( 255, 255, 255, 255 );
		break;
	default:
		AssertOnce( false );	// invalid team
		return bLocalPlayerInvolved ? COLOR_BLACK : COLOR_WHITE;
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Gets the expiry time for this death notice item
//-----------------------------------------------------------------------------
float DeathNoticeItem::GetExpiryTime()
{
	float flDuration = hud_deathnotice_time.GetFloat();
	if ( bLocalPlayerInvolved )
	{
		// if the local player is involved, make the message last longer
		flDuration *= 2;
	}
	return flCreationTime + flDuration;
}
