/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "types.h"
#include "BufferRequisities.h"

namespace P3D
{	

	struct VPosUV
	{
		VPosUV()
		{

		}

		VPosUV(Vec3 position, Vec2 uvcoord)
		{
			pos = position;
			uv = uvcoord;
		}

		Vec3 pos;
		Vec2 uv;
	};
	
	struct VPosUVCol
	{
		VPosUVCol()
		{

		}

		VPosUVCol(Vec3 position, Vec2 uvcoord, ColorRGBA color)
		{
			pos = position;
			uv = uvcoord;
			clr = color;
		}

		Vec3 pos;
		Vec2 uv;
		ColorRGBA clr;
	};

	struct VPosUVNormal
	{
		VPosUVNormal() {}
		VPosUVNormal(Vec3 &position, Vec2 &uvcoord, Vec3 &normalvector)
		{
			pos = position;
			uv = uvcoord;
			normal = normalvector;
		}
		
		Vec3 pos;
		Vec2 uv;
		Vec3 normal;
	};

	///////////////////////////////////////////////

	void RegisterVertexTypes();
	
};