#include "domainedialog.h"
#include <QDebug>

DomaineDialog::DomaineDialog(QSqlDatabase db, int selectedId, QWidget *parent, Qt::WindowFlags f)
    :AbstractModelFormDialog(parent,f)
{
    setWindowTitle(tr("Manage Domaine"));
    setForm(QString(":/form/domaineForm.ui"));
    setModel(new DomaineModel(this,db));

    // Populate Country and Statut Combos
    populateCombo("Country","Country","Country");
    populateCombo("Statut","Statut","Producer_Statut");

    // set initial Data
     setInitialData(selectedId);

    // Connect buttons
    buttonBox()->addButton((selectedId !=-1)? tr("Update"): tr("Create"),QDialogButtonBox::ActionRole);
    connect(buttonBox(), &QDialogButtonBox::rejected,this,&DomaineDialog::reject);
    connect(buttonBox(),SIGNAL(clicked(QAbstractButton*)),this,SLOT(doAction(QAbstractButton*)));
}

DomaineModel *DomaineDialog::domaineModel() const
{
    return static_cast<DomaineModel *>(model());
}

void DomaineDialog::doAction(QAbstractButton *button)
{
    // Check if field Domaine is not empty
    if (buttonBox()->buttonRole(button) != QDialogButtonBox::ActionRole)
        return;
    if (lineEdit().at(indexOf("Domaine"))->text().isEmpty())
        return;

    // Populate record with data
    QSqlRecord rec =  domaineModel()->record();
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
        domaineModel()->insertRecord(-1,rec);}
    else {
        domaineModel()->setRecord(domaineModel()->rowPosition(rec.value("Id").toInt()),rec);}
    domaineModel()->submitAll();
    accept();

}
