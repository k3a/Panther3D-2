/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "types.h"
#include <string>
#include <DX/d3d9.h>

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
		void SetTexSettings(sTexSettings settings);
		sTexSettings GetTexSettings();

		bool SetMipMapData(UINT texID, unsigned int miplevel, BYTE *data, unsigned int len);

		void OnLostDevice();
		void OnResetDevice();

	protected:
		IDirect3DTexture9* m_pTexture;
		Str m_Filename;
		IDirect3DSurface9 *m_pTopSurface;
		sTexSettings m_settings;
		D3DTEXTUREFILTERTYPE m_filter;
	};

};