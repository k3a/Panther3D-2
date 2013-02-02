/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "PhysManager.h"



namespace P3D
{

	NxControllerManager *g_pControllerManager = 0;
	extern NxScene *g_pPhysScene;
	
	CPhysManager::CPhysManager()
	{
		m_pAllocator = 0;
		m_pControllerManager = 0;

	}
	
	CPhysManager::~CPhysManager()
	{
		for (unsigned int i=0; i<m_bodies.size(); i++) delete m_bodies[i];
		m_bodies.clear();

		for (unsigned int i=0; i<m_trimeshes.size(); i++) delete m_trimeshes[i];
		m_trimeshes.clear();

		for (unsigned int i=0; i<m_characters.size(); i++) delete m_characters[i];
		m_characters.clear();

		SAFE_DELETE(m_pAllocator);
		if(m_pControllerManager) NxReleaseControllerManager(m_pControllerManager);
		m_pControllerManager  = 0;
	}

	bool CPhysManager::Create()
	{
		m_pAllocator = new NxUserAllocatorDefault;
		m_pControllerManager = NxCreateControllerManager(m_pAllocator);
		if(!m_pControllerManager) return false;
		g_pControllerManager = m_pControllerManager;
		return true;
	}

	void CPhysManager::DeleteBody(P3D::IPhysBody *body)
	{
		for (unsigned int i=0; i<m_bodies.size(); i++)
		{
			if(m_bodies[i] == body)
			{
				m_bodies.erase(m_bodies.begin()+i);
				CPhysBody *cbody = (CPhysBody*)body;
				SAFE_DELETE(cbody);
			}
		}
	}

	void CPhysManager::DeleteCharacter(P3D::IPhysCharacter *character)
	{
		for (unsigned int i=0; i<m_characters.size(); i++)
		{
			if(m_characters[i] == character)
			{
				m_characters.erase(m_characters.begin()+i);
				CPhysCharacter * ccharacter = (CPhysCharacter*)character;
				SAFE_DELETE(ccharacter);
			}
		}
	}

	IPhysBody* CPhysManager::CreateBody(P3D::AABB boundingbox, P3D::Vec3 position, bool dynamic)
	{
		CPhysBody *body = new CPhysBody;
		if(!body->Create(boundingbox, position, dynamic))
		{
			delete body;		
			return 0;
		}
		m_bodies.push_back(body);
		return body;
	}

	IPhysCharacter* CPhysManager::CreateCharacter(sPhysControllerDesc desc, Vec3 position)
	{
		CPhysCharacter *character = 0;
		if(!character->Create(desc, position))
		{
			delete character;
			return 0;
		}
		m_characters.push_back(character);
		return character;
	}

	void CPhysManager::UpdateControllers()
	{
		if(m_characters.size())
		{
			NxReal maxTimestep;
			NxTimeStepMethod method;
			NxU32 maxIter;
			NxU32 numSubSteps;
			g_pPhysScene->getTiming(maxTimestep, maxIter, method, &numSubSteps);
			if (numSubSteps)  m_pControllerManager->updateControllers(); 
		}
	}

	IPhysTriangleMesh* CPhysManager::CreateTriangleMesh(const wchar* filename, Vec3 position, Vec3 rotation, Vec3 scale)
	{
		CPhysTriangleMesh *trimesh = new CPhysTriangleMesh;
		if(!trimesh->Create(filename, position, rotation, scale))
		{
			delete trimesh;		
			return 0;
		}
		m_trimeshes.push_back(trimesh);
		return trimesh;
	}
}