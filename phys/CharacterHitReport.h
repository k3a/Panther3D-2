/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, am!go
*/
#pragma once

namespace P3D
{

	class ControllerHitReport : public NxUserControllerHitReport
	{
	public:
		virtual NxControllerAction  onShapeHit(const NxControllerShapeHit& hit)
		{
			if(hit.shape)
			{
				NxActor& actor = hit.shape->getActor();
				if(actor.isDynamic())
				{
					float pluspwr = 0.2f;
					if (abs(hit.dir.y) < 0.9f) // normal interaction 
					{ 
						NxVec3 dir = hit.dir; 
						//dir.y /= 10; 
						NxF32 coeff = actor.getMass() * hit.length * pluspwr; 
						actor.addForceAtLocalPos(dir*coeff, NxVec3(0,0,0), NX_IMPULSE); 
					} 
					else // y is almost -1 or 1, so up or down. It can be standing 
					{ 
						NxVec3 dir; 
						dir = hit.worldPos - hit.controller->getPosition(); 
						dir.normalize(); 
						dir += hit.dir; 
						dir.normalize(); 
						dir.y /= 15; // aby moc neskakalo 

						NxVec3 vel = hit.controller->getActor()->getLinearVelocity(); 
						vel *= 0.004f; // protoze kopeme a netlacime 
						NxF32 coeff = actor.getMass() * hit.length * vel.distance(NxVec3(0,0,0)); 
						actor.addForceAtLocalPos(dir*coeff, NxVec3(0,0,0), NX_IMPULSE); 
					} 
				}
			}

			return NX_ACTION_NONE;
		}

		virtual NxControllerAction  onControllerHit(const NxControllersHit& hit)
		{
			return NX_ACTION_NONE;
		}

	} gControllerHitReport;

}