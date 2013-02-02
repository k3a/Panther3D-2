/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "GUI.h"

namespace P3D
{
	class CGUIRollupBar : public IGUIRollupBar
	{
	public:
		void Render(float deltaTime);
		void Update(float deltaTime);
		IGUILabel* AddLabel(const wchar* caption, unsigned int tab=0);
	};
}; // ns