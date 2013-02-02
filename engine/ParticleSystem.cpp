/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "ParticleSystem.h"
#include "Engine.h"

namespace P3D
{

	CParticleSystem::CParticleSystem()
	{
		m_pParticleRenderer = 0;
	}

	CParticleSystem::~CParticleSystem()
	{
		SAFE_DELETE(m_pParticleRenderer);
	}

	bool CParticleSystem::Create()
	{
		
		return true;
	}

}