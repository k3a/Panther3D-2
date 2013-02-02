/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once

#include "IObjectManager.h"

namespace P3D
{
	// function prototype for engine object creation
	typedef void* (*CreateObjFn)();

	/// Engine Object registration support class
	class EngineObjectReg
	{
	public:
		~EngineObjectReg()
		{
			delete[] ObjName;
		}
		EngineObjectReg(const wchar* objName, CreateObjFn fn);
	public:
		wchar* ObjName;
		CreateObjFn CreateFn;
		EngineObjectReg* Next;
		unsigned long InstNum; // number of calls to CreateObject
	};

	/// Object manager is responsible for creation and management of engine objects
	/** Engine object is special class, which is registered to ObjectManager. 
	Instance of such class can be created at run-time, for example while loading
	from file or using game editor. **/
	class CObjectManager : public IObjectManager
	{
	public:
		BaseObject* CreateObject(const wchar* objectName);
	};

	/// Macro for registering engine object
	/** Should be placed in .cpp file at the bottom before namespace end bracket "}". */
	#define REGISTER_OBJECT(name) \
	void* __engine_obj_reg_##name##_fn(){ \
	  name##::ObjectName.SetEx( _W(#name) ); \
	  return new name##(); \
	}; \
	static EngineObjectReg __engine_obj_reg_##name##( _W(#name) , __engine_obj_reg_##name##_fn )


}; // ns