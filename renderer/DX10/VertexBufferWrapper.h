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

	class CVertexDeclarationWrapper
	{
	public:
		CVertexDeclarationWrapper(const VertexElement *vearray, void *buffer);
		~CVertexDeclarationWrapper();
		void Use();
		inline bool IsValid() { return m_valid; };
	private:
		ID3D10InputLayout* m_pInputLayout;
		bool m_valid;
	};

	class CVertexBufferWrapper
	{
	public:
		/** Constructor */
		CVertexBufferWrapper();
		/** Destructor */
		~CVertexBufferWrapper();

		/** Create the buffer 
		\param Desc Pointer to the buffer's data \see sVBDesc
		\param Data You can insert the data during the creation time. \note Immutable buffers are supposed to be filled with data this way.
		*/
		bool Create(sVBDesc &desc, void *data = 0);

		/** Bind the buffer */
		void Use();

		/** Return the buffer's description */
		inline const sVBDesc& GetDesc() { return m_desc; };

		/** Obtain a pointer to the buffer's data 
		\param Data Pointer to the buffer's data 
		\param Discard Whether or not to discard the current data \note Use this if you don't need to keep the previous data.
		\param DoNotWait If you set this to true and the buffer is currently in use and thus cannot be locked immediately, 
		the method will return false. The benefit is that you can do some useful stuff on the CPU instead of waiting.
		\note Works only when using the D3D10 renderer.
		*/
		bool Map(void **data, bool discard = false, bool doNotWait = false);

		/** Return access to the GPU */
		void Unmap ();

		/** Render non-indexed primitives */
		void Render(PrimitiveType primType, int start, int num);

		/** Render indexed primitives */
		void RenderIndexed(PrimitiveType primType, UINT startIndex, UINT num);

		/** Update the buffer's content. \note Use this method when the buffer's usage is set to DEFAULT. */
		bool Update(void *Data);

		/** Pre device reset method \note Has no effect when using D3D10 */
		void OnLostDevice();

		/** Post device reset method  \note Has no effect when using D3D10 */
		void OnResetDevice();

	private:
		unsigned int m_vertNum;
		unsigned int m_SizeInBytes;
		ID3D10Buffer* m_pVB;
		sVBDesc m_desc;
	};

};