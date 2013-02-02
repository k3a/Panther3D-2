/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#if defined(_DX) && _DX == 9

#include "ShaderManagerHLSL.h"
#include "../Renderer.h"

namespace P3D
{

	extern LPDIRECT3DDEVICE9 g_pD3ddev;

	CShaderManagerHLSL::CShaderManagerHLSL()
	{

	}

	CShaderManagerHLSL::~CShaderManagerHLSL()
	{

	}

	bool CShaderManagerHLSL::Create(void *device)
	{
		CON(MSG_INFO, _W("Using HLSL shaders."));
		CON(MSG_INFO, _W(" ...using %s vertex shader profile"), _A2W(D3DXGetVertexShaderProfile(g_pD3ddev)));
		CON(MSG_INFO, _W(" ...using %s pixel shader profile"), _A2W(D3DXGetPixelShaderProfile(g_pD3ddev)));
		return true;
	}

	inline const wchar* CShaderManagerHLSL::GetShaderProfile(eShaderType shaderType)
	{
		if(shaderType == SHADERTYPE_VERTEX_SHADER) return _A2W(D3DXGetVertexShaderProfile(g_pD3ddev));
		else if(shaderType == SHADERTYPE_PIXEL_SHADER) return _A2W(D3DXGetPixelShaderProfile(g_pD3ddev));
	}

};

#endif