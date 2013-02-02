/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#include "Engine.h"
#include "ObjectManager.h"

namespace P3D
{
	static EngineObjectReg* g_first_obj=NULL; // linear list of game object registrations

	// ----------------------------------------------------------------------
	BaseObject* CObjectManager::CreateObject(const wchar* objectName)
	{
		EngineObjectReg* optr=g_first_obj;
		while(optr)
		{
			if ( !wstricmp(objectName, optr->ObjName) )
			{
				optr->InstNum++;
				CON(MSG_INFO, _W("ObjectManager: Creating %s object (%d)."), objectName, optr->InstNum );
				return (BaseObject*)optr->CreateFn();
			}
			optr = optr->Next;
		}
		CON(MSG_ERR, _W("ObjectManager: Failed to create %s object! Object not found."), objectName );
		return NULL;
	}

	EngineObjectReg::EngineObjectReg(const wchar* objName, CreateObjFn fn)
		: CreateFn(fn), InstNum(0)
	{
		assert(objName && objName[0] && objName[1] );

		unsigned int nameLen = wstrlen(objName);

		ObjName = new wchar[nameLen+1];
		if (objName[0] == _W('C') && isupper(objName[1]) )
			wstrcpy(ObjName, &objName[1]); // remove beginning C from names like CTexture
		else
			wstrcpy(ObjName, objName);

		Next = g_first_obj;
		g_first_obj = this;
	};

}; // ns