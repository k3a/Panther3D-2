/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 11/12/2009 File created - Petr
*/

#pragma once

#include "types.h"
#include "mutex.h"

namespace P3D
{
	///Indicates the result of resource loading/saving.
	enum eFileResult
	{
		///Succeeded.
		P3D_FILERESULT_OK,
		///An error occurred.
		P3D_FILERESULT_FAILED,
		///Filesystem is currently busy. Probably being used by another thread.
		P3D_FILERESULT_BUSY
	};


	class CFilesystem
	{
	public:
		CFilesystem();
		~CFilesystem();
		bool Init();
		eFileResult Load(const wchar *name, void** outData, UINT &outSizeB, bool blocking = true);
		void FreeLoadedData(void *data);
		eFileResult Save(const wchar *name, const void *data, UINT dataSizeB, bool append = false, bool blocking = true);

	private:
		void ConvertToStdioPath(const wchar *inName, wchar *outPath);

		CMutex m_mutex;

	};


}; // namespace