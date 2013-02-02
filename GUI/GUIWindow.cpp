/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "GUIWindow.h"
#include "GUI.h"

#include "GUIRollupBar.h"

namespace P3D{

	CGUIWindow::CGUIWindow(const wchar* caption, Vec2i pos, Vec2i size, bool showInTaskBar) 
		: IGUIWindow(caption, pos, size, showInTaskBar)
	{
		m_rollupLeft = new CGUIRollupBar();
		m_rollupRight = new CGUIRollupBar();
	}

	CGUIWindow::~CGUIWindow()
	{
		SAFE_DELETE(m_rollupLeft);
		SAFE_DELETE(m_rollupRight);
	}

	// --------------------------------------
	void CGUIWindow::Render(float deltaTime)
	{
		ITextRenderer *tr = CGUI::mRenderer()->iTextRenderer();
		IPrimitiveRenderer *pr = CGUI::mRenderer()->iPrimitiveRenderer();

		pr->AddRectangle(GetPos(), GetPos()+GetSize(), CGUI::cGUIScheme()->GetColor(SCHEME_BACKGROUND) );
		tr->Print( GetCaption(), GetPos(), CGUI::cGUIScheme()->GetColor(SCHEME_TEXT), CGUI::cGUIScheme()->GetFont(SFONT_NORMAL) );
		
		/*if (m_rollupLeft->IsDirty())*/ m_rollupLeft->Render(deltaTime);

		MarkDirty(false);
	}

	// -----------------------------------
	void CGUIWindow::Update(float deltaTime)
	{
		Vec2i sz;
		sz = GetSize();
		sz.x /= 5;

		// make new layout
		m_rollupLeft->SetPos( GetPos() ); // temp.
		m_rollupLeft->SetSize(sz);

		m_rollupLeft->Update(deltaTime);
	}

}; // namespace
