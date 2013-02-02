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
#include "wx/combobox.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGCombo, wxXmlResourceHandlerEx)

CGCombo::CGCombo()
                     :wxXmlResourceHandlerEx()
                     ,m_insideBox(false)
{
    XRC_ADD_STYLE(wxCB_SIMPLE);
    XRC_ADD_STYLE(wxCB_SORT);
    XRC_ADD_STYLE(wxCB_READONLY);
    XRC_ADD_STYLE(wxCB_DROPDOWN);
    AddWindowStyles();
}

wxObject *CGCombo::DoCreateResource()
{
    if( m_class == wxT("wxComboBox"))
    {
        // find the selection
        long selection = GetLong( wxT("selection"), -1 );

        // need to build the list of strings from children
        m_insideBox = true;
        CreateChildrenPrivately(NULL, GetParamNode(wxT("content")));

        XRC_MAKE_INSTANCE(control, wxComboBox)

        control->Create(m_parentAsWindow,
                        GetID(),
                        GetText(wxT("value")),
                        GetPosition(), GetSize(),
                        strList,
                        GetStyle(),
                        wxDefaultValidator,
                        GetName());

        if (selection != -1)
            control->SetSelection(selection);

        SetupWindow(control);

        strList.Clear();    // dump the strings

        return control;
    }
    else
    {
        // on the inside now.
        // handle <item>Label</item>

        // add to the list
        wxString str = GetNodeContent(m_node);
        if (m_resource->GetFlags() & wxXRC_USE_LOCALE)
            str = wxGetTranslation(str, m_resource->GetDomain());
        strList.Add(str);

        return NULL;
    }
}

bool CGCombo::CanHandle(wxXmlNode *node)
{
    return (IsOfClass(node, wxT("wxComboBox")) ||
           (m_insideBox && node->GetName() == wxT("item")));
}

}; // namespace
