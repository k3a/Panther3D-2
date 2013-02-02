/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#include <DX/D3D10.h>
#include <DX/D3DX10.h>
//#include <DX/dxgi.h>
//#include <DX/D3DCompiler.h>
#include <types.h>
#include "IShader.h"
#include "../VertexBuffer.h"
#include "../ShaderBuffer.h"
#include "../Texture.h"

namespace P3D
{
	class CShaderHLSL : public IShader
	{
	public:
		CShaderHLSL();
		~CShaderHLSL();
		bool Create(sShaderDesc &desc);
		void Use();
		ShaderHandle GetHandle(const char *name);
		void SetBuffer(ShaderHandle handle, CShaderBuffer *buffer);
		void SetTexture(ShaderHandle handle, ITexture *tex);

		void OnLostDevice() {};
		void OnResetDevice() {};

		void SetHandleValueVec3(ShaderHandle handle, const Vec3 *value) {};
		void SetHandleValueMatrix(ShaderHandle handle, const Matrix *matrix) {};
		
	private:
		sShaderDesc m_desc;
		CVertexDeclaration *m_pVertDecl;
		UINT m_numResources;
		tArray<UINT> m_parameters;

		ID3D10VertexShader *m_pVS;
		ID3D10GeometryShader *m_pGS;
		ID3D10PixelShader *m_pPS;
		ID3D10ShaderReflection *m_pReflection;

	};
};
