/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
   * This contents may be used and/or copied only with the written permission or
   * terms and conditions stipulated in agreement/contract.
   -----------------------------------------------------------------------------
   Authors: kex1k
*/
#pragma once
#include "interface.h"

#include "IGraphicsManager.h"
#include "IGraphicsDevice.h"
#include "ITextRenderer.h"
#include "IShaderManager.h"
#include "ISpriteRenderer.h"
#include "IRendererEventListener.h"
#include "IWindow.h"
#include "IGraphicsRenderer.h"
#include "CommandQueue.h"

namespace P3D
{

	enum eAPI
	{
		RAPI_NULL=0,
		RAPI_DX9,
		RAPI_DX10,
		RAPI_OGL
	};

	///A top class of the Renderer module. 
	class IRenderer : public IBaseInterface
	{
	public:
		///Initialize renderer module
		virtual bool Initialize(void* pEngine, CDoubleCommandQueue *dcq)=0;
		///Render next frame to all active render views
		virtual void NextFrame()=0;
		virtual eAPI GetAPI()const=0;
		virtual void AddEventListener(IRendererEventListener* list)=0;

		// singletons
		DECLARE_ISUBCLASS(GraphicsManager);
		DECLARE_ISUBCLASS(GraphicsDevice);
		DECLARE_ISUBCLASS(ShaderManager);
		DECLARE_ISUBCLASS(TextRenderer);
		DECLARE_ISUBCLASS(PrimitiveRenderer);
		DECLARE_ISUBCLASS(SpriteRenderer);
		DECLARE_ISUBCLASS(GraphicsRenderer);
	};

};
