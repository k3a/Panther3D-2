
#include "engine.h"

IEngine* Engine::s_pEngine=NULL;

Engine::Engine()
{
	m_pRenderer = NULL;
	m_pGraphicsDevice = NULL;

	m_quit = true;
}

bool Engine::InitEngine()
{
	// ************* INITIALIZE ENGINE
 	s_pEngine = (IEngine*)I_GetModule(_W("engine"), NULL);
 	assertd(s_pEngine!=0, "Engine could not be loaded! Missing engine module?");
 
 	sDisplayProperties dp;
 	QRect r = QApplication::desktop()->geometry();
 	dp.HorRes = r.width(); dp.VertRes = r.height();
 	dp.Fullscreen = false;
 	if(!s_pEngine->Initialize(_W("game.ini"), true, &dp))
 	{
 		QMessageBox mb(QMessageBox::Critical, "Fatal error", "Failed to initialize engine, quitting...");
 		mb.show();
 		QApplication::quit();
 	}

	m_pRenderer = s_pEngine->mRenderer();
	m_pGraphicsDevice = m_pRenderer->iGraphicsDevice();

	m_quit = false;

	return true;
}

void Engine::ShutdownEngine()
{
	m_quit = true;
	this->wait();

	I_Shutdown();
}

void Engine::StartLoopingThread()
{
	this->run();
}

void Engine::run()
{
	while(!m_quit)
	{
		ProcessFrame();
	}
}

void Engine::ProcessFrame()
{
	// update engine and render all RenderViews
	s_pEngine->NextFrame();
	
	// now engine and all other threads are synced, 
	// so we can send script commands from queue
	s_pEngine->SendScriptCommands();
}