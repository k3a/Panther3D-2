/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include <DX/D3D10.h>
#include "types.h"
#include "IGraphicsDevice.h"

#include "IWindow.h"
#include "../RenderCanvas.h"


namespace P3D
{

	class CDeviceWrapper : public IGraphicsDevice
	{
	public:
		CDeviceWrapper();
		~CDeviceWrapper();
		bool Create(unsigned int width, unsigned int height, bool fullscreen, void* window);
		void Clear(ColorRGB color);
		void BeginScene(CRenderCanvas* wndTarget);
		void EndScene();
		void SetViewport(const Rect &viewport);
		void SetViewports(UINT num, const Rect *viewports); //requires geometry shader and thus not supported using DX9
		void Drop();
		void* GetInternalDevice()const;
		int GetDeviceHash()const{return m_hash;};

		void BeginRendering(UINT NumRenderTargets, ITexture *RenderTargets, ITexture *DepthStencil);
		void BeginRendering(UINT NumRenderTargets, ITexture *RenderTargets);
		void BeginRendering();
		void EndRendering();

		// render state objects
		void* CreateBlendStateInternal(const sBlendDesc* bd);
		void ReleaseBlendStateInternal(void* rapi_state=NULL);
		void SetBlendStateInternal(const sBlendDesc* bd, void* rapi_state=NULL);
		void* CreateRasterizerStateInternal(const sRasterizerDesc* rd);
		void ReleaseRasterizerStateInternal(void* rapi_state=NULL);
		void SetRasterizerStateInternal(const sRasterizerDesc* rd, void* rapi_state=NULL);

		// these methods have to be in every wrapper
		bool Restart();
	protected:
		//variables
		ID3D10Device *m_pD3dDevice;
		IDXGIFactory *m_pDXGIFactory;
		IDXGISwapChain *m_pSwapChain;
		ID3D10RenderTargetView* m_pRenderTargetView;
		ID3D10DepthStencilView* m_pDepthStencilView;
		ID3D10Texture2D* m_pDepthStencilBuffer;
		ID3D10SamplerState *m_pDefaultSampler;
		//methods
		void SetupD3D();

	protected:
		//variables
		CRenderCanvas *m_pTargetWindow;
		sBlendDesc m_lastBlend;
		sRasterizerDesc m_lastRasterizer;
		sGraphicsCardInfo m_cardInfo;
		int m_hash; // 256hash created from device name
		//methods
	};

};