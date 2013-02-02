/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <cstdio>
#include <vector>

#include "sound.h"

namespace P3D
{

	struct sOggInfo
	{
		unsigned int numChannels;
		int freq;
		unsigned int bitsPerSample;

		sOggInfo()
		{
			numChannels = 0;
			freq = 0;
			bitsPerSample = 0;
		}
	};

	struct sOggCallbacksInfo
	{
		IFileSystem* fileSystem;
		FSFILE *file;

		sOggCallbacksInfo()
		{
			fileSystem = 0;
			file = 0;
		}
	};
	
	class COggLoader
	{
	public:
		COggLoader();
		COggLoader(const wchar *filename);
		~COggLoader();
		bool Load(const wchar *filename);
		void Unload();
		inline const sOggInfo* GetInfo() { return &m_info; };
		inline const void* GetData(OUT unsigned int &size) { size = m_buffer.size(); return &m_buffer[0]; };
		inline bool IsLoaded() { return m_bInit; };

		static size_t OggFileRead(void *ptr, size_t size, size_t nmemb, void *datasource);
		static int OggFileSeek(void *datasource, ogg_int64_t offset, int whence);
		static int OggFileClose(void *datasource);
		static long OggFileTell(void *datasource);

	private:
		std::vector<wchar> m_buffer;
		sOggInfo m_info;
		bool m_bInit;
	};

}