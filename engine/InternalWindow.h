/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 09/04/2009 File created - kexik
*/
#pragma once

#include "types.h"

#if defined(XMapWindow)
#define X_WIN_PRESENT
#elif defined(_WIN32)
// handled by _WIN32 define
#else
# error Unknown window implementation (TODO: Consoles)
#endif

namespace P3D
{

class CInternalWindow
{
public:
	CInternalWindow();
	~CInternalWindow();

	bool Create(Vec2i size);
	void SetVisible(bool bVisible=true);
	void* GetHandle()const{return m_handle;};
	bool Destroy();

	bool BeginSplash();
	bool EndSplash();

	void Resize();
	void Update();

private:
	bool m_bCreated;
#ifdef _WIN32
	HWND m_handle;
	HWND m_hsplash;
#elif defined(X_WIN_PRESENT)
	Display m_display;
	Window m_window;
	Window m_SplashWindow;
#endif
};

}; // ns