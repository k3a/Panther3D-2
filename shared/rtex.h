/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
// Reversity Texture File Format

/*
  File contents:

   ================
  |   sRTHeader    |
   ================
  |    sRTInfo     |
   ================
  | sRTImageSubset |
   ================
        .....
   several subsets
        .....
   ================
  | sRTImageSubset |
   ================
  |  BINARY DATA   |  <= Unknown length image data for all subsets
   ================
*/

#pragma once
#include "types.h"

namespace P3D {

// *** CONFIGURATION
#define RT_MAJOR 2
#define RT_MINOR 0

/// Number of categories. \see RTHeader
#define RT_NUM_PARTS 2

/// Descriptor of one part of file.
struct sRTPart
{
	/// Offset of the data from the beginning of file
	long offset;
	/// Length of data. Can be x times longer than given structure.
	long length;
};

/// Reversity Texture file header.
struct sRTHeader
{
	/// File identifier. Must be "RT".
	char magic[2];
	/// Major version of file format. All files with the same major are compatible.
	BYTE major;
	/// Minor version. After using some previously "reserved" field, this number should be increased.
	BYTE minor;
	/// Contents of file in parts. Each category can contain subsequently several same struct data.
	sRTPart contents[RT_NUM_PARTS];
};

/// One file information structure. \see sRTInfo
#define RT_FILE_INFO		0
/// One image subset. \see sRTImageSubset
#define RT_IMAGE_SUBSET		1

/// Texture type enumeration.
enum eRTType
{
	RTT_UNKNOWN=0,
	RTT_STANDARD,
	RTT_VOLUME,
	RTT_CUBE
};

/// Texture format enumeration.
enum eRTFormat
{
	RTF_UNKNOWN,
	RTF_A8,
	RTF_P8,
	RTF_L8,
	RTF_L16,
	RTF_A8P8,
	RTF_A4L4,
	RTF_A8L8,
	RTF_R3G3B2,
	RTF_R4G4B4,
	RTF_R5G6B5,
	RTF_A1R5G5B5,
	RTF_B8G8R8, // WTF?? Really in this order?
	RTF_B8G8R8A8, // WTF?? Really in this order?
	RTF_X8R8G8B8,
	RTF_A8R8G8B8,
	RTF_R16G16B16,
	RTF_A16R16G16B16,
	RTF_DXT1, //DX 10 Name	BC1
	RTF_DXT1A,
	RTF_DXT2, //DX 10 Name	(none)
	RTF_DXT3, //DX 10 Name	BC2
	RTF_DXT4, //DX 10 Name	(none)
	RTF_DXT5, //DX 10 Name	BC3
	RTF_3DC
};

/// Mesh information structure. Can contain import path and setting. Longer strings are cut.
struct sRTInfo
{
	/// Long path to file, from which was this texture imported and created. Useful for re-import.
	char importpath[512];
	/// Username of person who imported this texture. :-P
	char username[32];
	/// DateTime stamp of file creation in string format YYYYMMDDHHMMSS
	char created[16];
	/// DateTime stamp of last (re)import in string format YYYYMMDDHHMMSS
	char imported[16];

	/// Keywords useful for searching
	char keywords[64];

	//- importer settings:

	/// Make texture dimensions power of two. It should be true for compatibility reasons.
	bool ipowoftwo;
	/// Build mip map chain?
	bool ibuildmips;

	bool reserved1;
	bool reserved2;
	bool reserved3;

	//- texture info
	/// Texture type (classic, volume, cube, ...)
	eRTType textype;
	/// Pixel format
	eRTFormat texformat;
	/// Number of mip levels. Will be 0 in case of no mip maps (only base texture)
	unsigned int nummips;
	/// Bits per pixel
	unsigned int bpp;
	/// Width of largest - base image
	unsigned long width;
	/// Height of largest - base image
	unsigned long height;
	/// Depth (in case of volume texture)
	unsigned int depth;
	
	/// Length of all image data stored in this .rtex
	unsigned long datasize;
};

/// Cube map face enumeration - DON'T CHANGE THIS ENUM!!!
enum eRTCubeSide
{
	RT_POSITIVEX=1, 
	RT_NEGATIVEX,
	RT_POSITIVEY, 
	RT_NEGATIVEY,
	RT_POSITIVEZ,
	RT_NEGATIVEZ
};

/// One image subset (one mip level, or one mip level of one cube side)
/** One separated graphic data of texture (one mip map or one level). 
	For standard 2D texture with 3 mip maps there will be 3 pieces of this structure. */
struct sRTImageSubset
{
	/// Cubemap face id (starting from 1=positive x) in case of cube map face. \see eRTCubeSide
	eRTCubeSide cubeFaceID;
	/// Mip level (starting from 0, where 0-largest,base texture)
	unsigned int mipmapID;
	/// Offset from the beginning of subset data, after sRTInfo - first data byte
	unsigned long offset;
	/// Length of this image subset in bytes
	unsigned long len;
	/// Width
	unsigned long width;
	// Height
	unsigned long height;
};


}; // namespace