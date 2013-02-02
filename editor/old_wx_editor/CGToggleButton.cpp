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
#include "wx/tglbtn.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGToggleButton, wxXmlResourceHandlerEx)

CGToggleButton::CGToggleButton()
: wxXmlResourceHandlerEx()
{
    AddWindowStyles();
}

wxObject *CGToggleButton::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxToggleButton)

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

bool CGToggleButton::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxToggleButton"));
}

}; // namespace
