/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

/*
This is platform-independent text configuration loading/saving system
*/

#include <time.h>
#include <stdio.h>
#include "json/json.h"

#include "Config.h"
#include "Engine.h"

namespace P3D {

bool  CConfig::Load(const wchar* path, const wchar* loadOnlyCategoryName/*=NULL*/)
{
	int numTotal=0;
	Json::Value root;
	BYTE* pData = NULL; unsigned long size = 0;

	wchar szPhPath[MAX_PATH];

	// construct physical path
	wstrcpy(szPhPath, I_GetBinaryPath());
	wstrcat(szPhPath, _W("../data/"));
	wstrcat(szPhPath, path);

	// open file
	FILE* fp = wfopen(szPhPath, _W("rb"));

	if (fp) // --- existing configuration found, so load it
	{
		// load file content
		fseek(fp, 0, SEEK_END);
		size = (unsigned long)ftell(fp);
		fseek(fp, 0, SEEK_SET);
		pData = new BYTE[size+1];
		fread(pData, 1, size, fp);
		pData[size]=0;

		// parse existing config
		Json::Reader reader;
		if (!reader.parse((const char*)pData, (const char*)(pData+size), root, true))
		{
			CON(MSG_ERR, _W("Configuration %s failed to parse!"), path);
			CON(MSG_ERR, _W("%s"), reader.getFormatedErrorMessages().c_str());
			return false;
		}

		delete[] pData;
		fclose(fp);
	}
	else
	{
		CON(MSG_ERR, _W("Configuration file %s cannot be opened!"), path);
		return false;
	}

	// got through all the Core SVarCategories
	for (SVarCategory *svc = g_pClsMgr->GetAllSVars(); svc; svc=svc->next)
	{
		if (loadOnlyCategoryName && wstricmp( svc->GetName(), loadOnlyCategoryName)) continue; // if category to save was specified, skip other

		const tArray<SVar*> & sv = svc->GetSVars(); // pairs from SVarCategory in core

		Json::Value* jsoncat = &root[svc->GetName()];
		if (jsoncat->type() == Json::nullValue) continue; // this category was not written in config, skip

		// go throught all SVars in this Core SVarCategory
		for (unsigned int svar=0; svar<sv.Size(); svar++)
		{
			if (!sv[svar]->CanBeSaved()) continue; // cannot be saved

			Json::Value* pair = &(*jsoncat)[sv[svar]->GetName()];
			if (pair->type() == Json::nullValue) continue; // this pair was not written in config, skip

			numTotal++;

			switch(sv[svar]->GetType())
			{
			case SVT_FLOAT:
				sv[svar]->SetFloat((float)pair->asDouble());
				break;
			case SVT_INT:
				sv[svar]->SetInt(pair->asInt());
				break;
			case SVT_STRING:
				sv[svar]->SetString( pair->asString().c_str() );
				break;
			default:
				assertd(false, "This should not happen!! Trying to save SVar %s with unknown type!!");
			}
		}
	}

	CON(MSG_INFO, _W("Configuration %s loaded (%d variables)"), path, numTotal);

	return (numTotal>0);
}

//-----------------------------------

bool  CConfig::Save(const wchar* path, const wchar* saveOnlyCategoryName/*=NULL*/)
{
	int numTotal=0;
	Json::Value root;
	BYTE* pData = NULL; unsigned long size = 0;

	wchar szPhPath[MAX_PATH];

	// construct physical path
	wstrcpy(szPhPath, I_GetBinaryPath());
	wstrcat(szPhPath, _W("../data/"));
	wstrcat(szPhPath, path);

	// if we are saving only one category, load previously saved vars from file
	if (saveOnlyCategoryName && *saveOnlyCategoryName)
	{
		// open file
		FILE* fp = wfopen(szPhPath, _W("rb"));

		if (fp) // --- existing configuration found, so load it
		{
			// load file content
			fseek(fp, 0, SEEK_END);
			size = (unsigned long)ftell(fp);
			fseek(fp, 0, SEEK_SET);
			pData = new BYTE[size+1];
			fread(pData, 1, size, fp);
			pData[size]=0;

			// parse existing config
			Json::Reader reader;
			if (!reader.parse((const char *)pData, (const char *)(pData+size), root, true))
				CON(MSG_ERR, _W("Configuration %s failed to parse!"), path);

			delete[] pData;
			fclose(fp);
		}
	}

	// construct head comment
	time_t tim;
	time(&tim);
	tm *timm = localtime(&tim);
	wchar head[64];
	wsprintf(head, 63, _W("// Panther3D Settings - saved %.2d.%.2d.%d at %.2d:%.2d:%.2d"), 
		timm->tm_mday, timm->tm_mon+1, timm->tm_year+1900, timm->tm_hour, timm->tm_min, timm->tm_sec);

	root.setComment(head, Json::commentBefore);

	// MAKE JSON STRUCTURE ------------------------------------------------

	// got through all the Core SVarCategories
	for (SVarCategory *svc = g_pClsMgr->GetAllSVars(); svc; svc=svc->next)
	{
		if (saveOnlyCategoryName && wstricmp( svc->GetName(), saveOnlyCategoryName)) continue; // if category to save was specified, skip other

		const tArray<SVar*> & sv = svc->GetSVars(); // pairs from SVarCategory in core

		Json::Value* jsoncat = &root[svc->GetName()];

		// go throught all SVars in this Core SVarCategory
		for (unsigned int svar=0; svar<sv.Size(); svar++)
		{
			if (!sv[svar]->CanBeSaved()) continue; // cannot be saved

			switch(sv[svar]->GetType())
			{
			case SVT_FLOAT:
				(*jsoncat)[sv[svar]->GetName()] = (double)sv[svar]->GetFloat();
				break;
			case SVT_INT:
				(*jsoncat)[sv[svar]->GetName()] = sv[svar]->GetInt();
				break;
			case SVT_STRING:
				(*jsoncat)[sv[svar]->GetName()] = sv[svar]->GetString();
				break;
			default:
				assertd(false, "This should not happen!! Trying to save SVar %s with unknown type!!");
			}

			if(sv[svar]->GetHelp()) 
			{
				wchar* hlp = new wchar[4+wstrlen(sv[svar]->GetHelp())];
				wsprintf(hlp, 999999, _W("// %s"), sv[svar]->GetHelp());
				(*jsoncat)[sv[svar]->GetName()].setComment(hlp, Json::commentAfterOnSameLine);
				delete[] hlp;
			}
			numTotal++;
		}
	}

	// SAVE TO FILE ------------------------------------------------
	FILE* fp = wfopen(szPhPath, _W("wb"));
	if (!fp) 
	{
		// make sure path to file is created
		wchar szdir[MAX_PATH];
		wstrcpy(szdir, szPhPath);
		for (unsigned int i=0;i<wstrlen(szdir);i++)
			if (szdir[i]=='/'||szdir[i]=='\\') { 
				szdir[i]=0;
#ifdef WIN32
				CreateDirectory(szdir, NULL) ? 0 : 1;
#else
				mkdir(szdir,777);
#endif
				szdir[i]=szPhPath[i];
			}
			fp = wfopen(szPhPath, _W("wb")); // try to open again
			if (!fp)
			{
				CON(MSG_INFO, _W("Configuration %s could not be saved! File not writable? Disk full?"), path);
				return false;
			}
	}

	// WRITE JSON TO FILE ------------------------------------------------
	Json::StyledWriter writer;
	std::string strout = writer.write(root);

	fwrite(strout.c_str(), 1, strout.length(), fp);
	fclose(fp);


	if (saveOnlyCategoryName && *saveOnlyCategoryName)
		CON(MSG_INFO, _W("Configuration %s saved (%d variables - only %s category)"), path, numTotal, saveOnlyCategoryName);
	else
		CON(MSG_INFO, _W("Configuration %s saved (%d variables - all categories)"), path, numTotal);

	return (numTotal>0);
}

}; // ns