/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#if defined(_DX) && _DX == 9

#include "ShaderHLSL.h"
#include "symbols.h"
#include "../Renderer.h"

namespace P3D
{

	extern LPDIRECT3DDEVICE9 g_pD3ddev;

	///Handles the includes in shaders
	class CIncludeHandler : public ID3DXInclude
	{
	public:
		CIncludeHandler()
		{
			pFS = NULL;
			fp = NULL;
			pFS = CRenderer::mEngine()->mFilesystem();
		}

		STDMETHOD(Open)(THIS_ D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
		{
			char *pData = NULL;
			ULONG fsize;
			wchar realfilename[512];
			wsprintf(realfilename, 511, _W("shaders/%s"), _A2W(pFileName)); 
			fp = pFS->Load(realfilename, (BYTE *&)pData, fsize, true);
			if(!fp) return E_FAIL;

			char *odata = new char[fsize];
			memcpy(odata, pData, fsize);

			//output
			*ppData = odata;
			*pBytes = fsize;

			//clean up
			pFS->UnLoad(fp, (BYTE *)pData);
			fp = NULL;

			return S_OK;
		}

		STDMETHOD(Close)(THIS_ LPCVOID pData)
		{
			char *data = (char*)pData;
			delete[] data;
			return S_OK;
		}

	private:
		IFileSystem* pFS;
		FSFILE *fp;
	};



	CShaderHLSL::CShaderHLSL()
	{
		m_pVS = NULL;
		m_pPS = NULL;
		m_pVertDecl = NULL;
	}

	CShaderHLSL::~CShaderHLSL()
	{
		SAFE_RELEASE(m_pVS);
		SAFE_RELEASE(m_pPS);
		SAFE_DELETE(m_pVertDecl);
	}

	bool CShaderHLSL::Create(P3D::sShaderDesc &desc)
	{
		const char *pData;
		ULONG fsize;
		IFileSystem* pFS = CRenderer::mEngine()->mFilesystem();
		wchar path[P3DMAX_PATH];
		wsprintf(path, P3DMAX_PATH-1, _W("shaders/%s.rshader"), desc.ShaderFile.Get());
		FSFILE *fp = pFS->Load(path, (BYTE *&)pData, fsize, true);
		if (!fp)
		{
			CON(MSG_ERR, _W("Can't open %s.hlsl shader file from data/shaders directory!"), desc.ShaderFile.Get());
			return false;
		}

		ID3DXBuffer *pShaderBlob = NULL;
		ID3DXBuffer *pErrors = NULL;
		DWORD flags = D3DXSHADER_DEBUG;  //D3DXSHADER_OPTIMIZATION_LEVEL3

		char profile[128];
		switch(desc.ShaderType)
		{
		case SHADERTYPE_VERTEX_SHADER:
			strcpy(profile, D3DXGetVertexShaderProfile(g_pD3ddev));
			break;
		case SHADERTYPE_PIXEL_SHADER:
			strcpy(profile, D3DXGetPixelShaderProfile(g_pD3ddev));
			break;
		case SHADERTYPE_GEOMETRY_SHADER:
			CON(MSG_ERR, _W("DX9 does not support geometry shaders."));
			return false;
		default:
			CON(MSG_ERR, _W("Chader creation failed. No apropriate ShaderType given."));
			return false;
		}

		CIncludeHandler includeHandler;
		
		D3DXMACRO Shader_Macros[] = 
		{
			{ "DX9", NULL },
			{ "SM3", NULL },
			NULL
		};

		if(FAILED(D3DXCompileShader(
			pData, 
			fsize, 
			Shader_Macros, 
			&includeHandler, 
			_W2A(desc.EntryFunction.Get()), 
			profile, 
			flags,
			&pShaderBlob, 
			&pErrors,
			&m_pConstTable
			)))
		{
			if(pErrors) CON(MSG_ERR, _W("%s"), _A2W((char*)pErrors->GetBufferPointer()));
			else CON(MSG_ERR, _W("Error description not given"));
			CON(MSG_ERR, _W("Shader %s could not be compiled"), desc.ShaderFile.Get());
			SAFE_RELEASE(pErrors);
			return false;
		}

		pFS->UnLoad(fp, (BYTE *)pData);

		//save to cache
		fp = pFS->Open(_W("cache/shaders/hlsl"), _W("wb"));
		const char* cs = (const char*)pShaderBlob->GetBufferPointer();
		pFS->Write(cs, 1, pShaderBlob->GetBufferSize(), fp);
		pFS->Close(fp);

		bool shaderCreated = false;
		switch(desc.ShaderType)
		{
		case SHADERTYPE_VERTEX_SHADER:
			shaderCreated = SUCCEEDED(g_pD3ddev->CreateVertexShader((const DWORD*)pShaderBlob->GetBufferPointer(), &m_pVS));
			break;
		case SHADERTYPE_PIXEL_SHADER:
			shaderCreated = SUCCEEDED(g_pD3ddev->CreatePixelShader((const DWORD*)pShaderBlob->GetBufferPointer(), &m_pPS));
			break;
		}

		if(!shaderCreated)
		{
			CON(MSG_ERR, _W("Shader creation error"));
			return false;
		}

		//set constant to their default values 
		m_pConstTable->SetDefaults(g_pD3ddev);

		//create vertex declaration
		if(desc.ShaderType == SHADERTYPE_VERTEX_SHADER)
			m_pVertDecl = new CVertexDeclaration(CRenderer::cGraphicsManager()->GetVertexDescByID(desc.VertexDescID), pShaderBlob);

		SAFE_RELEASE(pShaderBlob);

		m_desc = desc;

		CON(MSG_INFO, _W("Shader '%s' created"), desc.ShaderFile);

		return true;
	}

	void CShaderHLSL::Use()
	{
		if(m_desc.ShaderType == SHADERTYPE_VERTEX_SHADER) 
		{
			g_pD3ddev->SetVertexShader(m_pVS);
			m_pVertDecl->Use();
		}
		else g_pD3ddev->SetPixelShader(m_pPS);
	}

	ShaderHandle CShaderHLSL::GetHandle(const char *name)
	{
		D3DXHANDLE handle = m_pConstTable->GetConstantByName(NULL, name);
		m_parameters.AddEx(handle);
		return m_parameters.Size();
		//return 0; //not found
	}

	void CShaderHLSL::SetBuffer(ShaderHandle handle, CShaderBuffer *buffer)
	{
		m_pConstTable->SetValue(g_pD3ddev, m_parameters[handle-1], buffer->GetInternalBuffer(), buffer->GetDesc().BufferSize);
	}

	void CShaderHLSL::SetTexture(P3D::ShaderHandle handle, ITexture *tex)
	{
// 		D3DXCONSTANT_DESC desc;
// 		UINT count;
// 		CTexture *ctex = (CTexture*)tex;
// 		m_pConstTable->GetConstantDesc(m_parameters[handle-1], &desc, &count);
// 		g_pD3ddev->SetTexture(desc.RegisterIndex, (IDirect3DBaseTexture9*)ctex->GetInternalTexture());
	}

}//end ns

#endif