/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_toolb.h
// Purpose:     XML resource handler for wxToolBar
// Author:      Vaclav Slavik
// Created:     2000/08/11
// RCS-ID:      $Id: xh_toolb.h 53135 2008-04-12 02:31:04Z VZ $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

class WXDLLIMPEXP_FWD_CORE wxToolBar;

namespace P3D {

class WXDLLIMPEXP_XRC CGToolBar : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGToolBar)

public:
    CGToolBar();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_isInside;
    wxToolBar *m_toolbar;
};

}; // namespace


