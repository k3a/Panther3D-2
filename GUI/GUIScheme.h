/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "types.h"
#include "GUIScheme.h"
#include "ITextRenderer.h"

namespace P3D
{

enum eScheme
{
	SCHEME_SELECTED_HI=0,
	SCHEME_SELECTED_LO,
	SCHEME_HI,
	SCHEME_LO,
	SCHEME_BACKGROUND,
	SCHEME_CAPTION,
	SCHEME_CAPTION_TEXT,
	SCHEME_TEXT,
	SCHEME_ROLLUP_BACKGROUND,

	SCHEME_NUM
};

enum eSchemeFont
{
	SFONT_NORMAL,
	SFONT_NUM
};

class CGUIScheme
{
public:
	CGUIScheme();
	void LoadDefault();
	ColorRGBA GetColor(eScheme sch)const
	{
		if (sch<0) sch=(eScheme)0; 
		if (sch>=SCHEME_NUM) sch = (eScheme)0; 
		return m_colors[sch]; 
	};
	FontHandle GetFont(eSchemeFont sch)
	{
		if (sch<0) sch=SFONT_NORMAL; 
		if (sch>=SCHEME_NUM) sch = SFONT_NORMAL; 
		return m_fonts[sch];
	}
private:
	ColorRGBA m_colors[SCHEME_NUM];
	FontHandle m_fonts[SFONT_NUM];
};

}; // namespace