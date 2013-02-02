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
    #include "wx/frame.h"
    #include "wx/scrolwin.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGScrolledWindow, wxXmlResourceHandlerEx)

CGScrolledWindow::CGScrolledWindow()
: wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxHSCROLL);
    XRC_ADD_STYLE(wxVSCROLL);

    // wxPanel styles
#if WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxNO_3D);
#endif // WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxWS_EX_VALIDATE_RECURSIVELY);

    AddWindowStyles();
}

wxObject *CGScrolledWindow::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxScrolledWindow)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetPosition(), GetSize(),
                    GetStyle(wxT("style"), wxHSCROLL | wxVSCROLL),
                    GetName());

    SetupWindow(control);
    CreateChildren(control);

    if ( HasParam(wxT("scrollrate")) )
    {
        wxSize rate = GetSize(wxT("scrollrate"));
        control->SetScrollRate(rate.x, rate.y);
    }
	else
	{
		control->SetScrollRate(5, 5); // KEX: bug in wxFormBuilder - default will be 5,5
	}

    return control;
}

bool CGScrolledWindow::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxScrolledWindow"));
}


}; // namespace