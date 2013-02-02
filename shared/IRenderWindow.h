/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, am!go
*/
#pragma once
#include "IConsole.h"
#include "rtex.h"
#include "BufferRequisities.h"


namespace P3D
{

	struct sRenderWindowDesc
	{
		Vec2i size;
		bool windowed;
	};

	///Represents a texture object.
	class IRenderWindow
	{
	public:
		virtual inline sRenderWindowDesc GetDesc()=0;
	};
};
