#include "cbase.h"
#include "tf_hud_domination.h"
#include "vgui/IScheme.h"
#include "tf_gamerules.h"
#include "c_tf_team.h"
#include "c_team_objectiveresource.h"

extern ConVar tf2c_domination_points_per_round;

CTFHudDomination::CTFHudDomination( vgui::Panel *pParent, const char *pszName ) : vgui::EditablePanel( pParent, pszName )
{
	vgui::ivgui()->AddTickSignal( GetVPanel() );

	ListenForGameEvent( "game_maploaded" );
}

CTFHudDomination::~CTFHudDomination()
{
}

void CTFHudDomination::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	KeyValues *pConditions = NULL;

	if( TFGameRules() && TFGameRules()->IsFourTeamGame() )
	{
		pConditions = new KeyValues( "conditions" );
		AddSubKeyNamed( pConditions, "if_4team" );
	}

	LoadControlSettings( "resource/ui/HudObjectiveDomination.res", NULL, NULL, pConditions );

	if( pConditions )
	{
		pConditions->deleteThis();
	}
}

void CTFHudDomination::OnTick()
{
#if 0
	if( TFGameRules() && !TFGameRules()->IsFourTeamGame() )
	{
		return;
	}
#endif

	for( int i = TF_TEAM_RED; i < GetNumberOfTeams(); ++i )
	{
		C_TFTeam *pGlobalTFTeam = GetGlobalTFTeam( i );
		if( pGlobalTFTeam )
		{
			char szVar[16] = { NULL };
			Q_snprintf( szVar, sizeof( szVar ), "%sscore", g_aTeamParticleNames[i] );
			SetDialogVariable( szVar, pGlobalTFTeam->GetRoundScore() );

			int nMult = 0;
			if( ObjectiveResource() && (ObjectiveResource()->GetNumControlPoints() > 0) )
			{
				for( int j = 0; j < ObjectiveResource()->GetNumControlPoints(); j++ )
				{
					if( ObjectiveResource()->IsInMiniRound( j ) )
					{
						int v9 = (j < ObjectiveResource()->GetNumControlPoints()) ? ObjectiveResource()->GetOwningTeam( j ) : 0;
						if( v9 == i )
						{
							++nMult;
						}
					}
				}
			}
			Q_snprintf( szVar, sizeof( szVar ), "%smult", g_aTeamParticleNames[i] );
			char szMult[8] = { NULL };
			if( nMult != 0 )
			{
				Q_snprintf( szMult, sizeof( szMult ), "x%d", nMult );
			}
			SetDialogVariable( szVar, szMult );
		}
	}

	SetDialogVariable( "rounds", tf2c_domination_points_per_round.GetInt() );
}

bool CTFHudDomination::IsVisible()
{
	return !IsInFreezeCam() && (TFGameRules() && !TFGameRules()->IsInWaitingForPlayers()) && BaseClass::IsVisible();
}

void CTFHudDomination::FireGameEvent( IGameEvent *event )
{
	const char *pszEventName = event->GetName();
	if( !Q_stricmp( pszEventName, "game_maploaded" ) )
	{
		InvalidateLayout( false, true );
	}
}
