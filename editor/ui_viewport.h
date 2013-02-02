/********************************************************************************
** Form generated from reading ui file 'viewport.ui'
**
** Created: Wed 26. Aug 20:32:24 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VIEWPORT_H
#define UI_VIEWPORT_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Viewport
{
public:
    QAction *aCameraPerspective;
    QAction *aCameraOrtho;
    QAction *aCameraLeft;
    QAction *aCameraRight;
    QAction *aCameraFront;
    QAction *aCameraBack;
    QAction *aCameraTop;
    QAction *aCameraBottom;
    QAction *aView1;
    QAction *aView2;
    QAction *aView3;
    QAction *aView4;
    QAction *aViewAll;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QWidget *viewport;

    void setupUi(QWidget *Viewport)
    {
        if (Viewport->objectName().isEmpty())
            Viewport->setObjectName(QString::fromUtf8("Viewport"));
        Viewport->resize(400, 300);
        Viewport->setStyleSheet(QString::fromUtf8("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255)"));
        aCameraPerspective = new QAction(Viewport);
        aCameraPerspective->setObjectName(QString::fromUtf8("aCameraPerspective"));
        aCameraPerspective->setCheckable(true);
        aCameraOrtho = new QAction(Viewport);
        aCameraOrtho->setObjectName(QString::fromUtf8("aCameraOrtho"));
        aCameraOrtho->setCheckable(true);
        aCameraLeft = new QAction(Viewport);
        aCameraLeft->setObjectName(QString::fromUtf8("aCameraLeft"));
        aCameraLeft->setCheckable(true);
        aCameraRight = new QAction(Viewport);
        aCameraRight->setObjectName(QString::fromUtf8("aCameraRight"));
        aCameraRight->setCheckable(true);
        aCameraFront = new QAction(Viewport);
        aCameraFront->setObjectName(QString::fromUtf8("aCameraFront"));
        aCameraFront->setCheckable(true);
        aCameraBack = new QAction(Viewport);
        aCameraBack->setObjectName(QString::fromUtf8("aCameraBack"));
        aCameraBack->setCheckable(true);
        aCameraTop = new QAction(Viewport);
        aCameraTop->setObjectName(QString::fromUtf8("aCameraTop"));
        aCameraTop->setCheckable(true);
        aCameraBottom = new QAction(Viewport);
        aCameraBottom->setObjectName(QString::fromUtf8("aCameraBottom"));
        aCameraBottom->setCheckable(true);
        aView1 = new QAction(Viewport);
        aView1->setObjectName(QString::fromUtf8("aView1"));
        aView1->setCheckable(true);
        aView2 = new QAction(Viewport);
        aView2->setObjectName(QString::fromUtf8("aView2"));
        aView2->setCheckable(true);
        aView3 = new QAction(Viewport);
        aView3->setObjectName(QString::fromUtf8("aView3"));
        aView4 = new QAction(Viewport);
        aView4->setObjectName(QString::fromUtf8("aView4"));
        aView4->setCheckable(true);
        aViewAll = new QAction(Viewport);
        aViewAll->setObjectName(QString::fromUtf8("aViewAll"));
        aViewAll->setCheckable(true);
        verticalLayout = new QVBoxLayout(Viewport);
        verticalLayout->setSpacing(1);
        verticalLayout->setMargin(1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout->setContentsMargins(4, -1, 4, -1);
        label = new QLabel(Viewport);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMaximumSize(QSize(16777215, 12));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(179, 255, 184);"));
        label->setTextFormat(Qt::AutoText);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        viewport = new QWidget(Viewport);
        viewport->setObjectName(QString::fromUtf8("viewport"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(viewport->sizePolicy().hasHeightForWidth());
        viewport->setSizePolicy(sizePolicy1);
        viewport->setStyleSheet(QString::fromUtf8("background-color: rgb(97, 97, 97);"));
        viewport->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));

        verticalLayout->addWidget(viewport);


        retranslateUi(Viewport);

        QMetaObject::connectSlotsByName(Viewport);
    } // setupUi

    void retranslateUi(QWidget *Viewport)
    {
        Viewport->setWindowTitle(QApplication::translate("Viewport", "Form", 0, QApplication::UnicodeUTF8));
        aCameraPerspective->setText(QApplication::translate("Viewport", "Perspective", 0, QApplication::UnicodeUTF8));
        aCameraOrtho->setText(QApplication::translate("Viewport", "Ortho", 0, QApplication::UnicodeUTF8));
        aCameraLeft->setText(QApplication::translate("Viewport", "Left", 0, QApplication::UnicodeUTF8));
        aCameraRight->setText(QApplication::translate("Viewport", "Right", 0, QApplication::UnicodeUTF8));
        aCameraFront->setText(QApplication::translate("Viewport", "Front", 0, QApplication::UnicodeUTF8));
        aCameraBack->setText(QApplication::translate("Viewport", "Back", 0, QApplication::UnicodeUTF8));
        aCameraTop->setText(QApplication::translate("Viewport", "Top", 0, QApplication::UnicodeUTF8));
        aCameraBottom->setText(QApplication::translate("Viewport", "Bottom", 0, QApplication::UnicodeUTF8));
        aView1->setText(QApplication::translate("Viewport", "View 1", 0, QApplication::UnicodeUTF8));
        aView2->setText(QApplication::translate("Viewport", "View 2", 0, QApplication::UnicodeUTF8));
        aView3->setText(QApplication::translate("Viewport", "View 3", 0, QApplication::UnicodeUTF8));
        aView4->setText(QApplication::translate("Viewport", "View 4", 0, QApplication::UnicodeUTF8));
        aViewAll->setText(QApplication::translate("Viewport", "All Views", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Viewport", "Camera Label", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Viewport);
    } // retranslateUi

};

namespace Ui {
    class Viewport: public Ui_Viewport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWPORT_H
