/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 11/12/2009 File created - Petr
*/


#include "precompiled.h"

#include "Engine.h"
#include "Filesystem.h"

namespace P3D
{

	CFilesystem::CFilesystem()
	{

	}

	CFilesystem::~CFilesystem()
	{

	}

	bool CFilesystem::Init()
	{
		CON(MSG_INFO, _W("Filesystem initialized with stdio support"));
		return true;
	}

	eFileResult CFilesystem::Load(const wchar *name, void** outData, UINT &outSizeB, bool blocking)
	{
		wchar path[MAX_PATH];
		
		ConvertToStdioPath(name, path);

		FILE* fp = wfopen(path, _W("rb"));
		if (!fp)
		{
			CON(MSG_INFO, _W("Could not load '%s'"), path);
			return P3D_FILERESULT_FAILED;
		}

		// obtain file size:
		fseek(fp, 0, SEEK_END);
		outSizeB = ftell(fp);
		rewind (fp);

		BYTE* data = new BYTE[outSizeB];

		size_t result = fread(data, 1, outSizeB, fp);
		if(result != outSizeB)
		{
			CON(MSG_INFO, _W("Reading error '%s'"), path);
			fclose(fp);
			delete[] data;
			return P3D_FILERESULT_FAILED;
		}

		*outData = data;

		fclose(fp);

		CON(MSG_INFO, _W("'%s' loaded"), path);
		return P3D_FILERESULT_OK;
	}

	void CFilesystem::FreeLoadedData(void *data)
	{
		delete[] data;
	}

	void CFilesystem::ConvertToStdioPath(const wchar *inName, wchar *outPath)
	{
		wchar section[50];

		// construct physical path
		wstrcpy(outPath, I_GetBinaryPath());
		wstrcat(outPath, _W("../data/"));

		UINT pos = 0;
		for (UINT i=0; i<wstrlen(inName); i++){
			if(inName[i] == _W('.')) {
				pos = i;
				break;
			}
		}

		wstrncpy(section, inName, pos);
		section[pos] = 0;
		wstrcat(outPath, section);
		wstrcat(outPath, _W("/"));
		wstrcat(outPath, &inName[pos+1]);

		if( wstrcmp(section, _W("textures"))==0 ) wstrcat(outPath, _W(".rtex"));
		else if( wstrcmp(section, _W("shaders"))==0 ) wstrcat(outPath, _W(".rshader"));
		else if( wstrcmp(section, _W("scripts"))==0 ) wstrcat(outPath, _W(".rscript"));
	}

	eFileResult CFilesystem::Save(const wchar *name, const void *data, UINT dataSizeB, bool append, bool blocking)
	{
		wchar path[MAX_PATH];
		ConvertToStdioPath(name, path);
		
		FILE* fp = wfopen(path, _W("wb"));
		if (!fp)
		{
			CON(MSG_INFO, _W("Could not create '%s'"), path);
			return P3D_FILERESULT_FAILED;
		}
		
		size_t result = fwrite(data, 1, dataSizeB, fp);
		if(result != dataSizeB)
		{
			CON(MSG_INFO, _W("Writing error '%s'"), path);
			fclose(fp);
			return P3D_FILERESULT_FAILED;
		}
		
		fclose(fp);
		
		CON(MSG_INFO, _W("'%s' created"), path);
		
		return P3D_FILERESULT_OK;
	}


}//ns