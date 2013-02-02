#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include <QtGui/QDockWidget>

namespace Ui {
    class PropertyEditor;
}

class PropertyEditor : public QDockWidget {
    Q_OBJECT
    Q_DISABLE_COPY(PropertyEditor)
public:
    explicit PropertyEditor(QWidget *parent = 0);
    virtual ~PropertyEditor();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::PropertyEditor *m_ui;
};

#endif // PROPERTYEDITOR_H
