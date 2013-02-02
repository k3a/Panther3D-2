/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#include "editor.h"
#include "settings.h"

#include "CGUI.h" // test only

namespace P3D
{
	wxFrame* fr;

	//test
	TestGUI::TestGUI()
	{
		fr=NULL;
		if (wxXmlResourceEx::Get()->Load(wxT("../data/editor/EdGUI_Sample.xrc"))) //../data/editor/PEditor.xrc
		{
			fr = wxXmlResourceEx::Get()->LoadFrame(CEditor::MainWindow(), wxT("SampleFrame"), this);
			if (fr) fr->Show();
		}

		//		if(fr) delete fr;
	}
	TestGUI::~TestGUI()
	{
		//if (fr) fr->Destroy();
	}
	void TestGUI::evButton(eGEvent ev, unsigned int id, IGButton* obj, int value1, int value2)
	{
		CON(MSG_INFO, _W("EdGUI: Button with id %d clicked!"), id);

		// change caption of this button to it's #ID
		wchar test[64];
		wsprintf(test, _W("#%d"), id);
		obj->SetCaption(test);
		
		// find textbox by ID and change it's text
		IGTextBox *tb = this->GetTextBoxById(128);
		wsprintf(test, _W("Button with id %d changed this text."), id);
		if (tb) tb->SetText(test);

		// find gauge by id and set value
		IGGauge* ga = this->GetGaugeById(140);
		if (ga) ga->SetValue(70);

		// find button by name
		IGButton* toolButton = GetButtonByName(_W("m_button2"));
		if (toolButton) toolButton->SetCaption(_W("Found by name"));
	}
	void TestGUI::evTextBox(eGEvent ev, unsigned int id, IGTextBox* obj, int value1, int value2)
	{
		switch (ev)
		{ 
		case GE_TEXT_CHANGE:
			CON(MSG_INFO, _W("EdGUI: TextBox with id %d changed! Text value: %s"), id, obj->GetText());
			break;
		case GE_TEXT_ENTER: // only for textboxes with enter allowed
			obj->SetText(_W("Enter pressed"));
			break;
		case GE_ENTER:
			obj->SetText(_W("Mouse entered"));
			break;
		case GE_LEAVE:
			obj->SetText(_W("Mouse leaved"));
			break;
		case GE_GOT_FOCUS:
			obj->SetText(_W("Got focus"));
			break;
		case GE_LOST_FOCUS:
			obj->SetText(_W("Lost focus"));
			break;
		}
	}

	void TestGUI::evSlider(eGEvent ev, unsigned int id, IGSlider* obj, int value1, int value2)
	{
		wchar test[64];
		IGTextBox *tb = this->GetTextBoxById(128);
		wsprintf(test, _W("Slider: %d%%"), obj->GetValue() );
		if (tb) tb->SetText(test);

		if (IGGauge* g = GetGaugeByName(_W("m_gauge1"))) g->SetValue(obj->GetValue());
	}

TestGUI *gwin;

	IMPLEMENT_APP(CEditor)

	frmMain *CEditor::s_pfrmMain=NULL;

	IEngine *CEditor::s_pEngine=NULL;

	static wchar szExePath[MAX_PATH];

	CEditor::CEditor()
	{
	}

	CEditor::~CEditor()
	{ 

	}

	bool CEditor::OnInit()
	{
		//_CrtSetBreakAlloc(3732);
		SetVendorName(wxString("Reversity Studios"));
		SetClassName(wxString("P3DCrane2"));
		SetAppName(wxString("P3DCrane2"));

		// ---------------------------------
		// LOAD DLL MODULES FROM EXE PATH
		wstrcpy(szExePath, wxTheApp->argv[0]);
		for(int i=(int)wstrlen(szExePath); i > 0; i--)
		{
			if (szExePath[i]=='/' || szExePath[i]=='\\')
			{
				szExePath[i]=0;
				break;
			}
		}

		// ---------------------------------
		I_Initialize(szExePath); // initialize dll module system
		// ---------------------------------

		// SHOW SPLASH
		p_dlgSplash = new dlgSplashGui( (wxWindow*)NULL );
		p_dlgSplash->txtInfo->SetWindowStyle(p_dlgSplash->txtInfo->GetWindowStyle()|wxST_NO_AUTORESIZE);
		p_dlgSplash->Show();
		p_dlgSplash->txtInfo->SetLabel(_W(" Loading settings and initializing..."));
		p_dlgSplash->Update();

		// init all image handlers for wx (ico, bmp, ...)
		wxInitAllImageHandlers();

		// init all gui handlers
		EdGUI_RegisterHandlers();

		// redirect wx log to console
		EdGUI_EnableLogging();

		// load settings
		Settings::Load(_W("editor.ini"));

		p_dlgSplash->txtInfo->SetLabel(_W(" Initializing engine..."));
		p_dlgSplash->Update();

		// create main window
		s_pfrmMain = new frmMain( (wxWindow*)NULL );

		// Get engine
		/*s_pEngine = (IEngine *)I_GetModule("engine", NULL);
		assert(s_pEngine!=0);

		s_pEngine->LoadModules("dx9", NULL, NULL, NULL, NULL);
		s_pEngine->mRenderer()->AddEventListener(this);

		// initialize engine
		sDisplayProperties dp;
		dp.Fullscreen = false;
		wxDisplaySize((int *)&dp.HorRes, (int *)&dp.VertRes);
		s_pEngine->Initialize("editor.ini", true, &dp);*/

		// HIDE SPLASH
		delete p_dlgSplash;
		p_dlgSplash = NULL;

		Sleep(100);
		s_pfrmMain->Show();
		s_pfrmMain->SetFocus();

		// TEST XRC
		gwin = new TestGUI();

		return true;
	}

	int CEditor::OnExit()
	{
		delete gwin;
		EdGUI_UnregisterHandlers();

		Settings::Save(_W("editor.ini"));

		I_Shutdown();

		return 0;
	}

}