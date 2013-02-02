/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#ifdef _CG

#include "ShaderManagerCg.h"
#include "Renderer.h"

namespace P3D
{

	CGcontext *g_pcgcontext = 0;
	CGprofile   g_vertexProfile;
	CGprofile   g_pixelProfile;

	void MyCGErrorCallback() 
	{
		//KEX: I think it isn't useful. You should rather test returned values and get appropriate error listings
		CGerror myError = cgGetError();
		CON(MSG_ERR, _W("CG Error: %s"), _A2W(cgGetErrorString(myError)));
	}


	CShaderManagerCg::CShaderManagerCg()
	{

	}

	CShaderManagerCg::~CShaderManagerCg()
	{
		#if defined(_OGL)
		cgGLDisableProfile(m_vertexProfile);
		cgGLDisableProfile(m_pixelProfile);
		#endif

		//destroy shared parameters
		for(unsigned int i=0; i<m_sharedParams.Size(); i++)
		{
			cgDestroyParameter(m_sharedParams[i]);
		}

		#if defined(_DX) && _DX==9
		cgD3D9SetDevice(0);
		#elif defined(_DX) && _DX==10
		cgD3D10SetDevice(m_context, 0);
		#endif

		cgDestroyContext(m_context);
	}

	bool CShaderManagerCg::Create(void *device)
	{
		CON(MSG_INFO, _W("Using Cg version %s."), _A2W(cgGetString(CG_VERSION)) );

		m_context = cgCreateContext();
		if(!cgIsContext(m_context)) CON(MSG_ERR_FATAL, _W("Could not create CGcontext!"));

		cgSetErrorCallback(MyCGErrorCallback);
		cgSetCompilerIncludeCallback( m_context, IncludeCallback );

		g_pcgcontext = &m_context;

#if defined(_DX) && _DX==9
		if(device) cgD3D9SetDevice((IDirect3DDevice9*)device);
		else CON(MSG_ERR_FATAL, _W("Pointer to IDirect3DDevice9 is NULL!"));
#elif defined(_DX) && _DX==10
		if(device) cgD3D10SetDevice(m_context, (ID3D10Device*)device);
		else CON(MSG_ERR_FATAL, _W("Pointer to ID3D10Device is NULL!"));
#endif

		cgSetParameterSettingMode(m_context, CG_DEFERRED_PARAMETER_SETTING);

#if defined(_OGL)
		cgGLSetDebugMode( CG_FALSE );
		m_vertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
		m_pixelProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
#elif defined(_DX) && _DX==9
		m_vertexProfile = cgD3D9GetLatestVertexProfile();
		m_pixelProfile =  cgD3D9GetLatestPixelProfile();
#elif defined(_DX) && _DX==10
		m_vertexProfile = cgD3D10GetLatestVertexProfile();
		m_pixelProfile = cgD3D10GetLatestPixelProfile();
#endif

		if(m_vertexProfile == CG_PROFILE_UNKNOWN || m_pixelProfile == CG_PROFILE_UNKNOWN) CON(MSG_ERR_FATAL, _W("No appropriate shader profile is available!"));
		//cgGLSetOptimalOptions(m_vertexProfile);
		CON(MSG_INFO, _W(" ...using %s vertex shader profile"), _A2W(cgGetProfileString(m_vertexProfile)));
		CON(MSG_INFO, _W(" ...using %s pixel shader profile"), _A2W(cgGetProfileString(m_pixelProfile)));
		g_vertexProfile = m_vertexProfile;
		g_pixelProfile = m_pixelProfile;

#if defined(_OGL)
		cgGLEnableProfile(m_vertexProfile);
		cgGLEnableProfile(m_pixelProfile);
		cgGLSetManageTextureParameters(m_context, CG_TRUE);
#elif defined(_DX) && _DX==9
		cgD3D9SetManageTextureParameters(m_context, CG_TRUE);
#elif defined(_DX) && _DX==10
		cgD3D10SetManageTextureParameters(m_context, CG_TRUE);
#endif

		return true;
	}

	

	void CShaderManagerCg::OnLostDevice()
	{
		
	}

	void CShaderManagerCg::OnResetDevice()
	{
		
	}

	ShaderHandle CShaderManagerCg::CreateSharedParamHandle(P3D::eShaderParamHandleType handleType)
	{
		CGtype type;
		switch(handleType)
		{
		case P3D_SHADER_PARAM_HANDLE_FLOAT:
			type = CG_FLOAT;
			break;
		case P3D_SHADER_PARAM_HANDLE_FLOAT2:
			type = CG_FLOAT2;
			break;
		case P3D_SHADER_PARAM_HANDLE_FLOAT3:
			type = CG_FLOAT3;
			break;
		case P3D_SHADER_PARAM_HANDLE_FLOAT4:
			type = CG_FLOAT4;
			break;
		case P3D_SHADER_PARAM_HANDLE_FLOAT4x4:
			type = CG_FLOAT4x4;
			break;
		default:
			CON(MSG_ERR, _W("SharedParamHandle creation failed. Invalid hndleType given."));
			return 0;
		}

		CGparameter parameter = cgCreateParameter(m_context, type);
		if(!parameter) 
		{
			CON(MSG_ERR, _W("SharedParamHandle creation failed. Invalid hndleType given."));
			return 0;
		}

		m_sharedParams.AddEx(parameter);
		return m_sharedParams.Size();
	}

	ShaderHandle CShaderManagerCg::CreateSharedParamHandle(eShaderParamHandleType handleType, UINT numDimensions, const int *lenghts)
	{
		CGtype type;
		switch(handleType)
		{
		case P3D_SHADER_PARAM_HANDLE_FLOAT:
			type = CG_FLOAT;
			break;
		default:
			CON(MSG_ERR, _W("SharedParamHandle creation failed. Invalid hndleType given."));
			return 0;
		}

		CGparameter parameter = cgCreateParameterMultiDimArray(m_context, type, numDimensions, lenghts);
		if(!parameter) 
		{
			CON(MSG_ERR, _W("SharedParamHandle creation failed. Invalid hndleType given."));
			return 0;
		}

		m_sharedParams.AddEx(parameter);
		return m_sharedParams.Size();
	}

	bool CShaderManagerCg::DeleteSharedParamHandle(ShaderHandle handle, bool force)
	{
		if(cgGetNumConnectedToParameters(m_sharedParams[handle - 1]) && !force)  return false;
		else cgDestroyParameter(m_sharedParams[handle - 1]);
		return true;
	}

	void* CShaderManagerCg::GetInternalSharedParam(P3D::ShaderHandle handle)
	{
		//assert should be included in the tArray class
		return m_sharedParams[handle - 1];
	}

	inline const wchar* CShaderManagerCg::GetShaderProfile(P3D::eShaderType shaderType)
	{
		switch(shaderType)
		{
		case SHADERTYPE_VERTEX_SHADER:
			return _A2W(cgGetProfileString(m_vertexProfile));
			break;
		case SHADERTYPE_PIXEL_SHADER:
			return _A2W(cgGetProfileString(m_pixelProfile));
			break;
		}
	}

	void CShaderManagerCg::IncludeCallback( CGcontext context, const char *filename )
	{
		IFileSystem* pFS = CRenderer::mEngine()->mFilesystem();
		char *pData = NULL;
		ULONG fsize;
		wchar realfilename[512];
		wsprintf(realfilename, 511, _W("shaders%s"), _A2W(filename)); 
		FSFILE *fp = pFS->Load(realfilename, (BYTE *&)pData, fsize, true);
		//cgSetCompilerIncludeFile( context, filename, realfilename );
		cgSetCompilerIncludeString(context, filename, pData);
		pFS->UnLoad(fp, (BYTE *)pData);
	}


};

#endif
