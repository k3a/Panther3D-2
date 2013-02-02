/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#include "types.h"
#include "ITexture.h"

namespace P3D
{

	enum ShaderSource
	{
		P3DSS_FROM_FILE = 0,
		P3DSS_FROM_STRING
	};

	///A shader's type. 
	enum eShaderType
	{
		SHADERTYPE_VERTEX_SHADER = 0,
		SHADERTYPE_PIXEL_SHADER,
		SHADERTYPE_GEOMETRY_SHADER
	};

	///A shader's parameter handle. 
	typedef unsigned int ShaderHandle;

	struct sShaderDesc
	{
		//sShaderDesc() { VertexDescID = 0; };
		eShaderType ShaderType;
		Str ShaderName;
		Str ShaderFile;
		Str EntryFunction;
		UINT VertexDescID; //for Vertex Shaders only
	};

	///A shader.
	class IShader
	{
	public:
		///Binds the given shader and updates it's parameters. 
		virtual void Use()=0;
		///Returns a ShaderHandle. These handles are used while setting shader's parameters
		virtual ShaderHandle GetHandle(const char *name)=0;
		virtual void SetHandleValueVec3(ShaderHandle handle, const Vec3 *value)=0;
		virtual void SetHandleValueMatrix(ShaderHandle handle, const Matrix *matrix)=0;
	};
};
