/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "types.h"
#include <DX/d3d9.h>
#include "../BufferDescs.h"

namespace P3D
{

	class CIndexBufferWrapper
	{
	public:
		/** Constructor */
		CIndexBufferWrapper();
		/** Destructor */
		~CIndexBufferWrapper();
		bool Create(sIBDesc &desc, void *data = 0);

		/** Return the buffer's description */
		inline const sIBDesc& GetDesc() { return m_desc; };

		/** Setups the index buffer for rendering */
		void Use();
		/** Locks the index buffer so it's indexes can be accessed 
		\param **data a pointer to the index buffer's data
		\param discard If you select true the whole content of index buffer will be discarted 
		\param SizeToLock You pass 0 for whole vertex buffer */
		bool Map(void **data, bool discard = false, unsigned int SizeToLock = 0);
		/** UnLocks the vertex buffer */
		void Unmap ();

		bool OnLostDevice();
		bool OnResetDevice();

	protected:
		sIBDesc m_desc;
		D3DPOOL m_IBPool;
		DWORD m_usage;
		IDirect3DIndexBuffer9* m_pIB;
	};

};