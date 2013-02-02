/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 08/27/2009 File created - kexik
*/
#include "tabassetbrowser.h"
#include "ui_tabassetbrowser.h"

#include "mainwindow.h"
#include "IEngine.h"
#include "IFilesystem.h"

using namespace P3D;

//---------------------------------------------------------------------------
TabAssetBrowser::TabAssetBrowser(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::TabAssetBrowser)
{
    m_ui->setupUi(this);
}

//---------------------------------------------------------------------------
TabAssetBrowser::~TabAssetBrowser()
{
    delete m_ui;
}

//---------------------------------------------------------------------------
void TabAssetBrowser::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }

	if (e->type() == QEvent::ActivateControl)
	{
	/*	m_ui->treeCategories->clear();

		// temporary
		QList<QTreeWidgetItem *> items;

		IEngine* eng = MainWindow::mEngine();

		tArray<sFSSearchResult> &sr = eng->mFilesystem()->GetContentOfCategory(_W(""));
		for (unsigned int i=0; i<sr.Size(); i++)
		{
			if (sr[i].Type == FSI_DIR)
				items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(  QString( _W2A(sr[i].Name) )  )   ));
		}

		m_ui->treeCategories->insertTopLevelItems(0, items);*/
	}


	/*
	tArray<sFSSearchResult> &sr = this->mFilesystem()->GetContentOfCategory(_W("textures"));
	for (unsigned int i=0; i<sr.Size(); i++)
	{
	if (sr[i].Type == FSI_FILE)
	CON(MSG_INFO, _W("File %s"), sr[i].Name );
	else if (sr[i].Type == FSI_DIR)
	CON(MSG_INFO, _W("Dir %s"), sr[i].Name );
	//else
	}
	*/
}
