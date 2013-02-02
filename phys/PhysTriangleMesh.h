/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, am!go
*/
#pragma once
//#include "IPhysEngine.h"

#include <PhysX/NxPhysics.h>
#include <PhysX/NxCooking.h>
#include <fstream>
#include "Stream.h"
#include "P3DStream.h"
#include "types.h"
#include "IPhysTriangleMesh.h"

namespace P3D
{
	class CPhysTriangleMesh : public IPhysTriangleMesh
	{
	public:
		CPhysTriangleMesh();
		~CPhysTriangleMesh();
		bool Create(const wchar* filename, Vec3 position, Vec3 rotation, Vec3 scale);
	private:
		NxActor *m_pActor;
	};
}