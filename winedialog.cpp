#include "winedialog.h"
#include <QDebug>

WineDialog::WineDialog(QSqlDatabase db, int selectedId, QWidget *parent, Qt::WindowFlags f)
    :AbstractModelFormDialog(parent,f)
{
    setWindowTitle(tr("Manage Wine"));
    setForm(":/form/wineForm.ui");
    setModel(new WineModel(this,db));

    // Set Initial Data
    populateCombo("Type","Type","Wine_Type");
    populateAppelationCombo();
    setInitialData(selectedId);

    // Connect buttons
    buttonBox()->addButton((selectedId !=-1)? tr("Update"): tr("Create"),QDialogButtonBox::ActionRole);
    connect(buttonBox(), &QDialogButtonBox::rejected,this,&WineDialog::reject);
    connect(buttonBox(),SIGNAL(clicked(QAbstractButton*)),this,SLOT(doAction(QAbstractButton*)));
}

WineModel *WineDialog::wineModel() const
{
    return static_cast<WineModel *>(model());
}

int WineDialog::domaineId(const QString &text, bool &fUnique)
{
    int id=-1;
    QSqlQuery query;
    query.prepare(QString("SELECT Id FROM Domaine WHERE Domaine = '%1'").arg(text));
    query.exec();
    fUnique = query.first();
    if (fUnique) {
        id = query.value("Id").toInt();
        fUnique = !query.next();}
    return id;
}

void WineDialog::populateAppelationCombo(int wineTypeId)
{
    // Save Appelation Name

    QString str = combo().at(indexOf("Appelation"))->currentText();
    // Update combo
    combo().at(indexOf("Appelation"))->clear();
    QString filter = (wineTypeId==0)? QString() :QString("Couleur = %1").arg(wineTypeId);
    populateCombo("Appelation","Appelation","Appelation",filter);
    // Restore Appelation Name, if possible
    if(!str.isEmpty())
         combo()[indexOf("Appelation")]->setCurrentText(str);
}

void WineDialog::setInitialData(int id)
{
    // Execute standard settings
    AbstractModelFormDialog::setInitialData(id);

    // Find Domaine Name and fill the form field
    int domaineId = lineEdit().at(indexOf("Domaine"))->text().toInt();
    QSqlQuery query;
    query.prepare(QString("SELECT Domaine FROM Domaine WHERE Id = %1").arg(domaineId));
    query.exec();
    if (query.first())
        lineEdit().at(indexOf("Domaine"))->setText(query.value("Domaine").toString());
    else
        lineEdit().at(indexOf("Domaine"))->clear();

   // Populate the appelation Combo with Filter
    int wineTypeId = combo().at(indexOf("Type"))->currentIndex();
    if (wineTypeId !=0)
       populateAppelationCombo(wineTypeId);
}

void WineDialog::doAction(QAbstractButton *button)
{
    if (buttonBox()->buttonRole(button) != QDialogButtonBox::ActionRole)
        return;
    // Check if Domaine exists
    bool fUnique;
    int domaineid = domaineId(lineEdit().at(indexOf("Domaine"))->text(),fUnique);
    if (!fUnique)
        return;

    // Populate record with data
    QSqlRecord rec =  wineModel()->record();

    QString fieldName;
    for (int i=0, n=rec.count();i<n;i++) {
        fieldName = rec.fieldName(i);
        if (fieldName =="Id") {
            rec.setValue("Id",lineEdit().at(indexOf("Id"))->text().toInt());
            continue;}

        if (fieldName =="Domaine") {
            rec.setValue("Domaine", domaineid);
            continue;}

        switch (typeOf(fieldName)) {
        case LineEdit:
            rec.setValue(fieldName,lineEdit().at(indexOf(fieldName))->text());
            break;
        case ComboBox:
            if (fieldName == "Appelation")
                populateAppelationCombo();
            rec.setValue(fieldName, combo().at(indexOf(fieldName))->currentIndex());
            break;
        default:
            break;
        }
      }
    if (button->text() == tr("Create")) {
        wineModel()->insertRecord(-1,rec);}
    else {
        wineModel()->setRecord(wineModel()->rowPosition(rec.value("Id").toInt()),rec);}
        wineModel()->submitAll();
        accept();
}

void WineDialog::on_domaineButton_clicked()
{
    DomaineQueryDialog * dialog = new DomaineQueryDialog(lineEdit().at(indexOf("Domaine"))->text(),wineModel()->database());
     if (dialog->exec() == QDialog::Accepted)
         lineEdit().at(indexOf("Domaine"))->setText(dialog->selectedName());
     dialog->deleteLater();
}

void WineDialog::on_appelationButton_clicked()
{

}

void WineDialog::on_typeComboBox_activated(int index)
{
    if (index !=-1)
        populateAppelationCombo(index);
}
