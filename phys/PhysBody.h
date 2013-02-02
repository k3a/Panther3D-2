/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, am!go
*/
#pragma once
//#include "IPhysEngine.h"

#include <PhysX/NxPhysics.h>
#include "types.h"
#include "IPhysBody.h"

namespace P3D
{

	struct sNxActorUserData
	{
		bool changed;
		sNxActorUserData() {changed = false;}
		inline void OnChange() {changed = true;}
	};

	class CPhysBody : public IPhysBody
	{
	public:
		CPhysBody();
		~CPhysBody();
		bool Create(AABB boundingbox, Vec3 position, bool dynamic);
		void SetKinematic(bool kinematic);
		const Matrix* GetMatrix();
		//void AddShapeBox(AABB box, Vec3 position);
		bool Changed();
		sNxActorUserData userData;

		bool SetShape(sPhysShapeDesc *desc);
		bool AddShape(sPhysShapeDesc *desc);


	private:
		NxActorDesc m_desc;
		NxActor *m_pActor;
		unsigned int m_shapes;
		Matrix m_matrix;
	};

};