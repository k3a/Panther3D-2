/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#ifdef _OGL

#include "TextureWrapper.h"
#include "symbols.h"
#include "../Renderer.h"

#include "GLHelperFunctions.h"
#include <GL/glu.h>

namespace P3D
{


	CTextureWrapper::CTextureWrapper()
	{
		m_Filename = _W("new_texture");
		m_texID = 0;
		//m_backupData = 0;
	}

	CTextureWrapper::~CTextureWrapper()
	{
		if(m_texID) glDeleteTextures(1, &m_texID);
		//SAFE_DELETE_ARRAY(m_backupData);
	}


	const void* CTextureWrapper::GetInternalTexture()
	{
		return &m_texID;
	}

	bool CTextureWrapper::CreateTexture((const sTextureDesc &desc, const BYTE *data, unsigned long len)
	{
		bool compressed = false;

		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &m_texID);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); //automatic mipmap generation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GLint glInternalFormat = 0;
		GLenum glFormat;
		
		switch(desc.Format)
		{
		case P3D_FORMAT_L8_UNORM:
				glInternalFormat = GL_LUMINANCE8;
				glFormat = GL_LUMINANCE;
				m_formatSize = 1;
				break;
		case P3D_FORMAT_X8R8G8B8_UNORM:
			glInternalFormat = GL_LUMINANCE8_ALPHA8;
			glFormat = GL_LUMINANCE_ALPHA;
			m_formatSize = 2;
			break;
		case P3D_FORMAT_A8R8G8B8_UNORM:
			glInternalFormat = GL_RGB8;
			glFormat = GL_BGRA;
			m_formatSize = 4;
			break;
		case RTF_A8R8G8B8:
			glInternalFormat = GL_RGBA8;
			glFormat = GL_BGRA;
			m_formatSize = 4;
			break;
		case P3D_FORMAT_DXT1:
			glInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			glFormat = GL_BGRA;
			m_formatSize = 4;
			compressed = true;
			break;
		case P3D_FORMAT_DXT3:
			glInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			glFormat = GL_BGRA;
			m_formatSize = 4;
			compressed = true;
			break;
		case P3D_FORMAT_DXT5:
			glInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			glFormat = GL_RGBA;
			m_formatSize = 4;
			compressed = true;
			break;
		default:
			CON(MSG_ERR, _W("Requested texture data format is not (yet) supported!") );
			return false;
		}
		//glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, glInternalFormat, width, height, glFormat, GL_UNSIGNED_BYTE, data);
		//if(!compressed) gluBuild2DMipmaps(GL_TEXTURE_2D, glInternalFormat, width, height, glFormat, GL_UNSIGNED_BYTE, data);
		if(!compressed) glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
		else 
		{
			//CON(MSG_INFO, "Compressed.");
			glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, len, data);
// 			for(unsigned int i=1; i< numMipMaps; i++) 
// 			{
// 				
// 				width = width / 2;
// 				height = height / 2;
// 				len = len / 2;
// 				//glCompressedTexImage2DARB(GL_TEXTURE_2D, i, glInternalFormat, width, height, 0, 0, 0);
// 				//glCompressedTexImage2DARB(GL_TEXTURE_2D, i, glInternalFormat, width, height, 0, len, data);
// 			}
		}

		m_format = glFormat;
		m_internalFormat = glInternalFormat;

		CheckGLErrors(_W("Create texture"));

		//create a backup copy in the system memory
// 		m_texSize = Vec2i(width, height);
// 		m_format = glFormat;
// 		m_internalFormat = glInternalFormat;
// 		m_backupData = new BYTE[width * height * m_formatSize];
// 		memcpy(m_backupData, data, width * height * m_formatSize);

		return true;
	}

	void CTextureWrapper::OnLostDevice()
	{
		//if(m_texID) glDeleteTextures(1, &m_texID);
	}

	void CTextureWrapper::OnResetDevice()
	{
// 		glGenTextures(1, &m_texID);
// 		glBindTexture(GL_TEXTURE_2D, m_texID);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_W, GL_REPEAT);
// 		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// 		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
// 		gluBuild2DMipmaps(GL_TEXTURE_2D, m_internalFormat, m_texSize.x, m_texSize.y, m_format, GL_UNSIGNED_BYTE, m_backupData);
	}

	bool CTextureWrapper::SetMipMapData(unsigned int miplevel, BYTE *data, unsigned int len)
	{
// 		unsigned int w, h;
// 		if(miplevel == 0)
// 		{
// 			w = m_texSize.x;
// 			h = m_texSize.y;
// 		}
// 		else
// 		{
// 			w = m_texSize.x/(2*miplevel);
// 			h = m_texSize.y/(2*miplevel);
// 		}
// 		glBindTexture(GL_TEXTURE_2D, m_texID);
// 		glCompressedTexImage2DARB(GL_TEXTURE_2D, miplevel, m_internalFormat, w, h, 0, len, data);
// 
// 		CheckGLErrors("SetMipMapData");
		return true;
	}

};

#endif
