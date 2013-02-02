/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/

#pragma once

#include "interface.h"

namespace P3D{

///Internal filesystem file structure.
/** Please don't alter it directly, use filesystem methods. But you can read some useful information like nLen and bArchive.  */
struct FSFILE
{
	///Internal file data, should be FILE on Windows
	void* pFileData;
	///Relative offset in archive file when using archive subsystem, so current file position will be nOffset+nRelOffset.
	unsigned long nRelOffset;
	///Offset in current file if using stdio (hdd), offset in archive file when using archive
	unsigned long nOffset;
	///File length in bytes
	unsigned long nLen;
	///true=using "archive" subsystem, false=using stdio (hdd)
	bool bArchive;
	/// Filesystem class identification
	/** Determines which filesystem class ID (Stdio, Rar, Zip, ...) is used for this file inside module. */
	BYTE fsID;

#if defined(_DEBUG)
	wchar dbgPath[P3DMAX_PATH]; // only for debug
#endif
};

#define	FILE_ATTRIBUTE_PACK		0x10000000 // u FindFirstFile jako atribut vysledku v pripade ze je to soubor ze 7pk archivu

// seek type constants
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/// Type of filesystem item
enum eFSItemType
{
	FSI_UNKNOWN=0,
	FSI_FILE,
	FSI_DIR
};

/// Result of filesystem search or directory iteration
struct sFSSearchResult
{
	/// Name of item
	wchar Name[64];
	/// Type of item
	eFSItemType Type;
	/// Filesystem which contains this item
	BYTE fsID;
};

/// File Input/Output operations
/** Reading can be done using any filesystem plugin. Writing is done only by stdio standard plugin.
    There is no need to save data like save games and screenshots to archives. ;) */
class IFileSystem : public IBaseInterface
{
public:
	/// Initialize all the filesystem classes in FS library
	/** Must be called after core initialization (console must be available).
	\return True when all the internal classes sucessfully initialized. */
	virtual bool Initialize(void* pEngine)=0;

	///Open file using filesystem.
	/**
	It will return NULL if file couldn't be loaded, for example doesn't exists.
	\param szPath Path to file being opened.
	\param szMode Open mode equal to stdio (rb,wb,r,w,a).
	\see Close */
	virtual FSFILE* Open(const wchar* szPath, const wchar* szMode)=0;

	///Close previously opened Filesystem File
	/** It must be closed in plugin where it was opened, so it will return -1 when it is't correct plugin to close file.
	\return 0 on success, -1 on failure. \see Open */
	virtual int Close(FSFILE* file)=0;

	///Load all file into memory
	/** This method loads all content of file to memory. Memory will be created dynamically and pointer will be returned 
	to reference 'pData'. Size will be returned in 'size'. Please don't change returned 'pData' and 'size'!

	You can set parameter bNullTerminate to true if you want to allocate one more byte and set it to NULL. It is useful
	for loading string data as it makes correct null-terminated string.

	In both cases (null-terminated or not) real file size will be returned.

	<b>Example of use</b><br>
	<pre>
	BYTE *pData=NULL; unsigned long size;
	FSFILE* fp = fs->Load("myfile.dat", pData, size, false);
	// use loaded data in 'pData' with length of 'size'.
	fs->UnLoad(fp, pData);
	</pre>
	
	\warning Must be unloaded by calling UnLoad
	\param szFileName Path to file being loaded.
	\param pData Empty pointer to BYTE
	\param bNullTerminate Should be loaded data terminated with NULL? It is useful while loading string data.
	\size Size variable in which file size will be returned.
	\see UnLoad */
	virtual FSFILE* Load (IN const wchar *szFileName, OUT BYTE* &pData, OUT ULONG &size, bool bNullTerminate=false)=0;

	///Unload data previously allocated by Load().
	/** \see Load */
	virtual void UnLoad (FSFILE* pFileToUnload, BYTE *pData = NULL)=0;

	///Read data from file.
	virtual size_t Read(void* data, size_t size, size_t count, FSFILE* fp)=0;

	///Seek file position to pos bytes.
	virtual void Seek(FSFILE *fp, long pos, int type)=0;

	///Was end-of-file reached?
	virtual int Eof(FSFILE *fp)=0;

	///Write data to file.
	virtual size_t Write(const void *data, size_t size, size_t count, FSFILE *fp)=0;

	///Write Unicode string as UTF-8 to file
	/** String must be null-terminated!! 
	\return Number of bytes written will be returned. */
	virtual size_t WriteUTF8(const wchar* string, FSFILE *fp)=0;

	///Immediately flush file contents on HDD.
	virtual int Flush(FSFILE *fp)=0;

	///Get actual position in file.
	virtual long Tell(FSFILE *fp)=0;

	///Get character from file.
	virtual int Getc(FSFILE *fp)=0;

	///Put character into file.
	virtual int Putc(int chr, FSFILE *fp)=0;

	///Put string into file.
	virtual int Puts(const wchar* string, FSFILE* fp)=0;

	///File exists in initialized filesystem plug-ins?
	virtual bool Exists(const wchar* file)=0;

	///Get content of category 
	/** Input path must contain asset type i.e. textures/ and optional category path.
	Example of good path is textures/doors
	
	\warning item names beginning with . are skipped! */
	virtual tArray<sFSSearchResult> GetContentOfCategory(const wchar *path)=0;
};

}; // namespace