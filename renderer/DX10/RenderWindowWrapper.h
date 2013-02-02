/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "types.h"
#include <DX/D3D10.h>
#include "IRenderWindow.h"

namespace P3D
{

	class CRenderWindowWrapper : public IRenderWindow
	{
	public:
		CRenderWindowWrapper();
		~CRenderWindowWrapper();
		bool Create(const sRenderWindowDesc &desc, void *winHandle);
		inline sRenderWindowDesc GetDesc() { return m_desc; };

		//DO NOT use this methods outside the wrapper part of the renderer.
		inline ID3D10RenderTargetView* GetBackBuffer() { return m_pRenderTargetView; };
		inline ID3D10DepthStencilView* GetDepthBuffer() { return NULL; };
		inline IDXGISwapChain* GetInternal() { return m_pSwapChain; };

		//void OnResize(Vec2i newSize);

	protected:
		IDXGISwapChain*  m_pSwapChain;
		ID3D10RenderTargetView* m_pRenderTargetView;
		ID3D10DepthStencilView* m_pDepthStencil;
		sRenderWindowDesc m_desc;
	};

};