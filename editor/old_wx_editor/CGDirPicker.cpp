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
#include "wx/filepicker.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGDirPicker, wxXmlResourceHandlerEx)

CGDirPicker::CGDirPicker() : wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxDIRP_USE_TEXTCTRL);
    XRC_ADD_STYLE(wxDIRP_DIR_MUST_EXIST);
    XRC_ADD_STYLE(wxDIRP_CHANGE_DIR);
    XRC_ADD_STYLE(wxDIRP_DEFAULT_STYLE);
    AddWindowStyles();
}

wxObject *CGDirPicker::DoCreateResource()
{
   XRC_MAKE_INSTANCE(picker, wxDirPickerCtrl)

   picker->Create(m_parentAsWindow,
                  GetID(),
                  GetParamValue(wxT("value")),
                  GetParamValue(wxT("message")),
                  GetPosition(), GetSize(),
                  GetStyle(_T("style"), wxDIRP_DEFAULT_STYLE),
                  wxDefaultValidator,
                  GetName());

    SetupWindow(picker);

    return picker;
}

bool CGDirPicker::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxDirPickerCtrl"));
}

}; // namespace
