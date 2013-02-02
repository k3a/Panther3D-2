#ifndef TABSCENE_H
#define TABSCENE_H

#include <QtGui/QWidget>

namespace Ui {
    class TabScene;
}

class TabScene : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(TabScene)
public:
    explicit TabScene(QWidget *parent = 0);
    virtual ~TabScene();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::TabScene *m_ui;
};

#endif // TABSCENE_H
