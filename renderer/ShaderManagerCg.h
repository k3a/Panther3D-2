/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#include <Cg/cg.h>

#if defined(_DX) && _DX==9
#include <Cg/cgD3D9.h>
#elif defined(_OGL)
#include <Cg/cgGL.h>
#elif defined(_DX) && _DX==10
#include <Cg/cgD3D10.h>
#endif

//#include "Shader.h"
#include "IShaderManager.h"
#include <vector>

namespace P3D
{

	enum eShaderParamHandleType
	{
		P3D_SHADER_PARAM_HANDLE_FLOAT,
		P3D_SHADER_PARAM_HANDLE_FLOAT2,
		P3D_SHADER_PARAM_HANDLE_FLOAT3,
		P3D_SHADER_PARAM_HANDLE_FLOAT4,
		P3D_SHADER_PARAM_HANDLE_FLOAT4x4,
	};

	class CShaderManagerCg : public IShaderManager
	{
	public:
		CShaderManagerCg();
		~CShaderManagerCg();
		/** Creates the Shader manager
		\param *device If you're using D3D, pass d3ddevice otherwise pass NULL */
		bool Create(void *device);
		//IShader* CreateShader(sShaderDesc &desc);
		//void DeleteShader(IShader *shader);

		ShaderHandle CreateSharedParamHandle(eShaderParamHandleType handleType);
		ShaderHandle CreateSharedParamHandle(eShaderParamHandleType handleType, UINT numDimensions, const int *lenghts);
		bool DeleteSharedParamHandle(ShaderHandle handle, bool force = false);
		void* GetInternalSharedParam(ShaderHandle handle);
		inline const wchar* GetShaderProfile(eShaderType shaderType);

		void OnLostDevice();
		void OnResetDevice();

	private:

		static void IncludeCallback( CGcontext context, const char *filename );

		CGcontext m_context;
		CGprofile   m_vertexProfile; //The best vertex shader model (profile)
		CGprofile   m_pixelProfile; //The best pixel shader model (profile)
		tArray<CGparameter> m_sharedParams;
	};
};
