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
#endif

#include "wx/bmpcbox.h"


namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGBmpComboBox, wxXmlResourceHandlerEx)

CGBmpComboBox::CGBmpComboBox()
                     :wxXmlResourceHandlerEx()
                     ,m_combobox(NULL)
                     ,m_isInside(false)
{
    XRC_ADD_STYLE(wxCB_SORT);
    XRC_ADD_STYLE(wxCB_READONLY);
    AddWindowStyles();
}

wxObject *CGBmpComboBox::DoCreateResource()
{
    if (m_class == wxT("ownerdrawnitem"))
    {
        wxCHECK_MSG(m_combobox, NULL, wxT("Incorrect syntax of XRC resource: ownerdrawnitem not within a bitmapcombobox!"));

        m_combobox->Append(GetText(wxT("text")), GetBitmap(wxT("bitmap"), wxART_MISSING_IMAGE));

        return m_combobox;
    }
    else /*if( m_class == wxT("wxBitmapComboBox"))*/
    {
        // find the selection
        long selection = GetLong( wxT("selection"), -1 );

        XRC_MAKE_INSTANCE(control, wxBitmapComboBox)

        control->Create(m_parentAsWindow,
                        GetID(),
                        GetText(wxT("value")),
                        GetPosition(), GetSize(),
                        0,
                        NULL,
                        GetStyle(),
                        wxDefaultValidator,
                        GetName());

        m_isInside = true;
        m_combobox = control;

        wxXmlNode *children_node = GetParamNode(wxT("object"));

        wxXmlNode *n = children_node;

        while (n)
        {
            if ((n->GetType() == wxXML_ELEMENT_NODE) &&
                (n->GetName() == wxT("object")))
            {
                CreateResFromNode(n, control, NULL);
            }
            n = n->GetNext();
        }

        m_isInside = false;
        m_combobox = NULL;

        if (selection != -1)
            control->SetSelection(selection);

        SetupWindow(control);

        return control;
    }
}

bool CGBmpComboBox::CanHandle(wxXmlNode *node)
{
    return ((!m_isInside && IsOfClass(node, wxT("wxBitmapComboBox"))) ||
            (m_isInside && IsOfClass(node, wxT("ownerdrawnitem"))));
}

}; // namespace
