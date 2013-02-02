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

#include "wx/html/htmlwin.h"
#include "wx/filesys.h"
namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGHtml, wxXmlResourceHandlerEx)

CGHtml::CGHtml()
: wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxHW_SCROLLBAR_NEVER);
    XRC_ADD_STYLE(wxHW_SCROLLBAR_AUTO);
    XRC_ADD_STYLE(wxHW_NO_SELECTION);
    AddWindowStyles();
}

wxObject *CGHtml::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxHtmlWindow)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetPosition(), GetSize(),
                    GetStyle(wxT("style"), wxHW_SCROLLBAR_AUTO),
                    GetName());

    if (HasParam(wxT("borders")))
    {
        control->SetBorders(GetDimension(wxT("borders")));
    }

    if (HasParam(wxT("url")))
    {
        wxString url = GetParamValue(wxT("url"));
        wxFileSystem& fsys = GetCurFileSystem();

        wxFSFile *f = fsys.OpenFile(url);
        if (f)
        {
            control->LoadPage(f->GetLocation());
            delete f;
        }
        else
            control->LoadPage(url);
    }

    else if (HasParam(wxT("htmlcode")))
    {
        control->SetPage(GetText(wxT("htmlcode")));
    }

    SetupWindow(control);

    return control;
}

bool CGHtml::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxHtmlWindow"));
}

}; // namespace
