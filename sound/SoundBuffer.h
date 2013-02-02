/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once


#include <AL/al.h>
#include <AL/alc.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <cstdio>
#include <iostream>

#define BUFFER_SIZE     32768       // 32 KB buffers



namespace P3D
{

	enum eSoundFormat
	{
		SOUNDFORMAT_MONO8,
		SOUNDFORMAT_STEREO8,
		SOUNDFORMAT_MONO16,
		SOUNDFORMAT_STEREO16
	};
	
	///A buffer containing the main sound data which is shared across several sound sources.
	class CSoundBuffer
	{
	public:
		CSoundBuffer();
		~CSoundBuffer();
		bool Create(eSoundFormat format, int freq, void *data, unsigned int sizeInb);
		void* GetBuffer();
		void SetFilename(const wchar *filename) { wstrcpy(m_filename, filename); };
		const wchar* GetFilename() { return m_filename; };
		inline void AddReference() { m_references++; };
		inline void EraseReference() { m_references--; };
		inline int GetReferences() { return m_references; };
	
	private:
		ALuint m_buffer;
		wchar m_filename[50];
		int m_references;
	};

}