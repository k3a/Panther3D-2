/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once
#include "IWindow.h"
#include <SDL/SDL.h>

namespace P3D
{

///Renderer default window implementation using SDL
class CGraphicsWindow : public IWindow
{
public:
	CGraphicsWindow();
	~CGraphicsWindow();
	void* GetHandle()const;
	bool IsNativeWindow()const{ return false; };
	bool CanGoFullscreen()const{ return true; };
	sWEvent* GetEvents(OUT unsigned int &numEvents);
	Vec2i GetMousePos()const{ return m_mousePos; };
	Vec2i GetSize()const{return m_size;};
	unsigned int GetBPP()const{ return SDL_GetVideoInfo()->vfmt->BitsPerPixel; };
	void SetSize(Vec2i wsize, bool fullscreen=false);
	// --- only in renderer project
	bool Create(const wchar* caption, Vec2i size, bool fullscreen = false);
	bool IsCreated()const{return m_bCreated;};
	void HideWindow(bool hide=true);
	void SetMenuMode(bool bMenu);
private:
	bool m_bMenuMode;
	Vec2i m_mousePos;
	Vec2i m_size;
	bool m_bCreated;
	sWEvent* m_outev;
	void* m_hwnd;
	unsigned long m_x11win;
	SDL_Surface *m_screen;
};

}; //namespace
