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
		m_pPhysicalTexture = 0;
		m_pTexture = 0;
		m_pRenderTarget = 0;
		m_pDepthBuffer = 0;
		m_Filename = _W("new_texture");
	}

	CTextureWrapper::~CTextureWrapper()
	{
 		SAFE_RELEASE(m_pTexture);
 		SAFE_RELEASE(m_pRenderTarget);
 		SAFE_RELEASE(m_pDepthBuffer);
 		SAFE_RELEASE(m_pPhysicalTexture);
	}

	const void* CTextureWrapper::GetInternalTexture()
	{
		//return m_pPhysicalTexture;
		return m_pTexture;
	}

	bool CTextureWrapper::CreateTexture(sTextureDesc &desc, BYTE *data, unsigned long len)
	{
		DXGI_FORMAT dxFormat;
		DXGI_FORMAT dxRVFormat;
		DXGI_FORMAT dxDSFormat;
		bool useExtraDesc = false;
		
		D3D10_TEXTURE2D_DESC d3dtexdesc;
		d3dtexdesc.Width = desc.Width;
		d3dtexdesc.Height = desc.Height;
		d3dtexdesc.MipLevels = desc.MipLevels;
		if(!desc.AutoGenerateMips)
		{
			if(desc.MipLevels == 0) 
			{
				CON(MSG_ERR, _W("The number of MipLevels can be 0 only if AutoGenMips is set to true, fixing to 1."));
				d3dtexdesc.MipLevels = 1;
			}
		}
		d3dtexdesc.ArraySize = desc.ArraySize;
		
		switch(desc.Format)
		{
		case P3D_FORMAT_L8_UNORM:
			dxFormat = DXGI_FORMAT_R8_UNORM;
			break;
		case P3D_FORMAT_X8R8G8B8_UNORM:
			dxFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case P3D_FORMAT_A8R8G8B8_UNORM:
			dxFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case P3D_FORMAT_R16G16_SNORM:
			dxFormat = DXGI_FORMAT_R16G16_SNORM;
			break;
		case P3D_FORMAT_R32_FLOAT:
			dxFormat = DXGI_FORMAT_R32_FLOAT;
			break;
		case P3D_FORMAT_DXT1:
			dxFormat = DXGI_FORMAT_BC1_UNORM;
			break;
		case P3D_FORMAT_DXT3:
			dxFormat = DXGI_FORMAT_BC2_UNORM;
			break;
		case P3D_FORMAT_DXT5:
			dxFormat = DXGI_FORMAT_BC3_UNORM;
			break;
		case P3D_FORMAT_D24S8:
			//dxFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
			dxFormat = DXGI_FORMAT_R24G8_TYPELESS;
			dxRVFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			dxDSFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
			useExtraDesc = true;
			break;
		default:
			CON(MSG_ERR, _W("Requested texture data format is not (yet) supported!"));
			return false;
		} 
		d3dtexdesc.Format = dxFormat;
		d3dtexdesc.SampleDesc.Count = 1;
		d3dtexdesc.SampleDesc.Quality = 0;

		if(desc.ResourceUsage == P3D_RESOURCEUSAGE_IMMUTABLE) d3dtexdesc.Usage = D3D10_USAGE_IMMUTABLE;
		else if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DEFAULT) d3dtexdesc.Usage = D3D10_USAGE_DEFAULT;
		else if(desc.ResourceUsage == P3D_RESOURCEUSAGE_DYNAMIC)
		{
			if(desc.CPUAccess != P3D_CPUACCESS_READ) d3dtexdesc.Usage = D3D10_USAGE_DYNAMIC;
			else d3dtexdesc.Usage = D3D10_USAGE_STAGING;
		}
		else
		{
			CON(MSG_ERR, _W("Texture creation failed. No apropriate ResourceUsage given."));
			return false;
		}

		d3dtexdesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		if(desc.BindFlag == P3D_TEXBINDFLAG_RENDER_TARGET) d3dtexdesc.BindFlags |= D3D10_BIND_RENDER_TARGET; 
		else if(desc.BindFlag == P3D_TEXBINDFLAG_DEPTH_STENCIL) d3dtexdesc.BindFlags |= D3D10_BIND_DEPTH_STENCIL;

		if(desc.CPUAccess == P3D_CPUACCESS_NONE) d3dtexdesc.CPUAccessFlags = 0;
		else if(desc.CPUAccess == P3D_CPUACCESS_READ) d3dtexdesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
		else if(desc.CPUAccess == P3D_CPUACCESS_WRITE) d3dtexdesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		else if(desc.CPUAccess == P3D_CPUACCESS_READ_WRITE) d3dtexdesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ || D3D10_CPU_ACCESS_WRITE;
		else
		{
			CON(MSG_ERR, _W("Texture creation failed. No apropriate CPUAccess given."));
			return false;
		} 

		d3dtexdesc.MiscFlags = 0;
		if(desc.BindFlag == P3D_TEXBINDFLAG_TEXTURE3D) d3dtexdesc.MiscFlags |= D3D10_RESOURCE_MISC_TEXTURECUBE;
		else if(desc.AutoGenerateMips) 
		{ 
			d3dtexdesc.MiscFlags |= D3D10_RESOURCE_MISC_GENERATE_MIPS ;
			/*D3D10_RESOURCE_MISC_GENERATE_MIPS requires that the D3D10_BIND_RENDER_TARGET & D3D10_BIND_SHADER_RESOURCE flag be set.
			WTF?? */
			if(desc.BindFlag != P3D_TEXBINDFLAG_RENDER_TARGET)  d3dtexdesc.BindFlags |= D3D10_BIND_RENDER_TARGET; 
		}
		
		D3D10_SUBRESOURCE_DATA subdata;
		subdata.pSysMem = data;
		subdata.SysMemPitch = len;

		//TODO: enable initial data
		//if(!CheckHRResult(g_pD3ddev->CreateTexture2D(&d3dtexdesc, data != 0 ? &subdata : NULL, &texture))) return false;
		if(!CheckHRResult(g_pD3ddev->CreateTexture2D(&d3dtexdesc, NULL, &m_pPhysicalTexture))) return false;
		if(data)
			g_pD3ddev->UpdateSubresource(m_pPhysicalTexture, D3D10CalcSubresource(0, 0, m_desc.MipLevels), NULL, data, len / m_desc.Height, 0);

		//create a shader resource view
		D3D10_SHADER_RESOURCE_VIEW_DESC srwd;
		if(useExtraDesc)
		{
			srwd.Format = dxRVFormat;
			//TODO: make it available for arrays and 3D textures
			srwd.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
			srwd.Texture2D.MipLevels = d3dtexdesc.MipLevels;
			srwd.Texture2D.MostDetailedMip = 0; //what is this?
		}
		
		if(!CheckHRResult(g_pD3ddev->CreateShaderResourceView(m_pPhysicalTexture, useExtraDesc ? &srwd : 0, &m_pTexture))) 
		{
			//SAFE_RELEASE(texture);
			return false;
		}

		if(desc.AutoGenerateMips) g_pD3ddev->GenerateMips(m_pTexture);

		//create a render target if requested
		if(desc.BindFlag == P3D_TEXBINDFLAG_RENDER_TARGET)
		{
			if(!CheckHRResult(g_pD3ddev->CreateRenderTargetView(m_pPhysicalTexture, 0, &m_pRenderTarget))) 
			{
				//SAFE_RELEASE(texture);
				SAFE_RELEASE(m_pTexture);
				return false;
			}
		}
		
		//create a depth buffer if requested
		if(desc.BindFlag == P3D_TEXBINDFLAG_DEPTH_STENCIL)
		{
			D3D10_DEPTH_STENCIL_VIEW_DESC dsvd;
			if(useExtraDesc)
			{
				dsvd.Format = dxDSFormat;
				dsvd.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
				dsvd.Texture2D.MipSlice = 0;
			}

			if(!CheckHRResult(g_pD3ddev->CreateDepthStencilView(m_pPhysicalTexture, useExtraDesc ? &dsvd : 0, &m_pDepthBuffer))) 
			{
				//SAFE_RELEASE(texture);
				SAFE_RELEASE(m_pTexture);
				return false;
			}
		}

		m_desc = desc;

		return true;
	}

	bool CTextureWrapper::SetMipMapData(UINT texID, UINT miplevel, BYTE *data, unsigned int len)
	{
		//g_pD3ddev->UpdateSubresource(m_pTexture->GetResource(), D3D10CalcSubresource(miplevel, texID, m_desc.MipLevels), NULL, data, len / m_desc.Height, 0);
		return true;
	}

	void CTextureWrapper::OnLostDevice()
	{
		//nothing
	}

	void CTextureWrapper::OnResetDevice()
	{
		//nothing
	}

};

#endif
