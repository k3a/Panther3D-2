/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#if defined(_DX) && _DX==9
#include "DX10/RenderWindowWrapper.h"
#elif defined(_OGL)
#include "DX10/RenderWindowWrapper.h"
#elif defined(_DX) && _DX==10
#include "DX10/RenderWindowWrapper.h"
#endif

namespace P3D
{
	typedef CRenderWindowWrapper CRenderWindow;
};
