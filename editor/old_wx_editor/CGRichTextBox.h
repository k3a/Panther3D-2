/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_text.h
// Purpose:     XML resource handler for wxRichTextCtrl
// Author:      Julian Smart
// Created:     2006-11-08
// RCS-ID:      $Id: xh_richtext.h 43199 2006-11-08 15:45:11Z JS $
// Copyright:   (c) 2006 Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGRichTextBox : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGRichTextBox)

public:
    CGRichTextBox();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


