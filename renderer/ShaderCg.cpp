/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#ifdef _CG

#include "ShaderCg.h"
#include "Renderer.h"

namespace P3D
{

	extern CGcontext *g_pcgcontext;
	extern CGprofile   g_vertexProfile;
	extern CGprofile   g_pixelProfile;

	CShaderCg::CShaderCg()
	{
		m_arrayIndex = 0;
		m_compiled = false;
		m_pVertDecl = NULL;
	}

	CShaderCg::~CShaderCg()
	{

		for(unsigned int i = 0; i < m_parameters.Size(); i++)
		{
			//disconnect from the shared parameters
			if(cgGetConnectedParameter(m_parameters[i])) cgDisconnectParameter(m_parameters[i]);
			//and destroy
			cgDestroyParameter(m_parameters[i]);
		}

		SAFE_DELETE(m_pVertDecl);

		cgDestroyProgram(m_program);
	}

	bool CShaderCg::Create(sShaderDesc &desc)
	{
		m_compiled = false;
		IFileSystem* pFS = CRenderer::mEngine()->mFilesystem();

		CGprofile profile;
		if(desc.ShaderType == SHADERTYPE_VERTEX_SHADER) profile = g_vertexProfile;
		else if(desc.ShaderType == SHADERTYPE_PIXEL_SHADER) profile = g_pixelProfile;
		else 
		{
			CON(MSG_ERR, _W("Chader creation failed. No apropriate ShaderType given."));
			return false;
		}

		const char *pData = _W2A(desc.ShaderFile.Get());
		FSFILE* fp = NULL;
		ULONG fsize;

		// try to load cached data
		wchar cachefile[512];
		wsprintf(cachefile, 511, _W("cache/shaders/%s#%s#%d#%d"), desc.ShaderName.Get(), _A2W(cgGetProfileString(profile)), desc.ShaderFile.Hash2048(), CRenderer::cGraphicsDevice()->GetDeviceHash());
		//temporary commented by am!go 
		//fp = pFS->Load(cachefile, (BYTE *&)pData, fsize, true);

		bool bCreatedFromCache=false;
		if (fp) // try to create shader from cache
		{
#if defined(_OGL)
			cgGLSetOptimalOptions(profile);
			m_program = cgCreateProgram(*g_pcgcontext, CG_OBJECT, (char*)pData, profile, _W2A(desc.EntryFunction.Get()), NULL);
#elif defined(_DX) && _DX==9
			const char **profileOpts;
			profileOpts = cgD3D9GetOptimalOptions(profile);
			m_program = cgCreateProgram(*g_pcgcontext, CG_OBJECT, (char*)pData, profile, _W2A(desc.EntryFunction.Get()) , profileOpts);
#elif defined(_DX) && _DX==10
			const char **profileOpts = NULL;
			profileOpts = cgD3D10GetOptimalOptions(profile);
			m_program = cgCreateProgram(*g_pcgcontext, CG_OBJECT, (char*)pData, profile, _W2A(desc.EntryFunction.Get()) , profileOpts);
#endif

			if (m_program) 
			{
#if defined(_OGL)
				cgGLLoadProgram(m_program);
#elif defined(_DX) && _DX==9
				cgD3D9LoadProgram(m_program, false, 0);	//TODO: check HRESULT
#elif defined(_DX) && _DX==10
				CheckHRResult(cgD3D10LoadProgram(m_program, 0 ));
#endif
				bCreatedFromCache=true; 
				m_compiled=true;
			}

			pFS->UnLoad(fp, (BYTE *)pData); // close cached file
		}


		if (!fp || !m_compiled) // create, compile and insert to cache
		{
			// load shader using filesystem
			//if(location == P3DSS_FROM_FILE)
			//{
			wchar path[P3DMAX_PATH];
			//wsprintf(path, P3DMAX_PATH-1, _W("shaders/%s"), desc.ShaderFile.Get());
			wsprintf(path, P3DMAX_PATH-1, _W("shaders/%s.cg"), desc.ShaderFile.Get());
			fp = pFS->Load(path, (BYTE *&)pData, fsize, true);
			if (!fp)
			{
				CON(MSG_ERR, _W("Can't open %s shader file from data/shaders directory!"), desc.ShaderFile.Get());
				return false;
			}
			//}

			const char *attr[] =
			{
				"-DCG",
				"-DSM4",
				NULL
			};

#if defined(_OGL)
			cgGLSetOptimalOptions(profile);
			m_program = cgCreateProgram(*g_pcgcontext, CG_SOURCE, (char*)pData, profile, _W2A(desc.EntryFunction.Get()), NULL);
#elif defined(_DX) && _DX==9
			const char **profileOpts;
			profileOpts = cgD3D9GetOptimalOptions(profile);
			m_program = cgCreateProgram(*g_pcgcontext, CG_SOURCE, (char*)pData, profile, _W2A(desc.EntryFunction.Get()), profileOpts);
#elif defined(_DX) && _DX==10
			const char **profileOpts = NULL;
			profileOpts = cgD3D10GetOptimalOptions(profile);
			//m_program = cgCreateProgram(*g_pcgcontext, CG_SOURCE, (char*)pData, profile, _W2A(desc.EntryFunction.Get()) , profileOpts);
			m_program = cgCreateProgram(*g_pcgcontext, CG_SOURCE, (char*)pData, profile, _W2A(desc.EntryFunction.Get()) , attr);
#endif

			/*if (location==P3DSS_FROM_FILE)*/ pFS->UnLoad(fp, (BYTE *)pData);

			if (!m_program)
			{
				const char* err = cgGetLastListing(*g_pcgcontext);
				if (err && *err)
				{
					// output error to console
					CON(MSG_ERR, _W("%s"), _A2W(err) );
				}
				return false;
			}


#if defined(_OGL)
			cgGLLoadProgram(m_program);
#elif defined(_DX) && _DX==9
			cgD3D9LoadProgram(m_program, false, 0); //TODO: check HRESULT
#elif defined(_DX) && _DX==10
			CheckHRResult(cgD3D10LoadProgram(m_program, 0 ));
#endif

			m_compiled = true;

			// write compiled shader to cache
			FSFILE* fp = pFS->Open(cachefile, _W("wb"));
			const char* cs = GetCompiled();
			pFS->Write(cs, 1, strlen(cs), fp);
			pFS->Close(fp);
		}

		m_compiled = true;

		// get information about compiled string
		const char* strc = GetCompiled(); 
		strcpy(m_profile, "unknown"); m_instr=0;
		if (strc && *strc && *(strc+1) && *(strc+2) )
			if (strc[0]=='/') // DX10 CG version - hlsl10 output
			{
				for (unsigned int i=3; i<strlen(strc)-2; i++)
				{
					if ( strc[i] == ' ' || strc[i] == '\r' || strc[i] == '\n' || strc[i] == 0 )
					{
						strncpy(m_profile, &strc[3], (i-3)<8?(i-3):7);
						break;
					}
				}
				// TODO: get number of asm instr.
			}
			else // DX9/OGL CG version - assembler output
			{
				for (unsigned int i=0; i<strlen(strc)-2; i++)
				{
					if (m_profile[0]==0 && (strc[i] == '\r' || strc[i] == '\n')) {strncpy(m_profile, strc, i<8?i:7); m_profile[i]=0;} // copy profile string
					else if (strc[i] == '\n' && strc[i+1] != '/' && strc[i+1] != 0) m_instr++; // instruction found
				}
			}

			if (!bCreatedFromCache) 
				CON(MSG_INFO, _W("Shader %s compiled (%s, %d instr.) and cached to %s."), desc.ShaderName.Get(), _A2W(m_profile), m_instr, cachefile);
			else
				CON(MSG_INFO, _W("Shader %s (%s, %d instr.) was loaded from cache %s."), desc.ShaderName.Get(), _A2W(m_profile), m_instr, cachefile);


			if(desc.ShaderType == SHADERTYPE_VERTEX_SHADER)
			{
				//create vertex declarations
#if defined(_OGL)
				m_pVertDecl = new CVertexDeclaration(CRenderer::cGraphicsManager()->GetVertexDescByID(desc.VertexDescID), 0);
#elif defined(_DX) && _DX==9
				m_pVertDecl = new CVertexDeclaration(CRenderer::cGraphicsManager()->GetVertexDescByID(desc.VertexDescID), 0);
#elif defined(_DX) && _DX==10
				ID3D10Blob * buf = cgD3D10GetCompiledProgram(m_program);
				m_pVertDecl = new CVertexDeclaration(CRenderer::cGraphicsManager()->GetVertexDescByID(desc.VertexDescID), buf);
#endif

			}

			m_desc = desc;

			return true;
	}

	const char* CShaderCg::GetCompiled()
	{
		assertd(m_compiled, "Shader must be compiled!");
		// get compiled program
		const char* compiled = cgGetProgramString(m_program, CG_COMPILED_PROGRAM);
		if (!compiled[0]) { CON(MSG_ERR, _W("Can't get compiled shader!")); return NULL; }
		return compiled;
	}

	void CShaderCg::Use()
	{
		if(m_desc.ShaderType == SHADERTYPE_VERTEX_SHADER) m_pVertDecl->Use();

		assertd(m_compiled, "Shader must be compiled!");
#if defined(_OGL)
		cgGLBindProgram(m_program);
#elif defined(_DX) && _DX==9
		cgD3D9BindProgram(m_program);
#elif defined(_DX) && _DX==10
		cgD3D10BindProgram(m_program);
#endif
	}

	ShaderHandle CShaderCg::GetHandle(const char *name)
	{
		assertd(m_compiled, "Shader must be compiled!");

		CGparameter parameter = cgGetNamedParameter(m_program, name);

		// try to find existing handle
		int idx = m_parameters.FindIndex(parameter);
		if (idx>0) return idx+1; //Minimum handle value is 1, 0 means an error.

		if(parameter == NULL)
		{
			CON(MSG_ERR, _W("Cannot get a handle for parameter \"%s\"."), name);
			return 0; //Returning 0 means an error.
		}
		m_parameters.AddEx(parameter);
		return m_parameters.Size(); //Minimum handle value is 1, 0 means an error.
	}

	void CShaderCg::SetHandleValueVec3(P3D::ShaderHandle handle, const P3D::Vec3 *value)
	{
		cgSetParameter3fv(m_parameters[handle-1], &value->x);
	}

	void CShaderCg::SetHandleValueTexture(P3D::ShaderHandle handle, P3D::ITexture *tex)
	{
		static CTexture *ctex;
		ctex = (CTexture*)tex;
#if defined(_OGL)
		cgGLSetTextureParameter(m_parameters[handle-1],	*(unsigned int*)ctex->GetInternalTexture());
#elif defined(_DX) && _DX==9
		cgD3D9SetTextureParameter(m_parameters[handle-1], (IDirect3DBaseTexture9*)ctex->GetInternalTexture());
#elif defined(_DX) && _DX==10
		cgD3D10SetTextureParameter(m_parameters[handle-1], (ID3D10Resource*)ctex->GetInternalTexture());
		cgD3D10SetSamplerStateParameter(m_parameters[handle-1], NULL);
#endif
	}

	void CShaderCg::OnLostDevice()
	{
#if defined(_DX) && _DX==9
		cgD3D9UnloadProgram(m_program);
#endif
	}

	void CShaderCg::OnResetDevice()
	{
#if defined(_DX) && _DX==9
		cgD3D9LoadProgram(m_program, false, 0);
#endif

		const char* errra = cgGetLastListing(*g_pcgcontext);
		if (errra && *errra && strlen(errra)>3)
		{
			wchar* errr = new wchar[strlen(errra)+1];
			A2W(errr, errra); 
			//remove new line
			errr[wstrlen(errr)-2]='\0';

			// output error to console
			CON(MSG_ERR, _W("%s"), errr);
			delete[] errr;
		}
	}

	void CShaderCg::SetHandleValueMatrix(ShaderHandle handle, const Matrix *matrix)
	{
		//cgSetMatrixParameterfr(m_parameters[handle-1], (float*)matrix);
		cgSetMatrixParameterfc(m_parameters[handle-1], (float*)matrix);
	}

	void CShaderCg::SetHandleFloatArray(P3D::ShaderHandle handle, const float *farray, unsigned int numelements)
	{
		cgSetParameterValuefc(CheckArrayIndex(m_parameters[handle-1]), numelements, farray);
	}

	CGparameter CShaderCg::CheckArrayIndex(CGparameter param)
	{
		if(cgGetParameterType(param) == CG_ARRAY)
		{
			return cgGetArrayParameter(param, m_arrayIndex);
			m_arrayIndex = 0;
		}
		return param;
	}

	void CShaderCg::SetBuffer(ShaderHandle handle, CShaderBuffer *buffer)
	{
		cgSetProgramBuffer(m_program, cgGetParameterBufferIndex(m_parameters[handle-1]), (CGbuffer)buffer->GetInternalBuffer());
	}

};

#endif