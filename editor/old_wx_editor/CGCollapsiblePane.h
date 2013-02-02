/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_collpane.h
// Purpose:     XML resource handler for wxCollapsiblePane
// Author:      Francesco Montorsi
// Created:     2006-10-27
// RCS-ID:      $Id: xh_collpane.h 52422 2008-03-10 13:55:07Z VZ $
// Copyright:   (c) 2006 Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"
class WXDLLIMPEXP_FWD_CORE wxCollapsiblePane;

namespace P3D {

class WXDLLIMPEXP_XRC CGCollapsiblePane : public wxXmlResourceHandlerEx
{
public:
    CGCollapsiblePane();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_isInside;
    wxCollapsiblePane *m_collpane;

    DECLARE_DYNAMIC_CLASS(CGCollapsiblePane)
};

}; // namespace


