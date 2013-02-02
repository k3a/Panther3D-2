/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once

#include "BaseObject.h"

namespace P3D
{
	/// Base texture object
	class Texture : public BaseObject
	{
	public:
		Texture()
		{
			REGISTER_PARAM(TextureName, "Texture name from which load data");

			texID = 0;
		}

		StringParam TextureName;

	public:
		// --- internal
		unsigned int texID;
	}

}; // ns