/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "glex.h"
#include "types.h"


namespace P3D
{

	class CIndexBufferWrapper
	{
	public:
		CIndexBufferWrapper();
		~CIndexBufferWrapper();
		bool Create(const sIBDesc &desc, const void *data = 0);
		inline const sIBDesc& GetDesc() { return m_desc; };
		void Use();
		bool Map(void **data, bool discard = false, bool doNotWait = false);
		void Unmap();

		bool OnLostDevice();
		bool OnResetDevice();

	protected:
		sIBDesc m_desc;
		GLuint m_IBID;
		UINT m_indSize;
		//unsigned int m_indNum;
		//bool m_b32b;
		//bool m_writeonly;
		//GLenum m_usage;
		//unsigned int m_size;
	};

};
