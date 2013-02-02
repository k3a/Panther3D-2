/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 08/28/2009 File created - kexik
*/
#include "precompiled.h"

#include "filesystem.h"

#include "filesystem_sqlite.h"
#include "sqlite/sqlite3.h"

namespace P3D{

#define DBCALL(db, cmd) if (##cmd##!=SQLITE_OK) CON( MSG_ERR, _A2W(sqlite3_errmsg(db)) );

//---------------------------------------------------------------------------
CFS_Sqlite::~CFS_Sqlite()
{
	CON(MSG_INFO, _W("Database filesystem sqlite shutdown") );
	sqlite3_shutdown();
}

//---------------------------------------------------------------------------
bool CFS_Sqlite::Initialize(void* pEngine)
{
	int ret=0;

	CON(MSG_INFO, _W("Database filesystem sqlite %s:"), _A2W(sqlite3_version) );

	if (!sqlite3_threadsafe())
	{
		CON(MSG_ERR, _W("... not compiled thread-safe - will not be used!"));
		return false;
	}

	CON(MSG_INFO, _W("... setting multithread configuration"));
	sqlite3_config(SQLITE_CONFIG_MULTITHREAD);

	ret = sqlite3_initialize();
	if (ret == SQLITE_OK)
		CON(MSG_INFO, _W("... successfuly initialized"));
	else
	{
		CON(MSG_ERR, _W("... error code %d returned - will not be used!"), ret);
		return false;
	}

	return true;
}

//------------------------------------------
FSFILE* CFS_Sqlite::Open(const wchar* szPath, const wchar* szMode){
	wchar szPhPath[MAX_PATH];

	// construct physical path
	wstrcpy(szPhPath, I_GetBinaryPath());
	wstrcat(szPhPath, _W("../data/"));
	wstrcat(szPhPath, szPath);

	//FILE* fp = wfopen(szPhPath, szMode);
	// TODO: implement!
	// don't forget to create if write mode and doesn't exist
	// on error / not found return NULL
	return NULL; // until implemented
	
	FSFILE* ret = new FSFILE;
	ret->bArchive = false;
	ret->nOffset = ret->nRelOffset = 0;

	//fseek(fp, 0, SEEK_END);
	ret->nLen =  0; // TODO: implement!
	//fseek(fp, 0, SEEK_SET);

	//ret->pFileData = fp;

	return ret;
}

//------------------------------------------
int CFS_Sqlite::Close(FSFILE* file){
	if (!file) return -1;

	//FILE* fp = (FILE*)file->pFileData;
	delete file;

	// TODO: implement!

	return -1;
}

//------------------------------------------
FSFILE* CFS_Sqlite::Load (IN const wchar *szFileName, OUT BYTE* &pData, OUT ULONG &size, bool bNullTerminate/*=false*/){
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
void CFS_Sqlite::UnLoad (FSFILE* pFileToUnload, BYTE *pData /*= NULL*/){
	assert(pFileToUnload!=NULL);
	if (pData) SAFE_DELETE_ARRAY(pData);
	this->Close(pFileToUnload);
}


//------------------------------------------
size_t CFS_Sqlite::Read(void* data, size_t size, size_t count, FSFILE* fp){
	assert(fp->pFileData!=NULL);

	// TODO: implement!
	return -1;
}

//------------------------------------------
void CFS_Sqlite::Seek(FSFILE *fp, long pos, int type){
	assert(fp->pFileData!=NULL);
	
	// TODO: implement!
	//fp->nOffset = ftell((FILE*)fp->pFileData);
}

//------------------------------------------
int CFS_Sqlite::Eof(FSFILE *fp){
	assert(fp->pFileData!=NULL);

	// TODO: implement!
	return -1;
}

//------------------------------------------
size_t CFS_Sqlite::Write(const void *data, size_t size, size_t count, FSFILE *fp){
	assert(fp->pFileData!=NULL);

	// TODO: implement!
	return 0;
}

//------------------------------------------
size_t CFS_Sqlite::WriteUTF8(const wchar* string, FSFILE *fp)
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
int CFS_Sqlite::Flush(FSFILE *fp){
	assert(fp->pFileData!=NULL);

	// TODO: implement!
	return -1;
}

//------------------------------------------
long CFS_Sqlite::Tell(FSFILE *fp){
	assert(fp->pFileData!=NULL);

	// TODO: implement!
	return -1;
}

//------------------------------------------
int CFS_Sqlite::Getc(FSFILE *fp){
	assert(fp->pFileData!=NULL);

	// TODO: implement!
	return -1;
}

//------------------------------------------
int CFS_Sqlite::Putc(int chr, FSFILE *fp){
	assert(fp->pFileData!=NULL);
	
	// TODO: implement!
	return -1;
}

//------------------------------------------
int CFS_Sqlite::Puts(const wchar* string, FSFILE* fp){
	assert(fp->pFileData!=NULL);
	
	// TODO: implement!
	return -1;
}

//------------------------------------------
bool CFS_Sqlite::Exists(const wchar* file){
	assert(file!=NULL && file[0]!=0);

	// TODO: implement!
	return false;
}

//------------------------------------------
tArray<sFSSearchResult> CFS_Sqlite::GetContentOfCategory(const wchar *path)
{
	// TODO: implement!
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

}; // ns