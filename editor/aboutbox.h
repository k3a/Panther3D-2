#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <QtGui/QDialog>

namespace Ui {
    class AboutBox;
}

class AboutBox : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(AboutBox)
public:
    explicit AboutBox(QWidget *parent = 0);
    virtual ~AboutBox();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::AboutBox *m_ui;
};

#endif // ABOUTBOX_H
