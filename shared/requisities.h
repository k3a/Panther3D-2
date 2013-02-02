/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "types.h"

namespace P3D
{
	struct sCameraInfo
	{
		Vec3 position;
		Matrix viewMatrix;
		Matrix ProjectionMatrix
	};

}