/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "glex.h"
#include "types.h"
#include "../BufferDescs.h"

namespace P3D
{

	//A kind of variation on D3DVertexDeclaration
	struct OglVertexDeclaration
	{
		GLuint   attrIndex; 
		GLint   numComponents,  
		GLenum type;
		GLsizei   stride;
		
		unsigned int numElements;
		unsigned int oneElementSize;
		unsigned int offset;
		unsigned int usage;
	};	

	class CVertexDeclarationWrapper
	{
	public:
		CVertexDeclarationWrapper(const VertexElement *vearray, void *buffer);
		~CVertexDeclarationWrapper();
		void Use();
		inline bool IsValid() { return m_valid; };
	private:
		
		struct OglVertexElement
		{
			GLuint   attrIndex; 
			GLint   numComponents,  
			GLenum type;
			GLsizei   stride;
		};	
		tArray<OglVertexElement> m_vertexDeclaration;

		bool m_valid;
	};

	class CVertexBufferWrapper
	{
	public:
		CVertexBufferWrapper();
		~CVertexBufferWrapper();
		bool Create(const sVBDesc &desc, void *data = 0);
		void Use();
		inline const sVBDesc& GetDesc() { return m_desc; };
		bool Map(void **data, bool discard = false, bool doNotWait = false);
		void Unmap();
		void Render(PrimitiveType primType, int start, int num);
		void RenderIndexed(PrimitiveType primType, int startIndex, int num);
		bool Update(void *Data);

		void OnLostDevice();
		void OnResetDevice();

	protected:

		unsigned int m_vertNum;
		unsigned int m_SizeInBytes;
		sVBDesc m_desc;

		GLuint m_VBID;
		bool m_writeonly;
		std::vector<OglVertexDeclaration> m_vertDeclaration;
		GLenum m_usage;

		BYTE *m_data;

	};

};
