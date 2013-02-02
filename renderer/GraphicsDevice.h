/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#if defined(_DX) && _DX==9
#include "dx/DeviceWrapper.h"
#elif defined(_OGL)
#include "OGL/DeviceWrapper.h"
#elif defined(_DX) && _DX==10
#include "DX10/DeviceWrapper.h"
#endif

#include "IGraphicsDevice.h"

#include "PrimitiveRenderer.h"
#include "ShaderManager.h"


namespace P3D
{
	/// Internal version of IBlendState
	/** Renderer API internal representation of this state object should be set to rapi_state. But it is optional and can be NULL. */
	class CBlendState : public IBlendState
	{
	public:
		const sBlendDesc *GetDesc()const{ return &m_blend; };
		void SetDesc( const sBlendDesc* pDesc, CDeviceWrapper* dev, void* rapi_state=NULL ){ 
			m_blend=*pDesc; 
			m_dev = dev; 
			m_rapi_state=rapi_state; 
		};
		unsigned long Release(){
			if (m_cnt == 1 && m_dev) m_dev->ReleaseBlendStateInternal(m_rapi_state);
			return IReferenced::Release();
		};
		void SetState()const{ m_dev->SetBlendStateInternal(&m_blend, m_rapi_state); };
	private:
		sBlendDesc m_blend;
		CDeviceWrapper *m_dev;
		void* m_rapi_state;
	};

	/// Internal version of IRasterizerState
	/** Renderer API internal representation of this state object should be set to rapi_state. But it is optional and can be NULL. */
	class CRasterizerState : public IRasterizerState
	{
	public:
		const sRasterizerDesc *GetDesc()const{ return &m_rasterizer; };
		void SetDesc( const sRasterizerDesc* pDesc, CDeviceWrapper* dev, void* rapi_state=NULL ){ 
			m_rasterizer=*pDesc; 
			m_dev = dev; 
			m_rapi_state=rapi_state; 
		};
		unsigned long Release(){
			if (m_cnt == 1 && m_dev) m_dev->ReleaseRasterizerStateInternal(m_rapi_state);
			return IReferenced::Release();
		};
		void SetState()const{ m_dev->SetRasterizerStateInternal(&m_rasterizer, m_rapi_state); };
	private:
		sRasterizerDesc m_rasterizer;
		CDeviceWrapper *m_dev;
		void* m_rapi_state;
	};

	class CGraphicsDevice : public CDeviceWrapper
	{
	public:
		CGraphicsDevice();
		~CGraphicsDevice();
		bool Initialize(unsigned int width, unsigned int height, bool fullscreen, void* primaryWindowHandle);
		void BeginScene(IRenderCanvas* wndTarget, ColorRGB color);
		void EndScene();
		/// Create new CRenderCanvas instance 
		ID CreateRenderCanvas(const sRenderCanvasDesc &desc, void *windowHandle);
		/// Delete a CRenderCanvas instance 
		void DeleteRenderCanvas(ID canvasId);

		//garbage
		IRenderCanvas* CreateRenderWindow(const sRenderCanvasDesc &desc, void *windowHandle);
		void DestroyRenderWindow(IRenderCanvas *renderWindow);

		void SetViewMatrix(const Matrix &mat) { m_viewMatrix = mat; };
		void SetProjectionMatrix(const Matrix &mat) { m_projectionMatrix = mat; };
		const Matrix& GetViewMatrix() { return m_viewMatrix; };
		const Matrix& GetProjectionMatrix() { return m_projectionMatrix; };
		void SetCameraPosition(const Vec3 &position) { m_campos = position; };
		Vec3 GetCameraPosition() { return m_campos; };
		//void *GetInternalDevice()const{ return m_DeviceWrapper.GetInternalDevice(); };

		// ******************* RENDER STATE OBJECTS ************************
#pragma region Render state objects
		IBlendState*  CreateBlendState(const sBlendDesc* blendDesc);
		void SetBlendState(const IBlendState* blendState);
		IRasterizerState*  CreateRasterizerState(const sRasterizerDesc* rasterizerDesc);
		void SetRasterizerState(const IRasterizerState* rasterizerState);
	private:
		vector<CBlendState*> m_blend_objs;
		vector<CRasterizerState*> m_rasterizer_objs;
#pragma endregion
	private:
		tArray<CRenderCanvas*> m_renderCanvases;

		Matrix m_viewMatrix;
		Matrix m_projectionMatrix;
		Vec3 m_campos;
	};

};
