/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "Body.h"



namespace P3D
{
	extern NxScene *g_pPhysScene;

	CBody::CBody()
	{
		m_pActor = 0;
		m_shapes = 0;
	}

	CBody::~CBody()
	{

	}

	bool CBody::Create(AABB boundingbox, Vec3 position, bool dynamic)
	{
		NxActorDesc actorDesc;
		NxBodyDesc bodyDesc;

		NxBoxShapeDesc boxDesc;
		//set size
		boxDesc.dimensions.set(boundingbox.GetExtent(0), boundingbox.GetExtent(1), boundingbox.GetExtent(2));
		//set local position within the body
		boxDesc.localPose.t = NxVec3(boundingbox.GetCenter(0), boundingbox.GetCenter(1), boundingbox.GetCenter(2));
		actorDesc.shapes.pushBack(&boxDesc);

		if(dynamic) actorDesc.body = &bodyDesc;
		else actorDesc.body = 0;

		//set actor's global position
		actorDesc.globalPose.t = NxVec3(position.x, position.y, position.z);
		m_pActor = g_pPhysScene->createActor(actorDesc);
		return true;
	}

	void CBody::SetKinematic(bool kinematic)
	{
		if(m_pActor->readBodyFlag(NX_BF_KINEMATIC)) m_pActor->clearBodyFlag(NX_BF_KINEMATIC);
		else m_pActor->raiseBodyFlag(NX_BF_KINEMATIC);
	}
}