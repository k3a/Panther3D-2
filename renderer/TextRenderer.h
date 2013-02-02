 /* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
 * This contents may be used and/or copied only with the written permission or
 * terms and conditions stipulated in agreement/contract.
 -----------------------------------------------------------------------------
 Authors: am!go
 */
 
 #pragma once

#include <vector>
#include <ft2build.h>
#include <freetype/freetype.h> 
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#include FT_GLYPH_H
#include <freetype/ftimage.h>
#include FT_IMAGE_H
 
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "IGraphicsDevice.h"

#include "ITextRenderer.h"
 
 namespace P3D
 {
 
 	struct VertTRPosUVCol
 	{
 		VertTRPosUVCol()
 		{
 
 		}
 
 		VertTRPosUVCol(Vec3 position, Vec2 tex, ColorRGBA color)
 		{
 			pos = position;
 			uv = tex;
			col = color;
 		}
 
 		Vec3 pos;
 		Vec2 uv;
		ColorRGBA col;
 	};

	struct trCharacter
	{
		Vec3 pos[4];
		Vec2 uv[4];
		ColorRGBA col;
		float adv;
	};

	class CFont
	{
	public:
		CFont()
		{
			m_face = 0;
			m_pTexture = 0;
			m_pCharacters = 0;
			m_maxHeight=0;
		}

		~CFont()
		{
			SAFE_DELETE_ARRAY(m_pCharacters);
			//texture deleted in CTextRenderer destructor
			if(m_face) FT_Done_Face( m_face );
		}

		FT_Face m_face;
		CTexture *m_pTexture;
		trCharacter *m_pCharacters;
		std::vector<trCharacter> m_usedCharacters;
		int m_maxHeight;
	};

	class CTextRenderer : public ITextRenderer
 	{
 	public:
 		CTextRenderer();
 		~CTextRenderer();
 		bool Create();
		void Print(const wchar* string, Vec2i position, ColorRGBA color, Vec2i *size = NULL, Rect *restrict=NULL);
		void Print(const wchar* string, Vec2i position, ColorRGBA color, FontHandle font, Vec2i *size = NULL, Rect *restrict=NULL);
		void CalcSize(const wchar* string, Vec2i *size);
		void CalcSize(const wchar* string, FontHandle font, Vec2i *size);
		int GetFontHeight(FontHandle font);
		void Render();
		FontHandle CreateFont(const wchar* filename, unsigned int size);

		void OnLostDevice();
		void OnResetDevice();

 	private:
 		static const unsigned int MAX_CHARS = 2048;
 		static const unsigned int MAX_GLYPHS = 94;
 		FT_Library m_library;
 		CVertexBuffer *m_pVB;
		CIndexBuffer  *m_pIB; 
 		VertexElement VETRPosUV[4];
		CShader *m_PosUVColVS;
		CShader *m_PosUVColPS;
		ShaderHandle m_shtex;
		std::vector<CFont*> m_fonts;
		IBlendState *m_blend;
		FontHandle m_defaultFont;
 
 	};
 
 
 
 };