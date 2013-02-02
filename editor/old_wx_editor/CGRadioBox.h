/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_radbx.h
// Purpose:     XML resource handler for wxRadioBox
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id: xh_radbx.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGRadioBox : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGRadioBox)

public:
    CGRadioBox();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_insideBox;

    // the items labels
    wxArrayString labels;

    // the items tooltips
    wxArrayString tooltips;

    // the item help text
    wxArrayString helptexts;
    wxArrayInt    helptextSpecified;
};

}; // namespace


