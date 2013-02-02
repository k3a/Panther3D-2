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

#include "wx/listbook.h"
#include "wx/imaglist.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGListBook, wxXmlResourceHandlerEx)

CGListBook::CGListBook()
                     :wxXmlResourceHandlerEx(),
                      m_isInside(false),
                      m_listbook(NULL)
{
    XRC_ADD_STYLE(wxBK_DEFAULT);
    XRC_ADD_STYLE(wxBK_LEFT);
    XRC_ADD_STYLE(wxBK_RIGHT);
    XRC_ADD_STYLE(wxBK_TOP);
    XRC_ADD_STYLE(wxBK_BOTTOM);

    XRC_ADD_STYLE(wxLB_DEFAULT);
    XRC_ADD_STYLE(wxLB_LEFT);
    XRC_ADD_STYLE(wxLB_RIGHT);
    XRC_ADD_STYLE(wxLB_TOP);
    XRC_ADD_STYLE(wxLB_BOTTOM);

    AddWindowStyles();
}

wxObject *CGListBook::DoCreateResource()
{
    if (m_class == wxT("listbookpage"))
    {
        wxXmlNode *n = GetParamNode(wxT("object"));

        if ( !n )
            n = GetParamNode(wxT("object_ref"));

        if (n)
        {
            bool old_ins = m_isInside;
            m_isInside = false;
            wxObject *item = CreateResFromNode(n, m_listbook, NULL);
            m_isInside = old_ins;
            wxWindow *wnd = wxDynamicCast(item, wxWindow);

            if (wnd)
            {
                m_listbook->AddPage(wnd, GetText(wxT("label")),
                                         GetBool(wxT("selected")));
                if ( HasParam(wxT("bitmap")) )
                {
                    wxBitmap bmp = GetBitmap(wxT("bitmap"), wxART_OTHER);
                    wxImageList *imgList = m_listbook->GetImageList();
                    if ( imgList == NULL )
                    {
                        imgList = new wxImageList( bmp.GetWidth(), bmp.GetHeight() );
                        m_listbook->AssignImageList( imgList );
                    }
                    int imgIndex = imgList->Add(bmp);
                    m_listbook->SetPageImage(m_listbook->GetPageCount()-1, imgIndex );
                }
            }
            else
                wxLogError(wxT("Error in resource."));
            return wnd;
        }
        else
        {
            wxLogError(wxT("Error in resource: no control within listbook's <page> tag."));
            return NULL;
        }
    }

    else
    {
        XRC_MAKE_INSTANCE(nb, wxListbook)

        nb->Create(m_parentAsWindow,
                   GetID(),
                   GetPosition(), GetSize(),
                   GetStyle(wxT("style")),
                   GetName());

        wxListbook *old_par = m_listbook;
        m_listbook = nb;
        bool old_ins = m_isInside;
        m_isInside = true;
        CreateChildren(m_listbook, true/*only this handler*/);
        m_isInside = old_ins;
        m_listbook = old_par;

        return nb;
    }
}

bool CGListBook::CanHandle(wxXmlNode *node)
{
    return ((!m_isInside && IsOfClass(node, wxT("wxListbook"))) ||
            (m_isInside && IsOfClass(node, wxT("listbookpage"))));
}

}; // namespace
