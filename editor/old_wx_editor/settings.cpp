#include "precompiled.h"

#include "settings.h"
#include <windows.h>
#include <stdio.h>
#include <time.h>

namespace P3D
{
	std::vector<sEditorSetting> Settings::s_esettings;

	const wchar* Settings::GetString(const wchar* name, const wchar* def)
	{
		for (unsigned int i=0;i<s_esettings.size();i++)
		{
			if (!wstrcmp(s_esettings[i].name, name)) 
				return s_esettings[i].value;
		}
		return def;
	}

	float Settings::GetFloat(const wchar* name, float def)
	{
		for (unsigned int i=0;i<s_esettings.size();i++)
		{
			if (!wstrcmp(s_esettings[i].name, name)) 
				return (float)watof(s_esettings[i].value);
		}
		return def;
	}

	int Settings::GetInt(const wchar* name, int def)
	{
		for (unsigned int i=0;i<s_esettings.size();i++)
		{
			if (!wstrcmp(s_esettings[i].name, name)) 
				return watoi(s_esettings[i].value);
		}
		return def;
	}

	long Settings::GetLong(const wchar* name, long def)
	{
		for (unsigned int i=0;i<s_esettings.size();i++)
		{
			if (!wstrcmp(s_esettings[i].name, name)) 
				return watol(s_esettings[i].value);
		}
		return def;
	}

	bool Settings::GetBool(const wchar* name, bool def)
	{
		for (unsigned int i=0;i<s_esettings.size();i++)
		{
			if (!wstrcmp(s_esettings[i].name, name)) 
				return !!watoi(s_esettings[i].value);
		}
		return def;
	}





	void Settings::Set(const wchar* name, const wchar* data)
	{
		for (unsigned int i=0;i<s_esettings.size();i++)
		{
			if (!wstrcmp(s_esettings[i].name, name)) 
			{
				delete[] s_esettings[i].value;
				s_esettings[i].value = new wchar[wstrlen(data)+1];
				wstrcpy(s_esettings[i].value, data);
				return;
			}
		}
		sEditorSetting es;
		es.name = new wchar[wstrlen(name)+1];
		wstrcpy(es.name, name);
		es.value = new wchar[wstrlen(data)+1];
		wstrcpy(es.value, data);
		s_esettings.push_back(es);
	}
	void Settings::Set(const wchar* name, float data)
	{
		wchar va[32]; wsprintf(va, _W("%f"), data);
		Set(name, va);
	}
	void Settings::Set(const wchar* name, int data)
	{
		wchar va[32]; wsprintf(va, _W("%d"), data);
		Set(name, va);
	}
	void Settings::Set(const wchar* name, long data)
	{
		wchar va[32]; wsprintf(va, _W("%ld"), data);
		Set(name, va);
	}
	void Settings::Set(const wchar* name, bool data)
	{
		wchar va[32]; wsprintf(va, _W("%d"), (int)data);
		Set(name, va);
	}






	bool Settings::Load(const wchar* filename)
	{
		// try to load as much as you can
		FILE* fp = wfopen(filename, _W("rb"));
		if (!fp) return false;

		fseek(fp, 0, SEEK_END);
		long len = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		wchar *pData = new wchar[len]; // was unsigned char //W

		fread(pData, 1, len, fp);
		fclose(fp);

		// parse data
		bool somethingParsed=false; 
		long fromName=0; long fromValue=0;
		wchar name[32];
		for (long i=0;i<len;i++)
		{
			 // we skip new line chars
			if (pData[i]=='\n' || pData[i]=='\r')
				continue;

			// begin name
			if (pData[i]=='<') 
				fromName=i+1;

			// end name
			if (pData[i]=='>')
			{
				wstrncpy(name, (const wchar *)&pData[fromName], i-fromName);
				name[i-fromName]=0;
			}

			// begin value
			if (pData[i]=='[') 
				fromValue=i+1;

			// end value
			if (pData[i]==']')
			{
				sEditorSetting set;
				//name
				set.name = new wchar[wstrlen(name)+1];
				wstrcpy(set.name, name);
				//value
				set.value = new wchar[i-fromValue+1];
				wstrncpy(set.value, (const wchar *)&pData[fromValue], i-fromValue);
				set.value[i-fromValue]=0;

				s_esettings.push_back(set);
			}
		}

		//free buffer
		delete[] pData;
		return true;
	}

	bool Settings::Save(const wchar* filename)
	{
		// save all settings
	/*	FILE* fp = fopen(filename, "wb");
		if (!fp) return false;

		// write header
		time_t tim;
		time(&tim);
		tm *timm = localtime(&tim);
		wchar head[64];
		wsprintf(head, "P3DCrane2 Settings v1.0 - saved %d.%d.%d %d:%d:%d\r\n", 
			timm->tm_mday, timm->tm_mon+1, timm->tm_year, timm->tm_hour, timm->tm_min, timm->tm_sec);
		fwrite(head, 1, wstrlen(head), fp);

		// go through all settings
		wchar nl[2]="<"; wchar nr[2]=">";
		wchar vl[2]="["; wchar vr[2]="]";
		for (unsigned int i=0;i<s_esettings.size();i++)
		{
			// name
			fwrite(nl, 1, 1, fp);
			fwrite(s_esettings[i].name, 1, wstrlen(s_esettings[i].name), fp);
			fwrite(nr, 1, 1, fp);

			// new line
			fwrite("\r\n", 1, 2, fp);

			// value
			fwrite(vl, 1, 1, fp);
			fwrite(s_esettings[i].value, 1, wstrlen(s_esettings[i].value), fp);
			fwrite(vr, 1, 1, fp);

			// new line
			fwrite("\r\n", 1, 2, fp);

			// free buffers
			delete[] s_esettings[i].name;
			delete[] s_esettings[i].value;
		}

		//end
		fwrite("<end>", 1, 5, fp);

		fclose(fp);*/
		return true;
	}

}