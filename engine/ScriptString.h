/* Panther3D Engine � 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "ScriptEngine.h"

namespace P3D {

/* 
Note about referenced type in scripts
-------------------------------------
Referenced type can use all operators or methods of original C++ class except for methods returning
new class value, something like:
 Object Object::Add(Object &other)

In that case you must write new "wrapper" which will return pointer to new object, bacause it needs refCount=1.
For example you can do sth like this:
 ScriptObject* ScriptObject::Add(ScriptObject &other)
 {
	ScriptObject* ptr = new ScriptObject(); // constructor will make ScriptObject with refCount=1 instead of Object
	...do what should do
	return ptr;
 }
*/

/// Referenced string type for scripts
class CScriptString : public Str
{
public:
	// factories
	static CScriptString* CScriptString::DefaultFactory();
	static CScriptString* CScriptString::CopyFactory(const CScriptString& other);

	// constructor / destructor ------------
	CScriptString() : Str()
	{
		numRef = 1;
	}

	CScriptString(const char *str, unsigned int num) : Str(str, num)
	{
		numRef = 1;
	}

	// reference counting -------------------
	inline void AddRef()
	{ numRef++; };
	inline void Release()
	{ if( --numRef == 0 ) delete this; };

	// operators
	CScriptString &operator=(const CScriptString &other);
	CScriptString &operator+=(const CScriptString &other);
	CScriptString *operator+(const CScriptString &other) const;

	CScriptString *CScriptString::operator +(const float f) const;
	CScriptString *CScriptString::operator +(const bool b) const;
	CScriptString *CScriptString::operator +(const int b) const;
	CScriptString *CScriptString::operator +(const unsigned int b) const;

	// helpers
	/// Returns character at index
	const char &CharAt(unsigned int idx) const { static char tmpChr = W2A((*this)[idx]); return tmpChr; };
	const unsigned char GetOrdinalCode() const { return (unsigned char)CharAt(0); };
private:
	int numRef;
};

void RegisterStringType(asIScriptEngine *engine);

// --------------------------- UTF-8 x Unicode helpers ------------------------------------

#define UNICODE_BAD         0xffff  // Unicode value representing an error.

#define UTF8_VALUE1     0x00        // Value for set bits for single byte UTF-8 Code.
#define UTF8_MASK1      0x80        // Mask (i.e. bits not set by the standard) 0xxxxxxx
#define UTF8_WRITE1     0xff80      // Mask of bits we cannot allow if we are going to write one byte code
#define UTF8_VALUE2     0xc0        // Two byte codes
#define UTF8_MASK2      0xe0        // 110xxxxx 10yyyyyy
#define UTF8_WRITE2     0xf800      // Mask of mits we cannot allow if we are going to write two byte code
#define UTF8_VALUE3     0xe0        // Three byte codes   
#define UTF8_MASK3      0xf0        // 1110xxxx 10yyyyyy 10zzzzzz
#define UTF8_VALUE4     0xf0        // Four byte values
#define UTF8_MASK4      0xf8        // 11110xxx ----    (These values are not supported by JWPce).   
#define UTF8_VALUEC     0x80        // Continueation byte (10xxxxxx).
#define UTF8_MASKC      0xc0 

/// Converts single character of utf-8 (up to 4 chars) to unicode and increments pointer
inline int utf2unicode (unsigned char *&ptr) {
	int i;
	if      (UTF8_VALUE1 == (*ptr & UTF8_MASK1)) {
		i = *ptr++;
	}
	else if (UTF8_VALUE2 == (*ptr & UTF8_MASK2)) {
		i = ((ptr[0] & 0x1f) << 6) | (ptr[1] & 0x3f);
		ptr += 2;
	}
	else if (UTF8_VALUE3 == (*ptr & UTF8_MASK3)) {
		i = ((ptr[0] & 0xf) << 12) | ((ptr[1] & 0x3f) << 6) | (ptr[2] & 0x3f);
		ptr += 3;
	}
	else if (UTF8_VALUE4 == (*ptr & UTF8_MASK4)) {
		i = 0xffff;
		ptr += 4;
	}
	return (i);
}

/// Converts inchar to utf-8 and stores it in outptr (it must be array of 4 chars)
/** Number of characters stored in outptr will be returned */
inline int unicode2utf (unsigned char *outptr, const wchar* inchar)
{
	return 0;
}

}; // ns