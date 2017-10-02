#include "abstractformdialog.h"
#include <QDebug>

AbstractFormDialog::AbstractFormDialog(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{

}

void AbstractFormDialog::setForm(const QString &fileName)
{
    QUiLoader loader;
    QFile file(fileName);

   //Open File and load Form
    file.open(QFile::ReadOnly);
    QWidget *w = loader.load(&file,this);
    file.close();

   //Activate the slots by name to connect the form
    QMetaObject::connectSlotsByName(this);
    setMinimumHeight(w->height());
    setMinimumWidth(w->width());

    // Create a layout and add the form
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(w);

    //Assign the widgets of the form to the Dialog data
    setLineEdit();
    setTextEdit();
    setCombo();
    setSpinBox();
    setDoubleSpinBox();
    setDateEdit();
    setTableWidget();
    setButtonBox();
}

int AbstractFormDialog::indexOf(const QString &text)
{
    QString objectName;

    for(int i=0,n=lineEdit().size();i<n;i++) {
        objectName = lineEdit().at(i)->objectName();
        objectName.chop(QString("LineEdit").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return i;}

    for (int i=0,n=combo().size();i<n;i++) {
        objectName = combo().at(i)->objectName();
        objectName.chop(QString("ComboBox").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return i;}

    for(int i=0,n=textEdit().size();i<n;i++) {
        objectName = textEdit().at(i)->objectName();
        objectName.chop(QString("TextEdit").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return i;}

    for(int i=0,n=spinBox().size();i<n;i++) {
        objectName = spinBox().at(i)->objectName();
        objectName.chop(QString("SpinBox").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return i;}

    for(int i=0,n=doubleSpinBox().size();i<n;i++) {
        objectName = doubleSpinBox().at(i)->objectName();
        objectName.chop(QString("DoubleSpinBox").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return i;}

    for(int i=0,n=dateEdit().size();i<n;i++) {
        objectName = dateEdit().at(i)->objectName();
        objectName.chop(QString("DateEdit").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return i;}

    for(int i=0,n=tableWidget().size();i<n;i++) {
        objectName = tableWidget().at(i)->objectName();
        objectName.chop(QString("TableWidget").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return i;}

    return -1;
}

AbstractFormDialog::WidgetType AbstractFormDialog::typeOf(const QString &text)
{
    QString objectName;
    for(int i=0,n=lineEdit().size();i<n;i++) {
        objectName = lineEdit().at(i)->objectName();
        objectName.chop(QString("LineEdit").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return LineEdit;}

    for (int i=0,n=combo().size();i<n;i++) {
        objectName = combo().at(i)->objectName();
        objectName.chop(QString("ComboBox").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return ComboBox;}

    for(int i=0,n=textEdit().size();i<n;i++) {
        objectName = textEdit().at(i)->objectName();
        objectName.chop(QString("TextEdit").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return TextEdit;}

    for(int i=0,n=spinBox().size();i<n;i++) {
        objectName = spinBox().at(i)->objectName();
        objectName.chop(QString("SpinBox").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return SpinBox;}

    for(int i=0,n=doubleSpinBox().size();i<n;i++) {
        objectName = doubleSpinBox().at(i)->objectName();
        objectName.chop(QString("DoubleSpinBox").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return DoubleSpinBox;}

    for(int i=0,n=dateEdit().size();i<n;i++) {
        objectName = dateEdit().at(i)->objectName();
        objectName.chop(QString("DateEdit").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
              return DateEdit;}

    for(int i=0,n=tableWidget().size();i<n;i++) {
        objectName = tableWidget().at(i)->objectName();
        objectName.chop(QString("TableWidget").length());
        if (QString::compare(objectName,text,Qt::CaseInsensitive) == 0)
            return TableWidget;}
  return Unknown;
}


QList<QTableWidget *> AbstractFormDialog::tableWidget() const
{
    return m_tableWidget;
}

void AbstractFormDialog::setTableWidget()
{
    m_tableWidget = findChildren<QTableWidget*>();
}


QList<QDateEdit *> AbstractFormDialog::dateEdit() const
{
    return m_dateEdit;
}

void AbstractFormDialog::setDateEdit()
{
    m_dateEdit = findChildren<QDateEdit*>();
}

int AbstractFormDialog::getId()
{
    return lineEdit().at(indexOf("Id"))->text().toInt();
}

QList<QDoubleSpinBox *> AbstractFormDialog::doubleSpinBox() const
{
    return m_doubleSpinBox;
}

void AbstractFormDialog::setDoubleSpinBox()
{
    m_doubleSpinBox = findChildren<QDoubleSpinBox*>();
}

QList<QSpinBox *> AbstractFormDialog::spinBox() const
{
    return m_spinBox;
}

void AbstractFormDialog::setSpinBox()
{
    m_spinBox = findChildren<QSpinBox*>();
}

QDialogButtonBox *AbstractFormDialog::buttonBox() const
{
    return m_buttonBox;
}

void AbstractFormDialog::setButtonBox()
{
    m_buttonBox = findChild<QDialogButtonBox *>("buttonBox");
}

QList<QComboBox *> AbstractFormDialog::combo() const
{
    return m_combo;
}

void AbstractFormDialog::setCombo()
{
    m_combo = findChildren<QComboBox*>();
}

QList<QTextEdit *> AbstractFormDialog::textEdit() const
{
    return m_textEdit;
}

void AbstractFormDialog::setTextEdit()
{
    m_textEdit = findChildren<QTextEdit*>();
}

QList<QLineEdit *> AbstractFormDialog::lineEdit() const
{
    return m_lineEdit;
}

void AbstractFormDialog::setLineEdit()
{
    m_lineEdit = findChildren<QLineEdit*>();
}
