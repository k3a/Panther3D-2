/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "ScriptRegistrations.h"
#include "Engine.h"

#include "ScriptString.h"
#include "ScriptMath.h"

namespace P3D{

static void ScriptLog(const CScriptString & str)
{
	CON(MSG_INFO, _W(": %s"), str.Get() );
} 

static void ScriptLoad(const char *scriptpath)
{
	wchar path[P3DMAX_PATH];
	wsprintf(path, P3DMAX_PATH-1, _W("scripts/%s"), _A2W(scriptpath));
	CEngine::cScriptEngine()->LoadScript(path);
}

static void ScriptMemoryUsage()
{
	CON(MSG_INFO, _W("MEMORY USAGE INFO") );
	CON(MSG_INFO, _W(" Script: %d bytes"), CEngine::cScriptEngine()->GetMemoryUsage() );
	//CON(MSG_INFO, _W(""));
}

// --------------------------------------------------------

void RegisterBasicTypesToScript(asIScriptEngine * VM)
{
	RegisterStringType(VM);
	RegisterMath(VM);

	int r;

	// register common functions
	r = VM->RegisterGlobalFunction("void Log(String &str)", asFUNCTION(ScriptLog), asCALL_CDECL); assert( r >= 0 );
	r = VM->RegisterGlobalFunction("void log(String &str)", asFUNCTION(ScriptLog), asCALL_CDECL); assert( r >= 0 );

	/*using namespace luabind;

	module(L)
		[
			// global core functions
			def("Log", (void(*)(const char*))&ScriptLog),
			def("print", (void(*)(const char*))&ScriptLog),
			def("LoadScript", &ScriptLoad),
			def("GetLUAMem", &ScriptMemory),
			def("MemoryUsage", &ScriptMemoryUsage),

			// CTest
			class_<CTest>("Test")
				.def(constructor<>()),

			// Vec2


			// Vec3
			class_<Vec3>("Vec3")
				.def(constructor<>())
				.def(constructor<float, float, float>())
				// members
				.def_readwrite("x", &Vec3::x)
				.def_readwrite("y", &Vec3::y)
				.def_readwrite("z", &Vec3::z)
				// named functions
				.def("Len", &Vec3::Length)
				.def("LenSqr", &Vec3::LengthSqr)
				.def("Cross", &Vec3::Cross)
				.def("Dot", &Vec3::Dot)
				.def("Assign", &Vec3::Assign)
				.def("Abs", &Vec3::Abs)
				.def("Normalize", &Vec3::Normalize)
				.def("GetMax", &Vec3::GetMax)
				.def("GetMin", &Vec3::GetMin)
				.def("RotX", &Vec3::RotX)
				.def("RotY", &Vec3::RotY)
				.def("RotZ", &Vec3::RotZ)
				// operators
				.def(self + Vec3())
				.def(self - Vec3())
				//.def(self * Vec3) // DOT PRODUCT!!
				.def( - self)
				.def(self + float())
				.def(self - float())
				.def(self * float())
				.def(self / float())
				.def(self == Vec3())
				.def(self < Vec3())
				.def(self <= Vec3())
				// to string
				.def( tostring(const_self) )
		];*/
}


}; // ns