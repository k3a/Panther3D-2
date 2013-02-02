/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_listc.h
// Purpose:     XML resource handler for wxListCtrl
// Author:      Brian Gavin
// Created:     2000/09/09
// RCS-ID:      $Id: xh_listc.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Brian Gavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGList : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGList)

public:
    CGList();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


