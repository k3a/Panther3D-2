/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#include "frmMain.h"

#include "dlgAbout.h"
#include "settings.h"

namespace P3D
{
	panMain* frmMain::s_main_panel=NULL;
	panDataTool *frmMain::s_data_tool_panel=NULL;
	panRollup *frmMain::s_rollup_panel=NULL;

	frmMain::frmMain( wxWindow* parent )
	:
	frmMainGui( parent )
	{
		Hide(); // hide first while loading

		SetIcon(wxIcon("../data/editor/app.ico", wxBITMAP_TYPE_ICO));

		s_main_panel = new panMain(this);
		m_mgr.AddPane(s_main_panel, wxAuiPaneInfo().
			Name(wxT("main_window")).Caption(wxT("Main Window")).CloseButton(false)
			.Center());

		wxTextCtrl *tx = new wxTextCtrl(this, wxID_ANY);
		m_mgr.AddPane(tx, wxAuiPaneInfo().
			Name(wxT("console")).Caption(wxT("Console"))
			.Bottom());

		s_data_tool_panel = new panDataTool(this);
		m_mgr.AddPane(s_data_tool_panel, wxAuiPaneInfo().
			Name(wxT("data_tool")).Caption(wxT("Asset Browser"))
			.Left());


		s_rollup_panel = new panRollup(this);
		m_mgr.AddPane(s_rollup_panel, wxAuiPaneInfo().
			Name(wxT("rollup")).Caption(wxT("Rollup bar"))
			.Right());

		m_mgr.Update();


		// SETTINGS
		SetPosition( wxPoint(Settings::GetInt(_W("WinX"), 10), Settings::GetInt(_W("WinY"), 10)) );
		SetSize( wxSize(Settings::GetInt(_W("WinSizeX"), 800), Settings::GetInt(_W("WinSizeY"), 600)) );
		if (Settings::GetBool(_W("WinMaximized"))) { Maximize(); Refresh(); }
		// menu
		mbDontSwitch = Settings::GetBool(_W("DontSwitchMenu"));
		menu->Check(wxID_DONT_SWITCH, mbDontSwitch);
		// toolbar
		miToolType = Settings::GetInt(_W("SelectedToolType"));
		switch(miToolType)
		{
		case 0: toolbarManipulators->ToggleTool(wxID_SELECT, true); break;
		case 1: toolbarManipulators->ToggleTool(wxID_MOVE, true); break;
		case 2: toolbarManipulators->ToggleTool(wxID_ROTATE, true); break;
		case 3: toolbarManipulators->ToggleTool(wxID_SCALE, true); break;
		case 4: toolbarManipulators->ToggleTool(wxID_SCALE_SIDES, true); break;
		}

		mbLocal = Settings::GetBool(_W("LocalSpaceTool"));
		mbPhysInEditor = Settings::GetBool(_W("PhysInEditor"), true);
		mbOrbit = Settings::GetBool(_W("OrbitView"));
		mbGrid = Settings::GetBool(_W("ShowGrid"));
		miGridSize = Settings::GetInt(_W("GridSize"), 8);
		miFarPlane = Settings::GetInt(_W("FarPlane"), 50);
		miCameraSpeed = Settings::GetInt(_W("CameraSpeed"), 50);
		// set gui
		toolbarManipulators->ToggleTool(wxID_LOCAL, mbLocal);
		toolbarAdditional->ToggleTool(wxID_PHYS_IN_EDITOR, mbPhysInEditor);
		toolbarAdditional->ToggleTool(wxID_ORBIT, mbOrbit);
		toolbarAdditional->ToggleTool(wxID_SHOW_GRID, mbGrid);
		toolFarPlane->SetValue(miFarPlane);
		toolCameraSpeed->SetValue(miCameraSpeed);

		// perspective
		wxString str = m_mgr.SavePerspective();
		//Settings::Set("WinLayout", str); // DEBUG: to reset arrangement settings
		m_mgr.LoadPerspective(Settings::GetString(_W("WinLayout"), str));
	}

	void RecurClose(wxWindow* bw, bool mainwin=true)
	{
		wxWindowList lst = bw->GetChildren();
		for (unsigned int i=0; i<lst.size(); i++)
			RecurClose(lst[i], false);

		if (!mainwin) bw->Close();
	}

	void frmMain::OnClose( wxCloseEvent& event )
	{
		Hide(); // hide window ;)

		//close all childs
		RecurClose(this);

		// save settings
		wxPoint pnt; wxSize sz;

		pnt = GetPosition(); Settings::Set(_W("WinX"), pnt.x); Settings::Set(_W("WinY"), pnt.y);
		if (!IsMaximized())
		{
			sz = GetSize(); Settings::Set(_W("WinSizeX"), sz.x); Settings::Set(_W("WinSizeY"), sz.y);
		}
		Settings::Set(_W("WinMaximized"), IsMaximized());
		// toolbars
		Settings::Set(_W("SelectedToolType"), miToolType);
		Settings::Set(_W("LocalSpaceTool"), mbLocal);
		Settings::Set(_W("PhysInEditor"), mbPhysInEditor);
		Settings::Set(_W("OrbitView"), mbOrbit);
		Settings::Set(_W("ShowGrid"), mbGrid);
		Settings::Set(_W("ShowGrid"), miGridSize);
		Settings::Set(_W("FarPlane"), miFarPlane);
		Settings::Set(_W("CameraSpeed"), miCameraSpeed);

		// perspective
		wxString str = m_mgr.SavePerspective();
		Settings::Set(_W("WinLayout"), str);

		// delete panels - not necessary coz delete this will do it
		//SAFE_DELETE(s_main_panel);
		//SAFE_DELETE(s_data_tool_panel);
		//SAFE_DELETE(s_rollup_panel);

		// exit app
		wxTheApp->Exit();

		delete this; // delete this main window and all associated widgets and panels 
	}

	void frmMain::OnUpdateUI( wxUpdateUIEvent& event )
	{
	}

	void frmMain::btnFindEntityPressed( wxCommandEvent& event )
	{
		// TODO: Implement btnFindEntityPressed
	}

	void frmMain::txtPositionEnterPressed( wxCommandEvent& event )
	{
		// TODO: Implement txtPositionEnterPressed
	}

	void frmMain::tabEventsPickToggled( wxCommandEvent& event )
	{
		// TODO: Implement tabEventsPickToggled
	}

	void frmMain::tabEventsParamLeaveFocus( wxFocusEvent& event )
	{
		// TODO: Implement tabEventsParamLeaveFocus
	}

	void frmMain::tabEventsParamEnterFocus( wxFocusEvent& event )
	{
		// TODO: Implement tabEventsParamEnterFocus
	}

	void frmMain::tabEventsParamOnEnter( wxCommandEvent& event )
	{
		// TODO: Implement tabEventsParamOnEnter
	}

	void frmMain::tabEventsDelayOnEnter( wxCommandEvent& event )
	{
		// TODO: Implement tabEventsDelayOnEnter
	}

	void frmMain::tabEventsAddPressed( wxCommandEvent& event )
	{
		// TODO: Implement tabEventsAddPressed
	}

	void frmMain::tabEventsCopyPressed( wxCommandEvent& event )
	{
		// TODO: Implement tabEventsCopyPressed
	}

	void frmMain::tabEventsPastePressed( wxCommandEvent& event )
	{
		// TODO: Implement tabEventsPastePressed
	}

	void frmMain::tabEventsDeletePressed( wxCommandEvent& event )
	{
		// TODO: Implement tabEventsDeletePressed
	}

	void frmMain::tabCreatePickPosPressed( wxCommandEvent& event )
	{
		// TODO: Implement tabCreatePickPosPressed
	}

	void frmMain::tabCreateEntitiesDblClick( wxListEvent& event )
	{
		// TODO: Implement tabCreateEntitiesDblClick
	}

	void frmMain::tabCreateFindOnTexChanged( wxCommandEvent& event )
	{
		// TODO: Implement tabCreateFindOnTexChanged
	}

	void frmMain::tabCreateFindEnterPressed( wxCommandEvent& event )
	{
		// TODO: Implement tabCreateFindEnterPressed
	}

	void frmMain::tabCreateCreateBtnPressed( wxCommandEvent& event )
	{
		// TODO: Implement tabCreateCreateBtnPressed
	}

	void frmMain::tabGroupsSelectedOnCombo( wxCommandEvent& event )
	{
		// TODO: Implement tabGroupsSelectedOnCombo
	}

	void frmMain::tabGroupsSelectedOnKeyUp( wxKeyEvent& event )
	{
		// TODO: Implement tabGroupsSelectedOnKeyUp
	}

	void frmMain::tabGroupsSelectedOnText( wxCommandEvent& event )
	{
		// TODO: Implement tabGroupsSelectedOnText
	}

	void frmMain::tabGroupsSelectedOnEnterCreate( wxCommandEvent& event )
	{
		// TODO: Implement tabGroupsSelectedOnEnterCreate
	}

	void frmMain::tabGroupsCreateNewPressed( wxCommandEvent& event )
	{
		// TODO: Implement tabGroupsCreateNewPressed
	}

	void frmMain::tabGroupsDeletePressed( wxCommandEvent& event )
	{
		// TODO: Implement tabGroupsDeletePressed
	}

	void frmMain::tabGroupsAddSelectionToGroupPressed( wxCommandEvent& event )
	{
		// TODO: Implement tabGroupsAddSelectionToGroupPressed
	}

	void frmMain::tabGroupsSelectAllPressed( wxCommandEvent& event )
	{
		// TODO: Implement tabGroupsSelectAllPressed
	}

	void frmMain::tabGroupsListDblClick( wxCommandEvent& event )
	{
		// TODO: Implement tabGroupsListDblClick
	}

	void frmMain::tabGroupsMakeSelectionFromHighlightedPressed( wxCommandEvent& event )
	{
		// TODO: Implement tabGroupsMakeSelectionFromHighlightedPressed
	}

	void frmMain::tabGroupsRemoveHighlightedPressed( wxCommandEvent& event )
	{
		// TODO: Implement tabGroupsRemoveHighlightedPressed
	}

	void frmMain::tabGroupsClearListPressed( wxCommandEvent& event )
	{
		// TODO: Implement tabGroupsClearListPressed
	}

	void frmMain::toolNewClicked( wxCommandEvent& event )
	{
		// TODO: Implement toolNewClicked
	}

	void frmMain::toolOpenClicked( wxCommandEvent& event )
	{
		// TODO: Implement toolOpenClicked
	}

	void frmMain::toolSaveClicked( wxCommandEvent& event )
	{
		// TODO: Implement toolSaveClicked
	}

	void frmMain::toolSelectClicked( wxCommandEvent& event )
	{
		miToolType=0;
	}

	void frmMain::toolMoveClicked( wxCommandEvent& event )
	{
		miToolType=1;
	}

	void frmMain::toolRotateClicked( wxCommandEvent& event )
	{
		miToolType=2;
	}

	void frmMain::toolScaleClicked( wxCommandEvent& event )
	{
		miToolType=3;
	}

	void frmMain::toolScaleSidesClicked( wxCommandEvent& event )
	{
		miToolType=4;
	}

	void frmMain::toolLocalClicked( wxCommandEvent& event )
	{
		mbLocal = event.IsChecked();
	}

	void frmMain::toolPhysInEditorClicked( wxCommandEvent& event )
	{
		mbPhysInEditor = event.IsChecked();
	}

	void frmMain::toolOrbitClicked( wxCommandEvent& event )
	{
		mbOrbit = event.IsChecked();
	}

	void frmMain::toolShowGridClicked( wxCommandEvent& event )
	{
		mbGrid = event.IsChecked();
	}

	void frmMain::toolGridMinusClicked( wxCommandEvent& event )
	{
		miGridSize /= 2;
		if (miGridSize<1) miGridSize=1;
	}

	void frmMain::toolGridPlusClicked( wxCommandEvent& event )
	{
		miGridSize *= 2;
		if (miGridSize>512) miGridSize=512;
	}


	void frmMain::toolFarPlaneThumbScrollReleased( wxScrollEvent& event )
	{
		miFarPlane = toolFarPlane->GetValue();
	}

	void frmMain::toolCameraSpeedScrollReleased( wxScrollEvent& event )
	{
		miCameraSpeed = toolCameraSpeed->GetValue();
	}

	void frmMain::toolPlayClicked( wxCommandEvent& event )
	{
		// TODO: Implement toolPlayClicked
	}

	void frmMain::toolAssetBrowserClicked( wxCommandEvent& event )
	{
		// TODO: Implement toolAssetBrowserClicked
	}

	void frmMain::toolMaterialEditorClicked( wxCommandEvent& event )
	{
		// TODO: Implement toolMaterialEditorClicked
	}

	void frmMain::toolModelEditorClicked( wxCommandEvent& event )
	{
		// TODO: Implement toolModelEditorClicked
	}

	void frmMain::toolParticleEditorClicked( wxCommandEvent& event )
	{
		// TODO: Implement toolParticleEditorClicked
	}

	void frmMain::toolConsoleEnterPressed( wxCommandEvent& event )
	{
		//TODO:Consoleif (g_pConsole) g_pConsole->Command(toolConsole->GetLineText(0));
		toolConsole->Clear();
		this->SetFocus();
	}

	void frmMain::mnuFileNewClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuFileNewClicked
	}

	void frmMain::mnuFileOpenClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuFileOpenClicked
	}

	void frmMain::mnuFileSaveClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuFileSaveClicked
	}

	void frmMain::mnuFileSaveAsClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuFileSaveAsClicked
	}

	void frmMain::mnuFileExitClicked( wxCommandEvent& event )
	{
		OnClose(wxCloseEvent());
	}

	void frmMain::mnuEditUndoClicked( wxCommandEvent& event )
	{
		//
	}

	void frmMain::mnuEditRedoClicked( wxCommandEvent& event )
	{
		//
	}

	void frmMain::mnuEditCopyClicked( wxCommandEvent& event )
	{
		//
	}

	void frmMain::mnuEditPasteClicked( wxCommandEvent& event )
	{
		//
	}

	void frmMain::mnuEditDeleteClicked( wxCommandEvent& event )
	{
		//
	}


	void frmMain::mnuViewHideSelectedClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuViewHideSelectedClicked
	}

	void frmMain::mnuViewShowHiddenClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuViewShowHiddenClicked
	}

	void frmMain::mnuDontSwitchToPropertiesClicked( wxCommandEvent& event )
	{
		mbDontSwitch = event.IsChecked();
		Settings::Set(_W("DontSwitchMenu"), mbDontSwitch);
	}

	void frmMain::mnuMapPropertiesClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuMapPropertiesClicked
	}

	void frmMain::mnuMapPlayClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuMapPlayClicked
	}

	void frmMain::mnuToolsAssetBrowserClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuToolsAssetBrowserClicked
	}

	void frmMain::mnuToolsMaterialEditorClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuToolsMaterialEditorClicked
	}

	void frmMain::mnuToolsModelViewerClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuToolsModelViewerClicked
	}

	void frmMain::mnuParticleEditorClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuParticleEditorClicked
	}

	void frmMain::mnuHelpOnlineClicked( wxCommandEvent& event )
	{
		// TODO: Implement mnuHelpOnlineClicked
	}

	void frmMain::mnuHelpAboutClicked( wxCommandEvent& event )
	{
		dlgAbout *pDlg = new dlgAbout(this);
		pDlg->ShowModal();
	}

	/*panRender* frmMain::GetViewport( eVT vtype )
	{
		switch(vtype)
		{
		case LEFT_TOP: return viewLT;
		case LEFT_BOTTOM: return viewLB;
		case RIGHT_TOP: return viewRT;
		default: return viewRB;
		}
	}*/

	void frmMain::AddConsoleMsg(const wchar* text, const wchar* scopeName)
	{
		/*txtConsole->AppendText(wxString(text)+wxString("\r\n"));
		txtConsole->SetSelection(txtConsole->GetLastPosition(), txtConsole->GetLastPosition());*/
	}

}