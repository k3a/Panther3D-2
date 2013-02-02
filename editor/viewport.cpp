#include "viewport.h"
#include "ui_viewport.h"

//---------------------------------------------------------------------------
Viewport::Viewport(QWidget *parent) :
    QFrame(parent),
    m_ui(new Ui::Viewport)
{
    m_ui->setupUi(this);

	// *** create menu **************
	QMenuBar* mb = new QMenuBar(this);
	QMenu* m;
	QActionGroup* ag;

	// Camera
	m = new QMenu("Camera",mb);
	// - Scene cameras
	 m_mSceneCameras = m->addMenu("Scene Cameras");
	 m_mSceneCameras->addAction("(none)");
	/**/m->addSeparator();
	ag = new QActionGroup(this);
	ag->addAction(m_ui->aCameraPerspective);
	ag->addAction(m_ui->aCameraOrtho);
	m->addActions(ag->actions());
	/**/m->addSeparator();
	ag = new QActionGroup(this);
	ag->addAction(m_ui->aCameraLeft);
	ag->addAction(m_ui->aCameraRight);
	ag->addAction(m_ui->aCameraFront);
	ag->addAction(m_ui->aCameraBack);
	ag->addAction(m_ui->aCameraTop);
	ag->addAction(m_ui->aCameraBottom);
	m->addActions(ag->actions());
	//
	mb->addAction( m->menuAction() );

	// View
	m = new QMenu("View",mb);
	//
	ag = new QActionGroup(this);
	ag->addAction(m_ui->aView1);
	ag->addAction(m_ui->aView2);
	ag->addAction(m_ui->aView3);
	ag->addAction(m_ui->aView4);
	ag->addAction(m_ui->aViewAll);
	m->addActions(ag->actions());
	//
	mb->addAction( m->menuAction() );

	m_ui->horizontalLayout->addWidget(mb);
}

//---------------------------------------------------------------------------
Viewport::~Viewport()
{
    delete m_ui;
}

//---------------------------------------------------------------------------
void Viewport::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//---------------------------------------------------------------------------
void Viewport::on_mnuCamera_customContextMenuRequested(QPoint pos)
{
        QMessageBox::warning(
        this, windowTitle(),
        QString().sprintf("x=%d y=%d", pos.x(), pos.y())
    );

}
