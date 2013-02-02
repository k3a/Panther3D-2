/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#if defined(_DX) && _DX == 10
#include "DX10/ShaderBufferHLSL.h"
#elif defined(_DX) && _DX == 9
#include "DX/ShaderBufferHLSL.h"
#else
#include "ShaderBufferCg.h"
#endif

namespace P3D
{
	#if defined(_DX)
	//use HLSL shaders
	typedef CShaderBufferHLSL CShaderBuffer;
	#else
	//use Cg shaders
	typedef CShaderBufferCg CShaderBuffer;
	#endif
};
