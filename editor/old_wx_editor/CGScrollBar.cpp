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
#include "wx/scrolbar.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGScrollBar, wxXmlResourceHandlerEx)

CGScrollBar::CGScrollBar()
: wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxSB_HORIZONTAL);
    XRC_ADD_STYLE(wxSB_VERTICAL);
    AddWindowStyles();
}

wxObject *CGScrollBar::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxScrollBar)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetPosition(), GetSize(),
                    GetStyle(),
                    wxDefaultValidator,
                    GetName());

    control->SetScrollbar(GetLong( wxT("value"), 0),
                          GetLong( wxT("thumbsize"),1),
                          GetLong( wxT("range"), 10),
                          GetLong( wxT("pagesize"),1));

    SetupWindow(control);
    CreateChildren(control);

    return control;
}

bool CGScrollBar::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxScrollBar"));
}

}; // namespace
