#include "appelationquerydialog.h"

AppelationQueryDialog::AppelationQueryDialog(QString appelationStr, QSqlDatabase db,QWidget *parent, Qt::WindowFlags f)
    :QueryDialog(parent,f)
{
    // Create Title
    setWindowTitle(tr("Find Appelation"));

    // Set model and view
    setModelAndView(new AppelationModel(this,db));

    // Define Text and Hide selection
    QStringList textList {tr("Appelation :"),"",tr("Region :"), tr("Type :"), tr("Color")};
    setLabelText(textList);

    // Populate Combos
    populateCombo("Region","Region",0);
    populateCombo("Type","Appelation_Type",1);
    populateCombo("Type","Wine_Type",2);

    // Field to be return
    setSelectedFieldName("Appelation");

    // Fields to be shown
    QStringList shownFieldNameList {"Appelation","Region","Type"};
    setShownFieldNames(shownFieldNameList);

    // Show Color Field
     view()->showColumn(appelationModel()->columnCount()-1);

    // Set the initial appelation string
    queryLineEdit().at(0)->setText(appelationStr);

    //Connect matching signals from form to slots of object
     QMetaObject::connectSlotsByName(this);

     // Hide buttons
     QStringList list {"createButton", "showButton"};
     foreach (QString str, list) {
         hideButton(str);
     }

     // Select the appelation if any
     appelationModel()->selectRecords(appelationStr);
}

AppelationModel *AppelationQueryDialog::appelationModel() const
{
    return static_cast<AppelationModel *>(model());
}

void AppelationQueryDialog::on_queryLineEdit_textEdited(const QString &text)
{
     QString appelationStr =text;
     int regionId = findRegionId(queryCombo().at(0)->currentText());
     int appelationTypeId = findAppelationTypeId(queryCombo().at(1)->currentText());
     int wineTypeId = findWineTypeId(queryCombo().at(2)->currentText());

    appelationModel()->selectRecords(appelationStr,regionId,appelationTypeId,wineTypeId);
}

void AppelationQueryDialog::on_queryComboBox_activated(const QString &text)
{
    // Retrieve Data
    QString appelationStr  = queryLineEdit().at(0)->text();
    int regionId = findRegionId(text);
    int appelationTypeId = findAppelationTypeId(queryCombo().at(1)->currentText());
    int wineTypeId = findWineTypeId(queryCombo().at(2)->currentText());
    appelationModel()->selectRecords(appelationStr,regionId,appelationTypeId,wineTypeId);
}

void AppelationQueryDialog::on_queryComboBox2_activated(const QString &text)
{
    // Retrieve Data
    QString appelationStr  = queryLineEdit().at(0)->text();
    int regionId = findRegionId(queryCombo().at(0)->currentText());
    int appelationTypeId = findAppelationTypeId(text);
    int wineTypeId = findWineTypeId(queryCombo().at(2)->currentText());
    appelationModel()->selectRecords(appelationStr,regionId,appelationTypeId,wineTypeId);
}

void AppelationQueryDialog::on_queryComboBox3_activated(const QString &text)
{
    // Retrieve Data
    QString appelationStr  = queryLineEdit().at(0)->text();
    int regionId = findRegionId(queryCombo().at(0)->currentText());
    int appelationTypeId = findAppelationTypeId(queryCombo().at(1)->currentText());
    int wineTypeId = findWineTypeId(text);
    appelationModel()->selectRecords(appelationStr,regionId,appelationTypeId,wineTypeId);
}

int AppelationQueryDialog::findRegionId(const QString &text)
{
    int regionId=0;
   if (!text.isEmpty()) {
    QSqlQuery query;
    query.prepare(QString("SELECT Id FROM Region WHERE Region = '%1'").arg(text));
    query.exec();
    if (query.first())
        regionId = query.value("Id").toInt();}
    return regionId;
}

int AppelationQueryDialog::findAppelationTypeId(const QString &text)
{
    int appelationTypeId=0;
    if (!text.isEmpty()) {
    QSqlQuery query;
    query.prepare(QString("SELECT Id FROM Appelation_Type WHERE Type = '%1'").arg(text));
    query.exec();
    if (query.first())
        appelationTypeId = query.value("Id").toInt();}
    return appelationTypeId;
}


int AppelationQueryDialog::findWineTypeId(const QString &text)
{
    int wineTypeId=0;
     if (!text.isEmpty()) {
    QSqlQuery query;
    query.prepare(QString("SELECT Id FROM Wine_Type WHERE Type = '%1'").arg(text));
    query.exec();
    if (query.first())
        wineTypeId = query.value("Id").toInt();}
    return wineTypeId;
}
