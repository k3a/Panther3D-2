/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#if defined(_DX) && _DX==9
#include "DX10/RenderCanvasWrapper.h"
#elif defined(_OGL)
#include "DX10/RenderCanvasWrapper.h"
#elif defined(_DX) && _DX==10
#include "DX10/RenderCanvasWrapper.h"
#endif

#include "FrameBuffer.h"

namespace P3D
{
	class CRenderCanvas : public CRenderCanvasWrapper
	{
	public:
		CRenderCanvas();
		~CRenderCanvas();
		bool Create(const sRenderCanvasDesc &desc, void *winHandle);
		inline CFrameBuffer* GetGBuffer() { return m_pGBuffer; };
	private:
		CFrameBuffer *m_pGBuffer;
	};
};
