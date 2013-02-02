/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once

#include "BaseObject.h"

namespace P3D
{

class Material : public BaseObject
{
public:
	Material()
	{
		BaseShader.SetEx(_W("DefaultDeferred"));
		REGISTER_PARAM(BaseShader, "Base shader used for rendering material");

		Deferred = true;
		REGISTER_PARAM(Deferred, "Material used for deferred rendering");

		Transparent = false;
		REGISTER_PARAM(Transparent, "Material is partially transparent and needs sorting for alpha blending");

		vsPtr = psPtr = 0;
	}

	StringParam BaseShader;
	BoolParam Deferred;
	BoolParam Transparent;

	// --- private data for caching
	void* vsPtr; // cached vertex shader
	void* psPtr; // cached pixel shader
};

/// Material instance is used to override Material default parameters
/** This class is set to geometry subset and all its parameters is set before rendering. */
struct MaterialInstance : public BaseObject
{
	MaterialInstance()
	{
		REGISTER_PARAM(FloatParams, "Float parameters");
		REGISTER_PARAM(Vec3Params, "Vec3 parameters");
		//REGISTER_PARAM(TexParams, "Texture parameters");

		material = 0;
		data = 0;
	}
	~MaterialInstance()
	{
		if (data) delete data;
	}
public:
	MapParam<StringParam, FloatParam> FloatParams;
	MapParam<StringParam, Vec3Param> Vec3Params;
	//MapParam<StringParam, Texture> TexParams;

	// --- private data for internal purposes
	Material* material; // material for which is this instance set
	void* data; // data for constant buffer
};

}; // ns