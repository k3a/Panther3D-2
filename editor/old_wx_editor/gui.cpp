/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

frmMainGui::frmMainGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	m_mgr.SetManagedWindow(this);

	this->SetMinSize(wxSize(400,300));
	
	// Toolbar - Standard ------------------
	toolbarStandard = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
	toolbarStandard->AddTool( wxID_NEW, wxT("Create new map"), wxBitmap( wxT("../data/editor/new.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("New Map"), wxT("New Map") );
	toolbarStandard->AddTool( wxID_OPEN, wxT("Open map..."), wxBitmap( wxT("../data/editor/open.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Open Map..."), wxT("Open Map...") );
	toolbarStandard->AddTool( wxID_SAVE, wxT("Save current map"), wxBitmap( wxT("../data/editor/save.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Save Current Map"), wxT("Save Current Map") );
	toolbarStandard->AddSeparator();
	toolbarStandard->AddTool( wxID_UNDO, wxT("tool"), wxBitmap( wxT("../data/editor/undo.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Undo changes"), wxT("Undo changes") );
	toolbarStandard->AddTool( wxID_REDO, wxT("tool"), wxBitmap( wxT("../data/editor/redo.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Redo changes"), wxT("Redo changes") );
	toolbarStandard->Realize();
	m_mgr.AddPane(toolbarStandard, wxAuiPaneInfo().
		Name(wxT("toolbar_standard")).Caption(wxT("Standard")).
		ToolbarPane().Top().
		LeftDockable(false).RightDockable(false));

	// Toolbar - Manipulators ------------------
	toolbarManipulators = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
	toolbarManipulators->AddTool( wxID_SELECT, wxT("Selection tool"), wxBitmap( wxT("../data/editor/select.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Selection tool"), wxT("Selection tool") );
	toolbarManipulators->AddTool( wxID_MOVE, wxT("Move tool"), wxBitmap( wxT("../data/editor/move.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Move tool"), wxT("Move tool") );
	toolbarManipulators->AddTool( wxID_ROTATE, wxT("Rotation tool"), wxBitmap( wxT("../data/editor/rotate.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Rotation tool"), wxT("Rotation tool") );
	toolbarManipulators->AddTool( wxID_SCALE, wxT("Scale tool"), wxBitmap( wxT("../data/editor/scale.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Scale tool"), wxT("Scale tool") );
	toolbarManipulators->AddTool( wxID_SCALE_SIDES, wxT("ScaleSides tool"), wxBitmap( wxT("../data/editor/scalesides.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("ScaleSides tool"), wxT("ScaleSides tool") );
	toolbarManipulators->AddTool( wxID_LOCAL, wxT("tool"), wxBitmap( wxT("../data/editor/local.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Use local (object) space for manipulation"), wxT("Use local (object) space for manipulation") );
	toolbarManipulators->Realize();
	m_mgr.AddPane(toolbarManipulators, wxAuiPaneInfo().
		Name(wxT("toolbar_manipulators")).Caption(wxT("Manipulators")).
		ToolbarPane().Top().
		LeftDockable(false).RightDockable(false));


	// Toolbar - Additional ------------------
	toolbarAdditional = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
	toolbarAdditional->AddTool( wxID_PHYS_IN_EDITOR, wxT("tool"), wxBitmap( wxT("../data/editor/physInEditor.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Enable collisions in editor"), wxT("Enable collisions in editor") );
	toolbarAdditional->AddTool( wxID_ORBIT, wxT("tool"), wxBitmap( wxT("../data/editor/orbit.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Toggle orbit view"), wxT("Toggle orbit view") );
	toolbarAdditional->AddTool( wxID_SHOW_GRID, wxT("tool"), wxBitmap( wxT("../data/editor/grid.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Show grid"), wxT("Show grid") );
	toolbarAdditional->AddTool( wxID_GRID_MINUS, wxT("tool"), wxBitmap( wxT("../data/editor/gridminus.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Smaller grid cells"), wxT("Smaller grid cells") );
	toolbarAdditional->AddTool( wxID_GRID_PLUS, wxT("tool"), wxBitmap( wxT("../data/editor/gridplus.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Bigger grid cells"), wxT("Bigger grid cells") );
	toolFarPlane = new wxSlider( toolbarAdditional, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize( 60,-1 ), wxSL_HORIZONTAL );
	toolFarPlane->SetToolTip( wxT("Far plane distance") );
	toolbarAdditional->AddControl( toolFarPlane );
	toolCameraSpeed = new wxSlider( toolbarAdditional, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize( 60,-1 ), wxSL_HORIZONTAL );
	toolCameraSpeed->SetToolTip( wxT("Camera speed") );
	toolbarAdditional->AddControl( toolCameraSpeed );
	toolbarAdditional->AddSeparator();
	toolbarAdditional->AddTool( wxID_PLAY, wxT("Play!"), wxBitmap( wxT("../data/editor/play.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Play!"), wxT("Play!") );
	toolbarAdditional->Realize();
	m_mgr.AddPane(toolbarAdditional, wxAuiPaneInfo().
		Name(wxT("toolbar_additional")).Caption(wxT("Additional")).
		ToolbarPane().Top().
		LeftDockable(false).RightDockable(false));

	// Toolbar - Tools ------------------
	toolbarTools = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
	toolbarTools->AddTool( wxID_ASSET_BROWSER, wxT("tool"), wxBitmap( wxT("../data/editor/assetbrowser.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Asset Browser"), wxT("Show Asset Browser") );
	toolbarTools->AddTool( wxID_MATERIAL_BROWSER, wxT("tool"), wxBitmap( wxT("../data/editor/materialeditor.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Material Editor"), wxT("Show Material Editor") );
	toolbarTools->AddTool( wxID_MODEL_EDITOR, wxT("tool"), wxBitmap( wxT("../data/editor/modeleditor.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Model Editor"), wxT("Show Model Editor") );
	toolbarTools->AddTool( wxID_PARTICLE_EDITOR, wxT("tool"), wxBitmap( wxT("../data/editor/particleeditor.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Particle Editor"), wxT("Show Particle Editor") );
	toolbarTools->Realize();
	m_mgr.AddPane(toolbarTools, wxAuiPaneInfo().
		Name(wxT("toolbar_tools")).Caption(wxT("Tools")).
		ToolbarPane().Top().
		LeftDockable(false).RightDockable(false));

	// Toolbar - Console ------------------
	toolbarConsole = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
	m_staticText314 = new wxStaticText( toolbarConsole, wxID_ANY, wxT("Console:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText314->Wrap( -1 );
	toolbarConsole->AddControl( m_staticText314 );
	toolConsole = new wxTextCtrl( toolbarConsole, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 98,-1 ), wxTE_PROCESS_ENTER );
	toolbarConsole->AddControl( toolConsole );
	toolbarConsole->Realize();
	m_mgr.AddPane(toolbarConsole, wxAuiPaneInfo().
		Name(wxT("toolbar_console")).Caption(wxT("Console")).
		ToolbarPane().Top().
		LeftDockable(false).RightDockable(false));







	
	menu = new wxMenuBar( 0 );
	mnuFile = new wxMenu();
	wxMenuItem* mnuFileNew;
	mnuFileNew = new wxMenuItem( mnuFile, wxID_ANY, wxString( wxT("&New") ) + wxT('\t') + wxT("CTRL+N"), wxT("Create new map"), wxITEM_NORMAL );
	mnuFile->Append( mnuFileNew );
	
	wxMenuItem* mnuFileOpen;
	mnuFileOpen = new wxMenuItem( mnuFile, wxID_ANY, wxString( wxT("&Open...") ) + wxT('\t') + wxT("CTRL+O"), wxT("Open Map File..."), wxITEM_NORMAL );
	mnuFile->Append( mnuFileOpen );
	
	wxMenuItem* mnuFileSave;
	mnuFileSave = new wxMenuItem( mnuFile, wxID_ANY, wxString( wxT("Save") ) + wxT('\t') + wxT("CTRL+S"), wxT("Save current map"), wxITEM_NORMAL );
	mnuFile->Append( mnuFileSave );
	
	wxMenuItem* mnuFileSaveAs;
	mnuFileSaveAs = new wxMenuItem( mnuFile, wxID_ANY, wxString( wxT("Save &As...") ) + wxT('\t') + wxT("CTRL+SHIFT+S"), wxT("Save current map as..."), wxITEM_NORMAL );
	mnuFile->Append( mnuFileSaveAs );
	
	mnuFile->AppendSeparator();
	
	wxMenuItem* mnuFileExit;
	mnuFileExit = new wxMenuItem( mnuFile, wxID_ANY, wxString( wxT("&Exit") ) , wxT("Exit P3DCrane"), wxITEM_NORMAL );
	mnuFile->Append( mnuFileExit );
	
	menu->Append( mnuFile, wxT("&File") );
	
	mnuEdit = new wxMenu();
	wxMenuItem* mnuEditUndo;
	mnuEditUndo = new wxMenuItem( mnuEdit, wxID_ANY, wxString( wxT("&Undo") ) + wxT('\t') + wxT("CTRL+Z"), wxT("Undo changes"), wxITEM_NORMAL );
	mnuEdit->Append( mnuEditUndo );
	
	wxMenuItem* mnuEditRedo;
	mnuEditRedo = new wxMenuItem( mnuEdit, wxID_ANY, wxString( wxT("&Redo") ) + wxT('\t') + wxT("CTRL+Y"), wxT("Redo changes"), wxITEM_NORMAL );
	mnuEdit->Append( mnuEditRedo );
	
	mnuEdit->AppendSeparator();
	
	wxMenuItem* mnuEditCopy;
	mnuEditCopy = new wxMenuItem( mnuEdit, wxID_ANY, wxString( wxT("&Copy") ) + wxT('\t') + wxT("CTRL+C"), wxT("Copy selected"), wxITEM_NORMAL );
	mnuEdit->Append( mnuEditCopy );
	
	wxMenuItem* mnuEditPaste;
	mnuEditPaste = new wxMenuItem( mnuEdit, wxID_ANY, wxString( wxT("&Paste") ) + wxT('\t') + wxT("CTRL+V"), wxT("Paste copied"), wxITEM_NORMAL );
	mnuEdit->Append( mnuEditPaste );
	
	wxMenuItem* mnuEditDelete;
	mnuEditDelete = new wxMenuItem( mnuEdit, wxID_ANY, wxString( wxT("&Delete") ) + wxT('\t') + wxT("CTRL+D"), wxT("Delete selected"), wxITEM_NORMAL );
	mnuEdit->Append( mnuEditDelete );
	
	menu->Append( mnuEdit, wxT("&Edit") );
	
	mnuView = new wxMenu();
	wxMenuItem* mnuViewHideSelected;
	mnuViewHideSelected = new wxMenuItem( mnuView, wxID_ANY, wxString( wxT("Hide Selected Entities") ) + wxT('\t') + wxT("H"), wxT("Hide current selection"), wxITEM_NORMAL );
	mnuView->Append( mnuViewHideSelected );
	
	wxMenuItem* mnuViewShowHidden;
	mnuViewShowHidden = new wxMenuItem( mnuView, wxID_ANY, wxString( wxT("Show Hidden Entities") ) + wxT('\t') + wxT("SHIFT+H"), wxT("Show all hidden entities"), wxITEM_NORMAL );
	mnuView->Append( mnuViewShowHidden );
	
	mnuView->AppendSeparator();
	
	wxMenuItem* mnuDontSwitchToProperties;
	mnuDontSwitchToProperties = new wxMenuItem( mnuView, wxID_DONT_SWITCH, wxString( wxT("Don't Switch to Properties") ) , wxT("Don't Switch to Properties after entity creation"), wxITEM_CHECK );
	mnuView->Append( mnuDontSwitchToProperties );
	
	menu->Append( mnuView, wxT("&View") );
	
	mnuMap = new wxMenu();
	wxMenuItem* mnuMapProperties;
	mnuMapProperties = new wxMenuItem( mnuMap, wxID_ANY, wxString( wxT("Edit &Properties...") ) , wxT("Edit Map Properties..."), wxITEM_NORMAL );
	mnuMap->Append( mnuMapProperties );
	
	wxMenuItem* mnuMapPlay;
	mnuMapPlay = new wxMenuItem( mnuMap, wxID_ANY, wxString( wxT("Play!") ) , wxT("Play This Map!"), wxITEM_NORMAL );
	mnuMap->Append( mnuMapPlay );
	
	menu->Append( mnuMap, wxT("&Map") );
	
	mnuTools = new wxMenu();
	wxMenuItem* mnuToolsAssetBrowser;
	mnuToolsAssetBrowser = new wxMenuItem( mnuTools, wxID_ANY, wxString( wxT("&Asset Browser...") ) + wxT('\t') + wxT("CTRL+A"), wxT("Browse game assets..."), wxITEM_NORMAL );
	mnuTools->Append( mnuToolsAssetBrowser );
	
	wxMenuItem* mnuToolsMaterialEditor;
	mnuToolsMaterialEditor = new wxMenuItem( mnuTools, wxID_ANY, wxString( wxT("&Material Editor...") ) , wxT("Create and edit materials, assign textures, ..."), wxITEM_NORMAL );
	mnuTools->Append( mnuToolsMaterialEditor );
	
	wxMenuItem* mnuToolsModelViewer;
	mnuToolsModelViewer = new wxMenuItem( mnuTools, wxID_ANY, wxString( wxT("Model &Viewer...") ) , wxT("Edit game models, create physics shape, events, ..."), wxITEM_NORMAL );
	mnuTools->Append( mnuToolsModelViewer );
	
	wxMenuItem* mnuToolsParticleEditor;
	mnuToolsParticleEditor = new wxMenuItem( mnuTools, wxID_ANY, wxString( wxT("&Particle Editor...") ) , wxT("Create and edit particle effects..."), wxITEM_NORMAL );
	mnuTools->Append( mnuToolsParticleEditor );
	
	menu->Append( mnuTools, wxT("&Tools") );
	
	mnuHelp = new wxMenu();
	wxMenuItem* mnuHelpOnline;
	mnuHelpOnline = new wxMenuItem( mnuHelp, wxID_ANY, wxString( wxT("Online &Help...") ) + wxT('\t') + wxT("F1"), wxT("Open browser window with online help..."), wxITEM_NORMAL );
	mnuHelp->Append( mnuHelpOnline );
	
	wxMenuItem* mnuHelpAbout;
	mnuHelpAbout = new wxMenuItem( mnuHelp, wxID_ANY, wxString( wxT("&About P3DCrane...") ) , wxT("Show information about editor..."), wxITEM_NORMAL );
	mnuHelp->Append( mnuHelpAbout );
	
	menu->Append( mnuHelp, wxT("&Help") );
	
	this->SetMenuBar( menu );
	
	
	this->Centre( wxBOTH );

	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( frmMainGui::OnClose ) );
	this->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( frmMainGui::OnUpdateUI ) );
	this->Connect( wxID_NEW, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolNewClicked ) );
	this->Connect( wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolOpenClicked ) );
	this->Connect( wxID_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolSaveClicked ) );
	this->Connect( wxID_SELECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolSelectClicked ) );
	this->Connect( wxID_MOVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolMoveClicked ) );
	this->Connect( wxID_ROTATE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolRotateClicked ) );
	this->Connect( wxID_SCALE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolScaleClicked ) );
	this->Connect( wxID_SCALE_SIDES, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolScaleSidesClicked ) );
	this->Connect( wxID_LOCAL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolLocalClicked ) );
	this->Connect( wxID_PHYS_IN_EDITOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolPhysInEditorClicked ) );
	this->Connect( wxID_ORBIT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolOrbitClicked ) );
	this->Connect( wxID_SHOW_GRID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolShowGridClicked ) );
	this->Connect( wxID_GRID_MINUS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolGridMinusClicked ) );
	this->Connect( wxID_GRID_PLUS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolGridPlusClicked ) );
	toolFarPlane->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( frmMainGui::toolFarPlaneThumbScrollReleased ), NULL, this );
	toolCameraSpeed->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( frmMainGui::toolCameraSpeedScrollReleased ), NULL, this );
	this->Connect( wxID_PLAY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolPlayClicked ) );
	this->Connect( wxID_ASSET_BROWSER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolAssetBrowserClicked ) );
	this->Connect( wxID_MATERIAL_BROWSER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolMaterialEditorClicked ) );
	this->Connect( wxID_MODEL_EDITOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolModelEditorClicked ) );
	this->Connect( wxID_PARTICLE_EDITOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolParticleEditorClicked ) );
	toolConsole->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( frmMainGui::toolConsoleEnterPressed ), NULL, this );
	this->Connect( mnuFileNew->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuFileNewClicked ) );
	this->Connect( mnuFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuFileOpenClicked ) );
	this->Connect( mnuFileSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuFileSaveClicked ) );
	this->Connect( mnuFileSaveAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuFileSaveAsClicked ) );
	this->Connect( mnuFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuFileExitClicked ) );
	this->Connect( mnuEditUndo->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuEditUndoClicked ) );
	this->Connect( mnuEditRedo->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuEditRedoClicked ) );
	this->Connect( mnuEditCopy->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuEditCopyClicked ) );
	this->Connect( mnuEditPaste->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuEditPasteClicked ) );
	this->Connect( mnuEditDelete->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuEditDeleteClicked ) );
	this->Connect( mnuViewHideSelected->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuViewHideSelectedClicked ) );
	this->Connect( mnuViewShowHidden->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuViewShowHiddenClicked ) );
	this->Connect( mnuDontSwitchToProperties->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuDontSwitchToPropertiesClicked ) );
	this->Connect( mnuMapProperties->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuMapPropertiesClicked ) );
	this->Connect( mnuMapPlay->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuMapPlayClicked ) );
	this->Connect( mnuToolsAssetBrowser->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuToolsAssetBrowserClicked ) );
	this->Connect( mnuToolsMaterialEditor->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuToolsMaterialEditorClicked ) );
	this->Connect( mnuToolsModelViewer->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuToolsModelViewerClicked ) );
	this->Connect( mnuToolsParticleEditor->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuParticleEditorClicked ) );
	this->Connect( mnuHelpOnline->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuHelpOnlineClicked ) );
	this->Connect( mnuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuHelpAboutClicked ) );
}

frmMainGui::~frmMainGui()
{
	m_mgr.UnInit();

	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( frmMainGui::OnClose ) );
	this->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( frmMainGui::OnUpdateUI ) );
	this->Disconnect( wxID_NEW, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolNewClicked ) );
	this->Disconnect( wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolOpenClicked ) );
	this->Disconnect( wxID_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolSaveClicked ) );
	this->Disconnect( wxID_SELECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolSelectClicked ) );
	this->Disconnect( wxID_MOVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolMoveClicked ) );
	this->Disconnect( wxID_ROTATE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolRotateClicked ) );
	this->Disconnect( wxID_SCALE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolScaleClicked ) );
	this->Disconnect( wxID_SCALE_SIDES, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolScaleSidesClicked ) );
	this->Disconnect( wxID_LOCAL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolLocalClicked ) );
	this->Disconnect( wxID_PHYS_IN_EDITOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolPhysInEditorClicked ) );
	this->Disconnect( wxID_ORBIT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolOrbitClicked ) );
	this->Disconnect( wxID_SHOW_GRID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolShowGridClicked ) );
	this->Disconnect( wxID_GRID_MINUS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolGridMinusClicked ) );
	this->Disconnect( wxID_GRID_PLUS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolGridPlusClicked ) );
	toolFarPlane->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( frmMainGui::toolFarPlaneThumbScrollReleased ), NULL, this );
	toolCameraSpeed->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( frmMainGui::toolCameraSpeedScrollReleased ), NULL, this );
	this->Disconnect( wxID_PLAY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolPlayClicked ) );
	this->Disconnect( wxID_ASSET_BROWSER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolAssetBrowserClicked ) );
	this->Disconnect( wxID_MATERIAL_BROWSER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolMaterialEditorClicked ) );
	this->Disconnect( wxID_MODEL_EDITOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolModelEditorClicked ) );
	this->Disconnect( wxID_PARTICLE_EDITOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMainGui::toolParticleEditorClicked ) );
	toolConsole->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( frmMainGui::toolConsoleEnterPressed ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuFileNewClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuFileOpenClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuFileSaveClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuFileSaveAsClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuFileExitClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuEditUndoClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuEditRedoClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuEditCopyClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuEditPasteClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuEditDeleteClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuViewHideSelectedClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuViewShowHiddenClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuDontSwitchToPropertiesClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuMapPropertiesClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuMapPlayClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuToolsAssetBrowserClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuToolsMaterialEditorClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuToolsModelViewerClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuParticleEditorClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuHelpOnlineClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainGui::mnuHelpAboutClicked ) );
}

dlgSplashGui::dlgSplashGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer277;
	bSizer277 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmap8 = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("../data/editor/splash.bmp"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer277->Add( m_bitmap8, 0, wxALL, 0 );
	
	txtInfo = new wxStaticText( this, wxID_ANY, wxT(" Panther3D Engine 1.1.1 | Game"), wxPoint( -1,-1 ), wxSize( 441,-1 ), 0|wxDOUBLE_BORDER );
	txtInfo->Wrap( -1 );
	bSizer277->Add( txtInfo, 0, wxALL, 4 );
	
	this->SetSizer( bSizer277 );
	this->Layout();
}

dlgSplashGui::~dlgSplashGui()
{
}

panRenderGui::panRenderGui( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	wxBoxSizer* bSizer279;
	bSizer279 = new wxBoxSizer( wxVERTICAL );
	
	m_toolBar2 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); 
	m_toolBar2->AddTool( wxID_MAXIMIZE, wxT("Maximize this viewport"), wxBitmap( wxT("../data/editor/maximize.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Maximize this viewport"), wxT("Maximize this viewport") );
	m_toolBar2->AddTool( wxID_RIGHT, wxT("Right view"), wxBitmap( wxT("../data/editor/right.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Right view"), wxT("Right view") );
	m_toolBar2->AddTool( wxID_TOP, wxT("Top view"), wxBitmap( wxT("../data/editor/top.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Top view"), wxT("Top view") );
	m_toolBar2->AddTool( wxID_FRONT, wxT("Front view"), wxBitmap( wxT("../data/editor/front.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Front view"), wxT("Front view") );
	m_toolBar2->AddTool( wxID_PERSPECT, wxT("Perspective view"), wxBitmap( wxT("../data/editor/perspective.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Perspective view"), wxT("Perspective view") );
	m_toolBar2->AddSeparator();
	m_toolBar2->AddTool( wxID_WIRE, wxT("Wireframe mode"), wxBitmap( wxT("../data/editor/wireframe.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Wireframe mode"), wxT("Wireframe mode") );
	m_toolBar2->AddTool( wxID_TEXT, wxT("Textured mode"), wxBitmap( wxT("../data/editor/textured.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Textured mode"), wxT("Textured mode") );
	m_toolBar2->AddTool( wxID_LIT, wxT("Lit mode"), wxBitmap( wxT("../data/editor/lit.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Lit mode"), wxT("Lit mode") );
	m_toolBar2->AddSeparator();
	m_toolBar2->AddTool( wxID_REALTIME, wxT("Toggle realtime rendering"), wxBitmap( wxT("../data/editor/realtime.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Toggle realtime rendering"), wxT("Toggle realtime rendering") );
	m_toolBar2->AddTool( wxID_RENDER, wxT("tool"), wxBitmap( wxT("../data/editor/more.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Render parameters"), wxT("Render parameters") );
	m_toolBar2->Realize();
	
	bSizer279->Add( m_toolBar2, 0, wxEXPAND, 5 );
	
	panViewport = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	panViewport->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	bSizer279->Add( panViewport, 1, wxEXPAND | wxALL, 2 );
	
	this->SetSizer( bSizer279 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( panRenderGui::OnIdle ) );
	this->Connect( wxID_MAXIMIZE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolMaximizeClicked ) );
	this->Connect( wxID_RIGHT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolRightClicked ) );
	this->Connect( wxID_TOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolTopClicked ) );
	this->Connect( wxID_FRONT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolFrontClicked ) );
	this->Connect( wxID_PERSPECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolPerspectClicked ) );
	this->Connect( wxID_WIRE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolWireClicked ) );
	this->Connect( wxID_TEXT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolTextClicked ) );
	this->Connect( wxID_LIT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolLitClicked ) );
	this->Connect( wxID_REALTIME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolRealtimeClicked ) );
	this->Connect( wxID_RENDER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolRenderClicked ) );
	panViewport->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Connect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Connect( wxEVT_MIDDLE_UP, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Connect( wxEVT_MOTION, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
}

panRenderGui::~panRenderGui()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( panRenderGui::OnIdle ) );
	this->Disconnect( wxID_MAXIMIZE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolMaximizeClicked ) );
	this->Disconnect( wxID_RIGHT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolRightClicked ) );
	this->Disconnect( wxID_TOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolTopClicked ) );
	this->Disconnect( wxID_FRONT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolFrontClicked ) );
	this->Disconnect( wxID_PERSPECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolPerspectClicked ) );
	this->Disconnect( wxID_WIRE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolWireClicked ) );
	this->Disconnect( wxID_TEXT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolTextClicked ) );
	this->Disconnect( wxID_LIT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolLitClicked ) );
	this->Disconnect( wxID_REALTIME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolRealtimeClicked ) );
	this->Disconnect( wxID_RENDER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panRenderGui::toolRenderClicked ) );
	panViewport->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Disconnect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Disconnect( wxEVT_MIDDLE_UP, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
	panViewport->Disconnect( wxEVT_MOTION, wxMouseEventHandler( panRenderGui::OnMouse ), NULL, this );
}

dlgAboutGui::dlgAboutGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer91;
	fgSizer91 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer91->AddGrowableCol( 0 );
	fgSizer91->AddGrowableRow( 0 );
	fgSizer91->SetFlexibleDirection( wxBOTH );
	fgSizer91->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer94;
	sbSizer94 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer93;
	fgSizer93 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer93->AddGrowableCol( 1 );
	fgSizer93->AddGrowableRow( 0 );
	fgSizer93->SetFlexibleDirection( wxBOTH );
	fgSizer93->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bitmap9 = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("../data/editor/about.bmp"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer93->Add( m_bitmap9, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer94;
	fgSizer94 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer94->AddGrowableCol( 0 );
	fgSizer94->AddGrowableRow( 1 );
	fgSizer94->SetFlexibleDirection( wxVERTICAL );
	fgSizer94->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer331;
	bSizer331 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText323 = new wxStaticText( this, wxID_ANY, wxT("P3DCrane2\nPanther3D World Editor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText323->Wrap( -1 );
	m_staticText323->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );
	
	bSizer331->Add( m_staticText323, 0, wxALL|wxEXPAND, 5 );
	
	m_staticline11 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer331->Add( m_staticline11, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer332;
	bSizer332 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText324 = new wxStaticText( this, wxID_ANY, wxT("Panther3D Engine:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText324->Wrap( -1 );
	bSizer332->Add( m_staticText324, 0, wxLEFT, 5 );
	
	m_staticText326 = new wxStaticText( this, wxID_ANY, wxT("version string"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText326->Wrap( -1 );
	bSizer332->Add( m_staticText326, 0, 0, 2 );
	
	bSizer331->Add( bSizer332, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer333;
	bSizer333 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText327 = new wxStaticText( this, wxID_ANY, wxT("Loaded Modules:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText327->Wrap( -1 );
	bSizer333->Add( m_staticText327, 0, wxLEFT, 5 );
	
	m_staticText325 = new wxStaticText( this, wxID_ANY, wxT("Module.dll, Module.dll"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText325->Wrap( -1 );
	m_staticText325->SetMaxSize( wxSize( -1,12 ) );
	
	bSizer333->Add( m_staticText325, 0, 0, 3 );
	
	bSizer331->Add( bSizer333, 1, wxEXPAND, 5 );
	
	m_staticline12 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer331->Add( m_staticline12, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText330 = new wxStaticText( this, wxID_ANY, wxT("System information:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText330->Wrap( -1 );
	bSizer331->Add( m_staticText330, 0, wxALL, 5 );
	
	fgSizer94->Add( bSizer331, 1, wxEXPAND, 5 );
	
	m_staticText328 = new wxStaticText( this, wxID_ANY, wxT("Procesor: Intel Core 2 Duo 2,4 GHz\nCapabilities: MMX, SSE, SSE2\nSystem RAM: 2048 MB\nGraphic Card: nVidia GeForce 8800 GTS\nCapabilities: MULTIPLE_RENDER_TARGETS, ..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText328->Wrap( -1 );
	fgSizer94->Add( m_staticText328, 0, wxALL, 5 );
	
	fgSizer93->Add( fgSizer94, 1, wxEXPAND, 5 );
	
	sbSizer94->Add( fgSizer93, 1, wxEXPAND, 5 );
	
	fgSizer91->Add( sbSizer94, 1, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer92;
	fgSizer92 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer92->AddGrowableCol( 0 );
	fgSizer92->AddGrowableRow( 0 );
	fgSizer92->SetFlexibleDirection( wxBOTH );
	fgSizer92->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText322 = new wxStaticText( this, wxID_ANY, wxT(" Copyright (C) 2008-2009 Reversity Studios (www.reversity.org)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText322->Wrap( -1 );
	fgSizer92->Add( m_staticText322, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer328;
	bSizer328 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button175 = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button175->SetDefault(); 
	bSizer328->Add( m_button175, 0, wxALL, 5 );
	
	fgSizer92->Add( bSizer328, 1, wxEXPAND, 5 );
	
	fgSizer91->Add( fgSizer92, 1, wxEXPAND, 5 );
	
	this->SetSizer( fgSizer91 );
	this->Layout();
	
	// Connect Events
	m_button175->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgAboutGui::OnClose ), NULL, this );
}

dlgAboutGui::~dlgAboutGui()
{
	// Disconnect Events
	m_button175->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgAboutGui::OnClose ), NULL, this );
}

panDataToolGui::panDataToolGui( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer;
	fgSizer = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer->AddGrowableCol( 0 );
	fgSizer->AddGrowableRow( 0 );
	fgSizer->AddGrowableRow( 1 );
	fgSizer->SetFlexibleDirection( wxBOTH );
	fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	panTop = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* szToolNotebook;
	szToolNotebook = new wxBoxSizer( wxVERTICAL );
	
	toolbar = new wxToolBar( panTop, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); 
	toolbar->AddTool( wxID_ANY, wxT("Create Folder"), wxBitmap( wxT("../data/editor/add_folder.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Create Folder"), wxEmptyString );
	toolbar->AddTool( wxID_ANY, wxT("Import"), wxBitmap( wxT("../data/editor/import_add.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Import Data"), wxEmptyString );
	toolbar->AddTool( wxID_ANY, wxT("Delete Item"), wxBitmap( wxT("../data/editor/delete.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Delete Item"), wxEmptyString );
	toolbar->AddSeparator();
	toolbar->AddTool( wxID_SHOW_PREV, wxT("tool"), wxBitmap( wxT("../data/editor/eye.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Show Preview"), wxEmptyString );
	toolbar->AddSeparator();
	toolbar->AddTool( ID_SIZE10, wxT("tool"), wxBitmap( wxT("../data/editor/10_percent.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("10 % original size"), wxEmptyString );
	toolbar->AddTool( ID_SIZE30, wxT("tool"), wxBitmap( wxT("../data/editor/30_percent.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("30 % original size"), wxEmptyString );
	toolbar->AddTool( ID_SIZE50, wxT("tool"), wxBitmap( wxT("../data/editor/50_percent.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("50 % original size"), wxEmptyString );
	toolbar->AddTool( ID_SIZE100, wxT("tool"), wxBitmap( wxT("../data/editor/100_percent.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, wxT("Original size"), wxEmptyString );
	toolbar->Realize();
	
	szToolNotebook->Add( toolbar, 0, wxEXPAND, 0 );
	
	nbMain = new wxAuiNotebook( panTop, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxNO_BORDER );
	panModels = new wxPanel( nbMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	panModels->SetToolTip( wxT("Models") );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	treeModels = new wxTreeCtrl( panModels, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxNO_BORDER );
	bSizer1->Add( treeModels, 1, wxEXPAND, 5 );
	
	panModels->SetSizer( bSizer1 );
	panModels->Layout();
	bSizer1->Fit( panModels );
	nbMain->AddPage( panModels, wxEmptyString, true, wxBitmap( wxT("../data/editor/model.ico"), wxBITMAP_TYPE_ANY ) );
	panMaterials = new wxPanel( nbMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	panMaterials->SetToolTip( wxT("Materials") );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	treeMaterials = new wxTreeCtrl( panMaterials, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxNO_BORDER );
	bSizer2->Add( treeMaterials, 1, wxEXPAND, 5 );
	
	panMaterials->SetSizer( bSizer2 );
	panMaterials->Layout();
	bSizer2->Fit( panMaterials );
	nbMain->AddPage( panMaterials, wxEmptyString, false, wxBitmap( wxT("../data/editor/material.ico"), wxBITMAP_TYPE_ANY ) );
	panTextures = new wxPanel( nbMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	panTextures->SetToolTip( wxT("Textures") );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	treeTextures = new wxTreeCtrl( panTextures, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxNO_BORDER );
	bSizer3->Add( treeTextures, 1, wxEXPAND, 5 );
	
	panTextures->SetSizer( bSizer3 );
	panTextures->Layout();
	bSizer3->Fit( panTextures );
	nbMain->AddPage( panTextures, wxEmptyString, false, wxBitmap( wxT("../data/editor/texture.ico"), wxBITMAP_TYPE_ANY ) );
	panParticles = new wxPanel( nbMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	panParticles->SetToolTip( wxT("Particle Systems") );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	treeParticles = new wxTreeCtrl( panParticles, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxNO_BORDER );
	bSizer4->Add( treeParticles, 1, wxEXPAND, 5 );
	
	panParticles->SetSizer( bSizer4 );
	panParticles->Layout();
	bSizer4->Fit( panParticles );
	nbMain->AddPage( panParticles, wxEmptyString, false, wxBitmap( wxT("../data/editor/particle.ico"), wxBITMAP_TYPE_ANY ) );
	
	szToolNotebook->Add( nbMain, 1, wxEXPAND | wxALL, 1 );
	
	panTop->SetSizer( szToolNotebook );
	panTop->Layout();
	szToolNotebook->Fit( panTop );
	fgSizer->Add( panTop, 1, wxALL|wxEXPAND, 0 );
	
	wxFlexGridSizer* fgList;
	fgList = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgList->AddGrowableCol( 0 );
	fgList->AddGrowableRow( 0 );
	fgList->SetFlexibleDirection( wxBOTH );
	fgList->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	panList = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	fgList->Add( panList, 1, wxALL|wxEXPAND, 1 );
	
	scrList = new wxScrollBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
	scrList->SetMinSize( wxSize( 17,-1 ) );
	
	fgList->Add( scrList, 0, wxBOTTOM|wxEXPAND|wxRIGHT|wxTOP, 2 );
	
	fgSizer->Add( fgList, 1, wxEXPAND, 2 );
	
	panPrev = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,200 ), wxSTATIC_BORDER|wxTAB_TRAVERSAL );
	fgSizer->Add( panPrev, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 1 );
	
	this->SetSizer( fgSizer );
	this->Layout();
	
	// Connect Events
	this->Connect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolCreateFolderClicked ) );
	this->Connect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolImportClicked ) );
	this->Connect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolDeleteClicked ) );
	this->Connect( wxID_SHOW_PREV, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolPreviewClicked ) );
	this->Connect( ID_SIZE10, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolListSizeClicked ) );
	this->Connect( ID_SIZE30, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolListSizeClicked ) );
	this->Connect( ID_SIZE50, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolListSizeClicked ) );
	this->Connect( ID_SIZE100, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolListSizeClicked ) );
	panList->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( panDataToolGui::panListLeftDClick ), NULL, this );
	panList->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( panDataToolGui::panListLeftDown ), NULL, this );
}

panDataToolGui::~panDataToolGui()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolCreateFolderClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolImportClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolDeleteClicked ) );
	this->Disconnect( wxID_SHOW_PREV, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolPreviewClicked ) );
	this->Disconnect( ID_SIZE10, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolListSizeClicked ) );
	this->Disconnect( ID_SIZE30, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolListSizeClicked ) );
	this->Disconnect( ID_SIZE50, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolListSizeClicked ) );
	this->Disconnect( ID_SIZE100, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( panDataToolGui::toolListSizeClicked ) );
	panList->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( panDataToolGui::panListLeftDClick ), NULL, this );
	panList->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( panDataToolGui::panListLeftDown ), NULL, this );
}

panRollupGui::panRollupGui( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgMain;
	fgMain = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgMain->AddGrowableCol( 0 );
	fgMain->AddGrowableRow( 0 );
	fgMain->AddGrowableRow( 1 );
	fgMain->SetFlexibleDirection( wxBOTH );
	fgMain->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	nbMain = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxNO_BORDER );
	scrCreate = new wxScrolledWindow( nbMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	scrCreate->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgCreate;
	fgCreate = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgCreate->AddGrowableCol( 0 );
	fgCreate->SetFlexibleDirection( wxBOTH );
	fgCreate->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbObject;
	sbObject = new wxStaticBoxSizer( new wxStaticBox( scrCreate, wxID_ANY, wxT("Object") ), wxVERTICAL );
	
	wxGridSizer* grSizer;
	grSizer = new wxGridSizer( 2, 2, 0, 0 );
	
	btnEntity = new wxToggleButton( scrCreate, wxID_ANY, wxT("Entity"), wxDefaultPosition, wxDefaultSize, 0 );
	grSizer->Add( btnEntity, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 2 );
	
	btnSolid = new wxToggleButton( scrCreate, wxID_ANY, wxT("Solid"), wxDefaultPosition, wxDefaultSize, 0 );
	grSizer->Add( btnSolid, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 2 );
	
	btnArea = new wxToggleButton( scrCreate, wxID_ANY, wxT("Area"), wxDefaultPosition, wxDefaultSize, 0 );
	grSizer->Add( btnArea, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 2 );
	
	btnTerrain = new wxToggleButton( scrCreate, wxID_ANY, wxT("Terrain"), wxDefaultPosition, wxDefaultSize, 0 );
	grSizer->Add( btnTerrain, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 2 );
	
	sbObject->Add( grSizer, 1, wxEXPAND, 5 );
	
	fgCreate->Add( sbObject, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbEntity;
	sbEntity = new wxStaticBoxSizer( new wxStaticBox( scrCreate, wxID_ANY, wxT("Entity") ), wxVERTICAL );
	
	wxStaticBoxSizer* sbEntityPosition;
	sbEntityPosition = new wxStaticBoxSizer( new wxStaticBox( scrCreate, wxID_ANY, wxT("Position") ), wxVERTICAL );
	
	radCollisionPoint = new wxRadioButton( scrCreate, wxID_ANY, wxT("Collision Point"), wxDefaultPosition, wxDefaultSize, 0 );
	sbEntityPosition->Add( radCollisionPoint, 0, wxALL, 2 );
	
	radCameraPos = new wxRadioButton( scrCreate, wxID_ANY, wxT("Camera Position"), wxDefaultPosition, wxDefaultSize, 0 );
	sbEntityPosition->Add( radCameraPos, 0, wxALL, 2 );
	
	radSpecPos = new wxRadioButton( scrCreate, wxID_ANY, wxT("Specified Position:"), wxDefaultPosition, wxDefaultSize, 0 );
	sbEntityPosition->Add( radSpecPos, 0, wxALL, 2 );
	
	wxBoxSizer* bSizer313;
	bSizer313 = new wxBoxSizer( wxHORIZONTAL );
	
	txtX = new wxTextCtrl( scrCreate, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	bSizer313->Add( txtX, 0, wxALL, 2 );
	
	txtY = new wxTextCtrl( scrCreate, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	bSizer313->Add( txtY, 0, wxALL, 2 );
	
	txtZ = new wxTextCtrl( scrCreate, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	bSizer313->Add( txtZ, 0, wxALL, 2 );
	
	sbEntityPosition->Add( bSizer313, 1, wxEXPAND, 5 );
	
	sbEntity->Add( sbEntityPosition, 1, wxEXPAND, 5 );
	
	lstEntities = new wxListCtrl( scrCreate, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	lstEntities->SetMinSize( wxSize( -1,180 ) );
	
	sbEntity->Add( lstEntities, 0, wxALL|wxEXPAND, 2 );
	
	wxFlexGridSizer* fgFilter;
	fgFilter = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgFilter->AddGrowableCol( 1 );
	fgFilter->AddGrowableRow( 0 );
	fgFilter->SetFlexibleDirection( wxHORIZONTAL );
	fgFilter->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	lblFilter = new wxStaticText( scrCreate, wxID_ANY, wxT("Filter:"), wxDefaultPosition, wxDefaultSize, 0 );
	lblFilter->Wrap( -1 );
	fgFilter->Add( lblFilter, 0, wxALL, 5 );
	
	txtFilter = new wxTextCtrl( scrCreate, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), wxTE_PROCESS_ENTER );
	fgFilter->Add( txtFilter, 0, wxALL|wxEXPAND, 2 );
	
	sbEntity->Add( fgFilter, 0, wxEXPAND, 5 );
	
	btnCreateEntity = new wxButton( scrCreate, wxID_ANY, wxT("Create Entity"), wxDefaultPosition, wxDefaultSize, 0 );
	sbEntity->Add( btnCreateEntity, 0, wxALL|wxEXPAND, 1 );
	
	fgCreate->Add( sbEntity, 1, wxEXPAND, 5 );
	
	scrCreate->SetSizer( fgCreate );
	scrCreate->Layout();
	fgCreate->Fit( scrCreate );
	nbMain->AddPage( scrCreate, wxEmptyString, true, wxBitmap( wxT("../data/editor/create.ico"), wxBITMAP_TYPE_ANY ) );
	scrEdit = new wxScrolledWindow( nbMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	scrEdit->SetScrollRate( 5, 5 );
	scrEdit->SetToolTip( wxT("Edit") );
	
	wxFlexGridSizer* fgEdit;
	fgEdit = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgEdit->AddGrowableCol( 0 );
	fgEdit->SetFlexibleDirection( wxBOTH );
	fgEdit->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbProperties;
	sbProperties = new wxStaticBoxSizer( new wxStaticBox( scrEdit, wxID_ANY, wxT("Properties") ), wxVERTICAL );
	
	sbProperties->SetMinSize( wxSize( -1,450 ) ); 
//	pgProp = new wxPropertyGridManager(scrEdit, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxPG_BOLD_MODIFIED|wxPG_DESCRIPTION|wxPG_HIDE_MARGIN|wxPG_SPLITTER_AUTO_CENTER|wxPG_TOOLBAR|wxPG_TOOLTIPS);
//	pgProp->SetExtraStyle( wxPG_EX_MODE_BUTTONS ); 
//	sbProperties->Add( pgProp, 1, wxEXPAND, 5 );
	
	fgEdit->Add( sbProperties, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbEvents;
	sbEvents = new wxStaticBoxSizer( new wxStaticBox( scrEdit, wxID_ANY, wxT("Events") ), wxVERTICAL );
	
	wxFlexGridSizer* fgEvents;
	fgEvents = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgEvents->AddGrowableCol( 0 );
	fgEvents->AddGrowableRow( 0 );
	fgEvents->SetFlexibleDirection( wxBOTH );
	fgEvents->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgEventsTargets;
	fgEventsTargets = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgEventsTargets->AddGrowableCol( 0 );
	fgEventsTargets->AddGrowableRow( 1 );
	fgEventsTargets->SetFlexibleDirection( wxBOTH );
	fgEventsTargets->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	lblEventsTargets = new wxStaticText( scrEdit, wxID_ANY, wxT("Entity Events and Targets:"), wxDefaultPosition, wxDefaultSize, 0 );
	lblEventsTargets->Wrap( -1 );
	lblEventsTargets->SetToolTip( wxT("Selected entity events and which target entities and which methods they call") );
	
	fgEventsTargets->Add( lblEventsTargets, 0, wxALL, 5 );
	
	lstEvents = new wxListCtrl( scrEdit, wxID_ANY, wxDefaultPosition, wxSize( -1,170 ), wxLC_REPORT|wxLC_SINGLE_SEL );
	fgEventsTargets->Add( lstEvents, 0, wxALL|wxEXPAND, 3 );
	
	fgEvents->Add( fgEventsTargets, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbCreateEvent;
	sbCreateEvent = new wxStaticBoxSizer( new wxStaticBox( scrEdit, wxID_ANY, wxT("Create new event") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizerCE;
	fgSizerCE = new wxFlexGridSizer( 4, 3, 0, 0 );
	fgSizerCE->AddGrowableCol( 1 );
	fgSizerCE->SetFlexibleDirection( wxBOTH );
	fgSizerCE->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	lblOnEvent = new wxStaticText( scrEdit, wxID_ANY, wxT("On event"), wxDefaultPosition, wxDefaultSize, 0 );
	lblOnEvent->Wrap( -1 );
	lblOnEvent->SetToolTip( wxT("On which event it will happens?") );
	
	fgSizerCE->Add( lblOnEvent, 0, wxALL, 5 );
	
	cmbOnEvent = new wxComboBox( scrEdit, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizerCE->Add( cmbOnEvent, 0, wxALL|wxEXPAND, 2 );
	
	
	fgSizerCE->Add( 0, 0, 1, wxEXPAND, 5 );
	
	lblTarget = new wxStaticText( scrEdit, wxID_ANY, wxT("Target"), wxDefaultPosition, wxDefaultSize, 0 );
	lblTarget->Wrap( -1 );
	lblTarget->SetToolTip( wxT("Which target entity?") );
	
	fgSizerCE->Add( lblTarget, 0, wxALL, 5 );
	
	cmbTarget = new wxComboBox( scrEdit, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizerCE->Add( cmbTarget, 0, wxALL|wxEXPAND, 2 );
	
	btnPick = new wxToggleButton( scrEdit, wxID_ANY, wxT("P"), wxDefaultPosition, wxDefaultSize, 0 );
	btnPick->SetToolTip( wxT("Pick from scene") );
	btnPick->SetMinSize( wxSize( 20,20 ) );
	
	fgSizerCE->Add( btnPick, 0, wxTOP, 2 );
	
	lblMethod = new wxStaticText( scrEdit, wxID_ANY, wxT("Method"), wxDefaultPosition, wxDefaultSize, 0 );
	lblMethod->Wrap( -1 );
	lblMethod->SetToolTip( wxT("What call at target entity?") );
	
	fgSizerCE->Add( lblMethod, 0, wxALL, 5 );
	
	cmbMethod = new wxComboBox( scrEdit, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizerCE->Add( cmbMethod, 0, wxALL|wxEXPAND, 2 );
	
	
	fgSizerCE->Add( 0, 0, 1, wxEXPAND, 5 );
	
	lblParams = new wxStaticText( scrEdit, wxID_ANY, wxT("Params"), wxDefaultPosition, wxDefaultSize, 0 );
	lblParams->Wrap( -1 );
	lblParams->SetToolTip( wxT("Any method parameters?") );
	
	fgSizerCE->Add( lblParams, 0, wxALL, 5 );
	
	txtParams = new wxTextCtrl( scrEdit, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerCE->Add( txtParams, 0, wxALL|wxEXPAND, 2 );
	
	
	fgSizerCE->Add( 0, 0, 1, wxEXPAND, 5 );
	
	sbCreateEvent->Add( fgSizerCE, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizerCE2;
	fgSizerCE2 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizerCE2->AddGrowableCol( 0 );
	fgSizerCE2->SetFlexibleDirection( wxBOTH );
	fgSizerCE2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizerDelayOnlyOnce;
	bSizerDelayOnlyOnce = new wxBoxSizer( wxHORIZONTAL );
	
	lblDelay = new wxStaticText( scrEdit, wxID_ANY, wxT("Delay      "), wxDefaultPosition, wxDefaultSize, 0 );
	lblDelay->Wrap( -1 );
	lblDelay->SetToolTip( wxT("Delay in seconds") );
	
	bSizerDelayOnlyOnce->Add( lblDelay, 0, wxALL, 5 );
	
	txtDelay = new wxTextCtrl( scrEdit, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER );
	bSizerDelayOnlyOnce->Add( txtDelay, 0, wxALL, 2 );
	
	lblSecons = new wxStaticText( scrEdit, wxID_ANY, wxT(" s  "), wxDefaultPosition, wxDefaultSize, 0 );
	lblSecons->Wrap( -1 );
	bSizerDelayOnlyOnce->Add( lblSecons, 0, wxRIGHT|wxTOP, 5 );
	
	chkOnlyOnce = new wxCheckBox( scrEdit, wxID_ANY, wxT("Call only once"), wxDefaultPosition, wxDefaultSize, 0 );
	
	chkOnlyOnce->SetToolTip( wxT("Call this method only once") );
	
	bSizerDelayOnlyOnce->Add( chkOnlyOnce, 0, wxALIGN_CENTER_VERTICAL|wxALL, 4 );
	
	fgSizerCE2->Add( bSizerDelayOnlyOnce, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer339;
	bSizer339 = new wxBoxSizer( wxHORIZONTAL );
	
	btnAddEvent = new wxButton( scrEdit, wxID_ANY, wxT("Add!"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	btnAddEvent->SetToolTip( wxT("Add this event to selected entity") );
	
	bSizer339->Add( btnAddEvent, 0, wxLEFT|wxTOP, 5 );
	
	btnCopyEvent = new wxButton( scrEdit, wxID_ANY, wxT("Copy"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	btnCopyEvent->SetToolTip( wxT("Copy event") );
	
	bSizer339->Add( btnCopyEvent, 0, wxTOP, 5 );
	
	btnPasteEvent = new wxButton( scrEdit, wxID_ANY, wxT("Paste"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	btnPasteEvent->SetToolTip( wxT("Paste event") );
	
	bSizer339->Add( btnPasteEvent, 0, wxTOP, 5 );
	
	btnDelete = new wxButton( scrEdit, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	btnDelete->SetToolTip( wxT("Delete selected event") );
	
	bSizer339->Add( btnDelete, 0, wxTOP, 5 );
	
	fgSizerCE2->Add( bSizer339, 1, 0, 5 );
	
	wxBoxSizer* bSizer344;
	bSizer344 = new wxBoxSizer( wxVERTICAL );
	
	tabEventsHelpMethod = new wxStaticText( scrEdit, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,15 ), wxST_NO_AUTORESIZE );
	tabEventsHelpMethod->Wrap( 231 );
	tabEventsHelpMethod->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Arial") ) );
	tabEventsHelpMethod->SetForegroundColour( wxColour( 106, 0, 0 ) );
	tabEventsHelpMethod->SetMaxSize( wxSize( 50,11 ) );
	
	bSizer344->Add( tabEventsHelpMethod, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	tabEventsHelpMethodDesc = new wxStaticText( scrEdit, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,40 ), 0 );
	tabEventsHelpMethodDesc->Wrap( 200 );
	bSizer344->Add( tabEventsHelpMethodDesc, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	fgSizerCE2->Add( bSizer344, 1, wxEXPAND, 5 );
	
	sbCreateEvent->Add( fgSizerCE2, 1, wxEXPAND, 5 );
	
	fgEvents->Add( sbCreateEvent, 1, wxALL|wxEXPAND, 3 );
	
	sbEvents->Add( fgEvents, 1, wxEXPAND, 5 );
	
	fgEdit->Add( sbEvents, 0, wxEXPAND, 5 );
	
	scrEdit->SetSizer( fgEdit );
	scrEdit->Layout();
	fgEdit->Fit( scrEdit );
	nbMain->AddPage( scrEdit, wxEmptyString, false, wxBitmap( wxT("../data/editor/edit.ico"), wxBITMAP_TYPE_ANY ) );
	m_panel20 = new wxPanel( nbMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel20->SetToolTip( wxT("Groups") );
	
	wxFlexGridSizer* fgSizer97;
	fgSizer97 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer97->AddGrowableCol( 0 );
	fgSizer97->AddGrowableRow( 1 );
	fgSizer97->SetFlexibleDirection( wxBOTH );
	fgSizer97->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer334;
	bSizer334 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText331 = new wxStaticText( m_panel20, wxID_ANY, wxT("Group Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText331->Wrap( -1 );
	m_staticText331->SetToolTip( wxT("Select group or change text and press Create to create new group or press enter to rename selected") );
	
	bSizer334->Add( m_staticText331, 0, wxLEFT|wxTOP, 5 );
	
	tabGroupsSelected = new wxComboBox( m_panel20, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxTE_PROCESS_ENTER ); 
	tabGroupsSelected->SetToolTip( wxT("Select group or change text and press Create to create new group or press enter to rename selected") );
	
	bSizer334->Add( tabGroupsSelected, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer335;
	bSizer335 = new wxBoxSizer( wxHORIZONTAL );
	
	tabGroupsCreateNew = new wxButton( m_panel20, wxID_ANY, wxT("Create New"), wxDefaultPosition, wxDefaultSize, 0 );
	tabGroupsCreateNew->SetToolTip( wxT("Create new group") );
	
	bSizer335->Add( tabGroupsCreateNew, 0, wxBOTTOM|wxLEFT|wxTOP, 5 );
	
	tabGroupsDelete = new wxButton( m_panel20, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	tabGroupsDelete->SetToolTip( wxT("Delete selected group") );
	
	bSizer335->Add( tabGroupsDelete, 0, wxALL, 5 );
	
	bSizer334->Add( bSizer335, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer343;
	bSizer343 = new wxBoxSizer( wxHORIZONTAL );
	
	tabGroupsAddSelectionToGroup = new wxButton( m_panel20, wxID_ANY, wxT("Add selection to group"), wxDefaultPosition, wxDefaultSize, 0 );
	tabGroupsAddSelectionToGroup->SetToolTip( wxT("Add current selection to this group") );
	
	bSizer343->Add( tabGroupsAddSelectionToGroup, 0, wxBOTTOM|wxLEFT|wxTOP, 5 );
	
	tabGroupsSelectAll = new wxButton( m_panel20, wxID_ANY, wxT("Select all"), wxDefaultPosition, wxDefaultSize, 0 );
	tabGroupsSelectAll->SetToolTip( wxT("Make selection from all the entities in this group") );
	
	bSizer343->Add( tabGroupsSelectAll, 0, wxALL, 5 );
	
	bSizer334->Add( bSizer343, 1, wxEXPAND, 5 );
	
	fgSizer97->Add( bSizer334, 1, wxEXPAND, 5 );
	
	tabGroupsList = new wxListBox( m_panel20, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE ); 
	fgSizer97->Add( tabGroupsList, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer336;
	bSizer336 = new wxBoxSizer( wxVERTICAL );
	
	tabGroupsMakeSelectionFromHighlighted = new wxButton( m_panel20, wxID_ANY, wxT("Make selection from highlighted entities"), wxDefaultPosition, wxDefaultSize, 0 );
	tabGroupsMakeSelectionFromHighlighted->SetToolTip( wxT("Make selection from entities highlighted in list above") );
	
	bSizer336->Add( tabGroupsMakeSelectionFromHighlighted, 0, wxALL, 2 );
	
	tabGroupsRemoveHighlighted = new wxButton( m_panel20, wxID_ANY, wxT("Remove highlighted entities from group"), wxDefaultPosition, wxDefaultSize, 0 );
	tabGroupsRemoveHighlighted->SetToolTip( wxT("Remove highlighted entities in list above from selected group") );
	
	bSizer336->Add( tabGroupsRemoveHighlighted, 0, wxALL, 2 );
	
	tabGroupsClearList = new wxButton( m_panel20, wxID_ANY, wxT("Clear this group"), wxDefaultPosition, wxDefaultSize, 0 );
	tabGroupsClearList->SetToolTip( wxT("Remove all entities from group, but don't delete group") );
	
	bSizer336->Add( tabGroupsClearList, 0, wxALL, 2 );
	
	fgSizer97->Add( bSizer336, 1, wxALL|wxEXPAND, 3 );
	
	m_panel20->SetSizer( fgSizer97 );
	m_panel20->Layout();
	fgSizer97->Fit( m_panel20 );
	nbMain->AddPage( m_panel20, wxEmptyString, false, wxBitmap( wxT("../data/editor/groups.ico"), wxBITMAP_TYPE_ANY ) );
	m_scrolledWindow10 = new wxScrolledWindow( nbMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow10->SetScrollRate( 5, 5 );
	m_scrolledWindow10->SetToolTip( wxT("Display") );
	
	wxFlexGridSizer* fgSizer45;
	fgSizer45 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer45->AddGrowableCol( 0 );
	fgSizer45->SetFlexibleDirection( wxBOTH );
	fgSizer45->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer23;
	sbSizer23 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow10, wxID_ANY, wxT("Renderer options") ), wxVERTICAL );
	
	m_checkBox4 = new wxCheckBox( m_scrolledWindow10, wxID_ANY, wxT("Render models"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sbSizer23->Add( m_checkBox4, 0, wxALL, 5 );
	
	m_checkBox5 = new wxCheckBox( m_scrolledWindow10, wxID_ANY, wxT("Render entities"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sbSizer23->Add( m_checkBox5, 0, wxALL, 5 );
	
	m_checkBox6 = new wxCheckBox( m_scrolledWindow10, wxID_ANY, wxT("Render editor helpers"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sbSizer23->Add( m_checkBox6, 0, wxALL, 5 );
	
	fgSizer45->Add( sbSizer23, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer24;
	sbSizer24 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow10, wxID_ANY, wxT("Information and debugging") ), wxVERTICAL );
	
	m_checkBox7 = new wxCheckBox( m_scrolledWindow10, wxID_ANY, wxT("Show visibility data"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sbSizer24->Add( m_checkBox7, 0, wxALL, 5 );
	
	m_checkBox9 = new wxCheckBox( m_scrolledWindow10, wxID_ANY, wxT("Show physics information"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sbSizer24->Add( m_checkBox9, 0, wxALL, 5 );
	
	m_checkBox8 = new wxCheckBox( m_scrolledWindow10, wxID_ANY, wxT("Show frame stats"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sbSizer24->Add( m_checkBox8, 0, wxALL, 5 );
	
	fgSizer45->Add( sbSizer24, 1, wxEXPAND, 5 );
	
	m_scrolledWindow10->SetSizer( fgSizer45 );
	m_scrolledWindow10->Layout();
	fgSizer45->Fit( m_scrolledWindow10 );
	nbMain->AddPage( m_scrolledWindow10, wxEmptyString, false, wxBitmap( wxT("../data/editor/display.ico"), wxBITMAP_TYPE_ANY ) );
	
	fgMain->Add( nbMain, 1, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( fgMain );
	this->Layout();
	
	// Connect Events
	btnEntity->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnEntityToggled ), NULL, this );
	btnSolid->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnSolidToggled ), NULL, this );
	btnArea->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnAreaToggled ), NULL, this );
	btnTerrain->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnTerrainToggled ), NULL, this );
	lstEntities->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( panRollupGui::tabCreateEntitiesDblClick ), NULL, this );
	txtFilter->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( panRollupGui::txtFilterOnText ), NULL, this );
	txtFilter->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( panRollupGui::txtFilterOnEnter ), NULL, this );
	btnCreateEntity->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnCreateEntityClicked ), NULL, this );
	btnPick->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnPickClicked ), NULL, this );
	txtParams->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( panRollupGui::tabEventsParamLeaveFocus ), NULL, this );
	txtParams->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( panRollupGui::tabEventsParamEnterFocus ), NULL, this );
	txtParams->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( panRollupGui::btnAddEventClicked ), NULL, this );
	txtDelay->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( panRollupGui::btnAddEventClicked ), NULL, this );
	btnAddEvent->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnAddEventClicked ), NULL, this );
	btnCopyEvent->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnCopyEventClicked ), NULL, this );
	btnPasteEvent->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnPasteEventClicked ), NULL, this );
	btnDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabEventsDeletePressed ), NULL, this );
	tabGroupsSelected->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( panRollupGui::tabGroupsSelectedOnCombo ), NULL, this );
	tabGroupsSelected->Connect( wxEVT_KEY_UP, wxKeyEventHandler( panRollupGui::tabGroupsSelectedOnKeyUp ), NULL, this );
	tabGroupsSelected->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( panRollupGui::tabGroupsSelectedOnText ), NULL, this );
	tabGroupsSelected->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( panRollupGui::tabGroupsSelectedOnEnterCreate ), NULL, this );
	tabGroupsCreateNew->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsCreateNewPressed ), NULL, this );
	tabGroupsDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsDeletePressed ), NULL, this );
	tabGroupsAddSelectionToGroup->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsAddSelectionToGroupPressed ), NULL, this );
	tabGroupsSelectAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsSelectAllPressed ), NULL, this );
	tabGroupsList->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( panRollupGui::tabGroupsListDblClick ), NULL, this );
	tabGroupsMakeSelectionFromHighlighted->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsMakeSelectionFromHighlightedPressed ), NULL, this );
	tabGroupsRemoveHighlighted->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsRemoveHighlightedPressed ), NULL, this );
	tabGroupsClearList->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsClearListPressed ), NULL, this );
}

panRollupGui::~panRollupGui()
{
	// Disconnect Events
	btnEntity->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnEntityToggled ), NULL, this );
	btnSolid->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnSolidToggled ), NULL, this );
	btnArea->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnAreaToggled ), NULL, this );
	btnTerrain->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnTerrainToggled ), NULL, this );
	lstEntities->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( panRollupGui::tabCreateEntitiesDblClick ), NULL, this );
	txtFilter->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( panRollupGui::txtFilterOnText ), NULL, this );
	txtFilter->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( panRollupGui::txtFilterOnEnter ), NULL, this );
	btnCreateEntity->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnCreateEntityClicked ), NULL, this );
	btnPick->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnPickClicked ), NULL, this );
	txtParams->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( panRollupGui::tabEventsParamLeaveFocus ), NULL, this );
	txtParams->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( panRollupGui::tabEventsParamEnterFocus ), NULL, this );
	txtParams->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( panRollupGui::btnAddEventClicked ), NULL, this );
	txtDelay->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( panRollupGui::btnAddEventClicked ), NULL, this );
	btnAddEvent->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnAddEventClicked ), NULL, this );
	btnCopyEvent->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnCopyEventClicked ), NULL, this );
	btnPasteEvent->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::btnPasteEventClicked ), NULL, this );
	btnDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabEventsDeletePressed ), NULL, this );
	tabGroupsSelected->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( panRollupGui::tabGroupsSelectedOnCombo ), NULL, this );
	tabGroupsSelected->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( panRollupGui::tabGroupsSelectedOnKeyUp ), NULL, this );
	tabGroupsSelected->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( panRollupGui::tabGroupsSelectedOnText ), NULL, this );
	tabGroupsSelected->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( panRollupGui::tabGroupsSelectedOnEnterCreate ), NULL, this );
	tabGroupsCreateNew->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsCreateNewPressed ), NULL, this );
	tabGroupsDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsDeletePressed ), NULL, this );
	tabGroupsAddSelectionToGroup->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsAddSelectionToGroupPressed ), NULL, this );
	tabGroupsSelectAll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsSelectAllPressed ), NULL, this );
	tabGroupsList->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( panRollupGui::tabGroupsListDblClick ), NULL, this );
	tabGroupsMakeSelectionFromHighlighted->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsMakeSelectionFromHighlightedPressed ), NULL, this );
	tabGroupsRemoveHighlighted->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsRemoveHighlightedPressed ), NULL, this );
	tabGroupsClearList->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( panRollupGui::tabGroupsClearListPressed ), NULL, this );
}
