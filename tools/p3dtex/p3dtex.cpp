// p3dmesh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rtex.h"

#include <time.h>

#include <IL/il.h>
#include <IL/ilu.h>

#include "ddsloader.h"

// only for debug output
#include <iostream>
using namespace std;

namespace P3D {

int NearestPow2(int value)
{
	int size = 2;
	while(size*2 <= value)
	{
		size *= 2;
	}
	return size;
}

int Ukol(int uroven, int x, int y)
{
	if (uroven>100) return -1;

	if (x<y)
	{
		x = x+2;
		y = 3*x+y+6;
		uroven++;

		//printf("#%d : x=%d y=%d\n", uroven, x, y);
		return Ukol (uroven, x, y);
	}
	return uroven;
}

int main(int argc, char* argv[])
{
	for (int i=-100; i<100; i++)
	{
		int kroku = Ukol(0, -5, i);
		if (kroku>=0)
			printf("zastavi pro %d (%d kroku)\n", i, kroku);
		else
			printf("NEzastavi pro %d\n", i);
	}

	/*
	int x=-5;
	int y=1;

	int i=0;
	while(x<y)
	{
		i++;
		x=x+2;
		y=3*x+y+6;
		printf("#%d : x=%d y=%d\n", i, x, y);
	}
	printf("Necykli");*/
	
	getch();
	return 0;
	printf("P3DTex - Reversity Texture Convertor\n");
	printf("====================================\n\n");

	char szSrc[512]="";
	char szDest[512]="";

	if (argc<2)
	{
		printf("Usage: p3dtex.exe input_path [output_path]");
		getch();
		return 0;
	}
	else if (argc==2)
	{
		strcpy(szSrc, argv[1]);
		strcpy(szDest, argv[1]);
		// remove extension
		for (unsigned int i=strlen(szDest)-1; i>1; i--)
		{
			if (szDest[i]=='.')
			{
				szDest[i]=0;
				break;
			}
		}
		// add our extension
		strcat(szDest, ".rtex");
	}
	else
	{
		strcpy(szSrc, argv[1]);
		strcpy(szDest, argv[2]);
	}

	printf("Initializing converter...\n");

	//ilInit();
	//iluInit();

	printf("Loading and processing file...\n");

	// DDS LOADER
	CDDSLoader ddsl;
	if(!ddsl.Load(szSrc))
	{
		printf("ERROR: Failed to load '%s' input file.", szSrc);
		getch();
		return -1;
	}
	else
	{
		printf("Loading texture using native dds loader...\n");
	}

	if (ddsl.GetNumMips()==0)
	{
		printf("ERROR: Imported texture '%s' doesn't have mipmap chain!", szSrc);
		getch();
		return -1;
	}

/*
	ILuint image;
	ilGenImages(1, &image);

	ilBindImage(image);
	if( ilLoadImage(szSrc) == IL_FALSE )
	{
		ilDeleteImages(1, &image);
		printf("ERROR: Failed to load '%s' input file.", szSrc);
		getch();
		return -1;
	}*/

	printf("File successfuly loaded.\nExporting to rtex...\n");

	FILE* fp = fopen(szDest, "wb");
	if (!fp)
	{
		printf("ERROR: Failed to open output file!");
		getch();
		return -1;
	}

	// -- BASE HEADER - updated header will be written at the end  -------------------------
	sRTHeader rhead;
	rhead.magic[0]='R'; rhead.magic[1]='T';
	rhead.major=RT_MAJOR; rhead.minor=RT_MINOR;

	fwrite(&rhead, sizeof(sRTHeader), 1, fp);

	// -- FILE INFO ------------------------------------------------------------------------
	// these config parameters will be used while converting later in this code ;)
	rhead.contents[RT_FILE_INFO].offset = ftell(fp);
	sRTInfo rinfo;

	// in case of dds, copy info from loader
	rinfo = ddsl.GetInfo();

	time_t rawtime;
	struct tm * ti;
	time ( &rawtime );
	ti = localtime ( &rawtime );

	sprintf(rinfo.created, "%.4d%.2d%.2d%.2d%.2d%.2d", ti->tm_year+1900, ti->tm_mon+1, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
	strcpy(rinfo.imported, rinfo.created);
	rinfo.ipowoftwo = true; // should be power of two
	rinfo.ibuildmips = true; // temporary, but often true

#ifdef _WINDOWS_
#include <windows.h>
	DWORD len=31;
	GetUserName(rinfo.username, &len);
#else
	sprintf(rinfo.username,"N/A");
#endif
	strcpy(rinfo.keywords, "converter");
	
	// try to make absolute path
	char path[512];
	strcpy(path, szSrc);
#ifdef _WINDOWS_
	if (szSrc[1]!=':')
	{
		// not absolute, doesn't contain disk identifier so make absolute
		GetCurrentDirectory(254-strlen(szSrc), path);
		strcat(path, "\\");
		strcat(path, szSrc);
	}
#endif
	strcpy(rinfo.importpath, path);

	fwrite(&rinfo, sizeof(sRTInfo), 1, fp);
	rhead.contents[RT_FILE_INFO].length = ftell(fp)-rhead.contents[RT_FILE_INFO].offset;

	// -- DO IMAGE PROCESSING --------------------------------------------------------------

	// get source image info
	/*ILinfo info;
	iluGetImageInfo(&info);

	if (rinfo.ipowoftwo && (info.Width!=NearestPow2(info.Width) || info.Height!=NearestPow2(info.Height)) )
	{
		cout << "Scaling to power of two... ";
		if (iluScale(NearestPow2(info.Width), NearestPow2(info.Height), info.Depth))
			cout << "SUCCESSED" << endl;
		else
			cout << "FAILED" << endl;
	}

	if (info.NumMips<2 && rinfo.ibuildmips)
	{
		cout << "Creating mipmaps... ";
		if (iluBuildMipmaps())
			cout << "SUCCESSED" << endl;
		else
			cout << "FAILED" << endl;
	}

	iluGetImageInfo(&info);*/

	/*
	rinfo.width = info.Width; rinfo.height = info.Height; rinfo.depth = info.Depth; rinfo.nummips = info.NumMips;
	*/

	// in case of dds

	// basic info
	cout << "--------------------------------" << endl;
	cout << "Texture size: " << rinfo.width << "x" << rinfo.height << "\n";
	cout << "Texture depth: " << rinfo.depth << "\n";
	cout << "Number of mipmaps: " << rinfo.nummips << "\n";
	cout << "--------------------------------" << endl;

	// -- SAVE DDS DATA TO RTEX
	// write initial sRTImageSubset data
	rhead.contents[RT_IMAGE_SUBSET].offset=ftell(fp);
	sRTImageSubset *subsets = new sRTImageSubset[ddsl.GetNumSubsets()];
	fwrite(subsets, 1, sizeof(sRTImageSubset)*ddsl.GetNumSubsets(), fp);
	rhead.contents[RT_IMAGE_SUBSET].length = ftell(fp)-rhead.contents[RT_IMAGE_SUBSET].offset;;

	// write subset data
	unsigned int coffset=0;
	for (unsigned int s=0; s<ddsl.GetNumSubsets(); s++)
	{
		const sImageData* ida = ddsl.GetSubsetByID(s);
		subsets[s] = ida->subset;
		subsets[s].offset = coffset;
		subsets[s].len = ida->size;
		fwrite(ida->pData, 1, ida->size, fp); // write current subset data to rtex
		coffset+=ida->size;
	}

	// update subset struct (offsets was changed)
	fseek(fp, rhead.contents[RT_IMAGE_SUBSET].offset, SEEK_SET);
	for (unsigned int s=0; s<ddsl.GetNumSubsets(); s++)
		fwrite(&subsets[s], 1, sizeof(sRTImageSubset), fp);

	// -- UPDATED HEADER -------------------------------------------------------------------
	fseek(fp, 0, SEEK_SET);
	fwrite(&rhead, sizeof(sRTHeader), 1, fp);
	// ------------------

	printf("Clearing used memory...\n");
	//ilDeleteImages(1, &image);
	//ilShutDown();

	delete[] subsets;
	ddsl.Unload();

	fclose(fp);

	printf("Done.");

	getch();
	return 0;
}

}; // namespace

int main(int argc, char* argv[])
{
	return P3D::main(argc, argv);
}