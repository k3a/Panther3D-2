/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#if defined(_DX) && _DX == 9
#include "ShaderBufferHLSL.h"
#include "../renderer.h"

namespace P3D
{

	extern LPDIRECT3DDEVICE9 g_pD3ddev;

	CShaderBufferHLSL::CShaderBufferHLSL()
	{
		m_pBuffer = NULL;
	}

	CShaderBufferHLSL::~CShaderBufferHLSL()
	{
		SAFE_DELETE_ARRAY(m_pBuffer);
	}

	bool CShaderBufferHLSL::Create(const sShaderBufferDesc &desc, const void *data)
	{
		m_pBuffer = new BYTE[desc.BufferSize];

		m_desc = desc;
		return true;
	}

	void CShaderBufferHLSL::Update(const void *data)
	{
		memcpy(m_pBuffer, data, m_desc.BufferSize);
	}

	bool CShaderBufferHLSL::Map(void **data, bool discard)
	{
		*data = m_pBuffer;
		return true;
	}

	void CShaderBufferHLSL::Unmap()
	{
		
	}

};

#endif
