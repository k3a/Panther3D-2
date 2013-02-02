/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#if defined(_DX) && _DX == 10

#include "ShaderHLSL.h"
#include "symbols.h"
#include "../Renderer.h"

namespace P3D
{

	extern ID3D10Device* g_pD3ddev;

	///Handles the includes in shaders
	class CIncludeHandler : public ID3D10Include
	{
	public:
		CIncludeHandler()
		{
			pFS = NULL;
			fp = NULL;
			pFS = CRenderer::mEngine()->mFilesystem();
		}

		STDMETHOD(Open)(THIS_ D3D10_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
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
		m_pGS = NULL;
		m_pPS = NULL;
		m_pVertDecl = NULL;
		m_pReflection = NULL;
	}

	CShaderHLSL::~CShaderHLSL()
	{
 		SAFE_RELEASE(m_pReflection);
 		SAFE_RELEASE(m_pVS);
 		SAFE_RELEASE(m_pGS);
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

		ID3D10Blob *pShaderBlob = NULL;
		ID3D10Blob *pErrors = NULL;
		UINT flags = D3D10_SHADER_DEBUG;  //D3D10_SHADER_OPTIMIZATION_LEVEL3

		char profile[128];
		switch(desc.ShaderType)
		{
		case SHADERTYPE_VERTEX_SHADER:
			strcpy(profile, D3D10GetVertexShaderProfile(g_pD3ddev));
			break;
		case SHADERTYPE_GEOMETRY_SHADER:
			strcpy(profile, D3D10GetGeometryShaderProfile(g_pD3ddev));
			break;
		case SHADERTYPE_PIXEL_SHADER:
			strcpy(profile, D3D10GetPixelShaderProfile(g_pD3ddev));
			break;
		default:
			CON(MSG_ERR, _W("Chader creation failed. No apropriate ShaderType given."));
			return false;
		}

		CIncludeHandler includeHandler;
		
		D3D10_SHADER_MACRO Shader_Macros[] = 
		{
			{ "DX10", NULL },
			{ "SM4", NULL },
			NULL
		};

		if(!CheckHRResult(D3DX10CompileFromMemory(
			pData, 
			fsize, 
			NULL, 
			Shader_Macros, 
			&includeHandler, 
			_W2A(desc.EntryFunction.Get()), 
			profile, 
			flags,
			0,
			NULL,
			&pShaderBlob, 
			&pErrors,
			NULL
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
			shaderCreated = CheckHRResult(g_pD3ddev->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_pVS));
			break;
		case SHADERTYPE_GEOMETRY_SHADER:
			shaderCreated = CheckHRResult(g_pD3ddev->CreateGeometryShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_pGS));
			break;
		case SHADERTYPE_PIXEL_SHADER:
			shaderCreated = CheckHRResult(g_pD3ddev->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_pPS));
			break;
		}
		
		if(!shaderCreated)
		{
			CON(MSG_ERR, _W("Shader creation error"));
			return false;
		}

		//if(!CheckHRResult(D3DReflect((DWORD*)pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), __uuidof(ID3D10ShaderReflection), &m_pReflection)))
		if(!CheckHRResult(D3D10ReflectShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_pReflection)))
		{
			CON(MSG_ERR, _W("Could not create a Shader reflection"));
			return false;
		}
		//HRESULT D3DReflect(LPCVOID pSrcData, SIZE_T SrcDataSize, REFIID pInterface,	void **ppReflector);
		D3D10_SHADER_DESC shDesc;
		m_pReflection->GetDesc(&shDesc);
		m_numResources = shDesc.BoundResources; //not sure about this

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
			g_pD3ddev->VSSetShader(m_pVS);
			m_pVertDecl->Use();
		}
		else if(m_desc.ShaderType == SHADERTYPE_PIXEL_SHADER) g_pD3ddev->PSSetShader(m_pPS);
		else g_pD3ddev->GSSetShader(m_pGS);
	}

	ShaderHandle CShaderHLSL::GetHandle(const char *name)
	{
		D3D10_SHADER_INPUT_BIND_DESC desc;
		for(UINT i=0; i<m_numResources; i++)
		{
			m_pReflection->GetResourceBindingDesc(i, &desc);
			if( strcmp(name, desc.Name)==0 )
			{
				UINT p = desc.BindPoint;
				m_parameters.AddEx(p);
				return m_parameters.Size();
			}
		}

		return 0; //not found
	}

	void CShaderHLSL::SetBuffer(ShaderHandle handle, CShaderBuffer *buffer)
	{
		ID3D10Buffer *cbuff[1];
		cbuff[0] = (ID3D10Buffer*)buffer->GetInternalBuffer();
		if(m_desc.ShaderType == SHADERTYPE_VERTEX_SHADER) g_pD3ddev->VSSetConstantBuffers(0, 1, cbuff);
		else if(m_desc.ShaderType == SHADERTYPE_PIXEL_SHADER) g_pD3ddev->PSSetConstantBuffers(0, 1, cbuff);
		else g_pD3ddev->GSSetConstantBuffers(0, 1, cbuff);
	}

	void CShaderHLSL::SetTexture(P3D::ShaderHandle handle, ITexture *tex)
	{
		CTexture *ctex = (CTexture*)tex;
		ID3D10ShaderResourceView *srv[1];
		srv[0] = (ID3D10ShaderResourceView*)ctex->GetInternalTexture();
		ID3D10SamplerState *smp[1];
		smp[0] = NULL;

		if(m_desc.ShaderType == SHADERTYPE_VERTEX_SHADER) 
		{
			g_pD3ddev->VSSetShaderResources(m_parameters[handle-1], 1, srv);
			//g_pD3ddev->VSSetSamplers(0, 1, smp);
		}
		else if(m_desc.ShaderType == SHADERTYPE_PIXEL_SHADER)
		{
			g_pD3ddev->PSSetShaderResources(m_parameters[handle-1], 1, srv);
			//g_pD3ddev->PSSetSamplers(0, 1, smp);
		}
		else
		{
			g_pD3ddev->GSSetShaderResources(m_parameters[handle-1], 1, srv);
			//g_pD3ddev->GSSetSamplers(0, 1, smp);
		}
	}

}//end ns

#endif