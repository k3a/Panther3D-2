/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "GUI.h"
#include "GUIWindow.h"

namespace P3D
{

REGISTER_MODULE_CLASS(CGUI, _W("gui"), _W("internal"));

// - external modules
IEngine* CGUI::s_pEngine=NULL;
IRenderer* CGUI::s_pRenderer=NULL;

//-----------------------------------------
bool CGUI::Initialize(void* pEngineV)
{
	// memory leaks detection
#if defined(_DEBUG) && defined(_MSC_VER) && _MSC_VER >= 800
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(111);
#endif

	s_pEngine = (IEngine*) pEngineV;
	s_pRenderer = mEngine()->mRenderer();

	INIT_SUBCLASS(GUIScheme);
	return true;
}

//-----------------------------------------
CGUI::~CGUI()
{
	// release all window guis
	for (unsigned int i=0; i<m_windows.Size(); i++)
		SAFE_DELETE(m_windows[i]);

	UNLOAD_SUBCLASS(GUIScheme);
}

//-----------------------------------------------
IGUIWindow* CGUI::CreateWindow(const wchar* caption, Vec2i pos, Vec2i size, bool showInTaskBar)
{
	CGUIWindow *win = new CGUIWindow(caption, pos, size, showInTaskBar);
	m_windows.AddEx(win);
	return win;
}

//-----------------------------------------------
void CGUI::Update(float deltaTime)
{
	Vec2i mousePos;
	mEngine()->iInputManager()->GetMousePos(mousePos); // get new mouse pos

	// update all windows
	for (unsigned int i=0; i<m_windows.Size(); i++)
	{
		m_windows[i]->Update(deltaTime);
	}
}

//------------------------------------------------
void CGUI::Render(float deltaTime)
{
	// draw all windows
	for (unsigned int i=0; i<m_windows.Size(); i++)
		if (m_windows[i]->IsVisible()) 
			m_windows[i]->Render(deltaTime);

	// draw mouse cursor
//	mRenderer()->iPrimitiveRenderer()->AddLine(g_mousePos, g_mousePos+10, ColorRGBA(0, 1, 1, 1));

	mRenderer()->iPrimitiveRenderer()->Render();
}

}; // namespace
