/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/

#pragma once

#include "types.h"
#include "stdio.h"
#include "symbols.h"

#include <string>
#include <vector>
using namespace std;

namespace P3D
{

#define ENTRYPOINT_NAME		 DllSynapse // module entry point name
#define ENTRYPOINT_NAME_STR "DllSynapse" // must be the same as above, but as string

class ClassHolder;
class CClassManager;
class sModule;

//SVars
class SVarCategory;
class SVar;

typedef void* (*ClassFactoryFn)(const wchar* clsName);
typedef void* (*CreateClsFn)();
typedef sModule* (*LibraryEntrypoint)(CClassManager* pClsMgr);

typedef void (*InterfaceDebugFn)(const wchar* szMessage); // debug print function

void DbgPrint(const wchar* text, ...);

//-----------------------------------------------------------------------------
// CLASS MANAGER
//-----------------------------------------------------------------------------

extern sModule* g_pLastModule; // last module in linked list of this library

/// One module (main class) identifier
class sModule
{
public:
	sModule(CreateClsFn clsFn, const wchar* moduleIdentifier, const wchar* moduleImplementationName)
		: szModuleIdentifier(moduleIdentifier), szModuleImplementationName(moduleImplementationName)
	{
		m_clsFn = clsFn;
		m_pNext = g_pLastModule;
		g_pLastModule = this;

		m_pClass = NULL; // we don't have instance yet
		m_nID=0;
	}

	/// Concrete module implementation (i.e PhysX, ODE, Havok, ...)
	const wchar* szModuleImplementationName;
	/// Generic module identifier (i.e. Phys, Renderer, Sound, ...)
	const wchar* szModuleIdentifier;

	/// Function pointer to get instance of this singleton
	CreateClsFn m_clsFn;
	/// Next module in linked list
	sModule* m_pNext;
	/// Singleton initialization order. Shutdown will be done from hight to low m_nID.
	unsigned int m_nID;
	/// Stored pointer of created instance of singleton (in case of NULL => instance not yet created)
	void* m_pClass;
};

/// Library identifier
/** Structure identifying one dynamic-link library and its main classes which are registered by REGISTER_MODULE_CLASS makro. 
	One module can implement one or more modules (main classes). 
	Each module (i.e. PhysX implementation) belongs to some group (i.e. Phys) and is interchangeable in this group.
*/
struct sLibrary
{
	sLibrary(){ };
	/// Absolute path to loaded library
	wchar szFileName[512];
	/// Handle to loaded library
	HMODULE hLibrary;
	/// First implemented module (begin of linked list) in this library
	sModule* pFirstModule;
};

/// Class manager which manages class creation, loading and unloading of modules
class CClassManager
{
public:
	CClassManager();
	/// CALLED BY HOST EXE: finds all modules in specified directory
	void Initialize(const wchar* szBinPath);
	/// Load new module (abs or rel path with or without extension)
	bool LoadNewLibrary(const wchar* szLibraryName);
	/// CALLED BY HOST EXE: free all created singletons and modules
	void Shutdown(); 
	/// returns instance of module by identifier (i.e. Phys, Renderer, ...) and implementation name
	/** \see I_GetModule() */
	void* GetModule(const wchar* moduleIdentifier, const wchar* szImplementationName);

	/// Print all loaded libraries and modules
	void DebugPrint(InterfaceDebugFn outputFunction);

	/// Retreive all SVar categories with all parameters
	virtual SVarCategory* GetAllSVars();

	inline const wchar* GetBinaryPath(){ return m_szBinPath; };

	// platform-specific
	void ScanModulesLinux();
	void ScanModulesWindows();
private:
	wchar m_szBinPath[MAX_PATH]; // path to dlls WITH / or \\ at the end!!!
	unsigned int m_nLastInitID; // last initialization order number
	vector<sLibrary> m_libs; // loaded libraries
};

//-----------------------------------------------------------------------------
// HELPER FUNCTIONS AND DEFINES
//-----------------------------------------------------------------------------

/// Return module by it's identifier and implementation.
/** Module singleton with desired identifier and implementation will be returned. Class will be initialized.
Optionaly you can leave szImplementationName NULL to load first implementation.
\return It will return NULL if not found or failed to initialize. MessageBox with warning will be shown. */
void* I_GetModule(const wchar* szModuleIdentifier, const wchar* szImplementationName);

/// Initialize interface system.
/** Should be called only once by executable!*/
void I_Initialize(const wchar* szBinPath); 

/// Write all the libs and modules to output
void I_DebugPrint(InterfaceDebugFn outputFn);

/// Load new module at run-time.
/** This will not be affected by modules.cfg.
\param szMouleName Relative or absolute path of module with or without extension to be loaded. */
bool I_LoadNewModule(const wchar* szMouleName);

/// Shutdown interface system.
/** Should be called only once by executable! */
void I_Shutdown();

/// Get path to binaries
/** This path always end with / or \ so don't worry. */
const wchar* I_GetBinaryPath();

#if defined(WIN32)
#define  DLL_EXPORT   extern "C" __declspec( dllexport )
#elif defined(LINUX)
#define  DLL_EXPORT   extern "C"
#endif

///Basic interface for referenced classes similar to COM. 
/** Some core engine parts will control reference count of this class. */
class IReferenced
{
public:
	IReferenced() { m_cnt=1; m_bCorrectDelete=false; };
	///Referenced classes cannot be deleted directly. It can be done by calling Release().
	virtual ~IReferenced() {
		if (!m_bCorrectDelete)
		{
			assert(!"Instance of class derived from IReferenced can't be deleted. Use obj->Release() to decreased ref. count.");
		}
	};
	/// Increases class reference count by 1.
	/** \return Number of references after change will be returned. */
	virtual unsigned long AddRef(){ m_cnt++; return m_cnt; };
	/// Decreases class reference count by 1 and then delete instance if it will be zero.
	/** \return Number of references after change will be returned. */
	virtual unsigned long Release() { m_cnt--; if (m_cnt==0) { m_bCorrectDelete=true; delete this; } return m_cnt; };
protected:
	unsigned long m_cnt;
	bool m_bCorrectDelete;
};

/** BASE INTERFACE - ALL MODULES INHERIT FROM THIS */
class IBaseInterface
{
public:
	IBaseInterface(){};
	virtual ~IBaseInterface() {};
	virtual bool Init() { return true; };
};

extern CClassManager *g_pClsMgr;

//-----------------------------------------------------------------------------
// INTERFACE SYSTEM MAKROS (CLASS REGISTRATION)
//-----------------------------------------------------------------------------

/// Main module class registration definition
#define REGISTER_MODULE_CLASS(className, moduleIdentifier, moduleImplementationName) \
	static className* __g_##className##_singleton=NULL;\
	static bool __g##className##_bSuccess=false;\
	static void* __Create##className##ClassS() {\
		if (!__g_##className##_singleton) {\
			__g_##className##_singleton = new className;\
			__g##className##_bSuccess = __g_##className##_singleton->Init();\
		}\
		if (__g_##className##_singleton && !__g##className##_bSuccess) {delete __g_##className##_singleton;\
		__g_##className##_singleton=NULL;}\
		return __g_##className##_singleton;}\
	sModule __g_cls##className##_holderS(__Create##className##ClassS, moduleIdentifier, moduleImplementationName);

// MODULE SUBCLASS DECLARATION AND UNLOADING ------------------------------

#if defined(DEBUG) || defined(_DEBUG)
  /// Makro to define module subclass. Should be placed in public part of module main class in header .h.
  #define DECLARE_SUBCLASS(name) inline I##name##* __i##name##()const{ return (I##name##*)c##name##(); }; \
	static inline C##name##* c##name##(bool isUnloading=false){ static C##name##* s_p##name## = new C##name##(); static bool s_p##name##firstInit=true; assertd(s_p##name##firstInit, "Trying to get pointer to destroyed subclass! This happens when you have UNLOAD_SUBCLASS(cls1); UNLOAD_SUBCLASS(cls2); in module class destructor and try to use module::cls1() in destructor of cls2 (after cls1 was destroyed). For more info see call stack."); s_p##name##firstInit = !isUnloading; return s_p##name##; }
#else // release does not need destruction check
  /// Makro to define module subclass. Should be placed in public part of module main class in header .h.
  #define DECLARE_SUBCLASS(name) inline I##name##* __i##name##()const{ return (I##name##*)c##name##(); }; \
	static inline C##name##* c##name##(bool isUnloading=false){ static C##name##* s_p##name## = new C##name##(); return s_p##name##; }
#endif

/// Makro to define module subclass. Should be placed in public part of module main class interface header .h
#define DECLARE_ISUBCLASS(name) virtual I##name##* __i##name##()const=0; \
	inline I##name##* i##name##()const { assertd(this!=NULL, "Trying to get subclass from NULL module class poiter!"); static I##name##* inst##name##=__i##name##(); return inst##name##; }

#if defined(DEBUG) || defined(_DEBUG)
  /// Makro to define module subclass. Should be placed in public part of module main class in header .h.
  #define DECLARE_SUBCLASS_EX(ifaceName, name) inline ifaceName##* __i##name##()const{ return (##ifaceName##*)c##name##(); }; \
	static inline C##name##* c##name##(bool isUnloading=false){ static C##name##* s_p##name## = new C##name##(); static bool s_p##name##firstInit=true; assertd(s_p##name##firstInit, "Trying to get pointer to destroyed subclass! This happens when you have UNLOAD_SUBCLASS(cls1); UNLOAD_SUBCLASS(cls2); in module class destructor and try to use module::cls1() in destructor of cls2 (after cls1 was destroyed). For more info see call stack."); s_p##name##firstInit = !isUnloading; return s_p##name##; }
#else // release does not need destruction check
  /// Makro to define module subclass. Should be placed in public part of module main class in header .h.
  #define DECLARE_SUBCLASS_EX(ifaceName, name) inline ifaceName##* __i##name##()const{ return (##ifaceName##*)c##name##(); }; \
	static inline C##name##* c##name##(bool isUnloading=false){ static C##name##* s_p##name## = new C##name##(); return s_p##name##; }
#endif

/// Makro to define module subclass. Should be placed in public part of module main class interface header .h
#define DECLARE_ISUBCLASS_EX(ifaceName, name) virtual ifaceName##* __i##name##()const=0; \
	inline ifaceName##* i##name##()const { assertd(this!=NULL, "Trying to get subclass from NULL module class poiter!"); static ifaceName##* inst##name##= __i##name##(); return inst##name##; }

#if defined(DEBUG) || defined(_DEBUG)
/// Makro to define module subclass. Should be placed in public part of module main class in header .h.
#define DECLARE_SUBCLASS_NOIFACE(name) static inline C##name##* c##name##(bool isUnloading=false){ static C##name##* s_p##name## = new C##name##(); static bool s_p##name##firstInit=true; assertd(s_p##name##firstInit, "Trying to get pointer to destroyed subclass! This happens when you have UNLOAD_SUBCLASS(cls1); UNLOAD_SUBCLASS(cls2); in module class destructor and try to use module::cls1() in destructor of cls2 (after cls1 was destroyed). For more info see call stack."); s_p##name##firstInit = !isUnloading; return s_p##name##; }
#else // release does not need destruction check
/// Makro to define module subclass. Should be placed in public part of module main class in header .h.
#define DECLARE_SUBCLASS_NOIFACE(name) static inline C##name##* c##name##(bool isUnloading=false){ static C##name##* s_p##name## = new C##name##(); return s_p##name##; }
#endif

/// Makro to easy subclass unloading. These makros should be placed in module class destructor.
#define UNLOAD_SUBCLASS(name) delete c##name##(true)

#define INIT_SUBCLASS(name) c##name##()

//-----------------------------------------------------------------------------
// SVar (setting variable) manipulation
//-----------------------------------------------------------------------------

/// SVar will not be loaded and saved to settings
#define SV_INTERNAL 1
/// SVar will contain string representation
#define SV_STRING (1 << 1)
/// SVar contains object pointer
#define SV_POINTER (1 << 2)

enum eSVarType
{
	SVT_POINTER=0,
	SVT_STRING,
	SVT_FLOAT,
	SVT_INT
};

/// SVar data.
struct sSVarData
{
public:
	int m_flags;

	wchar* m_pszValue;
	unsigned int m_strlen; // length of string value buffer (in chars only)

	int m_nValue;
	float m_fValue;
	void* m_pValue;

	const wchar* szHelp;

	sSVarData() { m_pszValue=NULL; m_strlen=0; };
	void Free(){ if (m_pszValue) delete[] m_pszValue; m_strlen = 0; };
};


void DeleteModuleCats();

/// Multi-Module SVar Setting Variable
/** It is really easy to use. Just declare setting in some .cpp file like this:
SVar svMyVariable("EnableWater", "Renderer", SV_NO_STRING);

Please select reasonable name and reasonable category name. Also make first world letter UpperCase.
Names should not contain spaces, but can contain dots (i.e. Game.Window, Game.AI, ....).

Be careful about types!!
You can use svMyVariable.Set(16.8f) to set float value to previously declared integer SVar type!! 
In this case, conversion will be done, but you can easily forget about it and set for example 0.02f to an integer SVar and get 0 at the end.
*/
class SVar
{
	friend class SVarCategory;
public:
	~SVar()
	{
		if (m_bContainsData) 
		{
			m_data->Free();
			delete m_data;
		}

		DeleteModuleCats();
	}
	SVar(const wchar* szVarName, const wchar* szCategoryName, void* pDefaultValue, const wchar* szOptionalHelp=NULL, int nFlags=0)
	{
		assertd(!!szVarName, "Name of SVar cannot be NULL pointer");
		m_type = SVT_POINTER;

		// at first create data and mark this class as "contains data". 
		m_data = new sSVarData();
		m_bContainsData = true;

#if defined(_INTERNAL)
		m_data->szHelp = szOptionalHelp;
#else
		m_data->szHelp = NULL; // we don't need additional memory for help messages in release :-P
#endif

		m_data->m_flags = nFlags;
		m_data->m_flags |= SV_POINTER;

		m_pszName = szVarName; // copy pointer to static memory with name

		SetPointer(pDefaultValue);

		// make string value
		m_data->m_strlen = 0;
		m_data->m_pszValue = NULL;

		// make other values
		m_data->m_nValue = 0;
		m_data->m_fValue = 0;

		AddToCategory(szCategoryName);
	}

	SVar(const wchar* szVarName, const wchar* szCategoryName, float fDefaultValue, const wchar* szOptionalHelp=NULL, int nFlags=0)
	{
		assertd(!!szVarName, "Name of SVar cannot be NULL pointer");
		m_type = SVT_FLOAT;

		// at first create data and mark this class as "contains data". 
		m_data = new sSVarData();
		m_bContainsData = true;

#if defined(_INTERNAL)
		m_data->szHelp = szOptionalHelp;
#else
		m_data->szHelp = NULL; // we don't need additional memory for help messages in release :-P
#endif

		m_data->m_flags = nFlags;

		m_pszName = szVarName; // copy pointer to static memory with name

		m_data->m_strlen = 0;
		m_data->m_pszValue = NULL;

		SetFloat(fDefaultValue);

		AddToCategory(szCategoryName);
	}
	// bool is the same as Int type
	SVar(const wchar* szVarName, const wchar* szCategoryName, bool bDefaultValue, const wchar* szOptionalHelp=NULL, int nFlags=0) {
		assertd(!!szVarName, "Name of SVar cannot be NULL pointer");
		m_type = SVT_INT;

		// at first create data and mark this class as "contains data". 
		m_data = new sSVarData();
		m_bContainsData = true;

#if defined(_INTERNAL)
		m_data->szHelp = szOptionalHelp;
#else
		m_data->szHelp = NULL; // we don't need additional memory for help messages in release :-P
#endif

		m_data->m_flags = nFlags;

		m_pszName = szVarName; // copy pointer to static memory with name

		m_data->m_strlen = 0;
		m_data->m_pszValue = NULL;

		SetInt(bDefaultValue ? 1 : 0);

		AddToCategory(szCategoryName);
	};
	SVar(const wchar* szVarName, const wchar* szCategoryName, int nDefaultValue, const wchar* szOptionalHelp=NULL, int nFlags=0)
	{
		assertd(!!szVarName, "Name of SVar cannot be NULL pointer");
		m_type = SVT_INT;

		// at first create data and mark this class as "contains data". 
		m_data = new sSVarData();
		m_bContainsData = true;

#if defined(_INTERNAL)
		m_data->szHelp = szOptionalHelp;
#else
		m_data->szHelp = NULL; // we don't need additional memory for help messages in release :-P
#endif

		m_data->m_flags = nFlags;

		m_pszName = szVarName; // copy pointer to static memory with name

		m_data->m_strlen = 0;
		m_data->m_pszValue = NULL;

		SetInt(nDefaultValue);

		AddToCategory(szCategoryName);
	}

	SVar(const wchar* szVarName, const wchar* szCategoryName, const wchar* szDefaultValue, const wchar* szOptionalHelp=NULL, int nFlags=0)
	{
		assertd(!!szVarName, "Name of SVar cannot be NULL pointer");
		assertd(!!szDefaultValue, "Default value of SVar cannot be NULL pointer");
		m_type = SVT_STRING;

		// at first create data and mark this class as "contains data". 
		m_data = new sSVarData();
		m_bContainsData = true;

#if defined(_INTERNAL)
		m_data->szHelp = szOptionalHelp;
#else
		m_data->szHelp = NULL; // we don't need additional memory for help messages in release :-P
#endif

		m_data->m_flags = nFlags;

		m_pszName = szVarName; // copy pointer to static memory with name

		// make string value
		m_data->m_flags |= SV_STRING;
		m_data->m_strlen = (unsigned int)wstrlen(szDefaultValue);
		m_data->m_pszValue = new wchar[m_data->m_strlen+1];
		wstrcpy(m_data->m_pszValue, szDefaultValue);

		// make other values
		m_data->m_pValue = NULL;
		m_data->m_nValue = watoi(szDefaultValue);
		m_data->m_fValue = (float)watof(szDefaultValue);

		AddToCategory(szCategoryName);
	}

	inline float GetFloat()const{ return m_data->m_fValue; };
	inline const wchar* GetString()const{ 
		assertd(!!m_data->m_pszValue, "Trying to get string value from non-string SVar."); 
		return m_data->m_pszValue; 
	};
	inline bool GetBool()const{ return m_data->m_nValue==0 ? false : true; };
	inline int  GetInt()const{ return m_data->m_nValue; };
	inline void* GetPointer()const{ assertd((m_data->m_flags&SV_POINTER)!=0, "Trying to get pointer value from non-pointer SVar."); return m_data->m_pValue; };

	inline int GetFlags()const{ return m_data->m_flags; };

	inline void SetFloat(float val)
	{
		// make string value
		if (m_data->m_flags & SV_STRING)
		{
			if (m_data->m_strlen<13)
			{
				m_data->m_strlen = 13;
				m_data->m_pszValue = new wchar[m_data->m_strlen+1];
			}
			wsprintf(m_data->m_pszValue, 13, _W("%f"), val);
		}

		// make other values
		m_data->m_pValue = NULL;
		m_data->m_nValue = (int)val;
		m_data->m_fValue = val;
	}
	inline void SetBool(bool val){ SetInt(val ? 1 : 0); };
	inline void SetInt(int val)
	{
		// make string value
		if (m_data->m_flags & SV_STRING)
		{
			if (m_data->m_strlen<13)
			{
				m_data->m_strlen = 13;
				m_data->m_pszValue = new wchar[m_data->m_strlen+1];
			}
			wsprintf(m_data->m_pszValue, 13, _W("%d"), val);
		}

		// make other values
		m_data->m_pValue = NULL;
		m_data->m_nValue = val;
		m_data->m_fValue = (float)val;
	}
	inline void SetString(const wchar* val)
	{
		// make string value
		m_data->m_flags |= SV_STRING;

		// make new buffer if current is not long enough
		unsigned int ll = (unsigned int)wstrlen(val);
		if (m_data->m_strlen<ll)
		{
			if (m_data->m_pszValue) delete[] m_data->m_pszValue;
			m_data->m_strlen = ll;
			m_data->m_pszValue = new wchar[m_data->m_strlen+1];
		}
		wstrcpy(m_data->m_pszValue, val);

		// make other values
		m_data->m_pValue = NULL;
		m_data->m_nValue = watoi(val);
		m_data->m_fValue = (float)watof(val);
	}

	/// Set variable from string based on SVar type
	inline void SetFromString(const wchar* str)
	{
		switch(m_type)
		{
		case SVT_FLOAT: SetFloat((float)watof(str)); break;
		case SVT_INT: SetInt(watoi(str)); break;
		case SVT_STRING: SetString(str); break;
		default:
			assertd(false, "Cannot do SetFromString() at pointer type SVar!");
		}
	}

	/// Can be saved and loaded?
	inline bool CanBeSaved()
	{
		return (!(m_data->m_flags & SV_INTERNAL) && m_type!=SVT_POINTER);
	}

	inline void SetPointer(void* val)
	{
		assertd((m_data->m_flags&SV_POINTER)!=1, "Trying to assign pointer to non-pointer SVar. SVars for pointer values must be declared with this flag");

		m_data->m_pValue = val;
	}

	/// Get SVar name
	inline const wchar* GetName()const{ return m_pszName; };

	/// Get SVar type
	/** Useful for loading/saving */
	inline eSVarType GetType()const { return m_type; };

	// useful helpers - short name Set for every type (except for void*) ;)
	inline void Set(const wchar* str) { SetString(str); };
	inline void Set(int n) { SetInt(n); };
	inline void Set(float f) { SetFloat(f); };
	inline void Set(bool b) { SetBool(b); };

	/// Returns help string if any, otherwise "" or NULL!
	inline const wchar* GetHelp() const { return m_data->szHelp; };

	// RETURN OPERATORS
	inline operator const wchar*() { return GetString(); };
	inline operator const int () { return GetInt(); };
	inline operator const float () { return GetFloat(); };

private:
	/// (Only for internal purposes, do not use) Add other instance for the same variable under the same category
	void AddInstance(SVar *pOtheInst)
	{
		pOtheInst->SetData(m_data);
	}
	/// Only for internal purposes, do not use
	inline void SetData(sSVarData* data){ 
		if (m_bContainsData) 
			m_data->Free(); 
		m_data = data; 
	};
	/// Only for internal purposes, do not use
	inline sSVarData* GetData(){ return m_data; };
	/// Only for internal purposes, do not use
	inline bool ContainsData()const { return m_bContainsData; };
private:
	void AddToCategory(const wchar* szCategoryName);

private:
	bool m_bContainsData; // if true this SVar contains data, if false it contains only pointer to data structure (= this SVar is instance)
	//
	const wchar* m_pszName;
	eSVarType m_type;

	sSVarData* m_data;
};

/// SVar category holder for this module (dll)
/** Contains all SVars which belongs to this category. */
class SVarCategory
{
public:
	SVarCategory(const wchar* szName);
	~SVarCategory(){ delete[] m_szCat; };
	// Add single variable to this category
	void AddSVar(SVar* pVar) { 
		// try to find var with the same name
		for (unsigned int i=0; i<m_vars.Size(); i++)
		{
			if (!wstricmp(m_vars[i]->GetName(), pVar->GetName())) 
			{
				m_vars[i]->AddInstance(pVar);
				return;
			}
		}
		m_vars.AddEx(pVar); 
	};
	void AddSVarsFromCategory(SVarCategory * other)
	{
		assertd(!wstricmp(other->GetName(), m_szCat), "Trying to add SVars from other category with custom name!!!");
		const tArray<SVar *> & oth = other->GetSVars();

		for (unsigned int i=0; i<oth.Size(); i++) AddSVar(oth[i]);
	}
	/// Get category name
	const wchar* GetName()const{ return m_szCat; };

	const tArray<SVar *> & GetSVars(){ return m_vars; };

	SVarCategory *next;
private:
	wchar* m_szCat; // category name

	tArray<SVar*> m_vars;
};


}; // namespace