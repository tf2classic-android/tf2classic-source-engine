//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Desaturation shader
//
// $NoKeywords: $
//===========================================================================//

#include "BaseVSShader.h"
#include "desaturate_vs20.inc"
#include "desaturate_ps20b.inc"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


BEGIN_VS_SHADER_FLAGS(desaturate, "Help for desaturate", SHADER_NOT_EDITABLE)
BEGIN_SHADER_PARAMS
SHADER_PARAM(BASETEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "Framebuffer")
SHADER_PARAM(AMOUNT, SHADER_PARAM_TYPE_FLOAT, "1.0", "Desaturation amount")
END_SHADER_PARAMS

SHADER_INIT_PARAMS()
{
	SET_FLAGS2(MATERIAL_VAR2_NEEDS_FULL_FRAME_BUFFER_TEXTURE);
}

SHADER_FALLBACK
{
	// No fallback
	return 0;
}

SHADER_INIT
{
	// Load textures
	if (params[BASETEXTURE]->IsDefined())
	{
		LoadTexture(BASETEXTURE);
	}
	if (!params[AMOUNT]->IsDefined())
	{
		params[AMOUNT]->SetFloatValue(1.0);
	}
}

SHADER_DRAW
{
	SHADOW_STATE
	{
		// Set up shader
		pShaderShadow->EnableDepthWrites(false);
		pShaderShadow->VertexShaderVertexFormat(VERTEX_POSITION, 1, 0, 0);

		pShaderShadow->EnableTexture(SHADER_SAMPLER0, true);

		DECLARE_STATIC_VERTEX_SHADER(desaturate_vs20);
		SET_STATIC_VERTEX_SHADER(desaturate_vs20);

		DECLARE_STATIC_PIXEL_SHADER(desaturate_ps20b);
		SET_STATIC_PIXEL_SHADER(desaturate_ps20b);
	}

	DYNAMIC_STATE
	{
		// Set up shader data
		BindTexture(SHADER_SAMPLER0, BASETEXTURE);

		float amount = params[AMOUNT]->GetFloatValue();
		pShaderAPI->SetPixelShaderConstant(0, &amount);

		DECLARE_DYNAMIC_VERTEX_SHADER(desaturate_vs20);
		SET_DYNAMIC_VERTEX_SHADER(desaturate_vs20);

		DECLARE_DYNAMIC_PIXEL_SHADER(desaturate_ps20b);
		SET_DYNAMIC_PIXEL_SHADER(desaturate_ps20b);
	}
	Draw();
}
END_SHADER