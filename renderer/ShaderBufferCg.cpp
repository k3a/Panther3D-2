/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#ifdef _CG

#include "ShaderBufferCg.h"
#include "renderer.h"

namespace P3D
{

	extern CGcontext *g_pcgcontext;

	CShaderBufferCg::CShaderBufferCg()
	{
		m_buffer = NULL;
	}

	CShaderBufferCg::~CShaderBufferCg()
	{
		cgDestroyBuffer(m_buffer);
	}

	bool CShaderBufferCg::Create(const sShaderBufferDesc &desc, const void *data)
	{
		CGbufferusage usage;
		bool dynamic = false;

		if(desc.CPUAccess == P3D_CPUACCESS_NONE || desc.CPUAccess == P3D_CPUACCESS_WRITE)
		{
			if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DYNAMIC) usage = CG_BUFFER_USAGE_DYNAMIC_DRAW;
			else usage = CG_BUFFER_USAGE_STATIC_DRAW;
		}
		else if(desc.CPUAccess == P3D_CPUACCESS_READ)
		{
			if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DYNAMIC) usage = CG_BUFFER_USAGE_DYNAMIC_READ;
			else usage = CG_BUFFER_USAGE_STATIC_READ;
		}
		else if(desc.CPUAccess == P3D_CPUACCESS_READ_WRITE)
		{
			if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DYNAMIC) usage = CG_BUFFER_USAGE_DYNAMIC_COPY;
			else usage = CG_BUFFER_USAGE_STATIC_COPY;
		}
		else 
		{
			CON(MSG_ERR, _W("Shader buffer creation failed. No apropriate ResourceUsage given."));
			return false;
		}

		m_buffer = cgCreateBuffer(*g_pcgcontext, desc.BufferSize, data, usage);

		if(!m_buffer) 
		{
			CON(MSG_ERR, _W("Shader buffer creation failed."));
			return false;
		}

		m_desc = desc;
		return true;
	}

	bool CShaderBufferCg::SetData(const void *data)
	{
		cgSetBufferData(m_buffer, 0, data);
		return true;
	}

	bool CShaderBufferCg::Map(void **data, bool discard)
	{
		CGbufferaccess access;
		if(m_desc.CPUAccess == P3D_CPUACCESS_READ) access = CG_MAP_READ;
		else if(m_desc.CPUAccess == P3D_CPUACCESS_READ_WRITE)access = CG_MAP_READ_WRITE ;
		else if(m_desc.CPUAccess == P3D_CPUACCESS_WRITE)
		{
			if(discard) access = CG_MAP_WRITE_DISCARD;
			else CG_MAP_WRITE;
		}

		cgMapBuffer(m_buffer, access);
		return true;
	}

	void CShaderBufferCg::Unmap()
	{
		cgUnmapBuffer(m_buffer);
	}

};

#endif