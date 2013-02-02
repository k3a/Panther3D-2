/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#if defined(_DX) && _DX == 10
#include "DX10/ShaderManagerHLSL.h"
#elif defined(_DX) && _DX == 9
#include "DX/ShaderManagerHLSL.h"
#else
#include "ShaderManagerCg.h"
#endif

#include "Shader.h"
#include <vector>

namespace P3D
{
	
	#if defined(_DX)
	class CShaderManager : public CShaderManagerHLSL
	#else
	class CShaderManager : public CShaderManagerCg
	#endif
	{
	public:
		CShaderManager();
		~CShaderManager();
		IShader* CreateShader(sShaderDesc &desc);
		void DeleteShader(IShader *shader);

		void OnLostDevice();
		void OnResetDevice();

	private:
		std::vector<CShader*> m_shaders;
	};
};
