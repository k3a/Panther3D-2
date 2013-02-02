/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "IFilesystem.h"
#include "IEngine.h"

namespace P3D{

#define MAX_FS 4

class CFS : public IFileSystem
{
public:
	CFS();
	~CFS();
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

	//--- external modules
	static inline IEngine* mEngine(){ return s_pEngine; };
private:
	IFileSystem* m_pFS[MAX_FS];

	//--- external modules
	static IEngine* s_pEngine;

#if defined(_INTERNAL)
	tArray<FSFILE*> m_pOpenedFiles;
#endif
};

#define CON(type, text, ...) CFS::mEngine()->iConsole()->AddMsgEx(type, _W(__FUNCTION__), text, ##__VA_ARGS__)

}; // namespace