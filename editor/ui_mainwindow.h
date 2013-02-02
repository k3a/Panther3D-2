/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Wed 26. Aug 20:32:27 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *aToolSelect;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabs;
    QWidget *test;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8("app.ico")), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(140, 140, 140);\n"
"color: rgb(255, 255, 255);\n"
"alternate-background-color: rgb(97, 97, 97);\n"
"border-color: rgb(40, 40, 40);\n"
"gridline-color:  rgb(170, 170, 170);\n"
"selection-background-color: rgb(255, 83, 3);\n"
"selection-color: rgb(0, 0, 0);"));
        MainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        aToolSelect = new QAction(MainWindow);
        aToolSelect->setObjectName(QString::fromUtf8("aToolSelect"));
        aToolSelect->setCheckable(true);
        QIcon icon1;
        icon1.addPixmap(QPixmap(QString::fromUtf8(":/rc/tools/tool_select.png")), QIcon::Normal, QIcon::Off);
        aToolSelect->setIcon(icon1);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(5, 1, 0, 0);
        tabs = new QTabWidget(centralWidget);
        tabs->setObjectName(QString::fromUtf8("tabs"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy);
        tabs->setElideMode(Qt::ElideNone);
        tabs->setDocumentMode(true);
        tabs->setTabsClosable(true);
        tabs->setMovable(true);
        test = new QWidget();
        test->setObjectName(QString::fromUtf8("test"));
        tabs->addTab(test, QString());

        gridLayout->addWidget(tabs, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar);

        toolBar->addAction(aToolSelect);

        retranslateUi(MainWindow);

        tabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "P3DCrane 2", 0, QApplication::UnicodeUTF8));
        aToolSelect->setText(QApplication::translate("MainWindow", "Select", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(test), QApplication::translate("MainWindow", "Scene", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
