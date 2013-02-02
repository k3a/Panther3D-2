/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_bmpcbox.h
// Purpose:     XML resource handler for wxBitmapComboBox
// Author:      Jaakko Salli
// Created:     Sep-10-2006
// RCS-ID:      $Id: xh_bmpcbox.h 53135 2008-04-12 02:31:04Z VZ $
// Copyright:   (c) 2006 Jaakko Salli
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

class WXDLLIMPEXP_FWD_ADV wxBitmapComboBox;

namespace P3D {

class WXDLLIMPEXP_XRC CGBmpComboBox : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGBmpComboBox)

public:
    CGBmpComboBox();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    wxBitmapComboBox*    m_combobox;
    bool                m_isInside;
};

}; // namespace


