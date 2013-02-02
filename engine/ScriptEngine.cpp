/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"
#include "ScriptEngine.h"
#include "ScriptRegistrations.h"

#include "Engine.h"

namespace P3D{

// TODO: if clicking on error message is not working within VisualStudio, 
//       make second version using #ifdef _DEBUG && MSVC with OutputDebugString
//       or remove : from the begining of line
void ScriptMessageCallback(const asSMessageInfo *msg, void *param)
{
	const wchar *type = _W("ERR ");
	if( msg->type == asMSGTYPE_WARNING ) 
		type = _W("WARN");
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = _W("INFO");

	CON(MSG_INFO, _W(": %s (%d, %d) : %s : %s\n"), _A2W(msg->section), msg->row, msg->col, type, _A2W(msg->message) );
}


CScriptEngine::CScriptEngine(void)
: m_vm(NULL)
{

}

bool CScriptEngine::Initialize()
{
	// register custom memory management 
	//asSetGlobalMemoryFunctions()

	m_vm = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	m_vm->SetEngineProperty(asEP_USE_CHARACTER_LITERALS, true); // treat 'a' as character literal of type uint8
	if( m_vm == 0 )
	{
		CON( MSG_ERR, _W("Failed to create script engine!") );
		return false;
	}

	// set compiler message callback
	m_vm->SetMessageCallback(asFUNCTION(ScriptMessageCallback), 0, asCALL_CDECL);

	// write version info
	CON( MSG_INFO, _W(" AngelScript version %s successfuly initialized."), _A2W( asGetLibraryVersion() ) );
	CON( MSG_INFO, _W(" Build flags: %s"), _A2W( asGetLibraryOptions() ) );

	// register basic types
	RegisterBasicTypesToScript(m_vm);

	// Script UnitTest
	LoadScriptString(\
		"void UnitTest()\n"\
		"{\n"\
		"Log(\"--- Script engine unit test ---\");\n"\

		"\n"\
		"Log( \"String comparison: \" + ( (\"str\"==\"str\") ? \"OK\" : \"ERROR\" ) );\n"\

		"\n"\
		"String float2str = 0.123f;\n"\
		"Log(\"Float-to-String: \" + float2str);\n"\
		"String strTrue = true; String strFalse = false;\n"\
		"Log(\"Bool-to-String: \" + ( (strTrue == \"True\") && (strFalse == \"False\") ? \"OK\" : \"ERROR\" ) );\n"\
		"Log(\"String index operator: \" + ( (\"str\"[2]=='r') ? \"OK\" : \"ERROR\") );\n"\
		"Log(\"String length(): \" + ( (\"str\".length()==3) ? \"OK\" : \"ERROR\") );\n"\

		"\n"\
		"float f = 0.123f;\n"\
		"float f2 = sin(f);\n"\
		"Log(\"sin(\" + f + \") = \" + f2 );\n"\

		"\n"\
		"Log(\"rand(0.1f, 0.7f) = \" + rand(0.1f, 0.7f) );\n"\
		"}\n"\
	,0,"UnitTestSection");

	ExecuteString("UnitTest();");

	return true;
}

CScriptEngine::~CScriptEngine(void)
{
	if (m_vm)
	{
		m_vm->Release();
	}
}

void CScriptEngine::Step()
{
	
}

bool CScriptEngine::LoadScript( const wchar * path )
{
	IFileSystem* fs = CEngine::instance()->mFilesystem();

	CON(MSG_INFO, _W("ScriptEngine: Loading script file %s"), path);

	// load using FS
	BYTE* pData=NULL; unsigned long fSize=0;
	FSFILE* fp = fs->Load(path, pData, fSize, true);
	if (!fp)
	{
		CON(MSG_ERR, _W("Faile to open script file %s!"), path);
		return false;
	}

	if (!LoadScriptString( (const char*)pData, 0, _W2A(path) ))
	{
		CON(MSG_ERR, _W("Error loading %s script!"), path );
		fs->UnLoad(fp, pData);
		return false;
	}
	
	fs->UnLoad(fp, pData);
	return true;
}

class CDebugBinStream : public asIBinaryStream
{
public:
	CDebugBinStream(const wchar* path, bool write)
	{
		if (write)
			m_fp = wfopen(path, _W("wb"));
		else
			m_fp = wfopen(path, _W("rb"));
	}

	~CDebugBinStream()
	{
		fclose(m_fp);
	}

	void Read (void *ptr, asUINT size)
	{
		fread(ptr, 1, size, m_fp);
	}

	void Write (const void *ptr, asUINT size)
	{
		//asSBCInfo bcinfo = asBCInfo[*((asBYTE*)ptr)];
		//CON(MSG_INFO, _W("%s"), _A2W(bcinfo.name) );
		fwrite(ptr, 1, size, m_fp);
	}
private:
	FILE* m_fp;
};

bool CScriptEngine::LoadScriptString(const char* code, const char* moduleName, const char* sectionName)
{
	assert(code && *code);
	
	asIScriptModule *mod = m_vm->GetModule(moduleName, asGM_ALWAYS_CREATE);
	int r = mod->AddScriptSection(sectionName, code);
	if( r < 0 ) 
	{
		CON(MSG_ERR, _W("AddScriptSection() failed!") );
		return false;
	}

	// Compile the script. If there are any compiler messages they will
	// be written to the message stream that we set right after creating the 
	// script engine. If there are no errors, and no warnings, nothing will
	// be written to the stream.
	r = mod->Build();
	if( r < 0 )
	{
		CON(MSG_ERR, _W("Script build failed!") );
		return false;
	}

	CDebugBinStream bs( _W("compiled_script.txt"), true );
	mod->SaveByteCode( &bs );

	return true;
}

bool CScriptEngine::ExecuteString(const char* code, const char* moduleName)
{
	assert(code && *code);

	return m_vm->ExecuteString(moduleName, code)>=0;
}

void CScriptEngine::Tick()
{

}

unsigned long CScriptEngine::GetMemoryUsage()
{
	return 0; // TODO
}

}; // ns