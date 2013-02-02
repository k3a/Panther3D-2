/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#include <DX/d3d9.h>
#include <DX/d3dx9.h>
#include "IShaderManager.h"

namespace P3D
{

	class CShaderManagerHLSL : public IShaderManager
	{
	public:
		CShaderManagerHLSL();
		~CShaderManagerHLSL();
		bool Create(void *device);
		inline const wchar* GetShaderProfile(eShaderType shaderType);

		void OnLostDevice() {};
		void OnResetDevice() {};

	private:

	};
};
