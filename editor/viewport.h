#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QtGui/QFrame>
#include "IEngine.h"

using namespace P3D;

namespace Ui {
    class Viewport;
}

class Viewport : public QFrame {
    Q_OBJECT
    Q_DISABLE_COPY(Viewport)
public:
    explicit Viewport(QWidget *parent = 0);
    virtual ~Viewport();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::Viewport *m_ui;

	// *** engine ********
	IRenderWindow *m_pRenderWindow;

	// *** actions ********
	// cameras
	QMenu* m_mSceneCameras;
	QAction* m_aCameraPerspective;
	QAction* m_aCameraOrtho;
	//
	QAction* m_aCameraLeft;
	QAction* m_aCameraRight;
	QAction* m_aCameraFront;
	QAction* m_aCameraBack;
	QAction* m_aCameraTop;
	QAction* m_aCameraBottom;

private slots:
    void on_mnuCamera_customContextMenuRequested(QPoint pos);
};

#endif // VIEWPORT_H
