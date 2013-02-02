/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once

#include "interface.h"
#include "BaseObject.h"

#include "ITexture.h"

namespace P3D {

class CTexture : public BaseObject
{
public:
	CTexture();
	/// Load texture by name (Category.Subcategory.TextureName)
	bool Load(const wchar* name);
	/// Was texture loaded?
	bool IsLoaded(){return m_bLoaded;};

// ---- parameters
public:
	StringParam Name;
	SizeParam Size;
	IntParam NumMips;

// ---- private members
private:
	ITexture* m_texture;
	bool m_bLoaded;
};

}; //ns