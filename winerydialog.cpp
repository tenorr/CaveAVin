#include "winerydialog.h"
#include <QDebug>

WineryDialog::WineryDialog(QSqlDatabase db, int selectedId, QWidget *parent, Qt::WindowFlags f)
    :AbstractModelFormDialog(parent,f)
{
    setWindowTitle(tr("Manage Winery"));
    setForm(QString(":/form/wineryForm.ui"));
    setModel(new WineryModel(this,db));

    // Populate Country and Statut Combos
    populateCombo("Country","Country","Country");
    populateCombo("Status","Status","ProducerStatus");

    // set initial Data
     setInitialData(selectedId);

    // Connect buttons
    buttonBox()->addButton((selectedId !=-1)? tr("Update"): tr("Create"),QDialogButtonBox::ActionRole);
    connect(buttonBox(), &QDialogButtonBox::rejected,this,&WineryDialog::reject);
    connect(buttonBox(),SIGNAL(clicked(QAbstractButton*)),this,SLOT(doAction(QAbstractButton*)));
}

WineryModel *WineryDialog::wineryModel() const
{
    return static_cast<WineryModel *>(model());
}

void WineryDialog::doAction(QAbstractButton *button)
{
    // Check if field Winery is not empty
    if (buttonBox()->buttonRole(button) != QDialogButtonBox::ActionRole)
        return;
    if (lineEdit().at(indexOf("Winery"))->text().isEmpty())
        return;

    // Populate record with data
    QSqlRecord rec =  wineryModel()->record();
    QString fieldName;
    for (int i=0, n=rec.count();i<n;i++) {
        fieldName = rec.fieldName(i);
        if (fieldName =="Id") {
            rec.setValue("Id",lineEdit().at(indexOf("Id"))->text().toInt());
            continue;}

        switch (typeOf(fieldName)) {
        case LineEdit:
            rec.setValue(fieldName,lineEdit().at(indexOf(fieldName))->text());
            break;
        case TextEdit:
            rec.setValue(fieldName,textEdit().at(indexOf(fieldName))->toPlainText());
            break;
        case ComboBox:
            rec.setValue(fieldName, combo().at(indexOf(fieldName))->currentIndex());
            break;
        default:
            break;
        }
      }
        // Create or Update
    if (button->text() == tr("Create")) {
        wineryModel()->insertRecord(-1,rec);}
    else {
        wineryModel()->setRecord(wineryModel()->rowPosition(rec.value("Id").toInt()),rec);}
    wineryModel()->submitAll();
    accept();

}
