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

#include "wx/treebook.h"
#include "wx/imaglist.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGTreeBook, wxXmlResourceHandlerEx)

CGTreeBook::CGTreeBook()
                    : wxXmlResourceHandlerEx(),
                      m_tbk(NULL),
                      m_isInside(false)
{
    XRC_ADD_STYLE(wxBK_DEFAULT);
    XRC_ADD_STYLE(wxBK_TOP);
    XRC_ADD_STYLE(wxBK_BOTTOM);
    XRC_ADD_STYLE(wxBK_LEFT);
    XRC_ADD_STYLE(wxBK_RIGHT);

    AddWindowStyles();
}

bool CGTreeBook::CanHandle(wxXmlNode *node)
{
    return ((!m_isInside && IsOfClass(node, wxT("wxTreebook"))) ||
            (m_isInside && IsOfClass(node, wxT("treebookpage"))));
}


wxObject *CGTreeBook::DoCreateResource()
{
    if (m_class == wxT("wxTreebook"))
    {
        XRC_MAKE_INSTANCE(tbk, wxTreebook)

        tbk->Create(m_parentAsWindow,
                    GetID(),
                    GetPosition(), GetSize(),
                    GetStyle(wxT("style")),
                    GetName());

        wxTreebook * old_par = m_tbk;
        m_tbk = tbk;

        bool old_ins = m_isInside;
        m_isInside = true;

        wxArrayTbkPageIndexes old_treeContext = m_treeContext;
        m_treeContext.Clear();

        CreateChildren(m_tbk, true/*only this handler*/);

        m_treeContext = old_treeContext;
        m_isInside = old_ins;
        m_tbk = old_par;

        return tbk;
    }

//    else ( m_class == wxT("treebookpage") )
    wxXmlNode *n = GetParamNode(wxT("object"));
    wxWindow *wnd = NULL;

    if ( !n )
        n = GetParamNode(wxT("object_ref"));

    if (n)
    {
        bool old_ins = m_isInside;
        m_isInside = false;
        wxObject *item = CreateResFromNode(n, m_tbk, NULL);
        m_isInside = old_ins;
        wnd = wxDynamicCast(item, wxWindow);

        if (wnd == NULL && item != NULL)
            wxLogError(wxT("Error in resource: control within treebook's <page> tag is not a window."));
    }

    size_t depth = GetLong( wxT("depth") );

    if( depth <= m_treeContext.Count() )
    {
        // first prepare the icon
        int imgIndex = wxNOT_FOUND;
        if ( HasParam(wxT("bitmap")) )
        {
            wxBitmap bmp = GetBitmap(wxT("bitmap"), wxART_OTHER);
            wxImageList *imgList = m_tbk->GetImageList();
            if ( imgList == NULL )
            {
                imgList = new wxImageList( bmp.GetWidth(), bmp.GetHeight() );
                m_tbk->AssignImageList( imgList );
            }
            imgIndex = imgList->Add(bmp);
        }

        // then add the page to the corresponding parent
        if( depth < m_treeContext.Count() )
            m_treeContext.RemoveAt(depth, m_treeContext.Count() - depth );
        if( depth == 0)
        {
            m_tbk->AddPage(wnd,
                GetText(wxT("label")), GetBool(wxT("selected")), imgIndex);
        }
        else
        {
            m_tbk->InsertSubPage(m_treeContext.Item(depth - 1), wnd,
                GetText(wxT("label")), GetBool(wxT("selected")), imgIndex);
        }

        m_treeContext.Add( m_tbk->GetPageCount() - 1);

    }
    else
        wxLogError(wxT("Error in resource. wxTreebookPage has an invalid depth."));
    return wnd;
}

}; // namespace
