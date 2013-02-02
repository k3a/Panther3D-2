/********************************************************************************
** Form generated from reading ui file 'aboutbox.ui'
**
** Created: Wed 26. Aug 20:32:27 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ABOUTBOX_H
#define UI_ABOUTBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutBox
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AboutBox)
    {
        if (AboutBox->objectName().isEmpty())
            AboutBox->setObjectName(QString::fromUtf8("AboutBox"));
        AboutBox->resize(401, 256);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(AboutBox->sizePolicy().hasHeightForWidth());
        AboutBox->setSizePolicy(sizePolicy);
        AboutBox->setSizeGripEnabled(false);
        verticalLayout = new QVBoxLayout(AboutBox);
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(AboutBox);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        verticalLayout->addWidget(label);

        label_2 = new QLabel(AboutBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(false);
        font1.setWeight(50);
        label_2->setFont(font1);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(AboutBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(AboutBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(AboutBox);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(AboutBox);
        QObject::connect(buttonBox, SIGNAL(accepted()), AboutBox, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AboutBox, SLOT(reject()));

        QMetaObject::connectSlotsByName(AboutBox);
    } // setupUi

    void retranslateUi(QDialog *AboutBox)
    {
        AboutBox->setWindowTitle(QApplication::translate("AboutBox", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AboutBox", "P3DCrane[2]", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AboutBox", "Scene designer for Panther3D Engine", 0, QApplication::UnicodeUTF8));
        label_3->setText(QString());
        label_4->setText(QApplication::translate("AboutBox", "For more information visit www.reversity.org", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(AboutBox);
    } // retranslateUi

};

namespace Ui {
    class AboutBox: public Ui_AboutBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTBOX_H
