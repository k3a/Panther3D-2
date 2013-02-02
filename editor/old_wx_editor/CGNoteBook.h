/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_notbk.h
// Purpose:     XML resource handler for wxNotebook
// Author:      Vaclav Slavik
// RCS-ID:      $Id: xh_notbk.h 53135 2008-04-12 02:31:04Z VZ $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

class WXDLLIMPEXP_FWD_CORE wxNotebook;

namespace P3D {

class WXDLLIMPEXP_XRC CGNoteBook : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGNoteBook)

public:
    CGNoteBook();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_isInside;
    wxNotebook *m_notebook;
};

}; // namespace


