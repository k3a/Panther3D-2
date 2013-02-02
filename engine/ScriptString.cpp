/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"
#include "ScriptString.h"

namespace P3D {

	//----------------
	// String init & factories
	// ---------------

	CScriptString* CScriptString::DefaultFactory()
	{
		return new CScriptString();
	}

	CScriptString* CScriptString::CopyFactory(const CScriptString& other)
	{
		return new CScriptString(other);
	}

	CScriptString *StringFactory(asUINT length, const char *s)
	{
		return new CScriptString( s, length);
	}

	// ---------------------------------------------------------------------------------------------

	//-----------------
	// String + String
	//-----------------

	CScriptString *CScriptString::operator+(const CScriptString &other) const
	{
		// Return a new object as a script handle
		CScriptString *cs = new CScriptString(*this);
		cs->AppendEx(other);
		return cs;
	}

	//----------------
	// String + value
	//----------------

	CScriptString *CScriptString::operator +(const unsigned int i) const
	{
		wchar buf[64];
		wsprintf(buf, 63, _W("%u"), i);
		CScriptString *cs = new CScriptString(*this);
		cs->AppendEx(buf);
		return cs;
	}

	CScriptString *CScriptString::operator +(const int i) const
	{
		wchar buf[64];
		wsprintf(buf, 63, _W("%d"), i);
		CScriptString *cs = new CScriptString(*this);
		cs->AppendEx(buf);
		return cs;
	}

	CScriptString *CScriptString::operator +(const float f) const
	{
		wchar buf[64];
		wsprintf(buf, 63, _W("%g"), f);
		CScriptString *cs = new CScriptString(*this);
		cs->AppendEx(buf);
		return cs;
	}

	CScriptString *CScriptString::operator +(const bool b) const
	{
		wchar buf[6]=_W("False");
		if (b) wstrcpy(buf, _W("True"));
		
		CScriptString *cs = new CScriptString(*this);
		cs->AppendEx(buf);
		return cs;
	}
/*
	CScriptString *CScriptString::operator +(const double d) const
	{
		wchar buf[64];
		wsprintf(buf, 63, _W("%g"), d);
		CScriptString *cs = new CScriptString(*this);
		cs->AppendEx(buf);
		return cs;
	}*/

	//----------------
	// value + String
	//----------------

	static CScriptString *AddIntString(int i, const CScriptString &str)
	{
		wchar buf[64];
		wsprintf(buf, 63, _W("%d"), i);
		CScriptString *cs = new CScriptString(str);
		cs->AppendEx(buf);
		return cs;
	}

	static CScriptString *AddUIntString(unsigned int i, const CScriptString &str)
	{
		wchar buf[64];
		wsprintf(buf, 63, _W("%u"), i);
		CScriptString *cs = new CScriptString(str);
		cs->AppendEx(buf);
		return cs;
	}

	static CScriptString *AddFloatString(float f, const CScriptString &str)
	{
		wchar buf[64];
		wsprintf(buf, 63, _W("%g"), f);
		CScriptString *cs = new CScriptString(str);
		cs->AppendEx(buf);
		return cs;
	}

	static CScriptString *AddDoubleString(double f, const CScriptString &str)
	{
		wchar buf[64];
		wsprintf(buf, 63, _W("%g"), f);
		CScriptString *cs = new CScriptString(str);
		cs->AppendEx(buf);
		return cs;
	}

	//----------
	// String[]
	//----------

	/*static char *StringCharAt(unsigned int i, CScriptString &str)
	{
	if( i >= str.GetNumChars() )
	{
	// Set a script exception
	asIScriptContext *ctx = asGetActiveContext();
	ctx->SetException("Out of range");

	// Return a null pointer
	return 0;
	}

	return &str[i];
	}*/

	// ---------------------------------------------------------------------------------------------

	void RegisterStringType(asIScriptEngine *engine)
	{
		int r;

		// Register the type
		r = engine->RegisterObjectType("String", sizeof(CScriptString), asOBJ_REF); assert( r >= 0 );

		// Register the object operator overloads
		// Note: We don't have to register the destructor, since the object uses reference counting
		r = engine->RegisterObjectBehaviour("String", asBEHAVE_FACTORY, "String @f()", 
			asFUNCTION(CScriptString::DefaultFactory), asCALL_CDECL); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("String", asBEHAVE_FACTORY, "String @f(const String &in)",
			asFUNCTION(CScriptString::CopyFactory), asCALL_CDECL); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("String", asBEHAVE_ADDREF, "void f()",
			asMETHOD(CScriptString,AddRef), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("String", asBEHAVE_RELEASE, "void f()",
			asMETHOD(CScriptString,Release), asCALL_THISCALL); assert( r >= 0 );
		//r = engine->RegisterObjectBehaviour("String", asBEHAVE_ADD_ASSIGN, "String &f(const String &in)",
		//	asMETHODPR(CScriptString, operator+=, (const CScriptString&), CScriptString&), asCALL_THISCALL); assert( r >= 0 );

		// Register the factory to return a handle to a new String
		// Note: We must register the String factory after the basic behaviours,
		// otherwise the library will not allow the use of object handles for this type
		r = engine->RegisterStringFactory("String@", asFUNCTION(StringFactory), asCALL_CDECL); assert( r >= 0 );

		// Register the global operator overloads
		r = engine->RegisterObjectMethod("String", "bool opEquals(const String &other)",    asMETHODPR(Str, operator==, (const Str& other), bool), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "int opCmp(const String &in) const", asMETHOD(Str, Compare), asCALL_THISCALL); assert( r >= 0 );

		r = engine->RegisterObjectMethod("String", "String@ opAdd(const String &in)", asMETHODPR(CScriptString, operator +, (const CScriptString &) const, CScriptString*), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "String &opAddAssign(const String &in)", asMETHODPR(Str, operator+=, (const Str&), Str&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "String &opAssign(const String &in)", asMETHODPR(Str, operator =, (const Str&), Str&), asCALL_THISCALL); assert( r >= 0 );

		
		/*
		// Register the index operator, both as a mutator and as an inspector
		//r = engine->RegisterObjectBehaviour("String", asBEHAVE_INDEX, "uint8 &f(uint)", asFUNCTION(StringCharAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		//r = engine->RegisterObjectBehaviour("String", asBEHAVE_INDEX, "const uint8 &f(uint) const", asFUNCTION(StringCharAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		/*
		// Register the object methods
		if( sizeof(size_t) == 4 )
		{
		r = engine->RegisterObjectMethod("String", "uint length() const", asMETHOD(String,size), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "void resize(uint)", asMETHODPR(String,resize,(size_t),void), asCALL_THISCALL); assert( r >= 0 );
		}
		else
		{
		r = engine->RegisterObjectMethod("String", "uint64 length() const", asMETHOD(String,size), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "void resize(uint64)", asMETHODPR(String,resize,(size_t),void), asCALL_THISCALL); assert( r >= 0 );
		}
		*/

		// index operator
		// r = engine->RegisterObjectBehaviour("String", asBEHAVE_INDEX, "uint8 &f(uint)", asMETHOD(CScriptString, CharAt), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("String", asBEHAVE_INDEX, "const uint8 &f(uint) const", asMETHOD(CScriptString, CharAt), asCALL_THISCALL); assert( r >= 0 );

		// first string character to uint8 (one byte ordinal code)
		r = engine->RegisterObjectBehaviour("String", asBEHAVE_VALUE_CAST, "const uint f() const", asMETHOD(CScriptString, GetOrdinalCode), asCALL_THISCALL); assert( r >= 0 );

		// String helper methods
		r = engine->RegisterObjectMethod("String", "uint length()", asMETHOD(Str, GetNumChars), asCALL_THISCALL); assert( r >= 0 );

		// Automatic conversion from values
		r = engine->RegisterObjectMethod("String", "String &opAssign(bool)", asMETHODPR(Str, operator=, (const bool), Str&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "String &opAddAssign(bool)", asMETHODPR(Str, operator+=, (const bool), Str&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "String@ opAdd(bool) const", asMETHODPR(CScriptString, operator+, (const bool) const, CScriptString*), asCALL_THISCALL); assert( r >= 0 );
		//r = engine->RegisterObjectMethod("String", "String@ opAdd_r(bool) const", asFUNCTION(AddBoolString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("String", "String &opAssign(float)", asMETHODPR(Str, operator=, (const float), Str&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "String &opAddAssign(float)", asMETHODPR(Str, operator+=, (const float), Str&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "String@ opAdd(float) const", asMETHODPR(CScriptString, operator+, (const float) const, CScriptString*), asCALL_THISCALL); assert( r >= 0 );
		//r = engine->RegisterObjectMethod("String", "String@ opAdd_r(float) const", asFUNCTION(AddFloatString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("String", "String &opAssign(int)", asMETHODPR(Str, operator=, (const int), Str&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "String &opAddAssign(int)", asMETHODPR(Str, operator+=, (const int), Str&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "String@ opAdd(int) const", asMETHODPR(CScriptString, operator+, (const int) const, CScriptString*), asCALL_THISCALL); assert( r >= 0 );
		//r = engine->RegisterObjectMethod("String", "String@ opAdd_r(int) const", asFUNCTION(AddIntString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("String", "String &opAssign(uint)", asMETHODPR(Str, operator=, (const unsigned int), Str&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "String &opAddAssign(uint)", asMETHODPR(Str, operator+=, (const unsigned int), Str&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("String", "String@ opAdd(uint) const", asMETHODPR(CScriptString, operator+, (const unsigned int) const, CScriptString*), asCALL_THISCALL); assert( r >= 0 );
		//r = engine->RegisterObjectMethod("String", "String@ opAdd_r(uint) const", asFUNCTION(AddUIntString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	}



}; //ns