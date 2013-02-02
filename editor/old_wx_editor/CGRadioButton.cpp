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
#include "wx/radiobut.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGRadioButton, wxXmlResourceHandlerEx)

CGRadioButton::CGRadioButton()
: wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxRB_GROUP);
    XRC_ADD_STYLE(wxRB_SINGLE);
    AddWindowStyles();
}

wxObject *CGRadioButton::DoCreateResource()
{
    /* BOBM - implementation note.
     * once the wxBitmapRadioButton is implemented.
     * look for a bitmap property. If not null,
     * make it a wxBitmapRadioButton instead of the
     * normal radio button.
     */

    XRC_MAKE_INSTANCE(control, wxRadioButton)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetText(wxT("label")),
                    GetPosition(), GetSize(),
                    GetStyle(),
                    wxDefaultValidator,
                    GetName());

    control->SetValue(GetBool(wxT("value"), 0));
    SetupWindow(control);

    return control;
}

bool CGRadioButton::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxRadioButton"));
}

}; // namespace
