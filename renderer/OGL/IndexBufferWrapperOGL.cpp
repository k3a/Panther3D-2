/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#ifdef _OGL

#include "IndexBufferWrapper.h"
#include "symbols.h"

namespace P3D
{

	CIndexBufferWrapper::CIndexBufferWrapper()
	{
		m_IBID = 0;
		m_indSize = 0;
	}

	CIndexBufferWrapper::~CIndexBufferWrapper()
	{
		if(m_IBID) glDeleteBuffersARB(1, &m_IBID);
	}
	
	bool CIndexBufferWrapper::Create(const sIBDesc &desc, const void *data)
	{
		if( !desc.IsValid() ) return false;

		if(desc.IndexFormat = IBFORMAT_32) m_indSize = sizeof(DWORD);
		else m_indSize = sizeof(WORD);

		bool dynamic = false;
		GLenum usage;
		if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DYNAMIC) dynamic = true;

		if(desc.CPUAccess == P3D_CPUACCESS_NONE || desc.CPUAccess == P3D_CPUACCESS_WRITE)
		{
			if(dynamic) usage = GL_DYNAMIC_DRAW;
			else usage = GL_STATIC_DRAW;
		}
		else if(desc.CPUAccess == P3D_CPUACCESS_READ)
		{
			if(dynamic) usage = GL_DYNAMIC_READ;
			else usage = GL_STATIC_READ;
		}
		else if(desc.CPUAccess == P3D_CPUACCESS_READ_WRITE)
		{
			if(dynamic) usage = GL_DYNAMIC_COPY;
			else usage = GL_STATIC_COPY;
		}
		else
		{
			CON(MSG_ERR, _W("Index buffer creation failed. No apropriate Resource usage given."));
			return false;
		} 

		glGenBuffersARB(1, &m_IBID);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, m_IBID);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, desc.IndexCount * m_indSize, data, usage);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, NULL);

		//CheckGLErrors("Create Index Buffer");
		m_desc = desc;

		return true;
	}

	void CIndexBufferWrapper::Use()
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, m_IBID);
	}

	bool CIndexBufferWrapper::Map(void **data, bool discard, bool doNotWait)
	{
		glBindBufferARB(GL_ARRAY_BUFFER, m_IBID);
		GLenum access = GL_WRITE_ONLY;
		if(m_desc.CPUAccess == P3D_CPUACCESS_READ) access = GL_READ_ONLY;
		else if(m_desc.CPUAccess == P3D_CPUACCESS_READ_WRITE) access = GL_READ_WRITE;

		if(discard) glBufferDataARB(GL_ARRAY_BUFFER, m_desc.IndexCount * m_indSize, 0, m_usage);
		*data = glMapBufferARB(GL_ARRAY_BUFFER, access);

		return true;
	}

	void CIndexBufferWrapper::Unmap()
	{
		glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER);
	}

	bool CIndexBufferWrapper::OnLostDevice()
	{
// 		if(m_IBID)
// 		{
// 			glDeleteBuffersARB(1, &m_IBID);
// 			m_IBID = 0;
// 		}
 		return true;
	}

	bool CIndexBufferWrapper::OnResetDevice()
	{
// 		glGenBuffersARB(1, &m_IBID);
// 		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, m_IBID);
// 		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, m_indNum*m_size, NULL, m_usage);
// 		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, NULL);
// 		//CheckGLErrors();
		return true;
	}

};

#endif
