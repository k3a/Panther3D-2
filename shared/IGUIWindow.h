/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once
#include "interface.h"

namespace P3D{

/// Base GUI Widget class
class IGUIWidget
{
public:
	IGUIWidget() { m_bDirty=true; m_bVisible=true; m_pos = Vec2i(0,0); m_size=Vec2i(10,10); };
	bool IsVisible()const{return m_bVisible;};
	bool SetVisible(bool visible=true){ m_bVisible=visible;}; 
	bool Show(){SetVisible(true);};
	bool Hide(){SetVisible(false);};

	// --- position and size
	void SetPos(Vec2i pos){ 
		m_pos = pos;
		
		Vec2i size = GetSize();

		m_rect.x1 = pos.x;
		m_rect.x2 = pos.x+size.x;
		m_rect.y1 = pos.y;
		m_rect.y2 = pos.y+size.y;
	};
	Vec2i GetPos()const{ return m_pos; };
	void SetSize(Vec2i size){
		m_size = size;

		Vec2i pos = GetPos();

		m_rect.x1 = pos.x;
		m_rect.x2 = pos.x+size.x;
		m_rect.y1 = pos.y;
		m_rect.y2 = pos.y+size.y;
	};
	Vec2i GetSize()const{ return m_size; };
	void SetLocation(Vec2i pos, Vec2i size){ SetPos(pos); SetSize(size); };
	void GetLocation(Vec2i &pos, Vec2i &size)const{ pos = m_pos; size = m_size; };
	Rect GetRect()const{ return m_rect; };
	unsigned int GetWidth()const{ return m_size.x; };
	unsigned int GetHeight()const{ return m_size.y; };
	void SetWidth(unsigned int val ){ m_size.x = val; };
	void SetHeight(unsigned int val){ m_size.y = val; };

	void MarkDirty(bool dirty=true){ m_bDirty=dirty; };
	bool IsDirty()const{ return m_bDirty; };

	// --- virtual methods
	virtual void Update(float deltaTime){};
	virtual void Render(float deltaTime){};

protected:
	bool m_bVisible;
	Vec2i m_pos;
	Vec2i m_size;
	Rect m_rect;
	bool m_bDirty;
};

class IGUILabel : public IGUIWidget
{
public:
	IGUILabel(const wchar* caption)
	{
		m_caption=NULL;
		SetCaption(caption);
	};
	void SetCaption(const wchar* caption){ 
		if (m_caption) delete[] m_caption;
		m_caption = new wchar[wstrlen(caption)+1];
		wstrcpy(m_caption, caption);
		MarkDirty();
	};
	const wchar* GetCaption()const{ return m_caption; };

protected:
	wchar* m_caption;
};

struct sRollupTab
{
	tArray<IGUIWidget*> widgets;
	Str title;
};

class IGUIRollupBar : public IGUIWidget
{
public:
	IGUIRollupBar(){ 
		m_nTabs=0;
		sRollupTab tab;
		tab.title.SetEx(_W("General"));
		m_tabs.AddEx(tab);

		m_numVisibleWidgets = 0;
		m_nWidgets = 0;
		m_activeTab = 0;
	};
	unsigned int GetNumTabs()const{ return m_nTabs; };
	unsigned int GetNumVisibleWidgets()const { return m_numVisibleWidgets; };
	unsigned int GetActiveTab()const{ return m_activeTab; };
	void SetActiveTab(unsigned int tab){ m_activeTab=tab; };
	unsigned int GetNumWidgets(){ return m_nWidgets; };

	/// Adds new tab to rollup bar. Tab ID will be returned (first is zero).
	unsigned int AddTab(const wchar* caption)
	{
		if (m_nTabs==0)
		{
			m_tabs[0].title.SetEx(caption);
		}
		else
		{
			sRollupTab tab;
			tab.title.SetEx(caption);
			m_tabs.AddEx(tab);
		}
		return m_nTabs++;
	};

	virtual IGUILabel* AddLabel(const wchar* caption, unsigned int tab=0)=0;

protected:
	tArray<sRollupTab> m_tabs;
	unsigned int m_nWidgets;
	unsigned int m_nTabs;
	unsigned int m_numVisibleWidgets;
	unsigned int m_activeTab;
};

class IGUIWindow : public IGUIWidget
{
public:
	~IGUIWindow(){ 
		SAFE_DELETE_ARRAY(m_caption); 
		m_rollupLeft = NULL;
		m_rollupRight = NULL;
	};
	IGUIWindow(const wchar* caption, Vec2i pos, Vec2i size, bool showInTaskBar)
	{
		// title
		m_caption = NULL;
		SetCaption(caption);

		// other props
		m_showInTaskBar = showInTaskBar;
		SetPos(pos);
		SetSize(size);
	};
	void SetCaption(const wchar* caption){ 
		if (m_caption) delete[] m_caption;
		m_caption = new wchar[wstrlen(caption)+1];
		wstrcpy(m_caption, caption);
		MarkDirty();
	};
	const wchar* GetCaption()const{ return m_caption; };
	bool IsShownInTaskBar()const{ return m_showInTaskBar; };
	void ShowInTaskBar(bool show=true){ m_showInTaskBar=show; };

	IGUIRollupBar* LeftRollup()const{ return m_rollupLeft; };
	IGUIRollupBar* RightRollup()const{ return m_rollupRight; };

protected:
	wchar* m_caption;
	bool m_showInTaskBar;
	IGUIRollupBar* m_rollupLeft;
	IGUIRollupBar* m_rollupRight;
};

}; // namespace