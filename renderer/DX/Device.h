/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include <d3dx9.h>
#include "types.h"

namespace P3D
{

	class CDeviceWrapper
	{
	public:
		CDeviceWrapper();
		~CDeviceWrapper();
		bool Create(unsigned int width, unsigned int height, bool fullscreen, void* handle);
		void Clear(ColorValueRGB color);
		void BeginScene();
		void EndScene(void *hwndTarget=NULL);
		void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
		void Drop();
		
		void* GetInternalDevice()const;
	private:
		LPDIRECT3DDEVICE9 m_pD3ddev;
		LPDIRECT3D9 m_pD3d;
		LPDIRECT3DSURFACE9 m_pZbuffer;
		D3DPRESENT_PARAMETERS m_sD3dpp;
		D3DDISPLAYMODE m_sMode;

		void SetupD3D();
	};

};