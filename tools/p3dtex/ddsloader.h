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

typedef unsigned int uint;

// ==============================================

enum
{
	// Flags for Header
	DDSD_CAPS	   = 0x00000001,
	DDSD_HEIGHT	   = 0x00000002,
	DDSD_WIDTH	   = 0x00000004,
	DDSD_PITCH	   = 0x00000010,
	DDSD_PIXELFORMAT = 0x00001000,
	DDSD_MIPMAPCOUNT = 0x00020000,
	DDSD_LINEARSIZE  = 0x00080000,
	DDSD_DEPTH	   = 0x00800000,

	// Flags for Pixelformats
	DDPF_ALPHAPIXEL  = 0x00000001,
	DDPF_FOURCC	   = 0x00000004,
	DDPF_RGB	   = 0x00000040,
	DDPF_LUMINANCE   = 0x00020000,

	// Flags for complex caps
	DDSCAPS_COMPLEX  = 0x00000008,
	DDSCAPS_TEXTURE  = 0x00001000,
	DDSCAPS_MIPMAP   = 0x00400000,

	// Flags for cubemaps
	DDSCAPS2_CUBEMAP	     = 0x00000200,
	DDSCAPS2_CUBEMAP_POSITIVEX = 0x00000400,
	DDSCAPS2_CUBEMAP_NEGATIVEX = 0x00000800,
	DDSCAPS2_CUBEMAP_POSITIVEY = 0x00001000,
	DDSCAPS2_CUBEMAP_NEGATIVEY = 0x00002000,
	DDSCAPS2_CUBEMAP_POSITIVEZ = 0x00004000,
	DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x00008000,
	DDSCAPS2_VOLUME	     = 0x00200000
};

enum
{
	FORMAT_UNKNOWN = 0,
	FORMAT_DXT1,
	FORMAT_DXT2,
	FORMAT_DXT3,
	FORMAT_DXT4,
	FORMAT_DXT5,
	FORMAT_RGB,
	FORMAT_RGBA
};

struct DDS_PIXELFORMAT
{
	uint size;      // size of this structure (should be 32)

	uint flags;     // flags which values are present here
	uint fourcc;    // character code for image format
	uint bitdepth;  // number of bits per pixel (usually 16,24 or 32)

	uint redmask;   // mask for the red pixels (usually 0x00ff0000)
	uint greenmask; // mask for the green pixels (usually 0x0000ff00)
	uint bluemask;  // mask for the blue pixels (usually 0x000000ff)

	uint alphamask; // mask for the alpha value in a pixel
	// (usually 0xff000000)
};

struct DDS_CAPS
{
	uint caps1;
	uint caps2;
	uint reserved[2];
	/*uint caps3;
	uint caps4;
	uint texturestage;*/
};

struct DDS_HEADER
{
	uint magic;     // Magic Number (has to be "DDS "

	uint size;	    // Size of the descriptor structure (should be 124)

	uint flags;     // flags field
	uint height;
	uint width;

	uint linearsize;

	uint depth;	    // for volume textures: depth the FLAG_DEPTH flag
	// should be set in this case
	uint mipmapcount; // number of mipmap levels included. flags
	// should include FLAG_MIPMAPCOUNT in this case
	uint alphabitdepth;	// depth of alpha buffer

	uint reserved[10];

	DDS_PIXELFORMAT pixelformat;
	DDS_CAPS capabilities;

	uint reserved001;
};

#define MakeFourCC(c1, c2, c3, c4) \
	((uint) c1 | ((uint) c2<<8) | ((uint) c3<<16) | ((uint) c4<<24))

// ==============================================






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

/// DDS texture loader into internal rtex structures
class CDDSLoader
{
public:
	CDDSLoader(){
		m_bLoaded=false; 
		m_info.nummips = m_info.datasize = 0; 
		m_data=NULL;
		m_dataLen=0;
	};
	~CDDSLoader(){Unload();};
	/// Load new texture
	bool Load(const char* path);
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
	bool CheckDXT1Alpha (const unsigned char* source, int w, int h, int depth);
	eRTFormat GetFormat(DDS_PIXELFORMAT pf);

private:
	bool m_bLoaded;
	sRTInfo m_info;
	std::vector<sImageData*> m_subs;
	unsigned char* m_data;
	unsigned long m_dataLen;
};

}; // namespace