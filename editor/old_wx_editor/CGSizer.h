/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_sizer.h
// Purpose:     XML resource handler for wxBoxSizer
// Author:      Vaclav Slavik
// Created:     2000/04/24
// RCS-ID:      $Id: xh_sizer.h 49804 2007-11-10 01:09:42Z VZ $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"



#include "wx/sizer.h"
#include "wx/gbsizer.h"

class WXDLLIMPEXP_FWD_CORE wxSizer;

namespace P3D {

class WXDLLIMPEXP_XRC CGSizer : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGSizer)

public:
    CGSizer();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_isInside;
    bool m_isGBS;

    wxSizer *m_parentSizer;

    bool IsSizerNode(wxXmlNode *node);

    wxObject* Handle_sizeritem();
    wxObject* Handle_spacer();
    wxObject* Handle_sizer();
    wxSizer*  Handle_wxBoxSizer();

    wxSizer*  Handle_wxStaticBoxSizer();
    wxSizer*  Handle_wxGridSizer();
    wxSizer*  Handle_wxFlexGridSizer();
    wxSizer*  Handle_wxGridBagSizer();

    void SetGrowables(wxFlexGridSizer* fsizer, const wxChar* param, bool rows);
    wxGBPosition GetGBPos(const wxString& param);
    wxGBSpan GetGBSpan(const wxString& param);
    wxSizerItem* MakeSizerItem();
    void SetSizerItemAttributes(wxSizerItem* sitem);
    void AddSizerItem(wxSizerItem* sitem);
};



class WXDLLIMPEXP_XRC CGStdDialogButtonSizer
    : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGStdDialogButtonSizer)

public:
    CGStdDialogButtonSizer();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_isInside;
    wxStdDialogButtonSizer *m_parentSizer;
};


}; // namespace



