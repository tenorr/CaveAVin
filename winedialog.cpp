#include "winedialog.h"
#include <QDebug>

WineDialog::WineDialog(QSqlDatabase db, int selectedId, QWidget *parent, Qt::WindowFlags f)
    :AbstractModelFormDialog(parent,f)
{

    setWindowTitle(tr("Manage Wine"));
    setForm(":/form/wineForm.ui");
    setModel(new WineModel(this,db));

    // Connect buttons
    QPushButton *button = new QPushButton((selectedId !=-1)? tr("Update"): tr("Create"));
    button->setObjectName("actionButton");
    connect(button,&QPushButton::clicked,this,&WineDialog::doAction);
    buttonBox()->addButton(button,QDialogButtonBox::ActionRole);
    connect(buttonBox(), &QDialogButtonBox::rejected,this,&WineDialog::reject);

    // Set Initial Data
    populateCombo("Type","Type","Wine_Type");
    populateCombo("Region","Region","Region");
    populateAppelationCombo();
    setInitialData(selectedId);
    setActionButtonEnabled();

 //  lineEdit().at(indexOf("AppelationId"))->hide();
 //  lineEdit().at(indexOf("DomaineId"))->hide();

}

WineModel *WineDialog::wineModel() const
{
    return static_cast<WineModel *>(model());
}

int WineDialog::domaineId(const QString &text, bool &fUnique)
{
    int id=-1;
    QString str = text;
    str = str.replace("'","''");
    QSqlQuery query;
    query.prepare(QString("SELECT Id FROM Domaine WHERE Domaine = '%1'").arg(str));
    query.exec();
    fUnique = query.first();
    if (fUnique) {
        id = query.value("Id").toInt();
        fUnique = !query.next();}
    return id;
}

void WineDialog::populateAppelationCombo(int wineTypeId, int regionId)
{
    // Save Appelation Name
    QString str = appelationStr(false);
    // Update combo
    combo().at(indexOf("Appelation"))->clear();
    QString filter = (wineTypeId==0)? QString() :QString("Couleur = %1").arg(wineTypeId);
    if (regionId!=0) {
        filter.append((wineTypeId==0)? QString() : " AND ");
        filter.append(QString("Region = %1").arg(regionId));}
    populateCombo("Appelation","Appelation","Appelation",filter);

    // Restore Appelation Name, if possible
    if(!str.isEmpty()) {
        combo()[indexOf("Appelation")]->setCurrentText(str);
    // Clear Appelation Type Line Edit if appelation is null
    if (combo().at(indexOf("Appelation"))->currentIndex()==0) {
        lineEdit().at(indexOf("AppelationType"))->clear();
        lineEdit().at(indexOf("AppelationId"))->clear();}
    }
}

void WineDialog::setAppellationFields(int appelationId)
{
    if (appelationId !=0) {
    QSqlQuery query;
    query.prepare(QString("SELECT a.Region, a.Couleur, t.Type FROM Appelation AS a "
                          "INNER JOIN Appelation_Type AS t ON t.Id = a.type "
                          "WHERE a.Id = %1").arg(appelationId));
    query.exec();
    if (query.first()) {
        lineEdit().at(indexOf("AppelationType"))->setText(query.value("Type").toString());
        combo().at(indexOf("Region"))->setCurrentIndex(query.value("Region").toInt());
        combo().at(indexOf("Type"))->setCurrentIndex(query.value("Couleur").toInt());
        populateAppelationCombo(query.value("Couleur").toInt(),query.value("Region").toInt());
    }
    }
    else {
     lineEdit().at(indexOf("AppelationType"))->clear();
     combo().at(indexOf("Region"))->setCurrentIndex(0);}
}

int WineDialog::appelationId()
{
    if (!appelationStr().isEmpty()) {
    QString str = QString("SELECT Id FROM Appelation WHERE (Appelation = '%1') ").arg(appelationStr());

    // Don't filter the region if no Region displayed
    if (regionId()!=0)
        str = str.append(QString("AND (Region = %1)").arg(regionId()));

    QSqlQuery query;
    query.prepare(str);
    query.exec();
    if (query.first())
        return query.value("Id").toInt();
    return 0;}
    return 0;
}

QString WineDialog::appelationStr(bool fQuery)
{
    QString str = combo().at(indexOf("Appelation"))->currentText();
    if (fQuery)
        str= str.replace("'","''");
    return str;
}

int WineDialog::regionId()
{
    return combo().at(indexOf("Region"))->currentIndex();
}

int WineDialog::wineTypeId()
{
    return combo().at(indexOf("Type"))->currentIndex();
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
    if (query.first()) {
        lineEdit().at(indexOf("Domaine"))->setText(query.value("Domaine").toString());
        lineEdit().at(indexOf("DomaineId"))->setText(QString::number(domaineId));}
    else
        lineEdit().at(indexOf("Domaine"))->clear();

    int appelationId = combo().at(indexOf("Appelation"))->currentIndex();

   // Set the appelation fields and Populate the appelation Combo with Filter
    if (appelationId !=0) {
        lineEdit().at(indexOf("AppelationId"))->setText(QString::number(appelationId));
        setAppellationFields(appelationId);}
}

void WineDialog::doAction()
{
    // Populate record with data
    QSqlRecord rec =  wineModel()->record();

    QString fieldName;
    for (int i=0, n=rec.count();i<n;i++) {
        fieldName = rec.fieldName(i);
        if (fieldName =="Id") {
            rec.setValue("Id",lineEdit().at(indexOf("Id"))->text().toInt());
            continue;}

        if (fieldName =="Domaine") {
            rec.setValue("Domaine",lineEdit().at(indexOf("DomaineId"))->text().toInt());
            continue;}

        if (fieldName =="Appelation") {
            rec.setValue("Appelation",lineEdit().at(indexOf("AppelationId"))->text().toInt());
            continue;}

        switch (typeOf(fieldName)) {
        case LineEdit:
            rec.setValue(fieldName,lineEdit().at(indexOf(fieldName))->text());
            break;
        case ComboBox:
                rec.setValue(fieldName,combo().at(indexOf(fieldName))->currentIndex());
            break;
        default:
            break;
        }
      }
    QPushButton *button = buttonBox()->findChild<QPushButton *>("actionButton");
    if (button->text() == tr("Create"))
        wineModel()->insertRecord(-1,rec);
    else
        wineModel()->setRecord(wineModel()->rowPosition(rec.value("Id").toInt()),rec);

    wineModel()->submitAll();
    accept();
}

void WineDialog::on_domaineButton_clicked()
{
    DomaineQueryDialog * dialog = new DomaineQueryDialog(lineEdit().at(indexOf("Domaine"))->text(),wineModel()->database());
     if (dialog->exec() == QDialog::Accepted) {
         lineEdit().at(indexOf("DomaineId"))->setText(QString::number(dialog->selectedId()));
         lineEdit().at(indexOf("Domaine"))->setText(dialog->selectedName());}
     dialog->deleteLater();
}

void WineDialog::on_domaineLineEdit_textEdited(const QString &text)
{
    bool fUnique;
    int dId = domaineId(text,fUnique);
    if (fUnique)
        lineEdit().at(indexOf("DomaineId"))->setText(QString::number(dId));
    else
        lineEdit().at(indexOf("DomaineId"))->clear();
}

void WineDialog::on_appelationButton_clicked()
{
    AppelationQueryDialog *dialog = new AppelationQueryDialog(combo().at(indexOf("Appelation"))->currentText(),wineModel()->database());
    if (dialog->exec() == QDialog::Accepted) {
       int aId = dialog->selectedId();
       setCombosFromAppelationId(aId);
       lineEdit().at(indexOf("AppelationId"))->setText(QString::number(aId));
    }
    dialog->deleteLater();
}

void WineDialog::on_appelationComboBox_activated(int index)
{
    if (index!=-1) {
        int aId = appelationId();
        if (aId !=0)
            lineEdit().at(indexOf("AppelationId"))->setText(QString::number(aId));
        else
            lineEdit().at(indexOf("AppelationId"))->clear();
        setAppellationFields(aId);
    }
}

void WineDialog::on_regionComboBox_activated(int index)
{
    if (index !=-1)
       // Populate Appelation Combo according to Region and Wine Type
        populateAppelationCombo(wineTypeId(),index);
}

void WineDialog::on_typeComboBox_activated(int index)
{
    if (index !=-1)
        // Populate Appelation Combo according to Region and Wine Type
        populateAppelationCombo(index,regionId());
}

void WineDialog::on_appelationIdLineEdit_textChanged(const QString &text)
{
       setActionButtonEnabled();
    Q_UNUSED(text)
}

void WineDialog::on_domaineIdLineEdit_textChanged(const QString &text)
{
        setActionButtonEnabled();
    Q_UNUSED(text)
}

void WineDialog::setActionButtonEnabled()
{
    QPushButton *button = buttonBox()->findChild<QPushButton *>("actionButton");
    button->setEnabled(!lineEdit().at(indexOf("AppelationId"))->text().isEmpty() && !lineEdit().at(indexOf("DomaineId"))->text().isEmpty());
}

void WineDialog::setCombosFromAppelationId(const int &appelationId)
{
    int wineTypeId =0;
    int regionId=0;
    QString appelationStr;

    if (appelationId > 0) {
        // Find Wine Type and Region from Appelation
        QSqlQuery query;
        query.prepare(QString("SELECT Appelation, Region, Couleur FROM Appelation WHERE Id = %1").arg(appelationId));
        query.exec();

        if (query.first()) {
            wineTypeId = query.value("Couleur").toInt();
            regionId = query.value("Region").toInt();
            appelationStr = query.value("Appelation").toString();
               }
    }

    if (wineTypeId !=0)
        combo().at(indexOf("Type"))->setCurrentIndex(wineTypeId);
    else
        combo().at(indexOf("Type"))->clear();

    if (regionId !=0)
        combo().at(indexOf("Region"))->setCurrentIndex(regionId);
    else
        combo().at(indexOf("Region"))->clear();

    populateAppelationCombo(wineTypeId,regionId);
    if (!appelationStr.isEmpty()) {
        combo().at(indexOf("Appelation"))->setCurrentText(appelationStr);
        setAppellationFields(appelationId);}

     setActionButtonEnabled();
}
