/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 09/09/2009 File created - Petr
*/
#pragma once

namespace P3D
{
	
	class CPostProcessRenderer
	{
	public:
		CPostProcessRenderer();
		~CPostProcessRenderer();
		bool Initialize();

		void Render(CTexture *inRT, CTexture *outRT = NULL);

		
	private:
		CVertexBuffer *m_pVB; // fullscreen quad

	};

}; // ns