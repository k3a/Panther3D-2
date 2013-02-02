/********************************************************************************
** Form generated from reading ui file 'tabscene.ui'
**
** Created: Wed 26. Aug 20:32:25 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TABSCENE_H
#define UI_TABSCENE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TabScene
{
public:
    QGridLayout *layout;

    void setupUi(QWidget *TabScene)
    {
        if (TabScene->objectName().isEmpty())
            TabScene->setObjectName(QString::fromUtf8("TabScene"));
        TabScene->resize(400, 300);
        layout = new QGridLayout(TabScene);
        layout->setSpacing(1);
        layout->setMargin(1);
        layout->setObjectName(QString::fromUtf8("layout"));

        retranslateUi(TabScene);

        QMetaObject::connectSlotsByName(TabScene);
    } // setupUi

    void retranslateUi(QWidget *TabScene)
    {
        TabScene->setWindowTitle(QApplication::translate("TabScene", "Form", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(TabScene);
    } // retranslateUi

};

namespace Ui {
    class TabScene: public Ui_TabScene {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABSCENE_H
