/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/

namespace P3D
{
	class CGUILabel : public IGUILabel
	{
	public:
		CGUILabel(const wchar* caption) : IGUILabel(caption) {
		SetHeight(10);
		};
		void Render(float deltaTime);
	};

}; // ns