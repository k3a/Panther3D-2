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
#include "wx/textctrl.h"
#endif

#include "wx/listctrl.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGList, wxXmlResourceHandlerEx)

CGList::CGList()
: wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxLC_LIST);
    XRC_ADD_STYLE(wxLC_REPORT);
    XRC_ADD_STYLE(wxLC_ICON);
    XRC_ADD_STYLE(wxLC_SMALL_ICON);
    XRC_ADD_STYLE(wxLC_ALIGN_TOP);
    XRC_ADD_STYLE(wxLC_ALIGN_LEFT);
    XRC_ADD_STYLE(wxLC_AUTOARRANGE);
    XRC_ADD_STYLE(wxLC_USER_TEXT);
    XRC_ADD_STYLE(wxLC_EDIT_LABELS);
    XRC_ADD_STYLE(wxLC_NO_HEADER);
    XRC_ADD_STYLE(wxLC_SINGLE_SEL);
    XRC_ADD_STYLE(wxLC_SORT_ASCENDING);
    XRC_ADD_STYLE(wxLC_SORT_DESCENDING);
    XRC_ADD_STYLE(wxLC_VIRTUAL);
    XRC_ADD_STYLE(wxLC_HRULES);
    XRC_ADD_STYLE(wxLC_VRULES);
    XRC_ADD_STYLE(wxLC_NO_SORT_HEADER);
    AddWindowStyles();
}

wxObject *CGList::DoCreateResource()
{
    XRC_MAKE_INSTANCE(list, wxListCtrl)

    list->Create(m_parentAsWindow,
                 GetID(),
                 GetPosition(), GetSize(),
                 GetStyle(),
                 wxDefaultValidator,
                 GetName());

    // FIXME: add columns definition

    SetupWindow(list);

    return list;
}

bool CGList::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxListCtrl"));
}

}; // namespace
