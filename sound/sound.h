/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: Cutler, kalda
*/
#pragma once




#include "interface.h"
#include "ISound.h"
#include "IEngine.h"
#include "OALExtProvider.h"
#include "SoundManager.h"

#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alut.h>
#include <ogg/ogg.h>
//#include <vorbis/codec.h>
//#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#define BUFFER_SIZE     32768       // 32 KB buffers



namespace P3D
{
	class CSoundEngine : public ISoundEngine
	{
	public :
		CSoundEngine();
		~CSoundEngine();
		bool Initialize(void* pConsole);

		// this module subclasses - singletons
		DECLARE_SUBCLASS(SoundManager);

		// external modules
		static inline IEngine* mEngine(){ return s_pEngine; };
		
	private:
		static bool m_bInit;
		static IEngine *s_pEngine;
		COALExtProvider* m_extensions;
		ALCdevice* m_device;
		ALCcontext* m_context;
	};


#define CON(type, text, ...) CSoundEngine::mEngine()->iConsole()->AddMsgEx(type, _W(__FUNCTION__), text, ##__VA_ARGS__)


}; // namespace

