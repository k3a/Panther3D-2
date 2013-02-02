/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 09/16/2009 File created - Petr
*/
#include "precompiled.h"
#if defined(_DX) && _DX == 9

#include "RenderCanvasWrapper.h"
#include "symbols.h"
#include "../Renderer.h"

namespace P3D
{
	extern LPDIRECT3DDEVICE9 g_pD3ddev;
	
	CRenderCanvasWrapper::CRenderCanvasWrapper()
	{
		m_pSwapChain = NULL;
		m_pDepthStencil = NULL;
	}

	CRenderCanvasWrapper::~CRenderCanvasWrapper()
	{
		SAFE_RELEASE(m_pSwapChain);
		SAFE_RELEASE(m_pDepthStencil);

	}

	bool CRenderCanvasWrapper::Create(const sRenderCanvasDesc &desc, void *winHandle)
	{
		D3DPRESENT_PARAMETERS pp;
		ZeroMemory(pp, sizeof(D3DPRESENT_PARAMETERS) );
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pp.hDeviceWindow = (HWND)winHandle;
		pp.BackBufferCount = 1;
		pp.BackBufferWidth = desc.size.x;
		pp.BackBufferHeight = desc.size.y;
		pp.Windowed = desc.windowed;
		pp.EnableAutoDepthStencil = TRUE;
		pp.AutoDepthStencilFormat = D3DFMT_D24S8;
		pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //maximum fps
		pp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		pp.MultiSampleType = D3DMULTISAMPLE_NONE;
		pp.MultiSampleQuality = 0;
		
		HRESULT CreateAdditionalSwapChain(&pp, &m_pSwapChain);

	}

	bool CRenderCanvasWrapper::OnResize(P3D::Vec2i newSize)
	{

	}


}//NS

#endif