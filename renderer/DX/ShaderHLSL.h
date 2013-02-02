/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#include <DX/d3d9.h>
#include <DX/d3dx9.h>
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

		void SetHandleValueVec3(ShaderHandle handle, const Vec3 *value) {};
		void SetHandleValueMatrix(ShaderHandle handle, const Matrix *matrix) {};

		void OnLostDevice() {};
		void OnResetDevice() {};

	private:
		sShaderDesc m_desc;
		CVertexDeclaration *m_pVertDecl;
		UINT m_numResources;
		tArray<D3DXHANDLE> m_parameters;

		IDirect3DVertexShader9 *m_pVS;
		IDirect3DPixelShader9 *m_pPS;
		ID3DXConstantTable *m_pConstTable;
	};
};
