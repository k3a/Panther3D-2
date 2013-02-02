/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go, kexik
*/

#pragma once
#include "IWindow.h"

#include "IPrimitiveRenderer.h"

namespace P3D
{
	
	///Some basic info about the used graphics card and it's abilities.
	struct sGraphicsCardInfo
	{
		///The maximal texture resolution that can the used graphics card handle.
		unsigned int maxTextureResolution;
		///Hardware Shadow maps support
		bool hwShadowMaps;
		///Tells if the used graphics card can handle the non power of two textures.
		bool nonPow2Textures;
		/// Filtering of floating-point texture
		bool floatingPointFiltering;
		/// 16bit ARGB floating-point render target
		bool supports16bitRT;
		/// 32bit one component floating-point render target
		bool supports32bitRT;
		/// Independent bit depth in MRT support (if true, RTs in MRT can have different bit depths)
		bool independentBitDepth;
		/// DXT texture compression support
		bool supportsDXTCompression;
		/// ATI 3Dc texture compression support
		bool supports3DcCompression;
		///The maximal number of simultaneous render targets.
		unsigned int maxRenderTargets;
	};
	
	///Properties of display device
	struct sDisplayProperties
	{
		sDisplayProperties()
		{

		}

		sDisplayProperties(unsigned int horres, unsigned int vertres, bool fullscreen)
		{
			HorRes		= horres;
			VertRes		= vertres;
			Fullscreen	= fullscreen;
		}
		///Horizontal resolution (width)
		unsigned int	HorRes;
		///Vertical resolution (height)
		unsigned int	VertRes;
		///Run in fullscreen mode?
		bool			Fullscreen;
	};

	///Structure describing viewport.
	/** You can specify width and height of are to which render scene. You can also use offset X and Y. */
	struct sViewport
	{
		sViewport(UINT _width, UINT _height, UINT _x=0, UINT _y=0):
			width(_width), height(_height), x(_x), y(_y) {};

		sViewport():
			width(0), height(0), x(0), y(0) {};

		UINT width;
		UINT height;
		UINT x;
		UINT y;
	};

	// -----------------------------------------------------------------------------------------------------
	/** Blending sources
	Enumeration of all possible blending sources. */
	enum eBlend
	{
		/// Data source is the color black (0, 0, 0, 0).
		BLEND_ZERO = 1,
		/// The data source is the color white (1, 1, 1, 1).
		BLEND_ONE = 2,
		/// The data source is color data (RGB) from a pixel shader.
		BLEND_SRC_COLOR = 3,
		/// The data source is color data (RGB) from a pixel shader. The pre-blend operation inverts the data, generating 1 - RGB. 
		BLEND_INV_SRC_COLOR = 4,
		/// The data source is alpha data (A) from a pixel shader.
		BLEND_SRC_ALPHA = 5,
		/// The data source is alpha data (A) from a pixel shader. The pre-blend operation inverts the data, generating 1 - A. 
		BLEND_INV_SRC_ALPHA = 6,
		/// The data source is alpha data from a rendertarget.
		BLEND_DEST_ALPHA = 7,
		/// The data source is alpha data from a rendertarget. The pre-blend operation inverts the data, generating 1 - A. 
		BLEND_INV_DEST_ALPHA = 8,
		/// The data source is color data from a rendertarget.
		BLEND_DEST_COLOR = 9,
		/// The data source is color data from a rendertarget. The pre-blend operation inverts the data, generating 1 - RGB. 
		BLEND_INV_DEST_COLOR = 10,
		/*/// The data source is alpha data from a pixel shader. The pre-blend operation clamps the data to 1 or less. 
		BLEND_SRC_ALPHA_SAT = 11,
		BLEND_BLEND_FACTOR = 14,
		BLEND_INV_BLEND_FACTOR = 15,
		BLEND_SRC1_COLOR = 16,
		BLEND_INV_SRC1_COLOR = 17,
		BLEND_SRC1_ALPHA = 18,
		BLEND_INV_SRC1_ALPHA = 19,*/
	};

	// -----------------------------------------------------------------------------------------------------
	/** Blend operation
	Describes blend operation with sources.  */
	enum eBlendOp
	{
		/// Add source 1 and source 2. 
		BLEND_OP_ADD = 1,
		/// Subtract source 1 from source 2. 
		BLEND_OP_SUBTRACT = 2,
		/// Subtract source 2 from source 1. 
		BLEND_OP_REV_SUBTRACT = 3,
		/// Get the minimum of source 1 and source 2.
		BLEND_OP_MIN = 4,
		/// Get the maximum of source 1 and source 2.
		BLEND_OP_MAX = 5,
	};

	// -----------------------------------------------------------------------------------------------------
	/** Blend render state object
	    Describes blend operations and color write mask of output-merger part of rendering pipeline. */
	struct sBlendDesc {
		sBlendDesc(){
			AlphaToCoverageEnable = false;
			BlendEnable[0]=BlendEnable[1]=BlendEnable[2]=BlendEnable[3]=BlendEnable[4]=BlendEnable[5]=BlendEnable[6]=BlendEnable[7]=0;
			SrcBlend = BLEND_ONE;
			DestBlend = BLEND_ZERO;
			BlendOp = BLEND_OP_ADD;
			/*SrcBlendAlpha = BLEND_ONE;
			DestBlendAlpha = BLEND_ZERO;
			BlendOpAlpha = BLEND_OP_ADD;*/
			RenderTargetWriteMask[0]=RenderTargetWriteMask[1]=RenderTargetWriteMask[2]=RenderTargetWriteMask[3]=RenderTargetWriteMask[4]=\
			RenderTargetWriteMask[5]=RenderTargetWriteMask[6]=RenderTargetWriteMask[7]= /*255*/ 15; //4 bits
		};
		/// Use alpha-to-coverage multisampling? Default is false.
		bool AlphaToCoverageEnable;
		/// Enable blending for 8 elements. These are maximum number of render targets (output data of pixel shaders). Defaults are false.
		bool BlendEnable[8];
		/// Specifies first RGB data source.
		eBlend SrcBlend;
		/// Specifies second RGB data source.
		eBlend DestBlend;
		/// An operation specifying how to combine RGB sources.
		eBlendOp BlendOp;
		/*/// Specifies first alpha data source.
		eBlend SrcBlendAlpha;
		/// Specifies second alpha data source.
		eBlend DestBlendAlpha;
		/// An operation specifying how to combine alpha sources.
		eBlendOp BlendOpAlpha;*/
		/// Write mask for all 4 channels of render target. 
		/** Bits: 0000ABGR Default is 00001111 (==all ABGR bits are 1) */
		BYTE RenderTargetWriteMask[8];
	};

	/// Indicates triangles facing a particular direction are not drawn.
	enum eCullMode
	{
		/// All sides will be drawn (double sided).
		CULL_NONE,
		/// Front side of triangle will not be drawn.
		CULL_FRONT,
		/// Back side of triangle will not be drawn.
		CULL_BACK
	};

	// -----------------------------------------------------------------------------------------------------
	/** Rasterizer state object
	Describes rasterize operations, how the vector data are converted to pixels. */
	struct sRasterizerDesc {
		sRasterizerDesc(){
			Wireframe=false;
			CullMode=CULL_BACK;
			DepthBias=0;
			SlopeScaledDepthBias=0;
			DepthBufferEnable=true;
			DepthBufferWriteEnable=true;
			ScissorEnable=false;
			MultisampleEnable=false;
			AntialiasedLineEnable=false;
		}

		bool Wireframe;
		eCullMode CullMode;
		int DepthBias;
		float SlopeScaledDepthBias;
		bool DepthBufferEnable;
		bool DepthBufferWriteEnable;
		bool ScissorEnable;
		bool MultisampleEnable;
		bool AntialiasedLineEnable;
	};

	// -----------------------------------------------------------------------------------------------------
	/** Blend state object.
	    Describes one instance of blend operations and color write mask of output-merger part of rendering 
	    pipeline.<br><br>It must be created by calling IGraphicsDevice::CreateBlendState(). If you call 
		this function with parameters used by previously created blend state object, old state object will
		be returned and its reference count increased. 
		\warning These blend state object must be released by ->Release() method!!! */
	class IBlendState : public IReferenced
	{
	public:
		virtual const sBlendDesc *GetDesc()const=0;
	};

	// -----------------------------------------------------------------------------------------------------
	/** Rasterizer state object.
	Describes one instance of rasterizer states.<br><br>
	It must be created by calling IGraphicsDevice::CreateRasterizerState(). If you call 
	this function with parameters used by previously created rasterizer state object, old state object will
	be returned and its reference count increased. 
	\warning These rasterizer state object must be released by ->Release() method!!! */
	class IRasterizerState : public IReferenced
	{
	public:
		virtual const sRasterizerDesc *GetDesc()const=0;
	};

	// -----------------------------------------------------------------------------------------------------
	/** Graphics device interface.
		It controls rendering process, render states, shaders, ... */
	class IGraphicsDevice
	{
	public:
		///Device initialization
		virtual bool Initialize(unsigned int width, unsigned int height, bool fullscreen, void* primaryWindowHandle)=0;
		/// Begin new frame rendering.
		virtual void BeginScene(IRenderCanvas* wndTarget, ColorRGB color)=0;
		/// End rendering of current frame.
		/** Will swap buffers. */
		virtual void EndScene()=0;

		virtual void BeginRendering()=0;
		virtual void BeginRendering(UINT NumRenderTargets, ITexture *RenderTargets)=0;
		virtual void BeginRendering(UINT NumRenderTargets, ITexture *RenderTargets, ITexture *DepthStencil)=0;
		virtual void EndRendering()=0;

		virtual IRenderCanvas* CreateRenderWindow(const sRenderCanvasDesc &desc, void *windowHandle)=0;
		virtual void DestroyRenderWindow(IRenderCanvas *renderWindow)=0;

		///Setups the View matrix for the GraphicsDevice. You won't need to call this method probably.
		virtual void SetViewMatrix(const Matrix &mat)=0;
		///Setups the View matrix for the GraphicsDevice. You won't need to call this method probably.
		virtual void SetProjectionMatrix(const Matrix &mat)=0;

		virtual void SetCameraPosition(const Vec3 &position)=0;
		virtual Vec3 GetCameraPosition()=0;

		///Sets a viewport to which scene will be rendered
		//virtual void SetViewport(sViewport &viewport)=0;
		//virtual void SetDisplayProperties(sDisplayProperties DisplayProperties)=0;
		//virtual void TakeScreenshot(const wchar* filename)=0;

		// get internal device
		/** It is Renderer-API dependent so be careful! Check which API is running using IRenderer::GetAPI() before calling this...*/
		//virtual void *GetInternalDevice()const=0;
		

		// ******************* RENDER STATE OBJECTS ************************
		/// Create blend state object or return previously created with the same descriptor.
		virtual IBlendState*  CreateBlendState(const sBlendDesc* blendDesc)=0;
		/// Set active blend state object
		virtual void SetBlendState(const IBlendState* blendState)=0;
		/// Create rasterizer state object or return previously created with the same descriptor.
		virtual IRasterizerState*  CreateRasterizerState(const sRasterizerDesc* rasterizerDesc)=0;
		/// Set active rasterizer state object
		virtual void SetRasterizerState(const IRasterizerState* rasterizerState)=0;
	};

};
