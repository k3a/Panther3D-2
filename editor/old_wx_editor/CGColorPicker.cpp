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
#include "wx/clrpicker.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGColorPicker, wxXmlResourceHandlerEx)

CGColorPicker::CGColorPicker() : wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxCLRP_USE_TEXTCTRL);
    XRC_ADD_STYLE(wxCLRP_SHOW_LABEL);
    XRC_ADD_STYLE(wxCLRP_DEFAULT_STYLE);
    AddWindowStyles();
}

wxObject *CGColorPicker::DoCreateResource()
{
   XRC_MAKE_INSTANCE(picker, wxColourPickerCtrl)

   picker->Create(m_parentAsWindow,
                  GetID(),
                  GetColour(wxT("value"), *wxBLACK),
                  GetPosition(), GetSize(),
                  GetStyle(_T("style"), wxCLRP_DEFAULT_STYLE),
                  wxDefaultValidator,
                  GetName());

    SetupWindow(picker);

    return picker;
}

bool CGColorPicker::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxColourPickerCtrl"));
}

}; // namespace
