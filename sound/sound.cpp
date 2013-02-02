/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: Cutler
*/
#include "precompiled.h"

#include "sound.h"
#include "soundbuffer.h"
#include "soundsource.h"
#include "soundlistener.h"
#include "oggloader.h"

namespace P3D
{

	REGISTER_MODULE_CLASS(CSoundEngine, _W("sound"), _W("oal"));

	IEngine *CSoundEngine::s_pEngine=NULL;
	bool CSoundEngine::m_bInit = false;

	CSoundEngine::CSoundEngine()
	{
		m_device = NULL;
		m_context = NULL;
		m_extensions = NULL;
	}

	CSoundEngine::~CSoundEngine()
	{
		// unload subclasses
		UNLOAD_SUBCLASS(SoundManager);

		if(m_extensions) delete m_extensions;

		if(m_context)
		{
			alcSuspendContext(m_context);
			alcMakeContextCurrent(NULL);
			alcDestroyContext(m_context);
		}
		
		if(m_device) alcCloseDevice(m_device);
		
		m_context = NULL; 
		m_device = NULL;
		m_extensions = NULL;
	}

	bool CSoundEngine::Initialize(void *pEngine)
	{
		if (m_bInit) return false;
		m_bInit = true;

		s_pEngine = (IEngine*)pEngine;

		// memory leaks detection
#if defined(_DEBUG) && defined(_MSC_VER) && _MSC_VER >= 800
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		//_CrtSetBreakAlloc(76);
#endif

		CON(MSG_INFO, _W("= Sound Device initialization ="));

		m_device = alcOpenDevice(NULL);
		if(!m_device)
		{
			CON(MSG_ERR_FATAL, _W(" Can't initialize OpenAL! Please try to reinstall OpenAL libraries..."));
			return false;
		}

		m_extensions = new COALExtProvider();
		if(m_extensions == NULL)
		{
			CON(MSG_INFO, _W(" Can't create OpenAL Extension provider instance!"));
			m_context = alcCreateContext(m_device, NULL);
			if(m_context != NULL)
				alcMakeContextCurrent(m_context);
		}
		else
			m_extensions->Create(m_device, m_context);
		
		if(m_context == NULL)
		{
			CON(MSG_ERR_FATAL, _W(" Can't create OpenAL context!"));
			return false;
		}

		
		if (!cSoundManager()->Create(m_extensions))
			CON(MSG_ERR_FATAL, _W(" Can't create Sound Manager!"));

		CON(MSG_INFO, _W(" ...using OpenAL version %s"), _A2W(alGetString(AL_VERSION)) );

		return true;
	}

}; // namespace
