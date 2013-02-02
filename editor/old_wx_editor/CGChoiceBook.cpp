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

#include "wx/choicebk.h"
#include "wx/imaglist.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGChoiceBook, wxXmlResourceHandlerEx)

CGChoiceBook::CGChoiceBook()
                       :wxXmlResourceHandlerEx(),
                        m_isInside(false),
                        m_choicebook(NULL)
{
    XRC_ADD_STYLE(wxBK_DEFAULT);
    XRC_ADD_STYLE(wxBK_LEFT);
    XRC_ADD_STYLE(wxBK_RIGHT);
    XRC_ADD_STYLE(wxBK_TOP);
    XRC_ADD_STYLE(wxBK_BOTTOM);

    XRC_ADD_STYLE(wxCHB_DEFAULT);
    XRC_ADD_STYLE(wxCHB_LEFT);
    XRC_ADD_STYLE(wxCHB_RIGHT);
    XRC_ADD_STYLE(wxCHB_TOP);
    XRC_ADD_STYLE(wxCHB_BOTTOM);

    AddWindowStyles();
}

wxObject *CGChoiceBook::DoCreateResource()
{
    if (m_class == wxT("choicebookpage"))
    {
        wxXmlNode *n = GetParamNode(wxT("object"));

        if ( !n )
            n = GetParamNode(wxT("object_ref"));

        if (n)
        {
            bool old_ins = m_isInside;
            m_isInside = false;
            wxObject *item = CreateResFromNode(n, m_choicebook, NULL);
            m_isInside = old_ins;
            wxWindow *wnd = wxDynamicCast(item, wxWindow);

            if (wnd)
            {
                m_choicebook->AddPage(wnd, GetText(wxT("label")),
                                           GetBool(wxT("selected")));
                if ( HasParam(wxT("bitmap")) )
                {
                    wxBitmap bmp = GetBitmap(wxT("bitmap"), wxART_OTHER);
                    wxImageList *imgList = m_choicebook->GetImageList();
                    if ( imgList == NULL )
                    {
                        imgList = new wxImageList( bmp.GetWidth(), bmp.GetHeight() );
                        m_choicebook->AssignImageList( imgList );
                    }
                    int imgIndex = imgList->Add(bmp);
                    m_choicebook->SetPageImage(m_choicebook->GetPageCount()-1, imgIndex );
                }
            }
            else
                wxLogError(wxT("Error in resource."));
            return wnd;
        }
        else
        {
            wxLogError(wxT("Error in resource: no control within choicebook's <page> tag."));
            return NULL;
        }
    }

    else
    {
        XRC_MAKE_INSTANCE(nb, wxChoicebook)

        nb->Create(m_parentAsWindow,
                   GetID(),
                   GetPosition(), GetSize(),
                   GetStyle(wxT("style")),
                   GetName());

        wxChoicebook *old_par = m_choicebook;
        m_choicebook = nb;
        bool old_ins = m_isInside;
        m_isInside = true;
        CreateChildren(m_choicebook, true/*only this handler*/);
        m_isInside = old_ins;
        m_choicebook = old_par;

        return nb;
    }
}

bool CGChoiceBook::CanHandle(wxXmlNode *node)
{
    return ((!m_isInside && IsOfClass(node, wxT("wxChoicebook"))) ||
            (m_isInside && IsOfClass(node, wxT("choicebookpage"))));
}

}; // namespace
