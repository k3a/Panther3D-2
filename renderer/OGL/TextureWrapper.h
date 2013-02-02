/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "glex.h"
#include "types.h"

#include "ITexture.h"

namespace P3D
{

	class CTextureWrapper : public ITexture
	{
	public:
		CTextureWrapper();
		~CTextureWrapper();
		bool CreateTexture(const sTextureDesc &desc, const BYTE *data, unsigned long len);
		const void* GetInternalTexture();
		inline void* GetRenderTarget() { return m_pRenderTarget; };
		inline void* GetDepthStencil() { return m_pDepthBuffer; };

		bool SetMipMapData(UINT texID, UINT miplevel, BYTE *data, unsigned int len);

		void OnLostDevice();
		void OnResetDevice();

	protected:
		GLuint m_texID;
		Str m_Filename;
		sTextureDesc m_desc;

		Vec2i m_texSize;
		GLenum m_format;
		GLint m_internalFormat;
		unsigned int m_formatSize;
		//BYTE *m_backupData;
	};

};
