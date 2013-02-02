//#include "precompiled.h"

#include "mainwindow.h"

using namespace P3D;

#ifdef main
#undef main
#endif
int main(int argc, char *argv[])
{
	QApplication::setStyle("plastique");
    QApplication a(argc, argv);

	qApp->setOrganizationName("Reversity");
	qApp->setApplicationName("P3DCrane");

	// ---- splash ---------
	QLabel *splash = new QLabel("splash", 0, Qt::FramelessWindowHint );
	splash->setFrameStyle( QFrame::NoFrame );
	QPixmap pix( "rc/splash.bmp" );
	splash->setPixmap( pix );
	splash->adjustSize(); // This is needed to adjust the widget size to the image
	QRect r = QApplication::desktop()->geometry();
	splash->move( r.center() - splash->rect().center() );
	splash->setWindowModality(Qt::ApplicationModal);
	splash->show();
	splash->repaint(  );
	QApplication::flush();
	// ----------------------

	// initialize iface system
	I_Initialize( _A2W( QApplication::applicationDirPath().toAscii().data() ) );

    MainWindow w;
    w.show();

	// ----------------------
	delete splash;

    int ret = a.exec(); // RUN QT APPLICATION

	// shutdown iface system
	//I_Shutdown();

	return ret;
}
/*
int main(int argc, wchar* argv[])
{
	char **a_argv = new char*[argc];

	for (int i=0; i<argc; i++)
	{
		a_argv[i] = _W2A(argv[i]);
	}

	int ret = _main(argc, a_argv);

	delete[] a_argv;

	return ret;
}*/