/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#if defined(_DX) && _DX == 10

#include "IndexBufferWrapper.h"
#include "symbols.h"
#include "../Renderer.h"

namespace P3D
{

	extern ID3D10Device* g_pD3ddev;

	CIndexBufferWrapper::CIndexBufferWrapper()
	{
		m_pIB = 0;
	}

	CIndexBufferWrapper::~CIndexBufferWrapper()
	{
		SAFE_RELEASE(m_pIB);
	}

	bool CIndexBufferWrapper::Create(sIBDesc &desc, void *data)
	{
		unsigned int indsize = 0;
		if(desc.IndexFormat = IBFORMAT_32) 
		{
			indsize = sizeof(DWORD);
		}
		else 
		{
			indsize = sizeof(WORD);
		}

		D3D10_BUFFER_DESC ibd;

		if(desc.ResourceUsage == P3D_RESOURCEUSAGE_IMMUTABLE) ibd.Usage = D3D10_USAGE_IMMUTABLE;
		else if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DEFAULT) ibd.Usage = D3D10_USAGE_DEFAULT;
		else if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DYNAMIC)
		{
			if(desc.CPUAccess != P3D_CPUACCESS_READ) ibd.Usage = D3D10_USAGE_DYNAMIC;
			else ibd.Usage = D3D10_USAGE_STAGING;
		}
		else
		{
			CON(MSG_ERR, _W("Index buffer creation failed. No apropriate ResourceUsage given."));
			return false;
		}

		ibd.ByteWidth = indsize * desc.IndexCount;
		ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
		ibd.MiscFlags = 0;
		
		if(desc.CPUAccess == P3D_CPUACCESS_NONE) ibd.CPUAccessFlags = 0;
		else if(desc.CPUAccess == P3D_CPUACCESS_READ) ibd.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
		else if(desc.CPUAccess == P3D_CPUACCESS_WRITE) ibd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		else if(desc.CPUAccess == P3D_CPUACCESS_READ_WRITE) ibd.CPUAccessFlags = D3D10_CPU_ACCESS_READ || D3D10_CPU_ACCESS_WRITE;
		else
		{
			CON(MSG_ERR, _W("Index buffer creation failed. No apropriate CPUAccess given."));
			return false;
		} 
		
		D3D10_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = data;
		if(FAILED(g_pD3ddev->CreateBuffer(&ibd,  data ? &iinitData : NULL, &m_pIB)))
		{
			CON(MSG_ERR, _W("Index buffer creation failed."));
			return false;
		}

		m_desc = desc;

		return true;
	}

	void CIndexBufferWrapper::Use()
	{
		g_pD3ddev->IASetIndexBuffer(m_pIB, m_desc.IndexFormat == IBFORMAT_32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
	}

	bool CIndexBufferWrapper::Map(void **data, bool discard, bool doNotWait)
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
		
		if(FAILED(m_pIB->Map(maptype, doNotWait ? D3D10_MAP_FLAG_DO_NOT_WAIT : 0, data))) return false;
		return true;
	}

	void CIndexBufferWrapper::Unmap()
	{
		m_pIB->Unmap();
	}

	bool CIndexBufferWrapper::OnLostDevice()
	{
		//no need to do anything
		return false;
	}

	bool CIndexBufferWrapper::OnResetDevice()
	{
		//no need to do anything
		return false;
	}

};

#endif
