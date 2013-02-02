/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: Cutler
*/
#pragma once




#include "interface.h"
#include "ISound.h"
#include "IEngine.h"

#include <al.h>
#include <AL/alut.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <cstdio>
#include <iostream>

#define BUFFER_SIZE     32768       // 32 KB buffers



namespace P3D
{
	class CSound : public ISound
	{
	public :
		CSound();
		~CSound();
		bool Initialize(void* pConsole);

		void TestSoundModule();

		void OpenStream(string path);
		void ReleaseStream();
		void DisplayInfo();
		bool Playback();
		bool Playing();
		bool Update();
		bool Stream(unsigned int buffer);
		void Empty();
		void Check();
		string ErrorString(int code);
		void LoadOGG(char *fileName, vector<char> &buffer, int &format, int &freq);

		static inline IEngine* mEngine(){ return s_pEngine; };
	private:

		static bool m_bInit;

		static IEngine *s_pEngine;

		FILE*           oggFile;
        OggVorbis_File  oggStream;
        vorbis_info*    vorbisInfo;
        vorbis_comment* vorbisComment;

        ALuint buffers[2];
        ALuint source;
        ALenum format;


	};


#define CON(type, text, ...) CSound::mEngine()->iConsole()->AddMsgEx(type, __FUNCTION__, text, ##__VA_ARGS__)


}; // namespace

