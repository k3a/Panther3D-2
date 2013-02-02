/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "Engine.h"
#include "Scene.h"

namespace P3D {

Scene::Scene()
{

	Version = 1;
	REGISTER_PARAM(Version, "Scene format version");

	NumObjects = 123;
	REGISTER_PARAM(NumObjects, "Number of scene objects");

	FloatTest = 123.321f;
	REGISTER_PARAM(FloatTest, "");

	VectorTest = Vec3(11.22f, 22.33f, 33.44f);
	REGISTER_PARAM(VectorTest, "");

	QuatTest = Quat(11.22f, 22.33f, 33.44f, 44.55f);
	REGISTER_PARAM(QuatTest, "");

	StringTest = _W("Testovaci string \"uvozovky\" a novy radek \n :) Unicode? Yeah! \x3243");
	REGISTER_PARAM(StringTest, "");

	Vec3Array.AddEx(Vec3Param(1,2,3));
	Vec3Array.AddEx(Vec3Param(4,5,6));
	Vec3Array.AddEx(Vec3Param(7,8,9));
	REGISTER_PARAM(Vec3Array, "");

	MapTest.AddEx(_W("test"), 1);
	MapTest.AddEx("testAnsi", 2);
	MapTest.AddEx(StringTest, 3);
	REGISTER_PARAM(MapTest, "Testovaci mapa s klici a hodnotami");
}

REGISTER_OBJECT(Scene);

}; // ns