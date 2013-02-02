/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_listb.h
// Purpose:     XML resource handler for wxListbox
// Author:      Bob Mitchell & Vaclav Slavik
// Created:     2000/07/29
// RCS-ID:      $Id: xh_listb.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Bob Mitchell & Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGListBox : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGListBox)

public:
    CGListBox();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_insideBox;
    wxArrayString strList;
};

}; // namespace


