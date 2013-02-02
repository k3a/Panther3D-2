#ifndef TABASSETBROWSER_H
#define TABASSETBROWSER_H

#include <QtGui/QWidget>

namespace Ui {
    class TabAssetBrowser;
}

class TabAssetBrowser : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(TabAssetBrowser)
public:
    explicit TabAssetBrowser(QWidget *parent = 0);
    virtual ~TabAssetBrowser();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::TabAssetBrowser *m_ui;
};

#endif // TABASSETBROWSER_H
