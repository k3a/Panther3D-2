/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, am!go
*/
#pragma once
#include "IConsole.h"
#include "rtex.h"
#include "BufferRequisities.h"


namespace P3D
{

	///Defines texture filtering modes.
	enum eTexFilter
	{
		TF_NONE = 0,
		TF_POINT = 1,
		TF_LINEAR = 2,
		TF_ANISOTROPIC = 3
	};

	///Defines texture address modes.
	enum eTexAddress
	{
		TA_NONE = 0,
		TA_WRAP = 1,
		TA_CLAMP = 2,
	};

	///Defines texture settings like filtering and address modes.
	struct sTexSettings
	{
		sTexSettings()
		{
			MinFilter = TF_LINEAR;
			MagFilter = TF_LINEAR;
			MaxAnistropy = 0;
			AddressU = TA_WRAP;
			AddressV = TA_WRAP;
		}

		eTexFilter MinFilter;
		eTexFilter MagFilter;
		unsigned short MaxAnistropy;
		eTexAddress AddressU;
		eTexAddress AddressV;
	};


	



	//TODO: Delete most of that shit above

	enum eTextureBindFlag
	{
		P3D_TEXBINDFLAG_TEXTURE2D = 0,
		P3D_TEXBINDFLAG_TEXTURE3D,
		P3D_TEXBINDFLAG_RENDER_TARGET,
		P3D_TEXBINDFLAG_DEPTH_STENCIL //reserved for now
	};

	struct sTextureDesc
	{
		UINT Width;
		UINT Height;
		UINT MipLevels;
		UINT ArraySize;
		eResourceFormat Format;
		eTextureBindFlag BindFlag;
		eCPUAccess CPUAccess;
		eResourceUsage ResourceUsage;
		bool AutoGenerateMips;
	};
	
	///Represents a texture object.
	class ITexture
	{
	public:
		///Returns the texture's filename.
		virtual const wchar* GetFilename()=0;
		//virtual void SetTexSettings(sTexSettings settings)=0;
		//virtual sTexSettings GetTexSettings()=0;

	};
};
