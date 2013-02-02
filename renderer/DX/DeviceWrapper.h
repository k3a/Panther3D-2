/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include <DX/d3d9.h>
#include "types.h"
#include "IGraphicsDevice.h"

#include "IWindow.h"

namespace P3D
{

	class CDeviceWrapper : public IGraphicsDevice
	{
	public:
		CDeviceWrapper();
		~CDeviceWrapper();
		bool Create(unsigned int width, unsigned int height, bool fullscreen, void* window);
		void Clear(ColorRGB color);
		void BeginScene(IWindow* wndTarget);
		void EndScene();
		void SetViewport(P3D::Rect vp);
		void Drop();
		void* GetInternalDevice()const;
		int GetDeviceHash()const{return m_hash;};

		void BeginRendering(UINT NumRenderTargets, ITexture *RenderTargets, ITexture *DepthStencil);
		void BeginRendering(UINT NumRenderTargets, ITexture *RenderTargets);
		void BeginRendering();
		void EndRendering() { m_pD3ddev->EndScene(); };

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
		LPDIRECT3DDEVICE9 m_pD3ddev;
		LPDIRECT3D9 m_pD3d;
		LPDIRECT3DSURFACE9 m_pZbuffer;
		D3DPRESENT_PARAMETERS m_sD3dpp;
		D3DDISPLAYMODE m_sMode;
		IDirect3DQuery9* m_pQuery;

		IDirect3DSurface9 *m_pBackBuffer;

		sBlendDesc m_lastBlend;
		sRasterizerDesc m_lastRasterizer;
		void SetupD3D();

		sGraphicsCardInfo m_cardInfo;

		int m_hash; // 256hash created from device name
		UINT m_numBoundRenderTargets; //how many render targets are currently set up

	private:

	};

};