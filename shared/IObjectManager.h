/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "interface.h"

#include "BaseObject.h"

namespace P3D {

	class IObjectManager
	{
	public:
		virtual BaseObject* CreateObject(const wchar* objectName)=0;
	};

}; // ns