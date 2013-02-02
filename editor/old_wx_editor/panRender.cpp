/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#include "panRender.h"
#include "settings.h"
#include "editor.h"

namespace P3D
{

	//----------------------------------
	panRender::panRender( wxWindow* parent,  eVT vtype )
	:
	panRenderGui( parent )
	{
		m_outev=NULL;

		m_fOrthoZoom = 100;
		m_vPos.x = m_vPos.y = m_vPos.z = 0.0f;

		mvtype = vtype;
		notRealtimeCnt=0;

		switch(mvtype)
		{
		case LEFT_TOP: 
			miViewType = Settings::GetInt(_W("LeftTopViewType"), 1);
			miRenderType = Settings::GetInt(_W("LeftTopRenderType"), 0);
			break;
		case LEFT_BOTTOM:  
			miViewType = Settings::GetInt(_W("LeftBottomViewType"), 0);
			miRenderType = Settings::GetInt(_W("LeftBottomRenderType"), 0);
			break;
		case RIGHT_TOP: 
			miViewType = Settings::GetInt(_W("RightTopViewType"), 2);
			miRenderType = Settings::GetInt(_W("RightTopRenderType"), 0);
			break;
		default:
			miViewType = Settings::GetInt(_W("RightBottomViewType"), 3);
			miRenderType = Settings::GetInt(_W("RightBottomRenderType"), 2);
			break;
		}

		switch (miViewType)
		{
		case 0: CheckToolbar(wxID_RIGHT, true); break;
		case 1: CheckToolbar(wxID_TOP, true); break;
		case 2: CheckToolbar(wxID_FRONT, true); break;
		case 3: CheckToolbar(wxID_PERSPECT, true); break;
		}
		switch (miRenderType)
		{
		case 0: CheckToolbar(wxID_WIRE, true); break;
		case 1: CheckToolbar(wxID_TEXT, true); break;
		case 2: CheckToolbar(wxID_LIT, true); break;
		}
		if (mbRealtime) CheckToolbar(wxID_REALTIME, true);

		SetActive(false);

		m_mousePosPrev.x = m_mousePosPrev.y = 0;
	}

	//----------------------------------
	panRender::~panRender()
	{
		SAFE_DELETE_ARRAY(m_outev);
	}

	//----------------------------------
	void panRender::toolMaximizeClicked( wxCommandEvent& event )
	{
		if (event.IsChecked())
			CEditor::MainWindow()->MainPanel()->Make1ViewportLayout(mvtype);
		else
			CEditor::MainWindow()->MainPanel()->Make4ViewportLayout();
	}

	//----------------------------------
	void panRender::toolRightClicked( wxCommandEvent& event )
	{
		miViewType = 0;
	}

	//----------------------------------
	void panRender::toolTopClicked( wxCommandEvent& event )
	{
		miViewType = 1;
	}

	//----------------------------------
	void panRender::toolFrontClicked( wxCommandEvent& event )
	{
		miViewType = 2;
	}

	//----------------------------------
	void panRender::toolPerspectClicked( wxCommandEvent& event )
	{
		miViewType = 3;
	}

	//----------------------------------
	void panRender::toolWireClicked( wxCommandEvent& event )
	{
		miRenderType = 0;
	}

	//----------------------------------
	void panRender::toolTextClicked( wxCommandEvent& event )
	{
		miRenderType = 1;
	}

	//----------------------------------
	void panRender::toolLitClicked( wxCommandEvent& event )
	{
		miRenderType = 2;
	}

	//----------------------------------
	void panRender::toolRealtimeClicked( wxCommandEvent& event )
	{
	/*	mbRealtime = event.IsChecked();
		if (mbRealtime) 
			b_nRealtimeWindows++;
		else
			b_nRealtimeWindows--;*/
	}

	//----------------------------------
	void panRender::toolRenderClicked( wxCommandEvent& event )
	{
		// TODO: Implement toolRenderClicked
	}

	//----------------------------------
	void panRender::OnIdle( wxIdleEvent& event )
	{ 
		if (!CEditor::Engine()) {event.Skip(); return;}

		CEditor::Engine()->BeginScene((IWindow*)this);
		CEditor::Engine()->EndScene();

		event.RequestMore(true);
		//this->UpdateWindowUI();
		//if (mbRealtime) event.RequestMore(true);*/
	}

	//----------------------------------
	void panRender::OnMouse( wxMouseEvent& event )
	{
		// down, up and motion
		if (event.Moving() || event.Dragging())
		{
			sWEvent ev;
			ev.type = WE_MOUSE_MOTION;
			ev.x = event.m_x;
			ev.y = event.m_y;
			ev.xrel = event.m_x - m_mousePosPrev.x;
			ev.xrel = event.m_y - m_mousePosPrev.y;
			m_events.push_back(ev);
			//CON(MSG_INFO, "moving");
			m_mousePosPrev.x = ev.x;
			m_mousePosPrev.y = ev.y;
		}
		// button handling
		if (event.IsButton())
		{
			sWEvent ev;

			if (event.GetButton()==wxMOUSE_BTN_LEFT)
				ev.button = MOUSE_LEFT;
			else if (event.GetButton()==wxMOUSE_BTN_MIDDLE)
				ev.button = MOUSE_MIDDLE;
			else
				ev.button = MOUSE_RIGHT;

			if (event.ButtonDown())
				ev.type = WE_MOUSE_DOWN;
			else
				ev.type = WE_MOUSE_UP;

			m_events.push_back(ev);
			//CON(MSG_INFO, "x %d", ev.type);
		}

		// if clicked make active
		if (event.ButtonDown())
			CEditor::MainWindow()->MainPanel()->ActivateViewport(mvtype);
	}

	//----------------------------------
	void panRender::CheckToolbar(int id, bool checked)
	{
		m_toolBar2->ToggleTool(id, checked);
	}

	//----------------------------------
	void panRender::Destroying()
	{
		switch(mvtype)
		{
		case LEFT_TOP:
			Settings::Set(_W("LeftTopViewType"), miViewType);
			Settings::Set(_W("LeftTopRenderType"), miRenderType);
			break;
		case LEFT_BOTTOM:
			Settings::Set(_W("LeftBottomViewType"), miViewType);
			Settings::Set(_W("LeftBottomRenderType"), miRenderType);
			break;
		case RIGHT_TOP:
			Settings::Set(_W("RightTopViewType"), miViewType);
			Settings::Set(_W("RightTopRenderType"), miRenderType);
			break;
		default:
			Settings::Set(_W("RightBottomViewType"), miViewType);
			Settings::Set(_W("RightBottomRenderType"), miRenderType);
			break;
		}
	}

	//----------------------------------
	sWEvent* panRender::GetEvents(OUT unsigned int &numEvents)
	{
		SAFE_DELETE_ARRAY(m_outev);

		numEvents = m_events.size();
		if (!numEvents) return NULL;

		m_outev = new sWEvent[numEvents];
		for (unsigned int i=0; i<numEvents;i++)
		{
			m_outev[i] = m_events[i];
		}

		m_events.clear();
		return m_outev;
	};

	//------------------------------------
	void panRender::SetSize(Vec2i wsize, bool fullscreen/* =false */)
	{
		if (fullscreen)
		{
			CON(MSG_ERR, _W("Attempt to go fullscreen from editor viewport!"));
			return;
		}
		CON(MSG_ERR, _W("Force resize on editor viewport window not yet implemented!"));
	}

	//------------------------------------
	void panRender::SetMenuMode(bool bMenu)
	{
	}
}