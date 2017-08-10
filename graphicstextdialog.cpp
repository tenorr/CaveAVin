#include "graphicstextdialog.h"

GraphicsTextDialog::GraphicsTextDialog(QString currentName, QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{
        setCurrentName(currentName);
        setLineEdit(new QLineEdit);
        setFormLayout(new QFormLayout);
        setDialogButtonBox(new QDialogButtonBox(QDialogButtonBox::Ok
                                              | QDialogButtonBox::Cancel));
        setBoxLayout(new QBoxLayout(QBoxLayout::TopToBottom));

        setWindowTitle(tr("Set New Name"));
}

GraphicsTextDialog::~GraphicsTextDialog()
{

}

QLineEdit *GraphicsTextDialog::lineEdit() const
{
    return m_lineEdit;
}

void GraphicsTextDialog::setLineEdit(QLineEdit *lineEdit)
{
    m_lineEdit = lineEdit;
}

QFormLayout *GraphicsTextDialog::formLayout() const
{
    return m_formLayout;
}

void GraphicsTextDialog::setFormLayout(QFormLayout *formLayout)
{
       QLineEdit *currentNameLineEdit = new QLineEdit(currentName(),this);
       currentNameLineEdit->setReadOnly(true);
       formLayout->addRow(tr("Current Name"),currentNameLineEdit);
       formLayout->addRow(tr("New Name"),lineEdit());

       m_formLayout = formLayout;
}

QString GraphicsTextDialog::currentName() const
{
    return m_currentName;
}

void GraphicsTextDialog::setCurrentName(const QString &currentName)
{
    m_currentName = currentName;
}

QDialogButtonBox *GraphicsTextDialog::dialogButtonBox() const
{
    return m_dialogButtonBox;
}

void GraphicsTextDialog::setDialogButtonBox(QDialogButtonBox *dialogButtonBox)
{
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    m_dialogButtonBox = dialogButtonBox;
}

QBoxLayout *GraphicsTextDialog::boxLayout() const
{
    return m_boxLayout;
}

void GraphicsTextDialog::setBoxLayout(QBoxLayout *boxLayout)
{
       boxLayout->addLayout(formLayout());
       boxLayout->addWidget(dialogButtonBox());
       setLayout(boxLayout);
       m_boxLayout = boxLayout;
}
