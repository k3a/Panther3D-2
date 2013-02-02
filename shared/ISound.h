/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: Cutler
*/
#pragma once
#include "interface.h"
#include "ISoundSource.h"
#include "ISoundManager.h"

#include <vector>

namespace P3D
{
	class ISoundEngine : public IBaseInterface
	{
	public :

		virtual bool Initialize(void* pEngine)=0;
		DECLARE_ISUBCLASS(SoundManager);
	};


}