/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 09/04/2009 File created - kexik
*/
#include "precompiled.h"

//#include "InternalWindow.h"
#include "Engine.h"

#if defined(_WIN32) && !defined(CreateWindow)
# ifdef UNICODE
#  define CreateWindow  CreateWindowW
# else
#  define CreateWindow  CreateWindowA
# endif // !UNICODE
#endif

namespace P3D
{

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifdef _WIN32
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_CLOSE:
		ShowWindow(hWnd, 0);
		CEngine::instance()->Shutdown();
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}
#endif

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
CInternalWindow::CInternalWindow()
{
	m_bCreated = false;
}

//---------------------------------------------------------------------------
CInternalWindow::~CInternalWindow()
{
	Destroy();
}

//---------------------------------------------------------------------------
bool CInternalWindow::Create( Vec2i size )
{
	if (m_bCreated) return false;
#ifdef _WIN32

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
		_W("P3DEngineWindow"), NULL
	};
	RegisterClassEx( &wc );

	// Construct window title
	wchar winTitle[256];
	wsprintf( winTitle, 255, _W("%s :: %s"), _W("TechDemo"), CEngine::GetBuildString() );

	// Create the application's window
	m_handle = CreateWindow( _W("P3DEngineWindow"), winTitle,
		WS_OVERLAPPEDWINDOW, 100, 100, size.x, size.y,
		NULL, NULL, wc.hInstance, NULL );

	// Set standard arrow cursor
	SetClassLong( m_handle, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW) ); // standard cursor

	// Set as foreground window
	SetForegroundWindow( m_handle );

	return m_handle!=0;
#elif defined(X_WIN_PRESENT)
	m_display = XOpenDisplay(NULL);
#endif
}

//---------------------------------------------------------------------------
void CInternalWindow::SetVisible( bool bVisible/*=true*/ )
{
#ifdef _WIN32
	if (bVisible)
		ShowWindow( m_handle, SW_SHOWDEFAULT );
	else
		ShowWindow( m_handle, SW_HIDE );
	UpdateWindow( m_handle );
#elif defined(X_WIN_PRESENT)
	if (bVisible)
		XMapWindow(m_dipslay, m_window);
	else
		XUnmapWindow(m_dipslay, m_window);
	XFlush(m_display);
#endif
}

//---------------------------------------------------------------------------
bool CInternalWindow::Destroy()
{
#ifdef _WIN32
	DestroyWindow(m_handle);
	m_handle = 0;
	return true;
#elif defined(X_WIN_PRESENT)
	XDestroyWindow(m_display, m_window);
	XCloseDisplay(m_display);
#endif
	m_bCreated = false;
}

//---------------------------------------------------------------------------
bool CInternalWindow::BeginSplash()
{
#ifdef _WIN32
	// load bitmap
	HANDLE hbmpSplash = LoadImage(GetModuleHandle( NULL ), _W("splash.bmp"), IMAGE_BITMAP, 0,0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE); 
	if (!hbmpSplash) return false;

	// Register splash class
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = GetModuleHandle( NULL );
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = _W("P3DSplash");
	RegisterClass(&wc);

	// Create splash window and layered content widget
	m_hsplash = CreateWindow(_W("P3DSplash"), NULL, WS_POPUP,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	HWND m_hlayered = CreateWindowEx(WS_EX_LAYERED, _W("P3DSplash"), NULL, WS_POPUP | WS_VISIBLE,
		0, 0, 0, 0, m_hsplash, NULL, wc.hInstance, NULL);

	// get the size of the bitmap
	BITMAP bm;
	GetObjectW(hbmpSplash, sizeof(bm), &bm);
	SIZE sizeSplash = { bm.bmWidth, bm.bmHeight };


	// get the primary monitor's info
	POINT ptZero = { 0 };
	HMONITOR hmonPrimary = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO monitorinfo = { 0 };
	monitorinfo.cbSize = sizeof(monitorinfo);
	GetMonitorInfo(hmonPrimary, &monitorinfo);

	// center the splash screen in the middle of the primary work area
	const RECT & rcWork = monitorinfo.rcWork;

	POINT ptOrigin;
	ptOrigin.x = rcWork.left + (rcWork.right - rcWork.left - sizeSplash.cx) / 2;
	ptOrigin.y = rcWork.top + (rcWork.bottom - rcWork.top - sizeSplash.cy) / 2;

	// create a memory DC holding the splash bitmap
	HDC hdcScreen = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmpOld = (HBITMAP) SelectObject(hdcMem, hbmpSplash);

	// use the source image's alpha channel for blending
	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	// paint the window (in the right location) with the alpha-blended bitmap
	UpdateLayeredWindow(m_hlayered, hdcScreen, &ptOrigin, &sizeSplash,
		hdcMem, &ptZero, RGB(0, 0, 0), &blend, ULW_ALPHA);

	// delete temporary objects
	SelectObject(hdcMem, hbmpOld);
	DeleteDC(hdcMem);
	ReleaseDC(NULL, hdcScreen);
#elif defined(X_WIN_PRESENT)
	//load the splash image using Imlib2
	Imlib_Image image = imlib_load_image("splash.bmp");
	if(!image) return false;
	imlib_context_set_image(image);

	//obtain root window (current screen) attributes
	int screen = DefaultScreen(m_display);
	XWindowAttributes rootWinAttribs;
	XGetWindowAttributes
#endif
return true;
}

//---------------------------------------------------------------------------
bool CInternalWindow::EndSplash()
{
#ifdef _WIN32
	DestroyWindow(m_hsplash);
#elif defined(X_WIN_PRESENT)
	XDestroyWindow(m_display, m_SplashWindow);
#endif
return true;
}

//---------------------------------------------------------------------------
void CInternalWindow::Resize()
{

}

//---------------------------------------------------------------------------
void CInternalWindow::Update()
{
#ifdef _WIN32
	MSG msg;
	while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
#endif
}


}; // ns