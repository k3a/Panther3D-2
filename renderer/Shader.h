/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#if defined(_DX) && _DX == 10
#include "DX10/ShaderHLSL.h"
#elif defined(_DX) && _DX == 9
#include "DX/ShaderHLSL.h"
#else
#include "ShaderCg.h"
#endif

namespace P3D
{
	#if defined(_DX)
	//use HLSL shaders
	typedef CShaderHLSL CShader;
	#else
	//use Cg shaders
	typedef CShaderCg CShader;
	#endif
};
