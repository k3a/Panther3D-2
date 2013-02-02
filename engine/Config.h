/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once

#include "types.h"
#include "interface.h"
#include "IConfig.h"

namespace P3D{

class CConfig : public IConfig
{
public:
	bool Load(const wchar* path, const wchar* loadOnlyCategoryName=NULL);
	bool Save(const wchar* path, const wchar* saveOnlyCategoryName=NULL);
};

}; // ns