/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/

#pragma once

#include <math.h>
//#include "symbols.h"
//#include "types.h"

namespace P3D
{

	///3-dimensional vector class.
	/** General purpose float vector class. Useful to define positions, normals, vectors, etc.
	Contains methods and operators for frequently used operations like *,/,+,-,+=,-=,*=,/=.
	float * Vec3 isn't implemented - Vec3 * float is defined as "multiply vector by number".
	*/
	class Vec3
	{
	public:
		// struct
		float x,y,z;

		//////////////////////////////////////////////////////////////////////////
		// constructors
		Vec3 () { x=y=z=0.0f; };
		Vec3 (const float *ptr){x = ptr[0]; y = ptr[1]; z = ptr[2];}
		Vec3 (float fx, float fy, float fz){x = fx; y = fy; z = fz;}
		//////////////////////////////////////////////////////////////////////////
		// operators
		inline float& operator[] (int i){return (&x)[i];}
		inline const float& operator[] (int i) const{return (&x)[i];}

		inline Vec3 operator+ (const Vec3 &A) const {return Vec3 (x + A.x, y + A.y, z + A.z);}
		inline Vec3 operator- (const Vec3 &A) const {return Vec3 (x - A.x, y - A.y, z - A.z);}
		inline Vec3 &operator+= (const Vec3 &A) {x += A.x; y += A.y; z += A.z; return *this;}
		inline Vec3 &operator-= (const Vec3 &A) {x -= A.x; y -= A.y; z -= A.z; return *this;}
		inline Vec3 &operator*= (const Vec3 &A) {x *= A.x; y *= A.y; z *= A.z; return *this;}
		inline Vec3 &operator/= (const Vec3 &A) {x /= A.x; y /= A.y; z /= A.z; return *this;}
		/// unary operator: -Vec3 will change sign for all vector components
		inline Vec3 operator- () const {return Vec3 (-x, -y, -z);}

		inline Vec3 operator+ (const float s) const {return Vec3 (x + s, y + s, z + s);}
		inline Vec3 operator- (const float s) const {return Vec3 (x - s, y - s, z - s);}
		inline Vec3 operator* (const float s) const {return Vec3 (x * s, y * s, z * s);}
		inline Vec3 operator/ (const float s) const {return Vec3 (x / s, y / s, z / s);}

		inline Vec3 &operator+= (const float s){x += s, y += s, z += s; return *this;}
		inline Vec3 &operator-= (const float s){x -= s, y -= s, z -= s; return *this;}
		inline Vec3 &operator*= (const float s){x *= s, y *= s, z *= s; return *this;}

		inline bool operator== (const Vec3 &A){return ((x == A.x) && (y == A.y) && (z == A.z));}
		inline bool operator!= (const Vec3 &A){return ((x != A.x) || (y != A.y) || (z != A.z));}

		inline bool operator> (const Vec3 &A){return ((x > A.x) && (y > A.y) && (z > A.z));}
		inline bool operator< (const Vec3 &A){return ((x < A.x) && (y < A.y) && (z < A.z));}
		inline bool operator<= (const Vec3 &A){return ((x <= A.x) && (y <= A.y) && (z <= A.z));}

		/// dot product
		float operator* (const Vec3 &A) const {return x * A.x + y * A.y + z * A.z;}
		/// cross product
		Vec3 operator% (const Vec3 &A) const
		{
			return Vec3 (y * A.z - z * A.y,
				z * A.x - x * A.z,
				x * A.y - y * A.x);
		}
		Vec3 &operator%= (const Vec3 &A)
		{
			register float xBuf = x, yBuf = y;
			x = y * A.z - z * A.y;
			y = z * A.x - xBuf * A.z;
			z = xBuf * A.y - yBuf * A.x;
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		// functions

		inline void Assign (float nx, float ny, float nz) {x=nx; y=ny; z=nz;}
		/// normalization
		inline Vec3 &Normalize ()
		{
			float fDivider = sqrtf (x * x + y * y + z * z);
			if (fDivider == 0) return *this;
			register float fMultiplier = 1.0f / fDivider;
			x *= fMultiplier;
			y *= fMultiplier;
			z *= fMultiplier;
			return *this;
		}
		/// length
		inline float Length () const {return sqrtf (x * x + y * y + z * z);}
		/// length2 - squared length - used for distance comparsion
		inline float LengthSqr () const {return x*x + y*y + z*z;}
		/// minimum value
		inline float GetMin () const {return pmin(x,pmin(y,z));}
		/// maximum value
		inline float GetMax () const {return pmax(x,pmax(y,z));}
		/// absolute values
		inline Vec3 &Abs()
		{
			x = fabs(x); y = fabs(y); z = fabs(z);
			return *this;
		}
		// transformacia maticou (predpoklada ze stvrty element=1), obdoba D3DXVec3TransformCoord()
	/*	inline Vec3 &TransformAsPoint (const Matrix &B)
		{
			float fx, fy, fz;
			fx = x * B._11 + y * B._21 + z * B._31 + B._41;
			fy = x * B._12 + y * B._22 + z * B._32 + B._42;
			fz = x * B._13 + y * B._23 + z * B._33 + B._43;
			x = fx; y = fy; z = fz;
			return *this;
		}*/
		// transformacia maticou (predpoklada ze stvrty element=0), obdoba D3DXVec3TransformNormal()
		/*inline Vec3 &TransformAsVector (const Matrix &B)
		{
			float fx, fy, fz;
			fx = x * B._11 + y * B._21 + z * B._31;
			fy = x * B._12 + y * B._22 + z * B._32;
			fz = x * B._13 + y * B._23 + z * B._33;
			x = fx; y = fy; z = fz;
			return *this;
		}*/
		/// X-rotation
		inline void RotX (float angle)
		{
			float s = (float) sinf(angle );
			float c = (float) cosf(angle );
			float Y=y;
			y =  y*c - z*s;
			z =  Y*s + z*c;
		}
		/// Y-rotation
		inline void RotY (float angle)
		{
			float s = (float) sinf(angle );
			float c = (float) cosf(angle );
			float X=x;
			x =  x*c + z*s;
			z = -X*s + z*c;
		}
		/// Z-rotation
		inline void RotZ (float angle)
		{
			float s = (float) sinf(angle );
			float c = (float) cosf(angle );
			float X=x;
			x =  x*c - y*s;
			y =  X*s + y*c;
		}

		inline Vec3 Cross(const Vec3 &with)
		{
			return Vec3(y * with.z - z * with.y, 
						z * with.x - x * with.z, 
						x * with.y - y * with.x);
		}

		inline float Dot(const Vec3 &with)
		{
			return( x * with.x + y * with.y + z * with.z  );
		}

	};


	/** 3-dimensional vector class.
	General purpose int vector class. 
	*/

	class Vec3i
	{
	public:
		int x,y,z;

		Vec3i () {};
		Vec3i (const int *ptr){x = ptr[0]; y = ptr[1]; z = ptr[2];}
		Vec3i (const Vec3i &vec){x = vec.x; y = vec.y; z = vec.z;}
		Vec3i (int ix, int iy, int iz){x = ix; y = iy; z = iz;}
	};

	
	/** 4-dimensional vector class.
	General purpose float vector class. Useful to define positions, normals, vectors, etc.
	Contains methods and operators for frequently used operations like *,/,+,-,+=,-=,*=,/=.
	float * Vec3 isn't implemented - Vec3 * float is defined as "multiply vector by number".
	*/
	class Vec4
	{
	public:
		// struct
		float x,y,z,w;

		//////////////////////////////////////////////////////////////////////////
		// constructors
		Vec4 () {};
		Vec4 (const float *ptr){x = ptr[0]; y = ptr[1]; z = ptr[2]; w = 1.0f;}
		Vec4 (const Vec4 &vec){x = vec.x; y = vec.y; z = vec.z; w = 1.0f;}
		Vec4 (float fx, float fy, float fz, float fw = 1.0f){x = fx; y = fy; z = fz; w = fw;}
		//////////////////////////////////////////////////////////////////////////
		// operators
		inline float& operator[] (int i){return (&x)[i];}
		inline const float& operator[] (int i) const{return (&x)[i];}

		inline Vec4 operator+ (const Vec4 &A) const {return Vec4 (x + A.x, y + A.y, z + A.z, w);}
		inline Vec4 operator- (const Vec4 &A) const {return Vec4 (x - A.x, y - A.y, z - A.z, w);}
		inline Vec4 &operator+= (const Vec4 &A) {x += A.x; y += A.y; z += A.z; return *this;}
		inline Vec4 &operator-= (const Vec4 &A) {x -= A.x; y -= A.y; z -= A.z; return *this;}
		/// unary operator: -Vec4 will change sign for all vector components
		inline Vec4 operator- () const {return Vec4 (-x, -y, -z, -w);}

		inline Vec4 operator+ (const float s) const {return Vec4 (x + s, y + s, z + s, w + s);}
		inline Vec4 operator- (const float s) const {return Vec4 (x - s, y - s, z - s, w - s);}
		inline Vec4 operator* (const float s) const {return Vec4 (x * s, y * s, z * s, w * s);}

		inline Vec4 &operator+= (const float s) {x += s, y += s, z += s; w += s; return *this;}
		inline Vec4 &operator-= (const float s) {x -= s, y -= s, z -= s; w -= s; return *this;}
		inline Vec4 &operator*= (const float s) {x *= s, y *= s, z *= s; w *= s; return *this;}

		inline bool operator== (const Vec4 &A){return ((x == A.x) && (y == A.y) && (z == A.z) && (w == A.w));}
		inline bool operator!= (const Vec4 &A){return ((x != A.x) || (y != A.y) || (z != A.z) || (w != A.w));}

		/// dot product
		inline float operator* (const Vec4 &A) const
		 {return x * A.x + y * A.y + z * A.z + w * A.w;}

		//////////////////////////////////////////////////////////////////////////
		// functions

		inline void Assign (float nx, float ny, float nz, float nw) {x=nx; y=ny; z=nz; w=nw;}
		/// normalization
		inline Vec4 &Normalize ()
		{
			register float fMultiplier;
			float fDivider = sqrtf (x*x + y*y + z*z + w*w);
			if (fDivider == 0.0f) return *this;
			fMultiplier = 1.0f / fDivider;
			x *= fMultiplier;
			y *= fMultiplier;
			z *= fMultiplier;
			w *= fMultiplier;
			return *this;
		}
		/// length
		inline float Length () const {return sqrtf (x*x + y*y + z*z + w*w);}
		/// length2 - squared length - used for lenght comparsion
		inline float LengthSqr () const {return x*x + y*y + z*z + w*w;}

		/// minimum value
		inline float GetMin () const {return pmin(x,pmin(y,z));}
		/// maximum value
		inline float GetMax () const {return pmax(x,pmax(y,z));}
		/// absolute values
		inline Vec4 &Abs()
		{
			x = fabs(x); y = fabs(y); z = fabs(z); w = fabs(w);
			return *this;
		}
		// transformacia maticou
		/*inline Vec4 &Transform (const Matrix &B)
		{
			float fx, fy, fz;
			fx = x * B._11 + y * B._21 + z * B._31 + w * B._41;
			fy = x * B._12 + y * B._22 + z * B._32 + w * B._42;
			fz = x * B._13 + y * B._23 + z * B._33 + w * B._43;
			x = fx; y = fy; z = fz;
			return *this;
		}*/
	};

};
