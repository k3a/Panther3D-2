/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kalda
*/

#pragma once

#include "types.h"
#include "ISoundSource.h"
#include "ISoundListener.h"

namespace P3D
{
	
	class ISoundManager
	{
	public:
		virtual bool PrecacheSound(const wchar *filename) = 0;
		virtual ISoundSource* CreateSource(const wchar *filename, Vec3 position, bool relativeToListener) = 0;
		virtual void ClearAll() = 0;
		virtual void DestroySource(ISoundSource *source) = 0;
		virtual inline ISoundListener* iSoundListener() = 0;
	};

}