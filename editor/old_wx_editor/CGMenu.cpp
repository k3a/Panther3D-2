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
#include "wx/frame.h"
#include "wx/menu.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGMenu, wxXmlResourceHandlerEx)

CGMenu::CGMenu() :
        wxXmlResourceHandlerEx(), m_insideMenu(false)
{
    XRC_ADD_STYLE(wxMENU_TEAROFF);
}

wxObject *CGMenu::DoCreateResource()
{
    if (m_class == wxT("wxMenu"))
    {
        wxMenu *menu = m_instance ? wxStaticCast(m_instance, wxMenu)
                                  : new wxMenu(GetStyle());

        wxString title = GetText(wxT("label"));
        wxString help = GetText(wxT("help"));

        bool oldins = m_insideMenu;
        m_insideMenu = true;
        CreateChildren(menu, true/*only this handler*/);
        m_insideMenu = oldins;

        wxMenuBar *p_bar = wxDynamicCast(m_parent, wxMenuBar);
        if (p_bar)
        {
            p_bar->Append(menu, title);
        }
        else
        {
            wxMenu *p_menu = wxDynamicCast(m_parent, wxMenu);
            if (p_menu)
            {
                p_menu->Append(GetID(), title, menu, help);
                if (HasParam(wxT("enabled")))
                    p_menu->Enable(GetID(), GetBool(wxT("enabled")));
            }
        }

        return menu;
    }

    else
    {
        wxMenu *p_menu = wxDynamicCast(m_parent, wxMenu);

        if (m_class == wxT("separator"))
            p_menu->AppendSeparator();
        else if (m_class == wxT("break"))
            p_menu->Break();
        else /*wxMenuItem*/
        {
            int id = GetID();
            wxString label = GetText(wxT("label"));
            wxString accel = GetText(wxT("accel"), false);
            wxString fullLabel = label;
            if (!accel.empty())
                fullLabel << wxT("\t") << accel;

            wxItemKind kind = wxITEM_NORMAL;
            if (GetBool(wxT("radio")))
                kind = wxITEM_RADIO;
            if (GetBool(wxT("checkable")))
            {
                wxASSERT_MSG( kind == wxITEM_NORMAL, _T("can't have both checkable and radion button at once") );
                kind = wxITEM_CHECK;
            }

            wxMenuItem *mitem = new wxMenuItem(p_menu, id, fullLabel,
                                               GetText(wxT("help")), kind);

#if (!defined(__WXMSW__) && !defined(__WXPM__)) || wxUSE_OWNER_DRAWN
            if (HasParam(wxT("bitmap")))
                mitem->SetBitmap(GetBitmap(wxT("bitmap"), wxART_MENU));
#endif
            p_menu->Append(mitem);
            mitem->Enable(GetBool(wxT("enabled"), true));
            if (kind == wxITEM_CHECK)
                mitem->Check(GetBool(wxT("checked")));
        }
        return NULL;
    }
}



bool CGMenu::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxMenu")) ||
           (m_insideMenu &&
               (IsOfClass(node, wxT("wxMenuItem")) ||
                IsOfClass(node, wxT("break")) ||
                IsOfClass(node, wxT("separator")))
           );
}

IMPLEMENT_DYNAMIC_CLASS(wxMenuBarXmlHandler, wxXmlResourceHandlerEx)

wxMenuBarXmlHandler::wxMenuBarXmlHandler() : wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxMB_DOCKABLE);
}

wxObject *wxMenuBarXmlHandler::DoCreateResource()
{
    wxMenuBar *menubar = new wxMenuBar(GetStyle());
    CreateChildren(menubar);

    if (m_parentAsWindow)
    {
        wxFrame *parentFrame = wxDynamicCast(m_parent, wxFrame);
        if (parentFrame)
            parentFrame->SetMenuBar(menubar);
    }

    return menubar;
}



bool wxMenuBarXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxMenuBar"));
}

}; // namespace
