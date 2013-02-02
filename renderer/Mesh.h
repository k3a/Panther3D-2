/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "IMesh.h"
#include "Texture.h"
#include <vector>

namespace P3D
{

	struct sAttribRange
	{
		unsigned int FaceStart;
		unsigned int FaceCount;
	};

	struct sMeshDesc
	{
		sMeshDesc() {};
		sVBDesc VBDesc;
		sIBDesc IBDesc;
	};
	
	class CMesh : public IMesh
	{
	public:
		CMesh();
		~CMesh();
		bool Create(sMeshDesc &desc, void *VBData, void *IBData);
		
		inline CVertexBuffer* GetVertexBuffer() { return m_pVB; };
		inline CIndexBuffer* GetIndexBuffer() { return m_pIB; };
		
		void LockVertexBuffer(void **data, bool discard);
		void UnLockVertexBuffer();
		void LockIndexBuffer(void **data, bool discard);
		void UnLockIndexBuffer();

		inline unsigned int GetNumFaces();
		inline unsigned int GetNumVertices();
		void Optimize();
		inline unsigned int GetNumSubsets();
		inline AABB GetAABB();
		inline Sphere GetBoundingSphere();
		void SetTexture(unsigned int subset, CTexture *texture);
		ITexture* GetTexture(unsigned int subset);
		void AddSubset(sAttribRange attr);
		void SetFilename(const wchar *filename) { wstrcpy(m_filename, filename); };
		const wchar* GetFilename() { return m_filename; };

		void RenderSubset(unsigned int subset);

		bool OnLostDevice();
		bool OnResetDevice();

	private:
		void RebuildBBBS();
		std::vector<VertexElement> m_vVertDeclration;
		std::vector<unsigned short> m_vVertElementSize;
		std::vector<CTexture*> m_vTextures;
		unsigned int m_vertSize;
		CVertexBuffer *m_pVB;
		CIndexBuffer *m_pIB;
		unsigned int m_numFaces;
		unsigned int m_numVertices;
		unsigned int m_numSubsets;
		AABB m_AABB;
		Sphere m_sphere;
		std::vector<sAttribRange> m_AttInfo;
		BYTE **m_vbDataPointer;
		wchar m_filename[50];

		BYTE * m_VBBackup;
		BYTE * m_IBBackup;
	};

};