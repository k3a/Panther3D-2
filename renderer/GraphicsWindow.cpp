/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "GraphicsWindow.h"
#include "Renderer.h"


#include <SDL/SDL_syswm.h>
#include <vector>

namespace P3D
{

//------------------------------------------
CGraphicsWindow::CGraphicsWindow()
{
	m_bCreated = false;
	m_bMenuMode = false;
	m_outev = NULL;
	m_x11win = 0;
	m_screen = NULL;
}

//------------------------------------------
CGraphicsWindow::~CGraphicsWindow()
{
	SAFE_DELETE_ARRAY(m_outev);
}

//------------------------------------------
bool CGraphicsWindow::Create(const wchar* caption, Vec2i size, bool fullscreen)
{
	if (m_bCreated)
	{
		CON(MSG_ERR, _W("Second call of CGraphicsWindow::CreateWindow(). Window already created!"));
		return false;
	}

	m_size = size;

	CON(MSG_INFO, _W(" ...creating SDL window of size %d x %d"), m_size.x, m_size.y);

	// init sdl
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		CON(MSG_ERR, _W("Unable to initialize SDL video: %s!"), SDL_GetError());
		return false;
	}

	SDL_EnableUNICODE(true);

	if (caption && caption[0]!=0)
	{
		SDL_WM_SetCaption(_W2A(caption), NULL);
	}
	else
		SDL_WM_SetCaption(P3DNAME, NULL);

#ifdef _OGL
	/*
	const SDL_VideoInfo *pSDLVideoInfo = SDL_GetVideoInfo();

	if( !pSDLVideoInfo )
	{
		CON(MSG_ERR, _W("SDL_GetVideoInfo() failed. SDL Error: %s\n"), SDL_GetError());
		SDL_Quit();
		return 1;
	}

	int nFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE;

	if( pSDLVideoInfo->hw_available ) // Hardware surfaces enabled?
		nFlags |= SDL_HWSURFACE;
	else
		nFlags |= SDL_SWSURFACE;

	if( pSDLVideoInfo->blit_hw ) // Hardware supported blitting?
		nFlags |= SDL_HWACCEL;

	if (fullscreen)
		nFlags |= SDL_FULLSCREEN;
	else
		nFlags |= SDL_RESIZABLE;

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // Enable OpenGL Doublebuffering

	// create new window
	CRenderer::instance()->EV_Loading(_W("Creating SDL Window..."));
	m_screen = SDL_SetVideoMode(m_size.x, m_size.y, SDL_GetVideoInfo()->vfmt->BitsPerPixel, nFlags);
	CRenderer::instance()->EV_Loading(_W("Creating SDL Window..."));
	if (m_screen == NULL) {
		CON(MSG_ERR, _W("Unable to set OGL video mode %d x %d x %d for SDL: %s!"),
			m_size.x, m_size.y, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_GetError());
		return false;
	}
	*/

	int nFlags=0;
	if (fullscreen)
		nFlags = SDL_FULLSCREEN;
	else
		nFlags = SDL_RESIZABLE;

	// create new window
	CRenderer::instance()->EV_Loading(_W("Creating SDL Window..."));
	m_screen = SDL_SetVideoMode(m_size.x, m_size.y, SDL_GetVideoInfo()->vfmt->BitsPerPixel, nFlags);
	CRenderer::instance()->EV_Loading(_W("Creating SDL Window..."));
	if (m_screen == NULL) {
		CON(MSG_ERR, _W("Unable to set DX video mode %d x %d x %d for SDL: %s!"),
			m_size.x, m_size.y, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_GetError());
		return false;
	}

#endif

#ifdef _DX

	int nFlags=0;
	if (fullscreen)
		nFlags = SDL_FULLSCREEN;
	else
		nFlags = SDL_RESIZABLE;

	// create new window
	CRenderer::instance()->EV_Loading(_W("Creating SDL Window..."));
	m_screen = SDL_SetVideoMode(m_size.x, m_size.y, SDL_GetVideoInfo()->vfmt->BitsPerPixel, nFlags);
	CRenderer::instance()->EV_Loading(_W("Creating SDL Window..."));
	if (m_screen == NULL) {
		CON(MSG_ERR, _W("Unable to set DX video mode %d x %d x %d for SDL: %s!"),
			m_size.x, m_size.y, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_GetError());
		return false;
	}
#endif

	SetMenuMode(m_bMenuMode); // hack to initialize mouse pos and visibility

    SDL_SysWMinfo   info;
    m_hwnd = NULL;
    SDL_VERSION(&info.version);
    #if defined(WIN32)
    if (SDL_GetWMInfo(&info) != -1) m_hwnd = (void*)info.window;
    #elif defined(LINUX)
    if (SDL_GetWMInfo(&info) != -1) m_x11win = info.info.x11.window;
    m_hwnd = &m_x11win;
    #endif

	// center on screen
	SetWindowPos((HWND)m_hwnd, HWND_TOP, (GetSystemMetrics(SM_CXSCREEN)-m_size.x)/2-4, (GetSystemMetrics(SM_CYSCREEN)-m_size.y)/2-15, 0, 0, SWP_NOSIZE);

	if (!m_hwnd)
	{
		CON(MSG_ERR, _W("Can't get window handle from SDL!"));
		return false;
	}

	m_bCreated = true;
	return true;
}

//------------------------------------------
void CGraphicsWindow::SetSize(Vec2i wsize, bool fullscreen)
{
	m_size = wsize;

	int nFlags = 0;

	#if defined(_OGL)
    nFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE;

    const SDL_VideoInfo *pSDLVideoInfo = SDL_GetVideoInfo();

	if( pSDLVideoInfo->hw_available ) // Hardware surfaces enabled?
		nFlags |= SDL_HWSURFACE;
	else
		nFlags |= SDL_SWSURFACE;

	if( pSDLVideoInfo->blit_hw ) // Hardware supported blitting?
		nFlags |= SDL_HWACCEL;
    #endif

	if (fullscreen)
		nFlags |= SDL_FULLSCREEN;
	else
		nFlags |= SDL_RESIZABLE;



	#if defined (WIN32)
	CRenderer::OnLostDevice();
	#endif
	
	m_screen = SDL_SetVideoMode(m_size.x, m_size.y, SDL_GetVideoInfo()->vfmt->BitsPerPixel, nFlags);
	if (m_screen == NULL) {
		CON(MSG_ERR, _W("Unable to set new video mode %d x %d x %d for SDL: %s!"),
			m_size.x, m_size.y, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_GetError());
		return;
	}

	SetMenuMode(m_bMenuMode); // hack to initialize mouse pos and visibility

	#if defined (WIN32)
	//CRenderer::OnLostDevice();
	CRenderer::cGraphicsDevice()->Restart();
	CRenderer::OnResetDevice();
	//CON(MSG_INFO, "win");
	#endif

	#if defined (LINUX)
	Rect vp;
	vp.x1 = 0;
	vp.y1 = 0;
	vp.x2 = m_size.x;
	vp.y2 = m_size.y;
	CRenderer::cGraphicsDevice()->SetViewport(vp);
	//CON(MSG_INFO, "linux");
	#endif

	CON(MSG_INFO, _W("Renderer window size changed to %d x %d"), wsize.x, wsize.y );
}

void* CGraphicsWindow::GetHandle()const
{
	return m_hwnd;
}

//------------------------------------------
// convert Unicode to Central Europian Chars
wchar ToCE(Uint16 u)
{
	switch(u)
	{
		// main czech
	case 0x011A: return 'Ã';
	case 0x011B: return 'Ï';
	case 0x0160: return 'ä';
	case 0x0161: return 'ö';
	case 0x010C: return '»';
	case 0x010D: return 'Ë';
	case 0x0158: return 'ÿ';
	case 0x0159: return '¯';
	case 0x017D: return 'é';
	case 0x017E: return 'û';
	case 0x00DD: return '›';
	case 0x00FD: return '˝';
	case 0x00C1: return '¡';
	case 0x00E1: return '·';
	case 0x00CD: return 'Õ';
	case 0x00ED: return 'Ì';
	case 0x00C9: return '…';
	case 0x00E9: return 'È';
	case 0x00DA: return '⁄';
	case 0x00FA: return '˙';
	case 0x010E: return 'œ';
	case 0x010F: return 'Ô';
	case 0x0164: return 'ç';
	case 0x0165: return 'ù';
	case 0x0147: return '“';
	case 0x0148: return 'Ú';
	case 0x00D3: return '”';
	case 0x00F3: return 'Û';
	case 0x016E: return 'Ÿ';
	case 0x016F: return '˘';
		// other CE
	case 0x00E4: return '‰';
	case 0x00EB: return 'Î';
	case 0x00EF: return 'i';
	case 0x00F6: return 'ˆ';
	case 0x00FC: return '¸';
	case 0x00FF: return 'y';
	}
	return 0;
}

//------------------------------------------
sWEvent* CGraphicsWindow::GetEvents(OUT unsigned int &numEvents)
{
	if (!m_bCreated) { numEvents=0; return NULL; }

	SAFE_DELETE_ARRAY(m_outev);

	SDL_Event event;
	std::vector<sWEvent> events;

	sWEvent ev;
	wchar ch=0;

	bool isTopWindow=true;
	// detect if this window is top window - has focus
	Uint8 appState = SDL_GetAppState();
	if ( !(appState & SDL_APPINPUTFOCUS) )
		isTopWindow = false;

	// slow down framerate if no top window
	if (!isTopWindow) Sleep(100);

	// compute relative mouse pos
	int mrelx=0,mrely=0;
	SDL_GetMouseState(&mrelx, &mrely);
	mrelx-=m_size.x/2; mrely-=m_size.y/2; 

	while( SDL_PollEvent( &event ) )
		switch( event.type )
	{
		case SDL_QUIT:
			ev.type = WE_QUIT;
			events.push_back(ev);
			break;

		case SDL_VIDEORESIZE:
			m_size.x = event.resize.w;
			m_size.y = event.resize.h;

			SetSize(m_size);

			ev.type = WE_RESIZE;
			ev.x = (float)m_size.x; ev.y = (float)m_size.y;
			events.push_back(ev);
			break;

		case SDL_KEYDOWN:

			ev.type = WE_KEY_DOWN;
			ev.button = event.key.keysym.sym;

			if ( (event.key.keysym.unicode & 0xFF80) == 0 ) {
				ev.character = event.key.keysym.unicode & 0x7F;
			}
			else
			{
				// convert unicode chars :-P
				ev.character = ToCE(event.key.keysym.unicode);
				if(ev.character == 0)
					CON(MSG_INFO, _W("Unknown keyboard char 0x%04X"), event.key.keysym.unicode);
			}

			events.push_back(ev);
			break;

		case SDL_KEYUP:

			ev.type = WE_KEY_UP;
			ev.button = event.key.keysym.sym;

			if ( (event.key.keysym.unicode & 0xFF80) == 0 ) {
				ev.character = event.key.keysym.unicode & 0x7F;
			}
			else
			{
				// convert unicode chars :-P
				ev.character = ToCE(event.key.keysym.unicode);
				if(ev.character == 0)
					CON(MSG_INFO, _W("Unknown keyboard char 0x%04X"), event.key.keysym.unicode);
			}

			events.push_back(ev);
			break;

		case SDL_MOUSEBUTTONDOWN:
			ev.type = WE_MOUSE_DOWN;
			ev.button = event.button.button;
			events.push_back(ev);
			break;

		case SDL_MOUSEBUTTONUP:
			ev.type = WE_MOUSE_UP;
			ev.button = event.button.button;
			events.push_back(ev);
			break;

		case SDL_MOUSEMOTION:
			mrelx = ((int)event.motion.x)-m_size.x/2; 
			mrely = ((int)event.motion.y)-m_size.y/2; 
			m_mousePos.x = event.motion.x; 
			m_mousePos.y = event.motion.y;
		/*	ev.type = WE_MOUSE_MOTION;
			ev.x = (float)event.motion.x;
			ev.y = (float)event.motion.y;
			m_mousePos.x = (int)ev.x; m_mousePos.y = (int)ev.y;
			ev.xrel = mrelx;
			ev.yrel = mrely;
			events.push_back(ev);
			break;*/
	}

	if (mrelx!=0 || mrely!=0)
	{
		ev.type = WE_MOUSE_MOTION;
		ev.x = (float)m_mousePos.x;
		ev.y = (float)m_mousePos.y;
		if (m_bMenuMode || !isTopWindow)
		{
			ev.xrel = ev.yrel = 0.0f;
		}
		else
		{
			ev.xrel = (float)mrelx;
			ev.yrel = (float)mrely;
			SDL_WarpMouse(m_size.x/2, m_size.y/2); // keep mouse in the center of window
		}
		events.push_back(ev);
	}

	// process events

	numEvents = events.size();
	if (!numEvents) return NULL;

	m_outev = new sWEvent[numEvents];
	for (unsigned int i=0; i<numEvents;i++)
	{
		m_outev[i] = events[i];
	}

	return m_outev;
}

void CGraphicsWindow::HideWindow(bool hide/* =true */)
{
#ifdef WIN32
	ShowWindow((HWND)m_hwnd, !hide);
#endif
}

void CGraphicsWindow::SetMenuMode(bool bMenu)
{
	m_bMenuMode=bMenu;
	if (bMenu)
		SDL_ShowCursor(1);
	else
	{
		SDL_ShowCursor(0);
		SDL_WarpMouse(m_size.x/2, m_size.y/2); // set mouse to center of window
	}
}

}; // namespace
