/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#if defined(_DX) && _DX == 10

#include "VertexBufferWrapper.h"
#include "symbols.h"
#include "../Renderer.h"

namespace P3D
{

	extern ID3D10Device* g_pD3ddev;

	
	CVertexBufferWrapper::CVertexBufferWrapper()
	{
		m_pVB = 0;
	}

	CVertexBufferWrapper::~CVertexBufferWrapper()
	{
		SAFE_RELEASE(m_pVB);
	}

	bool CVertexBufferWrapper::Create(sVBDesc &desc, void *data)
	{
		if( !desc.IsValid() ) return false;

		D3D10_BUFFER_DESC vbd;

		if(desc.ResourceUsage == P3D_RESOURCEUSAGE_IMMUTABLE) vbd.Usage = D3D10_USAGE_IMMUTABLE;
		else if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DEFAULT) vbd.Usage = D3D10_USAGE_DEFAULT;
		else if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DYNAMIC)
		{
			if(desc.CPUAccess != P3D_CPUACCESS_READ) vbd.Usage = D3D10_USAGE_DYNAMIC;
			else vbd.Usage = D3D10_USAGE_STAGING;
		}
		else
		{
			CON(MSG_ERR, _W("Vertex buffer creation failed. No apropriate ResourceUsage given."));
			return false;
		}

		vbd.ByteWidth = desc.VertexSize * desc.VertexCount;
		vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;	//TODO: stream-output
		vbd.MiscFlags = 0;

		if(desc.CPUAccess == P3D_CPUACCESS_NONE) vbd.CPUAccessFlags = 0;
		else if(desc.CPUAccess == P3D_CPUACCESS_READ) vbd.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
		else if(desc.CPUAccess == P3D_CPUACCESS_WRITE) vbd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		else if(desc.CPUAccess == P3D_CPUACCESS_READ_WRITE) vbd.CPUAccessFlags = D3D10_CPU_ACCESS_READ || D3D10_CPU_ACCESS_WRITE;
		else
		{
			CON(MSG_ERR, _W("Vertex buffer creation failed. No apropriate CPUAccess given."));
			return false;
		} 

		D3D10_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;
		if(!CheckHRResult(g_pD3ddev->CreateBuffer(&vbd, data ? &initData : NULL, &m_pVB)))
		{
			CON(MSG_ERR, _W("Vertex buffer creation failed."));
			return false;
		}

		m_desc = desc;

		return true;
	}

	bool CVertexBufferWrapper::Map(void **data, bool discard, bool doNotWait)
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

		if(FAILED(m_pVB->Map(maptype, doNotWait ? D3D10_MAP_FLAG_DO_NOT_WAIT : 0, data))) return false;
		return true;
	}

	void CVertexBufferWrapper::Unmap()
	{
		m_pVB->Unmap();
	}

	void CVertexBufferWrapper::Use()
	{
		//TODO: Multiple streams
		//UINT offset = 0;
		UINT strides[1] = { m_desc.VertexSize };
		UINT offsets[1] = { 0 };
		g_pD3ddev->IASetVertexBuffers(0, 1, &m_pVB, strides, offsets);
	}

	bool CVertexBufferWrapper::Update(void *data)
	{
		g_pD3ddev->UpdateSubresource(m_pVB, 0, NULL, data, m_desc.VertexSize * m_desc.VertexCount, 0);
		return true;
	}

	void CVertexBufferWrapper::Render(P3D::PrimitiveType primType, int start, int num)
	{
		D3D10_PRIMITIVE_TOPOLOGY primtype = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		switch(primType)
		{
		case P3DPT_POINTLIST:
			primtype = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST ;
			break;
		case P3DPT_LINELIST:
			primtype = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		case P3DPT_LINESTRIP:
			primtype = D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
			break;
		case P3DPT_TRIANGLELIST:
			primtype = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			num *= 3;
			break;
		case P3DPT_TRIANGLESTRIP:
			primtype = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ;
			break;
		//TODO: that adjency stuff if needed
		};
		g_pD3ddev->IASetPrimitiveTopology(primtype);
		g_pD3ddev->Draw(num, start);
	}


	void CVertexBufferWrapper::OnLostDevice()
	{
		//nothing
	}

	void CVertexBufferWrapper::OnResetDevice()
	{
		//nothing
	}

	void CVertexBufferWrapper::RenderIndexed(PrimitiveType primType, UINT startIndex, UINT num)
	{
		D3D10_PRIMITIVE_TOPOLOGY primtype = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		switch(primType)
		{
		case P3DPT_POINTLIST:
			primtype = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST ;
			break;
		case P3DPT_LINELIST:
			primtype = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		case P3DPT_LINESTRIP:
			primtype = D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
			break;
		case P3DPT_TRIANGLELIST:
			primtype = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			num *= 3;
			break;
		case P3DPT_TRIANGLESTRIP:
			primtype = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ;
			break;
			//TODO: that adjency stuff if needed
		};
		g_pD3ddev->IASetPrimitiveTopology(primtype);
		g_pD3ddev->DrawIndexed(num, startIndex, 0);
	}
	
	////////////////////////////////////////////////////

	CVertexDeclarationWrapper::CVertexDeclarationWrapper(const P3D::VertexElement *vearray, void *buffer)
	{
		m_pInputLayout = NULL;
		m_valid = false;

		// We find out how many Elements has our Vertex Declaration
		UINT numElements = 0;
		while(vearray[numElements].vertexType != P3DVD_NONE)
		{
			numElements++;
		}
		if(numElements == 0) return;
		
		// Than we create the Vertex Declaration
		D3D10_INPUT_ELEMENT_DESC* ieDecl = new D3D10_INPUT_ELEMENT_DESC[numElements];
		//D3D10_INPUT_ELEMENT_DESC ieDecl[10];

		WORD offset = 0;
		UINT numVU = 15;	//There are 15 values in P3DVertexUsage enum
		BYTE* pusageIndex = new BYTE[numVU];
		for(UINT i=0; i<numVU; i++) pusageIndex[i] = 0;

		for(UINT i=0; i<numElements; i++)
		{
			// TODO: make this adjustable
			ieDecl[i].InputSlot = 0;
			ieDecl[i].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
			ieDecl[i].InstanceDataStepRate = 0;

			switch(vearray[i].vertexType)
			{
			case P3DVD_FLOAT1:
				ieDecl[i].Format = DXGI_FORMAT_R32_FLOAT;
				ieDecl[i].AlignedByteOffset = offset;
				offset += 4;
				break;
			case P3DVD_FLOAT2:
				ieDecl[i].Format = DXGI_FORMAT_R32G32_FLOAT;
				ieDecl[i].AlignedByteOffset = offset;
				offset += 8;
				break;
			case P3DVD_FLOAT3:
				ieDecl[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
				ieDecl[i].AlignedByteOffset = offset;
				offset += 12;
				break;
			case P3DVD_FLOAT4:
				ieDecl[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				ieDecl[i].AlignedByteOffset = offset;
				offset += 16;
				break;
			}

			switch(vearray[i].vertexUsage)
			{
			case P3DVU_POSITION:
				ieDecl[i].SemanticName = "POSITION";
				ieDecl[i].SemanticIndex = pusageIndex[P3DVU_POSITION];
				pusageIndex[P3DVU_POSITION]++;
				break;
			case P3DVU_NORMAL:
				ieDecl[i].SemanticName = "NORMAL";
				ieDecl[i].SemanticIndex = pusageIndex[P3DVU_NORMAL];
				pusageIndex[P3DVU_NORMAL]++;
				break;
			case P3DVU_TEXCOORD:
				ieDecl[i].SemanticName = "TEXCOORD";
				ieDecl[i].SemanticIndex = pusageIndex[P3DVU_TEXCOORD];
				pusageIndex[P3DVU_TEXCOORD]++;
				break;
			case P3DVU_COLOR:
				ieDecl[i].SemanticName = "COLOR";
				ieDecl[i].SemanticIndex = pusageIndex[P3DVU_COLOR];
				pusageIndex[P3DVU_COLOR]++;
				break;
			}

		}

		
		HRESULT hr;
		ID3D10Blob *vs = (ID3D10Blob*)buffer;
		hr = g_pD3ddev->CreateInputLayout(ieDecl, numElements, vs->GetBufferPointer(), vs->GetBufferSize(), &m_pInputLayout);
		delete[] ieDecl;
		delete[] pusageIndex;
		if(FAILED(hr)) return;
		m_valid = true;
	}

	CVertexDeclarationWrapper::~CVertexDeclarationWrapper()
	{
		SAFE_RELEASE(m_pInputLayout);
	}

	void CVertexDeclarationWrapper::Use()
	{
		g_pD3ddev->IASetInputLayout(m_pInputLayout);
	}

}

#endif
