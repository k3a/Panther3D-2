#pragma once

#include <wx/wx.h>
#include "frmMain.h"

// engine
#include "IEngine.h"
#include "IConsole.h"

// EdGUI test
#include "EdGUI.h"

namespace P3D
{

	class CEditor : public wxApp, public IRendererEventListener
	{
	public:
		CEditor();
		virtual ~CEditor();
		virtual bool OnInit();
		virtual int OnExit();

		// -- WINDOWS AND EDITOR SINGLETONS
		static inline frmMain *MainWindow(){ return s_pfrmMain; };

		// -- ENGINE AND OTHER P3D SINGLETONS
		static inline IEngine *Engine(){ return s_pEngine; };

		// -- LISTENERS
		void RendererLoading(const wchar* desc){
			if (!p_dlgSplash) return;
			p_dlgSplash->txtInfo->SetLabel(wxString(" ")+wxString(desc));
			p_dlgSplash->Update();
		};

	private:
		dlgSplashGui *p_dlgSplash;
		static frmMain *s_pfrmMain;
		// --
		static IEngine *s_pEngine;
	};

	DECLARE_APP(CEditor)

	//test
	class TestGUI : public IGWindow
	{
	public:
		TestGUI();
		~TestGUI();
		void evButton(eGEvent ev, unsigned int id, IGButton* obj, int value1, int value2);
		void evTextBox(eGEvent ev, unsigned int id, IGTextBox* obj, int value1, int value2);
		void evSlider(eGEvent ev, unsigned int id, IGSlider* obj, int value1, int value2);
	};

} // ns

// additional if(CEditor::Engine()) is not necessary, but useful in case we need editor GUI to load fast (without engine)
#define CON(type, text, ...) if(CEditor::Engine()) CEditor::Engine()->iConsole()->AddMsgEx(type, _W(__FUNCTION__), text, ##__VA_ARGS__)