/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

//#include "Shader.h"
#include "IShaderManager.h"
#include "../ShaderBuffer.h"

namespace P3D
{

	class CShaderManagerHLSL : public IShaderManager
	{
	public:
		CShaderManagerHLSL();
		~CShaderManagerHLSL();
		bool Create(void *device);
		inline const wchar* GetShaderProfile(eShaderType shaderType);
		void SetBuffer(eShaderType shaderType, UINT slot, CShaderBuffer *buffer);

		void OnLostDevice() {};
		void OnResetDevice() {};

	private:

	};
};
