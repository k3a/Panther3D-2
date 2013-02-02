/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once
#include "interface.h"
#include "IPhysManager.h"

namespace P3D
{

	///The main class of the Phys module. 
	class IPhysEngine : public IBaseInterface
	{
	public :

		virtual bool Initialize(void* pEngine)=0;
		///Begins the simulation by given time.
		virtual void SimulationBegin(float deltatime)=0;
		///Ends the simulation.
		virtual void SimulationEnd()=0;

		// - subclasses
		DECLARE_ISUBCLASS(PhysManager);
	};



}