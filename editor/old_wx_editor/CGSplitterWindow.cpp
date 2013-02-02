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
#include "wx/log.h"
#endif

#include "wx/splitter.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGSplitterWindow, wxXmlResourceHandlerEx)

CGSplitterWindow::CGSplitterWindow() : wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxSP_3D);
    XRC_ADD_STYLE(wxSP_3DSASH);
    XRC_ADD_STYLE(wxSP_3DBORDER);
#if WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxSP_FULLSASH);
#endif // WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxSP_BORDER);
    XRC_ADD_STYLE(wxSP_NOBORDER);
    XRC_ADD_STYLE(wxSP_PERMIT_UNSPLIT);
    XRC_ADD_STYLE(wxSP_LIVE_UPDATE);
    XRC_ADD_STYLE(wxSP_NO_XP_THEME);
    AddWindowStyles();
}

wxObject *CGSplitterWindow::DoCreateResource()
{
    XRC_MAKE_INSTANCE(splitter, wxSplitterWindow);

    splitter->Create(m_parentAsWindow,
                     GetID(),
                     GetPosition(), GetSize(),
                     GetStyle(wxT("style"), wxSP_3D),
                     GetName());

    SetupWindow(splitter);

    long sashpos = GetLong(wxT("sashpos"), 0);
    long minpanesize = GetLong(wxT("minsize"), -1);
    float gravity = GetFloat(wxT("gravity"), 0.0);
    if (minpanesize != -1)
        splitter->SetMinimumPaneSize(minpanesize);
    if (gravity != 0.0)
        splitter->SetSashGravity(gravity);

    wxWindow *win1 = NULL, *win2 = NULL;
    wxXmlNode *n = m_node->GetChildren();
    while (n)
    {
        if ((n->GetType() == wxXML_ELEMENT_NODE) &&
            (n->GetName() == wxT("object") ||
             n->GetName() == wxT("object_ref")))
        {
            wxObject *created = CreateResFromNode(n, splitter, NULL);
            wxWindow *win = wxDynamicCast(created, wxWindow);
            if (win1 == NULL)
            {
                win1 = win;
            }
            else
            {
                win2 = win;
                break;
            }
        }
        n = n->GetNext();
    }

    if (win1 == NULL)
        wxLogError(wxT("wxSplitterWindow node must contain at least one window."));

    bool horizontal = (GetParamValue(wxT("orientation")) != wxT("vertical"));
    if (win1 && win2)
    {
        if (horizontal)
            splitter->SplitHorizontally(win1, win2, sashpos);
        else
            splitter->SplitVertically(win1, win2, sashpos);
    }
    else
    {
        splitter->Initialize(win1);
    }

    return splitter;
}

bool CGSplitterWindow::CanHandle(wxXmlNode *node)
{
     return IsOfClass(node, wxT("wxSplitterWindow"));
}

}; // namespace
