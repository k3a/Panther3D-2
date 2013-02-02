/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 09/08/2009 File created - Petr
*/

#include "precompiled.h"

#include "Renderer.h"
#include "RenderCanvas.h"

namespace P3D
{

	CRenderCanvas::CRenderCanvas()
	{
		m_pGBuffer = NULL;
	}

	CRenderCanvas::~CRenderCanvas()
	{
		SAFE_DELETE(m_pGBuffer);
	}

	bool CRenderCanvas::Create(const sRenderCanvasDesc &desc, void *winHandle)
	{
		m_pGBuffer = new CFrameBuffer;
		if( !m_pGBuffer->Create(desc.size) )
		{
			SAFE_DELETE(m_pGBuffer);
			CON(MSG_ERR, _W("Cannot create RenderCanvas!"));
			return false;
		}

		if( !CRenderCanvasWrapper::Create(desc, winHandle) )
		{
			SAFE_DELETE(m_pGBuffer);
			CON(MSG_ERR, _W("Cannot create RenderCanvas!"));
			return false;
		}

		return true;
	}

}//NS
