#include "tabscene.h"
#include "ui_tabscene.h"

#include "viewport.h"

TabScene::TabScene(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::TabScene)
{
    m_ui->setupUi(this);
	m_ui->layout->addWidget( new Viewport(), 0, 0 );
	m_ui->layout->addWidget( new Viewport(), 0, 1 );
	m_ui->layout->addWidget( new Viewport(), 1, 0 );
	m_ui->layout->addWidget( new Viewport(), 1, 1 );
}

TabScene::~TabScene()
{
    delete m_ui;
}

void TabScene::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
