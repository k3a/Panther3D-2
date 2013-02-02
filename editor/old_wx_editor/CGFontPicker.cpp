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
#include "wx/fontpicker.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGFontPicker, wxXmlResourceHandlerEx)

CGFontPicker::CGFontPicker() : wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxFNTP_USE_TEXTCTRL);
    XRC_ADD_STYLE(wxFNTP_FONTDESC_AS_LABEL);
    XRC_ADD_STYLE(wxFNTP_USEFONT_FOR_LABEL);
    XRC_ADD_STYLE(wxFNTP_DEFAULT_STYLE);
    AddWindowStyles();
}

wxObject *CGFontPicker::DoCreateResource()
{
   XRC_MAKE_INSTANCE(picker, wxFontPickerCtrl)

    wxFont f = *wxNORMAL_FONT;
    if (HasParam(wxT("value")))
        f = GetFont(wxT("value"));

   picker->Create(m_parentAsWindow,
                  GetID(),
                  f,
                  GetPosition(), GetSize(),
                  GetStyle(_T("style"), wxFNTP_DEFAULT_STYLE),
                  wxDefaultValidator,
                  GetName());

    SetupWindow(picker);

    return picker;
}

bool CGFontPicker::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxFontPickerCtrl"));
}

}; // namespace
