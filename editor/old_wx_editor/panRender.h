/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/

#ifndef __panRender__
#define __panRender__

#include "Vector.h"

#include "gui.h"
#include "IWindow.h"
#include <vector>

namespace P3D
{


	enum eVT
	{
		LEFT_TOP,
		LEFT_BOTTOM,
		RIGHT_TOP,
		RIGHT_BOTTOM
	};

	/** Implementing panRenderGui */
	class panRender : public panRenderGui, public IWindow
	{
	protected:
		// Handlers for panRenderGui events.
		void toolMaximizeClicked( wxCommandEvent& event );
		void toolRightClicked( wxCommandEvent& event );
		void toolTopClicked( wxCommandEvent& event );
		void toolFrontClicked( wxCommandEvent& event );
		void toolPerspectClicked( wxCommandEvent& event );
		void toolWireClicked( wxCommandEvent& event );
		void toolTextClicked( wxCommandEvent& event );
		void toolLitClicked( wxCommandEvent& event );
		void toolRealtimeClicked( wxCommandEvent& event );
		void toolRenderClicked( wxCommandEvent& event );
		void OnMouse( wxMouseEvent& event );
		
	public:
		/** Constructor */
		panRender( wxWindow* parent, eVT vtype );
		~panRender();
		void OnIdle( wxIdleEvent& event );
		void CheckToolbar(int id, bool checked);
		void Destroying();
		HWND GetViewportHWND(){return (HWND)this->panViewport->GetHWND();};
		void SetActive(bool bActive=true){ 	
			m_bActive = bActive;
			if(bActive)
				this->SetBackgroundColour(wxColour(255, 0, 0));
			else
				this->SetBackgroundColour(wxColour(0, 0, 0));
			this->ClearBackground(); 
		};
		// ----- IWindow -----
		void* GetHandle()const{ return this->panViewport->GetHWND(); };
		bool IsNativeWindow()const{ return true; };
		bool CanGoFullscreen()const{ return false; };
		unsigned int GetBPP()const{return 32;};
		sWEvent* GetEvents(OUT unsigned int &numEvents);
		Vec2i GetMousePos()const{ return m_mousePosPrev; };
		Vec2i GetSize()const
		{ 
			wxSize sz = this->panViewport->GetSize();
			return Vec2i( sz.x, sz.y );
		};
		void SetSize(Vec2i wsize, bool fullscreen=false);
		void SetMenuMode(bool bMenu);
	private:
		int miViewType;
		int miRenderType;
		bool mbRealtime;
		eVT mvtype;
		DWORD notRealtimeCnt; // number of non-realtime ticks
		bool m_bActive;
		//
		float m_fOrthoZoom;
		Vec3 m_vPos;
		Vec3 m_vView;
		// ----- IWindow -----
		Vec2i m_mousePosPrev;
		std::vector<sWEvent> m_events;
		sWEvent* m_outev;
	};

}

#endif // __panRender__
