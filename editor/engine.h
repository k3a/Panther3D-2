#ifndef ENGINE_H
#define ENGINE_H

#include <QThread>
#include "IEngine.h"

using namespace P3D;

class Engine : public QThread
{
	Q_OBJECT

public:
	Engine();
	bool InitEngine();
	void ShutdownEngine();
	void StartLoopingThread();
	//static P3D::IEngine* mEngine(){ return s_pEngine; }; //TODO: A simple mutex should do the trick.

private:
	void run(); //starts the thread
	void ProcessFrame();
	// modules
	static IEngine* s_pEngine;

	IRenderer* m_pRenderer;
	IGraphicsDevice* m_pGraphicsDevice;

	bool m_quit;
};


#endif //ENGINE_H
