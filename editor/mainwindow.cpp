#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aboutbox.h"
#include "ui_aboutbox.h"

#include "tabscene.h"
#include "tabassetbrowser.h"

#include "EdPanel.h" // debug

Engine* MainWindow::s_pEngine=NULL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	MainWindow::setDockNestingEnabled(true);
	MainWindow::setDockOptions(QMainWindow::AllowTabbedDocks);

	// add properties editor
	propEditor = new PropertyEditor(this);
	MainWindow::addDockWidget(Qt::RightDockWidgetArea, propEditor );

	EdPanel *p = new EdPanel(this);
	MainWindow::addDockWidget(Qt::RightDockWidgetArea, p );

	MainWindow::tabifyDockWidget (propEditor, p);

	ui->tabs->removeTab(0); // dunno why designer can't create tab control without tabs :D
	ui->tabs->addTab(new TabScene(), tr("Scene"));
	ui->tabs->addTab(new TabAssetBrowser(), tr("AssetBrowser"));
/*
	ui->mdiArea->addSubWindow(new AboutBox());
	ui->mdiArea->addSubWindow(new AboutBox());*/

    //QMetaObject::connectSlotsByName(this);

	// ************* INITIALIZE ENGINE
// 	s_pEngine = (IEngine*)I_GetModule(_W("engine"), NULL);
// 	assertd(s_pEngine!=0, "Engine could not be loaded! Missing engine module?");
// 
// 	sDisplayProperties dp;
// 	QRect r = QApplication::desktop()->geometry();
// 	dp.HorRes = r.width(); dp.VertRes = r.height();
// 	dp.Fullscreen = false;
// 	if(!s_pEngine->Initialize(_W("game.ini"), true, &dp))
// 	{
// 		QMessageBox mb(QMessageBox::Critical, "Fatal error", "Failed to initialize engine, quiting...");
// 		mb.show();
// 		QApplication::quit();
// 	}

	s_pEngine = new Engine;
	s_pEngine->InitEngine();
	//s_pEngine->StartLoopingThread();
}

MainWindow::~MainWindow()
{
    s_pEngine->ShutdownEngine();
	delete s_pEngine;

	delete ui;
}

void MainWindow::on_menu_about()
{
	(new AboutBox(this))->show();
}

void MainWindow::on_testButton_clicked()
{
    QMessageBox::information(this, tr("No Image Name"),
             tr("Please supply a name for the image."), QMessageBox::Cancel);

}

void MainWindow::on_action_Exit_activated()
{
        QMessageBox::information(this, tr("No Image Name"),
             tr("Please supply a name for the image."), QMessageBox::Cancel);
}
