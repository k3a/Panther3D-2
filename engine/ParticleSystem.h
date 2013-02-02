/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once
#include "ISpriteRenderer.h"

namespace P3D
{

	class CParticleSystem
	{
	public:
		CParticleSystem();
		~CParticleSystem();
		bool Create();
		void Render();
	
	private:
		ISpriteRenderer *m_pParticleRenderer;
	};

}