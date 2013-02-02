/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "sound.h"
#include "SoundBuffer.h"



namespace P3D
{

	CSoundBuffer::CSoundBuffer()
	{
		m_buffer = AL_NONE;
		m_references = 0;
	}

	CSoundBuffer::~CSoundBuffer()
	{
		if(m_buffer != AL_NONE) alDeleteBuffers(1, &m_buffer);
		m_buffer = AL_NONE;
	}

	bool CSoundBuffer::Create(P3D::eSoundFormat format, int freq, void *data, unsigned int sizeInb)
	{
		ALenum alformat;
		if(format == SOUNDFORMAT_MONO16) alformat = AL_FORMAT_MONO16;
		else if(format == SOUNDFORMAT_STEREO16) alformat = AL_FORMAT_STEREO16;
		else if(format == SOUNDFORMAT_MONO8) alformat = AL_FORMAT_MONO8;
		else if(format == SOUNDFORMAT_STEREO8) alformat = AL_FORMAT_STEREO8;
		else
		{
			CON(MSG_ERR, _W("Unknown format specified!"));
			return false;
		}
		
		alGenBuffers(1, &m_buffer);
		alBufferData(m_buffer, alformat, data, sizeInb, freq);

		return true;
	}

	void* CSoundBuffer::GetBuffer()
	{
		return &m_buffer;
	}


}