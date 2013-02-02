/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "GraphicsManager.h"
#include "Renderer.h"
#include "IMeshLoader.h"
#include "TextureLoader.h"

#include <fstream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace P3D
{

	//------------------------------------------
	CGraphicsManager::CGraphicsManager()
	{
		m_pDefaultTexture = 0;
	}

	//------------------------------------------
	CGraphicsManager::~CGraphicsManager()
	{
		for(unsigned int i=0; i<m_textures.size(); i++) delete m_textures[i]; //Delete any survivors
		m_textures.clear();

		CON(MSG_DBG, _W("All the remaining textures deleted..."));

		for(unsigned int i=0; i<m_meshes.size(); i++) delete m_meshes[i]; //Delete any survivors
		m_meshes.clear();
		
		for(UINT i=0; i<m_vddescs.Size(); i++) delete m_vddescs[i];

		CON(MSG_DBG, _W("All the remaining meshes deleted..."));

		// delete cached vbs
		CON(MSG_DBG, _W("Deleting %d cached vertex buffers..."), m_cacheVB.Size());
		for(UINT i=0; i<m_cacheVB.Size(); i++) delete m_cacheVB[i];

		// delete cached ibs
		CON(MSG_DBG, _W("Deleting %d cached index buffers..."), m_cacheIB.Size());
		for(UINT i=0; i<m_cacheIB.Size(); i++) delete m_cacheIB[i];

	}

	//------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
	ITexture* CGraphicsManager::CreateTexture(sTextureDesc &desc, BYTE *data, unsigned long len)
	{
		CTexture *tex = new CTexture;
		if(!tex->CreateTexture(desc, data, len)) 
		{
			CON(MSG_ERR, _W("Creating new texture failed!"));
			return 0;
		}
		tex->AddReference();	//Increase the number of references to 1
		m_textures.push_back(tex);
		CON(MSG_DBG, _W("New texture created..."));
		return tex;
	}

	//------------------------------------------
	void CGraphicsManager::DeleteTexture(P3D::ITexture *texture)
	{
 		CTexture *ctex = (CTexture*)texture;
 		if(!ctex->CanRelease()) return; //m_pDefaultTexture cannot be released this way for example
 		unsigned int n = ctex->EraseReference(); //Decrease the number of references
 		if(n == 0)	//If there are no references
 		{
 			for(unsigned int i=0; i<m_textures.size(); i++)
 			{
 				if(ctex == m_textures[i])	//Find the desired texture
 				{
 					CON(MSG_DBG, _W("Texture \"%s\" deleted..."), texture->GetFilename());
 					m_textures.erase(m_textures.begin()+i);	//And erase it
 					CRenderer::cPrimitiveRenderer()->DeleteFromTextureList(ctex);
 					delete ctex;
 				}
 			}
 		}
	}

	//------------------------------------------
	void CGraphicsManager::CheckForFreeResources()
	{
		for(unsigned int i=0; i<m_textures.size(); i++)
		{
			if(m_textures[i]->GetNumReferences() == 0) 
			{
				delete m_textures[i];
				m_textures.erase(m_textures.begin()+i);	
			}
		}
	}

//---------------------------------------------------------------------------

	ITexture*  CGraphicsManager::LoadTexture(const wchar* filename)
	{
		CTextureLoader tl;
		//wchar path[512]="textures/test2.rtex";
		//wstrcat(path, filename);
		if (tl.Load(filename) == false)
		{
			CON(MSG_ERR, _W("Texture %s cannot be loaded!"), filename);
			return 0;
		}

		sRTInfo ti = tl.GetInfo();

		eResourceFormat texformat;
		switch(ti.texformat)
		{
		case RTF_DXT1:
			texformat = P3D_FORMAT_DXT1;
			break;
		case RTF_DXT3:
			texformat = P3D_FORMAT_DXT3;
			break;
		case RTF_DXT5:
			texformat = P3D_FORMAT_DXT5;
			break;
		default:
			CON(MSG_ERR, _W("Requested texture data format is not (yet) supported!") );
			return false;
		}

		sTextureDesc desc;
		desc.ArraySize = 1;
		desc.AutoGenerateMips = false;
		desc.BindFlag = P3D_TEXBINDFLAG_TEXTURE2D;
		desc.CPUAccess = P3D_CPUACCESS_NONE;
		desc.Format = texformat;
		desc.MipLevels = tl.GetNumMips();
		desc.ResourceUsage = P3D_RESOURCEUSAGE_DEFAULT;
		desc.Width = ti.width;
		desc.Height = ti.height;

		CTexture *tex = new CTexture;
		const sImageData *ida = tl.GetSubsetByID(0);
		//ida->subset.cubeFaceID
		if(!tex->CreateTexture(desc, (BYTE *)ida->pData, ida->size)) 
		{
			CON(MSG_ERR, _W("Creating texture %s failed!"), filename);
			delete tex;
			return 0;
		}

		for(unsigned int i=1; i<tl.GetNumMips(); i++)
		{
			const sImageData *ida = tl.GetSubsetByID(i);
			tex->SetMipMapData(0, i, (BYTE *)ida->pData, ida->size);
		}

		tex->AddReference();	//Increase the number of references to 1
		tex->SetFilename(filename);
		m_textures.push_back(tex);
		//CON(MSG_DBG, "mipmaps: %d size: %d", tl.GetNumMips(), tl.GetInfo().width);
		CON(MSG_DBG, _W("Texture \'%s\' loaded..."), filename);
		return tex;
		//return m_pDefaultTexture;
	}

	bool CGraphicsManager::Create()
	{
		// construct placeholder texture for "failed-to-load" textures
		BYTE *tdata = new BYTE[64*+64];
		memset(tdata, 255, sizeof(BYTE) * 64 * 64);

		sTextureDesc desc;
		desc.ArraySize = 1;
		desc.AutoGenerateMips = true;
		desc.BindFlag = P3D_TEXBINDFLAG_TEXTURE2D;
		desc.CPUAccess = P3D_CPUACCESS_NONE;
		desc.Format = P3D_FORMAT_L8_UNORM;
		desc.MipLevels = 0;
		//desc.ResourceUsage = P3D_RESOURCEUSAGE_IMMUTABLE; //FIXME: causes error
		desc.ResourceUsage = P3D_RESOURCEUSAGE_DEFAULT;
		desc.Width = 64;
		desc.Height = 64;

		m_pDefaultTexture = (CTexture*)CreateTexture(desc, tdata, 64*64);
		delete[] tdata;
		m_pDefaultTexture->EnableAutoRelease(false);

		//Declared in VertexTypes.h
		RegisterVertexTypes();

		return true;
	}

	/// TODO: Temporary/obsolete mesh vertex for rendering?? Should be based on material specific needs?
	struct MeshVertex
	{
		Vec3 pos;
		Vec3 normal;
		Vec2 uv;
	};

	//--------------------------------
	IMesh* CGraphicsManager::LoadMesh(const wchar *filename)
	{

		for(unsigned int i=0; i<m_meshes.size(); i++)
		{
			if( wstrcmp(filename, m_meshes[i]->GetFilename()) == 0 ) 
			{
				//CON(MSG_DBG, "Mesh %s already loaded...", filename);
				return m_meshes[i];
			}
		}


		IMeshLoader *loader = CRenderer::mEngine()->iMeshLoader();
		if (!loader->Load(filename, true)) return NULL;
		sRMVertex * vertex = 0;

		CMesh *mesh = new CMesh;

		VertexElement ve[] = 
		{	VertexElement(P3DVD_FLOAT3, P3DVU_POSITION), 
			VertexElement(P3DVD_FLOAT3, P3DVU_NORMAL), 
			VertexElement(P3DVD_FLOAT2, P3DVU_TEXCOORD),
			P3DVE_END()
		};

		unsigned int numVertices;
		unsigned int numIndices;
		unsigned int numSubsets;
		const sRMVertex *vertices = loader->GetVertices(numVertices);
		const unsigned int *indices = loader->GetIndices(numIndices);
		const sRMSubset *subsets = loader->GetSubsets(numSubsets);

		
		sVBDesc vbdesc;
		vbdesc.VertexCount = numVertices;
		vbdesc.VertexSize = sizeof(VPosUVNormal);
		vbdesc.CPUAccess = P3D_CPUACCESS_READ_WRITE;
		vbdesc.ResourceUsage = P3D_RESOURCEUSAGE_DEFAULT;
		vbdesc.PointParticles = false;

		sIBDesc ibdesc;
		ibdesc.IndexCount = numIndices;
		ibdesc.IndexFormat = IBFORMAT_32;
		ibdesc.CPUAccess = P3D_CPUACCESS_READ_WRITE;
		ibdesc.ResourceUsage = P3D_RESOURCEUSAGE_DEFAULT;

		sMeshDesc meshdesc;
		meshdesc.VBDesc = vbdesc;
		meshdesc.IBDesc = ibdesc;

// 		VPosUVNormal *v = new VPosUVNormal[numVertices];
// 		for(unsigned int i=0; i<numVertices; i++)
// 		{
// 			v[i].pos = vertices[i].position;
// 			v[i].normal = vertices[i].normal;
// 			v[i].uv = vertices[i].uv;
// 		}
		
		mesh->Create(meshdesc, 0, 0);

		//delete[] v;

		mesh->SetFilename(filename);
		
		unsigned int *i;
		mesh->LockIndexBuffer((void**)(&i), true);
		memcpy((void*)i, indices, sizeof(unsigned int)* numIndices);
		//for(UINT y=0; y<numIndices; y++) i[y] = indices[y];
		mesh->UnLockIndexBuffer();

		VPosUVNormal *v;
		mesh->LockVertexBuffer((void**)(&v), true);
		for(unsigned int i=0; i<numVertices; i++)
		{
			v[i].pos = vertices[i].position;
			v[i].normal = vertices[i].normal;
			v[i].uv = vertices[i].uv;
		}
		mesh->UnLockVertexBuffer();

		for(unsigned int i=0; i<numSubsets; i++)
		{
			sAttribRange ar;
			ar.FaceStart = subsets[i].firstindex/3;
			ar.FaceCount = subsets[i].numindices/3;
			mesh->AddSubset(ar);
			
			CTexture * tex = (CTexture*)LoadTexture( _A2W(subsets[i].texture) ); //W KEX: Temporary hack unless rmesh won't use wchars
			if(tex) mesh->SetTexture(i, tex);
		}

		// 	AttribRange ar;
		// 	ar.FaceStart = 0;
		// 	ar.FaceCount = numIndices/3;
		// 	mesh->AddSubset(ar);

		loader->Unload();

		m_meshes.push_back(mesh);
		CON(MSG_DBG, _W("Mesh %s loaded..."), filename);

		return mesh;
		//return 0;
	}

	ITexture* CGraphicsManager::GetDafaultTexture()
	{
		return m_pDefaultTexture;
	}

	void CGraphicsManager::OnLostDevice()
	{
		for (unsigned int i=0; i<m_textures.size(); i++)
		{
			m_textures[i]->OnLostDevice();
		}

		for (unsigned int i=0; i<m_meshes.size(); i++)
		{
			m_meshes[i]->OnLostDevice();
		}
	}

	void CGraphicsManager::OnResetDevice()
	{
		for (unsigned int i=0; i<m_textures.size(); i++)
		{
			m_textures[i]->OnResetDevice();
			CON(MSG_DBG, _W("Texture %s resetted..."), m_textures[i]->GetFilename());
		}

		for (unsigned int i=0; i<m_meshes.size(); i++)
		{
			m_meshes[i]->OnResetDevice();
			CON(MSG_DBG, _W("Mesh resetted..."));
		}

		// 	DWORD *mi;
		// 	m_meshes[0]->LockIndexBuffer((void**)&mi, false);
		// 	DWORD ind[100];
		// 	memcpy(ind, mi, 100*sizeof(DWORD));
		// 	m_meshes[0]->UnLockIndexBuffer();

		// 	MeshVertex *v;
		// 	m_meshes[0]->LockVertexBuffer((void**)(&v), false);
		// 	
		// 	m_meshes[0]->UnLockVertexBuffer();
	}

	void CGraphicsManager::RegisterVertexDesc(const VertexElement vearray[], Str &name)
	{
		sVertexDeclarationDesc *desc = new sVertexDeclarationDesc;
		desc->name.SetEx(name);
		
		UINT numElements = 0;
		while(vearray[numElements].vertexType != P3DVD_NONE)
		{
			numElements++;
		}
		numElements++; //copy P3DVE_END()

		desc->vearray = new VertexElement[numElements];
		for(UINT i=0; i<numElements; i++)	desc->vearray[i] = vearray[i];

		m_vddescs.AddEx(desc);
	}

	UINT CGraphicsManager::GetVertexDescIDByName(Str &name)
	{
		for(UINT i=0; i<m_vddescs.Size(); i++)	
		{
			if(m_vddescs[i]->name == name) return i;
		}

		assertd(false, "GetVertexDescIDByName failed! Vertex description with that name not found!! Returning 0...");
		return 0;
	}

	// ----------------------------------------------------
	bool CGraphicsManager::CacheMeshData(MeshData& md)
	{
		if (md.Indices.Size()==0)
		{
			CON(MSG_ERR, _W("Trying to cache MeshData without indices"));
			return false;
		}

		if (md.DataStreams.Size() ==0)
		{
			CON(MSG_ERR, _W("Trying to cache MeshData without vertex data streams"));
			return false;
		}

		if (   md.DataStreams[0].Float1.Size()==0 
			&& md.DataStreams[0].Float2.Size()==0 
			&& md.DataStreams[0].Float3.Size()==0 
			&& md.DataStreams[0].Float4.Size()==0 )
		{
			CON(MSG_ERR, _W("Trying to cache MeshData without at least one vertex stream data"));
			return false;
		}

		// TODO: check for all streams to have vertCnt items

		// compute immediate data
		unsigned int vertCnt=0;
		unsigned int vertSize=0; // in whole floats

		vertCnt = md.NumVerts;

		// compute vertex size
		for (unsigned int ds=0; ds<md.DataStreams.Size(); ds++) // for each data stream
		{
			vertSize += md.DataStreams[ds].DataSize;
		}

		float *vertData = new float[vertSize*vertCnt];

		// copy data
		unsigned int currVertexDataOffset=0;
		for (unsigned int ds=0; ds<md.DataStreams.Size(); ds++) // for each data stream
		{
			for (unsigned int vi=0; vi<vertCnt; vi++)
			{
				switch(md.DataStreams[ds].DataSize)
				{
				case 1:
					memcpy(&vertData[vertSize*vi+currVertexDataOffset], &md.DataStreams[ds].Float1[vi], 1*sizeof(float) );
					break;
				case 2:
					memcpy(&vertData[vertSize*vi+currVertexDataOffset], &((Vec2&)md.DataStreams[ds].Float2[vi]).x, 2*sizeof(float) );
					break;
				case 3:
					memcpy(&vertData[vertSize*vi+currVertexDataOffset], &((Vec3&)md.DataStreams[ds].Float3[vi]).x, 3*sizeof(float) );
					break;
				case 4:
					memcpy(&vertData[vertSize*vi+currVertexDataOffset], &((Vec4&)md.DataStreams[ds].Float4[vi]).x, 4*sizeof(float) );
					break;
				}
			}
			currVertexDataOffset += md.DataStreams[ds].DataSize;
		}

		CVertexBuffer *pVB = new CVertexBuffer();
		CIndexBuffer *pIB = new CIndexBuffer();

		// create VB
		sVBDesc vbDesc;
		vbDesc.CPUAccess = P3D_CPUACCESS_NONE;
		vbDesc.PointParticles = false;
		vbDesc.ResourceUsage = P3D_RESOURCEUSAGE_IMMUTABLE;
		vbDesc.VertexSize = vertSize * sizeof(float);
		vbDesc.VertexCount = vertCnt;

		// TODO: prolezt vd a doplnit co chybi

		if(!pVB->Create(vbDesc, vertData)) return false;

		// create IB
		sIBDesc ibDesc;
		ibDesc.CPUAccess = P3D_CPUACCESS_NONE;
		ibDesc.IndexFormat = IBFORMAT_32;
		ibDesc.ResourceUsage = P3D_RESOURCEUSAGE_IMMUTABLE;
		ibDesc.IndexCount = md.Indices.Size();

		int *indData = new int[md.Indices.Size()];
		for(unsigned int i=0; i<md.Indices.Size(); i++) indData[i] = md.Indices[i];
		
		//if(!pIB->Create(ibDesc, &( (int&)md.Indices[0] ) )) return false;
		if(!pIB->Create(ibDesc, indData )) return false;

		delete[] indData;
		delete[] vertData;

		// add to cached buffers
		md.ibId = m_cacheIB.Size();
		m_cacheIB.AddEx(pIB);

		md.vbId = m_cacheVB.Size();
		m_cacheVB.AddEx(pVB);

		for (unsigned int si = 0; si < md.Subsets.Size(); si++)
		{
			MeshSubset &ms = md.Subsets[si];

			// --- cache material
			if ( ms.materialPtr->BaseShader.GetNumChars() == 0 )
			{
				CON( MSG_ERR, _W("Warning: Material withou BaseShader defined!") );
				ms.materialPtr->BaseShader.SetEx( _W("DefaultDeferred") );
			}
			sShaderDesc sdesc;
			// VS
			sdesc.ShaderType = SHADERTYPE_VERTEX_SHADER;
			sdesc.EntryFunction = Str("vs_main");
			//sdesc.ShaderFile.FormatEx( _W("%s.cg"), ms.materialPtr->BaseShader.Get() );
			sdesc.ShaderFile.SetEx( ms.materialPtr->BaseShader);
			sdesc.ShaderName.FormatEx( _W("%s_VS"), ms.materialPtr->BaseShader.Get() );
			sdesc.VertexDescID = this->GetVertexDescIDByName( Str( "VDStaticMesh" ) );
			ms.materialPtr->vsPtr = CRenderer::cShaderManager()->CreateShader(sdesc);
			// PS
			sdesc.ShaderType = SHADERTYPE_PIXEL_SHADER;
			sdesc.EntryFunction = Str("ps_main");
			//sdesc.ShaderFile.FormatEx( _W("%s.cg"), ms.materialPtr->BaseShader.Get() );
			sdesc.ShaderFile.SetEx(ms.materialPtr->BaseShader);
			sdesc.ShaderName.FormatEx( _W("%s_PS"), ms.materialPtr->BaseShader.Get() );
			ms.materialPtr->psPtr = CRenderer::cShaderManager()->CreateShader(sdesc);

			// --- cache material instance
			// TODO: create constant buffers
		}

		return true;
	}
	
	// ----------------------------------------------------
	CVertexBuffer* CGraphicsManager::GetCachedVertexBuffer(unsigned int id)
	{
		assertd(id < m_cacheVB.Size(), "VertexBuffer Cache ID outside array bounds!");
		return m_cacheVB[id];
	}

	// ----------------------------------------------------
	CIndexBuffer* CGraphicsManager::GetCachedIndexBuffer(unsigned int id)
	{
		assertd(id < m_cacheIB.Size(), "IndexBuffer Cache ID outside array bounds!");
		return m_cacheIB[id];
	}

};