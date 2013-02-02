/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once
#include "IRenderableObject.h"
#include "ITexture.h"
#include "IMesh.h"
#include "IShader.h"

// caching
#include "OModel.h"

namespace P3D
{

/// Graphic manager for resource manipulation
class IGraphicsManager
{
public:
	///Creates new texture by given data.
	virtual ITexture* CreateTexture(sTextureDesc &desc, BYTE *data, unsigned long len)=0;
	///Loads texture from file.
	virtual ITexture* LoadTexture(const wchar* filename)=0;
	virtual ITexture* GetDafaultTexture()=0;
	///Loads rmesh file from data/models directory. You can use sub-dirs.
	/**
	\return In case of loading failed, NULL will be returned and reason will be printed in console.
	*/
	virtual IMesh* LoadMesh(const wchar *filename)=0;

	//Temporary hack 
	virtual inline void SetActiveVS(IShader *shader)=0;
	virtual inline IShader* GetActiveVS()=0;
	virtual inline void SetViewProjMatrix(const Matrix &m)=0;

	// ---- caching
	virtual bool CacheMeshData(MeshData& md)=0;
};

};
