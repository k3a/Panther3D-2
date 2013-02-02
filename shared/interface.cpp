/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "interface.h"
#include <string>
#include <vector>
#include <time.h>

#if defined(LINUX)
#include <sys/types.h>
#include <dirent.h>
#endif

#ifdef LINUX
#include <dlfcn.h>
#endif

namespace P3D
{

CClassManager *g_pClsMgr=NULL;
sModule* g_pLastModule=NULL;

static SVarCategory* g_pLastSVarCat=NULL; // this module's last SVar category

//-----------------------------------------------------------------------------
// CLASS MANAGER
//-----------------------------------------------------------------------------

// helper to output debug text
void DbgPrint(const wchar* text, ...)
{
#if defined(_DEBUG) && defined(WIN32)
	wchar buffer[1024];
	try
	{
		va_list argList;
		va_start(argList, text);
		wvsnprintf(buffer,1023,text,argList);
		va_end(argList);
	}
	catch(...)
	{
		return;
	}

	// time using standard library :)
   /* time_t rawtime;
    struct tm * ti;

    time ( &rawtime );
    ti = localtime ( &rawtime );*/


	wchar* buffer2=new wchar[wstrlen(buffer)+50];
	//wsprintf(buffer2, "IFACE : %s\r\n", ti->tm_hour, ti->tm_min, ti->tm_sec, s_szModuleName, buffer);
	wsprintf(buffer2, wstrlen(buffer)+49, _W("IFACE : %s\r\n"), buffer);

	OutputDebugString(buffer2);

	delete[] buffer2;
#endif
}

CClassManager::CClassManager()
{
	wstrcpy(m_szBinPath, _W("./"));
	m_nLastInitID=0;
}

bool CClassManager::LoadNewLibrary(const wchar* szLibraryName)
{
	if (!szLibraryName || !*szLibraryName) return false;

	wchar BinPath[MAX_PATH];

	bool bAbsolutePath = false;
	for(unsigned int i = wstrlen(szLibraryName); i>0; i--)
	{
		if (szLibraryName[i]=='\\' || szLibraryName[i]=='/')
		{
			bAbsolutePath = true;
			break;
		}
	}

	if (bAbsolutePath)
	{
		// check extension and copy
		if (wstrlen(szLibraryName)<5 || szLibraryName[wstrlen(szLibraryName)-5]!='.')
#if defined(WIN32)		
			wsprintf(BinPath, 259, _W("%s.wp3d"), szLibraryName);
#elif defined (LINUX)
			wsprintf(BinPath, 259, _W("%s.lp3d"), szLibraryName);
#endif
		else
			wstrcpy(BinPath, szLibraryName); // copy abspath with extension
	}
	else // make new path based on <bin_path>/<library_name>.xp3d
	{

#if defined(WIN32)
	if (wstrlen(szLibraryName)<5 || szLibraryName[wstrlen(szLibraryName)-5]!='.') 
		wsprintf(BinPath, 259, _W("%s%s.wp3d"), m_szBinPath, szLibraryName);
#elif defined (LINUX)
		wsprintf(BinPath, 259, _W("%s%s.lp3d"), m_szBinPath, szLibraryName);
#endif

	}


    #ifdef WIN32
	HMODULE hDLL = LoadLibrary(BinPath);
	#elif LINUX
	HMODULE hDLL = dlopen(BinPath, RTLD_LAZY);
	#endif

	if (!hDLL) // Library cannot be loaded
	{
		DbgPrint(_W("LOAD NEW MODULE: Cannot load library: %s"), BinPath);
		return false;
	}

	#ifdef WIN32
	LibraryEntrypoint dllEntry = (LibraryEntrypoint)GetProcAddress(hDLL, ENTRYPOINT_NAME_STR);
	#elif LINUX
    LibraryEntrypoint dllEntry = (LibraryEntrypoint)dlsym(hDLL, ENTRYPOINT_NAME_STR);
	#endif

	if (dllEntry!=NULL)
	{
		sLibrary lib;
		lib.hLibrary = hDLL;
		wstrcpy(lib.szFileName, BinPath);

		DbgPrint(_W("LOAD : Loading module '%s'"), BinPath);

		// Sent this class manager instance to loaded module
		dllEntry(this);
		
		// copy module main class sModule pointer
		lib.pFirstModule = dllEntry(0);

		// add SVars from module to this main module
		for (SVarCategory* svc = (SVarCategory*)dllEntry((CClassManager *)1);svc;svc=svc->next)
		{
			bool bFound=false;
			// try to find existing category
			for (SVarCategory* svcm = g_pLastSVarCat;svcm;svcm=svcm->next)
			{
				if (!wstricmp(svcm->GetName(), svc->GetName())) 
				{
					svcm->AddSVarsFromCategory(svc);
					bFound = true;
					break;
				}
			}

			// dont exist, add
			if (!bFound)
			{
				SVarCategory* svcn = new SVarCategory(svc->GetName());
				svcn->AddSVarsFromCategory(svc);
			}
		}

		m_libs.push_back(lib);
		return true;
	}
	else
	{
		DbgPrint(_W("LOAD NEW MODULE : Panther3D entrypoint not found in module %s"), BinPath);
		#if defined(WIN32)
		FreeLibrary(hDLL);
		#elif defined(LINUX)
		dlclose(hDLL);
		#endif
		return false;
	}
}

void CClassManager::ScanModulesLinux()
{
    #if defined(LINUX)

	DIR * dir = opendir (m_szBinPath);

	if (dir == NULL){
		//fprintf (stderr, "Unable to open the directory.\n");
		MessageBox(0, _W("Unable to open the current working directory! Can't scan for modules!"), _W("FATAL ERROR!"), 0);
		exit ( 1 );
	}

	struct dirent *d = readdir (dir);
	while (d != NULL){
		//printf (_W("\"%s\"\n"), d->d_name);
        if (wstrcmp (d->d_name, _W(".")) != 0 &&
            wstrcmp (d->d_name, _W("..")) != 0)
		{
            if (strstr(d->d_name, _W(".lp3d")))
            {
                wchar BinPath[MAX_PATH];
                wstrcpy(BinPath, m_szBinPath);
                wstrcat(BinPath, d->d_name);

                HMODULE hDLL = dlopen(BinPath, RTLD_LAZY);
                if (!hDLL) // cannot be loaded, skip
                {
                    MessageBox(0, dlerror(), _W("Library loading failed"),  MB_SYSTEMMODAL);
                    d = readdir (dir);
                    continue;
                }
                LibraryEntrypoint dllEntry = (LibraryEntrypoint)dlsym(hDLL, ENTRYPOINT_NAME_STR);
                if (dllEntry!=NULL)
                {
                    if (m_nNumModules == MAX_MODULES-1)
                    {
                        MessageBox(0, _W("Maximum number of project modules exceeded!"), _W("MAX_MODULES"),  MB_SYSTEMMODAL);
                        return;
                    }

                    DbgPrint(_W("LOAD : Loading module '%s'"), d->d_name);

                    m_modules[m_nNumModules] = hDLL; // copy module handle to list of modules

                    // Sent this class manager instance to loaded module
                    dllEntry(this);

                    // copy module classes to global holder
                    if (!m_pLastHolder)
                        m_pLastHolder = dllEntry(NULL);
                    else
                    {
                        ClassHolder *pTmpHold;
                        ClassHolder *pLast;
                        pLast = m_pLastHolder;
                        for(pTmpHold=m_pLastHolder; pTmpHold->m_pNext && pTmpHold->m_pNext!=m_pLastHolder;pTmpHold=pTmpHold->m_pNext) continue;
                        pTmpHold->m_pNext = dllEntry(NULL);
                    }

                    m_nNumModules++;
                }
                else
                    dlclose(hDLL);
            }
		}

		d = readdir (dir);
	}

	closedir (dir);

    #endif
}

void CClassManager::ScanModulesWindows()
{
    #if defined(WIN32)

	// ---> SCAN FOR MODULES AND TRY TO LOAD ALL

	int nValid;
	HANDLE searchHandle=NULL;
	WIN32_FIND_DATA fileData;

	wchar pathToSearch[MAX_PATH];
	wstrcpy(pathToSearch, m_szBinPath);
	wstrcat(pathToSearch, _W("*.wp3d"));

	searchHandle = FindFirstFile(pathToSearch, &fileData);
	nValid = (searchHandle == INVALID_HANDLE_VALUE) ? 0 : 1;

	while (nValid)
	{
		if (wstricmp (fileData.cFileName, _W(".") ) != 0 &&
			wstricmp (fileData.cFileName, _W("..") ) != 0)
		{
			// check if this is engine module
			wchar BinPath[512];
			wstrcpy(BinPath, m_szBinPath);
			wstrcat(BinPath, fileData.cFileName);

			LoadNewLibrary(BinPath);
		}
		nValid = FindNextFile(searchHandle, &fileData);
	}

	FindClose(searchHandle);

	#endif
}

void CClassManager::Initialize(const wchar *szBinPath)
{
	DbgPrint(_W("INITIALIZE : Loading modules from '%s'"), szBinPath);

    wstrcpy(m_szBinPath, szBinPath);
    if (m_szBinPath[wstrlen(m_szBinPath)-1]!='\\' && m_szBinPath[wstrlen(m_szBinPath)-1]!='/') wstrcat(m_szBinPath, _W("\\"));

	g_pLastModule=NULL;

    // PROCESS ALL FILES IN EXE DIRECTORY AND LOAD LIBRARIES
    #if defined(WIN32)
    ScanModulesWindows();
    #elif defined(LINUX)
    ScanModulesLinux();
    #endif

	if (m_libs.size()==0)
	{
		MessageBox(0, _W("Can't load any modules! Bad path? Modules doesn't exist?"), _W("FATAL ERROR!"), MB_ICONSTOP | MB_SYSTEMMODAL);
		exit(-1);
	}
}

// -----------------------------------------------------------------------------------
void* CClassManager::GetModule(const wchar* moduleIdentifier, const wchar* szImplementationName)
{
	// ---GO THROUGH ALL ENABLED LIBS AND MODULE CLASSES AND TRY TO FIND MODULE BY IT'S IDENTIFIER 
	for (unsigned int libI=0; libI<m_libs.size(); libI++) // library iteration
	{
		for(sModule *pMod=m_libs[libI].pFirstModule; pMod; pMod=pMod->m_pNext)
		{
			if(wstricmp(pMod->szModuleIdentifier, moduleIdentifier) == 0) // class found
			{
				// check for actual implementation if needed
				if (szImplementationName && wstricmp(pMod->szModuleImplementationName, szImplementationName)!=0) continue;
				
				if (pMod->m_nID==0)
				{
					// instance of module class not yet created, so create one
					void* pClass = pMod->m_clsFn();
					if (!pClass)
					{
						// TODO: we can output also library name or path m_libs[libI].szFileName
						DbgPrint(_W("GET MODULE : Can't create instance of module %s implementing %s!"), pMod->szModuleIdentifier, pMod->szModuleImplementationName);
						return NULL;
					}
					else
					{
						// instance created, write down info about it (init number and instance)
						m_nLastInitID++;
						pMod->m_nID = m_nLastInitID;
						pMod->m_pClass = pClass;

						return pClass;
					}
				}
				else
				{
					// already created instance, return it
					return pMod->m_pClass;
				}

			}
		} // end of module iteration inside one library
	} // end of library iteration

	// ---TRY TO FIND CLASS IN CURRENT MODULE
	for(sModule *pMod=g_pLastModule; pMod; pMod=pMod->m_pNext) // go through all modules inside this module
	{
		if(wstricmp(pMod->szModuleIdentifier, moduleIdentifier) == 0) // class found
		{
			// check for actual implementation if needed
			if (szImplementationName && wstricmp(pMod->szModuleImplementationName, szImplementationName)!=0) continue;

			if (pMod->m_nID==0)
			{
				// instance of module class not yet created, so create one
				void* pClass = (void*)pMod->m_clsFn();
				if (!pClass)
				{
					// TODO: we can output also library name or path m_libs[libI].szFileName
					DbgPrint(_W("GET MODULE : Can't get module %s implementing %s!"), pMod->szModuleIdentifier, pMod->szModuleImplementationName);
					return NULL;
				}
				else
				{
					// instance created, write down info about it (init number and instance)
					m_nLastInitID++;
					pMod->m_nID = m_nLastInitID;
					pMod->m_pClass = pClass;

					return pClass;
				}
			}
			else
			{
				// already created instance, return it
				return pMod->m_pClass;
			}

		}
	}

	wchar tmp[512];
	wsprintf(tmp, 511, _W("GET CLASS : Can't find module with identifier '%s' or class failed to initialize! Also check settings..."), moduleIdentifier);
	DbgPrint(tmp);
	MessageBox(0, tmp, _W("ERROR!"), MB_ICONEXCLAMATION);
	return NULL; // not found
}

void CClassManager::DebugPrint(InterfaceDebugFn outputFunction)
{
	wchar tmpMsg[256];

	wsprintf(tmpMsg, 255, _W("Loaded libraries and modules:"));
	outputFunction(tmpMsg);
	// ---ALL TE MODULES IN ALL LIBS
	for (unsigned int libI=0; libI<m_libs.size(); libI++) // library iteration
	{
		wsprintf(tmpMsg, 255, _W(" - Library : %s"), m_libs[libI].szFileName);
		outputFunction(tmpMsg);
		for(sModule *pMod=m_libs[libI].pFirstModule; pMod; pMod=pMod->m_pNext)
		{
			wsprintf(tmpMsg, 255, _W("  . %s : %s"), pMod->szModuleIdentifier, pMod->szModuleImplementationName);
			outputFunction(tmpMsg);
		} // end of module iteration inside one library
	} // end of library iteration
}

// -----------------------------------------------------------------------
void CClassManager::Shutdown()
{
	DbgPrint(_W("SHUTDOWN : Shutdowning singletons..."));

	// shutdown all created module classes
	for(unsigned int i = m_nLastInitID; i > 0; i--) // reverse order
	{

		for (unsigned int libI=0; libI<m_libs.size(); libI++)
		{
			bool bFoundInThisLib=false;
			for (sModule* pMod=m_libs[libI].pFirstModule; pMod; pMod=pMod->m_pNext)
			{
				if (pMod->m_nID==i && pMod->m_pClass) // has this init id and not yet deleted
				{
					DbgPrint(_W("SHUTDOWN : Deleting instance of module %s implementing %s..."), pMod->szModuleIdentifier, pMod->szModuleImplementationName);
					delete((IBaseInterface*)pMod->m_pClass);
					pMod->m_pClass=NULL;

					bFoundInThisLib=true;
					break;
				}
			} // end of module iteration in this lib
			if (bFoundInThisLib) break; // if found and deleted, go to next allocation id
		} // end of library iteration


		// Is this allocation ID from host executable?
		for(sModule *pMod=g_pLastModule; pMod; pMod=pMod->m_pNext) // go through all modules inside this module
		{
			if (pMod->m_nID==i && pMod->m_pClass) // has this init id and not yet deleted
			{
				DbgPrint(_W("SHUTDOWN : Deleting instance of module %s implementing %s..."), pMod->szModuleIdentifier, pMod->szModuleImplementationName);
				delete((IBaseInterface*)pMod->m_pClass);
				pMod->m_pClass=NULL;
				break;
			}
		}

	} // end of allocation ids iteration


	DbgPrint(_W("SHUTDOWN : Shutdowning modules..."));

	// shutdown all libraries
	for(UINT i=0;i<m_libs.size();i++)
	{
	    #if defined(WIN32)
			FreeLibrary(m_libs[i].hLibrary);
		#elif defined(LINUX)
			dlclose(m_libs[i].hLibrary);
		#endif
	}
}

//-----------------------------------------------------------------------------
// HELPER FUNCTIONS AND DEFINES
//-----------------------------------------------------------------------------

/** Get pointer to singleton or multi-instanced class by makro name */
void* I_GetModule(const wchar* szClassName, const wchar* szImplementationName)
{
	if (g_pClsMgr) return g_pClsMgr->GetModule(szClassName, szImplementationName);
	return NULL;
};

/** Load new library at run-time from path */
bool I_LoadNewLibrary(const wchar* szLibraryName)
{
	/*if (g_pClsMgr)*/ return g_pClsMgr->LoadNewLibrary(szLibraryName);
	return false;
};

/** Initialize interface system.

Should be called only once by executable! */
void I_Initialize(const wchar* szBinPath)
{
	if (g_pClsMgr)
	{
		MessageBox(0, _W("Second call of I_Initialize(...)!"), _W("Unexpected fatal Error!"),  MB_SYSTEMMODAL);
		exit(-1);
	}
	g_pClsMgr = new CClassManager();
	g_pClsMgr->Initialize(szBinPath);
}

void I_DebugPrint(InterfaceDebugFn outputFn)
{
	if (g_pClsMgr) g_pClsMgr->DebugPrint(outputFn);
}

void I_Shutdown()
{
	if (!g_pClsMgr)
	{
		MessageBox(0, _W("Shutdown() without I_Initialize(...)!"), _W("Unexpected fatal Error!"), MB_SYSTEMMODAL);
		exit(-1);
	}

	g_pClsMgr->Shutdown();

	DeleteModuleCats();

	DbgPrint(_W("SHUTDOWN FINISHED! :)"));
	delete g_pClsMgr;
	g_pClsMgr=NULL;
}

const wchar* I_GetBinaryPath()
{
	assertd(g_pClsMgr!=NULL, "For some strange reason we don't have pointer to class manager here!");
	return g_pClsMgr->GetBinaryPath();
}

/** MAIN MODULE ENTRY POINT
In case of pClsMgr == NULL: it will return module's last holder
In case of pClsMgr == pointer, this pointer will be set as module's last holder
*/
DLL_EXPORT sModule* ENTRYPOINT_NAME(CClassManager* pClsMgr)
{
	if (pClsMgr==0)
		return g_pLastModule; // return last module class holder (last sModule*)
	else if (pClsMgr==(CClassManager*)1)
		return (sModule *)g_pLastSVarCat; // return last module class holder (last sModule*)
	else
		g_pClsMgr = pClsMgr; // set instance of class manager

	return NULL;
}

SVarCategory::SVarCategory(const wchar* szName)
{
	// copy category name buffer
	m_szCat = new wchar[wstrlen(szName)+1];
	wstrcpy(m_szCat, szName);

	// add to list
	this->next = g_pLastSVarCat;
	g_pLastSVarCat = this;
};

void SVar::AddToCategory(const wchar* szCategoryName)
{
	// try to find category in global module cats
	for (SVarCategory* svc = g_pLastSVarCat; svc; svc=svc->next)
	{
		if (!wstricmp(szCategoryName, svc->GetName()))
		{
			svc->AddSVar(this);
			return;
		}
	}

	// add new category
	SVarCategory* nc = new SVarCategory(szCategoryName);
	nc->AddSVar(this);
}


void DeleteModuleCats()
{
	if (!g_pLastSVarCat) return;
	// try to find category in global module cats
	for (SVarCategory* svc = g_pLastSVarCat; svc; )
	{
		SVarCategory *todel = svc;
		svc=svc->next;
		delete todel;
	}
	g_pLastSVarCat = NULL;
}


SVarCategory* CClassManager::GetAllSVars(){
	return g_pLastSVarCat;
};

}; // namespace

