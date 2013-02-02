/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once
#include "IScene.h"

namespace P3D {

class Scene : public IScene
{
public:
	Scene();

private:
	IntParam Version;
	IntParam NumObjects;
	FloatParam FloatTest;
	Vec3Param VectorTest;
	QuatParam QuatTest;
	StringParam StringTest;

	// array of serializable classes (params)
	ArrayParam<Vec3Param> Vec3Array;

	// map of serializable key (IntParam or StringParam) 
	// and arbitrary serializable values
	MapParam<StringParam, IntParam> MapTest;
};

}; // ns