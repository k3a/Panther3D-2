/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go, kalda
*/
#pragma once

#include "SoundReverb.h"
#include "types.h"

namespace P3D
{
	enum eSoundState
	{
		Sound_Play,
		Sound_Pause,
		Sound_Stop
	};

	class ISoundSource
	{
	public :
		virtual void Play() = 0;
		virtual void Pause() = 0;
		virtual void Stop() = 0;
		virtual void Rewind() = 0;
		virtual void Seek(float seconds) = 0;

		virtual void SetState(eSoundState state) = 0;
		virtual void SetPitch(float pitch) = 0;
		virtual void SetGain(float gain) = 0;
		virtual void SetVolume(float volume) = 0;
		virtual void SetMaxDistance(float maxDist) = 0;
		virtual void SetRolloffFactor(float rolloff) = 0;
		virtual void SetReferenceDistance(float refDist) = 0;
		virtual void SetMinGain(float minGain) = 0;
		virtual void SetMaxGain(float maxGain) = 0;
		virtual void SetMinVolume(float minVolume) = 0;
		virtual void SetMaxVolume(float maxVolume) = 0;
		virtual void SetConeOuterGain(float outerGain) = 0;
		virtual void SetConeInnerAngle(float innerAngleDeg) = 0;
		virtual void SetConeOuterAngle(float outerAngleDeg) = 0;
		virtual void SetPosition(Vec3& position) = 0;
		virtual void SetVelocity(Vec3& velocity) = 0;
		virtual void SetDirection(Vec3& direction) = 0;
		virtual void SetPosition(float x, float y, float z) = 0;
		virtual void SetVelocity(float x, float y, float z) = 0;
		virtual void SetDirection(float x, float y, float z) = 0;
		virtual void SetSourceRelativity(bool relativeToListener) = 0;
		virtual void SetLooping(bool loop) = 0;
		virtual void SetOffsetInSeconds(float seconds) = 0;
		virtual void SetOffsetInSamples(int samples) = 0;
		virtual void SetOffsetInBytes(int bytes) = 0;
		virtual void SetReverb(sSoundReverbProperties reverb) = 0;
		
		virtual sSoundReverbProperties GetReverb() = 0;
		virtual eSoundState GetState() = 0;
		virtual float GetPitch() = 0;
		virtual float GetGain() = 0;
		virtual float GetVolume() = 0;
		virtual float GetMaxDistance() = 0;
		virtual float GetRolloffFactor() = 0;
		virtual float GetReferenceDistance() = 0;
		virtual float GetMinGain() = 0;
		virtual float GetMaxGain() = 0;
		virtual float GetMinVolume() = 0;
		virtual float GetMaxVolume() = 0;
		virtual float GetConeOuterGain() = 0;
		virtual float GetConeInnerAngle() = 0;
		virtual float GetConeOuterAngle() = 0;
		virtual Vec3  GetPosition() = 0;
		virtual Vec3  GetVelocity() = 0;
		virtual Vec3  GetDirection() = 0;
		virtual void  GetPosition(Vec3& position) = 0;
		virtual void  GetVelocity(Vec3& velocity) = 0;
		virtual void  GetDirection(Vec3& direction) = 0;
		virtual bool  GetSourceRelativity() = 0;
		virtual bool  GetLooping() = 0;
		virtual float GetOffsetInSeconds() = 0;
		virtual int   GetOffsetInSamples() = 0;
		virtual int   GetOffsetInBytes() = 0;
		virtual bool  IsStreamed() = 0;
		virtual void  GetReverb(sSoundReverbProperties& reverb) = 0;
		
	};

}