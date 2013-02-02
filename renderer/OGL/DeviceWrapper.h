/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "types.h"
#include "IGraphicsDevice.h"

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
		void SetViewport(Rect vp);
		void Drop();
		void* GetInternalDevice()const;
		int GetDeviceHash()const{return m_hash;};

		void BeginRendering(UINT NumRenderTargets, ITexture *RenderTargets, ITexture *DepthStencil);
		void BeginRendering(UINT NumRenderTargets, ITexture *RenderTargets);
		void BeginRendering();
		void EndRendering() { /*nothing*/ };

		// render state objects
		void* CreateBlendStateInternal(const sBlendDesc* bd);
		void ReleaseBlendStateInternal(void* rapi_state=NULL);
		void SetBlendStateInternal(const sBlendDesc* bd, void* rapi_state=NULL);
		void* CreateRasterizerStateInternal(const sRasterizerDesc* rd);
		void ReleaseRasterizerStateInternal(void* rapi_state=NULL);
		void SetRasterizerStateInternal(const sRasterizerDesc* rd, void* rapi_state=NULL);
	public:
		// these methods have to be in every wrapper
		bool Restart();
	private:

		HDC m_hDC;
		HGLRC m_hRC;
		GLuint m_frameBuffer; //Rendering to textures

		sBlendDesc m_lastBlend;
		sRasterizerDesc m_lastRasterizer;

		// capabilities
		bool m_bATC; // alpha-to-coverage

		sGraphicsCardInfo m_cardInfo;

		int m_hash; // 256hash created from device name
	};

};
