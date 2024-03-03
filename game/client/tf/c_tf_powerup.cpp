//=============================================================================//
//
// Purpose: CTF AmmoPack.
//
//=============================================================================//
#include "cbase.h"
#include "c_tf_powerup.h"
#include "entity_capture_flag.h"
#include "view.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_CLIENTCLASS_DT( C_TFPowerup, DT_TFPowerup, CTFPowerup )
	RecvPropBool( RECVINFO( m_bDisabled ) ),
	RecvPropBool( RECVINFO( m_bRespawning ) ),
	RecvPropTime( RECVINFO( m_flRespawnStartTime ) ),
	RecvPropTime( RECVINFO( m_flRespawnTime ) ),
END_RECV_TABLE()


C_TFPowerup::C_TFPowerup()
{
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void C_TFPowerup::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );

	if ( updateType == DATA_UPDATE_CREATED )
	{
		m_ReturnProgressMaterial_Full.Init( "vgui/flagtime_full", TEXTURE_GROUP_VGUI );
		m_ReturnProgressMaterial_Empty.Init( "vgui/flagtime_empty", TEXTURE_GROUP_VGUI );
	}
}

ShadowType_t C_TFPowerup::ShadowCastType( void ) 
{
	if ( m_bRespawning )
		 return SHADOWS_NONE;

	return SHADOWS_RENDER_TO_TEXTURE_DYNAMIC;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int C_TFPowerup::DrawModel( int flags )
{
	//int ret = 0;
	
	//Don't draw model if we're respawning -DAN_H
	//if( !m_bRespawning )
	//	ret = BaseClass::DrawModel(flags);

	int ret = BaseClass::DrawModel(flags);

	if ( m_bRespawning && ShouldShowRespawnTimer() )
	{
		if ( !m_ReturnProgressMaterial_Full.IsValid() || !m_ReturnProgressMaterial_Empty.IsValid() )
		{
			return ret;
		}

		CMatRenderContextPtr pRenderContext( materials );

		float flSize = GetRespawnTimerSize();

		// Align it towards the viewer
		Vector vUp = CurrentViewUp();
		Vector vRight = CurrentViewRight();
		if ( fabs( vRight.z ) > 0.95 )	// don't draw it edge-on
			return ret;

		Vector vOrigin = GetAbsOrigin();
		QAngle vAngle = vec3_angle;

		vRight.z = 0;
		VectorNormalize( vRight );

		Color colRespawn = GetRespawnTimerColor();
		unsigned char *ubColor = (unsigned char *)&colRespawn;

		// First we draw a quad of a complete icon, background
		CMeshBuilder meshBuilder;

		pRenderContext->Bind( m_ReturnProgressMaterial_Empty );
		IMesh *pMesh = pRenderContext->GetDynamicMesh();

		meshBuilder.Begin( pMesh, MATERIAL_QUADS, 1 );

		meshBuilder.Color4ubv( ubColor );
		meshBuilder.TexCoord2f( 0, 0, 0 );
		meshBuilder.Position3fv( ( vOrigin + ( vRight * -flSize ) + ( vUp * flSize ) ).Base() );
		meshBuilder.AdvanceVertex();

		meshBuilder.Color4ubv( ubColor );
		meshBuilder.TexCoord2f( 0, 1, 0 );
		meshBuilder.Position3fv( ( vOrigin + ( vRight * flSize ) + ( vUp * flSize ) ).Base() );
		meshBuilder.AdvanceVertex();

		meshBuilder.Color4ubv( ubColor );
		meshBuilder.TexCoord2f( 0, 1, 1 );
		meshBuilder.Position3fv( ( vOrigin + ( vRight * flSize ) + ( vUp * -flSize ) ).Base() );
		meshBuilder.AdvanceVertex();

		meshBuilder.Color4ubv( ubColor );
		meshBuilder.TexCoord2f( 0, 0, 1 );
		meshBuilder.Position3fv( ( vOrigin + ( vRight * -flSize ) + ( vUp * -flSize ) ).Base() );
		meshBuilder.AdvanceVertex();

		meshBuilder.End();

		pMesh->Draw();

		float flProgress = ( m_flRespawnTime - gpGlobals->curtime ) / ( m_flRespawnTime - m_flRespawnStartTime );

		pRenderContext->Bind( m_ReturnProgressMaterial_Full );
		pMesh = pRenderContext->GetDynamicMesh();

		vRight *= flSize * 2;
		vUp *= flSize * -2;

		// Next we're drawing the circular progress bar, in 8 segments
		// For each segment, we calculate the vertex position that will draw
		// the slice.
		int i;
		for ( i = 0; i<8; i++ )
		{
			if ( flProgress < Segments[i].maxProgress )
			{
				CMeshBuilder meshBuilder_Full;

				meshBuilder_Full.Begin( pMesh, MATERIAL_TRIANGLES, 3 );

				// vert 0 is ( 0.5, 0.5 )
				meshBuilder_Full.Color4ubv( ubColor );
				meshBuilder_Full.TexCoord2f( 0, 0.5, 0.5 );
				meshBuilder_Full.Position3fv( vOrigin.Base() );
				meshBuilder_Full.AdvanceVertex();

				// Internal progress is the progress through this particular slice
				float internalProgress = RemapVal( flProgress, Segments[i].maxProgress - 0.125, Segments[i].maxProgress, 0.0, 1.0 );
				internalProgress = clamp( internalProgress, 0.0, 1.0 );

				// Calculate the x,y of the moving vertex based on internal progress
				float swipe_x = Segments[i].vert2x - ( 1.0 - internalProgress ) * 0.5 * Segments[i].swipe_dir_x;
				float swipe_y = Segments[i].vert2y - ( 1.0 - internalProgress ) * 0.5 * Segments[i].swipe_dir_y;

				// vert 1 is calculated from progress
				meshBuilder_Full.Color4ubv( ubColor );
				meshBuilder_Full.TexCoord2f( 0, swipe_x, swipe_y );
				meshBuilder_Full.Position3fv( ( vOrigin + ( vRight * ( swipe_x - 0.5 ) ) + ( vUp *( swipe_y - 0.5 ) ) ).Base() );
				meshBuilder_Full.AdvanceVertex();

				// vert 2 is ( Segments[i].vert1x, Segments[i].vert1y )
				meshBuilder_Full.Color4ubv( ubColor );
				meshBuilder_Full.TexCoord2f( 0, Segments[i].vert2x, Segments[i].vert2y );
				meshBuilder_Full.Position3fv( ( vOrigin + ( vRight * ( Segments[i].vert2x - 0.5 ) ) + ( vUp *( Segments[i].vert2y - 0.5 ) ) ).Base() );
				meshBuilder_Full.AdvanceVertex();

				meshBuilder_Full.End();

				pMesh->Draw();
			}
		}
	}

	return ret;
}
