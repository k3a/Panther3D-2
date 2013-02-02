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
#include "DX10HelperFunctions.h"

namespace P3D
{	

	class CTextureWrapper : public ITexture
	{
	public:
		CTextureWrapper();
		~CTextureWrapper();
		bool CreateTexture(sTextureDesc &desc, BYTE *data, unsigned long len);
		const void* GetInternalTexture();
		inline void* GetRenderTarget() { return m_pRenderTarget; };
		inline void* GetDepthStencil() { return m_pDepthBuffer; };

		bool SetMipMapData(UINT texID, UINT miplevel, BYTE *data, unsigned int len);

		void OnLostDevice();
		void OnResetDevice();
	private:


	protected:
		Str m_Filename;
		sTextureDesc m_desc;
		ID3D10Texture2D *m_pPhysicalTexture;
		ID3D10ShaderResourceView *m_pTexture;
		ID3D10RenderTargetView *m_pRenderTarget;
		ID3D10DepthStencilView *m_pDepthBuffer;
	};

};