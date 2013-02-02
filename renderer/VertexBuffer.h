/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#if defined(_DX) && _DX==9
#include "DX/VertexBufferWrapper.h"
#elif defined(_OGL)
#include "OGL/VertexBufferWrapper.h"
#elif defined(_DX) && _DX==10
#include "DX10/VertexBufferWrapper.h"
#endif

namespace P3D
{
	#define MAX_VERTEX_DECLARATION_SIZE 50
	typedef CVertexBufferWrapper CVertexBuffer;
	typedef CVertexDeclarationWrapper CVertexDeclaration;
};
