/********************************************************************************
** Form generated from reading ui file 'tabassetbrowser.ui'
**
** Created: Wed 26. Aug 20:32:26 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TABASSETBROWSER_H
#define UI_TABASSETBROWSER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TabAssetBrowser
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tabBrowse;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *grpTypes;
    QVBoxLayout *verticalLayout_5;
    QLineEdit *txtSearchTypes;
    QListWidget *listTypes;
    QGroupBox *grpCategories;
    QVBoxLayout *verticalLayout_4;
    QLineEdit *txtSearchCategories;
    QTreeWidget *treeCategories;
    QGroupBox *grpBrowser;
    QVBoxLayout *verticalLayout_6;
    QLineEdit *txtSearchAssets;
    QWidget *widBrowser;
    QGroupBox *grpInfo;
    QVBoxLayout *verticalLayout_7;
    QWidget *tabImport;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QLabel *lblPath;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widPreview;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *txtName;
    QLabel *label_2;
    QComboBox *cmbCategory;
    QLabel *label_3;
    QLineEdit *txtTags;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnSave;

    void setupUi(QWidget *TabAssetBrowser)
    {
        if (TabAssetBrowser->objectName().isEmpty())
            TabAssetBrowser->setObjectName(QString::fromUtf8("TabAssetBrowser"));
        TabAssetBrowser->resize(586, 405);
        verticalLayout = new QVBoxLayout(TabAssetBrowser);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        verticalLayout->addLayout(horizontalLayout);

        tabWidget = new QTabWidget(TabAssetBrowser);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabBrowse = new QWidget();
        tabBrowse->setObjectName(QString::fromUtf8("tabBrowse"));
        horizontalLayout_6 = new QHBoxLayout(tabBrowse);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        grpTypes = new QGroupBox(tabBrowse);
        grpTypes->setObjectName(QString::fromUtf8("grpTypes"));
        grpTypes->setMaximumSize(QSize(300, 16777215));
        verticalLayout_5 = new QVBoxLayout(grpTypes);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        txtSearchTypes = new QLineEdit(grpTypes);
        txtSearchTypes->setObjectName(QString::fromUtf8("txtSearchTypes"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(txtSearchTypes->sizePolicy().hasHeightForWidth());
        txtSearchTypes->setSizePolicy(sizePolicy);

        verticalLayout_5->addWidget(txtSearchTypes);

        listTypes = new QListWidget(grpTypes);
        listTypes->setObjectName(QString::fromUtf8("listTypes"));
        listTypes->setResizeMode(QListView::Fixed);

        verticalLayout_5->addWidget(listTypes);


        verticalLayout_3->addWidget(grpTypes);

        grpCategories = new QGroupBox(tabBrowse);
        grpCategories->setObjectName(QString::fromUtf8("grpCategories"));
        grpCategories->setMaximumSize(QSize(300, 16777215));
        verticalLayout_4 = new QVBoxLayout(grpCategories);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        txtSearchCategories = new QLineEdit(grpCategories);
        txtSearchCategories->setObjectName(QString::fromUtf8("txtSearchCategories"));

        verticalLayout_4->addWidget(txtSearchCategories);

        treeCategories = new QTreeWidget(grpCategories);
        treeCategories->setObjectName(QString::fromUtf8("treeCategories"));
        treeCategories->setFrameShape(QFrame::NoFrame);
        treeCategories->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeCategories->setSortingEnabled(true);
        treeCategories->setColumnCount(0);
        treeCategories->header()->setVisible(false);

        verticalLayout_4->addWidget(treeCategories);


        verticalLayout_3->addWidget(grpCategories);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 2);

        horizontalLayout_6->addLayout(verticalLayout_3);

        grpBrowser = new QGroupBox(tabBrowse);
        grpBrowser->setObjectName(QString::fromUtf8("grpBrowser"));
        verticalLayout_6 = new QVBoxLayout(grpBrowser);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        txtSearchAssets = new QLineEdit(grpBrowser);
        txtSearchAssets->setObjectName(QString::fromUtf8("txtSearchAssets"));

        verticalLayout_6->addWidget(txtSearchAssets);

        widBrowser = new QWidget(grpBrowser);
        widBrowser->setObjectName(QString::fromUtf8("widBrowser"));

        verticalLayout_6->addWidget(widBrowser);


        horizontalLayout_6->addWidget(grpBrowser);

        grpInfo = new QGroupBox(tabBrowse);
        grpInfo->setObjectName(QString::fromUtf8("grpInfo"));
        verticalLayout_7 = new QVBoxLayout(grpInfo);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));

        horizontalLayout_6->addWidget(grpInfo);

        horizontalLayout_6->setStretch(1, 1);
        tabWidget->addTab(tabBrowse, QString());
        tabImport = new QWidget();
        tabImport->setObjectName(QString::fromUtf8("tabImport"));
        verticalLayout_2 = new QVBoxLayout(tabImport);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        pushButton = new QPushButton(tabImport);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        lblPath = new QLabel(tabImport);
        lblPath->setObjectName(QString::fromUtf8("lblPath"));

        horizontalLayout_2->addWidget(lblPath);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        widPreview = new QWidget(tabImport);
        widPreview->setObjectName(QString::fromUtf8("widPreview"));
        widPreview->setMinimumSize(QSize(200, 200));
        widPreview->setAutoFillBackground(false);

        horizontalLayout_3->addWidget(widPreview);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        formLayout->setHorizontalSpacing(6);
        formLayout->setVerticalSpacing(2);
        label = new QLabel(tabImport);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        txtName = new QLineEdit(tabImport);
        txtName->setObjectName(QString::fromUtf8("txtName"));

        formLayout->setWidget(0, QFormLayout::FieldRole, txtName);

        label_2 = new QLabel(tabImport);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        cmbCategory = new QComboBox(tabImport);
        cmbCategory->setObjectName(QString::fromUtf8("cmbCategory"));
        sizePolicy.setHeightForWidth(cmbCategory->sizePolicy().hasHeightForWidth());
        cmbCategory->setSizePolicy(sizePolicy);
        cmbCategory->setEditable(true);

        formLayout->setWidget(1, QFormLayout::FieldRole, cmbCategory);

        label_3 = new QLabel(tabImport);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        txtTags = new QLineEdit(tabImport);
        txtTags->setObjectName(QString::fromUtf8("txtTags"));

        formLayout->setWidget(2, QFormLayout::FieldRole, txtTags);


        horizontalLayout_3->addLayout(formLayout);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        btnSave = new QPushButton(tabImport);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setEnabled(false);

        horizontalLayout_4->addWidget(btnSave);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalLayout_2->setStretch(1, 1);
        tabWidget->addTab(tabImport, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(TabAssetBrowser);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TabAssetBrowser);
    } // setupUi

    void retranslateUi(QWidget *TabAssetBrowser)
    {
        TabAssetBrowser->setWindowTitle(QApplication::translate("TabAssetBrowser", "Form", 0, QApplication::UnicodeUTF8));
        grpTypes->setTitle(QApplication::translate("TabAssetBrowser", "Types", 0, QApplication::UnicodeUTF8));
        grpCategories->setTitle(QApplication::translate("TabAssetBrowser", "Categories", 0, QApplication::UnicodeUTF8));
        grpBrowser->setTitle(QApplication::translate("TabAssetBrowser", "Browser", 0, QApplication::UnicodeUTF8));
        grpInfo->setTitle(QApplication::translate("TabAssetBrowser", "Information", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabBrowse), QApplication::translate("TabAssetBrowser", "Browse", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("TabAssetBrowser", "Open file...", 0, QApplication::UnicodeUTF8));
        lblPath->setText(QApplication::translate("TabAssetBrowser", "(none selected)", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TabAssetBrowser", "Asset name:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TabAssetBrowser", "Category:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TabAssetBrowser", "Tags:", 0, QApplication::UnicodeUTF8));
        btnSave->setText(QApplication::translate("TabAssetBrowser", "Save/Import", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabImport), QApplication::translate("TabAssetBrowser", "Import", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(TabAssetBrowser);
    } // retranslateUi

};

namespace Ui {
    class TabAssetBrowser: public Ui_TabAssetBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABASSETBROWSER_H
