/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "PrimitiveRenderer.h"
#include "symbols.h"
#include "Renderer.h"

namespace P3D
{

	CPrimitiveRenderer::CPrimitiveRenderer()
	{
		m_pVB = 0;
		m_fZincrement = 0.00001f;
		m_PosUVColVS = 0;
		m_PosUVCol3DVS = 0;
		m_PosUVColPS = 0;

		m_fZValue = 0.0f;

		m_pWhiteTex = 0;

		m_shtex = 0;
		m_shmatvp = 0;
	}

	CPrimitiveRenderer::~CPrimitiveRenderer()
	{
		//TODO: Put the PrimitiveRenderer out of the GraphicsDevice
		//CRenderer::cGraphicsManager()->DeleteTexture(m_pWhiteTex);
		SAFE_DELETE(m_pVB);
		SAFE_RELEASE(m_blend);
	}

	bool CPrimitiveRenderer::Init()
	{
		sVBDesc desc;
		desc.VertexCount = 1000;
		desc.VertexSize = sizeof(VPosUVCol);
		desc.CPUAccess = P3D_CPUACCESS_WRITE;
		desc.ResourceUsage = P3D_RESOURCEUSAGE_DYNAMIC;
		desc.PointParticles = false;

		sShaderDesc vsdesc;
		vsdesc.EntryFunction.SetEx(_W("vsmain2D"));
		vsdesc.ShaderName.SetEx(_W("primitive_renderer_vs2D"));
		vsdesc.ShaderFile.SetEx(_W("PrimitiveRenderer"));
		vsdesc.ShaderType = SHADERTYPE_VERTEX_SHADER;
		vsdesc.VertexDescID = CRenderer::cGraphicsManager()->GetVertexDescIDByName(Str(_W("VDPosUVCol")));

		sShaderDesc psdesc;
		psdesc.EntryFunction.SetEx(_W("psmain"));
		psdesc.ShaderName.SetEx(_W("primitive_renderer_ps"));
		psdesc.ShaderFile.SetEx(_W("PrimitiveRenderer"));
		psdesc.ShaderType = SHADERTYPE_PIXEL_SHADER;

		sShaderDesc vs3Ddesc;
		vs3Ddesc.EntryFunction.SetEx(_W("vsmain3D"));
		vs3Ddesc.ShaderName.SetEx(_W("primitive_renderer_vs3D"));
		vs3Ddesc.ShaderFile.SetEx(_W("PrimitiveRenderer"));
		vs3Ddesc.ShaderType = SHADERTYPE_VERTEX_SHADER;
		vs3Ddesc.VertexDescID = CRenderer::cGraphicsManager()->GetVertexDescIDByName(Str(_W("VDPosUVCol")));

		m_pVB = new CVertexBuffer;
		if(!m_pVB->Create(desc)) return false;
		m_PosUVColVS = (CShader*)CRenderer::cShaderManager()->CreateShader(vsdesc);
		m_PosUVColPS = (CShader*)CRenderer::cShaderManager()->CreateShader(psdesc);
		m_PosUVCol3DVS = (CShader*)CRenderer::cShaderManager()->CreateShader(vs3Ddesc);
		if (!m_PosUVColVS || !m_PosUVColPS || !m_PosUVCol3DVS) CON(MSG_ERR_FATAL, _W("Could not load shaders!"));
		m_shtex = m_PosUVColPS->GetHandle("decal");
		m_shmatvp = m_PosUVCol3DVS->GetHandle("ViewProj");

		// create blending object
		sBlendDesc bd;
		bd.BlendEnable[0]=true;
		bd.SrcBlend=BLEND_SRC_ALPHA;
		bd.DestBlend=BLEND_INV_SRC_ALPHA;
		m_blend = CRenderer::cGraphicsDevice()->CreateBlendState(&bd);

		// construct placeholder texture for "failed-to-load" textures
		//BYTE *tdata = new BYTE[64*+64];
		//memset(tdata, 255, sizeof(BYTE) * 64 * 64);
		//m_pWhiteTex = (CTexture*)CRenderer::cGraphicsManager()->CreateTexture(64, 64, TF_L8, tdata);
		m_pWhiteTex = (CTexture*)CRenderer::cGraphicsManager()->GetDafaultTexture();
		//delete[] tdata;

		sTextureList tlist;
		tlist.tex = m_pWhiteTex;
		m_texlist.push_back(tlist);

		m_lines2D.Reserve(1000);
		m_lines3D.Reserve(1000);
		m_triangles2D.Reserve(1000);
		m_triangles3D.Reserve(1000);

		return true;
	}

	inline float ScrToProj(int val, int res)
	{
		return float( (float)val/res * 2 - 1);
	}

	void CPrimitiveRenderer::AddLine(Vec2i pos1, Vec2i pos2, ColorRGBA color)
	{
		// FIXME: 
/*		VPosUVCol v0( Vec3(ScrToProj(pos1.x, CRenderer::cGraphicsWindow()->GetSize().x), ScrToProj(pos1.y, CRenderer::cGraphicsWindow()->GetSize().y), m_fZincrement) , Vec2(0.0f, 0.0f) , color);
		VPosUVCol v1( Vec3(ScrToProj(pos2.x, CRenderer::cGraphicsWindow()->GetSize().x), ScrToProj(pos2.y, CRenderer::cGraphicsWindow()->GetSize().y), m_fZincrement) , Vec2(0.0f, 0.0f) , color);
		
		v0.pos.y *= -1;
		v1.pos.y *= -1;

		m_lines2D.Add(v0);
		m_lines2D.Add(v1);

		m_fZValue += m_fZincrement;*/
	}

	void CPrimitiveRenderer::AddLineRel(Vec2 pos1, Vec2 pos2, ColorRGBA color)
	{
		VPosUVCol v0(Vec3(float(pos1.x), float(pos1.y), m_fZincrement), Vec2(0.0f, 0.0f) , color);
		m_lines2D.Add(v0);
		VPosUVCol v1(Vec3(float(pos2.x), float(pos2.y), m_fZincrement), Vec2(0.0f, 0.0f) , color);
		m_lines2D.Add(v1);

		m_fZValue += m_fZincrement;
	}

	void CPrimitiveRenderer::AddRectangle(Vec2i minPos, Vec2i maxPos, ColorRGBA color, ITexture *texture)
	{
		
		// FIXME:
		/*AddToTL(texture);
		Vec2i res = CRenderer::cGraphicsWindow()->GetSize();*/
		
		// not this... this was commented before, maybe remove?
		/*VertPRPosUVCol v0(Vec3(ScrToProj(minPos.x, res.x), ScrToProj(minPos.y, res.y), m_fZincrement), Vec2(0.0f, 0.0f), color);
		m_triangleVertices.push_back(v0);
		VertPRPosUVCol v1(Vec3(ScrToProj(minPos.x, res.x), ScrToProj(maxPos.y, res.y), m_fZincrement), Vec2(0.0f, 1.0f), color);
		m_triangleVertices.push_back(v1);
		VertPRPosUVCol v2(Vec3(ScrToProj(maxPos.x, res.x), ScrToProj(maxPos.y, res.y), m_fZincrement), Vec2(1.0f, 1.0f), color);
		m_triangleVertices.push_back(v2);
		VertPRPosUVCol v3(Vec3(ScrToProj(maxPos.x, res.x), ScrToProj(maxPos.y, res.y), m_fZincrement), Vec2(1.0f, 1.0f), color);
		m_triangleVertices.push_back(v3);
		VertPRPosUVCol v4(Vec3(ScrToProj(maxPos.x, res.x), ScrToProj(minPos.y, res.y), m_fZincrement), Vec2(1.0f, 0.0f), color);
		m_triangleVertices.push_back(v4);
		VertPRPosUVCol v5(Vec3(ScrToProj(minPos.x, res.x), ScrToProj(minPos.y, res.y), m_fZincrement), Vec2(0.0f, 0.0f), color);
		m_triangleVertices.push_back(v5);*/

		// FIXME:
/*		VPosUVCol v0(Vec3(ScrToProj(minPos.x, res.x), -ScrToProj(minPos.y, res.y), m_fZincrement), Vec2(0.0f, 0.0f), color);
		m_triangles2D.Add(v0);
		VPosUVCol v4(Vec3(ScrToProj(maxPos.x, res.x), -ScrToProj(minPos.y, res.y), m_fZincrement), Vec2(1.0f, 0.0f), color);
		m_triangles2D.Add(v4);
		VPosUVCol v2(Vec3(ScrToProj(maxPos.x, res.x), -ScrToProj(maxPos.y, res.y), m_fZincrement), Vec2(1.0f, 1.0f), color);
		m_triangles2D.Add(v2);
		VPosUVCol v3(Vec3(ScrToProj(maxPos.x, res.x), -ScrToProj(maxPos.y, res.y), m_fZincrement), Vec2(1.0f, 1.0f), color);
		m_triangles2D.Add(v3);
		VPosUVCol v1(Vec3(ScrToProj(minPos.x, res.x), -ScrToProj(maxPos.y, res.y), m_fZincrement), Vec2(0.0f, 1.0f), color);
		m_triangles2D.Add(v1);
		VPosUVCol v5(Vec3(ScrToProj(minPos.x, res.x), -ScrToProj(minPos.y, res.y), m_fZincrement), Vec2(0.0f, 0.0f), color);
		m_triangles2D.Add(v5);

		m_fZValue += m_fZincrement;*/

	}

	void CPrimitiveRenderer::AddRectangleRel(Vec2 minPos, Vec2 maxPos, ColorRGBA color, ITexture *texture)
	{
		AddToTL(texture);
		VPosUVCol v0(Vec3(float(minPos.x), float(minPos.y), m_fZincrement), Vec2(0.0f, 0.0f), color);
		m_triangles2D.Add(v0);
		VPosUVCol v1(Vec3(float(maxPos.x), float(minPos.y), m_fZincrement), Vec2(1.0f, 0.0f), color);
		m_triangles2D.Add(v1);
		VPosUVCol v2(Vec3(float(maxPos.x), float(maxPos.y), m_fZincrement), Vec2(1.0f, 1.0f), color);
		m_triangles2D.Add(v2);
		VPosUVCol v3(Vec3(float(maxPos.x), float(maxPos.y), m_fZincrement), Vec2(1.0f, 1.0f), color);
		m_triangles2D.Add(v3);
		VPosUVCol v4(Vec3(float(minPos.x), float(maxPos.y), m_fZincrement), Vec2(0.0f, 1.0f), color);
		m_triangles2D.Add(v4);
		VPosUVCol v5(Vec3(float(minPos.x), float(minPos.y), m_fZincrement), Vec2(0.0f, 0.0f), color);
		m_triangles2D.Add(v5);

		m_fZValue += m_fZincrement;
	}

	void CPrimitiveRenderer::Render()
	{
		PROFILESAMPLE(PrimitiveRenderer);

		if(!m_triangles2D.Size() && !m_lines2D.Size() && !m_lines3D.Size() && !m_triangles3D.Size()) return; //nothing to draw


		VPosUVCol *v;
		UINT numLines2D = 0;
		UINT numLines3D = 0; 
		UINT numTriangles2D = 0; 
		UINT numTriangles3D = 0;

		
		m_pVB->Map((void**)(&v), true);

		//copy 2D triangles
		unsigned int vnum = 0;
		for(unsigned int i=0; i<m_texlist.size(); i++)
		{
			for(unsigned int y=0; y<m_texlist[i].vertnumbers.size(); y++)
			{
				v[vnum] = m_triangles2D[m_texlist[i].vertnumbers[y]];
				vnum++;
				numTriangles2D++;
			}
		}

		//copy 2DLines
		for(unsigned int i=0; i<m_lines2D.Size(); i++)
		{
			v[i+numTriangles2D] =  m_lines2D[i];
			numLines2D++;
		}

		//copy 3D triangles
		for(unsigned int i=0; i<m_triangles3D.Size(); i++)
		{
			v[i+numTriangles2D+numLines2D] =  m_triangles3D[i];
			numTriangles3D++;
		}

		//copy 3D lines
		for(unsigned int i=0; i<m_lines3D.Size(); i++)
		{
			v[i+numTriangles2D+numLines2D+numTriangles3D] =  m_lines3D[i];
			numLines3D++;
		}

		m_pVB->Unmap();

		m_PosUVColVS->Use();
		m_PosUVColPS->Use();

		//CRenderer::cGraphicsDevice()->SetBlendState(m_blend);

		m_pVB->Use();
		
		//CON(MSG_INFO, _W("pr begin"));
		
		//draw 2D triangles
		if(m_triangles2D.Size())
		{
			unsigned int n = 0;
			for(unsigned int i=0; i<m_texlist.size(); i++) 
			{
				m_PosUVColPS->SetTexture(m_shtex, m_texlist[i].tex);
				m_PosUVColPS->Use();
				//CON(MSG_INFO, _W("pr begin triangle 2d"));
				m_pVB->Render(P3DPT_TRIANGLELIST, n, int(m_texlist[i].vertnumbers.size()/3));
				//CON(MSG_INFO, _W("pr end triangle 2d"));
				n += m_texlist[i].vertnumbers.size();
				m_texlist[i].vertnumbers.clear();
			}
		}
		
		//draw 2D lines
		if(m_lines2D.Size())
		{
			m_PosUVColPS->SetTexture(m_shtex, m_texlist[0].tex);
			m_PosUVColPS->Use();
			//CON(MSG_INFO, _W("pr begin line 2d"));
			m_pVB->Render(P3DPT_TRIANGLELIST, numTriangles2D, int(m_lines2D.Size()/2));
			//CON(MSG_INFO, _W("pr end line 2d"));
		}

		
		static Matrix mat;
		mat = CRenderer::cGraphicsManager()->GetViewProjMatrix();
		m_PosUVCol3DVS->SetHandleValueMatrix(m_shmatvp, &mat);
		m_PosUVCol3DVS->Use();

		//draw 3D triangles
		if(m_triangles3D.Size())
		{
			m_PosUVColPS->SetTexture(m_shtex, m_texlist[0].tex);
			m_PosUVColPS->Use();
			//CON(MSG_INFO, _W("pr begin line"));
			m_pVB->Render(P3DPT_TRIANGLELIST, numTriangles2D+numLines2D, int(m_triangles3D.Size()/3));
			//CON(MSG_INFO, _W("pr end line"));
		}

		//draw 3D lines
		if(m_lines3D.Size())
		{
			m_PosUVColPS->SetTexture(m_shtex, m_texlist[0].tex);
			m_PosUVColPS->Use();
			//CON(MSG_INFO, _W("pr begin line"));
			m_pVB->Render(P3DPT_LINELIST, numTriangles2D+numLines2D+numTriangles3D, int(m_lines3D.Size()/2));
			//CON(MSG_INFO, _W("pr end line"));
		}

		//CON(MSG_INFO, _W("pr end"));

		m_triangles2D.Clear();
		m_lines2D.Clear();
		m_triangles3D.Clear();
		m_lines3D.Clear();
		m_fZincrement = 0.0f;
	}

	void CPrimitiveRenderer::OnLostDevice()
	{
		m_pVB->OnLostDevice();
	}

	void CPrimitiveRenderer::OnResetDevice()
	{
		m_pVB->OnResetDevice();
	}

	void CPrimitiveRenderer::AddToTL(ITexture *tex)
	{
		unsigned int num = m_triangles2D.Size();
		unsigned int index = 0;
		bool newtl = true;
		if(tex)
		{
			
			for(unsigned int i=0; i<m_texlist.size(); i++)
			{
				if(m_texlist[i].tex == tex)
				{
					index = i;
					newtl = false;
					break;
				}
			}

			if(newtl)
			{
				sTextureList tlist;
				tlist.tex = (CTexture*)tex;
				m_texlist.push_back(tlist);
				index = m_texlist.size() - 1; 
			}

			m_texlist[index].vertnumbers.push_back(num + 0);
			m_texlist[index].vertnumbers.push_back(num + 1);
			m_texlist[index].vertnumbers.push_back(num + 2);
			m_texlist[index].vertnumbers.push_back(num + 3);
			m_texlist[index].vertnumbers.push_back(num + 4);
			m_texlist[index].vertnumbers.push_back(num + 5);

		}
		else
		{
			m_texlist[0].vertnumbers.push_back(num + 0);
			m_texlist[0].vertnumbers.push_back(num + 1);
			m_texlist[0].vertnumbers.push_back(num + 2);
			m_texlist[0].vertnumbers.push_back(num + 3);
			m_texlist[0].vertnumbers.push_back(num + 4);
			m_texlist[0].vertnumbers.push_back(num + 5);
		}
	}

	void CPrimitiveRenderer::DeleteFromTextureList(CTexture *texture)
	{
		for(unsigned int i=0; i<m_texlist.size(); i++)
		{
			if(m_texlist[i].tex == texture)
			{
				m_texlist.erase(m_texlist.begin()+i);
				break;
			}
		}
	}

	void CPrimitiveRenderer::AddLines3D(const Vec3 *points, UINT numPoints, const ColorRGBA &color)
	{
		
		m_lines3D.Add(VPosUVCol(points[0], Vec2(0, 0), color));
		for(UINT i=1; i<numPoints-1; i++) 
		{
			m_lines3D.Add(VPosUVCol(points[i], Vec2(0, 0), color));
			m_lines3D.Add(VPosUVCol(points[i], Vec2(0, 0), color));
		}
		m_lines3D.Add(VPosUVCol(points[numPoints-1], Vec2(0, 0), color));
	}

	void CPrimitiveRenderer::AddQuad3D(const Vec3 &pos1, const Vec3 &pos2, ColorRGBA &color, ITexture *texture)
	{
		m_triangles3D.Add( VPosUVCol(Vec3(float(pos1.x), float(pos1.y), pos1.z), Vec2(0.0f, 0.0f), color) );
		m_triangles3D.Add( VPosUVCol(Vec3(float(pos2.x), float(pos1.y), pos1.z), Vec2(1.0f, 0.0f), color) );
		VPosUVCol v2(Vec3(float(pos2.x), float(pos2.y), pos2.z), Vec2(1.0f, 1.0f), color);
		m_triangles3D.Add(v2);
		VPosUVCol v3(Vec3(float(pos2.x), float(pos2.y), pos2.z), Vec2(1.0f, 1.0f), color);
		m_triangles3D.Add(v3);
		VPosUVCol v4(Vec3(float(pos1.x), float(pos2.y), pos2.z), Vec2(0.0f, 1.0f), color);
		m_triangles3D.Add(v4);
		VPosUVCol v5(Vec3(float(pos1.x), float(pos1.y), pos1.z), Vec2(0.0f, 0.0f), color);
		m_triangles3D.Add(v5);
	}

}
