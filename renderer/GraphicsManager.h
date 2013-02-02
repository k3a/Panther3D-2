/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once
#include "IGraphicsManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "types.h"
#include <vector>
#include "VertexTypes.h"

namespace P3D
{


class CGraphicsManager : public IGraphicsManager
{
public:
	CGraphicsManager();
	~CGraphicsManager();

	void Update();
	bool Create();

	///Creates new texture by given data.
	ITexture* CreateTexture(sTextureDesc &desc, BYTE *data, unsigned long len);
	
	///Loads texture from file.
	ITexture* LoadTexture(const wchar* filename);
	
	///Deletes texture. \note Texture is physically deleted only when there are no remaining references to it; 
	void DeleteTexture(ITexture *texture);

	ID CreateVertexBuffer(const sVBDesc &desc, const void *data);
	void DestroyVertexBuffer(ID VBId);

	ID CreateIndexBuffer(const sIBDesc &desc, const void *data);
	void DestroyIndexBuffer(ID IBId);

	ITexture* GetDafaultTexture();

	IMesh* LoadMesh(const wchar *filename);
	void DeleteMesh(CMesh *mesh);
	
	void OnLostDevice();
	void OnResetDevice();

	void RegisterVertexDesc(const VertexElement vearray[], Str &name);
	UINT GetVertexDescIDByName(Str &name);
	const inline VertexElement* GetVertexDescByID(UINT id) { return m_vddescs[id]->vearray; };
	inline UINT GetVertexDescCount() { return m_vddescs.Size(); };

	// internal methods:
	//CMesh* GetMesh(DWORD meshID);

	//Temporary hack 
	inline void SetActiveVS(IShader *shader) { m_pActiveVS = (CShader*)shader; };
	inline IShader* GetActiveVS() { return m_pActiveVS; };
	inline void SetViewProjMatrix(const Matrix &m) { m_vpmatrix = m; };
	inline Matrix& GetViewProjMatrix() { return m_vpmatrix; };

	// --- caching mgmt
	bool CacheMeshData(MeshData& md);
	
	CVertexBuffer* GetCachedVertexBuffer(unsigned int id);
	CIndexBuffer* GetCachedIndexBuffer(unsigned int id);
private:

	//content streaming
	//CTexture *m_textures[P3D_MAX_TEXTURES];

	void CheckForFreeResources();
	// misto pro vnitrni seznamy zdroju vcetne poctu referenci atd...
	// klidne mozno pouzit veci jako vector<CMesh*> m_meshes; atd...
	std::vector<CTexture*> m_textures;
	std::vector<CMesh*> m_meshes;
	CTexture* m_pDefaultTexture;

	struct sVertexDeclarationDesc
	{
		~sVertexDeclarationDesc() {	SAFE_DELETE_ARRAY(vearray);	}
		Str name;
		VertexElement *vearray;
	};
	tArray<sVertexDeclarationDesc*> m_vddescs;

	CShader *m_pActiveVS; //temporary hack
	Matrix m_vpmatrix;

	// --- caching
	tArray<CVertexBuffer*> m_cacheVB;
	tArray<CIndexBuffer*> m_cacheIB;
};

};
