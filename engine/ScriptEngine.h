/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "IScriptEngine.h"
#include "ScriptDebugHostWindows.h"

namespace P3D{

class CScriptEngine : public IScriptEngine
{
public:
	CScriptEngine(void);
	~CScriptEngine(void);

	bool LoadScript(const wchar * path);
	bool LoadScriptString(const char* code, const char* moduleName=0, const char* sectionName="script");
	bool ExecuteString(const char* code, const char* moduleName=0);
	asIScriptEngine* GetVM()	{ return m_vm; }
	unsigned long GetMemoryUsage();

public: // non-iface methods
	bool Initialize();
	void Tick(); // must be called every frame to update scripts

private:
	void InitialiseThreadTable();
	void Step();
private:
	asIScriptEngine *m_vm;
};

}; // ns