/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#if defined(_DX) && _DX == 10

#include "TextureWrapper.h"
#include "symbols.h"
#include "../Renderer.h"


namespace P3D
{

	extern ID3D10Device* g_pD3ddev;

	CTextureWrapper::CTextureWrapper()
	{
		m_pTexture = 0;
		m_Filename = _W("new_texture");
			}

	CTextureWrapper::~CTextureWrapper()
	{
		SAFE_RELEASE(m_pTexture);
	}

	const void* CTextureWrapper::GetInternalTexture()
	{
		return m_pTexture;
	}

	bool CTextureWrapper::CreateTexture(sTextureDesc &desc, BYTE *data, unsigned long len)
	{
		D3D10_TEXTURE2D_DESC d3dtexdesc;
		d3dtexdesc.Width = desc.Width;
		d3dtexdesc.Height = desc.Height;
		d3dtexdesc.MipLevels = desc.MipLevels;
		if(!desc.AutoGenerateMips)
		{
			if(desc.MipLevels == 0) 
			{
				CON(MSG_ERR, "The number of MipLevels can be 0 only if AutoGenMips is set to true, fixing to 1.")
				d3dtexdesc.MipLevels = 1;
			}
		}
		
		D3DFORMAT dxFormat;
		switch(desc.Format)
		{
		case P3D_FORMAT_L8:
			dxFormat = D3DFMT_L8;
			break;
		case P3D_FORMAT_X8R8G8B8:
			dxFormat = D3DFMT_X8R8G8B8;
			break;
		case P3D_FORMAT_A8R8G8B8:
			dxFormat = D3DFMT_A8R8G8B8;
			break;
		case P3D_FORMAT_DXT1:
			dxFormat = D3DFMT_DXT1;
			break;
		case P3D_FORMAT_DXT3:
			dxFormat = D3DFMT_DXT3;
			break;
		case P3D_FORMAT_DXT5:
			dxFormat = D3DFMT_DXT5;
			break;
		case P3D_FORMAT_D24S8:
			dxFormat = D3DFMT_D24S8;
			break;
		default:
			CON(MSG_ERR, _W("Requested texture data format is not (yet) supported!"));
			return false;
		} 

		//TODO: finish this!
		DWORD usage = 0;
		if(desc.BindFlag == P3D_TEXBINDFLAG_RENDER_TARGET) usage |= D3DUSAGE_RENDERTARGET;
		else if(desc.BindFlag == P3D_TEXBINDFLAG_DEPTH_STENCIL) usage |= D3DUSAGE_DEPTHSTENCIL;

		if(desc.AutoGenerateMips) usage |= D3DUSAGE_AUTOGENMIPMAP;

		D3DPOOL pool;
		if(desc.BindFlag == P3D_TEXBINDFLAG_RENDER_TARGET || desc.BindFlag == P3D_TEXBINDFLAG_DEPTH_STENCIL)
			pool = D3DPOOL_DEFAULT;
		else pool = D3DPOOL_MANAGED;
		//

		if(FAILED(g_pD3ddev->CreateTexture(desc.Width, desc.Height, desc.MipLevels, usage, dxFormat, pool, &m_pTexture, NULL))) return false;

		IDirect3DSurface9* surface;
		m_pTexture->GetSurfaceLevel(0, &surface);
		m_pTopSurface = surface;
		D3DLOCKED_RECT rect;
		if(data)
		{
			surface->LockRect(&rect, NULL, 0);
			BYTE *SurfaceData = static_cast<BYTE*>(rect.pBits);
			memcpy(SurfaceData, data, len);
			surface->UnlockRect();
		}
		//surface->Release();

		if(desc.AutoGenerateMips)
			if(desc.MipLevels == 1 || desc.MipLevels == 0) m_pTexture->GenerateMipSubLevels();

		//CON(MSG_INFO, _W("CreateTexture End"));	

		return true;
	}

	bool CTextureWrapper::SetMipMapData(unsigned int miplevel, BYTE *data, unsigned int len)
	{
		IDirect3DSurface9* surface;
		if(FAILED(m_pTexture->GetSurfaceLevel(miplevel, &surface))) return false;
		D3DLOCKED_RECT rect;
		if(FAILED(surface->LockRect(&rect, NULL, 0)))
		{
			surface->Release();
			return false;
		}
		BYTE *SurfaceData = static_cast<BYTE*>(rect.pBits);
		memcpy(SurfaceData, data, len);
		surface->UnlockRect();
		surface->Release();

		return true;
	}

	void CTextureWrapper::OnLostDevice()
	{

	}

	void CTextureWrapper::OnResetDevice()
	{

	}

	void CTextureWrapper::SetTexSettings(sTexSettings settings)
	{
		m_settings.MinFilter = settings.MinFilter;
		m_settings.MagFilter = settings.MagFilter;
		m_settings.MaxAnistropy = settings.MaxAnistropy;
		m_settings.AddressU = settings.AddressU;
		m_settings.AddressV = settings.AddressV;
	}

	sTexSettings CTextureWrapper::GetTexSettings()
	{
		return m_settings;
	}

};

#endif
