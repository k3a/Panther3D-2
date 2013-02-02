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
    #include "wx/bmpbuttn.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGBmpButton, wxXmlResourceHandlerEx)

CGBmpButton::CGBmpButton()
: wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxBU_AUTODRAW);
    XRC_ADD_STYLE(wxBU_LEFT);
    XRC_ADD_STYLE(wxBU_RIGHT);
    XRC_ADD_STYLE(wxBU_TOP);
    XRC_ADD_STYLE(wxBU_BOTTOM);
    XRC_ADD_STYLE(wxBU_EXACTFIT);
    AddWindowStyles();
}

wxObject *CGBmpButton::DoCreateResource()
{
    XRC_MAKE_INSTANCE(button, wxBitmapButton)

    button->Create(m_parentAsWindow,
                   GetID(),
                   GetBitmap(wxT("bitmap"), wxART_BUTTON),
                   GetPosition(), GetSize(),
                   GetStyle(wxT("style"), wxBU_AUTODRAW),
                   wxDefaultValidator,
                   GetName());
    if (GetBool(wxT("default"), 0))
        button->SetDefault();
    SetupWindow(button);

    if (GetParamNode(wxT("selected")))
        button->SetBitmapSelected(GetBitmap(wxT("selected")));
    if (GetParamNode(wxT("focus")))
        button->SetBitmapFocus(GetBitmap(wxT("focus")));
    if (GetParamNode(wxT("disabled")))
        button->SetBitmapDisabled(GetBitmap(wxT("disabled")));
    if (GetParamNode(wxT("hover")))
        button->SetBitmapHover(GetBitmap(wxT("hover")));

    return button;
}

bool CGBmpButton::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxBitmapButton"));
}

}; // namespace
