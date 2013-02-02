/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "IGUIManager.h"

namespace P3D{

class CGUIManager : public IGUIManager
{
public:
	~CGUIManager();
	IGUIWindow* GUICreateWindow(const char* szCaption, Vec2i position, Vec2i size, bool bSizable=true);

	void Render(float deltaTime);
	void Update(float deltaTime);
private:
	vector<IGUIWindow*> m_windows;
};

}; // namespace
