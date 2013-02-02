/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "ShaderBuffer.h"
#include "renderer.h"

namespace P3D
{

	extern CGcontext *g_pcgcontext;

	CShaderBuffer::CShaderBuffer()
	{
		m_buffer = NULL;
	}

	CShaderBuffer::~CShaderBuffer()
	{
		cgDestroyBuffer(m_buffer);
	}

	bool CShaderBuffer::Create(const sShaderBufferDesc &desc, const void *data)
	{
		CGbufferusage usage;
		if(desc.dynamic)
		{
			if(desc.cpuAccess == CPUACCESS_READ) usage = CG_BUFFER_USAGE_DYNAMIC_READ;
			else if(desc.cpuAccess == CPUACCESS_WRITE) usage = CG_BUFFER_USAGE_DYNAMIC_DRAW;
			else if(desc.cpuAccess == CPUACCESS_READ_WRITE) usage = CG_BUFFER_USAGE_DYNAMIC_COPY;
			else
			{
				CON(MSG_ERR, _W("No appropriate CPUAccess given. Creating ShaderBuffer failed."));
				return false;
			}
		}
		else
		{
			if(desc.cpuAccess == CPUACCESS_READ) usage = CG_BUFFER_USAGE_STATIC_READ;
			else if(desc.cpuAccess == CPUACCESS_WRITE) usage = CG_BUFFER_USAGE_STATIC_DRAW;
			else if(desc.cpuAccess == CPUACCESS_READ_WRITE) usage = CG_BUFFER_USAGE_STATIC_COPY;
			else
			{
				CON(MSG_ERR, _W("No appropriate CPUAccess given. Creating ShaderBuffer failed."));
				return false;
			}
		}

		m_buffer = cgCreateBuffer(*g_pcgcontext, desc.bufferSize, data, usage);

		if(!m_buffer) return false;

		m_desc = desc;
		return true;
	}

	bool CShaderBuffer::SetData(const void *data, UINT newBufferSize)
	{
		cgSetBufferData(m_buffer, newBufferSize, data);
		return true;
	}

	bool CShaderBuffer::SetSubData(UINT offset, UINT size, const void *data)
	{
		cgSetBufferSubData(m_buffer, offset, size, data);
		return true;
	}

	bool CShaderBuffer::Map(void **data, bool discard)
	{
		CGbufferaccess access;
		if(m_desc.cpuAccess == CPUACCESS_READ) access = CG_MAP_READ;
		else if(m_desc.cpuAccess == CPUACCESS_READ_WRITE)access = CG_MAP_READ_WRITE ;
		else if(m_desc.cpuAccess == CPUACCESS_WRITE)
		{
			if(discard) access = CG_MAP_WRITE_DISCARD;
			else CG_MAP_WRITE;
		}
		
		cgMapBuffer(m_buffer, access);
		return true;
	}

	void CShaderBuffer::Unmap()
	{
		cgUnmapBuffer(m_buffer);
	}

};
