/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once
#include "interface.h"
#include "IGUIWindow.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace P3D{

class IGUI : public IBaseInterface
{
public:
	virtual bool Initialize(void* pEngine)=0;
	virtual void Render(float deltaTime)=0;
	virtual void Update(float deltaTime)=0;

	virtual IGUIWindow* CreateWindow(const wchar* caption, Vec2i pos, Vec2i size, bool showInTaskBar=true)=0;
};


}; // namespace