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

#ifndef WX_PRECOMP
#include "wx/log.h"
#endif

#include "wx/collpane.h"
#include "CGUI.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGCollapsiblePane, wxXmlResourceHandlerEx)

CGCollapsiblePane::CGCollapsiblePane() 
: wxXmlResourceHandlerEx(), m_isInside(false)
{
    XRC_ADD_STYLE(wxCP_NO_TLW_RESIZE);
    XRC_ADD_STYLE(wxCP_DEFAULT_STYLE);
    AddWindowStyles();
}

wxObject *CGCollapsiblePane::DoCreateResource()
{
    if (m_class == wxT("panewindow"))   // read the XRC for the pane window
    {
        wxXmlNode *n = GetParamNode(wxT("object"));

        if ( !n )
            n = GetParamNode(wxT("object_ref"));

        if (n)
        {
            bool old_ins = m_isInside;
            m_isInside = false;
            wxObject *item = CreateResFromNode(n, m_collpane->GetPane(), NULL);
            m_isInside = old_ins;

            return item;
        }
        else
        {
            wxLogError(wxT("Error in resource: no control within collapsible pane's <panewindow> tag."));
            return NULL;
        }
    }
    else
    {
        XRC_MAKE_INSTANCE(ctrl, wxCollapsiblePane)

        wxString label = GetParamValue(wxT("label"));
        if (label.empty())
        {
            wxLogError(wxT("Error in resource: empty label for wxCollapsiblePane"));
            return NULL;
        }

        ctrl->Create(m_parentAsWindow,
                    GetID(),
                    label,
                    GetPosition(), GetSize(),
                    GetStyle(_T("style"), wxCP_DEFAULT_STYLE),
                    wxDefaultValidator,
                    GetName());

        ctrl->Collapse(GetBool(_T("collapsed")));
        SetupWindow(ctrl);

        wxCollapsiblePane *old_par = m_collpane;
        m_collpane = ctrl;
        bool old_ins = m_isInside;
        m_isInside = true;
        CreateChildren(m_collpane, true/*only this handler*/);
        m_isInside = old_ins;
        m_collpane = old_par;

        return ctrl;
    }
}

bool CGCollapsiblePane::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxCollapsiblePane")) ||
            (m_isInside && IsOfClass(node, wxT("panewindow")));
}

}; // namespace
