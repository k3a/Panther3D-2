#pragma once
#include <QtGui/QDockWidget>

class EdPanel : public QDockWidget
{
public:
	EdPanel(QWidget *parent);

private:
	QVBoxLayout *m_layout;
};