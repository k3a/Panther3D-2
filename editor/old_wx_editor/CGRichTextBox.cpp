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

#include "wx/richtext/richtextctrl.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGRichTextBox, wxXmlResourceHandlerEx)

CGRichTextBox::CGRichTextBox() : wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxTE_PROCESS_ENTER);
    XRC_ADD_STYLE(wxTE_PROCESS_TAB);
    XRC_ADD_STYLE(wxTE_MULTILINE);
    XRC_ADD_STYLE(wxTE_READONLY);
    XRC_ADD_STYLE(wxTE_AUTO_URL);

    AddWindowStyles();
}

wxObject *CGRichTextBox::DoCreateResource()
{
    XRC_MAKE_INSTANCE(text, wxRichTextCtrl)

    text->Create(m_parentAsWindow,
                 GetID(),
                 GetText(wxT("value")),
                 GetPosition(), GetSize(),
                 GetStyle(),
                 wxDefaultValidator,
                 GetName());

    SetupWindow(text);

    if (HasParam(wxT("maxlength")))
        text->SetMaxLength(GetLong(wxT("maxlength")));

    return text;
}

bool CGRichTextBox::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxRichTextCtrl"));
}

}; // namespace

