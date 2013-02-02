/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "OggLoader.h"

namespace P3D
{

	COggLoader::COggLoader()
	{
		m_bInit = false;
	}

	COggLoader::~COggLoader()
	{
		Unload();
	}

	COggLoader::COggLoader(const wchar *filename)
	{
		Load(filename);
	}

	bool COggLoader::Load(const wchar *filename)
	{
		Unload();

		wchar fspath[P3DMAX_PATH];
		wsprintf(fspath, P3DMAX_PATH-1, _W("sounds/%s.ogg"), filename);

		IFileSystem* pFS = CSoundEngine::mEngine()->mFilesystem();
		FSFILE* fp = pFS->Open(fspath, _W("rb"));
		if(!fp) 
		{
			CON(MSG_INFO, _W("Sound %s has not been found!"), filename);
			return false;
		}

		vorbis_info *pInfo;
		OggVorbis_File oggFile;
		long bytes;
		int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
		int bitStream;
		char array[128];    // Local fixed size array

		ov_callbacks callbacks;
		callbacks.close_func = OggFileClose;
		callbacks.read_func = OggFileRead;
		callbacks.seek_func = OggFileSeek;
		callbacks.tell_func = OggFileTell;

		sOggCallbacksInfo oci;
		oci.fileSystem = pFS;
		oci.file = fp;

		ov_open_callbacks(&oci, &oggFile, NULL, 0, callbacks);
		pInfo = ov_info(&oggFile, -1);
		m_info.numChannels = pInfo->channels;
		m_info.freq = pInfo->rate;
		m_info.bitsPerSample = 16;

		do 
		{
			bytes = ov_read(&oggFile, array, 128, endian, 2, 1, &bitStream);
			m_buffer.insert(m_buffer.end(), array, array + bytes);
		} while (bytes > 0);

		ov_clear(&oggFile);
		
		//pFS->Close(fp); //Closed automatically in OggCloseFile callback
		m_bInit = true;
		return true;
	}

	void COggLoader::Unload()
	{
		m_info.numChannels = 0;
		m_info.freq = 0;
		m_buffer.clear();
		m_bInit = false;
	}


	size_t COggLoader::OggFileRead(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		sOggCallbacksInfo* oci = (sOggCallbacksInfo*)datasource;
		size_t readed = oci->fileSystem->Read(ptr,size,nmemb,oci->file);
		//if(
		return readed;
	}

	int COggLoader::OggFileSeek(void *datasource, ogg_int64_t offset, int whence)
	{
		return -1; //Seeking is not supported
	}

	int COggLoader::OggFileClose(void *datasource)
	{
		sOggCallbacksInfo* oci = (sOggCallbacksInfo*)datasource;
		return oci->fileSystem->Close(oci->file);
	}

	long COggLoader::OggFileTell(void *datasource)
	{
		sOggCallbacksInfo* oci = (sOggCallbacksInfo*)datasource;
		return oci->fileSystem->Tell(oci->file);
	}

}