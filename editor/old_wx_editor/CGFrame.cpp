/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif



#include "CGUI.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/log.h"
    #include "wx/frame.h"
    #include "wx/dialog.h" // to get wxDEFAULT_DIALOG_STYLE
#endif

namespace P3D {

class CGFrameW : public IGBase, public wxEvtHandler {
public:
	CGFrameW(IGWindow* pwin, wxFrame *pobj){ obj = pobj; win = pwin; };

	void RecurDelete(wxWindow* bw)
	{
		wxWindowList lst = bw->GetChildren();
		for (unsigned int i=0; i<lst.size(); i++)
			RecurDelete(lst[i]);

		IGBase* b = (IGBase*)bw->GetClientData();
		if (b) delete b;
		bw->Destroy();
	}

	void EventClose(wxCloseEvent& event)
	{
		//event.Veto();
		RecurDelete(obj);
	}

	wxFrame *obj;
	IGWindow* win;
};

IMPLEMENT_DYNAMIC_CLASS(CGFrame, wxXmlResourceHandlerEx)

CGFrame::CGFrame() : wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxSTAY_ON_TOP);
    XRC_ADD_STYLE(wxCAPTION);
    XRC_ADD_STYLE(wxDEFAULT_DIALOG_STYLE);
    XRC_ADD_STYLE(wxDEFAULT_FRAME_STYLE);
#if WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxTHICK_FRAME);
#endif // WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxSYSTEM_MENU);
    XRC_ADD_STYLE(wxRESIZE_BORDER);
#if WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxRESIZE_BOX);
    XRC_ADD_STYLE(wxCLOSE_BOX);
#endif // WXWIN_COMPATIBILITY_2_6

    XRC_ADD_STYLE(wxFRAME_NO_TASKBAR);
    XRC_ADD_STYLE(wxFRAME_SHAPED);
    XRC_ADD_STYLE(wxFRAME_TOOL_WINDOW);
    XRC_ADD_STYLE(wxFRAME_FLOAT_ON_PARENT);
    XRC_ADD_STYLE(wxMAXIMIZE_BOX);
    XRC_ADD_STYLE(wxMINIMIZE_BOX);
    XRC_ADD_STYLE(wxSTAY_ON_TOP);

#if WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxNO_3D);
#endif // WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxWS_EX_VALIDATE_RECURSIVELY);
    XRC_ADD_STYLE(wxFRAME_EX_METAL);
    XRC_ADD_STYLE(wxFRAME_EX_CONTEXTHELP);

    AddWindowStyles();
}

wxObject *CGFrame::DoCreateResource()
{
    XRC_MAKE_INSTANCE(frame, wxFrame);

    frame->Create(m_parentAsWindow,
                  GetID(),
                  GetText(wxT("title")),
                  wxDefaultPosition, wxDefaultSize,
                  GetStyle(wxT("style"), wxDEFAULT_FRAME_STYLE),
                  GetName());

    if (HasParam(wxT("size")))
        frame->SetClientSize(GetSize(wxT("size"), frame));
    if (HasParam(wxT("pos")))
        frame->Move(GetPosition());
    if (HasParam(wxT("icon")))
        frame->SetIcon(GetIcon(wxT("icon"), wxART_FRAME_ICON));

    SetupWindow(frame);

    CreateChildren(frame);

    if (GetBool(wxT("centered"), false))
        frame->Centre();

	//TODO: GCOMPONENT_COMMON_INIT(Frame, frame); misto toho pod tim
	CGFrameW* wrp = new CGFrameW(GetImplClass(), frame);
	frame->SetClientData(wrp);

	// connect event handlers
	frame->Connect( frame->GetId(), wxEVT_CLOSE_WINDOW, wxCloseEventHandler(CGFrameW::EventClose) , NULL, (wxEvtHandler *)wrp );

    return frame;
}

bool CGFrame::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxFrame"));
}


}; // namespace