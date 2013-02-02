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
#include "wx/intl.h"
#include "wx/log.h"
#include "wx/checklst.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGCheckList, wxXmlResourceHandlerEx)

CGCheckList::CGCheckList()
: wxXmlResourceHandlerEx(), m_insideBox(false)
{
    // wxListBox styles:
    XRC_ADD_STYLE(wxLB_SINGLE);
    XRC_ADD_STYLE(wxLB_MULTIPLE);
    XRC_ADD_STYLE(wxLB_EXTENDED);
    XRC_ADD_STYLE(wxLB_HSCROLL);
    XRC_ADD_STYLE(wxLB_ALWAYS_SB);
    XRC_ADD_STYLE(wxLB_NEEDED_SB);
    XRC_ADD_STYLE(wxLB_SORT);

    AddWindowStyles();
}

wxObject *CGCheckList::DoCreateResource()
{
    if (m_class == wxT("wxCheckListBox")
        || m_class == wxT("wxCheckList")
       )
    {
        if (m_class == wxT("wxCheckList"))
            wxLogDebug(wxT("'wxCheckList' name is deprecated, use 'wxCheckListBox' instead."));
        // need to build the list of strings from children
        m_insideBox = true;
        CreateChildrenPrivately(NULL, GetParamNode(wxT("content")));

        XRC_MAKE_INSTANCE(control, wxCheckListBox)

        control->Create(m_parentAsWindow,
                        GetID(),
                        GetPosition(), GetSize(),
                        strList,
                        GetStyle(),
                        wxDefaultValidator,
                        GetName());

        // step through children myself (again.)
        wxXmlNode *n = GetParamNode(wxT("content"));
        if (n)
            n = n->GetChildren();
        int i = 0;
        while (n)
        {
            if (n->GetType() != wxXML_ELEMENT_NODE ||
                n->GetName() != wxT("item"))
               { n = n->GetNext(); continue; }

            // checking boolean is a bit ugly here (see GetBool() )
            wxString v = n->GetAttribute(wxT("checked"), wxEmptyString);
            v.MakeLower();
			if (!v.IsEmpty() && (v == wxT("1")))
                control->Check( i, true );

            i++;
            n = n->GetNext();
        }

        SetupWindow(control);

        strList.Clear();    // dump the strings

        return control;
    }
    else
    {
        // on the inside now.
        // handle <item checked="boolean">Label</item>

        // add to the list
        wxString str = GetNodeContent(m_node);
        if (m_resource->GetFlags() & wxXRC_USE_LOCALE)
            str = wxGetTranslation(str, m_resource->GetDomain());
        strList.Add(str);
        return NULL;
    }
}

bool CGCheckList::CanHandle(wxXmlNode *node)
{
    return (IsOfClass(node, wxT("wxCheckListBox")) ||
            IsOfClass(node, wxT("wxCheckList")) ||
           (m_insideBox && node->GetName() == wxT("item")));
}

}; // namespace
