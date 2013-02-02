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
#include "wx/sizer.h"
#endif

#include "wx/notebook.h"
#include "wx/imaglist.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGNoteBook, wxXmlResourceHandlerEx)

CGNoteBook::CGNoteBook()
                     :wxXmlResourceHandlerEx(),
                      m_isInside(false),
                      m_notebook(NULL)
{
    XRC_ADD_STYLE(wxBK_DEFAULT);
    XRC_ADD_STYLE(wxBK_LEFT);
    XRC_ADD_STYLE(wxBK_RIGHT);
    XRC_ADD_STYLE(wxBK_TOP);
    XRC_ADD_STYLE(wxBK_BOTTOM);

    XRC_ADD_STYLE(wxNB_DEFAULT);
    XRC_ADD_STYLE(wxNB_LEFT);
    XRC_ADD_STYLE(wxNB_RIGHT);
    XRC_ADD_STYLE(wxNB_TOP);
    XRC_ADD_STYLE(wxNB_BOTTOM);

    XRC_ADD_STYLE(wxNB_FIXEDWIDTH);
    XRC_ADD_STYLE(wxNB_MULTILINE);
    XRC_ADD_STYLE(wxNB_NOPAGETHEME);

    AddWindowStyles();
}

wxObject *CGNoteBook::DoCreateResource()
{
    if (m_class == wxT("notebookpage"))
    {
        wxXmlNode *n = GetParamNode(wxT("object"));

        if ( !n )
            n = GetParamNode(wxT("object_ref"));

        if (n)
        {
            bool old_ins = m_isInside;
            m_isInside = false;
            wxObject *item = CreateResFromNode(n, m_notebook, NULL);
            m_isInside = old_ins;
            wxWindow *wnd = wxDynamicCast(item, wxWindow);

            if (wnd)
            {
                m_notebook->AddPage(wnd, GetText(wxT("label")),
                                         GetBool(wxT("selected")));
                if ( HasParam(wxT("bitmap")) )
                {
                    wxBitmap bmp = GetBitmap(wxT("bitmap"), wxART_OTHER);
                    wxImageList *imgList = m_notebook->GetImageList();
                    if ( imgList == NULL )
                    {
                        imgList = new wxImageList( bmp.GetWidth(), bmp.GetHeight() );
                        m_notebook->AssignImageList( imgList );
                    }
                    int imgIndex = imgList->Add(bmp);
                    m_notebook->SetPageImage(m_notebook->GetPageCount()-1, imgIndex );
                }
            }
            else
                wxLogError(wxT("Error in resource."));
            return wnd;
        }
        else
        {
            wxLogError(wxT("Error in resource: no control within notebook's <page> tag."));
            return NULL;
        }
    }

    else
    {
        XRC_MAKE_INSTANCE(nb, wxNotebook)

        nb->Create(m_parentAsWindow,
                   GetID(),
                   GetPosition(), GetSize(),
                   GetStyle(wxT("style")),
                   GetName());

        SetupWindow(nb);

        wxNotebook *old_par = m_notebook;
        m_notebook = nb;
        bool old_ins = m_isInside;
        m_isInside = true;
        CreateChildren(m_notebook, true/*only this handler*/);
        m_isInside = old_ins;
        m_notebook = old_par;

        return nb;
    }
}

bool CGNoteBook::CanHandle(wxXmlNode *node)
{
    return ((!m_isInside && IsOfClass(node, wxT("wxNotebook"))) ||
            (m_isInside && IsOfClass(node, wxT("notebookpage"))));
}

}; // namespace
