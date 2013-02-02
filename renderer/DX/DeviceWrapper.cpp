/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#if defined(_DX) && _DX == 9

#include "DeviceWrapper.h"
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
		//CON(MSG_INFO, "dw constructor");
		m_hash=0;

		m_pBackBuffer = NULL;
	}

	CDeviceWrapper::~CDeviceWrapper()
	{
		Drop();
	}

	bool CDeviceWrapper::Create(unsigned int width, unsigned int height, bool fullscreen, void* window)
	{
		unsigned int adapter = D3DADAPTER_DEFAULT;
		D3DDEVTYPE	deviceType	= D3DDEVTYPE_HAL;
		bool software = false; // software emulation?

		CON(MSG_INFO, _W(" ...creating Direct3D device with backbuffer of %d x %d %s, adapter %u"), width, height, fullscreen?_W("fullscreen"):_W("windowed"), adapter);

		if (!window) CON(MSG_ERR_FATAL, _W("Calling Create() with window=NULL! Device must be initialized with window!"));

		CRenderer::instance()->EV_Loading(_W("Loading DirectX..."));
		m_pD3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(!m_pD3d)
		{
			CON(MSG_ERR_FATAL, _W("DirectX not available! Please read readme.txt for instruction how to install it."));
			return false;
		}

//Enable PerfHUD
		#define USEPERFHUD

		#ifdef USEPERFHUD 
		UINT adapnum = m_pD3d->GetAdapterCount();
		for (UINT Adapter=0; Adapter < m_pD3d->GetAdapterCount(); Adapter++)
		{
			D3DADAPTER_IDENTIFIER9 Identifier;
			HRESULT Res;
			Res = m_pD3d->GetAdapterIdentifier(Adapter,0,&Identifier);
			if(strstr(Identifier.Description, "PerfHUD") != 0)
			{
				adapter=Adapter;
				deviceType=D3DDEVTYPE_REF;
				CON(MSG_INFO, _W("PerfHUD enabled!"));
				break;
			}
		}
		#endif
//END Enable PerfHUD

		if (software) deviceType = D3DDEVTYPE_REF;

		m_pD3d->GetAdapterDisplayMode(adapter, &m_sMode);
		if(!fullscreen)
		{
			if(SUCCEEDED(m_pD3d->CheckDeviceType(adapter, deviceType, m_sMode.Format, m_sMode.Format, true)))
			{
				m_sD3dpp.Windowed = TRUE;
				m_sD3dpp.BackBufferFormat = m_sMode.Format;
			}
			else
			{
				CON(MSG_ERR_FATAL, _W("Device doesn't support windowed mode!"));
				return false;
			}
		}

		D3DADAPTER_IDENTIFIER9 adapident;
		if ( FAILED(m_pD3d->GetAdapterIdentifier(adapter, 0, &adapident)) )
		{
			CON(MSG_ERR_FATAL, _W("Could not get adapter identifier!"));
		}
		CON(MSG_INFO, _W(" ...using Direct3D 9.0c, sdk version %u"), D3D_SDK_VERSION);
		CON(MSG_INFO, _W(" ...using renderer %s"), _A2W(adapident.Description));
		m_hash = Str(adapident.Description).Hash256();

		m_sD3dpp.SwapEffect = fullscreen ? D3DSWAPEFFECT_DISCARD : D3DSWAPEFFECT_COPY;
		m_sD3dpp.hDeviceWindow = (HWND)window;
		m_sD3dpp.BackBufferCount = 1;
		m_sD3dpp.BackBufferWidth = width;
		m_sD3dpp.BackBufferHeight = height;
		m_sD3dpp.Windowed = !fullscreen;
		m_sD3dpp.EnableAutoDepthStencil = TRUE;
		m_sD3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		m_sD3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		m_sD3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //maximum fps
		m_sD3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		D3DCAPS9 caps;
		if (FAILED(m_pD3d->GetDeviceCaps(adapter, deviceType, &caps)))
			CON(MSG_ERR_FATAL, _W("Could not get device capabilities!"));

		DWORD vp;
		if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		if (software) vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		// -- check for capabilities
		D3DDISPLAYMODE	currentDisplayMode;
		if( m_pD3d->GetAdapterDisplayMode(adapter,&currentDisplayMode) )
				CON(MSG_ERR_FATAL, _W("Failed to get current adapter display mode!"));
		
		m_cardInfo.maxTextureResolution = min(caps.MaxTextureWidth, caps.MaxTextureHeight);
		m_cardInfo.nonPow2Textures = (caps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL) ? true : false;
		m_cardInfo.independentBitDepth = (caps.PrimitiveMiscCaps & D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS) ? true : false;
		m_cardInfo.maxRenderTargets = caps.NumSimultaneousRTs;

		if(FAILED(m_pD3d->CheckDeviceFormat(
					adapter,
					deviceType,
					currentDisplayMode.Format,
					D3DUSAGE_RENDERTARGET,
					D3DRTYPE_TEXTURE,
					D3DFMT_A16B16G16R16F
					)))
				m_cardInfo.supports16bitRT = false;
		else
				m_cardInfo.supports16bitRT = true;

		if(FAILED(m_pD3d->CheckDeviceFormat(
					adapter,
					deviceType,
					currentDisplayMode.Format,
					D3DUSAGE_RENDERTARGET,
					D3DRTYPE_TEXTURE,
					D3DFMT_R32F
					)))
				m_cardInfo.supports32bitRT = false;
		else
				m_cardInfo.supports32bitRT = true;


		m_cardInfo.floatingPointFiltering = SUCCEEDED(
				m_pD3d->CheckDeviceFormat(
					adapter,
					deviceType,
					currentDisplayMode.Format,
					D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_FILTER,
					D3DRTYPE_TEXTURE,
					m_cardInfo.supports16bitRT ? D3DFMT_A16B16G16R16F : D3DFMT_A32B32G32R32F
					)
				);

		if(FAILED(m_pD3d->CheckDeviceFormat(
						adapter,
						deviceType,
						currentDisplayMode.Format,
						D3DUSAGE_RENDERTARGET,
						D3DRTYPE_TEXTURE,
						D3DFMT_G16R16F
						)))
			CON(MSG_ERR_FATAL, _W("Device doesn't support D3DFMT_G16R16F format!"));

		m_cardInfo.hwShadowMaps = SUCCEEDED(
				m_pD3d->CheckDeviceFormat(
					adapter,
					deviceType,
					currentDisplayMode.Format,
					D3DUSAGE_DEPTHSTENCIL,
					D3DRTYPE_TEXTURE,
					D3DFMT_D24X8)) && SUCCEEDED(
				m_pD3d->CheckDeviceFormat(
					adapter,
					deviceType,
					currentDisplayMode.Format,
					D3DUSAGE_DEPTHSTENCIL,
					D3DRTYPE_TEXTURE,
					D3DFMT_D24S8)) && SUCCEEDED(
				m_pD3d->CheckDeviceFormat(
					adapter,
					deviceType,
					currentDisplayMode.Format,
					D3DUSAGE_DEPTHSTENCIL,
					D3DRTYPE_TEXTURE,
					D3DFMT_D16));

		// DXT support
		m_cardInfo.supportsDXTCompression = 
			!FAILED( m_pD3d->CheckDeviceFormat( 
						adapter,
						deviceType,
						currentDisplayMode.Format,
						0,
						D3DRTYPE_TEXTURE,
						(D3DFORMAT)MAKEFOURCC('D', 'X', 'T', '5'))  )
		&&	!FAILED( m_pD3d->CheckDeviceFormat( 
						adapter,
						deviceType,
						currentDisplayMode.Format,
						0,
						D3DRTYPE_TEXTURE,
						(D3DFORMAT)MAKEFOURCC('D', 'X', 'T', '1'))  );

		// 3Dc texture compression support
		m_cardInfo.supports3DcCompression = 
			!FAILED( m_pD3d->CheckDeviceFormat( 
						adapter,
						deviceType,
						currentDisplayMode.Format,
						0,
						D3DRTYPE_TEXTURE,
						(D3DFORMAT)MAKEFOURCC('A', 'T', 'I', '2'))  );

		CRenderer::instance()->EV_Loading(_W("Creating DirectX Device..."));

		m_pD3d->CreateDevice(adapter, deviceType, (HWND)window, vp, &m_sD3dpp, &m_pD3ddev);
		if(!m_pD3ddev) 
		{
			CON(MSG_ERR_FATAL, _W("Failed to create device!"));
			return false;
		}

		CON(MSG_INFO, _W("  Renderer capabilities:"));
		CON(MSG_INFO, _W("   * Max texture size: %d"), m_cardInfo.maxTextureResolution );
		CON(MSG_INFO, _W("   * Non power-of-two textures: %s"), m_cardInfo.nonPow2Textures ?_W("yes"):_W("no"));
		//CON(MSG_INFO, _W("   * Multisampling and ATC: %s"), caps. ?_W("yes"):_W("no"));
		//CON(MSG_INFO, _W("   * ATC???: %s"), IsExtensionSupported("GL_NV_multisample_coverage")?_W("yes"):_W("no"));
		CON(MSG_INFO, _W("   * Max render targets: %d"), caps.NumSimultaneousRTs);
		CON(MSG_INFO, _W("   * DXT compression: %s"), m_cardInfo.supportsDXTCompression ?_W("yes"):_W("no") );
		CON(MSG_INFO, _W("   * 3Dc compression: %s)"), m_cardInfo.supports3DcCompression ? _W("yes"):_W("no") );
		CON(MSG_INFO, _W("   * Hardware shadow maps: %s"), m_cardInfo.hwShadowMaps ?_W("yes"):_W("no"));
		CON(MSG_INFO, _W("   * Floating-point texture filtering: %s"), m_cardInfo.floatingPointFiltering ?_W("yes"):_W("no"));
		CON(MSG_INFO, _W("   * 16bit ARGB FP RT: %s"), m_cardInfo.supports16bitRT ?_W("yes"):_W("no"));
		CON(MSG_INFO, _W("   * 32bit R FP RT: %s"), m_cardInfo.supports32bitRT ?_W("yes"):_W("no"));
		CON(MSG_INFO, _W("   * Independent bit depth: %s"), m_cardInfo.independentBitDepth ?_W("yes"):_W("no"));

		//TODO: multisampling support
		m_pD3ddev->CreateDepthStencilSurface(width, height, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pZbuffer, NULL);
		if(!m_pZbuffer)
		{
			CON(MSG_ERR, _W("Unable to create depth stencil surface!"));
			return false;
		}

		//CON(MSG_INFO, " ...device sucessfuly created");

		SetupD3D();
		g_pD3ddev = m_pD3ddev;

		//backup the back buffer
		m_pD3ddev->GetRenderTarget(0, &m_pBackBuffer);

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
		//m_pD3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		//m_pD3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		m_pD3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		m_pD3ddev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pD3ddev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pD3ddev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		//m_pD3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		//m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	void CDeviceWrapper::Clear(ColorRGB color)
	{
		//TODO: [am!go] To clear stencil buffer but it shouldn't be cleared when we are not using it
		m_pD3ddev->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(int(color.r * 255), int(color.g * 255), int(color.b * 255)), 1.0f, 0);
	}


	

	void CDeviceWrapper::BeginScene(IWindow *wndTarget)
	{
		m_win = wndTarget;

		//is device lost?
		HRESULT hr = m_pD3ddev->TestCooperativeLevel();

		while(hr == D3DERR_DEVICELOST)
		{
			Sleep(20);
			hr = m_pD3ddev->TestCooperativeLevel();
		}

		if(hr == D3DERR_DRIVERINTERNALERROR)
		{
			CON(MSG_ERR_FATAL, _W("Internal driver error!"));
			return;
		}
		else if( hr == D3DERR_DEVICENOTRESET )
		{
			Restart();
		}

		//m_pD3ddev->BeginScene();

	}

#include <windows.h>

	void CDeviceWrapper::EndScene()
	{
		// check for valid IWindow
		if (!m_win) {
			CON(MSG_ERR, _W("You must specify IWindow pointer in BeginScene() first!"));
			return;
		}

		// check for HWND availability
		if (!m_win->GetHandle()) {
			CON(MSG_ERR, _W("Can't get HWND handle from IWindow specified in BeginScene()! There is nowhere to render to!"));
			return;
		}

		//m_pD3ddev->EndScene();

		RECT r;
		//GetClientRect((HWND)m_win->GetHandle(), &r);
		Vec2i sz = m_win->GetSize();
		r.left = r.top = 0;
		r.right = sz.x;
		r.bottom = sz.y;
		m_pD3ddev->Present(&r, 0, (HWND)m_win->GetHandle(), 0);

	}

	void CDeviceWrapper::SetViewport(P3D::Rect vp){
		D3DVIEWPORT9 dvp;
		dvp.X = vp.x1; 
		dvp.Y = vp.y1; 
		dvp.Width = vp.x2 - vp.x1; 
		dvp.Height = vp.y2 - vp.y1;
		dvp.MinZ = 0.0f; 
		dvp.MaxZ = 1.0f;
		m_pD3ddev->SetViewport(&dvp);
	}

	void CDeviceWrapper::Drop()
	{
		SAFE_RELEASE(m_pBackBuffer);

		SAFE_RELEASE(m_pD3d);
		SAFE_RELEASE(m_pZbuffer);
		ZeroMemory(&m_sD3dpp, sizeof(m_sD3dpp));
		
		//SAFE_RELEASE(m_pD3ddev);
		unsigned int references = 0;
		if(m_pD3ddev)
		{
			references = m_pD3ddev->Release();
			m_pD3ddev = 0;
		}
		g_pD3ddev = 0;
		if(references) CON(MSG_ERR, _W("There is/are %d unreleased D3D reference(s)!"), references);
	}


	void* CDeviceWrapper::GetInternalDevice()const
	{
		return m_pD3ddev;
	}

	bool CDeviceWrapper::Restart()
	{
		CON(MSG_INFO, _W("Restarting Device..."));
		SAFE_RELEASE(m_pZbuffer);

		m_sD3dpp.BackBufferWidth = CRenderer::cGraphicsWindow()->GetSize().x;
		m_sD3dpp.BackBufferHeight = CRenderer::cGraphicsWindow()->GetSize().y;
		CON(MSG_INFO, _W(" ...creating Direct3D device with backbuffer of %d x %d"), m_sD3dpp.BackBufferWidth, m_sD3dpp.BackBufferHeight);

		if(m_pD3ddev->Reset(&m_sD3dpp) != D3D_OK)
		{
			CON(MSG_ERR_FATAL, _W("Device restart Failed!"));
			//m_bRunning = false;
			return false;
		}

		m_pD3ddev->CreateDepthStencilSurface(m_sD3dpp.BackBufferWidth, m_sD3dpp.BackBufferHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pZbuffer, NULL);
		if(!m_pZbuffer) 
		{
			CON(MSG_ERR_FATAL, _W("Device restart Failed!"));		
			return false;
		}
		SetupD3D();

		CON(MSG_INFO, _W("Device restart succesful"));
		return true;

	}

	void CDeviceWrapper::BeginRendering()
	{
		m_pD3ddev->SetDepthStencilSurface(m_pZbuffer);
		m_pD3ddev->SetRenderTarget(0, m_pBackBuffer);
		for(UINT i=1; i<m_cardInfo.maxRenderTargets; i++) m_pD3ddev->SetRenderTarget(i, 0);
		Clear(ColorRGB(0.0f, 0.0f, 0.0f));
		m_pD3ddev->BeginScene();
	}

	void CDeviceWrapper::BeginRendering(UINT NumRenderTargets, ITexture *RenderTargets)
	{
		m_pD3ddev->SetDepthStencilSurface(m_pZbuffer);
		//m_pD3ddev->SetRenderTarget(0, m_pBackBuffer);
		CTexture *tex = (CTexture*)RenderTargets;
		m_pD3ddev->SetRenderTarget(0, (IDirect3DSurface9*)tex->GetRenderTarget());
		for(UINT i=NumRenderTargets; i<m_cardInfo.maxRenderTargets; i++) m_pD3ddev->SetRenderTarget(i, 0);
		Clear(ColorRGB(0.0f, 0.5f, 0.0f));
		m_pD3ddev->BeginScene();
	}

	void CDeviceWrapper::BeginRendering(UINT NumRenderTargets, P3D::ITexture *RenderTargets, P3D::ITexture *DepthStencil)
	{
		if(DepthStencil)
		{
			CTexture *depthtex = (CTexture*)DepthStencil;
			m_pD3ddev->SetDepthStencilSurface((IDirect3DSurface9*)depthtex->GetRenderTarget());
		}
		else m_pD3ddev->SetDepthStencilSurface(0);

		CTexture *tex = (CTexture*)RenderTargets;
		m_pD3ddev->SetRenderTarget(0, (IDirect3DSurface9*)tex->GetRenderTarget());
		for(UINT i=NumRenderTargets; i<m_cardInfo.maxRenderTargets; i++) m_pD3ddev->SetRenderTarget(i, 0);
		Clear(ColorRGB(0.0f, 0.0f, 0.0f));
		m_pD3ddev->BeginScene();
	}

	// **************************************** RENDER STATE OBJECTS *****************************************************

	DWORD Blend2D3D(eBlend b)
	{
		switch (b)
		{
		case BLEND_ZERO: return D3DBLEND_ZERO;
		case BLEND_ONE: return D3DBLEND_ONE;
		case BLEND_SRC_COLOR: return D3DBLEND_SRCCOLOR;
		case BLEND_INV_SRC_COLOR: return D3DBLEND_INVSRCCOLOR;
		case BLEND_SRC_ALPHA: return D3DBLEND_SRCALPHA;
		case BLEND_INV_SRC_ALPHA: return D3DBLEND_INVSRCALPHA;
		case BLEND_DEST_COLOR: return D3DBLEND_DESTCOLOR;
		case BLEND_INV_DEST_COLOR: return D3DBLEND_INVDESTCOLOR;
		case BLEND_DEST_ALPHA: return D3DBLEND_DESTALPHA;
		case BLEND_INV_DEST_ALPHA: return D3DBLEND_INVDESTALPHA;
		default: assert(!"Unknown blend source!");
		}
		return D3DBLEND_ONE;
	}

	void* CDeviceWrapper::CreateBlendStateInternal(const sBlendDesc* bd)
	{
		return NULL; // DX doesn't have state objects, so we don't need internal pointer to it
	}

	void CDeviceWrapper::ReleaseBlendStateInternal(void* rapi_state)
	{
		// nothing for OGL
	}
	void CDeviceWrapper::SetBlendStateInternal(const sBlendDesc* bd, void* rapi_state)
	{
		// apply render state changes

		//TODO: alpha-to-cverage under DX9??
		if (bd->AlphaToCoverageEnable != m_lastBlend.AlphaToCoverageEnable) 
		{
			if (bd->AlphaToCoverageEnable) 
			{
				/*if (m_bATC)
					glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
				else
				{
					glEnable(GL_ALPHA_TEST);
					glAlphaFunc(GL_GREATER, 0.5f);
				}*/
				m_pD3ddev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x08);
				m_pD3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 
				m_pD3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
			}
			else
			{
				/*if (m_bATC)
					glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
				else
				{*/
					m_pD3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE); 
				//}
			}

			m_lastBlend.AlphaToCoverageEnable = bd->AlphaToCoverageEnable;
		}

		// TODO: what about next 7 render targets???!!!
		if (!bd->BlendEnable[0] && m_lastBlend.BlendEnable[0]) 
		{
			m_pD3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_lastBlend.BlendEnable[0]=false;
		}
		else if (bd->BlendEnable[0])
		{
			if (!m_lastBlend.BlendEnable[0]) 
			{
				m_pD3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				m_lastBlend.BlendEnable[0]=true;
			}

			if (bd->BlendOp!=m_lastBlend.BlendOp) 
			{
				if (bd->BlendOp==BLEND_OP_MIN) m_pD3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_MIN);
				else if (bd->BlendOp==BLEND_OP_MAX) m_pD3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_MAX);
				else if (bd->BlendOp==BLEND_OP_ADD) m_pD3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				else if (bd->BlendOp==BLEND_OP_SUBTRACT) m_pD3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
				else if (bd->BlendOp==BLEND_OP_REV_SUBTRACT) m_pD3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
				m_lastBlend.BlendOp = bd->BlendOp;
			}

			if (bd->SrcBlend!=m_lastBlend.SrcBlend || bd->DestBlend!=m_lastBlend.DestBlend) 
			{
				DWORD src; DWORD dest;
				src = Blend2D3D(bd->SrcBlend);
				dest = Blend2D3D(bd->DestBlend);
				m_pD3ddev->SetRenderState(D3DRS_SRCBLEND, src);
				m_pD3ddev->SetRenderState(D3DRS_DESTBLEND, dest);
				m_lastBlend.SrcBlend = bd->SrcBlend;
				m_lastBlend.DestBlend = bd->DestBlend;
			}
		}

		if (bd->RenderTargetWriteMask[0]!=m_lastBlend.RenderTargetWriteMask[0])
		{
			m_pD3ddev->SetRenderState(D3DRS_COLORWRITEENABLE, bd->RenderTargetWriteMask[0]);
			m_lastBlend.RenderTargetWriteMask[0] = bd->RenderTargetWriteMask[0];
		}
		if (bd->RenderTargetWriteMask[1]!=m_lastBlend.RenderTargetWriteMask[1])
		{
			m_pD3ddev->SetRenderState(D3DRS_COLORWRITEENABLE1, bd->RenderTargetWriteMask[1]);
			m_lastBlend.RenderTargetWriteMask[1] = bd->RenderTargetWriteMask[1];
		}
		if (bd->RenderTargetWriteMask[2]!=m_lastBlend.RenderTargetWriteMask[2])
		{
			m_pD3ddev->SetRenderState(D3DRS_COLORWRITEENABLE2, bd->RenderTargetWriteMask[2]);
			m_lastBlend.RenderTargetWriteMask[2] = bd->RenderTargetWriteMask[2];
		}
		if (bd->RenderTargetWriteMask[3]!=m_lastBlend.RenderTargetWriteMask[3])
		{
			m_pD3ddev->SetRenderState(D3DRS_COLORWRITEENABLE3, bd->RenderTargetWriteMask[3]);
			m_lastBlend.RenderTargetWriteMask[3] = bd->RenderTargetWriteMask[3];
		}
	}

	void* CDeviceWrapper::CreateRasterizerStateInternal(const sRasterizerDesc* rd)
	{
		return NULL; // DX9 doesn't have state objects, so we don't need internal pointer to it
	}

	void CDeviceWrapper::ReleaseRasterizerStateInternal(void* rapi_state)
	{
		// nothing for DirectX9
	}

	void CDeviceWrapper::SetRasterizerStateInternal(const sRasterizerDesc* rd, void* rapi_state)
	{
		if (rd->Wireframe != m_lastRasterizer.Wireframe) m_pD3ddev->SetRenderState(D3DRS_FILLMODE, rd->Wireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID);

		if (rd->CullMode != m_lastRasterizer.CullMode) 
		{
			if (rd->CullMode==CULL_NONE) 
				 m_pD3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			else
			{
				m_pD3ddev->SetRenderState(D3DRS_CULLMODE, rd->CullMode==CULL_FRONT ? D3DCULL_CW : D3DCULL_CCW);
			}
		}

		if (rd->DepthBias!=m_lastRasterizer.DepthBias || rd->SlopeScaledDepthBias!=m_lastRasterizer.SlopeScaledDepthBias)
		{
			if (rd->DepthBias==0 && rd->SlopeScaledDepthBias==0) 
			{
				m_pD3ddev->SetRenderState(D3DRS_DEPTHBIAS, 0);
				m_pD3ddev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
			}
			else
			{
				m_pD3ddev->SetRenderState(D3DRS_DEPTHBIAS, rd->DepthBias);
				m_pD3ddev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, (DWORD)rd->SlopeScaledDepthBias);
			}
		}

		if (rd->ScissorEnable!=m_lastRasterizer.ScissorEnable)
		{
			if (rd->ScissorEnable)
				m_pD3ddev->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
			else
				m_pD3ddev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		}

		if (rd->DepthBufferEnable != m_lastRasterizer.DepthBufferEnable) 
		{
			if (rd->DepthBufferEnable)
				m_pD3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
			else
				m_pD3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);
		}

		if (rd->DepthBufferWriteEnable != m_lastRasterizer.DepthBufferWriteEnable) 
		{
			if (rd->DepthBufferWriteEnable)
				m_pD3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			else
				m_pD3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}

		if (rd->AntialiasedLineEnable != m_lastRasterizer.AntialiasedLineEnable)
		{
			// TODO: ANTIALIASED LINES
		/*	if (rd->AntialiasedLineEnable)
			{
				// temporarily enable blend as it is needed for antialiased line
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_LINE_SMOOTH); 
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			}
			else
			{
				glDisable(GL_LINE_SMOOTH); 
				glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

				// return blending to last blend state
				if (!m_lastBlend.BlendEnable[0]) glDisable(GL_BLEND);
				GLenum src; GLenum dest;
				src = Blend2GL(m_lastBlend.SrcBlend);
				dest = Blend2GL(m_lastBlend.DestBlend);
				glBlendFunc(src, dest);
			}*/
		}

		if (rd->MultisampleEnable!=m_lastRasterizer.MultisampleEnable)
		{
			// TODO: MULTISAMPLE
			/*if (rd->MultisampleEnable)
				glEnable(GL_MULTISAMPLE_ARB);
			else
				glDisable(GL_MULTISAMPLE_ARB);*/
		}

		// copy actual as last...
		m_lastRasterizer=*rd;
	}

	
};

#endif
