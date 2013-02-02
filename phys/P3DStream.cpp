/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "PhysX/NxPhysics.h"
#include "PhysEngine.h"
#include "P3dStream.h"



namespace P3D
{
	P3DStream::P3DStream(const wchar *filename, bool load)
	{
		m_pFS = CPhysEngine::mEngine()->mFilesystem();
		m_pF = m_pFS->Open(filename, load ? _W("rb") : _W("wb"));
	}

	P3DStream::~P3DStream()
	{
		if(m_pF) m_pFS->Close(m_pF);
	}

	NxU8 P3DStream::readByte() const
	{
		NxU8 b;
		size_t r = m_pFS->Read(&b, sizeof(NxU8), 1, m_pF);
		NX_ASSERT(r!=0);
		return b;
	}

	NxU16 P3DStream::readWord() const
	{
		NxU16 w;
		size_t r = m_pFS->Read(&w, sizeof(NxU16), 1, m_pF);
		NX_ASSERT(r!=0);
		return w;
	}

	NxU32 P3DStream::readDword() const
	{
		NxU32 d;
		size_t r = m_pFS->Read(&d, sizeof(NxU32), 1, m_pF);
		NX_ASSERT(r!=0);
		return d;
	}

	float P3DStream::readFloat() const
	{
		NxReal f;
		size_t r = m_pFS->Read(&f, sizeof(NxReal), 1, m_pF);
		NX_ASSERT(r!=0);
		return f;
	}

	double P3DStream::readDouble() const
	{
		NxF64 f;
		size_t r = m_pFS->Read(&f, sizeof(NxF64), 1, m_pF);
		NX_ASSERT(r!=0);
		return f;
	}

	void P3DStream::readBuffer(void* buffer, NxU32 size)	const
	{
		size_t w = m_pFS->Read(buffer, size, 1, m_pF);
		NX_ASSERT(w!=0);
	}

	// Saving API
	NxStream& P3DStream::storeByte(NxU8 b)
	{
		size_t w = m_pFS->Write(&b, sizeof(NxU8), 1, m_pF);
		NX_ASSERT(w!=0);
		return *this;
	}

	NxStream& P3DStream::storeWord(NxU16 w)
	{
		size_t ww = m_pFS->Write(&w, sizeof(NxU16), 1, m_pF);
		NX_ASSERT(ww!=0);
		return *this;
	}

	NxStream& P3DStream::storeDword(NxU32 d)
	{
		size_t w = m_pFS->Write(&d, sizeof(NxU32), 1, m_pF);
		NX_ASSERT(w!=0);
		return *this;
	}

	NxStream& P3DStream::storeFloat(NxReal f)
	{
		size_t w = m_pFS->Write(&f, sizeof(NxReal), 1, m_pF);
		NX_ASSERT(w!=0);
		return *this;
	}

	NxStream& P3DStream::storeDouble(NxF64 f)
	{
		size_t w = m_pFS->Write(&f, sizeof(NxF64), 1, m_pF);
		NX_ASSERT(w!=0);
		return *this;
	}

	NxStream& P3DStream::storeBuffer(const void* buffer, NxU32 size)
	{
		size_t w = m_pFS->Write(buffer, size, 1, m_pF);
		NX_ASSERT(w!=0);
		return *this;
	}

}