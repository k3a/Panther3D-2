/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/

#pragma once
#include "interface.h"
#include "IWindow.h"
#include "IInputManager.h"
#include "ISceneManager.h"
#include "IConsole.h"
#include "IConfig.h"
#include "IObjectManager.h"
#include "IScriptEngine.h"

#include "IMeshLoader.h"

//-- EXTERNAL
#include "IRenderer.h"
#include "ISound.h"
#include "IPhysEngine.h"
#include "IFilesystem.h"
#include "IGUI.h"

/// Panther3D Namespace
namespace P3D
{

	/*! \mainpage Panther3D 2.0 documentation
	*
	* \section intro_sec Introduction
	*
	* Welcome to the Panther3D Engine API documentation. Here you'll find any information you'll need to develop applications with the Panther3D Engine.
	*
	* \section install_sec Installation
	*
	* \subsection step1 Step 1: Basic application
	*  
	* etc...
	*/


	
	///Engine interface. Provides important initializations and interface handling.
	class IEngine : public IBaseInterface
	{
	public:
		///Initialize engine. Must be called before any other interface method!
		/** You can specify custom main window for rendering and display properties. These parameters OVERRIDE parameters defined in settings.
		\param settings Setting file from which to set whole engine and modules ConVars
		\param customWindow If set to true, no default renderer window will be used and you must render to custom window calling BeginScene() with correct parameter!
		\param dp Optional display properties to override*/
		virtual bool Initialize(const wchar* settings=_W("game.ini"), bool customWindow=false, sDisplayProperties *dp=NULL)=0;
		/// Begin new frame
		/**
		\return True will signalize success and begin of a new frame. False signalize an error or request to shutdown application!*/
		virtual bool NextFrame()=0;
		/// Get engine build number
//		virtual int GetBuildNum()const=0;
		/// Get previous frame duration in seconds.
		/* This delta time is filtered and can be used in all modules to control animation. */
		virtual float GetDeltaTime()const=0;

		/// Send script command
		/** \return Returns true if all commands executed successfuly (not often) */
		virtual bool SendScriptCommands(tArray<Str> &commands)=0;

		// --- singleton manipulation
		///Get global console singleton.
		DECLARE_ISUBCLASS(Console);
		DECLARE_ISUBCLASS(InputManager);
		DECLARE_ISUBCLASS(SceneManager);
		DECLARE_ISUBCLASS(Config);
		DECLARE_ISUBCLASS(MeshLoader);
		DECLARE_ISUBCLASS(ObjectManager);
		DECLARE_ISUBCLASS(ScriptEngine);

		// --- external modules
		virtual IRenderer* mRenderer()=0;
		virtual ISoundEngine* mSound()=0;
		virtual IPhysEngine* mPhys()=0;
		virtual IFileSystem* mFilesystem()=0;
		virtual IGUI* mGUI()=0;
	};

};
