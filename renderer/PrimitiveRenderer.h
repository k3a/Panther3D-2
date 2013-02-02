/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#include "VertexBuffer.h"
#include <vector>
#include "IPrimitiveRenderer.h"
#include "ShaderManager.h"
#include "VertexTypes.h"

#include "IGraphicsDevice.h"

namespace P3D
{

	struct sTextureList
	{
		CTexture *tex;
		std::vector<unsigned int> vertnumbers;
	};
	

	class CPrimitiveRenderer : public IPrimitiveRenderer
	{
	public:
		CPrimitiveRenderer();
		~CPrimitiveRenderer();
		bool Init();
		void AddLine(Vec2i pos1, Vec2i pos2, ColorRGBA color);
		void AddRectangle(Vec2i minPos, Vec2i maxPos, ColorRGBA color, ITexture* texture = 0);
		void AddLineRel(Vec2 pos1, Vec2 pos2, ColorRGBA color);
		void AddRectangleRel(Vec2 minPos, Vec2 maxPos, ColorRGBA color, ITexture* texture = 0);
		/*void AddTriangle(Vec2i pos1, Vec2i pos2, Vec2i pos3,ColorRGBA color);
		void AddTriangleRel(Vec2i pos1, Vec2i pos2, Vec2i pos3,ColorRGBA color);
		void AddTexturedRectangle(Vec2i minPos, Vec2i maxPos, ColorRGBA color, ITexture *texture);
		void AddTexturedRectangleRel(Vec2 minPos, Vec2 maxPos, ColorRGBA color, ITexture *texture);*/
		
		///Render a bunch of lines in 3D with a given color
		void AddLines3D(const Vec3 *points, UINT numPoints, const ColorRGBA &color); 

		void AddQuad3D(const Vec3 &pos1, const Vec3 &pos2, ColorRGBA &color, ITexture *texture = 0);
		
		void Render();

		void OnLostDevice();
		void OnResetDevice();
		void DeleteFromTextureList(CTexture *texture);

	private:

		void AddToTL(ITexture *tex);

		CVertexBuffer *m_pVB;
		tArray<VPosUVCol> m_lines2D;
		tArray<VPosUVCol> m_triangles2D;
		tArray<VPosUVCol> m_lines3D;
		tArray<VPosUVCol> m_triangles3D;
		float m_fZValue;
		float m_fZincrement;

		CShader *m_PosUVColVS;
		CShader *m_PosUVColPS;
		CShader *m_PosUVCol3DVS;
		std::vector<sTextureList> m_texlist;
		std::vector<sTextureList> m_blendtexlist;

		IBlendState *m_blend;
		CTexture* m_pWhiteTex;
		ShaderHandle m_shtex;
		ShaderHandle m_shmatvp;
	};



};
