/* Panther3D Engine � 2004-2008 Reversity Studios (www.reversity.org)
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
#include "wx/checkbox.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGCheckBox, wxXmlResourceHandlerEx)

CGCheckBox::CGCheckBox()
: wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxCHK_2STATE);
    XRC_ADD_STYLE(wxCHK_3STATE);
    XRC_ADD_STYLE(wxCHK_ALLOW_3RD_STATE_FOR_USER);
    XRC_ADD_STYLE(wxALIGN_RIGHT);
    AddWindowStyles();
}

wxObject *CGCheckBox::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxCheckBox)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetText(wxT("label")),
                    GetPosition(), GetSize(),
                    GetStyle(),
                    wxDefaultValidator,
                    GetName());

    control->SetValue(GetBool( wxT("checked")));
    SetupWindow(control);

    return control;
}

bool CGCheckBox::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxCheckBox"));
}

}; // namespace
