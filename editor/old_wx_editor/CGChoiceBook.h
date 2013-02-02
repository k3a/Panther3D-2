/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_choicbk.h
// Purpose:     XML resource handler for wxChoicebook
// Author:      Vaclav Slavik
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

class WXDLLIMPEXP_FWD_CORE wxChoicebook;

namespace P3D {

class WXDLLIMPEXP_XRC CGChoiceBook : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGChoiceBook)

public:
    CGChoiceBook();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_isInside;
    wxChoicebook *m_choicebook;
};

}; // namespace


