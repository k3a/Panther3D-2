/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#if defined(_DX) && _DX == 9

#include "IndexBufferWrapper.h"
#include "symbols.h"
#include "../Renderer.h"

namespace P3D
{

	extern LPDIRECT3DDEVICE9 g_pD3ddev;

	CIndexBufferWrapper::CIndexBufferWrapper()
	{
		m_pIB = 0;
		m_usage = 0;
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

		DWORD usage = 0;
		//D3DPOOL pool = D3DPOOL_MANAGED;
		D3DPOOL pool = D3DPOOL_MANAGED;

		if(desc.CPUAccess == P3D_CPUACCESS_NONE || desc.CPUAccess == P3D_CPUACCESS_WRITE) usage |= D3DUSAGE_WRITEONLY;
		if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DYNAMIC)
		{
			usage |= D3DUSAGE_DYNAMIC;
			pool = D3DPOOL_DEFAULT;
		}
		
		m_IBPool = pool;
		m_usage = usage;
		m_desc = desc;

		if(FAILED(g_pD3ddev->CreateIndexBuffer(desc.IndexCount * indsize, m_usage, desc.IndexFormat == IBFORMAT_32 ? D3DFMT_INDEX32 : D3DFMT_INDEX16, pool, &m_pIB, 0))) return false;

		if(data)
		{
			void *ibdata;
			if(FAILED(m_pIB->Lock(0, 0, &ibdata, usage == D3DUSAGE_DYNAMIC ? D3DLOCK_DISCARD : 0))) return false;
			memcpy(ibdata, data, desc.IndexCount * indsize);
			m_pIB->Unlock();
		}
		
		return true;
	}

	void CIndexBufferWrapper::Use()
	{
		g_pD3ddev->SetIndices(m_pIB);
	}

	bool CIndexBufferWrapper::Map(void **data, bool discard, unsigned int SizeToLock)
	{
		DWORD flags = 0;
		if(m_usage == D3DUSAGE_DYNAMIC)
		{
			if(discard) flags |= D3DLOCK_DISCARD;
		}
		if(m_desc.CPUAccess = P3D_CPUACCESS_READ) flags |= D3DLOCK_READONLY;
		if(FAILED(m_pIB->Lock (0, SizeToLock, data, flags))) return false;
		return true;
	}

	void CIndexBufferWrapper::Unmap()
	{
		m_pIB->Unlock();
	}

	bool CIndexBufferWrapper::OnLostDevice()
	{
		if(m_IBPool == D3DPOOL_DEFAULT) 
		{
			SAFE_RELEASE(m_pIB);
			return true;
		}

		return false;
	}

	bool CIndexBufferWrapper::OnResetDevice()
	{
		if(m_IBPool == D3DPOOL_DEFAULT) 
		{
			unsigned int indsize;
			if(m_desc.IndexFormat = IBFORMAT_32) indsize = sizeof(DWORD);
			else indsize = sizeof(WORD);

			if(FAILED(g_pD3ddev->CreateIndexBuffer(m_desc.IndexCount * indsize, m_usage, m_desc.IndexFormat == IBFORMAT_32 ? D3DFMT_INDEX32 : D3DFMT_INDEX16, m_IBPool, &m_pIB, 0))) return false;
			return true;
		}
		return false;
	}

};

#endif
