/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "OGL/glex.h"
#include "types.h"


namespace P3D
{

	class CIndexBufferWrapper
	{
	public:
		/** Constructor */
		CIndexBufferWrapper();
		/** Destructor */
		~CIndexBufferWrapper();
		/** Creates the index buffer
		\param IndNum Maximum number of indexes which can IB handle
		\param 32bIndices Select true if you want to use 32b indexes
		\param writeonly If you select true reading from the index buffer fill be forbidden and index buffer will be placed into the most suitable memory pool.
		\param dynamic Select true if your index buffer will be rewritten very often. */
		bool Create(unsigned int IndNum, bool use32bIndices = false, bool writeonly = false, bool dynamic = false);
		/** Setups the index buffer for rendering */
		void Use();
		/** Locks the index buffer so it's indexes can be accessed
		\param **data a pointer to the index buffer's data
		\param discard If you select true the whole content of index buffer will be discarted
		\param SizeToLock You pass 0 for whole vertex buffer */
		bool Lock (void **data, bool discard = false, unsigned int SizeToLock = 0);
		/** UnLocks the vertex buffer */
		void UnLock ();

	protected:
		unsigned int m_indNum;
		bool m_b32b;
	};

};
