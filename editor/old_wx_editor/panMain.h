#pragma once

#include "gui.h"

#include "panRender.h"

namespace P3D
{

	class panMain : public wxPanel 
	{
	private:

	protected:
		wxPanel* panRenderPlace;
		wxStaticLine* m_staticline6;
		wxPanel* panStatusbar;
		wxStaticText* txtStatus11;
		wxStaticLine* m_staticline41;
		wxGauge* gProgress1;
		wxStaticText* txtStatus21;
		wxStaticLine* m_staticline51;
		wxStaticText* m_staticText301;
		wxTextCtrl* txtX;
		wxStaticText* m_staticText311;
		wxTextCtrl* txtY;
		wxStaticText* m_staticText321;
		wxTextCtrl* txtZ;
		wxBoxSizer* szMain;

		// Virtual event handlers, overide them in your derived class
		virtual void OnXYZEnter( wxCommandEvent& event ){ event.Skip(); }


	public:
		panMain( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
		~panMain();
		void Make4ViewportLayout();
		void Make1ViewportLayout(eVT viewportType);
		void ActivateViewport(eVT viewportType);

		// viewports
		panRender *GetLeftTop()const{return m_pVLT;};
		panRender *GetLeftBottom()const{return m_pVLB;};
		panRender *GetRightTop()const{return m_pVRT;};
		panRender *GetRightBottom()const{return m_pVRB;};
	private:
		//viewports
		panRender *m_pVLT;
		panRender *m_pVLB;
		panRender *m_pVRT;
		panRender *m_pVRB;
	};

}
