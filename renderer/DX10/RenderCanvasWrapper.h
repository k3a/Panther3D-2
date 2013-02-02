/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "types.h"
#include <DX/D3D10.h>
#include "IRenderCanvas.h"

namespace P3D
{

	/// \see CRenderCanvas
	class CRenderCanvasWrapper : public IRenderCanvas
	{
	public:
		inline sRenderCanvasDesc GetDesc() { return m_desc; };

		//DO NOT use this methods outside the wrapper part of the renderer.
		inline ID3D10RenderTargetView* GetBackBuffer() { return m_pRenderTargetView; };
		inline ID3D10DepthStencilView* GetDepthBuffer() { return NULL; };
		inline IDXGISwapChain* GetInternal() { return m_pSwapChain; };

		bool OnResize(Vec2i newSize);

	protected:
		CRenderCanvasWrapper();
		~CRenderCanvasWrapper();
		bool Create(const sRenderCanvasDesc &desc, void *winHandle);
		sRenderCanvasDesc m_desc;
	
	private:
		bool CreateDepthBuffer(const Vec2i &size);
		IDXGISwapChain*  m_pSwapChain;
		ID3D10RenderTargetView* m_pRenderTargetView;
		ID3D10DepthStencilView* m_pDepthStencil;
	};

};