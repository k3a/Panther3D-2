/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#ifdef _OGL

#include "VertexBufferWrapper.h"
#include "symbols.h"
#include "../Renderer.h"

#include "GLHelperFunctions.h"

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

namespace P3D
{

	CVertexBufferWrapper::CVertexBufferWrapper()
	{
		m_vertNum = 0;
		m_SizeInBytes = 0;
		m_VBID = 0;
		m_writeonly = false; 
		m_data  = 0;
	}

	CVertexBufferWrapper::~CVertexBufferWrapper()
	{
		if(m_VBID) glDeleteBuffersARB(1, &m_VBID);
		SAFE_DELETE_ARRAY(m_data);
	}

	bool CVertexBufferWrapper::Create(const sVBDesc &desc, void *data = 0)
	{

		if( !desc.IsValid() ) return false;
		
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
			CON(MSG_ERR, _W("Vertex buffer creation failed. No apropriate Resource usage given."));
			return false;
		} 
		
		glGenBuffersARB(1, &m_VBID);
		glBindBufferARB(GL_ARRAY_BUFFER, m_VBID);
		glBufferDataARB(GL_ARRAY_BUFFER, desc.VertexCount*desc.VertexSize, data, usage);
		glBindBufferARB(GL_ARRAY_BUFFER, NULL);

		CheckGLErrors(_W("Create vertex buffer"));

		m_desc = desc;

		return true;
	}

	bool CVertexBufferWrapper::Map(void **data, bool discard, unsigned int SizeToLock)
	{
		glBindBufferARB(GL_ARRAY_BUFFER, m_VBID);
		GLenum access = GL_WRITE_ONLY;
		if(m_desc.CPUAccess == P3D_CPUACCESS_READ) access = GL_READ_ONLY;
		else if(m_desc.CPUAccess == P3D_CPUACCESS_READ_WRITE) access = GL_READ_WRITE;

		if(discard) glBufferDataARB(GL_ARRAY_BUFFER, m_desc.VertexCount * m_desc.VertexSize, 0, m_usage);
		*data = glMapBufferARB(GL_ARRAY_BUFFER, access);
		
		//CheckGLErrors(_W("On lock vertex buffer"));

		return true;
	}

	void CVertexBufferWrapper::Unmap()
	{
		glUnmapBufferARB(GL_ARRAY_BUFFER);
	}

	void CVertexBufferWrapper::Use()
	{

		glBindBufferARB(GL_ARRAY_BUFFER, m_VBID);

		///*
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		for(unsigned int i=0; i<m_vertDeclaration.size(); i++)
		{
			
			switch(m_vertDeclaration[i].usage)
			{
			case P3DVU_POSITION:
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(m_vertDeclaration[i].numElements, 
					m_vertDeclaration[i].type,
					m_SizeInBytes,
					BUFFER_OFFSET(m_vertDeclaration[i].offset));
				break;

			case P3DVU_TEXCOORD:
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(m_vertDeclaration[i].numElements, 
					m_vertDeclaration[i].type,
					m_SizeInBytes,
					BUFFER_OFFSET(m_vertDeclaration[i].offset));
				break;

			case P3DVU_COLOR:
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(m_vertDeclaration[i].numElements, 
					m_vertDeclaration[i].type,
					m_SizeInBytes,
					BUFFER_OFFSET(m_vertDeclaration[i].offset));
				break;

			case P3DVU_NORMAL:
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(m_vertDeclaration[i].type,
					m_SizeInBytes,
					BUFFER_OFFSET(m_vertDeclaration[i].offset));
				break;
			}
		}
		//*/

		//glVertexPointer(3, GL_FLOAT, 3*sizeof(float), BUFFER_OFFSET(0));
		//glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		//CheckGLErrors();
	}

	void CVertexBufferWrapper::Render(P3D::PrimitiveType primType, int start, int num)
	{
		GLenum mode;
		if	   (primType == P3DPT_POINTLIST)     mode = GL_POINTS;
		else if(primType == P3DPT_LINELIST)      
		{
			mode = GL_LINES;
			num *= 2;
		} 
		else if(primType == P3DPT_LINESTRIP)     mode = GL_LINE_STRIP;
		else if(primType == P3DPT_TRIANGLELIST)  
		{
			mode = GL_TRIANGLES;
			num *= 3;
		}
		else if(primType == P3DPT_TRIANGLESTRIP) mode = GL_TRIANGLE_STRIP;
		else                                     mode = GL_TRIANGLE_FAN; //P3DPT_TRIANGLEFAN

		glDrawArrays(mode, start, num);
	}

	void CVertexBufferWrapper::RenderIndexed(PrimitiveType primType, int startIndex, bool use32bIndices, int num)
	{
		PROFILESAMPLE(OGLVBRenderIndexed);
		
		static GLenum mode;
		static GLenum type;

		if	   (primType == P3DPT_POINTLIST)     mode = GL_POINTS;
		else if(primType == P3DPT_LINELIST)      
		{
			mode = GL_LINES;
			num *= 2;
		} 
		else if(primType == P3DPT_LINESTRIP)     mode = GL_LINE_STRIP;
		else if(primType == P3DPT_TRIANGLELIST)  
		{
			mode = GL_TRIANGLES;
			num *= 3;
		}
		else if(primType == P3DPT_TRIANGLESTRIP) mode = GL_TRIANGLE_STRIP;
		else                                     mode = GL_TRIANGLE_FAN; //P3DPT_TRIANGLEFAN

		if(use32bIndices) type = GL_UNSIGNED_INT;
		else type = GL_UNSIGNED_SHORT;

		//glDrawRangeElements IS FASTER!!! (by nVidia)
		glDrawElements (mode, num, type, ((char *)NULL + (startIndex)));
	}

	bool CVertexBufferWrapper::GenerateVertexDeclaration(VertexElement *vertDeclaration)
	{
		// We find out how many Elements has our Vertex Declaration
		UINT numElements = 0;
		while(vertDeclaration[numElements].vertexType != P3DVD_NONE)
		{
			numElements++;
		}
		if(numElements == 0) return false;

		GLenum type;
		unsigned int numVElements;
		unsigned int oneElementSize;
		unsigned int offset = 0;


		for(UINT i=0; i<numElements; i++)
		{
			switch(vertDeclaration[i].vertexType)
			{
			case P3DVD_NONE:
				{
					//don't know
					break;
				}

			case P3DVD_FLOAT1:
				{
					type = GL_FLOAT;
					numVElements = 1;
					oneElementSize = sizeof(float);
					offset += sizeof(float);
					break;
				}

			case P3DVD_FLOAT2:
				{
					type = GL_FLOAT;
					numVElements = 2;
					oneElementSize = sizeof(float);
					offset += 2*sizeof(float);
					break;
				}

			case P3DVD_FLOAT3:
				{
					type = GL_FLOAT;
					numVElements = 3;
					oneElementSize = sizeof(float);
					offset += 3*sizeof(float);
					break;
				}

			case P3DVD_FLOAT4:
				{
					type = GL_FLOAT;
					numVElements = 4;
					oneElementSize = sizeof(float);
					offset += 4*sizeof(float);
					break;
				}
			}

			OglVertexDeclaration vd;
			vd.numElements = numVElements;
			vd.offset = offset - numVElements * oneElementSize;
			vd.oneElementSize = oneElementSize;
			vd.type = type;
			vd.usage = vertDeclaration[i].vertexUsage;

			m_vertDeclaration.push_back(vd);
		}
		return true;
	}

	void CVertexBufferWrapper::OnLostDevice()
	{
		if(m_VBID)
		{
			glDeleteBuffersARB(1, &m_VBID);
			m_VBID = 0;
		}
	}

	void CVertexBufferWrapper::OnResetDevice()
	{
		glGenBuffersARB(1, &m_VBID);
		glBindBufferARB(GL_ARRAY_BUFFER, m_VBID);
		glBufferDataARB(GL_ARRAY_BUFFER, m_vertNum*m_SizeInBytes, NULL, m_usage);
		glBindBufferARB(GL_ARRAY_BUFFER, NULL);
		CheckGLErrors(_W("On reset vertex buffer"));
	}

}

#endif
