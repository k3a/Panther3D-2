/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#if defined(_DX) && _DX == 10

#include "DeviceWrapper.h"
#include "symbols.h"
#include "../Renderer.h" // because of CON macro

#include "ShaderHLSL.h"

namespace P3D
{
		ID3D10Device* g_pD3ddev = NULL;
		IDXGIFactory* g_pDXGIFactory = NULL;

		CDeviceWrapper::CDeviceWrapper()
		{
			m_pD3dDevice = NULL;
			m_pDXGIFactory = NULL;
			m_pSwapChain = NULL;
			m_hash=0;
			m_pRenderTargetView = NULL;
			m_pDepthStencilView = NULL;
			m_pDepthStencilBuffer = NULL;
			m_pDefaultSampler = NULL;
			m_pTargetWindow = NULL;
		}

		CDeviceWrapper::~CDeviceWrapper()
		{
			Drop();
		}

		bool CDeviceWrapper::Create(unsigned int width, unsigned int height, bool fullscreen, void* window)
		{
			CON(MSG_INFO, _W(" ...creating Direct3D device with backbuffer of %d x %d %s"), width, height, fullscreen?_W("fullscreen"):_W("windowed"));

			if (!window) CON(MSG_ERR_FATAL, _W("Calling Create() with window=NULL! Device must be initialized with window!"));

			CRenderer::instance()->EV_Loading(_W("Loading DirectX..."));

			//Enable PerfHUD
			if(!CheckHRResult(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pDXGIFactory)))
			{
				if (!window) CON(MSG_ERR_FATAL, _W("Cannot create IDXGIFactory!"));
				return false;
			}
			// Search for a PerfHUD adapter.
			UINT nAdapter = 0;
			IDXGIAdapter* adapter = NULL;
			IDXGIAdapter* selectedAdapter = NULL;
			D3D10_DRIVER_TYPE driverType = D3D10_DRIVER_TYPE_HARDWARE;
			while (m_pDXGIFactory->EnumAdapters(nAdapter, &adapter) != DXGI_ERROR_NOT_FOUND)
			{
				if (adapter)
				{
					DXGI_ADAPTER_DESC adaptDesc;
					if (SUCCEEDED(adapter->GetDesc(&adaptDesc)))
					{
						const bool isPerfHUD = wcscmp(adaptDesc.Description, _W("NVIDIA PerfHUD")) == 0;
						// Select the first adapter in normal circumstances or the PerfHUD one if it exists.
						if(nAdapter == 0 || isPerfHUD) selectedAdapter = adapter;
						if(isPerfHUD) driverType = D3D10_DRIVER_TYPE_REFERENCE;
					}
				}
				++nAdapter;
			}
			//SAFE_RELEASE(pDXGIFactory);
			//End Enable PerfHUD


			DXGI_SWAP_CHAIN_DESC sd;
			sd.BufferDesc.Width  = width;
			sd.BufferDesc.Height = height;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			//DXGI_FORMAT_D24_UNORM_S8_UINT
			sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			// No multisampling.
			sd.SampleDesc.Count   = 1;
			sd.SampleDesc.Quality = 0;
			sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount  = 1;
			sd.OutputWindow = (HWND)window;
			sd.Windowed     = true;
			sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
			sd.Flags        = 0;

			UINT createDeviceFlags = 0;
			#if defined(DEBUG) || defined(_DEBUG)  
			createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
			#endif
			createDeviceFlags |= D3D10_CREATE_DEVICE_SINGLETHREADED; //should be way faster

			CRenderer::instance()->EV_Loading(_W("Creating DirectX Device..."));

			if(!CheckHRResult(D3D10CreateDevice(
				selectedAdapter,
				driverType,
				NULL,
				createDeviceFlags,
				D3D10_SDK_VERSION,
				&m_pD3dDevice
				))) return false;

			if(!CheckHRResult(m_pDXGIFactory->CreateSwapChain(
				m_pD3dDevice,
				&sd,
				&m_pSwapChain
				))) return false;



			CON(MSG_INFO, _W(" ...using Direct3D 10"));
			DXGI_ADAPTER_DESC adapterdesc;
			selectedAdapter->GetDesc(&adapterdesc);
			CON(MSG_INFO, _W(" ...using renderer %s"), adapterdesc.Description);
			m_hash = Str(adapterdesc.Description).Hash256();
			CON(MSG_INFO, _W(" ...dedicated video memory  %dMB"), adapterdesc.DedicatedVideoMemory/1000000);
			CON(MSG_INFO, _W(" ...shared system memory  %dMB"), adapterdesc.SharedSystemMemory/1000000);


			ID3D10Texture2D* backBuffer;
			m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer));
			m_pD3dDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView);
			backBuffer->Release();

			// Create the depth/stencil buffer and view.
			D3D10_TEXTURE2D_DESC depthStencilDesc;
			depthStencilDesc.Width     = width;
			depthStencilDesc.Height    = height;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
			depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
			depthStencilDesc.Usage          = D3D10_USAGE_DEFAULT;
			depthStencilDesc.BindFlags      = D3D10_BIND_DEPTH_STENCIL;
			depthStencilDesc.CPUAccessFlags = 0; 
			depthStencilDesc.MiscFlags      = 0;

			m_pD3dDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer);
			m_pD3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView);

			m_pD3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

			D3D10_VIEWPORT vp;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			vp.Width    = width;
			vp.Height   = height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;

			m_pD3dDevice->RSSetViewports(1, &vp);

			SetupD3D();
			g_pD3ddev = m_pD3dDevice;
			g_pDXGIFactory = m_pDXGIFactory;

			/////////testing section////////////////
// 			sShaderDesc vsdesc;
// 			vsdesc.EntryFunction.SetEx(_W("psmain"));
// 			vsdesc.ShaderName.SetEx(_W("test"));
// 			vsdesc.ShaderFile.SetEx(_W("PrimitiveRenderer"));
// 			vsdesc.ShaderType = SHADERTYPE_PIXEL_SHADER;
// 			//vsdesc.VertexDescID = CRenderer::cGraphicsManager()->GetVertexDescIDByName(Str(_W("VDPosUVCol")));
// 			CShaderHLSL *testshader = new CShaderHLSL;
// 			testshader->Create(vsdesc);
// 			delete testshader;
			/////////end testing section////////////////

			return true;
		}

		void CDeviceWrapper::SetupD3D()
		{
			D3D10_SAMPLER_DESC sdesc;
			sdesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
			sdesc.AddressU = sdesc.AddressV = sdesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
			sdesc.MipLODBias = 0;
			sdesc.MaxAnisotropy = 1; //up to 16
			sdesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS; //take a look a this
			sdesc.BorderColor[0] = 0.0f; //green for debug purposes
			sdesc.BorderColor[1] = 1.0f;
			sdesc.BorderColor[2] = 0.0f;
			sdesc.BorderColor[3] = 1.0f;
			sdesc.MinLOD = 0;
			sdesc.MaxLOD = D3D10_FLOAT32_MAX;

			m_pD3dDevice->CreateSamplerState(&sdesc, &m_pDefaultSampler);
			ID3D10SamplerState *ss[1];
			ss[0] = m_pDefaultSampler;
			m_pD3dDevice->PSSetSamplers(0, 1, ss);
			m_pD3dDevice->GSSetShader( NULL );
		}

		void CDeviceWrapper::Clear(ColorRGB color)
		{
			float ccolor[4] = {color.r, color.g, color.b, 1.0f};
			m_pD3dDevice->ClearRenderTargetView(m_pRenderTargetView, ccolor);
			m_pD3dDevice->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
		}

		void CDeviceWrapper::BeginScene(CRenderCanvas* wndTarget)
		{
			m_pTargetWindow = wndTarget;
		}

		void CDeviceWrapper::EndScene()
		{
			if(!m_pTargetWindow) // render to the default location
				m_pSwapChain->Present(0, 0);
			else m_pTargetWindow->GetInternal()->Present(0, 0);
		}

		void CDeviceWrapper::SetViewport(const P3D::Rect &vp)
		{	
			D3D10_VIEWPORT dvp;
			dvp.MinDepth = 0.0f;
			dvp.MaxDepth = 1.0f;
			dvp.TopLeftX = vp.x1; 
			dvp.TopLeftY = vp.y1; 
			dvp.Width = vp.x2 - vp.x1; 
			dvp.Height = vp.y2 - vp.y1;
			m_pD3dDevice->RSSetViewports(1, &dvp);
		}

		void CDeviceWrapper::Drop()
		{
			SAFE_RELEASE(m_pDepthStencilBuffer);
			SAFE_RELEASE(m_pRenderTargetView);
			SAFE_RELEASE(m_pDepthStencilView);
			SAFE_RELEASE(m_pSwapChain);
			SAFE_RELEASE(m_pDefaultSampler);
			SAFE_RELEASE(m_pDXGIFactory);
			unsigned int references = 0;
			if(m_pD3dDevice)
			{
				references = m_pD3dDevice->Release();
				m_pD3dDevice = 0;
			}
			m_pD3dDevice = 0;
			if(references) CON(MSG_ERR, _W("There is/are %d unreleased D3D reference(s)!"), references);
			else CON(MSG_DBG, _W("All D3D references released"));
		}

		void CDeviceWrapper::BeginRendering()
		{
			ID3D10RenderTargetView *rt[1] = { m_pRenderTargetView };

			if(!m_pTargetWindow) // use default back buffer
			{
				rt[0] = m_pRenderTargetView;
				m_pD3dDevice->OMSetRenderTargets(1, rt, m_pDepthStencilView);
			}
			else
			{
				rt[0] = m_pTargetWindow->GetBackBuffer();
				m_pD3dDevice->OMSetRenderTargets(1, rt, m_pTargetWindow->GetDepthBuffer());
			}

			Clear(ColorRGB(0.0f, 0.0f, 0.5f));
		}

		void CDeviceWrapper::BeginRendering(UINT NumRenderTargets, ITexture *RenderTargets)
		{
			ID3D10RenderTargetView *rts[4];
			for(UINT i=0; i<NumRenderTargets; i++) 
			{
				CTexture *tex = (CTexture*)RenderTargets;
				rts[i] = (ID3D10RenderTargetView*)tex->GetRenderTarget();
			}
			m_pD3dDevice->OMSetRenderTargets(NumRenderTargets, rts, m_pDepthStencilView);
			Clear(ColorRGB(0.0f, 0.5f, 0.0f));
		}

		void CDeviceWrapper::BeginRendering(UINT NumRenderTargets, P3D::ITexture *RenderTargets, P3D::ITexture *DepthStencil)
		{
			ID3D10RenderTargetView *rts[4];
			for(UINT i=0; i<NumRenderTargets; i++) 
			{
				CTexture *tex = (CTexture*)RenderTargets;
				rts[i] = (ID3D10RenderTargetView*)tex->GetRenderTarget();
			}
			CTexture *depthtex = (CTexture*)DepthStencil;
			m_pD3dDevice->OMSetRenderTargets(1, rts, (ID3D10DepthStencilView*)depthtex->GetDepthStencil());
			Clear(ColorRGB(0.0f, 0.0f, 0.0f));
		}

		void CDeviceWrapper::EndRendering()
		{

		}

		bool CDeviceWrapper::Restart()
		{
			//TODO: resize backbuffer, etc..
			return true;
		}

		D3D10_BLEND Blend2D3D(eBlend b)
		{
			switch (b)
			{
			case BLEND_ZERO: return D3D10_BLEND_ZERO;
			case BLEND_ONE: return D3D10_BLEND_ONE;
			case BLEND_SRC_COLOR: return D3D10_BLEND_SRC_COLOR;
			case BLEND_INV_SRC_COLOR: return D3D10_BLEND_INV_SRC_COLOR;
			case BLEND_SRC_ALPHA: return D3D10_BLEND_SRC_ALPHA;
			case BLEND_INV_SRC_ALPHA: return D3D10_BLEND_INV_SRC_ALPHA;
			case BLEND_DEST_COLOR: return D3D10_BLEND_DEST_ALPHA;
			case BLEND_INV_DEST_COLOR: return D3D10_BLEND_INV_DEST_COLOR;
			case BLEND_DEST_ALPHA: return D3D10_BLEND_DEST_ALPHA;
			case BLEND_INV_DEST_ALPHA: return D3D10_BLEND_INV_DEST_ALPHA;
			default: assert(!"Unknown blend source!");
			}
			return D3D10_BLEND_ONE;
		}

		void* CDeviceWrapper::CreateBlendStateInternal(const sBlendDesc* bd)
		{
			assertd(bd!=0, "none or invalid blend description given");
			D3D10_BLEND_DESC bdesc;
			
			memset(&bdesc, 0, sizeof(D3D10_BLEND_DESC));
			bdesc.SrcBlendAlpha = D3D10_BLEND_ONE;
			bdesc.DestBlendAlpha = D3D10_BLEND_ZERO;
			bdesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;

			bdesc.AlphaToCoverageEnable = bd->AlphaToCoverageEnable;
			for(BYTE b=0; b<8; b++) bdesc.BlendEnable[b] = bd->BlendEnable[b];
			switch(bd->BlendOp)
			{
			case BLEND_OP_ADD:
				bdesc.BlendOp = D3D10_BLEND_OP_ADD;
				break;
			case BLEND_OP_SUBTRACT:
				bdesc.BlendOp = D3D10_BLEND_OP_SUBTRACT;
				break;
			case BLEND_OP_REV_SUBTRACT:
				bdesc.BlendOp = D3D10_BLEND_OP_REV_SUBTRACT;
				break;
			case BLEND_OP_MIN:
				bdesc.BlendOp = D3D10_BLEND_OP_MIN;
				break;
			case BLEND_OP_MAX:
				bdesc.BlendOp = D3D10_BLEND_OP_MAX;
				break;
			}
			bdesc.DestBlend = Blend2D3D(bd->DestBlend);
			for(BYTE b=0; b<8; b++) bdesc.RenderTargetWriteMask[b] = bd->RenderTargetWriteMask[b];
			bdesc.SrcBlend = Blend2D3D(bd->SrcBlend);

			ID3D10BlendState *bstate;
			if(!CheckHRResult(m_pD3dDevice->CreateBlendState(&bdesc, &bstate))) return NULL;
			
			return bstate;
		}

		void CDeviceWrapper::ReleaseBlendStateInternal(void* rapi_state)
		{
			ID3D10BlendState *bstate = (ID3D10BlendState*)rapi_state;
			SAFE_RELEASE(bstate);
		}
		void CDeviceWrapper::SetBlendStateInternal(const sBlendDesc* bd, void* rapi_state)
		{
			ID3D10BlendState *bstate = (ID3D10BlendState*)rapi_state;
			float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f}; //TODO: make this adjustable?
			m_pD3dDevice->OMSetBlendState(bstate, blendFactors, 0xffffffff);
		}

		void* CDeviceWrapper::CreateRasterizerStateInternal(const sRasterizerDesc* rd)
		{
			assertd(rd!=0, "none or invalid rasterizer description given");
			D3D10_RASTERIZER_DESC rdesc;
			rdesc.AntialiasedLineEnable = rd->AntialiasedLineEnable;
			switch(rd->CullMode)
			{
			case CULL_NONE:
				rdesc.CullMode = D3D10_CULL_NONE;
			case CULL_FRONT:
				rdesc.CullMode = D3D10_CULL_FRONT;
			case CULL_BACK:
				rdesc.CullMode = D3D10_CULL_BACK;
			}
			rdesc.DepthBias = rd->DepthBias;
			if(rd->Wireframe) rdesc.FillMode = D3D10_FILL_WIREFRAME;
			else rdesc.FillMode = D3D10_FILL_SOLID;
			rdesc.MultisampleEnable = rd->MultisampleEnable;
			rdesc.ScissorEnable = rd->ScissorEnable;
			rdesc.SlopeScaledDepthBias = rd->SlopeScaledDepthBias;

			ID3D10RasterizerState *rstate;
			if(!CheckHRResult(m_pD3dDevice->CreateRasterizerState(&rdesc, &rstate))) return NULL;
			
			return rstate;
		}

		void CDeviceWrapper::ReleaseRasterizerStateInternal(void* rapi_state)
		{
			ID3D10RasterizerState *rstate = (ID3D10RasterizerState*)rapi_state;
			SAFE_RELEASE(rstate);
		}

		void CDeviceWrapper::SetRasterizerStateInternal(const sRasterizerDesc* rd, void* rapi_state)
		{
			m_pD3dDevice->RSSetState((ID3D10RasterizerState*)rapi_state);
		}

}

#endif