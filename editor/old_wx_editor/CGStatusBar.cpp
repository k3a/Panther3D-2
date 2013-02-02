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
#include "wx/string.h"
#include "wx/log.h"
#include "wx/frame.h"
#include "wx/statusbr.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGStatusBar, wxXmlResourceHandlerEx)

CGStatusBar::CGStatusBar()
                      :wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxST_SIZEGRIP);
    AddWindowStyles();
}

wxObject *CGStatusBar::DoCreateResource()
{
    XRC_MAKE_INSTANCE(statbar, wxStatusBar)

    statbar->Create(m_parentAsWindow,
                    GetID(),
                    GetStyle(),
                    GetName());

    int fields = GetLong(wxT("fields"), 1);
    wxString widths = GetParamValue(wxT("widths"));
    wxString styles = GetParamValue(wxT("styles"));

    if (fields > 1 && !widths.IsEmpty())
    {
        int *width = new int[fields];

        for (int i = 0; i < fields; ++i)
        {
            width[i] = wxAtoi(widths.BeforeFirst(wxT(',')));
            if(widths.Find(wxT(',')))
                widths.Remove(0, widths.Find(wxT(',')) + 1);
        }
        statbar->SetFieldsCount(fields, width);
        delete[] width;
    }
    else
        statbar->SetFieldsCount(fields);

    if (!styles.empty())
    {
        int *style = new int[fields];
        for (int i = 0; i < fields; ++i)
        {
            style[i] = wxSB_NORMAL;

            wxString first = styles.BeforeFirst(wxT(','));
            if (first == wxT("wxSB_NORMAL"))
                style[i] = wxSB_NORMAL;
            else if (first == wxT("wxSB_FLAT"))
                style[i] = wxSB_FLAT;
            else if (first == wxT("wxSB_RAISED"))
                style[i] = wxSB_RAISED;
            else if (!first.empty())
                wxLogError(wxT("Error in resource, unknown statusbar field style: ") + first);

            if(styles.Find(wxT(',')))
                styles.Remove(0, styles.Find(wxT(',')) + 1);
        }
        statbar->SetStatusStyles(fields, style);
        delete [] style;
    }

    CreateChildren(statbar);

    if (m_parentAsWindow)
    {
        wxFrame *parentFrame = wxDynamicCast(m_parent, wxFrame);
        if (parentFrame)
            parentFrame->SetStatusBar(statbar);
    }

    return statbar;
}

bool CGStatusBar::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxStatusBar"));
}

}; // namespace
