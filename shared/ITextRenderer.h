/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#include "interface.h"

namespace P3D
{

typedef unsigned int FontHandle;

/// Text rendering class
/** This class can be used to render text and characters. You can load TrueType font from file. Font is cached before rendering. */
class ITextRenderer
{
public:
	virtual ~ITextRenderer(){};
	/// Print text
	/** Text will not be printed immediately, but it will be added to cache and rendered with other text using Render().
	\see Render */
	virtual void Print(const wchar* string, Vec2i position, ColorRGBA color, Vec2i *size = NULL, Rect *restrict=NULL)=0;
	/// Print text size with specified font
	/** Text will not be printed immediately, but it will be added to cache and rendered with other text using Render().
	\see Render */
	virtual void Print(const wchar* string, Vec2i position, ColorRGBA color, FontHandle font, Vec2i *size = NULL, Rect *restrict=NULL)=0;
	/// Calculate text size
	virtual void CalcSize(const wchar* string, Vec2i *size)=0;
	/// Calculate text size with specified font
	virtual void CalcSize(const wchar* string, FontHandle font, Vec2i *size)=0;
	/// Get height of specified font
	virtual int GetFontHeight(FontHandle font)=0;
	/// Render all cached text to screen
	virtual void Render()=0;
	/// Create font from file
	/** The font texture will be generated from input font file. You can specify font size.
	\return Font handle useful for manipulation with this font */
	virtual FontHandle CreateFont(const wchar* filename, unsigned int size)=0;
};

}; // namespace