/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
11/23/2009 File created - Petr
*/

#pragma once

#include "types.h"

namespace P3D{

	class CComponent
	{
	public:
		virtual void Update() {};
		virtual const Str& GetType()=0;
	};

}//ns