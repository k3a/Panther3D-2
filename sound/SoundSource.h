/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go, kalda
*/
#pragma once

#include "SoundBuffer.h"
#include "ISoundSource.h"
#include "OALExtProvider.h"
#include "types.h"

namespace P3D
{
	class CSoundSource : public ISoundSource
	{
	public:
		CSoundSource();
		~CSoundSource();
		bool Create(COALExtProvider *exts, CSoundBuffer *buffer, Vec3 position, bool relativeToListener);
		//bool Create(CSoundStream *stream, Vec3 position, bool relativeToListener); TODO: implement streamed sound

		//Playback
		void Play();
		void Pause();
		void Stop();
		void Rewind();
		void Seek(float seconds);

		void SetState(eSoundState state);
		void SetPitch(float pitch);
		void SetGain(float gain);
		void SetVolume(float volume);
		void SetMaxDistance(float maxDist);
		void SetRolloffFactor(float rolloff);
		void SetReferenceDistance(float refDist);
		void SetMinGain(float minGain);
		void SetMaxGain(float maxGain);
		void SetMinVolume(float minVolume);
		void SetMaxVolume(float maxVolume);
		void SetConeOuterGain(float outerGain);
		void SetConeInnerAngle(float innerAngleDeg);
		void SetConeOuterAngle(float outerAngleDeg);
		void SetPosition(Vec3& position);
		void SetVelocity(Vec3& velocity);
		void SetDirection(Vec3& direction);
		void SetPosition(float x, float y, float z);
		void SetVelocity(float x, float y, float z);
		void SetDirection(float x, float y, float z);
		void SetSourceRelativity(bool relativeToListener);
		void SetLooping(bool loop);
		void SetOffsetInSeconds(float seconds);
		void SetOffsetInSamples(int samples);
		void SetOffsetInBytes(int bytes);
		void SetReverb(sSoundReverbProperties reverb);

		sSoundReverbProperties GetReverb();
		eSoundState GetState();
		float GetPitch();
		float GetGain();
		float GetVolume();
		float GetMaxDistance();
		float GetRolloffFactor();
		float GetReferenceDistance();
		float GetMinGain();
		float GetMaxGain();
		float GetMinVolume();
		float GetMaxVolume();
		float GetConeOuterGain();
		float GetConeInnerAngle();
		float GetConeOuterAngle();
		Vec3  GetPosition();
		Vec3  GetVelocity();
		Vec3  GetDirection();
		void  GetPosition(Vec3& position);
		void  GetVelocity(Vec3& velocity);
		void  GetDirection(Vec3& direction);
		bool  GetSourceRelativity();
		bool  GetLooping();
		float GetOffsetInSeconds();
		int   GetOffsetInSamples();
		int   GetOffsetInBytes();
		bool  IsStreamed();
		void  GetReverb(sSoundReverbProperties& reverb);

		inline CSoundBuffer* GetBuffer() { return m_buffer; };
	private:
		sSoundReverbProperties m_reverb;
		CSoundBuffer *m_buffer;
		COALExtProvider *m_exts;
		ALuint m_source;
		ALuint m_effectSlot;
		ALuint m_effect;
		bool m_reverbInitialized;
	};
};