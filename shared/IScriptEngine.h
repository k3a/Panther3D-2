/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include <angelscript.h>

class IScriptEngine
{
public:
	virtual bool LoadScript(const wchar * path)=0;
	virtual bool LoadScriptString(const char* code, const char* module=0, const char* sectionName="script")=0;
	virtual bool ExecuteString(const char* code, const char* module=0)=0;
	virtual asIScriptEngine *	GetVM()=0;
	virtual unsigned long GetMemoryUsage()=0;
	//virtual void			Step()=0;
};