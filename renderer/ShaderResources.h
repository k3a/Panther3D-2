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

	enum eLightType
	{
		P3D_LIGHTTYPE_DIRECTIONAL,
		P3D_LIGHTTYPE_POINT,
		P3D_LIGHTTYPE_SPOT
	};

	class CLight
	{
		eLightType type;
		bool enabled;
		bool castShadows;
		ColorRGBA diffuseColor;
		float range;
		float specPower;
		bool useAttenuation;
		Vec3 attenuation;
		float spotPower
	};
	
	//cbuffers
	struct sCbPerFrame {	// slot 0
		float gTime;
	};

	struct sCbPerCamera {	// slot 1
		Matrix  gViewProj;
		Vec3    gCamPos;
	};

	struct sCbPerMaterial {	// slot 2
		ColorRGB diffuse;
	};

};