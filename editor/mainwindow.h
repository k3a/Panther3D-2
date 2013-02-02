#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "propertyeditor.h"
#include "engine.h"

namespace Ui
{
    class MainWindow;
}

using namespace P3D;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	PropertyEditor* GetPropertyEditor()const{ return propEditor; }

	//static IEngine* mEngine(){ return s_pEngine; };

private slots:
    void on_menu_about();
    void on_testButton_clicked();
    void on_action_Exit_activated();

private:
    Ui::MainWindow *ui;

	static Engine* s_pEngine;

	// modules
	//static IEngine* s_pEngine;

	//
	PropertyEditor* propEditor;
};

#endif // MAINWINDOW_H
