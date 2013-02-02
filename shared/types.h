/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
Common types and structs
*/
#pragma once

#ifdef WIN32
#include "windows.h"
#else
typedef unsigned int UINT;
typedef long LONG;
typedef BYTE BYTE;
typedef long HRESULT;
typedef wchar* LPSTR;
typedef const wchar* LPCSTR;
typedef void* LPVOID;
typedef const void * LPCVOID;
typedef float FLOAT;
typedef unsigned long ULONG;
typedef unsigned short WORD;
typedef unsigned long DWORD;
#define S_OK 0x0
#define S_FALSE 0x1
#define E_FAIL 0x1
#define E_OUTOFMEMORY 0x2
#define E_INVALIDARG 0x3
#define SUCCEEDED(x) ((x) == S_OK)
#define FAILED(x) ((x) != S_OK)
#endif

#include "Matrix.h"
#include "Quat.h"
//#include "Vector.h"

#include "tArray.h"
#include "tMap.h"
#include "rstring.h"


namespace P3D
{
	// Reversity types
	typedef unsigned int Ruint;
	typedef int rint;
	typedef unsigned long rulong;
	typedef long rlong;
	typedef unsigned char ruchar;
	typedef char rchar;
	typedef float rfloat;

	typedef unsigned int ID;

	// basic types
	//typedef unsigned int uint;
	//typedef unsigned long ulong;

	///Converts degrees to radians
	inline float DegToRad(float degrees);

// ***************************************************************************
// -------------------------- Matrix --------------------------------------

// definicia (takmer) zhodna s D3DMATRIX
/*struct Matrix{
	union {
		struct {
 			union
			{
				struct {float        _11, _12, _13, _14;};
				Vec4	m_front;
				Vec3	m_front3;
			};
			union
			{
				struct {float        _21, _22, _23, _24;};
				Vec4	m_up;
				Vec3	m_up3;
			};
			union
			{
				struct {float        _31, _32, _33, _34;};
				Vec4	m_right;
				Vec3	m_right3;
			};
			union
			{
				struct {float        _41, _42, _43, _44;};
				Vec4	m_posit;
				Vec3	m_posit3;
			};
		};
		float m[4][4];
		float m16[16];
	};
};*/


// *****************************************************************************************************
// STRUCT

class Vec2
{
public:
	float x, y;

	Vec2(){x=y=0;};
	Vec2(float xx, float yy){x=xx; y=yy;};
	Vec2(float pos[2]){x=pos[0]; y=pos[1];};
	Vec2(const float *pos){x=pos[0]; y=pos[1];};

	inline Vec2 operator+ (const float s) const {return Vec2 (x + s, y + s);}
	inline Vec2 &operator+= (const float s){x += s, y += s; return *this;}
	inline Vec2 operator+ (const Vec2 &A) const {return Vec2 (x + A.x, y + A.y);}
	inline Vec2 &operator+= (const Vec2 &A){x += A.x, y += A.y; return *this;}

	inline Vec2 operator- (const float s) const {return Vec2 (x - s, y - s);}
	inline Vec2 &operator-= (const float s){x -= s, y -= s; return *this;}
	inline Vec2 operator- (const Vec2 &A) const {return Vec2 (x - A.x, y - A.y);}
	inline Vec2 &operator-= (const Vec2 &A){x -= A.x, y -= A.y; return *this;}

	inline bool operator== (const Vec2 &A){return ((x == A.x) && (y == A.y));}
	inline bool operator!= (const Vec2 &A){return ((x != A.x) || (y != A.y));}
};

typedef Vec2 Vec2f;

class Vec2i
{
public:
	int x, y;

	Vec2i(){x=y=0;};
	Vec2i(int xx, int yy){x=xx; y=yy;};
	Vec2i(int pos[2]){x=pos[0]; y=pos[1];};
	Vec2i(const int *pos){x=pos[0]; y=pos[1];};

	inline Vec2i operator+ (const int s) const {return Vec2i (x + s, y + s);}
	inline Vec2i &operator+= (const int s){x += s, y += s; return *this;}
	inline Vec2i operator+ (const Vec2i &A) const {return Vec2i (x + A.x, y + A.y);}
	inline Vec2i &operator+= (const Vec2i &A){x += A.x, y += A.y; return *this;}

	inline Vec2i operator- (const int s) const {return Vec2i (x - s, y - s);}
	inline Vec2i &operator-= (const int s){x -= s, y -= s; return *this;}
	inline Vec2i operator- (const Vec2i &A) const {return Vec2i (x - A.x, y - A.y);}
	inline Vec2i &operator-= (const Vec2i &A){x -= A.x, y -= A.y; return *this;}
};

struct Rect
{
	Rect() : x1(0), y1(0), x2(0), y2(0) {}; 
	Rect(int _x1, int _y1, int _x2, int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {};
	Rect(int width, int height) : x1(0), y1(0), x2(width), y2(height) {}; 
	int x1, y1, x2, y2;
};


struct ColorRGB
{
	float r, g, b;
	ColorRGB() {};
	ColorRGB (float inR, float inG, float inB) : r(inR), g(inG), b(inB) {};
};


struct ColorRGBA
{
	float r, g, b, a;
	ColorRGBA() {};
	ColorRGBA (float inR, float inG, float inB, float inA) : r(inR), g(inG), b(inB), a(inA) {};
	ColorRGBA (ColorRGB inRGB) : r(inRGB.r), g(inRGB.g), b(inRGB.b), a(1.0f) {};
};

class ColorValue
{
public:
	unsigned long value;

	ColorValue(unsigned long colorValue){ value = colorValue; };
	ColorValue(ColorRGB &val) { FromFRGBA(val.r, val.g, val.b); };
	ColorValue(ColorRGBA &val) { FromFRGBA(val.r, val.g, val.b, val.a); };

	//inline unsigned long operator= ()const { return value; };

	inline ColorValue& operator= (ColorValue& B)
	 { value = B.value; return *this; };

	inline void FromRGBA(int r, int g, int b, int a=255)
	 { value = ((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)); };

	inline void FromFRGBA(float r, float g, float b, float a=1.0f)
	 { FromRGBA((int)(r*255.0f), (int)(g*255.0f), (int)(b*255.0f), (int)(a*255.0f)); };
};


struct Sphere {
	float			fRadius;
	Vec3	pointOrigin;
	Sphere() {fRadius=0; pointOrigin.x=0; pointOrigin.y=0; pointOrigin.z=0;}
	Sphere(Vec3 origin, float radius) {fRadius=radius; pointOrigin=origin;}
	Sphere(float radius) {fRadius=radius; pointOrigin.x=0; pointOrigin.y=0; pointOrigin.z=0;}
};

struct Ray {
	Vec3	rayOrigin;
	Vec3	rayVector;
	Ray() {rayOrigin.x=rayOrigin.y=rayOrigin.z=0; rayVector.x=rayVector.y=rayVector.z=0;}
	Ray(Vec3	origin, Vec3	vector) {rayOrigin=origin; rayVector=vector;}
};

struct Cylinder
{
	float		fRadius;
	Vec3	position;
	Vec3 axis;
	Cylinder(){fRadius=0.0f; position.x=position.y=position.z=0.0f; axis.x=axis.y=axis.z=0.0f;};
	Cylinder(Vec3 pos, Vec3 vecAxis, float radius){fRadius=radius; position=pos; axis=vecAxis;};
};

/// Axis-Aligned Bounding Box
class AABB
{
public:
	Vec3	min;
	Vec3	max;

	AABB() {min.x=min.y=min.z=0; max.x=max.y=max.z=0;}
	AABB(Vec3 pntMin, Vec3 pntMax) {min=pntMin; max=pntMax;}
	AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
		min.x=minX; min.y=minY; min.z=minZ;    max.x=maxX; max.y=maxY; max.z=maxZ;
	}
	AABB (const AABB &B) { this->min = B.min; this->max = B.max; };

	void GetCenter(Vec3 &center)
	{
		center.x = GetCenter(0); center.y = GetCenter(1); center.z = GetCenter(2);
	}
	/// Get center of AABB
	float GetCenter(int axisIndex)
	{
		switch(axisIndex)
		{
			case 0: return min.x + (max.x - min.x)/2.0f;
			case 1: return min.y + (max.y - min.y)/2.0f;
			case 2: return min.z + (max.z - min.z)/2.0f;
		}
		return 0.0f;
	}
	/// Distances from center (half of sizes)
	void GetExtent(Vec3 &extent)
	{
		extent.x = GetExtent(0); extent.y = GetExtent(1); extent.z = GetExtent(2);
	}
	/// Distance from center at given axis
	float GetExtent(int axisIndex)
	{
		switch(axisIndex)
		{
			case 0: return (max.x - min.x)/2.0f;
			case 1: return (max.y - min.y)/2.0f;
			case 2: return (max.z - min.z)/2.0f;
		}
		return 0.0f;
	}
};

/// Plane structure
struct Plane {
	/// Normal vector
	Vec3 normal;
	/// Distance to origin
	float dist;
};

struct Intersect
{
	Vec3 pos;
	Vec3 normal;
};

// ***************************************************************************
// -------------------------- CBitset ----------------------------------------
class CBitset
{
public:

	CBitset() : m_bits(0), m_size(0) {}

	~CBitset()
	{
		if(m_bits)
		{delete[] m_bits; m_bits = NULL;}
	}

	void Resize(int count)
	{
		m_size = count/32 + 1;
		if(m_bits)
			delete[] m_bits;

		m_bits = new unsigned int[m_size];
		ClearAll();
	}

	void Set(int i)
	{
		m_bits[i >> 5] |= (1 << (i & 31));
	}

	int On(int i)
	{
		return m_bits[i >> 5] & (1 << (i & 31 ));
	}

	void Clear(int i)
	{
		m_bits[i >> 5] &= ~(1 << (i & 31));
	}

	void ClearAll()
	{
		if (m_bits)
			memset(m_bits, 0, sizeof(unsigned int) * m_size);
	}

	void SetAll()
	{
		if (m_bits)
			memset(m_bits, 0xFFFFFFFF, sizeof(unsigned int) * m_size);
	}

	bool IsAllZero()
	{
		if (m_bits)
			for (register int i=0; i<m_size; i++)
				if (m_bits[i] != 0)
					return false;
		return true;
	}

	// zluci dve bitsety, musia mat rovnaku velkost
	void Or (const CBitset &A)
	{
		if ((m_bits!=NULL) && (A.m_bits!=NULL) && (m_size == A.m_size))
			for (register int i=0; i<m_size; i++)
				m_bits[i] |= A.m_bits[i];
	}

	// bitove nasobenie bitsetov, musia mat rovnaku velkost
	void And (const CBitset &A)
	{
		if ((m_bits!=NULL) && (A.m_bits!=NULL) && (m_size == A.m_size))
			for (register int i=0; i<m_size; i++)
				m_bits[i] &= A.m_bits[i];
	}

	// bitovy XOR bitsetov, musia mat rovnaku velkost
	void Xor (const CBitset &A)
	{
		if ((m_bits!=NULL) && (A.m_bits!=NULL) && (m_size == A.m_size))
			for (register int i=0; i<m_size; i++)
				m_bits[i] ^= A.m_bits[i];
	}

	// negacia hodnot bitsetu
	void Negate ()
	{
		if (m_bits)
			for (register int i=0; i<m_size; i++)
				m_bits[i] = ~m_bits[i];
	}

private:
	unsigned int *m_bits;
	int m_size;
};


// *****************************************************************************************************
// KOLIZNI FUNKCE (Test-bool, Dist-float, Intersection)
float Ray2SphereDist(const Ray &ray, const Sphere &sphere);
bool  Ray2SphereTest(const Ray &ray, const Sphere &sphere);
bool  Ray2CylinderIntersect(const Ray &ray, const Cylinder& cylinder, float& lambda, OUT Intersect &info);
bool  Ray2AABBTest(const Ray& ray, const AABB& aabb);

}; // namespace
