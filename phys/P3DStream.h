/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "PhysX/NxStream.h"
#include "IFilesystem.h"

namespace P3D
{

	class P3DStream : public NxStream
	{
	public:
		P3DStream(const wchar* filename, bool load);
		virtual						~P3DStream();

		virtual		NxU8			readByte()								const;
		virtual		NxU16			readWord()								const;
		virtual		NxU32			readDword()								const;
		virtual		float			readFloat()								const;
		virtual		double			readDouble()							const;
		virtual		void			readBuffer(void* buffer, NxU32 size)	const;

		virtual		NxStream&		storeByte(NxU8 b);
		virtual		NxStream&		storeWord(NxU16 w);
		virtual		NxStream&		storeDword(NxU32 d);
		virtual		NxStream&		storeFloat(NxReal f);
		virtual		NxStream&		storeDouble(NxF64 f);
		virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size);

		IFileSystem* m_pFS;
		FSFILE *m_pF;
	};

}