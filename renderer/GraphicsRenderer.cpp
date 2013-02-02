/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#include "Renderer.h"
#include "GraphicsRenderer.h"

namespace P3D{

	// -----------------------------------------------------
	CGraphicsRenderer::CGraphicsRenderer()
	{
		m_mrt_diffuse = NULL;
		m_mrt_normal = NULL;
		m_mrt_depth = NULL;
	}

	// -----------------------------------------------------
	CGraphicsRenderer::~CGraphicsRenderer()
	{
		CRenderer::cGraphicsManager()->DeleteTexture(m_mrt_diffuse);
		CRenderer::cGraphicsManager()->DeleteTexture(m_mrt_normal);
		CRenderer::cGraphicsManager()->DeleteTexture(m_mrt_depth);
	}

	// -----------------------------------------------------
	bool CGraphicsRenderer::Initialize()
	{
		sTextureDesc desc;
		desc.ArraySize = 1;
		desc.AutoGenerateMips = false;
		desc.BindFlag = P3D_TEXBINDFLAG_RENDER_TARGET;
		desc.CPUAccess = P3D_CPUACCESS_NONE;
		desc.MipLevels = 1;
		desc.ResourceUsage = P3D_RESOURCEUSAGE_DEFAULT;
		//desc.Width = 1024;
		//desc.Height = 576;
		// TODO: sizes
		desc.Width = 400;
		desc.Height = 300;

		// Initialize Diffuse Render Target
		desc.Format = P3D_FORMAT_X8R8G8B8_UNORM;
		m_mrt_diffuse = CRenderer::cGraphicsManager()->CreateTexture(desc, NULL, 0);
		if (!m_mrt_diffuse) 
		{
			CON(MSG_ERR_FATAL, _W("Failed to create diffuse RT!") );
			return false;
		}

		// Initialize Normal Render Target
		desc.Format = P3D_FORMAT_R16G16_SNORM;
		m_mrt_normal = CRenderer::cGraphicsManager()->CreateTexture(desc, NULL, 0);
		if (!m_mrt_diffuse) 
		{
			CON(MSG_ERR_FATAL, _W("Failed to create normal RT!") );
			return false;
		}

		// Initialize Depth Render Target
		desc.Format = P3D_FORMAT_R32_FLOAT;
		m_mrt_depth = CRenderer::cGraphicsManager()->CreateTexture(desc, NULL, 0);
		if (!m_mrt_diffuse) 
		{
			CON(MSG_ERR_FATAL, _W("Failed to create depth RT!") );
			return false;
		}

		return true;
	}

	// -----------------------------------------------------
	void CGraphicsRenderer::RenderScene()
	{
		// render scene to diffuse RT
		CRenderer::cGraphicsDevice()->BeginRendering(1, m_mrt_diffuse);
		//CRenderer::cGraphicsDevice()->Clear(ColorRGB(0,1,1));

		for (unsigned int i=0; i<m_chunks.Size(); i++)
		{
			const GeometryChunk& chunk = m_chunks[i];

			CIndexBuffer* ib = CRenderer::cGraphicsManager()->GetCachedIndexBuffer(chunk.Data->ibId);
			CVertexBuffer* vb = CRenderer::cGraphicsManager()->GetCachedVertexBuffer(chunk.Data->vbId);

			ib->Use();
			vb->Use();

			// OPTIM: if we ensure valid pointer every time, remove this if-s
			if (chunk.Subset->materialPtr)
			{
				CShader *vs = (CShader*)chunk.Subset->materialPtr->vsPtr;
				CShader *ps = (CShader*)chunk.Subset->materialPtr->psPtr;

				if (ps) 
					ps->Use();

				if (vs)
				{
					vs->Use();
				}
			}


			vb->RenderIndexed(P3DPT_TRIANGLELIST, chunk.Subset->StartIndex, chunk.Subset->NumTriangles);
			
		}

		CRenderer::cGraphicsDevice()->EndRendering();

		// render diffuse RT on standard backbuffer
		CRenderer::cGraphicsDevice()->BeginRendering();
			
			CRenderer::cPrimitiveRenderer()->AddRectangleRel(Vec2(0.0f, 0.0f), Vec2(1.0f, -1.0f), ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), m_mrt_diffuse);
			CRenderer::cPrimitiveRenderer()->Render();

		CRenderer::cGraphicsDevice()->EndRendering();
	}

	// -----------------------------------------------------
	void CGraphicsRenderer::RenderScene(sFrameBufferUnit *FBUnits, UINT numUnits)
	{
		if(FBUnits != NULL) 
		{
			//TODO: setup the appropriate render targets
			CRenderer::cGraphicsDevice()->BeginRendering(1, m_mrt_diffuse);
			//CRenderer::cGraphicsDevice()->Clear(ColorRGB(0,1,1));
		}
		else CRenderer::cGraphicsDevice()->BeginRendering(); //render to back buffer

		for (unsigned int i=0; i<m_chunks.Size(); i++)
		{
			const GeometryChunk& chunk = m_chunks[i];

			CIndexBuffer* ib = CRenderer::cGraphicsManager()->GetCachedIndexBuffer(chunk.Data->ibId);
			CVertexBuffer* vb = CRenderer::cGraphicsManager()->GetCachedVertexBuffer(chunk.Data->vbId);

			ib->Use();
			vb->Use();

			// OPTIM: if we ensure valid pointer every time, remove this if-s
			if (chunk.Subset->materialPtr)
			{
				CShader *vs = (CShader*)chunk.Subset->materialPtr->vsPtr;
				CShader *ps = (CShader*)chunk.Subset->materialPtr->psPtr;

				if (ps) 
					ps->Use();

				if (vs)
				{
					vs->Use();
				}
			}


			vb->RenderIndexed(P3DPT_TRIANGLELIST, chunk.Subset->StartIndex, chunk.Subset->NumTriangles);

		}

		CRenderer::cGraphicsDevice()->EndRendering();

	}

	// -----------------------------------------------------
	void CGraphicsRenderer::OnResetDevice()
	{
		
	}

	// -----------------------------------------------------
	void CGraphicsRenderer::OnLostDevice()
	{
		
	}

	// -----------------------------------------------------
	void CGraphicsRenderer::SendChunks(const tArray<GeometryChunk> &chunks)
	{
		m_chunks = chunks;
	}

}; // ns