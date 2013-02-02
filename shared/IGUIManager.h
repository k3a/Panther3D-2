/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once
#include "interface.h"
#include "IGUIWindow.h"

namespace P3D{

///Represents a manager for the engine's GUI system.
class IGUIManager : public IBaseInterface
{
public:
	virtual IGUIWindow* GUICreateWindow(const char* szCaption, Vec2i position, Vec2i size, bool bSizable=true)=0;
	virtual void Render(float deltaTime)=0;
	virtual void Update(float deltaTime)=0;
};

#define IFACE_GUI_MANAGER "gui_manager_1"

}; // namespace