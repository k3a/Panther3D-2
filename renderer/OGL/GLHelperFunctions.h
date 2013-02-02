/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "glex.h"
#include "types.h"


namespace P3D
{

	bool CheckGLErrors(const wchar *info = _W("Info unavailable"));
	void CheckFramebufferStatus();
}