/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "ddsloader.h"
#include <stdio.h>

namespace P3D
{

bool CDDSLoader::CheckDXT1Alpha (const unsigned char* source, int w, int h, int depth)
{
	int				x, y, z;
	unsigned char	*Temp;
	unsigned short	color_0, color_1;

	Temp = (unsigned char*) source;
	for (z = 0; z < depth; z++) 
	{
		for (y = 0; y < w; y += 4) 
		{
			for (x = 0; x < h; x += 4) 
			{
				color_0 = *((unsigned char*)Temp); 
				color_1 = *((unsigned char*)Temp+1);
				Temp += 8;

				if (color_0 <= color_1) 
				{
					return false;
				}
			}
		}
	}
	return true;
}

static unsigned int CalcDataSize (eRTFormat format, int bpp, int w, int h, int depth)
{
	switch (format)
	{
	case RTF_DXT1:
	case RTF_DXT1A:
	case RTF_DXT2:
	case RTF_DXT3:
	case RTF_DXT4:
	case RTF_DXT5:
		{
			int minW = ((w + 3) / 4) * 4;
			int minH = ((h + 3) / 4) * 4;
			return (minW * minH * bpp) / 8;
		}
	default:
		return (w * h * depth * bpp) / 8;
	}
}



eRTFormat CDDSLoader::GetFormat(DDS_PIXELFORMAT pf)
{
	// Any fourcc formats?
	if (pf.flags & DDPF_FOURCC)
	{
		switch (pf.fourcc)
		{
		case MakeFourCC ('D','X','T','1'):
			m_info.bpp = 4;
			return RTF_DXT1;
		case MakeFourCC ('D','X','T','2'):
			m_info.bpp = 8;
			return RTF_DXT2;
		case MakeFourCC ('D','X','T','3'):
			m_info.bpp = 8;
			return RTF_DXT3;
		case MakeFourCC ('D','X','T','4'):
			m_info.bpp = 8;
			return RTF_DXT4;
		case MakeFourCC ('D','X','T','5'):
			m_info.bpp = 8;
			return RTF_DXT5;
		case MakeFourCC ('A','T','I','2'):
			m_info.bpp = 8;
			return RTF_3DC;
		}
	}
	else // any normal known pixel format?
	{
		m_info.bpp = pf.bitdepth;
		if (pf.bitdepth == 8)
		{
			if (pf.redmask == 0xff)
			{
				if ((pf.flags & DDPF_LUMINANCE) && !(pf.flags & DDPF_ALPHAPIXEL))
					return RTF_L8;
			}
		}
		else if (pf.bitdepth == 16)
		{
			if ((pf.redmask == 0xf800) && (pf.greenmask == 0x07e0) && (pf.bluemask == 0x001f))
			{
				if (!(pf.flags & DDPF_ALPHAPIXEL))
					return RTF_R5G6B5;
			}
		}
		else if (pf.bitdepth == 24)
		{
			if ((pf.redmask == 0x00ff0000) && (pf.greenmask == 0x0000ff00) && (pf.bluemask == 0x00000ff))
			{
				if (!(pf.flags & DDPF_ALPHAPIXEL))
					return RTF_B8G8R8;
			}
		}
		else if (pf.bitdepth == 32)
		{
			if ((pf.redmask == 0x00ff0000) && (pf.greenmask == 0x0000ff00) && (pf.bluemask == 0x00000ff))
			{
				if ((pf.flags & DDPF_ALPHAPIXEL) && (pf.alphamask == 0xff000000))
					return RTF_B8G8R8A8;
			}
		}
	}

	return RTF_UNKNOWN;
}

// ------------------------------------------------
bool CDDSLoader::Load(const char* path)
{
	Unload();
	memset(&m_info, 0, sizeof(sRTInfo));

	FILE* fp;
	fp = fopen(path, "rb");
	if (!fp)
	{
		return false;
	}

	fseek(fp, 0, SEEK_END);
	m_dataLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (m_dataLen < sizeof(DDS_HEADER))
	{
		fclose(fp);
		return false;
	}

	// read header
	DDS_HEADER head;
	fread(&head, 1, sizeof(DDS_HEADER), fp);

	// check header
	if (head.magic!=MakeFourCC ('D','D','S',' '))
	{
		fclose(fp);
		return false;
	}

	const uint minimumflags = DDSD_CAPS | DDSD_HEIGHT | 
		DDSD_WIDTH | DDSD_PIXELFORMAT;
	if ((head.flags & minimumflags) != minimumflags)
	{
		fclose(fp);
		return false;
	}

	// get pixel format
	m_info.texformat = GetFormat(head.pixelformat);
	if (m_info.texformat==RTF_UNKNOWN)
	{
		fclose(fp);
		return false;
	}

	// check type
	if ( (head.capabilities.caps2 & DDSCAPS2_CUBEMAP)
		&& (head.capabilities.caps2 & (DDSCAPS2_CUBEMAP_POSITIVEX
		| DDSCAPS2_CUBEMAP_NEGATIVEX | DDSCAPS2_CUBEMAP_POSITIVEY
		| DDSCAPS2_CUBEMAP_NEGATIVEY | DDSCAPS2_CUBEMAP_POSITIVEZ
		| DDSCAPS2_CUBEMAP_NEGATIVEZ)) )
		m_info.textype = RTT_CUBE;
	else if (head.capabilities.caps2 & DDSCAPS2_VOLUME)
		m_info.textype = RTT_VOLUME;
	else
		m_info.textype = RTT_STANDARD;

	// get mipmap count
	m_info.nummips = 0;
	if ((head.flags & DDSD_MIPMAPCOUNT) 
		&& (head.capabilities.caps1 & DDSCAPS_MIPMAP)
		&& (head.mipmapcount != 0))
		m_info.nummips = head.mipmapcount;

	// load data
	m_info.datasize = m_dataLen-sizeof(DDS_HEADER); // FIXME: is it good?
	m_data = new unsigned char[m_dataLen-sizeof(DDS_HEADER)];
	fread(m_data, 1, m_dataLen-sizeof(DDS_HEADER), fp);
	m_bLoaded=true;
	fclose(fp);
	
	// check for DXT1 Alpha
	m_info.depth = (head.flags & DDSD_DEPTH) ? head.depth : 1;
	if (m_info.texformat==RTF_DXT1 && CheckDXT1Alpha(m_data, head.width, head.height, m_info.depth) )
		m_info.texformat = RTF_DXT1A;

	// copy width and height
	m_info.width = head.width;
	m_info.height = head.height;

	unsigned int dataSize = 0; unsigned int offset=0; // offset in source data of dds without header
	for (int f = m_info.textype==RTT_CUBE?6:1; f > 0; f--) // go through all the cube faces
	{
		sImageData* id = new sImageData();
		dataSize = CalcDataSize(m_info.texformat, m_info.bpp, head.width, head.height, m_info.depth);
		id->pData = &m_data[offset];
		id->size = id->subset.len = dataSize;
		id->subset.offset = offset;
		id->subset.cubeFaceID = (P3D::eRTCubeSide)(7-f);
		id->subset.width = head.width;
		id->subset.height = head.height;
		id->subset.mipmapID = 0;

		offset+=dataSize;
		m_subs.push_back(id);

		// go throught all mips
		if (m_info.nummips>0)
		{
			uint mipCount = head.mipmapcount - 1;
			uint w = head.width; uint h = head.height; uint d = m_info.depth;
			while (mipCount-- > 0)
			{
				sImageData* id = new sImageData();
				w = max (1, w >> 1); h = max (1, h >> 1); d = max (1, d >> 1);
				dataSize = CalcDataSize(m_info.texformat, m_info.bpp, w, h, m_info.depth);
				id->pData = &m_data[offset];
				id->size = id->subset.len = dataSize;
				id->subset.offset = offset;
				id->subset.cubeFaceID = (P3D::eRTCubeSide)(7-f);
				id->subset.width = w;
				id->subset.height = h;
				id->subset.mipmapID = head.mipmapcount - mipCount - 1;

				offset+=dataSize;
				m_subs.push_back(id);
			}
		}
	}

	int i = 1; // debug

	return true;
}

const sImageData* CDDSLoader::GetMipmapSubset(unsigned int mipmapID)const
{
	// not useful for dds loader
	return NULL;
}

const sImageData* CDDSLoader::GetCubemapSubset(eRTCubeSide cubeSide, unsigned int mipmapID/*=0*/)const
{
	// not useful for dds loader
	return NULL;
}

const sImageData* CDDSLoader::GetSubsetByID(unsigned int subsetID)const
{
	if (subsetID>=m_subs.size()) return NULL;
	return m_subs[subsetID];
}



}; // namespace