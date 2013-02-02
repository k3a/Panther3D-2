/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go, kalda
*/

#pragma once

#include "ISoundManager.h"
#include "SoundSource.h"
#include "SoundListener.h"
#include "OALExtProvider.h"
#include <vector>

namespace P3D
{
	
	class CSoundManager : public ISoundManager
	{
	public:
		CSoundManager();
		~CSoundManager();
		bool Create(COALExtProvider* exts);
		
		bool PrecacheSound(const wchar *filename);
		ISoundSource* CreateSource(const wchar *filename, Vec3 position, bool relativeToListener);
		void ClearAll();
		void DestroySource(ISoundSource *source);
		inline ISoundListener* iSoundListener() { return m_pListener; };

		void DestroyBuffer(CSoundBuffer* buffer);
		CSoundBuffer* CreateBuffer(const wchar* name);

	private:
		COALExtProvider *m_extensions;
		CSoundListener *m_pListener;
		std::vector<CSoundBuffer*> m_buffers;
		std::vector<CSoundSource*> m_sources;
	};

}