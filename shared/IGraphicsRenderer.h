/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once

#include "interface.h"
#include "tArray.h"

#include "OModel.h"

namespace P3D {

	/// Class responsible for drawing all scene geometry
	class IGraphicsRenderer
	{
	public:
		/// Send GeometryChunks for drawing
		virtual void SendChunks(const tArray<GeometryChunk> &chunks)=0;
	};

}