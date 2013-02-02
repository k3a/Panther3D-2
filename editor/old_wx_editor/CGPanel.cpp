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
    #include "wx/panel.h"
    #include "wx/frame.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGPanel, wxXmlResourceHandlerEx)

CGPanel::CGPanel() : wxXmlResourceHandlerEx()
{
#if WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxNO_3D);
#endif // WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxWS_EX_VALIDATE_RECURSIVELY);

    AddWindowStyles();
}

wxObject *CGPanel::DoCreateResource()
{
    XRC_MAKE_INSTANCE(panel, wxPanel)

    panel->Create(m_parentAsWindow,
                  GetID(),
                  GetPosition(), GetSize(),
                  GetStyle(wxT("style"), wxTAB_TRAVERSAL),
                  GetName());


    SetupWindow(panel);
    CreateChildren(panel);

    return panel;
}

bool CGPanel::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxPanel"));
}


}; // namespace