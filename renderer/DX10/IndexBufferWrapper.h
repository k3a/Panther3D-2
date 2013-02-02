/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
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

	class CIndexBufferWrapper
	{
	public:
		CIndexBufferWrapper();
		~CIndexBufferWrapper();
		bool Create(sIBDesc &desc, void *data = 0);
		inline const sIBDesc& GetDesc() { return m_desc; };
		void Use();
		bool Map(void **data, bool discard = false, bool doNotWait = false);
		void Unmap();

		bool OnLostDevice();
		bool OnResetDevice();

	protected:
		sIBDesc m_desc;
		ID3D10Buffer* m_pIB;
	};

};