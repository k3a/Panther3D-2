 /* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
 * This contents may be used and/or copied only with the written permission or
 * terms and conditions stipulated in agreement/contract.
 -----------------------------------------------------------------------------
 Authors: am!go

 THX to http://videogameprogramming.blogspot.com for some 
 information about freetype implementation.
 */
#include "precompiled.h"

 #include "TextRenderer.h"
 #include "symbols.h"
 #include "Renderer.h"
 
 namespace P3D
 {
 
 	inline int next_p2(int a)
 	{
 		int x = 2;
 		while(x < a) x *= 2;
 		return x;
 	}
 
 	CTextRenderer::CTextRenderer()
 	{
 		m_pVB = 0;
		m_pIB = 0;
		m_library = 0;
		m_PosUVColVS = 0;
		m_PosUVColPS = 0;
		m_defaultFont = 0;
 	}
 
 	CTextRenderer::~CTextRenderer()
 	{
		for(unsigned int i=0; i< m_fonts.size(); i++) 
		{
			CRenderer::cGraphicsManager()->DeleteTexture(m_fonts[i]->m_pTexture);
			delete m_fonts[i];
		}
		SAFE_DELETE(m_pVB);
		SAFE_DELETE(m_pIB);
		SAFE_RELEASE(m_blend);
 		if(m_library) FT_Done_FreeType(m_library);
 	}
 
 	bool CTextRenderer::Create()
 	{

		if(FT_Init_FreeType(&m_library)) 
		{
			CON(MSG_ERR, _W("FT_Init_FreeType failed!"));
			return false;
		}
 
 		m_pVB = new CVertexBuffer;
		m_pIB = new CIndexBuffer;

		sVBDesc desc;
		desc.VertexCount = MAX_CHARS * 4;
		desc.VertexSize = sizeof(VertTRPosUVCol);
		desc.CPUAccess = P3D_CPUACCESS_WRITE;
		desc.ResourceUsage = P3D_RESOURCEUSAGE_DYNAMIC;
		desc.PointParticles = false;

		sIBDesc ibdesc;
		ibdesc.IndexCount = MAX_CHARS*6;
		ibdesc.IndexFormat = IBFORMAT_32;
		ibdesc.CPUAccess = P3D_CPUACCESS_NONE;
		ibdesc.ResourceUsage = P3D_RESOURCEUSAGE_IMMUTABLE;

		DWORD *indexes = new DWORD[MAX_CHARS*6];
		for(unsigned int i=0; i<MAX_CHARS; i++)
 		{
 			indexes[i*6+0] = i*4+3;
 			indexes[i*6+1] = i*4+2;
 			indexes[i*6+2] = i*4+1;
 			indexes[i*6+3] = i*4+1;
 			indexes[i*6+4] = i*4+0;
 			indexes[i*6+5] = i*4+3;
 		}

 		if(!m_pVB->Create(desc)) return false;
		if(!m_pIB->Create(ibdesc, indexes)) return false;

		delete[] indexes;
 
		m_defaultFont = CTextRenderer::CreateFont(_W("arial.ttf"), 16);

		sShaderDesc vsdesc;
		vsdesc.EntryFunction.SetEx(_W("main"));
		vsdesc.ShaderName.SetEx(_W("text_renderer"));
		vsdesc.ShaderFile.SetEx(_W("vsPosUV"));
		vsdesc.ShaderType = SHADERTYPE_VERTEX_SHADER;
		vsdesc.VertexDescID = CRenderer::cGraphicsManager()->GetVertexDescIDByName(Str(_W("VDPosUVCol")));

		sShaderDesc psdesc;
		psdesc.EntryFunction.SetEx(_W("main"));
		psdesc.ShaderName.SetEx(_W("text_renderer"));
		psdesc.ShaderFile.SetEx(_W("psPosUV"));
		psdesc.ShaderType = SHADERTYPE_PIXEL_SHADER;
 		
		//m_PosUVColVS = (CShader*)CRenderer::cShaderManager()->CreateShader(vsdesc);
		//m_PosUVColPS = (CShader*)CRenderer::cShaderManager()->CreateShader(psdesc);
		//m_shtex = m_PosUVColPS->GetHandle("decal");

		// create blending object
		sBlendDesc bd;
		bd.BlendEnable[0]=true;
		bd.SrcBlend=BLEND_SRC_ALPHA;
		bd.DestBlend=BLEND_INV_SRC_ALPHA;
		m_blend = CRenderer::cGraphicsDevice()->CreateBlendState(&bd);
 		
  		return true;
 	}

	inline float ScrToProj(int val, int res)
	{
		return float( (float)val/res * 2 - 1);
	}

	void CTextRenderer::Print(const wchar *string, P3D::Vec2i position, P3D::ColorRGBA color, Vec2i *size, Rect *restrict)
	{
		Print(string, position, color, m_defaultFont, size, restrict);
	}

	void CTextRenderer::Print(const wchar *string, P3D::Vec2i position, ColorRGBA color, FontHandle font, Vec2i *size, Rect *restrict)
	{
		if(!font || m_fonts.size() < font) return;
		
		// FIXME
	/*	unsigned int len = wstrlen(string);

		int currentX = 0;

		font -= 1;  //FontHandle 0 means error by default so we've got to decrease it

		Vec2i scrSize = CRenderer::cGraphicsWindow()->GetSize();
		if( position.y > scrSize.y ) return; // check if text is outside the bottom screen edge

		position.y = scrSize.y - position.y - m_fonts[font]->m_maxHeight; // flip y coordinate so y=0 will be top

		//if (len>100) len=100;
		unsigned int y = 0;
		trCharacter ch;
		for(unsigned int i=0; i<len; i++)
		{
			y = string[i] - 32;

			if (y>=MAX_GLYPHS) continue; // cannot draw so high character

			ch = m_fonts[font]->m_pCharacters[y];

			// we have line restriction
			if (restrict)
			{
				if (position.x+currentX < restrict->x1) // we are before restriction
				{
					currentX += (int)m_fonts[font]->m_pCharacters[y].adv;
					continue;
				}
				else if (position.x+currentX + (int)m_fonts[font]->m_pCharacters[y].adv > restrict->x2) // we are after restriction
				{
					break;
				}
			}

			ch.pos[0] = Vec3(ScrToProj(int(ch.pos[0].x + currentX + position.x), scrSize.x), ScrToProj(int(ch.pos[0].y + position.y), scrSize.y), 0.0f);
			ch.pos[1] = Vec3(ScrToProj(int(ch.pos[1].x + currentX + position.x), scrSize.x), ScrToProj(int(ch.pos[1].y + position.y), scrSize.y), 0.0f);
			ch.pos[2] = Vec3(ScrToProj(int(ch.pos[2].x + currentX + position.x), scrSize.x), ScrToProj(int(ch.pos[2].y + position.y), scrSize.y), 0.0f);
			ch.pos[3] = Vec3(ScrToProj(int(ch.pos[3].x + currentX + position.x), scrSize.x), ScrToProj(int(ch.pos[3].y + position.y), scrSize.y), 0.0f);

			ch.col = color;
			m_fonts[font]->m_usedCharacters.push_back(ch);

			currentX += (int)m_fonts[font]->m_pCharacters[y].adv;

			//Check if the next letter is outside the screen.
			if(currentX + position.x > scrSize.x) break;

		}

		if(size) *size = Vec2i(currentX, m_fonts[font]->m_maxHeight);*/

	}

	void CTextRenderer::CalcSize(const wchar* string, Vec2i *size)
	{
		CalcSize(string, m_defaultFont, size);
	}

	void CTextRenderer::CalcSize(const wchar* string, FontHandle font, Vec2i *size)
	{
		if(m_fonts.size() == 0) return;

		unsigned int len = wstrlen(string);
		unsigned int currentX = 0;

		font -= 1;  //FontHandle 0 means error by default so we've got to decrease it

		for(unsigned int i=0; i<len; i++)
		{
			unsigned int y = string[i] - 32;
			if (y>=MAX_GLYPHS) continue; // cannot draw so high character
			currentX += (unsigned int)m_fonts[font]->m_pCharacters[y].adv;
		}

		if(size) *size = Vec2i(currentX, m_fonts[font]->m_maxHeight);
	}

	int CTextRenderer::GetFontHeight(FontHandle font)
	{
		if(m_fonts.size() == 0) return 0;
		font -= 1;  //FontHandle 0 means error by default so we've got to decrease it
		return m_fonts[font]->m_maxHeight;
	}

	void CTextRenderer::Render()
	{	
		return; //I've got to adjust the shaders
		PROFILESAMPLE(TextRendering);
		
		if(m_fonts.size() == 0) return;

		VertTRPosUVCol *v;

		
		CRenderer::cGraphicsDevice()->SetBlendState(m_blend);

		// draw all text in MAX_CHARS batches
		for(unsigned int f=0; f<m_fonts.size(); f++)
		{
			unsigned int charsDrawn=0;
			do {
				// determine num. of chars to drawn. Maximum is MAX_CHARS
				unsigned int charsToDraw;
				if (m_fonts[f]->m_usedCharacters.size()-charsDrawn<MAX_CHARS)
					charsToDraw = m_fonts[f]->m_usedCharacters.size()-charsDrawn;
				else
					charsToDraw = MAX_CHARS;

				// Fill in vertex buffer with charsDrawn offset and charsToDraw

				PROFILESAMPLE(TextRenderingVB);
				PROFILESAMPLE(TextRenderingVBLock);
				m_pVB->Map((void**)(&v), true);
				ENDPROFILESAMPLE(TextRenderingVBLock);
				unsigned int currID=0;

				for(unsigned int i=charsDrawn; i<charsDrawn+charsToDraw; i++)
				{
					v[currID*4+0] = VertTRPosUVCol(m_fonts[f]->m_usedCharacters[i].pos[0], m_fonts[f]->m_usedCharacters[i].uv[0], m_fonts[f]->m_usedCharacters[i].col);
					v[currID*4+1] = VertTRPosUVCol(m_fonts[f]->m_usedCharacters[i].pos[1], m_fonts[f]->m_usedCharacters[i].uv[1], m_fonts[f]->m_usedCharacters[i].col);
					v[currID*4+2] = VertTRPosUVCol(m_fonts[f]->m_usedCharacters[i].pos[2], m_fonts[f]->m_usedCharacters[i].uv[2], m_fonts[f]->m_usedCharacters[i].col);
					v[currID*4+3] = VertTRPosUVCol(m_fonts[f]->m_usedCharacters[i].pos[3], m_fonts[f]->m_usedCharacters[i].uv[3], m_fonts[f]->m_usedCharacters[i].col);
					currID++;
				}

				PROFILESAMPLE(TextRenderingVBUnLock);
				m_pVB->Unmap();
				ENDPROFILESAMPLE(TextRenderingVBUnLock);
				ENDPROFILESAMPLE(TextRenderingVB);

				// Draw current batch

				unsigned int numind = 0;

				m_PosUVColPS->SetTexture(m_shtex, m_fonts[f]->m_pTexture);
				m_PosUVColVS->Use();
				m_PosUVColPS->Use();


				m_pIB->Use();
				m_pVB->Use();

				if(m_fonts[f]->m_usedCharacters.size()) 
				{
						//CON(MSG_INFO, _W("textrenderer begin"));
						m_pVB->RenderIndexed(P3DPT_TRIANGLELIST, numind, charsToDraw*2);
						//CON(MSG_INFO, _W("textrenderer end"));
						numind += charsToDraw*6;
				}

				charsDrawn+=charsToDraw; // add current batch to all chars drawn
			} while (m_fonts[f]->m_usedCharacters.size() > charsDrawn);
			m_fonts[f]->m_usedCharacters.clear(); // all characters using font f was drawn
		}

/*  OLD VERSION OF RENDER CODE -> NOT USING BATCH
    WARNING m_usedChars was removed. m_fonts[f]->m_usedCharacters.size() can be > MAX_CAHARS
		if(m_fonts.size() == 0) return;

		VertTRPosUVCol *v;

		m_pVB->Use();
		m_pIB->Use();
		CRenderer::cGraphicsDevice()->SetBlendState(m_blend);

		m_pVB->Lock((void**)(&v), true);
		unsigned int currID=0;
		for(unsigned int f=0; f<m_fonts.size(); f++)
		{
			for(unsigned int i=0; i<m_fonts[f]->m_usedCharacters.size(); i++)
			{
				v[currID*4+0] = VertTRPosUVCol(m_fonts[f]->m_usedCharacters[i].pos[0], m_fonts[f]->m_usedCharacters[i].uv[0], m_fonts[f]->m_usedCharacters[i].col);
				v[currID*4+1] = VertTRPosUVCol(m_fonts[f]->m_usedCharacters[i].pos[1], m_fonts[f]->m_usedCharacters[i].uv[1], m_fonts[f]->m_usedCharacters[i].col);
				v[currID*4+2] = VertTRPosUVCol(m_fonts[f]->m_usedCharacters[i].pos[2], m_fonts[f]->m_usedCharacters[i].uv[2], m_fonts[f]->m_usedCharacters[i].col);
				v[currID*4+3] = VertTRPosUVCol(m_fonts[f]->m_usedCharacters[i].pos[3], m_fonts[f]->m_usedCharacters[i].uv[3], m_fonts[f]->m_usedCharacters[i].col);
				currID++;
			}
		}

		m_pVB->UnLock();
		m_pVB->Use();
		m_pIB->Use();
		unsigned int numvert = 0;
		unsigned int numind = 0;
		CRenderer::cGraphicsDevice()->SetBlendState(m_blend);
		for(unsigned int f=0; f<m_fonts.size(); f++)
		{
			m_PosUVColPS->SetHandleValueTexture(m_shtex, m_fonts[f]->m_pTexture);
			m_PosUVColVS->Use();
			m_PosUVColPS->Use();
			if(m_fonts[f]->m_usedCharacters.size()) 
			{
				m_pVB->RenderIndexed(P3DPT_TRIANGLELIST, numind, true, m_fonts[f]->m_usedCharacters.size()*2);
				numind += m_fonts[f]->m_usedCharacters.size()*6; //this is not reacting
			}
			m_fonts[f]->m_usedCharacters.clear();
		}*/
	}

	FontHandle CTextRenderer::CreateFont(const wchar *filename, unsigned int size)
	{
		IFileSystem* pFS = CRenderer::mEngine()->mFilesystem();

		CFont *font = new CFont;

		font->m_usedCharacters.reserve(MAX_CHARS);

		// load font using filesystem
		wchar path[P3DMAX_PATH];
		wsprintf(path, P3DMAX_PATH-1, _W("fonts/%s"), filename);
		BYTE *pData; ULONG fsize;
		FSFILE* fp = pFS->Load(path, pData, fsize, false);
		if (!fp)
		{
			CON(MSG_ERR, _W("Can't open %s font file from data/fonts directory!"), filename);
			return 0;
		}

		// create new FT font face
		if(FT_New_Memory_Face(m_library, pData, fsize, 0, &font->m_face))
		{
			CON(MSG_ERR, _W("FT_New_Memory_Face failed! Possibly unsupported font file."));
			return 0; //0 means failed
		}


		CON(MSG_INFO, _W("Font file %s loaded. Available faces: %d."), filename, font->m_face->num_faces);

		
		FT_Set_Char_Size( font->m_face, size * 64, size * 64, 96, 96 );
		//FT_Set_Char_Size( font->m_face, size * 64, size * 64, CRenderer::cGraphicsWindow()->GetSize().x, CRenderer::cGraphicsWindow()->GetSize().y );
		FT_Glyph Glyphs[ MAX_GLYPHS ];

		int TotalWidth = 0;
		int MaxHeight = 0;
		int PAD = 0;

		font->m_pCharacters = new trCharacter[MAX_GLYPHS];

		for( unsigned int i = 0; i < MAX_GLYPHS; i++ )
		{
			FT_Load_Glyph(font->m_face, FT_Get_Char_Index( font->m_face, 32 + i ),	FT_LOAD_DEFAULT);
			FT_Get_Glyph(font->m_face->glyph, &Glyphs[i]);
			FT_Glyph_To_Bitmap( &Glyphs[ i ], FT_RENDER_MODE_NORMAL, 0, 1 );
			FT_Bitmap &Bitmap = ((FT_BitmapGlyph)Glyphs[ i ])->bitmap;
			TotalWidth += Bitmap.width + 2 * PAD;
			MaxHeight = max( MaxHeight, Bitmap.rows + 2 * PAD );
			font->m_pCharacters[i].adv = font->m_face->glyph->advance.x / 64.0f;
		}
		font->m_maxHeight = MaxHeight;

		unsigned int FontMapWidth = next_p2(TotalWidth);
		unsigned int FontMapHeight = next_p2(MaxHeight);

		
		BYTE *pixels = new BYTE[FontMapWidth * FontMapHeight];
		memset(pixels, 0, sizeof(BYTE) * FontMapWidth * FontMapHeight); //default white color

		

		TotalWidth = 0;

		for( unsigned int i = 0; i < MAX_GLYPHS; i++ )
		{
			FT_BitmapGlyph BitmapGlyph = (FT_BitmapGlyph)Glyphs[ i ];
			const FT_Bitmap &Bitmap = BitmapGlyph->bitmap;
			unsigned int Pitch = abs( Bitmap.pitch );
			for( unsigned int y = 0; y < unsigned int(Bitmap.rows); y++ )
			{
				for( unsigned int x = 0; x < unsigned int(Bitmap.width); x++ )
				{
					if(Bitmap.buffer == NULL || x >= unsigned int(Bitmap.width) ||	y >= unsigned int(Bitmap.rows))
					{
						// If we're outside the bounds of the FreeType bitmap then fill with black.
						pixels[ (y + PAD) * FontMapWidth + TotalWidth + x + PAD ] = 255;
					}
					else
					{
						// Otherwise copy the FreeType bits.
						pixels[ (y + PAD) * FontMapWidth + TotalWidth + x + PAD ] = Bitmap.buffer[ y * Pitch + x ];
					}
				}
			}

			//some dx stuff required
			float plusy = 0.0f;
			float plusx = 0.0f;
			#if defined(_DX) && _DX == 9
			plusy = 0.5f / (FontMapHeight-1);
			plusx = 0.5f / (FontMapWidth-1);
			#endif

			font->m_pCharacters[i].uv[0] = Vec2( float(TotalWidth + PAD) / FontMapWidth + plusx,					float(Bitmap.rows + PAD) / FontMapHeight + plusy); 
			font->m_pCharacters[i].uv[1] = Vec2( float(TotalWidth + Bitmap.width + PAD) / FontMapWidth + plusx,		float(Bitmap.rows + PAD) / FontMapHeight + plusy); 
			font->m_pCharacters[i].uv[2] = Vec2( float(TotalWidth + Bitmap.width + PAD) / FontMapWidth + plusx,		plusy ); 
			font->m_pCharacters[i].uv[3] = Vec2( float(TotalWidth + PAD) / FontMapWidth + plusx,					plusy ); 

			font->m_pCharacters[i].pos[0].Assign( float(BitmapGlyph->left),					float(BitmapGlyph->top - Bitmap.rows), 0.0f );
			font->m_pCharacters[i].pos[1].Assign( float(BitmapGlyph->left + Bitmap.width),	float(BitmapGlyph->top - Bitmap.rows), 0.0f );
			font->m_pCharacters[i].pos[2].Assign( float(BitmapGlyph->left + Bitmap.width),	float(BitmapGlyph->top), 0.0f );
			font->m_pCharacters[i].pos[3].Assign( float(BitmapGlyph->left),					float(BitmapGlyph->top), 0.0f );

			TotalWidth += Bitmap.width + PAD * 2;
		}

		sTextureDesc desc;
		desc.ArraySize = 1;
		desc.AutoGenerateMips = true;
		desc.BindFlag = P3D_TEXBINDFLAG_TEXTURE2D;
		desc.CPUAccess = P3D_CPUACCESS_NONE;
		desc.Format = P3D_FORMAT_L8_UNORM;
		desc.MipLevels = 0;
		//desc.ResourceUsage = P3D_RESOURCEUSAGE_IMMUTABLE; //FIXME: causes error
		desc.ResourceUsage = P3D_RESOURCEUSAGE_DEFAULT;
		desc.Width = FontMapWidth;
		desc.Height = FontMapHeight;
		
		font->m_pTexture = (CTexture*)CRenderer::cGraphicsManager()->CreateTexture(desc, pixels, FontMapWidth * FontMapHeight);
		delete[] pixels;

		for( unsigned int i = 0; i < MAX_GLYPHS; i++ )
		{
			FT_Done_Glyph( Glyphs[ i ] );
		}

		pFS->UnLoad(fp, pData); // clear memory of loaded font file

		m_fonts.push_back(font);

		return m_fonts.size();
		return true;
	}

	void CTextRenderer::OnLostDevice()
	{
		m_pVB->OnLostDevice();
		m_pIB->OnLostDevice();
	}

	void CTextRenderer::OnResetDevice()
	{
		m_pVB->OnResetDevice();	//VB is dynamic
		if(m_pIB->OnResetDevice())	//if IB is reseted
		{
			DWORD *indexes = 0;
			m_pIB->Map((void**)(&indexes), true);
			for(unsigned int i=0; i<MAX_CHARS; i++)
			{
				indexes[i*6+0] = i*4+3;
				indexes[i*6+1] = i*4+2;
				indexes[i*6+2] = i*4+1;
				indexes[i*6+3] = i*4+1;
				indexes[i*6+4] = i*4+0;
				indexes[i*6+5] = i*4+3;
			}
			m_pIB->Unmap();
		}
	}

 };