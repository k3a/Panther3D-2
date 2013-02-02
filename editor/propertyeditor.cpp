#include "propertyeditor.h"
#include "ui_propertyeditor.h"

PropertyEditor::PropertyEditor(QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::PropertyEditor)
{
    m_ui->setupUi(this);
}

PropertyEditor::~PropertyEditor()
{
    delete m_ui;
}

void PropertyEditor::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
