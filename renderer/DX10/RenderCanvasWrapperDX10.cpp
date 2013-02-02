/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#if defined(_DX) && _DX == 10

#include "RenderCanvasWrapper.h"
#include "symbols.h"
#include "../Renderer.h"

namespace P3D
{

	extern ID3D10Device* g_pD3ddev;
	extern IDXGIFactory* g_pDXGIFactory;

	CRenderCanvasWrapper::CRenderCanvasWrapper()
	{
		m_pSwapChain = NULL;
		m_pRenderTargetView = NULL;
		m_pDepthStencil = NULL;
	}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
	CRenderCanvasWrapper::~CRenderCanvasWrapper()
	{
		SAFE_RELEASE(m_pSwapChain);
		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pDepthStencil);
	}

	bool CRenderCanvasWrapper::Create(const sRenderCanvasDesc &desc, void *winHandle)
	{
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width  = desc.size.x;
		sd.BufferDesc.Height = desc.size.y;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count   = 1; // No multisampling.
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount  = 1;
		sd.OutputWindow = (HWND)winHandle;
		sd.Windowed     = desc.windowed;
		sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags        = 0;

		if(!CheckHRResult( g_pDXGIFactory->CreateSwapChain(
			g_pD3ddev,
			&sd,
			&m_pSwapChain
			) )) return false;


		ID3D10Texture2D* backBuffer;
		if(!CheckHRResult( m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer)) ))
		{
			SAFE_RELEASE(m_pSwapChain);
			return false;
		}
		if(!CheckHRResult( g_pD3ddev->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView) ))
		{
			SAFE_RELEASE(m_pSwapChain);
			backBuffer->Release();
			return false;
		}
		backBuffer->Release();

		if(!CreateDepthBuffer(desc.size))
		{
			SAFE_RELEASE(m_pSwapChain);
			return false;
		}

		m_desc = desc;

		return true;
	}

	bool CRenderCanvasWrapper::CreateDepthBuffer(const Vec2i &size)
	{
		D3D10_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width     = size.x;
		depthStencilDesc.Height    = size.y;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
		depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
		depthStencilDesc.Usage          = D3D10_USAGE_DEFAULT;
		depthStencilDesc.BindFlags      = D3D10_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0; 
		depthStencilDesc.MiscFlags      = 0;

		ID3D10Texture2D* depthBuffer;
		if(!CheckHRResult( g_pD3ddev->CreateTexture2D(&depthStencilDesc, 0, &depthBuffer) )) return false;
		if(!CheckHRResult( g_pD3ddev->CreateDepthStencilView(depthBuffer, 0, &m_pDepthStencil) ))
		{
			depthBuffer->Release();
			return false;
		}
		depthBuffer->Release();

		return true;
	}

	bool CRenderCanvasWrapper::OnResize(P3D::Vec2i newSize)
	{
		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pDepthStencil);
		
		//resize back buffer
		if(!CheckHRResult( m_pSwapChain->ResizeBuffers(1, newSize.x, newSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0) )) return false;

		//and acquire it back
		ID3D10Texture2D* backBuffer;
		if(!CheckHRResult( m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer)) )) return false;
		if(!CheckHRResult( g_pD3ddev->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView) ))
		{
			backBuffer->Release();
			return false;
		}
		backBuffer->Release();

		return true;
	}


};

#endif
