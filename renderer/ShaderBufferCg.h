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

#include <types.h>
#include "BufferDescs.h"

namespace P3D
{
	
	class CShaderBufferCg
	{
	public:
		CShaderBufferCg();
		~CShaderBufferCg();
		bool Create(const sShaderBufferDesc &desc, const void *data);
		bool SetData(const void *data);
		bool Map(void **data, bool discard);
		void Unmap();
		inline sShaderBufferDesc GetDesc() { return m_desc; };
		inline void* GetInternalBuffer() { return m_buffer; };
	private:
		CGbuffer m_buffer;
		sShaderBufferDesc m_desc;
	};
};
