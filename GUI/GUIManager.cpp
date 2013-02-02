/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "GUIManager.h"

#include "GUIWindow.h"

namespace P3D{

REGISTER_SINGLE_CLASS(CGUIManager, IFACE_GUI_MANAGER);

CGUIManager::~CGUIManager()
{
	for (unsigned int i=0; i<m_windows.size(); i++)
		SAFE_DELETE(m_windows[i]);
}

IGUIWindow* CGUIManager::GUICreateWindow(const char* szCaption, Vec2i position, Vec2i size, bool bSizable/*=true*/)
{
	if (size.x < 20) size.x = 20;
	if (size.y < 20) size.y = 20;

	CGUIWindow *pObj = new CGUIWindow();
	pObj->SetCaption(szCaption);
	pObj->SetPosition(position);
	pObj->SetSizableX(true);
	pObj->SetSizableY(true);
	pObj->SetMinSize(size); // set min size to actual size ;) not necessary but for sure
	pObj->SetSize(size);

	if (bSizable)
	{
		pObj->SetSizableX(true);
		pObj->SetSizableY(true);
	}

	m_windows.push_back(pObj);

	return pObj;
}

void CGUIManager::Render(float deltaTime)
{
	for (unsigned int i=0; i<m_windows.size(); i++)
		m_windows[i]->Render(deltaTime);
}
void CGUIManager::Update(float deltaTime)
{
	for (unsigned int i=0; i<m_windows.size(); i++)
		m_windows[i]->Update(deltaTime);
}

}; // namespace
