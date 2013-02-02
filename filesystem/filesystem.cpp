/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "filesystem.h"
#include "IEngine.h"

#include "Filesystem_stdio.h"
#include "filesystem_sqlite.h"

namespace P3D
{
	REGISTER_MODULE_CLASS(CFS, _W("filesystem"), _W("stdio")); // TODO: rename to internal

	IEngine* CFS::s_pEngine=NULL;

	CFS::CFS()
	{
		memset(m_pFS, 0, MAX_FS*sizeof(IFileSystem*));
		m_pFS[0] = new CFS_Stdio();
		m_pFS[1] = new CFS_Sqlite();
	}

	CFS::~CFS()
	{
#if defined(_INTERNAL)
		for(unsigned int i=0; i<m_pOpenedFiles.Size(); i++)
		{
			if (m_pOpenedFiles[i])
			{
				CON(MSG_ERR, _W("Warning: Filesystem file %s was not closed!"), m_pOpenedFiles[i]->dbgPath);
				Close(m_pOpenedFiles[i]);
			}
		}
#endif
		for (int i=0; i<MAX_FS; i++) if (m_pFS[i]) delete m_pFS[i];
	}

	bool CFS::Initialize(void* pEngine)
	{
		// memory leaks detection
#if defined(_DEBUG) && defined(_MSC_VER) && _MSC_VER >= 800
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		_CrtSetBreakAlloc(78);
#endif

		s_pEngine = (IEngine*)pEngine;

		CON(MSG_INFO, _W("= Filesystem initialization ="));

		bool r=true;
		for (unsigned int i=0; i<MAX_FS; i++)
		{
			if(m_pFS[i]) 
			{
				if(!m_pFS[i]->Initialize(pEngine))
				{
					CON(MSG_ERR, _W("Filesystem class #%d failed to initialize!"), i);
					delete m_pFS[i]; m_pFS[i]=NULL;
					r=false;
				}
			}
		}
		return r;
	}

	FSFILE* CFS::Open(const wchar* szPath, const wchar* szMode)
	{
		assert(szPath!=NULL&&szPath[0]!=0&&szMode!=NULL&&szMode[0]!=0);
		assertd(wstrlen(szPath)<P3DMAX_PATH, "Filesystem path must be shorter than P3DMAX_PATH!!");
		for (unsigned int i=0; i<MAX_FS; i++)
		{
			if(m_pFS[i]) // filesystem class available
			{
				//Try to open file using this filesystem class
				FSFILE* pf = m_pFS[i]->Open(szPath, szMode);
				if (pf) 
				{
					pf->fsID = i; // assign fs class id
#if defined(_INTERNAL)
					// add to list of opened files
					wstrcpy(pf->dbgPath, szPath);
					m_pOpenedFiles.AddEx(pf);
#endif
					return pf; // success
				}
			}
		}
		return NULL; // cannot be opened using any fs class
	}

	int CFS::Close(FSFILE* file)
	{
#if defined(_INTERNAL)
		// remove from opened files list
		int idx = m_pOpenedFiles.FindIndex(file);
		if (idx!=-1) m_pOpenedFiles[idx] = NULL;
#endif
		// close file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) return m_pFS[file->fsID]->Close(file);
		return -1;
	}

	FSFILE* CFS::Load (IN const wchar *szFileName, OUT BYTE* &pData, OUT ULONG &size, bool bNullTerminate)
	{
		for (unsigned int i=0; i<MAX_FS; i++)
		{
			if(m_pFS[i]) // filesystem class available
			{
				//Try to open file using this filesystem class
				FSFILE* pf = m_pFS[i]->Load(szFileName, pData, size, bNullTerminate);
				if (pf) 
				{
					pf->fsID = i; // assign fs class id
					return pf; // success
				}
			}
		}
		return NULL; // cannot be loaded using all the classes
	}

	void CFS::UnLoad (FSFILE* file, BYTE *pData)
	{
		// unload file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) m_pFS[file->fsID]->UnLoad(file,pData);
	}

	size_t CFS::Read(void* data, size_t size, size_t count, FSFILE* file)
	{
		// read file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) return m_pFS[file->fsID]->Read(data,size,count,file);
		return 0;
	}

	void CFS::Seek(FSFILE *file, long pos, int type)
	{
		// avoid ordinary unnecessary seeks
		if (type==SEEK_CUR && pos==0) return;
		else if (type==SEEK_SET && pos == file->nRelOffset) return;

		// seek file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) m_pFS[file->fsID]->Seek(file,pos,type);
	}

	int CFS::Eof(FSFILE *file)
	{
		// check end of file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) return m_pFS[file->fsID]->Eof(file);
		return 1;
	}

	size_t CFS::Write(const void *data, size_t size, size_t count, FSFILE *file)
	{
		assert(file!=NULL);
		// write to file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) return m_pFS[file->fsID]->Write(data,size,count,file);
		return 0;
	}

	size_t CFS::WriteUTF8(const wchar* string, FSFILE *file)
	{
		assert(file!=NULL);
		// write to file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) return m_pFS[file->fsID]->WriteUTF8(string,file);
		return 0;
	}

	int CFS::Flush(FSFILE *file)
	{
		assert(file!=NULL);
		// flush file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) return m_pFS[file->fsID]->Flush(file);
		return 0;
	}

	long CFS::Tell(FSFILE *file)
	{
		assert(file!=NULL);
		// Get position in file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) return m_pFS[file->fsID]->Tell(file);
		return 0;
	}

	int CFS::Getc(FSFILE *file)
	{
		assert(file!=NULL);
		// get wchar from file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) return m_pFS[file->fsID]->Getc(file);
		return 0;
	}

	int CFS::Putc(int chr, FSFILE *file)
	{
		assert(file!=NULL);
		// flush file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) return m_pFS[file->fsID]->Putc(chr,file);
		return 1;
	}

	int CFS::Puts(const wchar* string, FSFILE* file)
	{
		assert(file!=NULL);
		// put string to file using class which created (opened) it
		if (file->fsID<MAX_FS && m_pFS[file->fsID]) return m_pFS[file->fsID]->Puts(string,file);
		return 1;
	}

	bool CFS::Exists(const wchar* file)
	{
		assert(file!=NULL&&*file);
		assertd(wstrlen(file)<P3DMAX_PATH, "Filesystem path must be shorter than P3DMAX_PATH!!");
		// check if file exists in at least one filesystem class
		for (unsigned int i=0; i<MAX_FS; i++)
		{
			if(m_pFS[i]) // filesystem class available
			{
				if(m_pFS[i]->Exists(file)) return true; // exists
			}
		}
		return false;
	}

	tArray<sFSSearchResult> CFS::GetContentOfCategory(const wchar *path)
	{
		assert(path && *path);

		tArray<sFSSearchResult> ret;
		for (unsigned int i=0; i<MAX_FS; i++)
		{
			if(m_pFS[i]) // filesystem class available
			{
				tArray<sFSSearchResult> curr = m_pFS[i]->GetContentOfCategory(path);
				for (unsigned int i=0; i<curr.Size(); i++)
				{
					curr[i].fsID = i;
				}
				if (curr.Size()>0) ret.AppendEx(curr);
			}
		}

		return ret;
	}

	
}; // namespace