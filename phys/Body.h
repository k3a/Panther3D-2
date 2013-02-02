/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, am!go
*/
#pragma once
//#include "IPhysEngine.h"

#include <NxPhysics.h>
#include "types.h"

namespace P3D
{

	class CBody
	{
	public:
		CBody();
		~CBody();
		bool Create(AABB boundingbox, Vec3 position, bool dynamic);
		void SetKinematic(bool kinematic);
		//void AddShapeBox(AABB box, Vec3 position);

	private:
		NxActorDesc m_desc;
		NxActor *m_pActor;
		unsigned int m_shapes;
	};

};