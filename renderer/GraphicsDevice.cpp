/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, am!go
*/
#include "precompiled.h"

#include "GraphicsDevice.h"
#include "Renderer.h"

namespace P3D
{

	CGraphicsDevice::CGraphicsDevice()
	{
		m_viewMatrix.SetIdentityMatrix();
		m_projectionMatrix.SetIdentityMatrix();
	}

	//------------------------------------------
	CGraphicsDevice::~CGraphicsDevice()
	{		
		for(UINT i=0; i<m_renderCanvases.Size(); i++) SAFE_DELETE(m_renderCanvases[i]);
	
	}

	//------------------------------------------
	bool CGraphicsDevice::Initialize(unsigned int width, unsigned int height, bool fullscreen, void* primaryWindowHandle)
	{

		if(!Create(width, height, fullscreen, primaryWindowHandle)) return false;
		
		CShaderManager *sm = CRenderer::cShaderManager();
		#if defined(_DX) && _DX==9
		if(!sm->Create(m_pD3ddev)) return false;
		#elif defined(_DX) && _DX==10
		if(!sm->Create(m_pD3dDevice)) return false;
		#elif defined(_OGL)
		if(!sm->Create(NULL)) return false;
		#endif

		CGraphicsManager *gm = CRenderer::cGraphicsManager();
		gm->Create();

		CPrimitiveRenderer *pr = CRenderer::cPrimitiveRenderer();
		pr->Init();

		CSpriteRenderer *sr = CRenderer::cSpriteRenderer();
		sr->Create();

		/*ITexture *loadingtex = CRenderer::cGraphicsManager()->LoadTexture("loading.jpg");
		pr->AddRectangleRel(Vec2(-1.f, 1.0f), Vec2(1.0f, -1.0f), ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), loadingtex);
		BeginScene(0, ColorRGB(0.0f, 1.0f, 1.0f));
		pr->Render();
		EndScene();
		CRenderer::cGraphicsManager()->DeleteTexture((CTexture*)loadingtex);*/

		CTextRenderer *tr = CRenderer::cTextRenderer();
		tr->Create();

		CRenderer::cGraphicsRenderer()->Initialize();

		return true;
	}

	//------------------------------------------
	void CGraphicsDevice::BeginScene(IRenderCanvas* wndTarget, ColorRGB color)
	{
		PROFILESAMPLE(BeginScene);
		CDeviceWrapper::BeginScene((CRenderCanvas*)wndTarget);
	}

	

	//------------------------------------------
	void CGraphicsDevice::EndScene()
	{
		PROFILESAMPLE(EndScene);

		// FIXME: decide where to call scene rendering. It should be based on scene and wiewport
		CRenderer::cGraphicsRenderer()->RenderScene();

		//CRenderer::cTextRenderer()->Render();
		CDeviceWrapper::EndScene();
	}

	IRenderCanvas* CGraphicsDevice::CreateRenderWindow(const sRenderCanvasDesc &desc, void *windowHandle)
	{
		CRenderCanvas *rc = new CRenderCanvas;
		if(!rc->Create(desc, windowHandle))
		{
			delete rc;		
			return NULL;
		}

		m_renderCanvases.AddEx(rc);
		
		return rc;
	}

	void CGraphicsDevice::DestroyRenderWindow(IRenderCanvas *renderWindow)
	{
// 		assertd( renderWindow!=0 , "NULL pointer given!" );
// 		CRenderWindow *rw = (CRenderWindow*)renderWindow;
// 		
// 		int index = m_renderWindows.FindIndex(rw);
// 		if(index != -1)
// 		{
// 			m_renderWindows.Remove( (UINT)index );
// 			SAFE_DELETE(rw);
// 		}
	}
//---------------------------------------------------------------------------
	ID CGraphicsDevice::CreateRenderCanvas(const sRenderCanvasDesc &desc, void *windowHandle)
	{
		CRenderCanvas *rc = new CRenderCanvas;
		if(!rc->Create(desc, windowHandle))
		{
			delete rc;		
			return -1;
		}
		
		m_renderCanvases.AddEx(rc);
		return (int)m_renderCanvases.Size() - 1;
	}
//---------------------------------------------------------------------------
	void CGraphicsDevice::DeleteRenderCanvas(ID canvasId)
	{
		SAFE_DELETE(m_renderCanvases[canvasId]);
		m_renderCanvases.Remove( (UINT)canvasId );
	}

// ******************* RENDER STATE OBJECTS ************************************************************************************************

#pragma region Render states

	bool CompareArray(const bool A[], const bool B[], int num)
	{
		for(int i=0; i<num; i++)
			if (A[i]!=B[i]) return false;
		return true;
	}

	bool CompareArray(const BYTE A[], const BYTE B[], int num)
	{
		for(int i=0; i<num; i++)
			if (A[i]!=B[i]) return false;
		return true;
	}

	IBlendState*  CGraphicsDevice::CreateBlendState(const sBlendDesc* blendDesc)
	{
		assertd(blendDesc!=0, "Attempt to create render state object with NULL descriptor!");

		// first try to find existing state object
		for (unsigned long i=0; i<m_blend_objs.size(); i++)
		{
			CBlendState *istat = m_blend_objs[i];
			const sBlendDesc A = *istat->GetDesc();
			const sBlendDesc B = (const sBlendDesc)*blendDesc;
			if (A.AlphaToCoverageEnable == B.AlphaToCoverageEnable &&
				A.SrcBlend == B.SrcBlend &&/* A.SrcBlendAlpha == B.SrcBlendAlpha &&*/
				A.DestBlend == B.DestBlend &&/* A.DestBlendAlpha == B.DestBlendAlpha &&*/
				A.BlendOp == B.BlendOp &&/* A.BlendOpAlpha==B.BlendOpAlpha && */
				CompareArray(A.BlendEnable, B.BlendEnable, 8) && CompareArray(A.RenderTargetWriteMask, B.RenderTargetWriteMask, 8)) 
			{
				// found - increase reference count and return
				istat->AddRef();
				return (IBlendState*)istat;
			}
		}

		// make structure for state object and assign descriptor
		CBlendState* pnt = new CBlendState();

		pnt->SetDesc(blendDesc, this, CDeviceWrapper::CreateBlendStateInternal(blendDesc));

		m_blend_objs.push_back(pnt);

		return pnt;
	}

	void CGraphicsDevice::SetBlendState(const IBlendState* blendState)
	{
		assert(blendState!=NULL && "Trying to set NULL blend state to device!");
		((CBlendState*)blendState)->SetState();
	}

	IRasterizerState*  CGraphicsDevice::CreateRasterizerState(const sRasterizerDesc* rasterizerDesc)
	{
		assertd(rasterizerDesc!=0, "Attempt to create render state object with NULL descriptor!");

		// first try to find existing state object
		for (unsigned long i=0; i<m_rasterizer_objs.size(); i++)
		{
			CRasterizerState *istat = m_rasterizer_objs[i];
			const sRasterizerDesc A = *istat->GetDesc();
			const sRasterizerDesc B = (const sRasterizerDesc)*rasterizerDesc;
			if (A.Wireframe==B.Wireframe && A.CullMode==B.CullMode && 
				A.DepthBias==B.DepthBias && A.SlopeScaledDepthBias==B.SlopeScaledDepthBias &&
				A.DepthBufferEnable==B.DepthBufferEnable && A.DepthBufferWriteEnable==B.DepthBufferWriteEnable && 
				A.ScissorEnable==B.ScissorEnable &&	A.MultisampleEnable==B.MultisampleEnable && 
				A.AntialiasedLineEnable==B.AntialiasedLineEnable) 
			{
				// found - increase reference count and return
				istat->AddRef();
				return (IRasterizerState*)istat;
			}
		}

		// make structure for state object and assign descriptor
		CRasterizerState* pnt = new CRasterizerState();

		pnt->SetDesc(rasterizerDesc, this, CDeviceWrapper::CreateRasterizerStateInternal(rasterizerDesc));

		m_rasterizer_objs.push_back(pnt);

		return pnt;
	}

	void CGraphicsDevice::SetRasterizerState(const IRasterizerState* rasterizerState)
	{
		assertd(rasterizerState!=NULL, "Trying to set NULL rasterizer state to device!");
		((CRasterizerState*)rasterizerState)->SetState();
	}

#pragma endregion //Render state management similar to DirectX 10
};
