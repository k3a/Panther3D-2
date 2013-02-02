/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_combo.h
// Purpose:     XML resource handler for wxComboBox
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id: xh_combo.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGCombo : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGCombo)

public:
    CGCombo();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_insideBox;
    wxArrayString strList;
};

}; // namespace


