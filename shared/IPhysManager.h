/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once
#include "interface.h"
#include "IPhysBody.h"
#include "IPhysCharacter.h"
#include "IPhysTriangleMesh.h"


namespace P3D
{

	///The PhysManager takes care about all the Phys resources.
	class IPhysManager : public IBaseInterface
	{
	public :
		///Creates a new body.
		virtual IPhysBody* CreateBody(AABB boundingbox, Vec3 position, bool dynamic = true)=0;
		///Deletes a body
		virtual void DeleteBody(IPhysBody *body)=0;
		///Creates a new character by given description.
		virtual IPhysCharacter* CreateCharacter(sPhysControllerDesc desc, Vec3 position)=0;
		///Deletes a character.
		virtual void DeleteCharacter(IPhysCharacter *character)=0;
		///Creates a new triangle mesh.
		virtual IPhysTriangleMesh* CreateTriangleMesh(const wchar* filename, Vec3 position, Vec3 rotation, Vec3 scale)=0;
	};
}
