/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once
#include "interface.h"

namespace P3D {


/// Global Configuration
/** With this class you are able to load or save .ini configuration files. 
These files are used to store so called SVars (global multi-module setting variables). 
For more information about in see SVar documentation. 
\see SVar
*/
class IConfig
{
public:
	/// Load settings from file
	/** 
	\param loadOnlyCategoryName Optionally you can specify which one category to load. NULL to load all categories from this file.
	\return false if configuration file not found or 0 variables actually set to Core. But you can continue with false state, no need to write message to console.
	*/
	virtual bool Load(const wchar* path, const wchar* loadOnlyCategoryName=NULL)=0;
	/// Load settings from file
	/** 
	\param loadOnlyCategoryName Optionally you can specify which one category to save. NULL to save all categories of all SVars.
	*/
	virtual bool Save(const wchar* path, const wchar* saveOnlyCategoryName=NULL)=0;
};

}; // ns