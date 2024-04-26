#include "cbase.h"
#include "tf_rotatingpanel.h"
#include <vgui/ISurface.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: SPINNING SHIT
//-----------------------------------------------------------------------------
DECLARE_BUILD_FACTORY( CTFRotatingImagePanel );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFRotatingImagePanel::CTFRotatingImagePanel( Panel *parent, const char *name ) : EditablePanel( parent, name )
{
	m_flAngle = 0.0f;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFRotatingImagePanel::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );
	m_Material.Init( inResourceData->GetString( "image", "" ), TEXTURE_GROUP_VGUI );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
float CTFRotatingImagePanel::GetAngleRotation( void )
{
	int _x, _y;
	surface()->SurfaceGetCursorPos( _x, _y );
	GetParent()->LocalToScreen( _x, _y );
	int x, y;
	GetPos( x, y );
	x += GetWide() / 2.0;
	y += GetTall() / 2.0;
	float deltaY = y - _y;
	float deltaX = x - _x;
	return RAD2DEG( atan2( deltaY, deltaX ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFRotatingImagePanel::Paint()
{
	IMaterial *pMaterial = m_Material;
	int x = 0;
	int y = 0;
	ipanel()->GetAbsPos( GetVPanel(), x, y );
	int nWidth = GetWide();
	int nHeight = GetTall();

	m_flAngle += 15.0f * gpGlobals->frametime;
	AngleNormalizePositive( m_flAngle );

	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->MatrixMode( MATERIAL_MODEL );
	pRenderContext->PushMatrix();

	VMatrix panelRotation;
	panelRotation.Identity();
	MatrixBuildRotationAboutAxis( panelRotation, Vector( 0, 0, 1 ), m_flAngle );
	panelRotation.SetTranslation( Vector( x + nWidth / 2, y + nHeight / 2, 0 ) );

	pRenderContext->LoadMatrix( panelRotation );
	IMesh *pMesh = pRenderContext->GetDynamicMesh( true, NULL, NULL, pMaterial );

	CMeshBuilder meshBuilder;
	meshBuilder.Begin( pMesh, MATERIAL_QUADS, 1 );

	meshBuilder.TexCoord2f( 0, 0, 0 );
	meshBuilder.Position3f( -nWidth / 2, -nHeight / 2, 0 );
	meshBuilder.Color4ub( 255, 255, 255, 255 );
	meshBuilder.AdvanceVertex();

	meshBuilder.TexCoord2f( 0, 1, 0 );
	meshBuilder.Position3f( nWidth / 2, -nHeight / 2, 0 );
	meshBuilder.Color4ub( 255, 255, 255, 255 );
	meshBuilder.AdvanceVertex();

	meshBuilder.TexCoord2f( 0, 1, 1 );
	meshBuilder.Position3f( nWidth / 2, nHeight / 2, 0 );
	meshBuilder.Color4ub( 255, 255, 255, 255 );
	meshBuilder.AdvanceVertex();

	meshBuilder.TexCoord2f( 0, 0, 1 );
	meshBuilder.Position3f( -nWidth / 2, nHeight / 2, 0 );
	meshBuilder.Color4ub( 255, 255, 255, 255 );
	meshBuilder.AdvanceVertex();
	meshBuilder.End();

	pMesh->Draw();
	pRenderContext->PopMatrix();
}
