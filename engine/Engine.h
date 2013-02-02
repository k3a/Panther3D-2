/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/

#pragma once
#include "IEngine.h"
#include "Timer.h"
#include "CommandQueue.h"
//
#include "Console.h"
#include "Filesystem.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Config.h"
#include "InternalWindow.h"
#include "InternalInput.h"

#include "MeshLoader.h"
#include "ObjectManager.h"

#include "ScriptEngine.h"

namespace P3D
{
	enum eModuleClass
	{
		MC_UNKNOWN=0,
		MC_RENDERER
	};

	/// Max buffered values for mouse smoothing
	#define MAX_FILTER_VALUES 128

	class CEngine : public IEngine, public IQueueCommandReceiver
	{
	public:
		CEngine();
		~CEngine();
		void LoadModules(const wchar* renderer, const wchar* phys, const wchar* sound, const wchar* filesystem, const wchar* gui);
		bool Initialize(const wchar* settings=_W("game.ini"), bool customWindow=false, sDisplayProperties *dp=NULL);
		bool NextFrame();
		static int GetBuildNum();
		static const wchar* GetBuildString();
		float GetDeltaTime()const{return m_fDeltaTime;};
		bool SendScriptCommands(tArray<Str> &commands);
		/// Request engine shutdown
		void Shutdown(){ /* FIXME: some mutex needed? */ m_bShutdownRequested=true; };
		void EnqueueMessage(eModuleClass toModule, IQueueCommandReceiver* receiver, unsigned int commandQueueIdentifier, void* &inOutPointer, unsigned int memSizeInBytes);
		void OnMessageReceived(unsigned int commandQueueIdentifier, const void* &outPointer);

		static inline CEngine* instance() { return s_pEngine; };

		// --- singleton manipulation
		DECLARE_SUBCLASS(Console);
		DECLARE_SUBCLASS(InputManager);
		DECLARE_SUBCLASS(SceneManager);
		DECLARE_SUBCLASS(Config);
		DECLARE_SUBCLASS(MeshLoader);
		DECLARE_SUBCLASS(ObjectManager);
		DECLARE_SUBCLASS(ScriptEngine);
		DECLARE_SUBCLASS_NOIFACE(InternalWindow);
		DECLARE_SUBCLASS_NOIFACE(InternalInput);
		DECLARE_SUBCLASS_NOIFACE(Filesystem);

		// --- external singletons
		inline IRenderer* mRenderer(){ return s_pRenderer; };
		inline ISoundEngine* mSound() { return s_pSound; };
		inline IPhysEngine* mPhys(){ return s_pPhys; };
		inline IFileSystem* mFilesystem(){ return s_pFS; };
		inline IGUI* mGUI(){ return s_pGUI; };

	private:
		bool m_bModulesLoaded;
		static CEngine *s_pEngine; // instance of engine singleton
		static bool s_bInit; // is engine initialized?
		wchar* m_szSettings; // settings filename

		CTimer m_timer;
		float m_fDeltaTime; // time of last frame in seconds
		float m_fFilterArray[MAX_FILTER_VALUES]; // array for time filtering
		int m_nFrameNumber; // current frame number from engine start
		int m_nFPS;

		bool m_bShutdownRequested;

		// - command queues and asynchronous comm
		CDoubleCommandQueue m_queueRenderer;

		// - external singletons
		static IRenderer *s_pRenderer;
		static ISoundEngine *s_pSound;
		static IPhysEngine *s_pPhys;
		static IFileSystem *s_pFS;
		static IGUI *s_pGUI;
	};

#define CON(type, text, ...) CEngine::cConsole()->AddMsgEx(type, _W(__FUNCTION__), text, ##__VA_ARGS__)

};
