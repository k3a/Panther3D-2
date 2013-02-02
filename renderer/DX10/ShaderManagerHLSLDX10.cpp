/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#if defined(_DX) && _DX == 10

#include "ShaderManagerHLSL.h"
#include "../Renderer.h"

namespace P3D
{

	extern ID3D10Device* g_pD3ddev;
	
	CShaderManagerHLSL::CShaderManagerHLSL()
	{

	}

	CShaderManagerHLSL::~CShaderManagerHLSL()
	{

	}

	bool CShaderManagerHLSL::Create(void *device)
	{
		CON(MSG_INFO, _W("Using HLSL shaders."));
		CON(MSG_INFO, _W(" ...using %s vertex shader profile"), _A2W(D3D10GetVertexShaderProfile(g_pD3ddev)));
		CON(MSG_INFO, _W(" ...using %s geometry shader profile"), _A2W(D3D10GetGeometryShaderProfile(g_pD3ddev)));
		CON(MSG_INFO, _W(" ...using %s pixel shader profile"), _A2W(D3D10GetPixelShaderProfile(g_pD3ddev)));
		return true;
	}

	inline const wchar* CShaderManagerHLSL::GetShaderProfile(eShaderType shaderType)
	{
		if(shaderType == SHADERTYPE_VERTEX_SHADER) return _A2W(D3D10GetVertexShaderProfile(g_pD3ddev));
		else if(shaderType == SHADERTYPE_GEOMETRY_SHADER) return _A2W(D3D10GetGeometryShaderProfile(g_pD3ddev));
		else if(shaderType == SHADERTYPE_PIXEL_SHADER) return _A2W(D3D10GetPixelShaderProfile(g_pD3ddev));
	}

	void CShaderManagerHLSL::SetBuffer(eShaderType shaderType, UINT slot, CShaderBuffer *buffer)
	{
		//assertd(buffer, _W("NULL pointer to buffer"));
		ID3D10Buffer *cbuff[1];
		cbuff[0] = (ID3D10Buffer*)buffer->GetInternalBuffer();
		if(shaderType == SHADERTYPE_VERTEX_SHADER) g_pD3ddev->VSSetConstantBuffers(slot, 1, cbuff);
		else if(shaderType == SHADERTYPE_PIXEL_SHADER) g_pD3ddev->PSSetConstantBuffers(slot, 1, cbuff);
		else g_pD3ddev->GSSetConstantBuffers(slot, 1, cbuff);
	}

};

#endif