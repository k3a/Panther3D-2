// p3dmesh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rmesh.h"

#include <time.h>

#include <assimp/assimp.h>
#include <assimp/aiAssert.h>
#include <assimp/aiFileIO.h>
#include <assimp/aiPostProcess.h>
#include <assimp/aiScene.h>

namespace P3D {

int main(int argc, char* argv[])
{
	printf("P3DMesh - Reversity Mesh Convertor\n");
	printf("==================================\n\n");

	char szSrc[512]="";
	char szDest[512]="";

	bool bParamsOK=false;
	bool bForceNormas=false;
	bool bSmoothNormals=false;
	bool bNoAnim=false;

	if (argc>1)
	{
		if (*argv[argc-2]!='-') // user entered source and dest. paths
		{
			strcpy(szSrc, argv[argc-2]);
			strcpy(szDest, argv[argc-1]);
			bParamsOK=true;
		}
		else if (*argv[argc-1]!='-') // user entered only dest. path
		{
			strcpy(szSrc, argv[argc-1]);

			strcpy(szDest, argv[argc-1]);
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
			strcat(szDest, ".rmesh");
			bParamsOK=true;
		}

		// parse additional parameters
		for (int i=argc-1; i>=0; i--)
		{
			if (*argv[i]=='-')
			{
				switch(*(argv[i]+1))
				{
				case 'r':
					bForceNormas=true;
					break;
				case 's':
					bSmoothNormals=true;
					break;
				case 'a':
					bNoAnim=true;
					break;
				}
			}
		}
	}

	if (!bParamsOK)
	{
		printf("Usage: p3dmesh.exe [-r,-s] input_path [output_path]\r\n");
		printf("       -r : Force recompute normals if already exists\r\n");
		printf("       -s : Smooth normals (is normals already exists, you must force gen. with -r)\r\n");
		printf("       -a : Don't convert animation data (useful for LODs of skeletal mesh)\r\n");
		getch();
		return 0;
	}

	printf("Loading and processing file...\n");

	unsigned int flags =  aiProcess_CalcTangentSpace
						| aiProcess_JoinIdenticalVertices
						| aiProcess_Triangulate
						| aiProcess_ConvertToLeftHanded
						| aiProcess_SplitLargeMeshes
						| aiProcess_ValidateDataStructure
						| aiProcess_ImproveCacheLocality
						| aiProcess_RemoveRedundantMaterials;

	if (bForceNormas)
	{
		flags |= aiProcess_KillNormals;
	}

	if (bSmoothNormals)
		flags |= aiProcess_GenSmoothNormals;
	else
		flags |= aiProcess_GenNormals;


	const aiScene* scene = aiImportFile(szSrc, flags );

	if (!scene)
	{
		printf("ERROR: Failed to load input file. %s", aiGetErrorString());
		getch();
		return -1;
	}

	printf("File successfuly loaded.\nExporting to rmesh...\n");

	// check whether mesh contains at least one subset
	if (scene->mNumMeshes==0)
	{
		printf("ERROR: This mesh doesn't contain any mesh data!");
		aiReleaseImport(scene);
		getch();
		return -1;
	}

	// check for vertex position data
	for (unsigned int i=0; i<scene->mNumMeshes; i++)
		if (!scene->mMeshes[i]->HasPositions())
		{
			printf("ERROR: Subset #%d doesn't contain vertex positions!", i);
			aiReleaseImport(scene);
			getch();
			return -1;
		}

	// open dest. file
	FILE* fp = fopen(szDest, "wb");
	if (!fp)
	{
		printf("ERROR: Failed to open output file!");
		getch();
		return -1;
	}

	// -- BASE HEADER - updated header will be written at the end  -------------------------
	sRMHeader rhead;
	rhead.magic[0]='R'; rhead.magic[1]='M';
	rhead.major=RM_MAJOR; rhead.minor=RM_MINOR;

	fwrite(&rhead, sizeof(sRMHeader), 1, fp);

	// -- FILE INFO ------------------------------------------------------------------------
	rhead.contents[RM_FILE_INFO].offset = ftell(fp);
	sRMInfo rinfo;

	time_t rawtime;
	struct tm * ti;
	time ( &rawtime );
	ti = localtime ( &rawtime );

	sprintf(rinfo.created, "%.4d%.2d%.2d%.2d%.2d%.2d", ti->tm_year+1900, ti->tm_mon+1, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
	strcpy(rinfo.imported, rinfo.created);
	rinfo.idontimportanim=bNoAnim;
	rinfo.ifixinfacingnormals=true;
	rinfo.igennewnormals=bForceNormas;
	rinfo.ioptimize=true;
	rinfo.ismoothnormals=bSmoothNormals;

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

	fwrite(&rinfo, sizeof(sRMInfo), 1, fp);
	rhead.contents[RM_FILE_INFO].length = ftell(fp)-rhead.contents[RM_FILE_INFO].offset;

	// -- SUBSETS ------------------------------------------------------------------------
	rhead.contents[RM_SUBSETS].offset = ftell(fp);
	unsigned int numSubsets = scene->mNumMeshes;
	sRMSubset *rsubsets = new sRMSubset[numSubsets];

	printf("\nMesh contain %d subsets:\n", numSubsets);

	unsigned int lastVertID=0; unsigned int lastIndexID=0;
	for(unsigned int i=0; i<numSubsets; i++)
	{
		rsubsets[i].bTexCoords=true;
		if (!scene->mMeshes[i]->HasTextureCoords(0))
		{
			printf("\n Warning: Subset #%d doesn't contain texture coords!\n", i);
			rsubsets[i].bTexCoords=false;
		}

		rsubsets[i].numvertices=scene->mMeshes[i]->mNumVertices;
		rsubsets[i].firstindex=lastIndexID;
		rsubsets[i].numindices=scene->mMeshes[i]->mNumFaces*3; // we have only triangles, see aiProcess_Triangulate

		// try to get diffuse texture path
		strcpy(rsubsets[i].texture, "");
		aiString szTPath;
		if(AI_SUCCESS == aiGetMaterialString(scene->mMaterials[scene->mMeshes[i]->mMaterialIndex],AI_MATKEY_TEXTURE_DIFFUSE(0),&szTPath))
		{
			// remove extension and path
			unsigned int charsFromRight=0; bool exStripped=false;
			for (unsigned int t=szTPath.length-1; t>=0; t--) 
			{
				// strip extension if number of chars from right > 2
				if (!exStripped && szTPath.data[t]=='.' && charsFromRight > 2)
				{
					szTPath.data[t]=0;
					exStripped=true;
				}
				charsFromRight++;

				// first path delimiter found
				if (t==0 || szTPath.data[t-1]=='\\' || szTPath.data[t-1]=='/')
				{
					strncpy(rsubsets[i].texture, &szTPath.data[t], 255);
					break;
				}
			}
		}

		printf(" Subset #%d Verts: %d Inds: %d Tex: %s\n", i, rsubsets[i].numvertices, rsubsets[i].numindices, rsubsets[i].texture);

		lastVertID+=scene->mMeshes[i]->mNumVertices;
		lastIndexID+=scene->mMeshes[i]->mNumFaces*3;
	}

	printf("\n");

	fwrite(rsubsets, sizeof(sRMSubset), numSubsets, fp);
	rhead.contents[RM_SUBSETS].length = ftell(fp)-rhead.contents[RM_SUBSETS].offset;
	delete[] rsubsets;

	// -- VERTICES ------------------------------------------------------------------------
	rhead.contents[RM_VERTICES].offset = ftell(fp);
	unsigned int numVerts = 0;
	for (unsigned int m=0; m<scene->mNumMeshes; m++)
		numVerts+=scene->mMeshes[m]->mNumVertices;
	sRMVertex *rverts = new sRMVertex[numVerts];

	unsigned int currV=0; bool bTC=true;
	for (unsigned int m=0; m<scene->mNumMeshes; m++)
	{
		bTC = scene->mMeshes[m]->HasTextureCoords(0);

		for (unsigned int i=0; i<scene->mMeshes[m]->mNumVertices; i++)
		{
			rverts[currV].bitangent = &scene->mMeshes[m]->mBitangents[i].x;
			rverts[currV].normal = &scene->mMeshes[m]->mNormals[i].x;
			rverts[currV].position = &scene->mMeshes[m]->mVertices[i].x;
			rverts[currV].tangent = &scene->mMeshes[m]->mTangents[i].x;

			if (bTC)
				rverts[currV].uv = &scene->mMeshes[m]->mTextureCoords[0][i].x;
			else
				rverts[currV].uv.x = rverts[currV].uv.y = 0;

			// anim
			rverts[currV].numbones=0;
			//
			currV++;
		}
	}

	fwrite(rverts, sizeof(sRMVertex), numVerts, fp);
	rhead.contents[RM_VERTICES].length = ftell(fp)-rhead.contents[RM_VERTICES].offset;
	delete[] rverts;

	// -- INDICES ------------------------------------------------------------------------
	rhead.contents[RM_INDICES].offset = ftell(fp);
	unsigned int numInds = 0;
	for (unsigned int m=0; m<scene->mNumMeshes; m++)
		numInds+=scene->mMeshes[m]->mNumFaces*3;
	sRMIndex *rinds = new sRMIndex[numInds];

	int currVert=0; int currInd=0;
	for (unsigned int m=0; m<scene->mNumMeshes; m++)
	{
		for(unsigned int f=0; f<scene->mMeshes[m]->mNumFaces; f++)
		{
			rinds[currInd] = currVert+scene->mMeshes[m]->mFaces[f].mIndices[0];
			rinds[currInd+1] = currVert+scene->mMeshes[m]->mFaces[f].mIndices[1];
			rinds[currInd+2] = currVert+scene->mMeshes[m]->mFaces[f].mIndices[2];
			currInd+=3;
		}
		currVert+=scene->mMeshes[m]->mNumVertices;
	}

	fwrite(rinds, sizeof(sRMIndex), numInds, fp);
	rhead.contents[RM_INDICES].length = ftell(fp)-rhead.contents[RM_INDICES].offset;
	delete[] rinds;

	// -- UPDATED HEADER -------------------------------------------------------------------
	fseek(fp, 0, SEEK_SET);
	fwrite(&rhead, sizeof(sRMHeader), 1, fp);
	// ------------------

	printf("Clearing used memory...\n");
	aiReleaseImport(scene);
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