

//#include "precompiled.h"
#include "EdPanel.h"

EdPanel::EdPanel(QWidget *parent)
: QDockWidget(parent)
{
	this->setWindowTitle("Dynamic Panel");
	this->setMinimumWidth(300);

	QWidget* dockContent = new QWidget(this);
	m_layout = new QVBoxLayout( dockContent );
	m_layout->addWidget(new QPushButton("Test", this));

	QMenuBar* mb = new QMenuBar(this);
	QMenu* m = new QMenu("MetuTitle",mb);
	m->addAction("TestAction");
	mb->addAction( m->menuAction() );


	m_layout->addWidget( mb );


	m_layout->addItem( new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding) );

	//m_layout->addWidget( new QDockAreaLayout(this) )

	this->setWidget(dockContent);
}