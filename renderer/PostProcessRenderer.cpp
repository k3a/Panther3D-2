/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 09/11/2009 File created - Petr
*/

#include "precompiled.h"

#include "Renderer.h"
#include "PostProcessRenderer.h"

namespace P3D
{
	
	CPostProcessRenderer::CPostProcessRenderer()
	{
		m_pVB = NULL;
	}

	CPostProcessRenderer::~CPostProcessRenderer()
	{
		SAFE_DELETE(m_pVB);
	}

	bool CPostProcessRenderer::Initialize()
	{
		sVBDesc desc;
		desc.CPUAccess = P3D_CPUACCESS_NONE;
		desc.PointParticles = false;
		desc.ResourceUsage = P3D_RESOURCEUSAGE_IMMUTABLE;
		desc.VertexCount = 6;
		desc.VertexSize = sizeof(VPosUV);

		VPosUV v[] = {
			VPosUV(Vec3(-1.0f, -1.0f, 0.0f), Vec2(0.0f, 0.0f)),
			VPosUV(Vec3(-1.0f, 1.0f, 0.0f), Vec2(0.0f, 1.0f)),
			VPosUV(Vec3(1.0f, 1.0f, 0.0f), Vec2(1.0f, 1.0f)),
			VPosUV(Vec3(1.0f, 1.0f, 0.0f), Vec2(1.0f, 1.0f)),
			VPosUV(Vec3(1.0f, -1.0f, 0.0f), Vec2(1.0f, 0.0f)),
			VPosUV(Vec3(-1.0f, -1.0f, 0.0f), Vec2(0.0f, 0.0f))
		};

		m_pVB = new CVertexBuffer;
		if(!m_pVB->Create(desc, v))
		{
			SAFE_DELETE(m_pVB);
			return false;
		}

		return true;
	}

	void CPostProcessRenderer::Render(P3D::CTexture *inRT, P3D::CTexture *outRT)
	{
		//TODO: setup shaders

		if(outRT != NULL) 
			CRenderer::cGraphicsDevice()->BeginRendering(1, outRT);
		else
			CRenderer::cGraphicsDevice()->BeginRendering();

		//TODO: setup shader variables, render the quad
		
		CRenderer::cGraphicsDevice()->EndRendering();
	}

}//NS