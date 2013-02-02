/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
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
#include "DX10/DX10HelperFunctions.h"
#endif

#include <types.h>
#include "Texture.h"
#include "IShader.h"
#include "VertexBuffer.h"
#include "ShaderBuffer.h"

namespace P3D
{
	///Cg implementation of a CShader class
	class CShaderCg : public IShader
	{
	public:
		CShaderCg();
		~CShaderCg();
		/// Create shader from source
		bool Create(sShaderDesc &desc);
		/// Retrieve compiled shader string
		const char* GetCompiled();
		void Use();
		ShaderHandle GetHandle(const char *name);
		void SetHandleValueVec3(ShaderHandle handle, const Vec3 *value);
		void SetHandleValueTexture(ShaderHandle handle, ITexture *tex);
		void SetHandleValueMatrix(ShaderHandle handle, const Matrix *matrix);
		inline unsigned int GetArraySize(ShaderHandle handle) { return cgGetArrayTotalSize(m_parameters[handle-1]); };
		inline void SetArrayIndex(unsigned int index) { m_arrayIndex = index; };
		void SetHandleFloatArray(ShaderHandle handle, const float *farray, unsigned int numelements);
		void SetBuffer(ShaderHandle handle, CShaderBuffer *buffer);
		void ConnectSharedParam(ShaderHandle connectTo, void *sharedParam) { cgConnectParameter((CGparameter)sharedParam, m_parameters[connectTo-1]); };
		//TODO: get shader type and desc

		void OnLostDevice();
		void OnResetDevice();

	private:
		CGparameter CheckArrayIndex(CGparameter param);
		CGprogram m_program;
		tArray<CGparameter> m_parameters;
		unsigned int m_arrayIndex;
		// compilation info
		bool m_compiled;
		char m_profile[8]; // profile string
		int m_instr; // instruction count

		sShaderDesc m_desc;
		CVertexDeclaration *m_pVertDecl;
	};
};
