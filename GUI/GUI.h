/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "IGUI.h"

// external modules
#include "IEngine.h"
//#include "IRenderer.h" // is part of IEngine.h

// subclasses
#include "GUIScheme.h"
#include "GUIWindow.h"

namespace P3D
{

/// Main GUI module class
class CGUI : public IGUI
{
public:
	~CGUI();
	bool Initialize(void* pEngine);
	void Render(float deltaTime);
	void Update(float deltaTime);

	IGUIWindow* CreateWindow(const wchar* caption, Vec2i pos, Vec2i size, bool showInTaskBar=true);

	// --- subclasses
	DECLARE_SUBCLASS_EX(void, GUIScheme); // subclass without interface ;)

	// --- external modules
	static inline IEngine* mEngine(){ return s_pEngine; };
	static inline IRenderer* mRenderer(){ return s_pRenderer; };
private:
	tArray<CGUIWindow*> m_windows;
	// - external modules
	static IEngine* s_pEngine;
	static IRenderer* s_pRenderer;
};

#define CON(type, text, ...) CGUI::mEngine()->iConsole()->AddMsgEx(type, _W(__FUNCTION__), text, ##__VA_ARGS__)

}; // namespace

