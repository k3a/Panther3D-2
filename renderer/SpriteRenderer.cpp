/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "SpriteRenderer.h"
#include "symbols.h"
#include "Renderer.h"

namespace P3D
{

	CSpriteRenderer::CSpriteRenderer()
	{
		m_pVB = 0;
		m_pIB = 0;
		//m_technique = SPT_POINTSPRITE;
		m_technique = SPT_POINTSPRITEINSTANCED;
		m_matrix.SetIdentityMatrix();
		m_matrix.Translate(0.0f, 0.0f, 0.0f);
		m_pVS = m_pPS = 0;
	}

	CSpriteRenderer::~CSpriteRenderer()
	{
		SAFE_DELETE(m_pVB);
		SAFE_DELETE(m_pIB);
	}

	bool CSpriteRenderer::Create()
	{
		////unsigned int maxParticles = 1000;
		////m_particles.resize(maxParticles);
		////m_pAliveParticles.reserve(maxParticles);
		////m_pDeadParticles.reserve(maxParticles);

		//m_time = 0.0f;
		//m_numAliveParticles = 0;
		//m_maxNumParticles = 1000;

		////for(unsigned int i=0; i<m_maxNumParticles; i++) m_pDeadParticles[i] = &m_particles[i];

		//VertexElement ve[2];
		//ve[0] = VertexElement(P3DVD_FLOAT3, P3DVU_POSITION);

		//ve[1] = P3DVE_END();

		//m_pVB = new CVertexBuffer;

		//if(m_technique == SPT_POINTSPRITE)
		//{
		//	sVBDesc desc;
		//	desc.VertexCount = m_maxNumParticles;
		//	desc.VertexSize = sizeof(SpriteVertex);
		//	desc.CPUAccess = CPUACCESS_WRITE;
		//	desc.ResourceUsage = RESOURCEUSAGE_DYNAMIC;
		//	desc.PointParticles = true;
		//	
		//	if(!m_pVB->Create(desc, ve)) return false;
		//	m_pVS = (CShader*)CRenderer::cShaderManager()->CreateShader(P3DST_VERTEX_SHADER, "sprite_renderer", "main", "vsParticle.cg", P3DSS_FROM_FILE);
		//	m_pPS = (CShader*)CRenderer::cShaderManager()->CreateShader(P3DST_PIXEL_SHADER, "sprite_renderer", "main", "psParticle.cg", P3DSS_FROM_FILE);
		//	if(!m_pVS || !m_pVS) return false;
		//	m_matrixHandle = m_pVS->GetHandle("WVP");
		//}
		//else if(m_technique == SPT_POINTSPRITEINSTANCED)
		//{
		//	sVBDesc desc;
		//	desc.VertexCount = m_maxNumParticles;
		//	desc.VertexSize = sizeof(SpriteVertex);
		//	desc.CPUAccess = CPUACCESS_WRITE;
		//	desc.ResourceUsage = RESOURCEUSAGE_DEFAULT;
		//	desc.PointParticles = false;

		//	if(!m_pVB->Create(desc, ve)) return false;
		//	SpriteVertex *v;
		//	m_pVB->Lock((void**)&v, true);
		//	for(unsigned int i=0; i<m_maxNumParticles; i++)
		//	{
		//		v[i].pos.Assign(1.0f, 1.0f, (float)i);
		//	}
		//	m_pVB->UnLock();

		//	m_pVS = (CShader*)CRenderer::cShaderManager()->CreateShader(P3DST_VERTEX_SHADER, "sprite_renderer_instanced", "main", "vsParticleInstanced.cg", P3DSS_FROM_FILE);
		//	m_pPS = (CShader*)CRenderer::cShaderManager()->CreateShader(P3DST_PIXEL_SHADER, "sprite_renderer_instanced", "main", "psParticle.cg", P3DSS_FROM_FILE);

		//	if(!m_pVS || !m_pVS) return false;

		//	m_matrixHandle = m_pVS->GetHandle("WVP");
		//	m_arrayHandle = m_pVS->GetHandle("particles");
		//}
		//else if(m_technique == SPT_BILLBOARD)
		//{
		//	m_pIB = new CIndexBuffer;

		//	sVBDesc desc;
		//	desc.VertexCount = m_maxNumParticles * 4;
		//	desc.VertexSize = sizeof(SpriteVertex);
		//	desc.CPUAccess = CPUACCESS_WRITE;
		//	desc.ResourceUsage = RESOURCEUSAGE_DYNAMIC;
		//	desc.PointParticles = false;

		//	sIBDesc ibdesc;
		//	ibdesc.IndexCount = m_maxNumParticles * 6;
		//	ibdesc.IndexFormat = IBFORMAT_32;
		//	ibdesc.CPUAccess = CPUACCESS_WRITE;
		//	ibdesc.ResourceUsage = RESOURCEUSAGE_DEFAULT;

		//	if(!m_pVB->Create(desc, ve)) return false;
		//	if(!m_pIB->Create(ibdesc)) return false;

		//	DWORD *indexes = 0;
		//	m_pIB->Lock((void**)(&indexes), true);
		//	for(unsigned int i=0; i<m_maxNumParticles; i++)
		//	{
		//		indexes[i*6+0] = i*4+0;
		//		indexes[i*6+1] = i*4+1;
		//		indexes[i*6+2] = i*4+2;
		//		indexes[i*6+3] = i*4+2;
		//		indexes[i*6+4] = i*4+1;
		//		indexes[i*6+5] = i*4+3;
		//	}
		//	m_pIB->UnLock();

		//	m_pVS = (CShader*)CRenderer::cShaderManager()->CreateShader(P3DST_VERTEX_SHADER, "sprite_renderer_billboard", "main", "vsParticle.cg", P3DSS_FROM_FILE);
		//	m_pPS = (CShader*)CRenderer::cShaderManager()->CreateShader(P3DST_PIXEL_SHADER, "sprite_renderer_billboard", "main", "psParticle.cg", P3DSS_FROM_FILE);
		//	if(!m_pVS || !m_pVS) return false;
		//	m_matrixHandle = m_pVS->GetHandle("WVP");
		//}
		//else return false;

		//CON(MSG_INFO, _W("Sprite Renderer created..."));
		return true;
	}

	void CSpriteRenderer::Render()
	{
		//CON(MSG_INFO, "wiiii...");

		if(m_technique == SPT_POINTSPRITE)
		{
			SpriteVertex *v;
			m_pVB->Map((void**)&v);
			for(unsigned int i=0; i<m_maxNumParticles; i++)
			{
				v[i].pos = Vec3(randf(20.0f), randf(20.0f), randf(20.0f));
			}
			m_pVB->Unmap();
		}
		else if(m_technique == SPT_POINTSPRITEINSTANCED)
		{
			for(unsigned int i=0; i<m_maxNumParticles; i++)
			{
				m_spriteInfos[i].pos.Assign(randf(20.0f), randf(20.0f), randf(20.0f));
				m_spriteInfos[i].color = ColorRGBA(randf(1.0f), randf(1.0f), randf(1.0f), 1.0f);
				m_spriteInfos[i].size = randf(1.0f, 2.0f);
			}
			m_pVB->Use();
			Matrix viewproj;
			viewproj = CRenderer::cGraphicsDevice()->GetViewMatrix() * CRenderer::cGraphicsDevice()->GetProjectionMatrix();
			m_pVS->SetHandleValueMatrix(m_matrixHandle, &( m_matrix * viewproj ));
			//m_arrayHandle; m_pVS->SetHandleFloatArray(m_arrayHandle, &m_spriteInfos[0].pos.x, 8);
			m_pVS->Use();
			m_pPS->Use();
			m_pVB->Render(P3DPT_POINTLIST, 0, 10);
			return;
		}
		else if(m_technique == SPT_BILLBOARD)
		{
 			Vec3 lookvector(CRenderer::cGraphicsDevice()->GetViewMatrix()._13, CRenderer::cGraphicsDevice()->GetViewMatrix()._23, CRenderer::cGraphicsDevice()->GetViewMatrix()._33);
			lookvector *= -1;
			lookvector.Normalize();
			Vec3 upvector(0.0f, 1.0f, 0.0f);
			Vec3 rightvector = upvector.Cross(lookvector);
			rightvector.Normalize();
			upvector = lookvector.Cross(rightvector);
			upvector.Normalize();
			Vec2i psize(1, 1);
			Vec3 ul, ur, ll, lr;
			SpriteVertex *v;
			m_pVB->Map((void**)&v);
			for(unsigned int i=0; i<( m_maxNumParticles ); i++)
			{
				Vec3 pos(randf(60.0f), randf(60.0f), randf(60.0f));
				ll = pos - (rightvector - upvector) * (float)psize.x;
				lr = pos + (rightvector + upvector) * (float)psize.x;
				ul = pos - (rightvector + upvector) * (float)psize.x;
				ur = pos + (rightvector - upvector) * (float)psize.x;
				v[i*4+0].pos = ul;
				v[i*4+1].pos = ur;
				v[i*4+2].pos = ll;
				v[i*4+3].pos = lr;
			}
			m_pVB->Unmap();
		}

		

		m_pVB->Use();

		
		Matrix viewproj;
		viewproj = CRenderer::cGraphicsDevice()->GetViewMatrix() * CRenderer::cGraphicsDevice()->GetProjectionMatrix();

		m_pVS->SetHandleValueMatrix(m_matrixHandle, &( m_matrix * viewproj ));

		m_pVS->Use();
		m_pPS->Use();

		if(m_technique == SPT_POINTSPRITE) m_pVB->Render(P3DPT_POINTLIST, 0, m_maxNumParticles);
		else if(m_technique == SPT_BILLBOARD)
		{
			m_pIB->Use();
			m_pVB->RenderIndexed(P3DPT_TRIANGLELIST, 0, m_maxNumParticles);
		}
	}

}