/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#if defined(_DX) && _DX == 10

#include "ShaderBufferHLSL.h"
#include "../renderer.h"

namespace P3D
{

	extern ID3D10Device* g_pD3ddev;

	CShaderBufferHLSL::CShaderBufferHLSL()
	{
		m_pBuffer = NULL;
	}

	CShaderBufferHLSL::~CShaderBufferHLSL()
	{
		SAFE_RELEASE(m_pBuffer);
	}

	bool CShaderBufferHLSL::Create(const sShaderBufferDesc &desc, const void *data)
	{
		D3D10_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = desc.BufferSize;
		cbDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
		cbDesc.MiscFlags = 0;

		if(desc.ResourceUsage == P3D_RESOURCEUSAGE_IMMUTABLE) cbDesc.Usage = D3D10_USAGE_IMMUTABLE;
		else if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DEFAULT)cbDesc.Usage = D3D10_USAGE_DEFAULT;
		else if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DYNAMIC)
		{
			if(desc.CPUAccess != P3D_CPUACCESS_READ) cbDesc.Usage = D3D10_USAGE_DYNAMIC;
			else cbDesc.Usage = D3D10_USAGE_STAGING;
		}
		else
		{
			CON(MSG_ERR, _W("Shader buffer creation failed. No apropriate ResourceUsage given."));
			return false;
		}
		
		if(desc.CPUAccess == P3D_CPUACCESS_NONE) cbDesc.CPUAccessFlags = 0;
		else if(desc.CPUAccess == P3D_CPUACCESS_READ) cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
		else if(desc.CPUAccess == P3D_CPUACCESS_WRITE) cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		else if(desc.CPUAccess == P3D_CPUACCESS_READ_WRITE) cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ || D3D10_CPU_ACCESS_WRITE;
		else
		{
			CON(MSG_ERR, _W("Shader buffer creation failed. No apropriate CPUAccess given."));
			return false;
		} 
		
		D3D10_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;
		if(!CheckHRResult( g_pD3ddev->CreateBuffer( &cbDesc, data ? &initData : NULL, &m_pBuffer ) ))
		{
			CON(MSG_ERR, _W("Shader buffer creation failed."));
			return false;
		}

		m_desc = desc;
		return true;
	}

	void CShaderBufferHLSL::Update(const void *data)
	{
		g_pD3ddev->UpdateSubresource(m_pBuffer, 0, NULL, data, m_desc.BufferSize, 0);
	}

	bool CShaderBufferHLSL::Map(void **data, bool discard)
	{
		D3D10_MAP maptype;
		if(discard)
		{
			if(m_desc.CPUAccess == P3D_CPUACCESS_WRITE) maptype = D3D10_MAP_WRITE_DISCARD;
			else CON(MSG_ERR, _W("VertexBuffer: CPU access is not set P3D_CPUACCESS_WRITE, discard impossible."));
		}
		else
		{
			if(m_desc.CPUAccess == P3D_CPUACCESS_READ) maptype = D3D10_MAP_READ;
			else if(m_desc.CPUAccess == P3D_CPUACCESS_READ_WRITE) maptype = D3D10_MAP_READ_WRITE;
			else if(m_desc.CPUAccess == P3D_CPUACCESS_WRITE) maptype = D3D10_MAP_WRITE;
		}

		return CheckHRResult(m_pBuffer->Map(maptype, 0, data));
	}

	void CShaderBufferHLSL::Unmap()
	{
		m_pBuffer->Unmap();
	}

};

#endif
