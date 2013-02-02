/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once
//#include "IPhysEngine.h"

#include <PhysX/NxPhysics.h>
#include <PhysX/NxControllerManager.h>
#include <PhysX/NxCapsuleController.h>
#include <PhysX/NxBoxController.h>
#include "types.h"
#include "IPhysCharacter.h"

namespace P3D
{
	
	class CPhysCharacter : public IPhysCharacter
	{
	public:
		CPhysCharacter();
		~CPhysCharacter();
		bool Create(sPhysControllerDesc desc, Vec3 position);
		void SetPosition(Vec3 position);
		Vec3 Move(Vec3 displacement);
	private:
		NxController *m_pController;

	};

}