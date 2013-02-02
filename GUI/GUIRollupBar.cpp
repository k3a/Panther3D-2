/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "GUI.h"
#include "GUIRollupBar.h"

// rollup bar widgets
#include "GUILabel.h"

namespace P3D
{
	void CGUIRollupBar::Render(float deltaTime)
	{
		ITextRenderer *tr = CGUI::mRenderer()->iTextRenderer();
		IPrimitiveRenderer *pr = CGUI::mRenderer()->iPrimitiveRenderer();

		// background
		pr->AddRectangle(GetPos(), GetPos()+GetSize(), CGUI::cGUIScheme()->GetColor(SCHEME_BACKGROUND) );

		unsigned int y = GetPos().y;

		// render tabs


		// render all visible widgets
		for (unsigned int w=0; w<m_tabs[m_activeTab].widgets.Size(); w++)
		{
			if (m_tabs[m_activeTab].widgets[w]->IsVisible()) 
			{
				m_tabs[m_activeTab].widgets[w]->Render(deltaTime);
			}
		}

		MarkDirty(false);
	}

	void CGUIRollupBar::Update(float deltaTime)
	{
		// count visible widgets and rearrange widgets and make dirty if changed
		m_numVisibleWidgets = 0;
		unsigned int y = GetPos().y;
		// todo: add height of tabs
		for (unsigned int w=0; w<m_tabs[m_activeTab].widgets.Size(); w++)
		{
			if (m_tabs[m_activeTab].widgets[w]->IsVisible()) 
			{
				m_numVisibleWidgets+=1;
				// TODO: rearrange
				m_tabs[m_activeTab].widgets[w]->SetPos(Vec2i(GetPos().x+2, y ));
				m_tabs[m_activeTab].widgets[w]->SetWidth(GetSize().x - 4);
				
				y += m_tabs[m_activeTab].widgets[w]->GetHeight()+2;
			}
		}
	}

	// ------------------------------
	IGUILabel* CGUIRollupBar::AddLabel(const wchar* caption, unsigned int tab)
	{
		assertd((m_nTabs==0 && tab==0) || tab<m_nTabs, "Trying to add label to undefined rollupbar tab!");
		
		CGUILabel *label = new CGUILabel(caption);

		m_tabs[tab].widgets.AddEx(label);

		return label;
	}

}; // ns