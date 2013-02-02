/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#include "types.h"
#include <DX/D3D10.h>
#include "../BufferDescs.h"

namespace P3D
{
	
	class CShaderBufferHLSL
	{
	public:
		CShaderBufferHLSL();
		~CShaderBufferHLSL();
		bool Create(const sShaderBufferDesc &desc, const void *data);
		void Update(const void *data);
		bool Map(void **data, bool discard);
		void Unmap();
		inline sShaderBufferDesc GetDesc() { return m_desc; };
		inline void* GetInternalBuffer() { return m_pBuffer; };
	private:
		ID3D10Buffer *m_pBuffer;
		sShaderBufferDesc m_desc;
	};
};
