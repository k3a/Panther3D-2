/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "IFilesystem.h"

namespace P3D
{

class CFS_Stdio : public IFileSystem
{
public:
	~CFS_Stdio();
	bool Initialize(void* pEngine);
	FSFILE* Open(const wchar* szPath, const wchar* szMode);
	int Close(FSFILE* file);
	FSFILE* Load (IN const wchar *szFileName, OUT BYTE* &pData, OUT ULONG &size, bool bNullTerminate=false);
	void UnLoad (FSFILE* pFileToUnload, BYTE *pData = NULL);
	size_t Read(void* data, size_t size, size_t count, FSFILE* fp);
	void Seek(FSFILE *fp, long pos, int type);
	int Eof(FSFILE *fp);
	size_t Write(const void *data, size_t size, size_t count, FSFILE *fp);
	size_t WriteUTF8(const wchar* string, FSFILE *fp);
	int Flush(FSFILE *fp);
	long Tell(FSFILE *fp);
	int Getc(FSFILE *fp);
	int Putc(int chr, FSFILE *fp);
	int Puts(const wchar* string, FSFILE* fp);
	bool Exists(const wchar* file);
	tArray<sFSSearchResult> GetContentOfCategory(const wchar *path);
};

};

