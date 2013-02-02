/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once
#include "interface.h"

namespace P3D
{

	///Defines a controller description of a capsule shape.
	struct sPhysControllerDesc
	{
		sPhysControllerDesc()
		{
			height = 3;
			radius = 1;
			stepOffset = 1.0;
			slopeLimit = 60.0f;
			skinWidth = 1.0f;
		}

		///Height of the capsule.
		float height;
		///Radius of the capsule.
		float radius;
		///The maximal angle which the character can climb.
		float slopeLimit;
		///Defines the skin width.
		float skinWidth;
		///The maximum height of an obstacle which the character can climb.
		float stepOffset;
	};

	///Defines the physical character which can represent the player, a soldier and so on.
	class IPhysCharacter
	{
	public:
		///Sets the world position.
		virtual void SetPosition(Vec3 position)=0;
		///Moves the character by given displacement vector.
		virtual Vec3 Move(Vec3 displacement)=0;
	};

}