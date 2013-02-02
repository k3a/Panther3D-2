/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once
//#include "IPhysEngine.h"

#include <PhysX/NxPhysics.h>
#include <vector>
#include "types.h"
#include "PhysBody.h"
#include "PhysTriangleMesh.h"
#include "IPhysManager.h"
#include <PhysX/NxControllerManager.h>
#include <PhysX/NxUserAllocatorDefault.h>
#include "PhysCharacter.h"

namespace P3D
{
	class CPhysManager : public IPhysManager
	{
	public:
		CPhysManager();
		~CPhysManager();
		bool Create();
		IPhysBody* CreateBody(AABB boundingbox, Vec3 position, bool dynamic = true);
		void DeleteBody(IPhysBody *body);
		IPhysTriangleMesh* CreateTriangleMesh(const wchar* filename, Vec3 position, Vec3 rotation, Vec3 scale);
		void DeleteTriangleMesh(IPhysTriangleMesh *trimesh);
		IPhysCharacter* CreateCharacter(sPhysControllerDesc desc, Vec3 position);
		void DeleteCharacter(IPhysCharacter *character);
		void UpdateControllers();

	private:
		std::vector<CPhysBody*> m_bodies;
		std::vector<CPhysTriangleMesh*> m_trimeshes;
		std::vector<CPhysCharacter*> m_characters;
		NxUserAllocatorDefault* m_pAllocator;
		NxControllerManager* m_pControllerManager;
	};

};