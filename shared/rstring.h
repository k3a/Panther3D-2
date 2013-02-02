/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/

/*
I tried made this as effective as possible, 
but sometimes there can be redundant wchar overwrite in one operation.
Further optimalizations could be clever. ;)
*/

#pragma once
#include "types.h"
#include <string.h>
#include <cstdarg>

// unicode compatibility
/*namespace std {

	typedef basic_string<wchar, char_traits<wchar>,
	allocator<wchar> > wstring;

	typedef basic_istream<wchar, char_traits<wchar> > tistream;

}*/

namespace P3D {

class Str;

/// Str class with fixed data length. All data MUST be NULL-TERMINATED!
/**
It is alternative to fixed wchar[x] buffer. It must be preallocated for maximum number of characters
we need to store in this string. It is fixed from initialization, but can be resized using Resize(),
SetEx(), FormatEx() and AppendEx(). All standard operations not marked as Ex WILL NOT expand buffer
(this apply to << operators and += + and so on).

The reason of this behavior is to get as fast as possible string class without any additional
checking. So at release it is equally fast as standard wchar[] manipulation.

User is responsible for buffer length check! Buffer overflow isn't automatically checked!!!
However it is checked in debug version. But this check can't determine all cases.

For buffer length check you can use GetBufferLength().

\see GetBufferlength
*/
class Str
{
public:
	/// Create string with empty buffer
	/** This buffer cannot store any characters unless you reserve more space by calling Reserve() or some Ex functions which will expand buffer. */
	Str() { 
		m_len=0;
		m_num=0;
		m_data = new wchar[1];
		m_data[0]=0;
	};
	/// Create string from other string. Buffer length will be the same.
	Str(const Str &inp) { 
		m_len=inp.GetBufferLength();
		m_data = new wchar[m_len+1];
		Set(inp);
	};

	/// Create string buffer with length of str and copy this string
	Str(const wchar* str) {
		m_len=(unsigned int)wstrlen(str);
		m_data = new wchar[m_len+1];
		Set(str);
	}

	/// Create string buffer with length of str and copy this string
	Str(const char* str) {
		m_len=(unsigned int)strlen(str);
		m_data = new wchar[m_len+1];
		Set(str);
	}

	/// Create string buffer with specified length of str and copy this string
	/** \param str Input string to copy data from
		\param numChars Desired buffer size in characters. Should be >= number of character in str */
	Str(const wchar* str, unsigned int numChars) {
		m_len = numChars;
		assert( numChars>=wstrlen(str) );
		m_data = new wchar[m_len+1];
		Set(str);
	}

	/// Create string buffer with specified length of str and copy this string
	/** \param str Input string to copy data from
	\param numChars Desired buffer size in characters. Should be >= number of character in str */
	Str(const char* str, unsigned int numChars) {
		m_len = numChars;
		assert( numChars>=strlen(str) );
		m_data = new wchar[m_len+1];
		Set(str);
	}

	/// Create string class with specified memory for characters
	Str(unsigned int numChars)
	{
		m_data = new wchar[numChars+1];
		m_data[0]=0;
		m_len = numChars;
		m_num = 0;
	}
	~Str(){ Clear(true); };

	/// Retrieve string as const wchar*
	inline const wchar* Get()const{ return m_data; };

	/// Format string (maximum 4096 chars of formated output!)
	inline void Format(const wchar* fmt, ...)
	{
		va_list ap;

		wchar buf[4096];

		va_start(ap, fmt);
#ifdef _WIN32
		wvsnprintf(buf,4096,fmt,ap);
#else
		vsntprintf(buf,4096,fmt,ap);
#endif
		va_end(ap);
		Set(buf);
	}
	/// Format string (maximum 4096 chars of formated output!) and expand buffer where necessary
	inline void FormatEx(const wchar* fmt, ...)
	{
		va_list ap;

		wchar buf[4096];

		va_start(ap, fmt);
#ifdef _WIN32
		wvsnprintf(buf,4096,fmt,ap);
#else
		vsntprintf(buf,4096,fmt,ap);
#endif
		va_end(ap);
		SetEx(buf);
	}

	/// Set new string value. Input must be null-terminated!!!
	inline void Set(const wchar* str)
	{
		assertd(wstrlen(str)<=m_len, "Length of string buffer exceeded. If you don't know string size in advance, you can use SetEx instead which will expand capacity where necessary.");

		for(m_num=0;str[m_num];m_num++) m_data[m_num] = str[m_num];

		m_data[m_num]=0; // zero terminate
	}

	/// Set new string value. Input must be null-terminated!!!
	inline void Set(const char* str)
	{
		assertd(strlen(str)<=m_len, "Length of string buffer exceeded. If you don't know string size in advance, you can use SetEx instead which will expand capacity where necessary.");

		for(m_num=0;str[m_num];m_num++) m_data[m_num] = (wchar)str[m_num];

		m_data[m_num]=0; // zero terminate
	}

	/// Set new string value and expand buffer where necessary. Input must be null-terminated!!!
	inline void SetEx(const wchar* str)
	{
		// try to copy
		for(m_num=0;str[m_num];m_num++) 
		{
			// buffer needs to be expanded?
			if (m_num==m_len) 
			{
				for(;str[m_num];) m_num++; // count desired buffer length
				wchar* newBuff = new wchar[m_num+1];
				m_len = m_num;
				wstrcpy(newBuff, str);
				delete[] m_data;
				m_data = newBuff;
				return;
			}
			m_data[m_num] = str[m_num];
		}

		m_data[m_num]=0; // zero terminate
	}
	inline void Set(const Str &str) { assert(m_len>=str.GetNumChars()); m_num=str.GetNumChars(); wstrcpy(m_data, str.Get()); };
	inline void SetEx(const Str &str) { 
		if (str.GetNumChars()<=m_len) 
			Set(str); 
		else 
		{
			m_len = str.GetNumChars();
			m_num = m_len;
			wchar* newBuff = new wchar[m_len+1];
			wstrcpy(newBuff, str.Get()); 
			delete[] m_data;
			m_data = newBuff;
		}
	};

	/// Append additional string at the end
	inline void Append(const wchar* str)
	{
		assertd(m_num+wstrlen(str)<=m_len, "Length of string buffer exceeded. If you don't know string size in advance, you can use SetEx instead which will expand capacity where necessary.");

		// just append
		register unsigned int isrc=0;
		while(str[isrc]!=0)
		{
			m_data[m_num]=str[isrc];
			isrc++; m_num++;
		}
		m_data[m_num]=0; // zero terminate
	}
	/// Append additional string at the end and expand buffer where necessary.
	inline void AppendEx(const wchar* str)
	{
		// try just append
		register unsigned int isrc=0;
		while(str[isrc]!=0)
		{
			// hmm, buffer needs to be expanded
			if (m_num>=m_len)
			{
				m_data[m_num]=0; // finish current buffer;
				register int lisrc = isrc;
				for(;str[isrc];) isrc++; // count num of remaining source chars
				m_len = m_num+isrc;
				wchar* newBuff = new wchar[m_len+1];
				// copy current buffer
				isrc = lisrc;
				wstrcpy(newBuff, m_data);
				// copy additional part which didn't fit to actual buffer to the new one
				while(str[isrc]!=0)
				{
					newBuff[m_num]=str[isrc];
					isrc++; m_num++;
				}
				newBuff[m_num]=0; // zero terminate
				// swap buffers
				delete[] m_data;
				m_data=newBuff;
				return;
			}
			m_data[m_num]=str[isrc];
			isrc++; m_num++;
		}
		m_data[m_num]=0; // zero terminate
	}
	/// Append additional string at the end
	inline void Append(const Str &str) { 
		assertd(m_len>=m_num+str.GetNumChars(), "Length of string buffer exceeded. If you don't know string size in advance, you can use SetEx instead which will expand capacity where necessary."); 
		wstrcpy(&m_data[m_num], str.Get()); m_num+=str.GetNumChars(); 
	};
	/// Append additional string at the end and expand buffer where necessary.
	/** In case of buffer resize, buffer will grow of exact number of characters of str. */
	inline void AppendEx(const Str &str) { 
		if(m_len>=m_num+str.GetNumChars()) 
			Append(str); // buffer is long enough, just append
		else {
			m_len += str.GetNumChars();
			wchar* newBuff = new wchar[m_len+1];
			wstrcpy(newBuff, m_data);
			register int istr = 0; register wchar c;
			for (;c = str.Get()[istr];istr++) newBuff[m_num++]=c;
			newBuff[m_num] = 0;
			delete[] m_data;
			m_data=newBuff;
		} 
	};
	/// Append character at the end of string. Will not expand buffer.
	inline void Append(const wchar c)
	{
		assertd(m_num<m_len, "Length of string buffer exceeded. If you don't know string size in advance, you can use SetEx instead which will expand capacity where necessary.");
		// just append
		m_data[m_num]=c;
		m_data[++m_num]=0;
	}
	/// Append additional character at the end and expand buffer where necessary.
	inline void AppendEx(const wchar c)
	{
		if(m_num>=m_len)
		{
			m_len = m_num+3;  // add 3 more chars
			wchar* newBuff = new wchar[m_len+1];
			m_num++;
			register int i;
			for (i=0;m_data[i];i++) newBuff[i] = m_data[i];
			newBuff[i]=c;
			newBuff[++i]=0;
			delete[] m_data;
			m_data = newBuff;
			return;
		}
		// just append
		m_data[m_num]=c;
		m_data[++m_num]=0;
	}

	/// Get number of characters actually stored in Str class (without termianting zero)
	inline unsigned int GetNumChars() const
	{
		return m_num;
	}

	/// Clear string (clearBuffer=true to delete memory of inner buffer)
	inline void Clear(bool clearBuffer=false)
	{
		m_data[0]=0;
		m_num=0;
		if (clearBuffer) 
		{
			delete[] m_data;
			m_len = 0;
		}
	}

	/// Dynamically allocate new wchar* buffer and copy string. User is responsible for deallocation!!
	inline wchar* MakeDynamicCopy()
	{
		wchar* nbuff = new wchar[m_num+1];
		wstrcpy(nbuff, m_data);
		return nbuff;
	}

	/// Fit buffer size to character count 
	/* 
	\return new buffer size will be returned. 
	*/ 
	inline unsigned int ClearUnusedBuffer() 
	{  
		if (m_len>m_num) 
		{ 
			m_len = m_num; 
			wchar* newBuff = new wchar[m_len]; 
			wstrcpy(newBuff, m_data); 
			delete[] m_data; 
			m_data = newBuff; 
		}  
		return m_len; 
	} 


	/// Returns internal buffer length without zero at the end.
	/** You are allowed to store only GetBufferLength() characters here. Memory for terminating zero is automatically allocated. */
	unsigned int GetBufferLength()const{return m_len;};

	/// Reserves buffer for numChars characters. All current data will be lost!!!
	/** In case you want to resize buffer and copy current characters in buffer, use Resize() instead.
	\see Resize()*/
	void Reserve(unsigned int numChars){
		delete[] m_data;
		m_data = new wchar[numChars+1];
		m_data[0]=0;
		m_len = numChars;
		m_num = 0;
	};
	/// Changes buffer size and copies all data to new buffer.
	void Resize(unsigned int numChars){
		assertd(numChars>=m_num, "Target number of characters for Resize() operation must be >= than actual num. of characters!");
		wchar* newBuff = new wchar[numChars+1];
		wstrcpy(newBuff, m_data);
		m_len=numChars;
		delete[] m_data;
		m_data = newBuff;
	};

	// *** OPERATORS
	// return operators
	inline operator const wchar*() { return m_data; };
	inline operator const wchar*()const{ return m_data; };
	inline wchar operator[](unsigned int index)const{ assertd(index<=m_num, "Trying to index character at position > num. of characters in string!"); return m_data[index]; };
	inline wchar &operator[](unsigned int index){ assertd(index<=m_num, "Trying to index character at position > num. of characters in string!"); return m_data[index]; };

	//compare operators
	inline bool operator ==(const Str &str) { 
		return !wstrcmp( Get(), str.Get() );
	}
	inline bool operator >(const Str &str) { 
		return wstrcmp( Get(), str.Get() )>0;
	}
	inline bool operator <(const Str &str) { 
		return wstrcmp( Get(), str.Get() )<0;
	}
	inline int Compare(const Str& other)
	{
		int cmp=0;
		if (*this>other) 
			cmp = 1;
		else if (*this<other) 
			cmp = -1;
		return cmp;
	}

	// assignments
	inline Str& operator =(const wchar* str){ SetEx(str); return *this; };
	inline Str& operator =(const Str &str){ Reserve(str.GetNumChars()); Set(str); return *this; }; 

	inline Str& operator =(const float n){ wchar str[64]=_W("0"); wsprintf(str, 63, _W("%f"), n); SetEx(str); return *this; };
	inline Str& operator =(const int n){ wchar str[33]=_W("0"); witoa(n, str, 10); SetEx(str); return *this; };
	inline Str& operator =(const unsigned int n){ wchar str[33]=_W("0"); wsprintf(str, 33, _W("%u"), n); SetEx(str); return *this; };
	inline Str& operator =(const bool b){ wchar str[6]=_W("False"); if (b) wstrcpy(str, _W("True")); SetEx(str); return *this; };

	// append
	inline Str operator+(const wchar c) const { Str ret(m_num+1); ret=*this; ret.Append(c); return ret; };
	inline Str operator+(const wchar* str) const { Str ret(m_num+1); ret=*this; ret.Append(str); return ret; };
	inline Str operator+(const Str &str) const { Str ret(m_num+1); ret=*this; ret.Append(str); return ret; };
	inline Str& operator+=(const wchar c) { AppendEx(c); return *this; };
	inline Str& operator+=(const wchar* str) { AppendEx(str); return *this; };
	inline Str& operator+=(const Str &str) { AppendEx(str); return *this; };

	// append other types
	inline Str operator+(const float n) const { Str ret(64); wchar str[64]=_W("0"); wsprintf(str, 63, _W("%f"), n); ret.SetEx(str); return ret; };
	inline Str operator+(const int n) const { Str ret(33); wchar str[33]=_W("0"); wsprintf(str, 63, _W("%n"), n); ret.SetEx(str); return ret; };
	inline Str operator+(const unsigned int n) const { Str ret(33); wchar str[33]=_W("0"); wsprintf(str, 33, _W("%u"), n); ret.SetEx(str); return ret; };
	inline Str operator+(const bool b) const { Str ret(_W("False")); if (b) ret.Set(_W("True")); return ret; };
	inline Str& operator+=(const float n) { wchar str[64]=_W("0"); wsprintf(str, 63, _W("%f"), n); AppendEx(str); return *this; };
	inline Str& operator+=(const int n) { wchar str[33]=_W("0"); witoa(n, str, 10); AppendEx(str); return *this; };
	inline Str& operator+=(const unsigned int n) { wchar str[33]=_W("0"); wsprintf(str, 33, _W("%u"), n); AppendEx(str); return *this; };
	inline Str& operator+=(const bool b) { wchar str[6]=_W("False"); if (b) wstrcpy(str, _W("True")); AppendEx(str); return *this; };

	// streams
	inline Str& operator<<(const wchar* str) { AppendEx(str); return *this; };
	inline Str& operator<<(const Str &str) { AppendEx(str); return *this; };
	inline Str& operator<<(const int n) { return (*this)+=n; };
	inline Str& operator<<(const float n) { return (*this)+=n; };
	inline Str& operator<<(const unsigned int n) { return (*this)+=n; };

	// *** special string functions
	/// Return integer hash representation of this string in range [0-255]
	inline unsigned int Hash256(){ 
		unsigned int _hash=0; 
		for(register unsigned int i=0; i<m_num; i++) 
			_hash += (unsigned int)m_data[i]; 
		return g_randHash256[_hash % 256];
	};
	/// Return integer hash representation of this string in range [0,511] best for strings with more than 2 chars
	inline unsigned int Hash512(){ 
		unsigned int _hash=0; 
		for(register unsigned int i=0; i<m_num; i++) 
			_hash += (unsigned int)g_randHash256[m_data[i]]; 
		return g_randHash512[_hash % 512];
	};
	/// Return integer hash representation of this string in range [0,2047] best for strings with more than 8 chars
	inline unsigned int Hash2048(){ 
		unsigned int _hash=0; 
		for(register unsigned int i=0; i<m_num; i++) 
			_hash += (unsigned int)g_randHash256[m_data[i]]; 
		return g_randHash2048[_hash % 2048];
	};
	/// Return integer hash representation of this string in range [0,1048575] best for strings with more than 4096 chars
	inline unsigned int Hash1048576(){ 
		unsigned int _hash=0; 
		for(register unsigned int i=0; i<m_num; i++) 
			_hash += (unsigned int)g_randHash256[m_data[i]]; 
		return _hash % 1048576;
	}


	// *** parser operation

private:
	wchar* m_data; // data memory length of m_len + 1 (additional zero termination)
	unsigned int m_len; // buffer length in characters (without terminating zero)
	unsigned int m_num; // number of characters; if (!m_num && m_data) -> must be recalculated!
	unsigned int m_iparser; // index of parser position
};


}; // ns