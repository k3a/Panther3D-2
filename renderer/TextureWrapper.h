/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "types.h"
#include <DX/D3D10.h>

#include "ITexture.h"

namespace P3D
{	

	class CTextureWrapper : public ITexture
	{
	public:
		CTextureWrapper();
		~CTextureWrapper();
		bool CreateTexture(sTextureDesc &desc, BYTE *data, unsigned long len);
		const void* GetInternalTexture();
		void* GetRenderTarget() { return m_pTopSurface; };

		bool SetMipMapData(unsigned int miplevel, BYTE *data, unsigned int len);

		void OnLostDevice();
		void OnResetDevice();

	protected:
		Str m_Filename;
	};

};