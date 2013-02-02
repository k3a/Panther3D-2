/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once
#include "IShader.h"

namespace P3D
{

	///Represents a manager for handling shaders.
	class IShaderManager
	{
	public:
		virtual IShader* CreateShader(sShaderDesc &desc)=0;
		virtual void DeleteShader(IShader *shader)=0;
	};
};
