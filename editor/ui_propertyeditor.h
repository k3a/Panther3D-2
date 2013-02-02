/********************************************************************************
** Form generated from reading ui file 'propertyeditor.ui'
**
** Created: Wed 26. Aug 20:32:26 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PROPERTYEDITOR_H
#define UI_PROPERTYEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_PropertyEditor
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QTreeWidget *treeWidget;
    QWebView *webView;
    QTextBrowser *textBrowser;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents_2;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QDockWidget *PropertyEditor)
    {
        if (PropertyEditor->objectName().isEmpty())
            PropertyEditor->setObjectName(QString::fromUtf8("PropertyEditor"));
        PropertyEditor->resize(402, 444);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(PropertyEditor->sizePolicy().hasHeightForWidth());
        PropertyEditor->setSizePolicy(sizePolicy);
        PropertyEditor->setSizeIncrement(QSize(1, 1));
        PropertyEditor->setFloating(false);
        PropertyEditor->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        dockWidgetContents->setStyleSheet(QString::fromUtf8("background-color: rgb(128, 128, 128);"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton = new QPushButton(dockWidgetContents);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        treeWidget = new QTreeWidget(dockWidgetContents);
        new QTreeWidgetItem(treeWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        verticalLayout->addWidget(treeWidget);

        webView = new QWebView(dockWidgetContents);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setUrl(QUrl("http://www.google.cz/"));

        verticalLayout->addWidget(webView);

        textBrowser = new QTextBrowser(dockWidgetContents);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        dockWidget = new QDockWidget(groupBox);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        dockWidget->setWidget(dockWidgetContents_2);

        verticalLayout_2->addWidget(dockWidget);

        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout_2->addWidget(pushButton_2);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        PropertyEditor->setWidget(dockWidgetContents);

        retranslateUi(PropertyEditor);

        QMetaObject::connectSlotsByName(PropertyEditor);
    } // setupUi

    void retranslateUi(QDockWidget *PropertyEditor)
    {
        PropertyEditor->setWindowTitle(QApplication::translate("PropertyEditor", "DockWidget", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("PropertyEditor", "PushButton", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("PropertyEditor", "1", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("PropertyEditor", "tset", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget->topLevelItem(1);
        ___qtreewidgetitem2->setText(0, QApplication::translate("PropertyEditor", "dsfs", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem3 = ___qtreewidgetitem2->child(0);
        ___qtreewidgetitem3->setText(0, QApplication::translate("PropertyEditor", "sdfsdf", 0, QApplication::UnicodeUTF8));
        treeWidget->setSortingEnabled(__sortingEnabled);

        textBrowser->setHtml(QApplication::translate("PropertyEditor", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">teset</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("PropertyEditor", "GroupBox", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("PropertyEditor", "PushButton", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(PropertyEditor);
    } // retranslateUi

};

namespace Ui {
    class PropertyEditor: public Ui_PropertyEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTYEDITOR_H
