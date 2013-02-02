/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once
#include "wx/xrc/xmlres.h"

namespace P3D
{

class WXDLLIMPEXP_XRC CGButton : public wxXmlResourceHandlerEx
{
	DECLARE_DYNAMIC_CLASS(CGButton)

public:
	CGButton();
	wxObject *DoCreateResource();
	bool CanHandle(wxXmlNode *node);
	void EventClick(wxCommandEvent& event);

};

};