/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "PhysTriangleMesh.h"
#include "IMeshLoader.h"
#include "PhysEngine.h"



namespace P3D
{
	extern NxScene *g_pPhysScene;

	CPhysTriangleMesh::CPhysTriangleMesh()
	{
		m_pActor = 0;
	}

	CPhysTriangleMesh::~CPhysTriangleMesh()
	{
		if(m_pActor)
		{
			g_pPhysScene->releaseActor(*m_pActor);
		}
		m_pActor = 0;
	}

	bool CPhysTriangleMesh::Create(const wchar *filename, P3D::Vec3 position, P3D::Vec3 rotation, P3D::Vec3 scale)
	{
  		bool generate = true;
 		IFileSystem* pFS = CPhysEngine::mEngine()->mFilesystem();

		IMeshLoader *loader = CPhysEngine::mEngine()->iMeshLoader();
		if (!loader->Load(filename, true)) return NULL;
		
		const sRMInfo *info = loader->GetInfo();
		////////////////////////
		wchar cachefile[512];
		wsprintf(cachefile, 511, _W("cache/phys/%s#%s"), filename, _A2W(info->created) );
		FSFILE* cfp = pFS->Open(cachefile, _W("rb"));
		if(cfp) 
		{
			generate = false;
			pFS->Close(cfp);
		}
		////////////////////////

		NxActorDesc actorDesc;
		NxBodyDesc bodyDesc;
		NxTriangleMeshShapeDesc triShapeDesc;

		
		if(generate)
		{
			unsigned int numVertices;
			unsigned int numIndices;
			const sRMVertex *vertices = loader->GetVertices(numVertices);
			const unsigned int *indices = loader->GetIndices(numIndices);

			NxVec3 *pxvertices = new NxVec3[numVertices];
			NxU32 *pxindices = new NxU32[numIndices];

			for(unsigned int i=0; i<numVertices; i++) pxvertices[i] = NxVec3(vertices[i].position.x * scale.x, vertices[i].position.y * scale.y, vertices[i].position.z * scale.z);
			for(unsigned int i=0; i<numIndices; i++) pxindices[i] = indices[i];

			NxTriangleMeshDesc triDesc;
			triDesc.numVertices = numVertices;
			triDesc.numTriangles = numIndices/3;
			triDesc.pointStrideBytes = sizeof(NxVec3);
			triDesc.triangleStrideBytes = 3*sizeof(NxU32);
			triDesc.points = pxvertices;
			triDesc.triangles = pxindices;							
			triDesc.flags = 0;

			NxInitCooking();
			NxCookTriangleMesh(triDesc, P3DStream(cachefile, false));

			delete[] pxvertices;
			delete[] pxindices;
			NxCloseCooking();
		}

		triShapeDesc.meshData = g_pPhysScene->getPhysicsSDK().createTriangleMesh(P3DStream(cachefile, true));

		
		
		//MemoryWriteBuffer buf;
		//bool status = NxCookTriangleMesh(triDesc, buf);
		//triShapeDesc.meshData = g_pPhysScene->getPhysicsSDK().createTriangleMesh(MemoryReadBuffer(buf.data));


		if (!triShapeDesc.meshData) return false;
		actorDesc.shapes.pushBack(&triShapeDesc);
		
		Matrix m;
		m.SetIdentityMatrix();
		m.Translate(position.x, position.y, position.z);
		m.RotateZXY(rotation.x, rotation.y, rotation.z);
		NxMat34 mat;
		mat.setColumnMajor44(&m.m16[0]);
		actorDesc.globalPose  = mat;

		m_pActor = g_pPhysScene->createActor(actorDesc);
		
		//g_pPhysScene->getPhysicsSDK().releaseTriangleMesh(*triShapeDesc.meshData);
		if(!generate) CON(MSG_INFO, _W(" ...trianglemesh \'%s\' created (geometry loaded from \'%s#%s\')"), filename, filename, _A2W(info->created));
		else CON(MSG_INFO, _W(" Trianglemesh \'%s\' created (geometry saved to \'%s#%s\')"), filename, filename, _A2W(info->created));
		loader->Unload();

		return true;
	}

}