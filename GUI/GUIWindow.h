/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "IGUIWindow.h"

namespace P3D{

class CGUIWindow : public IGUIWindow
{
public:
	CGUIWindow(const wchar* caption, Vec2i pos, Vec2i size, bool showInTaskBar);
	~CGUIWindow();

	void Render(float deltaTime);
	void Update(float deltaTime);
};

}; // namespace
