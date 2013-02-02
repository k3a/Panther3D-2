/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "Renderer.h"

#include "ShaderCodeGenerator.h" // DEBUG!!!

namespace P3D
{
	

#if defined(_DX) && _DX == 9
	REGISTER_MODULE_CLASS(CRenderer, _W("renderer"), _W("dx9"));
#elif defined(_DX) && _DX == 10
	REGISTER_MODULE_CLASS(CRenderer, _W("renderer"), _W("dx10"));
#elif defined(_OGL)
	REGISTER_MODULE_CLASS(CRenderer, _W("renderer"), _W("ogl"));
#else
	REGISTER_MODULE_CLASS(CRenderer, _W("renderer"), _W("unknown"));
#endif

CRenderer* CRenderer::s_pRenderer=NULL;

CProfiler *CRenderer::s_pProfiler=NULL;

bool CRenderer::m_bInit=false;

// external: 
IEngine* CRenderer::s_pEngine=NULL;

//------------------------------------------
CRenderer::CRenderer(){
	s_pRenderer = this;
}

//------------------------------------------
CRenderer::~CRenderer()
{
	s_pProfiler->Process();

	// - singletons - order is essential!
	UNLOAD_SUBCLASS(GraphicsRenderer);
	UNLOAD_SUBCLASS(TextRenderer);
	UNLOAD_SUBCLASS(PrimitiveRenderer);
	UNLOAD_SUBCLASS(SpriteRenderer);
	UNLOAD_SUBCLASS(ShaderManager);
	UNLOAD_SUBCLASS(GraphicsManager);
	UNLOAD_SUBCLASS(GraphicsDevice);

	SAFE_DELETE(s_pProfiler);
	CON(MSG_DBG, _W("All Renderer's singletons deleted..."));
}

//------------------------------------------
bool CRenderer::Initialize(void* pEngine, CDoubleCommandQueue* dcq){
	if (m_bInit) return false;
	m_bInit = true;

	CRenderer::instance()->EV_Loading(_W("Initializing Renderer..."));

	// memory leaks detection
#if defined(_DEBUG) && defined(_MSC_VER) && _MSC_VER >= 800
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(76);
#endif

	s_pEngine = (IEngine*)pEngine;
	m_queue = dcq;
	CON(MSG_INFO, _W("= Renderer initialization ="));

	// -------------------- SHADER CODE GENERATION TEST -----------------

	CON(MSG_INFO, _W("SHADER CODE GENERATION TEST") );
	CShaderCodeGenerator* scg = new CShaderCodeGenerator();

	float fp[4];

	sShaderCodeOutput a = scg->CreateValue(SV_FLOAT, 4);
	 CON(MSG_INFO, _W("%s"), a.shaderCode );
    fp[0]=1.2f; fp[1]=2.3f; fp[2]=3.4f; fp[3]=4.5f;
	sShaderCodeOutput ad = scg->SetFloat(a.outputVar, fp, 4);
	 CON(MSG_INFO, _W("%s"), ad.shaderCode );

	sShaderCodeOutput b = scg->CreateValue(SV_FLOAT, 4);
	 CON(MSG_INFO, _W("%s"), b.shaderCode );
    fp[0]=1; fp[1]=2; fp[2]=3; fp[3]=4;
	sShaderCodeOutput bd = scg->SetFloat(b.outputVar, fp, 4);
	 CON(MSG_INFO, _W("%s"), bd.shaderCode );

	sShaderCodeOutput _add = scg->Add(a.outputVar, b.outputVar);
	 CON(MSG_INFO, _W("%s"), _add.shaderCode );

	delete scg;

	// -------------------- SHADER CODE GENERATION TEST END -----------------

	// initialize profiler
	s_pProfiler = new CProfiler(_W("RendererModule"));

	return true;
}

eAPI CRenderer::GetAPI()const
{
#if defined(_DX) && _DX == 9
	return RAPI_DX9;
#elif defined(_DX) && _DX == 10
	return RAPI_DX10;
#elif defined(_OGL)
	return RAPI_OGL;
#else
	return RAPI_NULL;
#endif
}

//------------------------------------------
void CRenderer::NextFrame()
{
	// process queue
	unsigned int ci; const void* ptr; IQueueCommandReceiver *recv=NULL;
	while ( m_queue->ModuleQueue()->Dequeue(ci, ptr, recv) )
	switch(ci)
	{
	case CQI_ENGINE2RENDERER_CANVAS_CREATE:
		{
			/*const sE2RCanvasDesc *cdesc = (sE2RCanvasDesc *)ptr;
			sRenderCanvasDesc rcd;
			rcd.size = cdesc->size;
			rcd.windowed = cdesc->windowed;
			ID canvas = this->cGraphicsDevice()->CreateRenderCanvas(rcd, cdesc->handle);

			sR2EGeneralReply *reply=NULL;
			m_queue->ModuleQueue()->Enqueue(NULL, CQI_RENDERER2ENGINE_GENERAL_REPLY, (void*&)reply, sizeof(sR2EGeneralReply) );
			if (canvas >= 0) {
				reply->id = canvas;
				reply->ok = true;
			}
			else {
				reply->id = canvas;
				reply->ok = false;
			}*/
		}
		break;
	}

	// render scene
// 	cGraphicsDevice->BeginScene(ActiveCanvas);
// 		cGraphicsRenderer->RenderScene(NULL, 0);
// 	cGraphicsDevice->EndScene();
}

//------------------------------------------
void CRenderer::OnLostDevice()
{
	cPrimitiveRenderer()->OnLostDevice();
	cTextRenderer()->OnLostDevice();
	cShaderManager()->OnLostDevice();
	cGraphicsManager()->OnLostDevice();
	cGraphicsRenderer()->OnLostDevice();
}

//------------------------------------------
void CRenderer::OnResetDevice()
{
	cPrimitiveRenderer()->OnResetDevice();
	cTextRenderer()->OnResetDevice();
	cShaderManager()->OnResetDevice();
	cGraphicsManager()->OnResetDevice();
	cGraphicsRenderer()->OnResetDevice();
}

};
