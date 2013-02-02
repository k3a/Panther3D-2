/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "Engine.h"
#include "Console.h"

#include "Scene.h" // debug
#include "OModel.h" // debug
#include "CommandQueue.h" // debug

namespace P3D
{
	Model *model; // debug

	REGISTER_MODULE_CLASS(CEngine, _W("engine"), _W("internal"));

	CEngine* CEngine::s_pEngine=NULL;
	bool CEngine::s_bInit=false;

	// external
	IRenderer		*CEngine::s_pRenderer=NULL;
	ISoundEngine	*CEngine::s_pSound=NULL;
	IPhysEngine		*CEngine::s_pPhys=NULL;
	IFileSystem		*CEngine::s_pFS=NULL;
	IGUI			*CEngine::s_pGUI=NULL;

	//------------------------------------------
	CEngine::CEngine(){
		assert(s_pEngine==NULL);
		s_pEngine=this;

		m_fDeltaTime=0.001f;
		m_nFrameNumber=0;
		
		m_bModulesLoaded=false;
		m_bShutdownRequested=false;
	}

	//------------------------------------------
	CEngine::~CEngine(){
		cConfig()->Save(m_szSettings);
		delete[] m_szSettings;

		UNLOAD_SUBCLASS(Filesystem);
		UNLOAD_SUBCLASS(InputManager);
		UNLOAD_SUBCLASS(SceneManager);
		UNLOAD_SUBCLASS(Config);
		UNLOAD_SUBCLASS(MeshLoader);
		UNLOAD_SUBCLASS(ObjectManager);
		UNLOAD_SUBCLASS(ScriptEngine);
		UNLOAD_SUBCLASS(InternalWindow);
		UNLOAD_SUBCLASS(InternalInput);

		UNLOAD_SUBCLASS(Console); // should be last
	}

	//------------------------------------------
	int CEngine::GetBuildNum()
	{
		// make build number based on number of days since
		const char* pszDate = __DATE__; // will be in format of Apr 24 2006
		const char *pszMonths[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
		BYTE nMonthDays[12] = { 31,     28,    31,    30,    31,    30,    31,    31,    30,    31,    30,    31 };
		// number of days from 1.1.2008 to 1.1.ThisYear
		UINT nBuild = (atoi(&pszDate[7])-2008)*365;
		for(UINT i=0;i<12;i++)// go through all the months
		{
			if(!strncmp(pszDate, pszMonths[i], 3)) break; // Linux: byl tu strnicmp
			nBuild+=nMonthDays[i];
		}
		nBuild+=atoi(&pszDate[4]); // add number of days in month today in this month
		nBuild-=60; // subtract days from 1.1.2008 to 1.3.2008 = 60

		return nBuild;
	}

	//------------------------------------------
	const wchar* CEngine::GetBuildString()
	{
		static wchar bs[256];
		wchar platform[16];
#if defined(_WIN32)
# if defined(_WIN64)
		wstrcpy(platform, _W("Win64") );
# else
		wstrcpy(platform, _W("Win32") );
# endif
#elif LINUX
		wstrcpy(platform, _W("Linux") );
#elif __MACOSX__
		wstrcpy(platform, _W("Mac") );
#else
		wstrcpy(platform, _W("Other platform") );
#endif

		wsprintf( bs, 255, _W("%s (build %d, %s) <%s>"), _W(P3DNAME), GetBuildNum(), _W(__DATE__), platform );
		return bs;
	}

	void ConsoleMsg(const wchar* msg)
	{
		CEngine::cConsole()->AddMsg(MSG_INFO, _W("%s"), msg);
	}

	// Engine.Modules
	static SVar svRenderer(_W("Renderer"), _W("Engine.Modules"), _W("DX9"), _W("Renderer module implementation to load."));
	static SVar svSound(_W("Sound"), _W("Engine.Modules"), _W("OAL"), _W("Sound engine module implementation to load."));
	static SVar svPhysics(_W("Physics"), _W("Engine.Modules"), _W("PhysX"), _W("Physics engine module implementation to load."));
	static SVar svFileSystem(_W("FileSystem"), _W("Engine.Modules"), _W("StdIO"), _W("Filesystem module implementation to load."));
	static SVar svGUI(_W("GUI"), _W("Engine.Modules"), _W("Internal"), _W("GUI module implementation to load."));

	// Engine.Client
	static SVar svResolutionX(_W("ResolutionX"), _W("Engine.Client"), 800, _W("Startup screen/window width."));
	static SVar svResolutionY(_W("ResolutionY"), _W("Engine.Client"), 600, _W("Startup screen/window height."));
	static SVar svFullscreen(_W("Fullscreen"), _W("Engine.Client"), false, _W("Start fullscreen?")); // TODO: FINAL: change to true?

	double milisec() /* From 1970-01-01T00:00:00 */
	{
		SYSTEMTIME    st;    /* unsigned wMilliseconds; */
		union _t
		{
			FILETIME    ft;
			__int64     tlong;
		} t;
		GetSystemTime(&st);
		SystemTimeToFileTime(&st,&t.ft);
		return (double)((t.tlong-116444736000000000i64)/10000);
	}

	//------------------------------------------
	bool CEngine::Initialize(const wchar* settings/*="game.ini"*/, bool customWindow/*=false*/, sDisplayProperties *dp/*=NULL*/){
		assertd(s_bInit==false, "DOUBLE ENGINE INITIALIZATION!");

		s_bInit = true; //predict :-P

		// initialize random generator
		srand( (unsigned int)milisec() );

		cInternalWindow()->BeginSplash();

		// memory leaks detection
#if defined(_DEBUG) && defined(_MSC_VER) && _MSC_VER >= 800
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		//_CrtSetBreakAlloc(23296);
#endif

		// ---------------------------------------------------
		// QUEUE TESTS
		CON(MSG_INFO, _W("------- Command Queue Test -------"));
		CCommandQueue q;
		DWORD tim;
		for (int t=0; t<10; t++)
		{
			tim = GetTickCount();
			// FILL IN QUEUE
			for (int i=0; i<20000; i++)
			{
					sE2RCanvasDesc *cd;
					q.Enqueue(NULL, CQI_ENGINE2RENDERER_CANVAS_CREATE, (void*&)cd, sizeof(sE2RCanvasDesc));
					cd->handle = 0;
			}
			CON(MSG_INFO, _W("Iteration #%d: Enqueue %d ms"), t, GetTickCount()-tim );

			// DEQUEUE
			unsigned int cqi; const void* optr; IQueueCommandReceiver *recv;
			tim = GetTickCount();
			while (q.Dequeue(cqi, optr, recv))
			{
				switch(cqi)
				{
				case CQI_ENGINE2RENDERER_CANVAS_CREATE:
					{
						sE2RCanvasDesc *cd = (sE2RCanvasDesc *)optr;
					}
					break;
				}
			}
			CON(MSG_INFO, _W("Iteration #%d: Dequeue %d ms"), t, GetTickCount()-tim );
		}
		CON(MSG_INFO, _W("----------------------------------"));
		// ---------------------------------------------------


		// --- WRITE INFO ABOUT LIBRARIES AND MODULES
		I_DebugPrint( ConsoleMsg );

		CON(MSG_INFO, _W("= %s %s build %d initialization ="), _W(P3DNAME), sizeof(void*)==8?_W("x64"):_W("x86"), GetBuildNum());
		if (sizeof(wchar)>1) 
			CON(MSG_INFO, _W("Unicode support \x263A"));
		else
			CON(MSG_INFO, _W("Multibyte character set"));

		// --- LOAD SETTINGS
		m_szSettings = new wchar[wstrlen(settings)+1];
		wstrcpy(m_szSettings, settings);

		cConfig()->Load(m_szSettings);
		//cConfig()->Save(m_szSettings);

		// --- INITIALIZE SCRIPT SYSTEM
		CON(MSG_INFO, _W("Script system initialization"));
		cScriptEngine()->Initialize();

		// --- LOAD KEY BINDINGS
		cInputManager()->Assign(_W("GUI_CURSOR"), WE_MOUSE_MOTION, 0);
		cInputManager()->Assign(_W("GUI_SELECT"), WE_MOUSE_DOWN, 0);

		cInputManager()->Save(_W("keys.ini")); // TODO: FIXME: What about load? :D

		// --- CREATE MAIN WINDOW
		Vec2i mainWindowSize(800,600);
		bool initFullscreen = false;
		if (customWindow)
		{
			// use primary screen resolution for frame buffer
			// TODO: multiplatform
#ifdef _WIN32
			mainWindowSize.x = GetSystemMetrics(SM_CXSCREEN);
			mainWindowSize.y = GetSystemMetrics(SM_CYSCREEN);
#endif
		}
		else
		{
			if (dp) 
			{
				// use user-defined resolution
				initFullscreen = dp->Fullscreen;
				mainWindowSize.x = dp->HorRes;
				mainWindowSize.y = dp->VertRes;
			}
			else
			{
				// use settings
				initFullscreen = svFullscreen.GetBool();
				mainWindowSize.x = svResolutionX.GetInt();
				mainWindowSize.y = svResolutionY.GetInt();
			}
		}
		cInternalWindow()->Create(mainWindowSize);

		//Init the internal input system
		cInternalInput()->Init( cInternalWindow()->GetHandle() );
		//Init the Filesystem
		cFilesystem()->Init();

		
		// --- LOAD SELECTED MODULES
		s_pRenderer = (IRenderer*)I_GetModule(_W("renderer"), svRenderer.GetString());
		if (!s_pRenderer) CON(MSG_ERR_FATAL, _W("Cannot load renderer module. It is a core module, cannot continue!"));
		s_pSound = (ISoundEngine*)I_GetModule(_W("sound"), svSound.GetString());
		if (!s_pSound) CON(MSG_ERR_FATAL, _W("Cannot load sound module. It is a core module, cannot continue!"));
		s_pPhys = (IPhysEngine*)I_GetModule(_W("physics"), svPhysics.GetString());
		if (!s_pPhys) CON(MSG_ERR_FATAL, _W("Cannot load phys module. It is a core module, cannot continue!"));
		s_pFS = (IFileSystem*)I_GetModule(_W("filesystem"), svFileSystem.GetString());
		if (!s_pFS) CON(MSG_ERR_FATAL, _W("Cannot load filesystem module. It is a core module, cannot continue!"));
		s_pGUI = (IGUI*)I_GetModule(_W("gui"), svGUI.GetString());
		if (!s_pGUI) CON(MSG_ERR_FATAL, _W("Cannot load GUI module. It is a core module, cannot continue!"));
		m_bModulesLoaded=true;

		// ==== INITIALIZE MODULES ====
		s_pRenderer->Initialize(this, &m_queueRenderer);

		bool ret = s_pRenderer->iGraphicsDevice()->Initialize(mainWindowSize.x, mainWindowSize.y, initFullscreen, cInternalWindow()->GetHandle());
		if (!ret)
		{
			CON(MSG_ERR_FATAL, _W("Failed to initialize graphics device!"));
		}

		// ===== SERIALIZATION DEBUG AND TEST =======
		Scene scene;
		//scene.Save(s_pFS, _W("scenes/test.robject"), iConsole());
		
		//CModel model;

		/*model = (Model*)cObjectManager()->CreateObject( _W("Model") );
		MeshData md;
          MeshSubset ms;
//           Material m;
//		  ms.Material = m;
		  ms.NumTriangles = 1;
		  ms.StartIndex = 0;
		  //ms.StartVertex = 0;
		 md.Subsets.AddEx(ms);
		 md.Indices.AddEx(0);
		 md.Indices.AddEx(1);
		 md.Indices.AddEx(2);
		 md.NumIndices = 3;
		  MeshVertexData mvd;
		  mvd.Usage = _W("P3DVU_POSITION");
		  mvd.DataSize = 3;
		  mvd.Float3.AddEx(Vec3Param(0.0f, 0.0f, 0.0f));
		  mvd.Float3.AddEx(Vec3Param(0.0f, 1.0f, 0.0f));
		  mvd.Float3.AddEx(Vec3Param(1.0f, 1.0f , 0.0f));
		 md.DataStreams.AddEx(mvd);
		 md.NumVerts = 3;
		model->LODs.AddEx(md);

		model->Save(s_pFS, _W("triangle.robject"), iConsole());
		model->PreCache();*/
		// ===========================================

		bool initOK=true;
		initOK &= s_pSound->Initialize(this);
		initOK &= s_pPhys->Initialize(this);
		initOK &= s_pFS->Initialize(this);
		initOK &= s_pGUI->Initialize(this);

		if (!initOK)
			CON(MSG_ERR_FATAL, _W("Failed to initialize some core module(s)! Cannot continue. For more details see console.htm."));

		cSceneManager()->Create();

		cConsole()->InitializeGUI(); // at this time, coz it uses GUI module and it must be initialized ;)

		// FIXME: register engine to script, temporary here
		/*using namespace luabind;
		lua_State* L = cScriptEngine()->GetLVM();
		module(L)
		[
			class_<CEngine>("Engine")
			.scope
			[
				def("GetBuildNum", &CEngine::GetBuildNum )
			]
		];*/

		// load bootstrap script
		cScriptEngine()->LoadScript(_W("scripts/bootstrap.rscript"));

		// DEBUG TESTING OF QUEUE TO RENDERER
		sE2RCanvasDesc* canvas;
		EnqueueMessage( MC_RENDERER, this, CQI_ENGINE2RENDERER_CANVAS_CREATE, (void*&)canvas, sizeof(sE2RCanvasDesc) );
		canvas->handle = cInternalWindow()->GetHandle();
		canvas->size = Vec2i(800,600);
		canvas->windowed = !initFullscreen;

		//Sleep(10000);
		cInternalWindow()->EndSplash();
		cInternalWindow()->SetVisible(true);

		
		//Filesystem test
		/*UINT oSize;
		void *lData=0;
		if(cFilesystem()->Load(_W("scripts.bootstrap"), &lData, oSize)==P3D_FILERESULT_OK)
			cFilesystem()->FreeLoadedData(lData);
		const char* testStr = "this is a test string";
		cFilesystem()->Save(_W("scripts.fstest"), testStr, sizeof(char)*strlen(testStr));*/

		return true;
	}

	//------------------------------------------
	bool CEngine::NextFrame(){
		// -------- compute and filter frame time ------------------------------------------------------------
		m_timer.Loop();

		// move all time buffer values to the right and at set new value at the beginning
		for(int i=MAX_FILTER_VALUES-1; i>0; i--) m_fFilterArray[i]=m_fFilterArray[i-1];
		m_fFilterArray[0] = m_timer.GetTime_s();

		// filter values only when buffer is filled
		if (m_nFrameNumber<MAX_FILTER_VALUES) 
		{
			m_fDeltaTime = m_fFilterArray[0]; // immediate, non filtered
			m_nFPS = (int)(1.0f/m_fDeltaTime);
			m_nFrameNumber++;
		}
		else
		{
			m_nFPS = (int)(1.0f/m_fDeltaTime);
			// filter values based on FPS
			int numVal = m_nFPS;
			float fSum=0;

			if (numVal<2) 
				numVal=2;
			else if(numVal>MAX_FILTER_VALUES-1)
				numVal = MAX_FILTER_VALUES-1;
			
			for (int i=0; i<numVal; i++) fSum+=m_fFilterArray[i]/(i+1);

			m_fDeltaTime = fSum/log((float)numVal*1.75f);
		}
		// -----------------------------------------------------------------------------------------

		// ---- update script engine
		cScriptEngine()->Tick();

		// ---- begin frame ------------------------------------------------------------------------

		// {** this is main engine frame time... here should be all script work done, messages added to queues **}

		// -- Swap all command queues
		m_queueRenderer.Swap();

		// -- Execute rendering in separate thread; TODO: separate thread
		mRenderer()->NextFrame(); // ...but in separate thread
		// todo: execute other threads

		// -- Wait for all threads (also maybe proccess queues from finished threads?)

		// -- Process renderer back command queue
		unsigned int cqi=0; const void* ptr=NULL; IQueueCommandReceiver* recv=NULL;
		while( m_queueRenderer.EngineQueue()->Dequeue(cqi, ptr, recv) )
		{
			if ( recv ) recv->OnMessageReceived( cqi, ptr );
		}

		// old code commented out, just for reference, remove after implemented in a new way
		/*
		unsigned int numEvents=0;
		sWEvent* we=NULL;

		if (wndTarget)
		{
			sViewport vp;
			vp.x = vp.y = 0;
			vp.width = wndTarget->GetSize().x;
			vp.height = wndTarget->GetSize().y;
			//s_pRenderer->iGraphicsDevice()->SetViewport(vp);

			// get events from custom window
			we = wndTarget->GetEvents(numEvents);
		}
		else
		{
			// get events from internal window
			we = s_pRenderer->iGraphicsWindow()->GetEvents(numEvents);
		}

		// send events to InputManager
		cInputManager()->ProcessNewEvents(we, numEvents);

		// process all returned events
		for (unsigned int i=0;i<numEvents;i++)
		{
			if (we[i].type==WE_RESIZE)
			{
				break;
			} else if (we[i].type==WE_KEY_DOWN)
			{*/
				/*if (we[i].button==KEY_BACKQUOTE) 
				{
					bool bVisible = cConsole()->ToggleVisible();
					if (wndTarget)
						wndTarget->SetMenuMode(bVisible);
					else
						s_pRenderer->iGraphicsWindow()->SetMenuMode(bVisible);
				}*/
			/*} else if (we[i].type==WE_QUIT)
			{
				ret = false; // inform about shutdown request
				CON(MSG_INFO, _W("Shutdown request."));
			}
		}*/

		//TODO: Run this in a separate thread
		cInternalWindow()->Update();
		cInternalInput()->Update();

		tArray<sInputEvent> ievents;
		cInternalInput()->SwapEvents(ievents);
		cInputManager()->ProcessNewEvents(ievents);


		//return false; // DEBUG:!! Temporary do only one frame
		return !m_bShutdownRequested;
	}

	//------------------------------------------
	bool CEngine::SendScriptCommands(tArray<Str> &commands)
	{
		bool allOk=true;
		for(unsigned int i=0; i<commands.Size(); i++)
		{
			if( !cScriptEngine()->LoadScriptString( _W2A(commands[i].Get()) ) )
				allOk = false;
		}
		return allOk;
	}

	//------------------------------------------
	void CEngine::EnqueueMessage(eModuleClass toModule, IQueueCommandReceiver* receiver, unsigned int commandQueueIdentifier, void* &inOutPointer, unsigned int memSizeInBytes)
	{
		switch(toModule)
		{
		case MC_RENDERER:
			m_queueRenderer.EngineQueue()->Enqueue(receiver, commandQueueIdentifier, inOutPointer, memSizeInBytes);
			break;
		}
	}

	//------------------------------------------
	void CEngine::OnMessageReceived(unsigned int commandQueueIdentifier, const void* &outPointer)
	{
		int debug=0;
	}


	//------------------------------------------
	//void CEngine::EndScene(){

			//debug
		/*Matrix mat;
		mat.SetIdentityMatrix();
		GeometryChunk gc;
		gc.Data = &model->LODs[0];
		gc.Subset = &model->LODs[0].Subsets[0];
		gc.WorldMatrix = &mat;

		tArray<GeometryChunk> chunks;
		chunks.AddEx(gc);
		s_pRenderer->iGraphicsRenderer()->SendChunks(chunks);*/

			/*s_pGUI->Update(m_fDeltaTime);
			s_pGUI->Render(m_fDeltaTime);*/

			// draw text on screen
			//cConsole()->Draw();
			//mRenderer()->iTextRenderer()->Render();

			//s_pRenderer->iGraphicsDevice()->EndScene();
	//}

};
