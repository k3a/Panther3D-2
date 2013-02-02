/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "GUI.h"

namespace P3D
{

CGUIScheme::CGUIScheme()
{
	LoadDefault();
}

void CGUIScheme::LoadDefault()
{
	m_colors[SCHEME_SELECTED_HI]=ColorRGBA(1, 0.5f, 0, 1);
	m_colors[SCHEME_SELECTED_LO]=ColorRGBA(0.5f, 0.25f, 0, 1);
	m_colors[SCHEME_HI]=ColorRGBA(0.7f, 0.7f, 0.7f, 1);
	m_colors[SCHEME_LO]=ColorRGBA(0.3f, 0.3f, 0.3f, 1);
	m_colors[SCHEME_BACKGROUND]=ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f);
	m_colors[SCHEME_CAPTION]=ColorRGBA(0, 0.5f, 0.75f, 1);
	m_colors[SCHEME_CAPTION_TEXT]=ColorRGBA(1, 1, 1, 1);
	m_colors[SCHEME_TEXT]=ColorRGBA(1, 1, 1, 1);
	m_colors[SCHEME_ROLLUP_BACKGROUND]=ColorRGBA(0.6f, 0.55f, 0.5f, 1);

	m_fonts[SFONT_NORMAL] = CGUI::mRenderer()->iTextRenderer()->CreateFont(_W("arial.ttf"), 10 );
	//m_fonts[SFONT_TEXT] = CGUI::mRenderer()->iTextRenderer()->CreateFont(_W("arial.ttf"), 8 );
}

}; // namespace