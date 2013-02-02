// Code::Blocks project maker by KEXIk

#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

#if defined(WIN32)
 #include <conio.h>
 #include <windows.h>
#elif defined(LINUX)
 #include <sys/types.h>
 #include <dirent.h>
 #define stricmp strcasecmp
 /* #if !defined(getch)
  void getch()
  {
      char c; cin >> c;
  }
  #endif*/
  #define getch getchar
#endif

/**
project - only string like "engine"
folder - wthout / at the beginning or end like "platform/src_linux"
*/
void AddFolderWindows(FILE* fpOUT, const char* project, const char* folder)
{
#if defined(WIN32)
	int nValid;
	HANDLE searchHandle=NULL;
	WIN32_FIND_DATA fileData;

	char pathToSearch[MAX_PATH];
	strcpy(pathToSearch, project);

	if (folder && *folder)
	{
		strcat(pathToSearch, "/");
		strcat(pathToSearch, folder);
		strcat(pathToSearch, "/*");
	}
	else
		strcat(pathToSearch, "/*");

	searchHandle = FindFirstFile(pathToSearch, &fileData);
	nValid = (searchHandle == INVALID_HANDLE_VALUE) ? 0 : 1;

	while (nValid)
	{
		if (strcmp (fileData.cFileName, ".") != 0 &&
			strcmp (fileData.cFileName, "..") != 0 &&
			( strstr(fileData.cFileName, ".h") || strstr(fileData.cFileName, ".cpp") || strstr(fileData.cFileName, ".c") ) )
		{
			// check for new line at the end of file and add file
			char fpath[256];

			if (folder && *folder)
			{
				sprintf(fpath, "%s/%s/%s", project, folder, fileData.cFileName);
				fprintf(fpOUT, "\t\t<Unit filename=\"%s/%s\" />\n", folder, fileData.cFileName);
			}
			else
			{
				sprintf(fpath, "%s/%s", project, fileData.cFileName);
				fprintf(fpOUT, "\t\t<Unit filename=\"%s\" />\n", fileData.cFileName);
			}

			FILE* tfp = fopen(fpath, "rb");
			if (!tfp)
			{
				cout << " ...cannot open " << fpath << " to check new line at the end!" << endl;
			}
			else
			{
				fseek(tfp, -1, SEEK_END);
				char tch[2]="";
				fread(tch, 1, 1, tfp);
				fclose(tfp);
				if (*tch!='\n')
				{
					cout << " ...adding new line to " << fpath << endl;
					tfp = fopen(fpath, "a+");
					*tch='\n';
					fwrite(tch, 1, 1, tfp);
					fclose(tfp);
				}
			}
		}
		nValid = FindNextFile(searchHandle, &fileData);
	}

	FindClose(searchHandle);
#endif
}

/**
project - only string like "engine"
folder - wthout / at the beginning or end like "platform/src_linux"
*/
void AddFolderLinux(FILE* fpOUT, const char* project, const char* folder)
{
#if defined(LINUX)
	char pathToSearch[256];

	if (folder && *folder)
	{
	    sprintf(pathToSearch, "./%s/%s/", project, folder);
	}
	else
		sprintf(pathToSearch, "./%s/", project);

	DIR * dir = opendir (pathToSearch);

	if (dir == NULL) return;

    struct dirent *d = readdir (dir);

	while (d != NULL){
		//printf ("\"%s\"\n", d->d_name);
        if (strcmp (d->d_name, ".") != 0 &&
            strcmp (d->d_name, "..") != 0 &&
            ( strstr(d->d_name, ".h") || strstr(d->d_name, ".cpp") ) )
        {
			// check for new line at the end of file and add file
			char fpath[256];

			if (folder && *folder)
			{
				sprintf(fpath, "%s/%s/%s", project, folder, d->d_name);
				fprintf(fpOUT, "\t\t<Unit filename=\"%s/%s\" />\n", folder, d->d_name);
			}
			else
			{
				sprintf(fpath, "%s/%s", project, d->d_name);
				fprintf(fpOUT, "\t\t<Unit filename=\"%s\" />\n", d->d_name);
			}

			FILE* tfp = fopen(fpath, "rb");
			if (!tfp)
			{
				cout << " ...cannot open " << fpath << " to check new line at the end!" << endl;
			}
			else
			{
				fseek(tfp, -1, SEEK_END);
				char tch[2]="";
				fread(tch, 1, 1, tfp);
				fclose(tfp);
				if (*tch!='\n')
				{
					cout << " ...adding new line to " << fpath << endl;
					tfp = fopen(fpath, "a+");
					*tch='\n';
					fwrite(tch, 1, 1, tfp);
					fclose(tfp);
				}
			}
        }

    d = readdir (dir);
	}

	closedir(dir);

#endif
}

/**
project - only string like "engine"
*/
void ConstructProject(FILE* fpWorkspace, const char* project)
{
	cout << "Converting project " << project << "..." << endl;

	char path[256];
	sprintf(path, "%s/%s.cbp", project, project);
	FILE* fp = fopen(path, "wb");
	if (!fp)
	{
			cout << " ...can't open " << path << " for writing!" << endl;
			getch();
			return;
	}

	// ******** CONFIGURE PROJECT
	char outputName[64]=""; // output file name
	int projType=3; // type:0-executable, 2-static .a lib, 3-dynamic .so lib
	char libsString[2048]="";
	char dirs[1024]="|"; // delimited and terminated by |

	if (!stricmp(project, "shared"))
	{
		strcpy(outputName, "libshared");
		projType=2; // maybe 3, not sure :-D

		strcat(dirs, "platform|");
		strcat(dirs, "platform/src-linux|");
		strcat(dirs, "platform/src-portable|");
	}
	else if (!stricmp(project, "renderer"))
	{
		strcpy(outputName, "rendererOGL.lp3d");
		projType=3;

		strcat(libsString, "\t\t\t\t\t<Add library=\"SDL\" />\n");
		strcat(libsString, "\t\t\t\t\t<Add library=\"SDLmain\" />\n");
		strcat(libsString, "\t\t\t\t\t<Add library=\"GL\" />\n");
		strcat(libsString, "\t\t\t\t\t<Add library=\"GLU\" />\n");
		strcat(libsString, "\t\t\t\t\t<Add library=\"Cg\" />\n");
		strcat(libsString, "\t\t\t\t\t<Add library=\"CgGL\" />\n");

		strcat(dirs, "OGL|");
	}
	else if (!stricmp(project, "test"))
	{
		strcpy(outputName, "test");
		projType=0;
	}
	else if (!stricmp(project, "engine"))
	{
		strcpy(outputName, "engine.lp3d");
		projType=3;
	}
	else if (!stricmp(project, "GUI"))
	{
		strcpy(outputName, "GUI.lp3d");
		projType=3;
	}
	else if (!stricmp(project, "phys"))
	{
		strcpy(outputName, "phys.lp3d");
		strcat(libsString, "\t\t\t\t\t<Add library=\"PhysXLoader\" />\n");
		projType=3;
	}
	else
	{
		cout << " ...ERROR: Project " << project << " isn't configured for conversion!" << endl;
	}

	// ******** WRITE HEADER
	fputs("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n", fp);
	fputs("<CodeBlocks_project_file>\n", fp);
	fputs("\t<FileVersion major=\"1\" minor=\"6\" />\n", fp);
	fputs("\t<Project>\n", fp);
	fprintf(fp, "\t\t<Option title=\"%s\" />\n", project);
	fputs("\t\t<Option pch_mode=\"2\" />\n", fp);
	fputs("\t\t<Option compiler=\"gcc\" />\n", fp);
	fputs("\t\t<Build>\n", fp);

	// ******** BEGIN TARGETS
	const char tarNames[2][32] = {"Debug Linux32", "Release Linux32"};
	const char tarFolder[2][32] = {"bin_linux32_debug", "bin_linux32"};
	for (int i=0; i<2; i++) //0-debug, 1-release
	{

	fprintf(fp, "\t\t\t<Target title=\"%s\">\n", tarNames[i]);
	fprintf(fp, "\t\t\t\t<Option output=\"%s/%s\" prefix_auto=\"1\" extension_auto=\"0\" />\n", tarNames[i], outputName);
	fputs("\t\t\t\t<Option working_dir=\"\" />\n", fp);
	fprintf(fp, "\t\t\t\t<Option object_output=\"%s\" />\n", tarNames[i]);
	fprintf(fp, "\t\t\t\t<Option type=\"%d\" />\n", projType);
	fputs("\t\t\t\t<Option compiler=\"gcc\" />\n", fp);
	if (projType!=0) fputs("\t\t\t\t<Option createDefFile=\"1\" />\n", fp);
	if (projType==3) fputs("\t\t\t\t<Option createStaticLib=\"1\" />\n", fp); // dunno if it is necessary to createStaticLib=1 when compilling dynamic .so
	fputs("\t\t\t\t<Compiler>\n", fp);

	if (!stricmp(project, "shared"))
	{
		fputs("\t\t\t\t\t<Add option=\"-DP3DSTATIC_LIBRARY\" />\n", fp);
	}
	else if (!stricmp(project, "renderer"))
	{
		fputs("\t\t\t\t\t<Add option=\"-DGL_GLEXT_PROTOTYPES\" />\n", fp);
		fputs("\t\t\t\t\t<Add option=\"-DGLX_GLXEXT_PROTOTYPES\" />\n", fp);
		fputs("\t\t\t\t\t<Add option=\"-D_OGL\" />\n", fp);
	}

	fputs("\t\t\t\t\t<Add option=\"-W\" />\n", fp);
	fputs("\t\t\t\t\t<Add option=\"-g\" />\n", fp);
	if (i==0)
		fputs("\t\t\t\t\t<Add option=\"-O0\" />\n", fp);
	else
		fputs("\t\t\t\t\t<Add option=\"-O3\" />\n", fp);
	fputs("\t\t\t\t\t<Add option=\"-D_CRT_SECURE_NO_DEPRECATE\" />\n", fp);
	fputs("\t\t\t\t\t<Add option=\"-D_CRT_NONSTDC_NO_DEPRECATE\" />\n", fp);
	fputs("\t\t\t\t\t<Add option=\"-D_USE_32BI_T_TIME_T\" />\n", fp);
	if (i==0)
		fputs("\t\t\t\t\t<Add option=\"-D_DEBUG\" />\n", fp);
	else
		fputs("\t\t\t\t\t<Add option=\"-D_NDEBUG\" />\n", fp);
	fputs("\t\t\t\t\t<Add option=\"-DLINUX\" />\n", fp);
	fputs("\t\t\t\t\t<Add option=\"-D_LINUX\" />\n", fp);
	fputs("\t\t\t\t<Add directory=\"../shared\" />\n", fp);

	fputs("\t\t\t\t</Compiler>\n", fp);

	fputs("\t\t\t\t<ResourceCompiler>\n", fp);
	fputs("\t\t\t\t\t<Add directory=\"../shared\" />\n", fp);
	fputs("\t\t\t\t</ResourceCompiler>\n", fp);

	fputs("\t\t\t\t<Linker>\n", fp);

	if (stricmp(project, "shared")) fputs("\t\t\t\t\t<Add library=\"shared\" />\n", fp);
	fputs(libsString, fp);

	fputs("\t\t\t\t\t<Add directory=\"../../lib\" />\n", fp);
	fputs("\t\t\t\t</Linker>\n", fp);

	fputs("\t\t\t\t<ExtraCommands>\n", fp);

	if (projType==0)
		fprintf(fp, "\t\t\t\t\t<Add after=\"cp &quot;./%s/%s&quot; ../../%s/%s\" />\n", tarNames[i], outputName, tarFolder[i], outputName);
	else if (projType==2)
		fprintf(fp, "\t\t\t\t\t<Add after=\"cp &quot;./%s/%s.a&quot; ../../lib/%s.a\" />\n", tarNames[i], outputName, outputName);
	else if (projType==3)
		fprintf(fp, "\t\t\t\t\t<Add after=\"cp &quot;./%s/%s.so&quot; ../../%s/%s.so\" />\n", tarNames[i], outputName, tarFolder[i], outputName);

	fputs("\t\t\t\t</ExtraCommands>\n", fp);
	fputs("\t\t\t</Target>\n", fp);

	}

	// ******** END TARGETS

	fputs("\t\t</Build>\n", fp);

	// ******** BEGIN FILES
	unsigned int len = strlen(dirs); int last=-1; char tpath[64];
	for (unsigned int i=0; i<len; i++)
	{
		if (dirs[i]=='|')
		{
			dirs[i]=0;
#if defined(WIN32)
			AddFolderWindows(fp, project, &dirs[last+1]);
#elif defined(LINUX)
			AddFolderLinux(fp, project, &dirs[last+1]);
#endif

			last=(int)i;
		}
	}
	//fprintf(fp, "\t\t<Unit filename=\"%s\" />\n", );
	// ******** END FILES

	fputs("\t\t<Extensions>\n", fp);
	fputs("\t\t\t<code_completion />\n", fp);
	fputs("\t\t\t<debugger />\n", fp);
	fputs("\t\t</Extensions>\n", fp);
	fputs("\t</Project>\n", fp);
	fputs("</CodeBlocks_project_file>\n", fp);
	// ********

	fclose(fp);
	fprintf(fpWorkspace, "\t\t<Project filename=\"%s/%s.cbp\" />\n", project, project);
}

int main(int argc, char* argv[])
{
	cout << "VS2CB " << __DATE__ << " " << __TIME__ << " by kex1k. Must be run under sln dir!" << endl << endl;
	cout << "Converting VisualStudio solution to Code::Block project files... " << endl << endl;

	FILE* chk = fopen("Panther3D.sln", "rb");
	if (!chk)
	{
		cout << "Can't find Panther3D.sln! This tool must be run under snd dir!" << endl;
		getch();
		return -1;
	}
	fclose(chk);

	// *********** BUILD WORKSPACE *************************************************
	FILE* fpWS = fopen("Panther3D.workspace", "wb");
	if (!fpWS)
	{
		cout << "Can't open Panther3D.workspace for writing!" << endl;
		getch();
		return -1;
	}

	fputs("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n", fpWS);
	fputs("<CodeBlocks_workspace_file>\n", fpWS);
	fputs("\t<Workspace title=\"Panther3D\">\n", fpWS);

	// *** Add all VS project files
	ConstructProject(fpWS, "shared");
	ConstructProject(fpWS, "test");
	ConstructProject(fpWS, "engine");
	ConstructProject(fpWS, "renderer");
	ConstructProject(fpWS, "GUI");
	ConstructProject(fpWS, "phys");


	fputs("\t</Workspace>\n", fpWS);
	fputs("</CodeBlocks_workspace_file>\n", fpWS);

	fclose(fpWS);



	cout << endl << "Done :)" << endl;
	getch();
	return 0;
}

