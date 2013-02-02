/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "filesystem.h"
#include "Filesystem_stdio.h"
#include "IEngine.h"
#include <stdio.h>

// exists

#include <sys/stat.h>
#include <sys/types.h>

namespace P3D
{

CFS_Stdio::~CFS_Stdio()
{
}

bool CFS_Stdio::Initialize(void* pEngine)
{
	CON(MSG_INFO, _W("StdIO filesystem"));
	CON(MSG_INFO, _W("... successfuly initialized"));
	return true;
}

//------------------------------------------
FSFILE* CFS_Stdio::Open(const wchar* szPath, const wchar* szMode){
	wchar szPhPath[MAX_PATH];

	// construct physical path
	wstrcpy(szPhPath, I_GetBinaryPath());
	wstrcat(szPhPath, _W("../data/"));
	wstrcat(szPhPath, szPath);

	FILE* fp = wfopen(szPhPath, szMode);
	if (!fp) 
	{
		// if we are opening for write mode, make sure path to file is created
		if (szMode[0]=='w')
		{
			wchar szdir[MAX_PATH];
			wstrcpy(szdir, szPhPath);
			for (unsigned int i=0;i<wstrlen(szdir);i++)
				if (szdir[i]=='/'||szdir[i]=='\\') { 
					szdir[i]=0;
#ifdef WIN32
				CreateDirectory(szdir, NULL) ? 0 : 1;
#else
				mkdir(szdir,777);
#endif
				szdir[i]=szPhPath[i];
			}
			fp = wfopen(szPhPath, szMode); // try to open again
			if (!fp) return NULL;
		}
		else
			return NULL;
	}

	FSFILE* ret = new FSFILE;
	ret->bArchive = false;
	ret->nOffset = ret->nRelOffset = 0;

	fseek(fp, 0, SEEK_END);
	ret->nLen =  ftell(fp);
	fseek(fp, 0, SEEK_SET);

	ret->pFileData = fp;

	return ret;
}

//------------------------------------------
int CFS_Stdio::Close(FSFILE* file){
	if (!file) return -1;

	FILE* fp = (FILE*)file->pFileData;
	delete file;

	if (fp) return fclose(fp);

	return -1;
}

//------------------------------------------
FSFILE* CFS_Stdio::Load (IN const wchar *szFileName, OUT BYTE* &pData, OUT ULONG &size, bool bNullTerminate/*=false*/){
	FSFILE* fp = this->Open(szFileName, _W("rb"));
	if (fp)
	{
		this->Seek(fp, 0, SEEK_END);
		size = this->Tell(fp);
		this->Seek(fp, 0, SEEK_SET);
		bNullTerminate ? pData = new BYTE[size+1] : pData = new BYTE[size];
		this->Read(pData, 1, size, fp);
		if (bNullTerminate) pData[size]=NULL;
	}
	return fp;
}


//------------------------------------------
void CFS_Stdio::UnLoad (FSFILE* pFileToUnload, BYTE *pData /*= NULL*/){
	assert(pFileToUnload!=NULL);
	if (pData) SAFE_DELETE_ARRAY(pData);
	this->Close(pFileToUnload);
	return;
}


//------------------------------------------
size_t CFS_Stdio::Read(void* data, size_t size, size_t count, FSFILE* fp){
	assert(fp->pFileData!=NULL);
	return fread(data, size, count, (FILE*)fp->pFileData);
}

//------------------------------------------
void CFS_Stdio::Seek(FSFILE *fp, long pos, int type){
	assert(fp->pFileData!=NULL);
	fseek((FILE*)fp->pFileData, pos, type);
	fp->nOffset = ftell((FILE*)fp->pFileData);
}

//------------------------------------------
int CFS_Stdio::Eof(FSFILE *fp){
	assert(fp->pFileData!=NULL);
	return feof((FILE*)fp->pFileData);
}

//------------------------------------------
size_t CFS_Stdio::Write(const void *data, size_t size, size_t count, FSFILE *fp){
	assert(fp->pFileData!=NULL);
	return fwrite(data, size, count, (FILE*)fp->pFileData);
}

//------------------------------------------
size_t CFS_Stdio::WriteUTF8(const wchar* string, FSFILE *fp)
{
	if (!string || !*string) return 0;

	// if not unicode
#ifndef UNICODE
	return Write(string, 1, strlen(string), fp);
#else

	unsigned int inputLen = wstrlen(string);
	unsigned char *target;
	target = new unsigned char[inputLen*4+1]; // make buffer long enough
	UErrorCode status = U_ZERO_ERROR;
	int32_t     len;

	//// set up the converter
	UConverter *conv = ucnv_open("utf-8", &status);
	assertd(U_SUCCESS(status)!=0, "Failed to open utf-8 converter!");

	//// convert 
	len = ucnv_fromUChars(conv, (char*)target, inputLen*4, string, -1, &status);
	assertd(U_SUCCESS(status)!=0, "Failed to convert string to utf-8!");

	//// write to file
	size_t wrlen = Write(target, 1, len, fp);

	//// close and clean
	delete[] target;
	ucnv_close(conv);

	return wrlen;
#endif
}

//------------------------------------------
int CFS_Stdio::Flush(FSFILE *fp){
	assert(fp->pFileData!=NULL);
	return fflush((FILE*)fp->pFileData);
}

//------------------------------------------
long CFS_Stdio::Tell(FSFILE *fp){
	assert(fp->pFileData!=NULL);
	return fp->nOffset; // must be actual position in stdio
}

//------------------------------------------
int CFS_Stdio::Getc(FSFILE *fp){
	assert(fp->pFileData!=NULL);
	return fgetc((FILE*)fp->pFileData);
}

//------------------------------------------
int CFS_Stdio::Putc(int chr, FSFILE *fp){
	assert(fp->pFileData!=NULL);
	int ret = fputc(chr,(FILE*)fp->pFileData);
	fp->nOffset = ftell((FILE*)fp->pFileData);
	return ret;
}

//------------------------------------------
int CFS_Stdio::Puts(const wchar* string, FSFILE* fp){
	assert(fp->pFileData!=NULL);
	int ret = wfputs(string, (FILE*)fp->pFileData);
	fp->nOffset = ftell((FILE*)fp->pFileData);
	return ret;
}

//------------------------------------------
bool CFS_Stdio::Exists(const wchar* file){
	assert(file!=NULL && file[0]!=0);

	wchar szPhPath[MAX_PATH];

	// construct physical path
	wstrcpy(szPhPath, file);

	struct _stat mystat;
	if (!wstat(szPhPath,&mystat))
		return true;
	else
		return false;
}

//------------------------------------------
tArray<sFSSearchResult> CFS_Stdio::GetContentOfCategory(const wchar *path)
{
	tArray<sFSSearchResult> ret;

	// construct physical path
	wchar szPhPath[MAX_PATH];
	wsprintf(szPhPath, MAX_PATH-1, _W("%s../data/%s/*"), I_GetBinaryPath(), path );

#ifdef _WIN32

	WIN32_FIND_DATA wfd;
	ZeroMemory(&wfd, sizeof(wfd));
	HANDLE hFind = FindFirstFile(szPhPath, &wfd);
	if ( hFind == INVALID_HANDLE_VALUE )
		return ret;

	do 
	{
		if (!*wfd.cFileName || wfd.cFileName[0]==_W('.')) continue;

		sFSSearchResult sr;

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			sr.Type = FSI_DIR;
		else
			sr.Type = FSI_FILE;

		assert( wstrlen(wfd.cFileName)<64 );
		wstrcpy(sr.Name, wfd.cFileName);

		ret.AddEx(sr);
	} while ( FindNextFile(hFind, &wfd) );

	FindClose(hFind);

#else
	// TODO:!!
	#error todo
#endif
	return ret;
}


};
