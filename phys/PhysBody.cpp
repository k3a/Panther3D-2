/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "PhysBody.h"



namespace P3D
{
	extern NxScene *g_pPhysScene;

	CPhysBody::CPhysBody()
	{
		m_pActor = 0;
		m_shapes = 0;
	}

	CPhysBody::~CPhysBody()
	{
		if(m_pActor)
		{
			sNxActorUserData *udata = (sNxActorUserData*)m_pActor->userData;
			SAFE_DELETE(udata);
			g_pPhysScene->releaseActor(*m_pActor);
		}
		m_pActor = 0;
	}

	bool CPhysBody::Create(AABB boundingbox, Vec3 position, bool dynamic)
	{
		NxActorDesc actorDesc;
		NxBodyDesc bodyDesc;

		NxBoxShapeDesc boxDesc;
		//set size
		//boxDesc.dimensions.set(boundingbox.GetExtent(0), boundingbox.GetExtent(1), boundingbox.GetExtent(2));
		boxDesc.dimensions.set((boundingbox.max.x - boundingbox.min.x)/2, (boundingbox.max.y - boundingbox.min.y)/2, (boundingbox.max.z - boundingbox.min.z)/2);
		//set local position within the body
		boxDesc.localPose.t = NxVec3(boundingbox.GetCenter(0), boundingbox.GetCenter(1), boundingbox.GetCenter(2));
		//boxDesc.localPose.t = NxVec3(0.0f, 0.0f, 0.0f);
		actorDesc.shapes.pushBack(&boxDesc);

		if(dynamic) actorDesc.body = &bodyDesc;
		else actorDesc.body = 0;

		//set actor's global position
		actorDesc.globalPose.t = NxVec3(position.x, position.y, position.z);
		actorDesc.density = 10;
		m_pActor = g_pPhysScene->createActor(actorDesc);
		m_pActor->userData = new sNxActorUserData;
		return true;
	}

	void CPhysBody::SetKinematic(bool kinematic)
	{
		if(kinematic)
		{
			if(m_pActor->readBodyFlag(NX_BF_KINEMATIC))return;
			m_pActor->raiseBodyFlag(NX_BF_KINEMATIC);
		}
		else
		{
			if(m_pActor->readBodyFlag(NX_BF_KINEMATIC)) m_pActor->clearBodyFlag(NX_BF_KINEMATIC);
		}
	}

	const Matrix* CPhysBody::GetMatrix()
	{
		
		m_pActor->getGlobalPose().getColumnMajor44(m_matrix.m);
		return &m_matrix;
	}

	bool CPhysBody::Changed()
	{
		sNxActorUserData *udata = (sNxActorUserData*)m_pActor->userData;

		if( udata->changed ) 
		{
			udata->changed = false;
			return true;
		}
		
		return false;
	}

	bool CPhysBody::AddShape(sPhysShapeDesc *desc)
	{
		bool res = false;

		if(desc->shape == PHYSS_CUBE)
		{
			sBoxShapeDesc *bdesc = (sBoxShapeDesc*)desc;
			NxBoxShapeDesc boxDesc;
			boxDesc.dimensions.set(bdesc->size.x/2, bdesc->size.y/2, bdesc->size.z/2);
			boxDesc.localPose.t = NxVec3(bdesc->position.x, bdesc->position.y, bdesc->position.z);
			boxDesc.density = 10.0f;
			m_pActor->createShape(boxDesc);
			res = true;
		}
		else if(desc->shape == PHYSS_SPHERE)
		{
			sSphereShapeDesc *sdesc = (sSphereShapeDesc*)desc;
			NxSphereShapeDesc sprDesc;
			sprDesc.radius = sdesc->radius;
			sprDesc.localPose.t = NxVec3(sdesc->position.x, sdesc->position.y, sdesc->position.z);
			sprDesc.density = 10.0f;
			m_pActor->createShape(sprDesc);
			res =  true;
		}
		else if(desc->shape == PHYSS_CAPSULE)
		{
			sCapsuleShapeDesc *cdesc = (sCapsuleShapeDesc*)desc;
			NxCapsuleShapeDesc capDesc;
			capDesc.radius = cdesc->radius;
			capDesc.height = cdesc->height;
			capDesc.localPose.t = NxVec3(cdesc->position.x, cdesc->position.y, cdesc->position.z);
			capDesc.density = 10.0f;
			m_pActor->createShape(capDesc);
			res =  true;
		}

		if(res)
		{
			m_pActor->updateMassFromShapes(1, 0); //Not shore about the values.
			return true;
		}
		
		return false;
	}

	bool CPhysBody::SetShape(sPhysShapeDesc *desc)
	{
		NxShape *const *shapes = m_pActor->getShapes();
		unsigned int numShapes = m_pActor->getNbShapes();
		bool res = false;

		if(desc->shape == PHYSS_CUBE)
		{
			sBoxShapeDesc *bdesc = (sBoxShapeDesc*)desc;
			NxBoxShapeDesc boxDesc;
			boxDesc.dimensions.set(bdesc->size.x/2, bdesc->size.y/2, bdesc->size.z/2);
			boxDesc.localPose.t = NxVec3(bdesc->position.x, bdesc->position.y, bdesc->position.z);
			boxDesc.density = 10.0f;
			m_pActor->createShape(boxDesc);
			res = true;
		}
		else if(desc->shape == PHYSS_SPHERE)
		{
			sSphereShapeDesc *sdesc = (sSphereShapeDesc*)desc;
			NxSphereShapeDesc sprDesc;
			sprDesc.radius = sdesc->radius;
			sprDesc.localPose.t = NxVec3(sdesc->position.x, sdesc->position.y, sdesc->position.z);
			sprDesc.density = 10.0f;
			m_pActor->createShape(sprDesc);
			res =  true;
		}

		if(res)
		{
			for(unsigned int i=0; i<numShapes; i++)
			{
				m_pActor->releaseShape(*shapes[i]);
			}
			m_pActor->updateMassFromShapes(1, 0); //Not shore about the values.
			return true;
		}
		

		return false;
	}

	
}