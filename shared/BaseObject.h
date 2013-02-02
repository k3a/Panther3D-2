/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once

#include "types.h"
#include "json/json.h"
#include <time.h>

#include "IFilesystem.h"
#include "IConsole.h"

namespace P3D {

/*
enum eParamType
{
	TYPE_UNKNOWN,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_VECTOR,
	TYPE_STRING
};*/

/// Base object parameter virtual class
/** Every serializable object (parameter) must deliver from this. */
class ISerializable
{
public:
	virtual void Serialize(Json::Value &node, IConsole * con = NULL)=0;
	virtual void DeSerialize(Json::Value &node, IConsole * con = NULL)=0;
	virtual const wchar* ToString(){ 
		assertd(false, "Trying to convert unknown class (ISerializable) to string!");
		return _W("UnknownClass"); 
	};
	virtual void FromString(const wchar* str){ assertd(false, "Trying to assign value to unknown class (ISerializable) from string!"); };
};

/// Integer object parameter
class StringParam : public Str, public ISerializable
{
public:
	StringParam() { SetEx(_W("")); };
	StringParam(Str & str) : Str(str) { };
	StringParam(const wchar* str) : Str(str) { };
	StringParam(const char* str) : Str(str) { };

	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		node = Get();
	}
	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
		SetEx(node.asString().c_str());
	}
	const wchar* ToString(){ return Get(); };
	void FromString(const wchar* str){ SetEx(str); };

	inline operator Str & () { return *this; };
	inline operator const wchar * () { return Get(); };
	inline Str & operator =(const wchar * param){ SetEx(param); return *this; };
	inline Str & operator =(const Str & param){ SetEx(param); return *this; };
};

/// Boolean object parameter
class BoolParam : public ISerializable
{
public:
	BoolParam() { m_param = true; };
	BoolParam(bool & b) { m_param = b; };
	BoolParam(bool b) { m_param = b; };

	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		node = m_param;
	}
	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
		m_param = node.asBool();
	}
	const wchar* ToString(){ static wchar str[16]; wsprintf(str, 15, _W("%d"), m_param?"1":"0"); return str; };
	void FromString(const wchar* str){ m_param = !!watoi(str); };

	inline operator bool & () { return m_param; };
	inline bool & operator =(bool param){ m_param = param; return m_param; };
private:
	bool m_param;
};

/// Integer object parameter
class IntParam : public ISerializable
{
public:
	IntParam() { m_param = 0; };
	IntParam(int & i) { m_param = i; };
	IntParam(int i) { m_param = i; };

	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		node = m_param;
	}
	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
		m_param = node.asInt();
	}
	const wchar* ToString(){ static wchar str[16]; wsprintf(str, 15, _W("%d"), m_param); return str; };
	void FromString(const wchar* str){ m_param = watoi(str); };

	inline operator int & () { return m_param; };
	inline int & operator =(int param){ m_param = param; return m_param; };
private:
	int m_param;
};

/// Float object parameter
class FloatParam : public ISerializable
{
public:
	FloatParam() { m_param = 0; };
	FloatParam(float & f) { m_param = f; };
	FloatParam(float f) { m_param = f; };

	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		node = (double)m_param;
	}
	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
		m_param = (float)node.asDouble();
	}
	const wchar* ToString(){ static wchar str[16]; wsprintf(str, 15, _W("%f"), m_param); return str; };
	void FromString(const wchar* str){ m_param = (float)watof(str); };

	inline operator float & () { return m_param; };
	inline float & operator =(float param){ m_param = param; return m_param; };
private:
	float m_param;
};

/// Vector with 4 dimensions - object parameter
class Vec4Param : public ISerializable
{
public:
	Vec4Param() : m_param(0,0,0,0) {};
	Vec4Param(const Vec4 & param) : m_param(param) {};
	Vec4Param(float x, float y, float z, float w) : m_param(x, y, z, w) {};

	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		wchar vecstr[128];
		wsprintf(vecstr, 127, _W("%f %f %f %f"), m_param.x, m_param.y, m_param.z, m_param.w);
		node = vecstr;
	}
	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
		std::wstring vecstr = node.asString();
		FromString(vecstr.c_str());
	}
	/// Set vector value from string "x y z w" floats
	void FromString(const wchar* vecstr)
	{
		int ret = scanf("%f %f %f %f", &m_param.x, &m_param.y, &m_param.z, &m_param.w);
		assertd(ret==4, "Failed to parse vec4 object value!");
		//return ret==4;
	}
	const wchar* ToString()
	{
		static wchar vecstr[128];
		wsprintf(vecstr, 127, _W("%f %f %f %f"), m_param.x, m_param.y, m_param.z, m_param.w);
		return vecstr;
	}
	inline operator Vec4 & () { return m_param; };
	inline Vec4 & operator =(Vec4 param){ m_param = param; return m_param; };
	inline Vec4 & operator =(Vec4 & param){ m_param = param; return m_param; };
	inline Vec4 & operator =(const wchar* param){ FromString(param); return m_param; };
private:
	Vec4 m_param;
};

/// Vector with 3 dimensions - object parameter
class Vec3Param : public ISerializable
{
public:
	Vec3Param() : m_param(0,0,0) {};
	Vec3Param(const Vec3 & param) : m_param(param) {};
	Vec3Param(float x, float y, float z) : m_param(x, y, z) {};

	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		wchar vecstr[128];
		wsprintf(vecstr, 127, _W("%f %f %f"), m_param.x, m_param.y, m_param.z);
		node = vecstr;
	}
	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
		std::wstring vecstr = node.asString();
		FromString(vecstr.c_str());
	}
	/// Set vector value from string "x y z" floats
	void FromString(const wchar* vecstr)
	{
		int ret = scanf("%f %f %f", &m_param.x, &m_param.y, &m_param.z);
		assertd(ret==3, "Failed to parse vec3 object value!");
		//return ret==3;
	}
	const wchar* ToString()
	{
		static wchar vecstr[128];
		wsprintf(vecstr, 127, _W("%f %f %f"), m_param.x, m_param.y, m_param.z);
		return vecstr;
	}
	inline operator Vec3 & () { return m_param; };
	inline Vec3 & operator =(Vec3 param){ m_param = param; return m_param; };
	inline Vec3 & operator =(Vec3 & param){ m_param = param; return m_param; };
	inline Vec3 & operator =(const wchar* param){ FromString(param); return m_param; };
private:
	Vec3 m_param;
};

/// Vector with 2 dimensions - object parameter
class Vec2Param : public ISerializable
{
public:
	Vec2Param() : m_param(0,0) {};
	Vec2Param(const Vec2 & param) : m_param(param) {};
	Vec2Param(float x, float y) : m_param(x, y) {};

	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		wchar vecstr[128];
		wsprintf(vecstr, 127, _W("%f %f"), m_param.x, m_param.y);
		node = vecstr;
	}
	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
		std::wstring vecstr = node.asString();
		FromString(vecstr.c_str());
	}
	/// Set vector value from string "x y" floats
	void FromString(const wchar* vecstr)
	{
		int ret = scanf("%f %f", &m_param.x, &m_param.y);
		assertd(ret==2, "Failed to parse vec2 object value!");
		//return ret==2;
	}
	const wchar* ToString()
	{
		static wchar vecstr[128];
		wsprintf(vecstr, 127, _W("%f %f"), m_param.x, m_param.y);
		return vecstr;
	}
	inline operator Vec2 & () { return m_param; };
	inline Vec2 & operator =(Vec2 param){ m_param = param; return m_param; };
	inline Vec2 & operator =(Vec2 & param){ m_param = param; return m_param; };
	inline Vec2 & operator =(const wchar* param){ FromString(param); return m_param; };
private:
	Vec2 m_param;
};

/// Integer size object parameter
class SizeParam : public ISerializable
{
public:
	SizeParam() : m_param(0,0) {};
	SizeParam(const Vec2i & param) : m_param(param) {};
	SizeParam(int width, int height) : m_param(width, height) {};

	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		wchar vecstr[64];
		wsprintf(vecstr, 63, _W("%d %d"), m_param.x, m_param.y);
		node = vecstr;
	}
	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
		std::wstring vecstr = node.asString();
		FromString(vecstr.c_str());
	}
	/// Set vector value from string "x y z" floats
	void FromString(const wchar* vecstr)
	{
		int ret = scanf("%d %d", &m_param.x, &m_param.y);
		assertd(ret==2, "Failed to parse vec2 size object value!");
		//return ret==2;
	}
	const wchar* ToString()
	{
		static wchar vecstr[64];
		wsprintf(vecstr, 63, _W("%d %d"), m_param.x, m_param.y);
		return vecstr;
	}
	inline operator Vec2i & () { return m_param; };
	inline Vec2i & operator =(Vec2i param){ m_param = param; return m_param; };
	inline Vec2i & operator =(Vec2i & param){ m_param = param; return m_param; };
	inline Vec2i & operator =(const wchar* param){ FromString(param); return m_param; };
private:
	Vec2i m_param;
};

/// Quaternion object parameter
class QuatParam : public ISerializable
{
public:
	QuatParam() : m_param(0,0,0) {};
	QuatParam(const Quat & param) : m_param(param) {};
	QuatParam(float x, float y, float z, float w) : m_param(x, y, z, w) {};

	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		wchar quatstr[128];
		wsprintf(quatstr, 127, _W("%f %f %f %f"), m_param.x, m_param.y, m_param.z, m_param.w);
		node = quatstr;
	}
	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
		std::wstring quatstr = node.asString();
		FromString(quatstr.c_str());
	}
	/// Set vector value from string "x y z" floats
	void FromString(const wchar* vecstr)
	{
		int ret = scanf("%f %f %f %f", &m_param.x, &m_param.y, &m_param.z, &m_param.w);
		assertd(ret==4, "Failed to parse quat object value!");
		//return ret==4;
	}
	const wchar* ToString()
	{
		static wchar quatstr[128];
		wsprintf(quatstr, 127, _W("%f %f %f %f"), m_param.x, m_param.y, m_param.z, m_param.w);
		return quatstr;
	}
	inline operator Quat & () { return m_param; };
	inline Quat & operator =(Quat param){ m_param = param; return m_param; };
	inline Quat & operator =(Quat & param){ m_param = param; return m_param; };
	inline Quat & operator =(const wchar* param){ FromString(param); return m_param; };
private:
	Quat m_param;
};

/// Array object parameter
/** Items of this array must be serializable objects, so they must derive from ISerializable */
template <class T> class ArrayParam : public tArray<T>, public ISerializable 
{
public:
	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		for (register unsigned int i=0; i<this->Size(); i++)
		{
			(*this)[i].Serialize(node[i]);
		}
	}
	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
		this->Delete(); // clear previous array memory buffer

		// add items from serializer
		for (register unsigned int i=0; i<node.size(); i++)
		{
			T item;
			item.DeSerialize(node[i], con);
			AddEx(item);
		}
	}
};

/// Map parameter for objects
/** Key parameter class must be convertable to string as key,
value can be arbitrary serializable class value. 
*/
template <class CKEY, class CVALUE> class MapParam : public ISerializable
{
public:
	void Add(const CKEY& key, const CVALUE& val)
	{
		m_keys.Add(key);
		m_values.Add(val);
	}
	void AddEx(const CKEY& key, const CVALUE& val)
	{
		m_keys.AddEx(key);
		m_values.AddEx(val);
	}
	const CVALUE* Find(const CKEY& key)
	{
		for (register unsigned int i=0; i<m_keys.Size(); i++)
			if (m_keys[i]==key) return &m_values[i];
		return NULL;
	}
	// ---
	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		for (register unsigned int i=0; i<m_keys.Size(); i++)
		{
			m_values[i].Serialize( node[m_keys[i].ToString()] );
		}
	}
	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
		// clear previous array memory buffer
		m_keys.Delete();
		m_values.Delete();

		// add items from serializer
		Json::Value::Members members = node.getMemberNames();
		for (register unsigned int i=0; i<members.size(); i++)
		{
			m_keys.AddEx( members[i].c_str() );

			CVALUE val;
			val.DeSerialize(node[ members[i].c_str() ], con);

			m_values.AddEx(val);
		}
	}
private:
	tArray<CKEY> m_keys;
	tArray<CVALUE> m_values;
};

/// Object parameters information (name and help string)
class ParamInfo
{
public:
	void AddParamInfo(const wchar* name, const wchar* help, bool readonly=false)
	{
		m_paramNames.AddEx(name);
		m_paramHelp.AddEx(help);
		m_paramReadOnly.AddEx(readonly);
	};

	tArray<const wchar*> m_paramNames;
	tArray<const wchar*> m_paramHelp;
	tArray<bool> m_paramReadOnly;
};

/// Macro for engine object parameter registration
/** Should be placed in object class constructor. */
#define REGISTER_PARAM(name, description) m_params.AddEx(&name); \
	m_paraminfo.AddParamInfo(_W(#name), _W(description)) 

/// Macro for engine object read-only parameter registration
/** Should be placed in object class constructor. */
#define REGISTER_PARAM_READONLY(name, description) m_params.AddEx(&name); \
	m_paraminfo.AddParamInfo(_W(#name), _W(description), true)

/// Base serializable engine object
class BaseObject : public  ISerializable
{
public:
	BaseObject() {
		REGISTER_PARAM(ObjectName, "Internal name of this object");
	}
	virtual ~BaseObject(){
	}

	/// Save object to file and perform serialization
	bool Save(IFileSystem * fs, const wchar *path, IConsole * con = NULL)
	{
		assertd(ObjectName.GetNumChars()>0, "Trying to save unnamed object. Only objects created by ObjectManager can be saved! Please use standard macros to define objects!");

		FSFILE * fp = fs->Open(path, _W("wb"));
		if (!fp) 
		{
			if (con) con->AddMsgEx(MSG_ERR, _W(__FUNCTION__), _W("Failed to open file %s to save object %s!"), path, ObjectName.Get());
			return false;
		}

		Json::Value root;

		// construct head comment
		time_t tim;
		time(&tim);
		tm *timm = localtime(&tim);
		wchar head[128];
		wsprintf(head, 127, _W("// %s (Panther3D Object) - saved %.2d.%.2d.%d at %.2d:%.2d:%.2d"), ObjectName.Get(),
			timm->tm_mday, timm->tm_mon+1, timm->tm_year+1900, timm->tm_hour, timm->tm_min, timm->tm_sec);

		root.setComment(head, Json::commentBefore);

		// serialize all the params
		Serialize(root, con);

		// write output
		Json::FastWriter writer;
		std::string outstr = writer.write(root);
		fs->Write(outstr.c_str(), 1, outstr.size(), fp);

		fs->Close(fp);

		if (con) con->AddMsgEx(MSG_ERR, _W(__FUNCTION__), _W("Object %s successfuly saved to %s."), ObjectName.Get(), path);
		return true;
	}

	/// Load object from file and perform deserialization
	bool Load(IFileSystem * fs, const wchar *path, IConsole * con = NULL)
	{
		/*
		BYTE* pData = NULL; ULONG size=0; 
		FSFILE * fp = fs->Load(path, pData, size, true);
		if (!fp) return false;
		*/

		/*Json::Reader reader;

		if (!reader.parse((const wchar *)pData, (const wchar *)(pData+size), root, false)) 
		{
			if (con) con->AddMsgEx(MSG_ERR, _W(__FUNCTION__), _W("Failed to parse %s!"), path);
			return false;
		}*/

		// do parameter check
		/*if (m_paraminfo)
		{
		Json::Value::Members memb = root.getMemberNames();
		if (memb.size() != m_paraminfo.m_paramNames.Size())
		{
		if (con) con->AddMsgEx(MSG_ERR, _W(__FUNCTION__), _W("Number of parameters in JSON %s for object %s (%d) doesn't qeual to C++ (%d)!"), path, m_objname, memb.size(), m_paraminfo->m_paramNames.Size());
		}
		}*/

		//fs->UnLoad(fp, pData);

		return false;
	}

	void Serialize(Json::Value &node, IConsole * con = NULL)
	{
		/*if (!node.hasComment(Json::commentBefore))
		{
			wchar head[128];
			wsprintf(head, 127, _W("// %s"), m_objname);

			node.setComment(head, Json::commentBefore);
		}*/

		// serialize all the params
		for (unsigned int i=0; i<m_paraminfo.m_paramNames.Size(); i++)
		{
			m_params[i]->Serialize(node[m_paraminfo.m_paramNames[i]]);
		}
	}

	void DeSerialize(Json::Value &node, IConsole * con = NULL)
	{
	}

	/// Precache object
	/** Precaching is procedure in which object data are cached to device format.
	For example mesh data should be cached to vertex buffer. */
	virtual bool PreCache(){ return true; };

public:
	static StringParam ObjectName;

protected:
	tArray<ISerializable*> m_params;
	ParamInfo m_paraminfo;
};



}; // ns