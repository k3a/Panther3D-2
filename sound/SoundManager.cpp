/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kalda
*/
#include "precompiled.h"

#include "SoundManager.h"
#include "OggLoader.h"

namespace P3D
{
	CSoundManager::CSoundManager()
	{
		m_pListener = 0;
		m_extensions = 0;
	}

	CSoundManager::~CSoundManager()
	{
		ClearAll();

		if(m_pListener)
		{
			delete m_pListener;
			m_pListener = 0;
			
			CON(MSG_DBG, _W("Listener deleted..."));
		}

		m_extensions = 0;
	}

	bool CSoundManager::Create(COALExtProvider* exts)
	{
		m_pListener = new CSoundListener();
		m_extensions = exts;

		if(!m_pListener)
		{
			CON(MSG_ERR_FATAL, _W("Can't create listener instance!"));
			return false;
		}

		return m_pListener->Create(Vec3(),Vec3(0,0,-1));
	}
		
	bool CSoundManager::PrecacheSound(const wchar *filename)
	{
		return (CreateBuffer(filename) != NULL);
	}

	ISoundSource* CSoundManager::CreateSource(const wchar *filename, Vec3 position, bool relativeToListener)
	{
		CSoundBuffer* buff = CreateBuffer(filename);
		if(!buff) return NULL;

		CSoundSource* source = new CSoundSource();
		if(!source)
		{
			DestroyBuffer(buff);
			return NULL;
		}

		source->Create(m_extensions, buff, position, relativeToListener);
		m_sources.push_back(source);

		return source;
	}

	void CSoundManager::ClearAll()
	{
		unsigned int i,s;
		s = m_sources.size();
		for(i = 0; i < s; i++)
			delete m_sources[i]; //Delete any survivors
		m_sources.clear();

		s = m_buffers.size();
		for(i = 0; i < s; i++)
			delete m_buffers[i]; //Delete any survivors
		m_buffers.clear();

		CON(MSG_DBG, _W("All the remaining sound sources deleted..."));
	}

	void CSoundManager::DestroySource(ISoundSource *source)
	{
		if(!source) return;
		
		CSoundBuffer* buff = ((CSoundSource*)source)->GetBuffer();

		int size = m_sources.size();
		for(int i = 0; i < size; i++)
		{
			if(m_sources[i] == source)
			{
				m_sources.erase(m_sources.begin() + i);
				break;
			}
		}

		delete source;

		DestroyBuffer(buff);
	}

	void CSoundManager::DestroyBuffer(CSoundBuffer* buffer)
	{
		if(!buffer) return;
		
		int size = m_buffers.size();

		for(int i = 0; i < size; i++)
		{
			if(m_buffers[i] == buffer)
			{
				buffer->EraseReference();
				if(!buffer->GetReferences())
				{
					m_buffers.erase(m_buffers.begin() + i);
					delete buffer;
					break;
				}
			}
		}
	}

	CSoundBuffer* CSoundManager::CreateBuffer(const wchar* name)
	{
		int arrSize = m_buffers.size();

		for(int i = 0; i < arrSize; i++)
		{
			if(!wstricmp(m_buffers[i]->GetFilename(), name))
			{
				m_buffers[i]->AddReference();
				return m_buffers[i];
			}
		}

		COggLoader loader(name);
		const sOggInfo* info = loader.GetInfo();
		unsigned int size = 0;
		const void* data = loader.GetData(size);

		if(data == 0)
			return NULL;
		
		eSoundFormat fmt;
		bool stereo = false;

		switch(info->numChannels)
		{
		case 1:
			stereo = false;
			break;
		case 2:
			stereo = true;
			break;
		default:
			return 0;
		}

		switch(info->bitsPerSample)
		{
		case 8:
			fmt = stereo ? SOUNDFORMAT_STEREO8 : SOUNDFORMAT_MONO8;
			break;
		case 16:
			fmt = stereo ? SOUNDFORMAT_STEREO16 : SOUNDFORMAT_MONO16;
			break;
		default:
			return 0;
		}

		CSoundBuffer *buffer = new CSoundBuffer();
		if(!buffer)
			return NULL;

		if(!buffer->Create(fmt, info->freq, (void*)data, size))
		{
			delete buffer;
			return NULL;
		}

		buffer->AddReference();
		buffer->SetFilename(name);
		m_buffers.push_back(buffer);

		return buffer;
	}

}