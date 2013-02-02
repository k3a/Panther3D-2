/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "GraphicsManager.h"
#include "GraphicsDevice.h"
#include "ShaderManager.h"
#include "TextRenderer.h"
#include "SpriteRenderer.h"
#include "GraphicsRenderer.h"
//#include "IRenderer.h"
#include "IEngine.h"

#include "profiler.h"	//PROBABLY TEMPORARY

namespace P3D
{

class CRenderer : public IRenderer
{
public:
	CRenderer();
	~CRenderer();
	bool Initialize(void* pEngine, CDoubleCommandQueue *dcq);
	eAPI GetAPI()const;
	void NextFrame();

	// --- events
	void AddEventListener(IRendererEventListener* list){
		m_listeners.push_back(list);
	};
	void EV_Loading(const wchar* desc){
		for (unsigned int i=0; i<m_listeners.size(); i++)
			m_listeners[i]->RendererLoading(desc);
	};

	static void OnLostDevice();
	static void OnResetDevice();

	// --- singletons
	DECLARE_SUBCLASS(GraphicsManager);
	DECLARE_SUBCLASS(GraphicsDevice);
	DECLARE_SUBCLASS(ShaderManager);
	DECLARE_SUBCLASS(TextRenderer);
	DECLARE_SUBCLASS(PrimitiveRenderer);
	DECLARE_SUBCLASS(SpriteRenderer);
	DECLARE_SUBCLASS(GraphicsRenderer);

	static inline CRenderer* instance(){return s_pRenderer;};

	static inline CProfiler *cProfiler(){ assert(m_bInit==true); return s_pProfiler; };

	// --- external singletons
	static inline IEngine* mEngine(){ return s_pEngine; };
private:
	static bool m_bInit;
	static CRenderer* s_pRenderer;
	std::vector<IRendererEventListener*> m_listeners;
	CDoubleCommandQueue *m_queue;
	// - - from shared
	static CProfiler *s_pProfiler;
	// - external singletons
	static IEngine* s_pEngine;
};

#define CON(type, text, ...) CRenderer::mEngine()->iConsole()->AddMsgEx(type, _W(__FUNCTION__), text, ##__VA_ARGS__)
#define PROFILERLOCATION CRenderer::cProfiler()

};
