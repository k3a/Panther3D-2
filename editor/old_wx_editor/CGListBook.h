/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_listbk.h
// Purpose:     XML resource handler for wxListbook
// Author:      Vaclav Slavik
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

class WXDLLIMPEXP_FWD_CORE wxListbook;

namespace P3D {

class WXDLLIMPEXP_XRC CGListBook : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGListBook)

public:
    CGListBook();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_isInside;
    wxListbook *m_listbook;
};

}; // namespace


