/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#if defined(_DX) && _DX == 9

#include "VertexBufferWrapper.h"
#include "symbols.h"
#include "../Renderer.h"

namespace P3D
{

	extern LPDIRECT3DDEVICE9 g_pD3ddev;

	CVertexBufferWrapper::CVertexBufferWrapper()
	{
		m_pVB = 0;
		m_pVertexDeclaration = 0;
		m_VBPool = D3DPOOL_MANAGED;
		m_usage = 0;
	}

	CVertexBufferWrapper::~CVertexBufferWrapper()
	{
		SAFE_RELEASE(m_pVertexDeclaration);
		SAFE_RELEASE(m_pVB);
	}

	bool CVertexBufferWrapper::Create(sVBDesc &Desc, void *Data)
	{
		if( !Desc.IsValid() ) return false;

		DWORD usage = 0;
		D3DPOOL pool = D3DPOOL_MANAGED;

		if(Desc.CPUAccess == P3D_CPUACCESS_NONE || Desc.CPUAccess == P3D_CPUACCESS_WRITE) usage |= D3DUSAGE_WRITEONLY;
		if(Desc.ResourceUsage == P3D_RESOURCEUSAGE_DYNAMIC)
		{
			usage |= D3DUSAGE_DYNAMIC;
			pool = D3DPOOL_DEFAULT;
		}
		if(Desc.PointParticles) usage |= D3DUSAGE_POINTS;

		m_VBPool = pool;
		m_usage = usage;

		if(FAILED(g_pD3ddev->CreateVertexBuffer(Desc.VertexCount * Desc.VertexSize, usage, 0, pool, &m_pVB, 0))) return false;

		if(Data)
		{
			void *vbdata;
			if(FAILED(m_pVB->Lock(0, 0, &vbdata, usage == D3DUSAGE_DYNAMIC ? D3DLOCK_DISCARD : 0))) return false;
			memcpy(vbdata, Data, Desc.VertexCount * Desc.VertexSize);
			m_pVB->Unlock();
		}

		m_desc = Desc;

		return true;
	}

	bool CVertexBufferWrapper::Map(void **Data, bool Discard, bool DoNotWait)
	{
		DWORD flags = 0;
		if(m_desc.ResourceUsage == P3D_RESOURCEUSAGE_DYNAMIC)
		{
			if(Discard) flags |= D3DLOCK_DISCARD;
		}
		if(FAILED(m_pVB->Lock (0, 0, Data, flags))) return false;
		return true;
	}

	void CVertexBufferWrapper::Unmap()
	{
		m_pVB->Unlock();
	}

	void CVertexBufferWrapper::Use()
	{
		//TODO: Multiple streams
		g_pD3ddev->SetStreamSource( 0, m_pVB, 0, m_desc.VertexSize );
	}

	bool CVertexBufferWrapper::Update(void *Data)
	{
		if(Data)
		{
			void *vbdata;
			if(FAILED(m_pVB->Lock(0, 0, &vbdata, m_usage == D3DUSAGE_DYNAMIC ? D3DLOCK_DISCARD : 0))) return false;
			memcpy(vbdata, Data, m_desc.VertexCount * m_desc.VertexSize);
			m_pVB->Unlock();
		}
		return true;
	}

	void CVertexBufferWrapper::Render(P3D::PrimitiveType primType, int start, int num)
	{
		D3DPRIMITIVETYPE primtype = D3DPT_TRIANGLELIST;
		switch(primType)
		{
		case P3DPT_POINTLIST:
			primtype = D3DPT_POINTLIST;
			break;
		case P3DPT_LINELIST:
			primtype = D3DPT_LINELIST;
			break;
		case P3DPT_LINESTRIP:
			primtype = D3DPT_LINESTRIP;
			break;
		case P3DPT_TRIANGLELIST:
			primtype = D3DPT_TRIANGLELIST;
			break;
		case P3DPT_TRIANGLESTRIP:
			primtype = D3DPT_TRIANGLESTRIP;
			break;
		case P3DPT_TRIANGLEFAN:
			primtype = D3DPT_TRIANGLEFAN;
			break;

		};
		g_pD3ddev->DrawPrimitive( primtype, start, num );
	}


	void CVertexBufferWrapper::OnLostDevice()
	{
		if(m_VBPool == D3DPOOL_DEFAULT) SAFE_RELEASE(m_pVB);
	}

	void CVertexBufferWrapper::OnResetDevice()
	{
		if(m_VBPool == D3DPOOL_DEFAULT) g_pD3ddev->CreateVertexBuffer(m_desc.VertexCount * m_desc.VertexSize, m_usage, 0, m_VBPool, &m_pVB, 0);
	}

	void CVertexBufferWrapper::RenderIndexed(PrimitiveType primType, int startIndex, int num)
	{
		PROFILESAMPLE(D3D9VBRenderIndexed);

		D3DPRIMITIVETYPE primtype = D3DPT_TRIANGLELIST;
		//unsigned int numVertices = 0;
		switch(primType)
		{
		case P3DPT_POINTLIST:
			primtype = D3DPT_POINTLIST;
			break;
		case P3DPT_LINELIST:
			primtype = D3DPT_LINELIST;
			break;
		case P3DPT_LINESTRIP:
			primtype = D3DPT_LINESTRIP;
			break;
		case P3DPT_TRIANGLELIST:
			primtype = D3DPT_TRIANGLELIST;
			break;
		case P3DPT_TRIANGLESTRIP:
			primtype = D3DPT_TRIANGLESTRIP;
			break;
		case P3DPT_TRIANGLEFAN:
			primtype = D3DPT_TRIANGLEFAN;
			break;

		};
		g_pD3ddev->DrawIndexedPrimitive( primtype, 0, 0, m_desc.VertexCount, startIndex, num);
	}

	///////////////////////////////////////////////////////////

	CVertexDeclarationWrapper::CVertexDeclarationWrapper(const P3D::VertexElement *vearray, void *buffer)
	{
		m_pVertexDeclaration = NULL;
		m_valid = false;
		
		// We find out how many Elements has our Vertex Declaration
		UINT numElements = 0;
		while(vearray[numElements].vertexType != P3DVD_NONE)
		{
			numElements++;
		}
		if(numElements == 0) return;
		//g_pConsole->Message(MSG_CON_INFO, ")
		//CON(MSG_CON_INFO, "numElements je %d", numElements);

		// Than we create the Vertex Declaration
		D3DVERTEXELEMENT9* dwDecl = new D3DVERTEXELEMENT9[numElements+1];
		
		WORD offset = 0;
		UINT numVU = 15;	//There are 15 values in P3DVertexUsage enum
		BYTE* pusageIndex = new BYTE[numVU];
		for(UINT i=0; i<numVU; i++) pusageIndex[i] = 0;

		for(UINT i=0; i<numElements; i++)
		{
			// Generating Vertex Declaration how D3D wants it
			dwDecl[i].Stream = 0;
			dwDecl[i].Method = D3DDECLMETHOD_DEFAULT;

			switch(vearray[i].vertexType)
			{
			case P3DVD_NONE:
				{
					dwDecl[i].Type = D3DDECLTYPE_UNUSED;
					dwDecl[i].Offset = offset;
					offset += 0;
					break;
				}

			case P3DVD_FLOAT1:
				{
					dwDecl[i].Type = D3DDECLTYPE_FLOAT1;
					dwDecl[i].Offset = offset;
					offset += 4;
					break;
				}

			case P3DVD_FLOAT2:
				{
					dwDecl[i].Type = D3DDECLTYPE_FLOAT2;
					dwDecl[i].Offset = offset;
					offset += 8;
					break;
				}

			case P3DVD_FLOAT3:
				{
					dwDecl[i].Type = D3DDECLTYPE_FLOAT3;
					dwDecl[i].Offset = offset;
					offset += 12;
					break;
				}

			case P3DVD_FLOAT4:
				{
					dwDecl[i].Type = D3DDECLTYPE_FLOAT4;
					dwDecl[i].Offset = offset;
					offset += 16;
					break;
				}
			}

			switch(vearray[i].vertexUsage)
			{
			case P3DVU_POSITION:
				dwDecl[i].Usage = D3DDECLUSAGE_POSITION;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_POSITION];
				pusageIndex[P3DVU_POSITION]++;
				break;

			case P3DVU_BLENDWEIGHT:
				dwDecl[i].Usage = D3DDECLUSAGE_BLENDWEIGHT;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_BLENDWEIGHT];
				pusageIndex[P3DVU_BLENDWEIGHT]++;
				break;

			case P3DVU_BLENDINDICES:
				dwDecl[i].Usage = D3DDECLUSAGE_BLENDINDICES;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_BLENDINDICES];
				pusageIndex[P3DVU_BLENDINDICES]++;
				break;

			case P3DVU_NORMAL:
				dwDecl[i].Usage = D3DDECLUSAGE_NORMAL;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_NORMAL];
				pusageIndex[P3DVU_NORMAL]++;
				break;

			case P3DVU_PSIZE:
				dwDecl[i].Usage = D3DDECLUSAGE_PSIZE;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_PSIZE];
				pusageIndex[P3DVU_PSIZE]++;
				break;

			case P3DVU_TEXCOORD:
				dwDecl[i].Usage = D3DDECLUSAGE_TEXCOORD;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_TEXCOORD];
				pusageIndex[P3DVU_TEXCOORD]++;
				break;

			case P3DVU_TANGENT:
				dwDecl[i].Usage = D3DDECLUSAGE_TANGENT;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_TANGENT];
				pusageIndex[P3DVU_TANGENT]++;
				break;

			case P3DVU_BINORMAL:
				dwDecl[i].Usage = D3DDECLUSAGE_BINORMAL;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_BINORMAL];
				pusageIndex[P3DVU_BINORMAL]++;
				break;

			case P3DVU_TESSFACTOR:
				dwDecl[i].Usage = D3DDECLUSAGE_TESSFACTOR;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_TESSFACTOR];
				pusageIndex[P3DVU_TESSFACTOR]++;
				break;

			case P3DVU_POSITIONT:
				dwDecl[i].Usage = D3DDECLUSAGE_POSITIONT;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_POSITIONT];
				pusageIndex[P3DVU_POSITIONT]++;
				break;

			case P3DVU_COLOR:
				dwDecl[i].Usage = D3DDECLUSAGE_COLOR;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_COLOR];
				pusageIndex[P3DVU_COLOR]++;
				break;

			case P3DVU_FOG:
				dwDecl[i].Usage = D3DDECLUSAGE_FOG;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_FOG];
				pusageIndex[P3DVU_FOG]++;
				break;

			case P3DVU_DEPTH:
				dwDecl[i].Usage = D3DDECLUSAGE_DEPTH;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_DEPTH];
				pusageIndex[P3DVU_DEPTH]++;
				break;

			case P3DVU_SAMPLE:
				dwDecl[i].Usage = D3DDECLUSAGE_SAMPLE;
				dwDecl[i].UsageIndex = pusageIndex[P3DVU_SAMPLE];
				pusageIndex[P3DVU_SAMPLE]++;
				break;
			}

		}

		//D3DDECL_END() Last member of an array
		dwDecl[numElements].Stream = 0xFF;
		dwDecl[numElements].Offset = 0;
		dwDecl[numElements].Type = D3DDECLTYPE_UNUSED;
		dwDecl[numElements].Method = 0;
		dwDecl[numElements].Usage = 0;
		dwDecl[numElements].UsageIndex = 0;

		HRESULT hr;
		//		if(g_pD3ddev==0) MessageBox(NULL, "g_pD3ddev is null", "IMPORTANT MESSAGE!", MB_OK);
		hr = g_pD3ddev->CreateVertexDeclaration(dwDecl, &m_pVertexDeclaration);
		delete[] dwDecl;
		delete[] pusageIndex;
		if(FAILED(hr)) return;
		m_valid = true;
	}

	CVertexDeclarationWrapper::~CVertexDeclarationWrapper()
	{
		SAFE_RELEASE(m_pVertexDeclaration);
	}

	void CVertexDeclarationWrapper::Use()
	{
		g_pD3ddev->SetVertexDeclaration(m_pVertexDeclaration);
	}

}

#endif
