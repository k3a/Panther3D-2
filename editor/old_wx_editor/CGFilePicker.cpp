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

IMPLEMENT_DYNAMIC_CLASS(CGFilePicker, wxXmlResourceHandlerEx)

CGFilePicker::CGFilePicker() : wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxFLP_OPEN);
    XRC_ADD_STYLE(wxFLP_SAVE);
    XRC_ADD_STYLE(wxFLP_OVERWRITE_PROMPT);
    XRC_ADD_STYLE(wxFLP_FILE_MUST_EXIST);
    XRC_ADD_STYLE(wxFLP_CHANGE_DIR);
    XRC_ADD_STYLE(wxFLP_DEFAULT_STYLE);
    XRC_ADD_STYLE(wxFLP_USE_TEXTCTRL);
    AddWindowStyles();
}

wxObject *CGFilePicker::DoCreateResource()
{
    XRC_MAKE_INSTANCE(picker, wxFilePickerCtrl)

    picker->Create(m_parentAsWindow,
                  GetID(),
                  GetParamValue(wxT("value")),
                  GetParamValue(wxT("message")),
                  GetParamValue(wxT("wildcard")),
                  GetPosition(), GetSize(),
                  GetStyle(_T("style"), wxFLP_DEFAULT_STYLE),
                  wxDefaultValidator,
                  GetName());

    SetupWindow(picker);
    return picker;
}

bool CGFilePicker::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxFilePickerCtrl"));
}

}; // namespace
