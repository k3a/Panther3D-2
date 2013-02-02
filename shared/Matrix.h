/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, mess
*/
#pragma once

#include "types.h"
#include "math.h"
#include <string.h>


#include "Vector.h"

namespace P3D
{


#define MATRIX_EPSILON	0.005f



//---------------------------------
/// Left-handed matrix class.
class Matrix
{
public:
	//struct
	union {
		struct {
			/*union
			{*/
				struct {float        _11, _12, _13, _14;};
				/*Vec4	m_front;
				Vec3	m_front3;*/
			/*};
			union
			{*/
				struct {float        _21, _22, _23, _24;};
				/*Vec4	m_up;
				Vec3	m_up3;*/
			/*};
			union
			{*/
				struct {float        _31, _32, _33, _34;};
				/*Vec4	m_right;
				Vec3	m_right3;*/
			/*};
			union
			{*/
				struct {float        _41, _42, _43, _44;};
				/*Vec4	m_posit;
				Vec3	m_posit3;*/
			/*};*/
		};
		float m[4][4];
		float m16[16];
	};

	//////////////////////////////////////////////////////////////////////////
	// constructors
	Matrix () {};
	Matrix (float *pMatrix)
	{memcpy (&_11, pMatrix, sizeof(Matrix));}
	Matrix (const float fInitValue)
	{
		for (register DWORD i = 0; i<4; i++)
			for (register DWORD j = 0; i<4; i++)
				m[i][j] = fInitValue;
	}
	Matrix (Matrix &pMatrix)
	{memcpy (&_11, &pMatrix, sizeof(Matrix));}
	Matrix (const Vec4 &front, const Vec4 &up, const Vec4 &right, const Vec4 &posit)
	{
		_11 = front.x; _12 = front.y; _13 = front.z; _14 = front.w;
		_21 = up.x; _22 = up.y; _23 = up.z; _24 = up.w;
		_31 = right.x; _32 = right.y; _33 = right.z; _34 = right.w;
		_41 = posit.x; _42 = posit.y; _43 = posit.z; _44 = posit.w;
	}
	Matrix (float f11, float f12, float f13, float f14,
		float f21, float f22, float f23, float f24,
		float f31, float f32, float f33, float f34,
		float f41, float f42, float f43, float f44)
	{
		_11 = f11; _12 = f12; _13 = f13; _14 = f14;
		_21 = f21; _22 = f22; _23 = f23; _24 = f24;
		_31 = f31; _32 = f32; _33 = f33; _34 = f34;
		_41 = f41; _42 = f42; _43 = f43; _44 = f44;
	};
	//////////////////////////////////////////////////////////////////////////
	// operators
	bool operator== (const Matrix &B) const;
	Matrix operator* (const Matrix &B) const;
	Matrix &operator*= (const Matrix &B);
	// 	Matrix operator% (const Matrix &B) const;
	// 	Matrix &operator%= (const Matrix &B);
	//////////////////////////////////////////////////////////////////////////
	// functions
	Matrix &SetIdentityMatrix();
	Matrix &SetZeroMatrix();
	bool Compare (const Matrix &A, float fEpsilon=MATRIX_EPSILON) const;
	Matrix &Transpose3x3 ();
	Matrix &Transpose4X4 ();
	Matrix &Translate (const float fShiftX, const float fShiftY, const float fShiftZ);
	Matrix &Scale (const float fScaleX, const float fScaleY, const float fScaleZ);
	Matrix &RotateX (const float fAngle);
	Matrix &RotateY (const float fAngle);
	Matrix &RotateZ (const float fAngle);
	Matrix &Rotate (Vec3 vAxis, const float fAngle);
	Matrix &RotateZXY (const float fAngleX, const float fAngleY, const float fAngleZ);
	Matrix &RotateRelativeX (const float fAngle);
	Matrix &RotateRelativeY (const float fAngle);
	Matrix &RotateRelativeZ (const float fAngle);
	Matrix &RotateRelativeZXY (const float fAngleX, const float fAngleY, const float fAngleZ);
	Matrix &LookAt(Vec3 &vEye, Vec3 &vAt,Vec3 &vUp);
	Matrix &PerspectiveFov(float fovy, float aspect, float zNearPlane, float zFarPlane);
	void World2Local(Vec3 &vec);
	void TransformVector(Vec3 &v);
	

	float			Cofactor (DWORD r, DWORD c) const;
	float			Determinant () const;
	float			Det2() const;
	float			Det3() const;
	float			Det4() const;
	Matrix	&Inverse ();
	Matrix	&Inverse3x3 ();
};

//---------------------------------
inline Matrix &Matrix::SetIdentityMatrix()
{
	_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14= 0.0f;
	_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24= 0.0f;
	_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34= 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44= 1.0f;
	return *this;
}

//---------------------------------
inline Matrix &Matrix::SetZeroMatrix()
{
	memset (&_11, 0, sizeof(Matrix));
	return *this;
}

// pre ortonormalne matice, ktore neobsahuju scaling staci pre inverziu rotacie zavolat Transpose3x3()
// ortonormalne matice su tie ktorych osi su na seba kolme, co je pripad DX transform. matic
//---------------------------------
inline Matrix &Matrix::Transpose3x3 ()
{
	register float fBuf;
	fBuf = _12; _12 = _21; _21 = fBuf;
	fBuf = _13; _13 = _31; _31 = fBuf;
	fBuf = _23; _23 = _32; _32 = fBuf;
	_14 = 0.0f; _24 = 0.0f; _34 = 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
	return *this;
}

//---------------------------------
inline Matrix &Matrix::Transpose4X4 ()
{
	register float fBuf;
	fBuf = _12; _12 = _21; _21 = fBuf;
	fBuf = _13; _13 = _31; _31 = fBuf;
	fBuf = _14; _14 = _41; _41 = fBuf;
	fBuf = _23; _23 = _32; _32 = fBuf;
	fBuf = _24; _24 = _42; _42 = fBuf;
	fBuf = _34; _34 = _43; _43 = fBuf;
	return *this;
}

//---------------------------------
inline Matrix &Matrix::Translate (const float fShiftX, const float fShiftY, const float fShiftZ)
{
	_41 += fShiftX;
	_42 += fShiftY;
	_43 += fShiftZ;

	// staci to ???, plati to iba v pripade ze posledny stlpec matice je vzdy <0,0,0,1>
	// inak je spravne:
	/*	_11 += _14 * fShiftX;
	_21 += _24 * fShiftX;
	_31 += _34 * fShiftX;
	_41 += _44 * fShiftX;

	_12 += _14 * fShiftY;
	_22 += _24 * fShiftY;
	_32 += _34 * fShiftY;
	_42 += _44 * fShiftY;

	_13 += _14 * fShiftZ;
	_23 += _24 * fShiftZ;
	_33 += _34 * fShiftZ;
	_43 += _44 * fShiftZ;*/

	return *this;
}

//---------------------------------
inline Matrix &Matrix::Scale (const float fScaleX, const float fScaleY, const float fScaleZ)
{
	_11 *= fScaleX; _21 *= fScaleX; _31 *= fScaleX; _41 *= fScaleX;
	_12 *= fScaleY; _22 *= fScaleY; _32 *= fScaleY; _42 *= fScaleY;
	_13 *= fScaleZ; _23 *= fScaleZ; _33 *= fScaleZ; _43 *= fScaleZ;
	return *this;
}

//---------------------------------
inline Matrix &Matrix::RotateX (const float fAngle)
{
	Matrix	matBuf(*this);
	float fSin = sinf(fAngle), fCos = cosf (fAngle);

	_12 = matBuf._12 * fCos - matBuf._13 * fSin;
	_13 = matBuf._12 * fSin + matBuf._13 * fCos;
	_22 = matBuf._22 * fCos - matBuf._23 * fSin;
	_23 = matBuf._22 * fSin + matBuf._23 * fCos;
	_32 = matBuf._32 * fCos - matBuf._33 * fSin;
	_33 = matBuf._32 * fSin + matBuf._33 * fCos;
	_42 = matBuf._42 * fCos - matBuf._43 * fSin;
	_43 = matBuf._42 * fSin + matBuf._43 * fCos;
	return *this;
}

//---------------------------------
inline Matrix &Matrix::RotateY (const float fAngle)
{
	Matrix	matBuf = *this;
	float fSin = sinf(fAngle), fCos = cosf (fAngle);

	_11 = matBuf._11 * fCos + matBuf._13 * fSin;
	_13 = -matBuf._11 * fSin + matBuf._13 * fCos;
	_21 = matBuf._21 * fCos + matBuf._23 * fSin;
	_23 = -matBuf._21 * fSin + matBuf._23 * fCos;
	_31 = matBuf._31 * fCos + matBuf._33 * fSin;
	_33 = -matBuf._31 * fSin + matBuf._33 * fCos;
	_41 = matBuf._41 * fCos + matBuf._43 * fSin;
	_43 = -matBuf._41 * fSin + matBuf._43 * fCos;
	return *this;
}

//---------------------------------
inline Matrix &Matrix::RotateZ (const float fAngle)
{
	Matrix	matBuf = *this;
	float fSin = sinf(fAngle), fCos = cosf (fAngle);

	_11 = matBuf._11 * fCos - matBuf._12 * fSin;
	_12 = matBuf._11 * fSin + matBuf._12 * fCos;
	_21 = matBuf._21 * fCos - matBuf._22 * fSin;
	_22 = matBuf._21 * fSin + matBuf._22 * fCos;
	_31 = matBuf._31 * fCos - matBuf._32 * fSin;
	_32 = matBuf._31 * fSin + matBuf._32 * fCos;
	_41 = matBuf._41 * fCos - matBuf._42 * fSin;
	_42 = matBuf._41 * fSin + matBuf._42 * fCos;
	return *this;
}

inline Matrix &Matrix::Rotate (Vec3 vAxis, const float fAngle)
{
	Matrix	matBuf = *this;
	float fSin = sinf(fAngle), fCos = cosf (fAngle);

	//float s = sin(DEGTORAD(angle));
	//float c = cos(DEGTORAD(angle));

	vAxis.Normalize();

	float ux = vAxis.x;
	float uy = vAxis.y;
	float uz = vAxis.z;

	_11  = fCos + (1-fCos) * ux;
	_12 = (1-fCos) * ux*uy + fSin*uz;
	_13  = (1-fCos) * ux*uz - fSin*uy;
	_14  = 0;

	_21  = (1-fCos) * uy*ux - fSin*uz;
	_22  = fCos + (1-fCos) * pow(uy,2);
	_23  = (1-fCos) * uy*uz + fSin*ux;
	_24  = 0;

	_31  = (1-fCos) * uz*ux + fSin*uy;
	_32  = (1-fCos) * uz*uz - fSin*ux;
	_33 = fCos + (1-fCos) * pow(uz,2);
	_34 = 0;

	_41 = 0;
	_42 = 0;
	_43 = 0;
	_44 = 1;

	return *this;
}

// poradie uskutocnenych rotacii je ZXY!, rovnake pouziva DirectX
// DX ekvivalent je D3DMatrixRotationYawPitchRoll (fAngleY, fAngleX, fAngleZ) - poradie parametrov YXZ!
//---------------------------------
inline Matrix &Matrix::RotateZXY (const float fAngleX, const float fAngleY, const float fAngleZ)
{
	this->RotateZ (fAngleZ);
	this->RotateX (fAngleX);
	this->RotateY (fAngleY);
	return *this;
}

// Relative - verzie funkcii nerotuju okolo absolutneho pociatku ale
// zachovavaju povodnu poziciu matice
//---------------------------------
inline Matrix &Matrix::RotateRelativeX (const float fAngle)
{
	Matrix	matBuf = *this;
	float fSin = sinf(fAngle), fCos = cosf (fAngle);

	_12 = matBuf._12 * fCos - matBuf._13 * fSin;
	_13 = matBuf._12 * fSin + matBuf._13 * fCos;
	_22 = matBuf._22 * fCos - matBuf._23 * fSin;
	_23 = matBuf._22 * fSin + matBuf._23 * fCos;
	_32 = matBuf._32 * fCos - matBuf._33 * fSin;
	_33 = matBuf._32 * fSin + matBuf._33 * fCos;
	return *this;
}

//---------------------------------
inline Matrix &Matrix::RotateRelativeY (const float fAngle)
{
	Matrix	matBuf = *this;
	float fSin = sinf(fAngle), fCos = cosf (fAngle);

	_11 = matBuf._11 * fCos + matBuf._13 * fSin;
	_13 = -matBuf._11 * fSin + matBuf._13 * fCos;
	_21 = matBuf._21 * fCos + matBuf._23 * fSin;
	_23 = -matBuf._21 * fSin + matBuf._23 * fCos;
	_31 = matBuf._31 * fCos + matBuf._33 * fSin;
	_33 = -matBuf._31 * fSin + matBuf._33 * fCos;
	return *this;
}

//---------------------------------
inline Matrix &Matrix::RotateRelativeZ (const float fAngle)
{
	Matrix	matBuf = *this;
	float fSin = sinf(fAngle), fCos = cosf (fAngle);

	_11 = matBuf._11 * fCos - matBuf._12 * fSin;
	_12 = matBuf._11 * fSin + matBuf._12 * fCos;
	_21 = matBuf._21 * fCos - matBuf._22 * fSin;
	_22 = matBuf._21 * fSin + matBuf._22 * fCos;
	_31 = matBuf._31 * fCos - matBuf._32 * fSin;
	_32 = matBuf._31 * fSin + matBuf._32 * fCos;
	return *this;
}

// poradie uskutocnenych rotacii je ZXY!, rovnake pouziva DirectX
// DX ekvivalent je D3DMatrixRotationYawPitchRoll (fAngleY, fAngleX, fAngleZ) - poradie parametrov YXZ!
//---------------------------------
inline Matrix &Matrix::RotateRelativeZXY (const float fAngleX, const float fAngleY, const float fAngleZ)
{
	this->RotateRelativeZ (fAngleZ);
	this->RotateRelativeX (fAngleX);
	this->RotateRelativeY (fAngleY);
	return *this;
}

//---------------------------------
inline float Matrix::Cofactor (DWORD i, DWORD j) const
{
	DWORD		i0, i1, i2, j0, j1, j2;
	switch (i)
	{
	case 0:	i0 = 1; i1 = 2; i2 = 3; break;
	case 1:	i0 = 0; i1 = 2; i2 = 3; break;
	case 2:	i0 = 0; i1 = 1; i2 = 3; break;
	case 3:	i0 = 0; i1 = 1; i2 = 2;
	}
	switch (j)
	{
	case 0:	j0 = 1; j1 = 2; j2 = 3; break;
	case 1:	j0 = 0; j1 = 2; j2 = 3; break;
	case 2:	j0 = 0; j1 = 1; j2 = 3; break;
	case 3:	j0 = 0; j1 = 1; j2 = 2;
	}
	//	compute the determinant of the resulting 3x3 matrix_3d
	float det =	m[i0][j0] * (m[i1][j1] * m[i2][j2] - m[i1][j2] * m[i2][j1]) -
		m[i0][j1] * (m[i1][j0] * m[i2][j2] - m[i1][j2] * m[i2][j0]) +
		m[i0][j2] * (m[i1][j0] * m[i2][j1] - m[i1][j1] * m[i2][j0]);
	return ((i+j) & 0x00000001) ? -det : det;
}

//---------------------------------
inline float Matrix::Determinant () const
{
	return 	m[0][0] * Cofactor (0, 0) +
		m[0][1] * Cofactor (0, 1) +
		m[0][2] * Cofactor (0, 2) +
		m[0][3] * Cofactor (0, 3);
}

//---------------------------------
inline float Matrix::Det2() const
{
	return _11 * _22 - _21 * _12;
}

//---------------------------------
inline float Matrix::Det3() const
{
	return	_11 * (_22 * _33 - _23 * _32) +
				_12 * (_23 * _31 - _21 * _33) +
				_13 * (_21 * _32 - _22 * _31);
}

//---------------------------------
inline float Matrix::Det4() const
{
	const float M3344 = _33 * _44 - _34 * _43;
	const float M2344 = _32 * _44 - _34 * _42;
	const float M2334 = _32 * _43 - _33 * _42;
	const float M1344 = _31 * _44 - _34 * _41;
	const float M1334 = _31 * _43 - _33 * _41;
	const float M1324 = _31 * _42 - _32 * _41;
	return	_11 * (_22 * M3344 - _23 * M2344 + _24 * M2334) -
		_12 * (_21 * M3344 - _23 * M1344 + _24 * M1334) +
		_13 * (_21 * M2344 - _22 * M1344 + _24 * M1324) -
		_14 * (_21 * M2334 - _22 * M1334 + _23 * M1324);
}

//---------------------------------
inline Matrix &Matrix::Inverse ()
{
	Matrix	inverse;
	float	det = Determinant ();
	if (det == 0) return *this;		// neexistuje inverzna matica
	det = 1.0f / det;
	for (DWORD i = 0; i < 4; i++)
		for (DWORD j = 0; j < 4; j++)
			inverse.m[j][i] = Cofactor (i, j) * det;
	*this = inverse;
	return *this;
}

/*
// podstatne rychlejsi sposob vypoctu inverse matice
// pravdepodobne funguje iba pre matice bez scalingu
//---------------------------------
inline Matrix &Matrix::Inverse()
{
	Matrix temp;

	// Transponovani rotace
	temp.m16[0] = m16[0]; temp.m16[1] = m16[4]; temp.m16[2]  = m16[8];
	temp.m16[4] = m16[1]; temp.m16[5] = m16[5]; temp.m16[6]  = m16[9];
	temp.m16[8] = m16[2]; temp.m16[9] = m16[6]; temp.m16[10] = m16[10];

	// Vynulovani prave strany matice
	temp.m16[3] = 0.0f; temp.m16[7] = 0.0f; temp.m16[11] = 0.0f; temp.m16[15] = 1.0f;

	// Translace je minus skalarni soucin translace a rotace
	temp.m16[12] = -(m16[12]*m16[0]) - (m16[13]*m16[1]) - (m16[14]*m16[2]);
	temp.m16[13] = -(m16[12]*m16[4]) - (m16[13]*m16[5]) - (m16[14]*m16[6]);
	temp.m16[14] = -(m16[12]*m16[8]) - (m16[13]*m16[9]) - (m16[14]*m16[10]);

	return (*this = temp);
}
*/

//---------------------------------
inline Matrix &Matrix::LookAt(Vec3 &vEye, Vec3 &vAt,Vec3 &vUp)
{
	Vec3 zaxis = (vAt - vEye).Normalize();
	Vec3 xaxis = (vUp%zaxis).Normalize();
	Vec3 yaxis = zaxis%xaxis;

	_11 = xaxis.x;       _12 = yaxis.x;       _13 = zaxis.x;       _14 = 0.0f;
	_21 = xaxis.y;       _22 = yaxis.y;       _23 = zaxis.y;       _24 = 0.0f;
	_31 = xaxis.z;       _32 = yaxis.z;       _33 = zaxis.z;       _34 = 0.0f;
	_41 = -(xaxis*vEye); _42 = -(yaxis*vEye); _43 = -(zaxis*vEye); _44 = 1.0f;

	return *this;
}

//---------------------------------
inline Matrix &Matrix::PerspectiveFov(float fovy, float aspect, float zNearPlane, float zFarPlane)
{
	float yscale = 1.0f/tanf(fovy/2.0f);
	float xscale = yscale/aspect;

	_11 = xscale;	_12 = 0.0f;		_13 = 0.0f;												_14 = 0.0f;
	_21 = 0.0f;		_22 = yscale;	_23 = 0.0f;												_24 = 0.0f;
	_31 = 0.0f;		_32 = 0.0f;		_33 = zFarPlane/(zFarPlane-zNearPlane);					_34 = 1.0f;
	_41 = 0.0f;		_42 = 0.0f;		_43 = -zNearPlane*zFarPlane/(zFarPlane-zNearPlane);		_44 = 0.0f;

	return *this;
}

// vypocita iba inverznu rotaciu a scaling, nemeni posun matice
// pre ortonormalne matice, ktore neobsahuju scaling staci pre inverziu zavolat Transpose3x3()
//---------------------------------
inline Matrix &Matrix::Inverse3x3 ()
{
	Matrix inverse;
	float	det;

	det = _11*_22*_33 + _12*_23*_31 + _13*_21*_32 - _13*_22*_31 - _11*_23*_32 - _12*_21*_33;
	if (det == 0) return *this;	// neexistuje inverzna matica
	det = 1.0f / det;

	inverse._11 = (_22*_33 - _23*_32) * det;
	inverse._12 = (_12*_33 - _13*_32) * -det;
	inverse._13 = (_12*_23 - _13*_22) * det;
	inverse._14 = 0.0f;

	inverse._21 = (_21*_33 - _23*_31) * -det;
	inverse._22 = (_11*_33 - _13*_31) * det;
	inverse._23 = (_11*_23 - _13*_21) * -det;
	inverse._24 = 0.0f;

	inverse._31 = (_21*_32 - _22*_31) * det;
	inverse._32 = (_11*_32 - _12*_31) * -det;
	inverse._33 = (_11*_22 - _12*_21) * det;
	inverse._34 = 0.0f;

	// copy posit from this
	inverse._41 = _41;
	inverse._42 = _42;
	inverse._43 = _43;
	inverse._44 = _44;

	*this = inverse;
	return *this;
}

// presne porovnanie
//---------------------------------
inline bool Matrix::operator== (const Matrix &B) const
{
	for (register DWORD i=0; i<4; i++)
		for (register DWORD j=0; j<4; j++)
			if (B.m[i][j] != this->m[i][j])
				return false;
	return true;
}

// verzia s epsilon chybou
//---------------------------------
inline bool Matrix::Compare (const Matrix &B, float fEpsilon) const
{
	for (register DWORD i=0; i<4; i++)
		for (register DWORD j=0; j<4; j++)
			if (fabs(B.m[i][j] - this->m[i][j]) > fEpsilon)
				return false;
	return true;
}

//---------------------------------
inline Matrix Matrix::operator* (const Matrix &B) const
{
	Matrix A;
	A._11 = _11 * B._11 + _12 * B._21 + _13 * B._31 + _14 * B._41;
	A._12 = _11 * B._12 + _12 * B._22 + _13 * B._32 + _14 * B._42;
	A._13 = _11 * B._13 + _12 * B._23 + _13 * B._33 + _14 * B._43;
	A._14 = _11 * B._14 + _12 * B._24 + _13 * B._34 + _14 * B._44;

	A._21 = _21 * B._11 + _22 * B._21 + _23 * B._31 + _24 * B._41;
	A._22 = _21 * B._12 + _22 * B._22 + _23 * B._32 + _24 * B._42;
	A._23 = _21 * B._13 + _22 * B._23 + _23 * B._33 + _24 * B._43;
	A._24 = _21 * B._14 + _22 * B._24 + _23 * B._34 + _24 * B._44;

	A._31 = _31 * B._11 + _32 * B._21 + _33 * B._31 + _34 * B._41;
	A._32 = _31 * B._12 + _32 * B._22 + _33 * B._32 + _34 * B._42;
	A._33 = _31 * B._13 + _32 * B._23 + _33 * B._33 + _34 * B._43;
	A._34 = _31 * B._14 + _32 * B._24 + _33 * B._34 + _34 * B._44;

	A._41 = _41 * B._11 + _42 * B._21 + _43 * B._31 + _44 * B._41;
	A._42 = _41 * B._12 + _42 * B._22 + _43 * B._32 + _44 * B._42;
	A._43 = _41 * B._13 + _42 * B._23 + _43 * B._33 + _44 * B._43;
	A._44 = _41 * B._14 + _42 * B._24 + _43 * B._34 + _44 * B._44;
	return A;
}

//---------------------------------
inline Matrix &Matrix::operator*= (const Matrix &B)
{
    Matrix tmp = *this;
    tmp *= B;
    *this = tmp;
	return *this;
}

//---------------------------------
inline void Matrix::TransformVector(Vec3 &v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;

	v.x = x * _11 + y * _21 + z * _31;
	v.y = x * _12 + y * _22 + z * _32;
	v.z = x * _13 + y * _23 + z * _33;
}

/*
// nasobenie matic, predpoklada, ze posledny stlpec oboch matic je <0,0,0,1>
// plati asi len pre World matice (neplati pre View a Projection)
// asi o 40% rychlejsia ako bezne nasobenie
//---------------------------------
inline Matrix Matrix::operator% (const Matrix &B) const
{
Matrix A;
A._11 = _11 * B._11 + _12 * B._21 + _13 * B._31;
A._12 = _11 * B._12 + _12 * B._22 + _13 * B._32;
A._13 = _11 * B._13 + _12 * B._23 + _13 * B._33;
A._14 = 0.0f;

A._21 = _21 * B._11 + _22 * B._21 + _23 * B._31;
A._22 = _21 * B._12 + _22 * B._22 + _23 * B._32;
A._23 = _21 * B._13 + _22 * B._23 + _23 * B._33;
A._24 = 0.0f;

A._31 = _31 * B._11 + _32 * B._21 + _33 * B._31;
A._32 = _31 * B._12 + _32 * B._22 + _33 * B._32;
A._33 = _31 * B._13 + _32 * B._23 + _33 * B._33;
A._34 = 0.0f;

A._41 = _41 * B._11 + _42 * B._21 + _43 * B._31 + B._41;
A._42 = _41 * B._12 + _42 * B._22 + _43 * B._32 + B._42;
A._43 = _41 * B._13 + _42 * B._23 + _43 * B._33 + B._43;
A._44 = 1.0f;
return A;
}

//---------------------------------
inline Matrix &Matrix::operator%= (const Matrix &B)
{
return *this = (*this) % B;
}
*/



// useful for skinning, doesn't contain .w component
//---------------------------------
struct Matrix4x3
{
public:
	union
	{
		struct {
			/*Vec3	m_front3;
			Vec3	m_up3;
			Vec3	m_right3;
			Vec3	m_posit3;*/

			struct {float        _11, _12, _13;};
            struct {float        _21, _22, _23;};
            struct {float        _31, _32, _33;};
            struct {float        _41, _42, _43;};

		};
		float m[4][3];
		float m12[12];
	};

	Matrix4x3 () {};
	Matrix4x3 (const Matrix &B)
	{*this = B;};

	Matrix4x3 & operator= (const Matrix&B)
	{
		/*m_front3 = Vec3(B._11, B._12, B._13);
		m_up3 = Vec3(B._21, B._22, B._23);
		m_right3 = Vec3(B._31, B._32, B._33);
		m_posit3 = Vec3(B._41, B._42, B._43);*/

		_11 = B._11; _12 = B._12; _13 = B._13;
		_21 = B._21; _22 = B._22; _23 = B._23;
		_31 = B._31; _32 = B._32; _33 = B._33;
		_41 = B._41; _12 = B._42; _43 = B._43;

		return *this;
	}
};

};
