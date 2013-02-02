/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#ifdef _OGL

#include "DeviceWrapper.h"
#include "symbols.h"
#include "../Renderer.h" // because of CON macro
#include "glex.h"
#include "../VertexBuffer.h"
#include "GLHelperFunctions.h"
//#include <SDL/SDL_video.h>

namespace P3D
{

	CDeviceWrapper::CDeviceWrapper()
	{
		m_hash=0;
		m_frameBuffer = 0;
	}

	CDeviceWrapper::~CDeviceWrapper()
	{
		Drop();
	}

	void SetVSync(int interval=1)
	{
#ifdef WIN32
		typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
		PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

		const char *extensions = (const char *)glGetString( GL_EXTENSIONS );

		if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
			return;
		else
		{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

			if( wglSwapIntervalEXT )
				wglSwapIntervalEXT(interval);
		}
#endif
	}

	bool CDeviceWrapper::Create(unsigned int width, unsigned int height, bool fullscreen, void* window)
	{
		CON(MSG_INFO, _W(" ...creating OpenGL device with backbuffer of %d x %d %s"), width, height, fullscreen?"fullscreen":"windowed");


		CRenderer::instance()->EV_Loading(_W("Preparing OpenGL Device..."));

		if (!window) CON(MSG_ERR_FATAL, _W("Calling Create() with window=NULL! Device must be initialized with window!") );

		/////////////////////////////////
		GLuint		PixelFormat;			// Holds The Results After Searching For A Match

		static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			window->GetBPP(),							// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			1,											// Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			24,											// 24Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};


		if (!(m_hDC=GetDC((HWND)window)))							// Did We Get A Device Context?
		{
			CON(MSG_ERR_FATAL, _W("Cannot create an OpenGL Device Context."));
			return false;
		}

		if (!(PixelFormat=ChoosePixelFormat(m_hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			CON(MSG_ERR_FATAL, _W("Cannot find a suitable PixelFormat.") );
			return false;
		}

		if(!SetPixelFormat(m_hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		{
			CON(MSG_ERR_FATAL, _W("Cannot set the PixelFormat.") );
			return false;
		}


		if (!(m_hRC=wglCreateContext(m_hDC)))				// Are We Able To Get A Rendering Context?
		{
			CON(MSG_ERR_FATAL, _W("Cannot create an OpenGL Rendering Context!") );
			return false;
		}


		if(!wglMakeCurrent(m_hDC,m_hRC))					// Try To Activate The Rendering Context
		{
			CON(MSG_ERR_FATAL, _W("Cannot activate the OpenGL Rendering Context!.") );
			return false;
		}

		/////////////////////////////////////////

		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");	
		if (wglChoosePixelFormatARB) 
		{
			int		NewPixelFormat;
			int		valid;
			UINT	numFormats;
			float	fAttributes[] = {0,0};

			int iAttributes[] =
			{
				WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
				WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
				WGL_COLOR_BITS_ARB,window->GetBPP(),
				WGL_ALPHA_BITS_ARB,8,
				WGL_DEPTH_BITS_ARB,24,
				WGL_STENCIL_BITS_ARB,0,
				WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
				//WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
				//WGL_SAMPLES_ARB,4,
				0,0
			};

			Drop();
			
			if (!(m_hDC=GetDC((HWND)window)))							// Did We Get A Device Context?
			{
				CON(MSG_ERR_FATAL, _W("Cannot create an OpenGL Device Context."));
				return false;
			}

			valid = wglChoosePixelFormatARB(m_hDC,iAttributes,fAttributes,1,&NewPixelFormat,&numFormats);

			if (valid && numFormats >= 1)
			{

				if(!SetPixelFormat(m_hDC,NewPixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
				{
					CheckGLErrors(_W("PixelFormat"));
					CON(MSG_ERR_FATAL, _W("Cannot set the PixelFormat EX!!!.") );
					return false;
				}

				if (!(m_hRC=wglCreateContext(m_hDC)))				// Are We Able To Get A Rendering Context?
				{
					CON(MSG_ERR_FATAL, _W("Cannot create an OpenGL Rendering Context!") );
					return false;
				}


				if(!wglMakeCurrent(m_hDC,m_hRC))					// Try To Activate The Rendering Context
				{
					CON(MSG_ERR_FATAL, _W("Cannot activate the OpenGL Rendering Context!.") );
					return false;
				}

			}
			else CON(MSG_ERR_FATAL, _W("No valid Pixel Format!") );
		}

		/////////////////////////////////////////

		glViewport(0,0,width,height);

		/////////////////////////////////

		CON(MSG_INFO, _W(" ...using OpenGL version %s"), _A2W(glGetString(GL_VERSION)) );
		CON(MSG_INFO, _W(" ...using renderer %s"), _A2W(glGetString(GL_RENDERER)) );
		m_hash = Str(_A2W(glGetString(GL_RENDERER))).Hash256();

		//Setup OpenGL. The hard work is already done by SDL.
		glShadeModel(GL_SMOOTH);
		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glEnable( GL_TEXTURE_2D );
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE); //We compute the size in VS.

		// back-face culling
		glEnable(GL_CULL_FACE);
		glCullFace( GL_FRONT );

		// disable v-sync, TODO: later based on settings
		SetVSync(0);

		// load extensions
		LoadGLExt();

		// get device capabilities
		GLint texSize; 
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
		m_cardInfo.maxTextureResolution = texSize;
		m_cardInfo.nonPow2Textures = IsExtensionSupported("GL_ARB_texture_non_power_of_two");
		m_cardInfo.supports3DcCompression = IsExtensionSupported("GL_ATI_texture_compression_3dc");
		// TODO: !HIGH PRIORITY: check for all m_cardInfo capabilities

		int tmpInt;
		CON(MSG_INFO, _W("  Renderer capabilities:"));
		CON(MSG_INFO, _W("   * Max texture size: %d"), texSize);
		CON(MSG_INFO, _W("   * Non power-of-two textures: %s"), m_cardInfo.nonPow2Textures?_W("yes"):_W("no"));
		CON(MSG_INFO, _W("   * Multisampling and ATC: %s"), IsExtensionSupported("GL_ARB_multisample")?_W("yes"):_W("no"));
		//CON(MSG_INFO, _W("   * ATC???: %s"), IsExtensionSupported("GL_NV_multisample_coverage")?_W("yes"):_W("no"));
		glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &tmpInt);
		CON(MSG_INFO, _W("   * Max render targets: %d"), tmpInt);
		CON(MSG_INFO, _W("   * DXT compression: %s"), IsExtensionSupported("GL_EXT_texture_compression_s3tc")?_W("yes"):_W("no"));

		// create FrameBuffer
		glGenFramebuffersEXT(1, &m_frameBuffer);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);

		CheckGLErrors(_W("Create Device Wrapper"));

		return true;
	}


	void CDeviceWrapper::Clear(ColorRGB color)
	{
		glClearColor( color.r, color.g, color.b, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}

	void CDeviceWrapper::BeginScene(IWindow *wndTarget)
	{
		if(wndTarget && m_win != wndTarget) m_win = wndTarget;
		//nothind to do so far
		//vb.Use();
		//vb.Render(P3DPT_TRIANGLELIST, 0, 1);
	}

	void CDeviceWrapper::EndScene()
	{
		// check for valid IWindow
		if (!m_win) {
			CON(MSG_ERR, _W("You must specify IWindow pointer in BeginScene() first!"));
			return;
		}

		if (!m_win->IsNativeWindow())
		{
			//SDL_GL_SwapBuffers();
			PROFILESAMPLE(OGLSwapBuffers);
			SwapBuffers(m_hDC);
		}
		else
		{
			// check for HWND availability
			if (!m_win->GetHandle()) {
				CON(MSG_ERR, _W("Can't get HWND handle from IWindow specified in BeginScene()! There is nowhere to render to!"));
				return;
			}
		}
		//CheckGLErrors(_W("End Scene"));
	}

	void CDeviceWrapper::SetViewport(Rect vp)
	{
		glViewport(vp.x1, vp.y1, vp.x2 - vp.x1, vp.y2 - vp.y1);
	}

	void CDeviceWrapper::Drop()
	{
		glDeleteFramebuffersEXT(1, &m_frameBuffer);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(m_hRC);
		m_hRC=NULL;
		ReleaseDC((HWND)m_win->GetHandle(),m_hDC);
		m_hDC = NULL;
	}


	void* CDeviceWrapper::GetInternalDevice()const
	{
		return 0;
	}

	bool CDeviceWrapper::Restart()
	{
		glShadeModel(GL_SMOOTH);
		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glEnable( GL_TEXTURE_2D );
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glViewport(0, 0, m_win->GetSize().x, m_win->GetSize().y);	
		glEnable(GL_CULL_FACE);
		glCullFace( GL_FRONT );

		// load extensions
		LoadGLExt();

		return true;
	}

	void CDeviceWrapper::BeginRendering()
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, NULL);
		Clear(ColorRGB(0.0f, 0.0f, 0.5f));
	}

	void CDeviceWrapper::BeginRendering(UINT NumRenderTargets, ITexture *RenderTargets)
	{
		CTexture *tex = (CTexture*)RenderTargets;
		GLuint gltex = (GLuint)*tex->GetRenderTarget();
		//TODO: multiple color attachments
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, gltex, 0);
		Clear(ColorRGB(0.0f, 0.5f, 0.0f));
	}

	void CDeviceWrapper::BeginRendering(UINT NumRenderTargets, P3D::ITexture *RenderTargets, P3D::ITexture *DepthStencil)
	{
		CTexture *depthtex = (CTexture*)DepthStencil;
		CTexture *tex = (CTexture*)RenderTargets;
		GLuint gltex = (GLuint)*tex->GetRenderTarget();
		GLuint gldtex = (GLuint)*tex->GetDepthStencil();
		//TODO: multiple color attachments
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, gltex, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, gldtex, 0);
		Clear(ColorRGB(0.0f, 0.0f, 0.0f));
	}

	void CDeviceWrapper::EndRendering()
	{

	}

	// **************************************** RENDER STATE OBJECTS *****************************************************

	GLenum Blend2GL(eBlend b)
	{
		switch (b)
		{
		case BLEND_ZERO: return GL_ZERO;
		case BLEND_ONE: return GL_ONE;
		case BLEND_SRC_COLOR: return GL_SRC_COLOR;
		case BLEND_INV_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
		case BLEND_SRC_ALPHA: return GL_SRC_ALPHA;
		case BLEND_INV_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
		case BLEND_DEST_COLOR: return GL_DST_COLOR;
		case BLEND_INV_DEST_COLOR: return GL_ONE_MINUS_DST_COLOR;
		case BLEND_DEST_ALPHA: return GL_DST_ALPHA;
		case BLEND_INV_DEST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
		default: assert(!"Unknown blend source!");
		}
		return GL_ONE;
	}

	void* CDeviceWrapper::CreateBlendStateInternal(const sBlendDesc* bd)
	{
		return NULL; // OpenGL doesn't have state objects, so we don't need internal pointer to it
	}

	void CDeviceWrapper::ReleaseBlendStateInternal(void* rapi_state)
	{
		// nothing for OGL
	}
	void CDeviceWrapper::SetBlendStateInternal(const sBlendDesc* bd, void* rapi_state)
	{
		// apply render state changes

		if (bd->AlphaToCoverageEnable != m_lastBlend.AlphaToCoverageEnable) 
		{
			if (bd->AlphaToCoverageEnable) 
			{
				if (m_bATC)
					glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
				else
				{
					glEnable(GL_ALPHA_TEST);
					glAlphaFunc(GL_GREATER, 0.5f);
				}
			}
			else
			{
				if (m_bATC)
					glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
				else
				{
					glDisable(GL_ALPHA_TEST);
				}
			}

			m_lastBlend.AlphaToCoverageEnable = bd->AlphaToCoverageEnable;
		}

		// TODO: what about next 7 render targets???!!!
		if (!bd->BlendEnable[0] && m_lastBlend.BlendEnable[0]) 
		{
			glDisable(GL_BLEND); // request to disable blending
			m_lastBlend.BlendEnable[0]=false;
		}
		else if (bd->BlendEnable[0])
		{
			if (!m_lastBlend.BlendEnable[0]) 
			{
				glEnable(GL_BLEND);
				m_lastBlend.BlendEnable[0]=true;
			}

			if (bd->BlendOp!=m_lastBlend.BlendOp) 
			{
				if (bd->BlendOp==BLEND_OP_MIN) glBlendEquation(GL_MIN);
				else if (bd->BlendOp==BLEND_OP_MAX) glBlendEquation(GL_MAX);
				else if (bd->BlendOp==BLEND_OP_ADD) glBlendEquation(GL_FUNC_ADD);
				else if (bd->BlendOp==BLEND_OP_SUBTRACT) glBlendEquation(GL_FUNC_SUBTRACT);
				else if (bd->BlendOp==BLEND_OP_REV_SUBTRACT) glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
				m_lastBlend.BlendOp = bd->BlendOp;
			}

			if (bd->SrcBlend!=m_lastBlend.SrcBlend || bd->DestBlend!=m_lastBlend.DestBlend) 
			{
				GLenum src; GLenum dest;
				src = Blend2GL(bd->SrcBlend);
				dest = Blend2GL(bd->DestBlend);
				glBlendFunc(src, dest);
				m_lastBlend.SrcBlend = bd->SrcBlend;
				m_lastBlend.DestBlend = bd->DestBlend;
			}
		}

		// TODO: what about next 7 render targets???!!!
		if (bd->RenderTargetWriteMask[0]!=m_lastBlend.RenderTargetWriteMask[0])
		{
			glColorMask(bd->RenderTargetWriteMask[0]&1, bd->RenderTargetWriteMask[0]&2, bd->RenderTargetWriteMask[0]&4, bd->RenderTargetWriteMask[0]&8 );
			m_lastBlend.RenderTargetWriteMask[0] = bd->RenderTargetWriteMask[0];
		}
	}

	void* CDeviceWrapper::CreateRasterizerStateInternal(const sRasterizerDesc* rd)
	{
		return NULL; // OpenGL doesn't have state objects, so we don't need internal pointer to it
	}

	void CDeviceWrapper::ReleaseRasterizerStateInternal(void* rapi_state)
	{
		// nothing for OGL
	}

	void CDeviceWrapper::SetRasterizerStateInternal(const sRasterizerDesc* rd, void* rapi_state)
	{
		if (rd->Wireframe != m_lastRasterizer.Wireframe) glPolygonMode(GL_FRONT_AND_BACK, rd->Wireframe ? GL_LINE : GL_FILL);

		if (rd->CullMode != m_lastRasterizer.CullMode) 
		{
			if (rd->CullMode==CULL_NONE) 
				glDisable(GL_CULL_FACE);
			else
			{
				if (m_lastRasterizer.CullMode==CULL_NONE) glEnable(GL_CULL_FACE);
				glCullFace(rd->CullMode==CULL_FRONT ? GL_BACK : GL_FRONT ); // we are using CW winding
			}
		}

		if (rd->DepthBias!=m_lastRasterizer.DepthBias || rd->SlopeScaledDepthBias!=m_lastRasterizer.SlopeScaledDepthBias)
		{
			if (rd->DepthBias==0 && rd->SlopeScaledDepthBias==0) 
				glDisable(GL_POLYGON_OFFSET_FILL);
			else
			{
				if (m_lastRasterizer.DepthBias==0 && m_lastRasterizer.SlopeScaledDepthBias==0) glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset((float)rd->DepthBias, rd->SlopeScaledDepthBias); // FIXME: are these parameters factor and units equal to DX bias and slope scale?
			}
		}

		if (rd->ScissorEnable!=m_lastRasterizer.ScissorEnable)
		{
			if (rd->ScissorEnable)
				glEnable(GL_SCISSOR_TEST);
			else
				glDisable(GL_SCISSOR_TEST);
		}

		if (rd->DepthBufferEnable != m_lastRasterizer.DepthBufferEnable) 
		{
			if (rd->DepthBufferEnable)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
		}

		if (rd->DepthBufferWriteEnable != m_lastRasterizer.DepthBufferWriteEnable) 
		{
			if (rd->DepthBufferWriteEnable)
				glDepthMask(GL_TRUE);
			else
				glDepthMask(GL_FALSE);
		}

		if (rd->AntialiasedLineEnable != m_lastRasterizer.AntialiasedLineEnable)
		{
			if (rd->AntialiasedLineEnable)
			{
				// temporarily enable blend as it is needed for antialiased line
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_LINE_SMOOTH); 
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			}
			else
			{
				glDisable(GL_LINE_SMOOTH); 
				glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

				// return blending to last blend state
				if (!m_lastBlend.BlendEnable[0]) glDisable(GL_BLEND);
				GLenum src; GLenum dest;
				src = Blend2GL(m_lastBlend.SrcBlend);
				dest = Blend2GL(m_lastBlend.DestBlend);
				glBlendFunc(src, dest);
			}
		}

		if (rd->MultisampleEnable!=m_lastRasterizer.MultisampleEnable)
		{
			if (rd->MultisampleEnable)
				glEnable(GL_MULTISAMPLE_ARB);
			else
				glDisable(GL_MULTISAMPLE_ARB);
		}

		// copy actual as last...
		m_lastRasterizer=*rd;
	}

};

#endif
