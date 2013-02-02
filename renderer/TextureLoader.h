/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "rtex.h"
#include <vector>

namespace P3D
{
	/// One image subset data
	struct sImageData
	{
		/// Pointer at first byte of image data
		void* pData;
		/// Size of image data
		unsigned long size;
		/// Subset data
		sRTImageSubset subset; 
	};

	/// General texture loader into internal rtex structures
	class CTextureLoader
	{
	public:
		CTextureLoader(){
			m_bLoaded=false; 
			m_info.nummips = m_info.datasize = 0; 
			m_data=NULL;
			m_dataLen=0;
		};
		~CTextureLoader(){Unload();};
		/// Load new texture
		bool Load(const wchar* path);
		/// Unload current texture
		void Unload()
		{
			if (!m_bLoaded) return;
			for (unsigned int i=0; i<m_subs.size(); i++)
			{
				delete m_subs[i]; m_subs[i]=NULL;
			}
			m_subs.clear();
			m_info.nummips = m_info.datasize = 0;
			if (m_data) delete[] m_data;
			m_data = NULL;
			m_dataLen=0;
			m_bLoaded=false;
		};
		/// Is a texture loaded?
		bool IsLoaded()const{return m_bLoaded;};
		/// Get structure with texture information including import settings
		sRTInfo GetInfo()const{return m_info;};
		/// Get type of texture
		eRTType GetTextureType()const{ return m_info.textype; };
		/// Get number of mipmaps (0 if only base without mipmaps)
		/** In case of cube textures, all the sides have the same mipmap count. */
		unsigned int GetNumMips()const{ return m_info.nummips; };
		/// Get mipmap. Each image type has at least one base with id=0, first mip is 1
		const sImageData* GetMipmapSubset(unsigned int mipmapID)const;
		/// Get cubemap side (optionally mipmap level of this cube side, where 0 is not mip, it is base)
		const sImageData* GetCubemapSubset(eRTCubeSide cubeSide, unsigned int mipmapID=0)const;
		/// Get image data specified by id of any texture type and category (mip, cube side, ...).
		/** Texture subset will be returned. You can get number of subsets by calling GetNumSubsets(). */
		const sImageData* GetSubsetByID(unsigned int subsetID)const;
		/// Get number of all subsets
		const unsigned int GetNumSubsets()const{ return m_subs.size(); };

	private:
		bool m_bLoaded;
		sRTInfo m_info;
		std::vector<sImageData*> m_subs;
		BYTE* m_data;
		unsigned long m_dataLen;
	};

}; // namespace