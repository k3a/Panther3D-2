/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "GUI.h"
#include "GUILabel.h"

namespace P3D {

	void CGUILabel::Render(float deltaTime)
	{
		ITextRenderer *tr = CGUI::mRenderer()->iTextRenderer();

		tr->Print( GetCaption(), GetPos(), CGUI::cGUIScheme()->GetColor(SCHEME_TEXT), CGUI::cGUIScheme()->GetFont(SFONT_NORMAL), &m_size );

		MarkDirty(false);
	}

}; // ns