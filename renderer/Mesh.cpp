/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "Mesh.h"
#include "Renderer.h"

namespace P3D
{

	CMesh::CMesh()
	{
		m_pVB = 0;
		m_pIB = 0;
		m_numFaces = 0;
		m_numVertices = 0;
		m_vertSize = 0;
		m_vbDataPointer = 0;

		m_VBBackup = 0;
		m_IBBackup = 0;
	}

	CMesh::~CMesh()
	{
		SAFE_DELETE(m_pVB);
		SAFE_DELETE(m_pIB);
		SAFE_DELETE_ARRAY(m_VBBackup);
		SAFE_DELETE_ARRAY(m_IBBackup);
	}

	

	bool CMesh::Create(P3D::sMeshDesc &desc, void *VBData, void *IBData)
	{
		m_pVB = new CVertexBuffer;
		m_pIB = new CIndexBuffer;
		m_numSubsets = 0;

		if(!m_pVB->Create(desc.VBDesc), VBData) return false;
		if(!m_pIB->Create(desc.IBDesc), IBData) return false;

		return true;
	}


	inline Sphere CMesh::GetBoundingSphere()
	{
		return m_sphere;
	}

	inline unsigned int CMesh::GetNumSubsets()
	{
		return m_numSubsets;
	}

	inline unsigned int CMesh::GetNumVertices()
	{
		return m_pVB->GetDesc().VertexCount;
	}

	inline unsigned int CMesh::GetNumFaces()
	{
		return m_pIB->GetDesc().IndexCount / 3;
	}

	void CMesh::LockVertexBuffer(void **data, bool discard)
	{
		m_pVB->Map(data, discard);
		m_vbDataPointer = (BYTE**)data;
	}

	void CMesh::UnLockVertexBuffer()
	{
		//TODO: compute bounding box and sphere
		m_pVB->Unmap();
		RebuildBBBS();
	}

	void CMesh::LockIndexBuffer(void **data, bool discard)
	{
		m_pIB->Map(data, discard);
	}

	void CMesh::UnLockIndexBuffer()
	{
		m_pIB->Unmap();
	}

	void CMesh::RenderSubset(unsigned int subset)
	{
		PROFILESAMPLE(MeshSubsetRendering);

		if(subset >= m_numSubsets) return;
		m_pVB->Use();
		m_pIB->Use();
		CShader* avs = (CShader*)CRenderer::cGraphicsManager()->GetActiveVS();
		//avs->SetVertexDeclaration(m_pVB->GetVertexDescID());
		//CON(MSG_INFO, _W("mesh begin"));
		m_pVB->RenderIndexed(P3DPT_TRIANGLELIST, m_AttInfo[subset].FaceStart*3, m_AttInfo[subset].FaceCount);
		//CON(MSG_INFO, _W("mesh end"));
	}

	bool CMesh::OnLostDevice()
	{
		#if defined(_OGL)
		m_pVB->OnLostDevice();
		m_pIB->OnLostDevice();
		#endif
		return false;
	}

	bool CMesh::OnResetDevice()
	{
		#if defined(_OGL)
		m_pVB->OnResetDevice();
		m_pIB->OnResetDevice();

		void *data;
		m_pVB->Map(&data, true);
		memcpy(data, m_VBBackup, m_numVertices * m_vertSize);
		m_pVB->Unmap();

		void *ibdata;
		m_pIB->Map(&ibdata, true);
		memcpy(ibdata, m_IBBackup, m_numFaces * 3 * sizeof(DWORD));
		m_pIB->Unmap();
		#endif
		return false;
	}

	

	void CMesh::RebuildBBBS()
	{
		Vec3 bbmin(0.0f, 0.0f, 0.0f);
		Vec3 bbmax(0.0f, 0.0f, 0.0f);
		Vec3 bscenter(0.0f, 0.0f, 0.0f);
		float bsradius = 0.0f;
		bool firsttime = true;

		unsigned int offset = 0; //Positions is just supposed to be the first element
// 		for(unsigned int i=0; i<m_vVertDeclration.size(); i++)
// 		{
// 			if(m_vVertDeclration[i].vertexUsage == P3DVU_POSITION) break;
// 			offset += m_vVertElementSize[i];
// 		}

		BYTE* vbdata;
		m_pVB->Map((void**)(&vbdata));

		unsigned int cursor = offset;
		for(unsigned int i=0; i<m_pVB->GetDesc().VertexCount; i++)
		{
			Vec3 v;
			v = *(Vec3*)&vbdata[cursor]; //both works
			//memcpy(&v, (void*)&vbdata[cursor], sizeof(Vec3));
			//cursor += m_vertSize;
			cursor += m_pVB->GetDesc().VertexSize;

			if(firsttime)
			{
				bbmin = bbmax = v;
				firsttime = false;
			}
			else
			{
				if(v.x > bbmax.x) bbmax.x = v.x;
				else if(v.x < bbmin.x) bbmin.x = v.x;

				if(v.y > bbmax.y) bbmax.y = v.y;
				else if(v.y < bbmin.y) bbmin.y = v.y;

				if(v.z > bbmax.z) bbmax.z = v.z;
				else if(v.z < bbmin.z) bbmin.z = v.z;
			}
		}
 		m_pVB->Unmap();

		bscenter = (bbmin + bbmax)*0.5f;
		Vec3 bsvec = bbmax - bbmin;
		bsradius = bsvec.Length()/2;

		m_AABB = AABB(bbmin, bbmax);
		m_sphere = Sphere(bscenter, bsradius);
	}

	void CMesh::SetTexture(unsigned int subset, P3D::CTexture *texture)
	{
		assertd(subset>=0 && subset<m_numSubsets, "Mesh::SetTexture(subset, texture): subset out of range!");
		m_vTextures[subset]->EraseReference();
		m_vTextures[subset] = texture;
		m_vTextures[subset]->AddReference();
	}

	void CMesh::AddSubset(P3D::sAttribRange attr)
	{
		m_AttInfo.push_back(attr);
		m_vTextures.push_back((CTexture*)CRenderer::cGraphicsManager()->GetDafaultTexture());
		m_numSubsets++;
	}

	ITexture* CMesh::GetTexture(unsigned int subset)
	{
		if(subset >= m_numSubsets) return CRenderer::cGraphicsManager()->GetDafaultTexture();
		else return m_vTextures[subset];
	}

	inline AABB CMesh::GetAABB()
	{
		return m_AABB;
	}


};