/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#include "Device.h"
#include "symbols.h"
#include "../Renderer.h" // because of CON macro

namespace P3D
{

	LPDIRECT3DDEVICE9 g_pD3ddev = 0;

	CDeviceWrapper::CDeviceWrapper()
	{
		m_pD3ddev = 0;
		m_pD3d = 0;
		m_pZbuffer = 0;
		ZeroMemory(&m_sD3dpp, sizeof(m_sD3dpp));
		//MessageBox(NULL, "aaa", "IMPORTANT MESSAGE!", MB_OK);
	}

	CDeviceWrapper::~CDeviceWrapper()
	{
		Drop();
	}

	bool CDeviceWrapper::Create(unsigned int width, unsigned int height, bool fullscreen, void* handle)
	{
		CON(MSG_INFO, " ...creating DirectX device with backbuffer of %d x %d %s", width, height, fullscreen?"fullscreen":"windowed");

		m_pD3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(!m_pD3d) return false;

		m_pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_sMode);
		if(!fullscreen)
		{
			if(SUCCEEDED(m_pD3d->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_sMode.Format, m_sMode.Format, true)))
			{
				m_sD3dpp.Windowed = TRUE;
				m_sD3dpp.BackBufferFormat = m_sMode.Format;
			}
			else return false;
		}

		m_sD3dpp.SwapEffect = fullscreen ? D3DSWAPEFFECT_DISCARD : D3DSWAPEFFECT_COPY;
		m_sD3dpp.hDeviceWindow = (HWND)handle;    
		m_sD3dpp.BackBufferWidth = width;    
		m_sD3dpp.BackBufferHeight = height;
		m_sD3dpp.EnableAutoDepthStencil = TRUE;
		m_sD3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		m_sD3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		m_sD3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //maximum fps

		D3DCAPS9 caps;
		m_pD3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
		DWORD vp;
		if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		//TODO: pure device support

		m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)handle, vp, &m_sD3dpp, &m_pD3ddev);
		if(!m_pD3ddev) return false;

		//TODO: multisampling support
		m_pD3ddev->CreateDepthStencilSurface(width, height, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pZbuffer, NULL);
		if(!m_pZbuffer) return false;

		CON(MSG_INFO, " ...device sucessfuly created");

		SetupD3D();
		g_pD3ddev = m_pD3ddev;
		
		return true;
	}

	void CDeviceWrapper::SetupD3D()
	{
		//FIXME: some of these parameters could be changed on runtime
		m_pD3ddev->SetRenderState(D3DRS_LIGHTING, false);
		m_pD3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
		m_pD3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(0, 0, 0));
		m_pD3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);    // handle normals in scaling// ambient light
		m_pD3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pD3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pD3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		m_pD3ddev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pD3ddev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pD3ddev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	}

	void CDeviceWrapper::Clear(ColorValueRGB color)
	{
		//TODO: [am!go] To clear stencil buffer but it shouldn't be cleared when we are not using it
		m_pD3ddev->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(int(color.r * 255), int(color.g * 255), int(color.b * 255)), 1.0f, 0);
	}

	///DEBUG:!!!
	struct SCREENVERT
	{
		float x, y, z, rhw; // The transformed position for the vertex
		unsigned long color;        // The vertex color
	};
	///DEBUG:!!!
	struct Vert3D
	{
		float x, y, z; // The transformed position for the vertex
		unsigned long color;        // The vertex color
	};

	void CDeviceWrapper::BeginScene()
	{
		m_pD3ddev->BeginScene();

		///DEBUG:!!!

		/*
		m_pD3ddev->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
		SCREENVERT verts[3];
		verts[0].x = 60; verts[0].y = 120; verts[0].z = 0; verts[0].rhw = 1; verts[0].color = COLOR_RED;
		verts[1].x = 100; verts[1].y = 60; verts[1].z = 0; verts[1].rhw = 1; verts[1].color = COLOR_GREEN;
		verts[2].x = 140; verts[2].y = 120; verts[2].z = 0; verts[2].rhw = 1; verts[2].color = COLOR_BLUE;
		m_pD3ddev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, verts, sizeof(SCREENVERT));


		m_pD3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pD3ddev->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
		Vert3D verts3[3];
		verts3[0].x = -1; verts3[0].y = 1; verts3[0].z = 0.1f; verts3[0].color = COLOR_RED;
		verts3[1].x = 0; verts3[1].y = -1; verts3[1].z = 0.1f; verts3[1].color = COLOR_GREEN;
		verts3[2].x = 1; verts3[2].y = 1; verts3[2].z = 0.1f; verts3[2].color = COLOR_BLUE;
		m_pD3ddev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, verts3, sizeof(Vert3D));
		*/
	}

#include <windows.h>

	void CDeviceWrapper::EndScene(void* hwndTarget)
	{
		m_pD3ddev->EndScene();

		RECT r;
		GetClientRect(HWND(hwndTarget), &r);

		m_pD3ddev->Present(&r, 0, (HWND)hwndTarget, 0);
	}

	void CDeviceWrapper::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height){
		D3DVIEWPORT9 vp;
		vp.X = x; vp.Y = y; vp.Width = width; vp.Height = height;
		vp.MinZ = 0; vp.MaxZ = 1;
		m_pD3ddev->SetViewport(&vp);
	}

	void CDeviceWrapper::Drop()
	{
		SAFE_RELEASE(m_pD3ddev);
		SAFE_RELEASE(m_pD3d);
		SAFE_RELEASE(m_pZbuffer);
		ZeroMemory(&m_sD3dpp, sizeof(m_sD3dpp));
		g_pD3ddev = 0;
	}


	void* CDeviceWrapper::GetInternalDevice()const
	{
		return m_pD3ddev;
	}

};