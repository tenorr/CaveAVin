#include "appellationquerydialog.h"

AppellationQueryDialog::AppellationQueryDialog(QString appellationStr, QSqlDatabase db, QWidget *parent, Qt::WindowFlags f)
    :QueryDialog(parent,f)
{
    // Create Title
    setWindowTitle(tr("Find Appellation"));

    // Set model and view
    setModelAndView(new AppellationModel(this,db));

    // Define Text and Hide selection
    QStringList textList {tr("Appellation :"),"",tr("Region :"), tr("Type :"), tr("Color")};
    setLabelText(textList);

    // Populate Combos
    populateCombo("Region","Region",0);
    populateCombo("Type","AppellationType",1);
    populateCombo("Type","WineType",2);

    // Field to be return
    setSelectedFieldName("Appellation");

    // Fields to be shown
    QStringList shownFieldNameList {"Appellation","Region","Type"};
    setShownFieldNames(shownFieldNameList);

    // Show Color Field
     view()->showColumn(appellationModel()->columnCount()-1);

    // Set the initial Appellation string
    queryLineEdit().at(0)->setText(appellationStr);

    //Connect matching signals from form to slots of object
     QMetaObject::connectSlotsByName(this);

     // Hide buttons
     QStringList list {"createButton", "showButton"};
     foreach (QString str, list) {
         hideButton(str);
     }

     // Select the Appellation if any
     appellationModel()->selectRecords(appellationStr);
     adjustViewSize();
}

AppellationModel *AppellationQueryDialog::appellationModel() const
{
    return static_cast<AppellationModel *>(model());
}

void AppellationQueryDialog::on_queryLineEdit_textEdited(const QString &text)
{
     QString appellationStr =text;
     int regionId = findRegionId(queryCombo().at(0)->currentText());
     int appellationTypeId = findAppellationTypeId(queryCombo().at(1)->currentText());
     int wineTypeId = findWineTypeId(queryCombo().at(2)->currentText());

    appellationModel()->selectRecords(appellationStr,regionId,appellationTypeId,wineTypeId);
    adjustViewSize();
}

void AppellationQueryDialog::on_queryComboBox_activated(const QString &text)
{
    // Retrieve Data
    QString appellationStr  = queryLineEdit().at(0)->text();
    int regionId = findRegionId(text);
    int appellationTypeId = findAppellationTypeId(queryCombo().at(1)->currentText());
    int wineTypeId = findWineTypeId(queryCombo().at(2)->currentText());
    appellationModel()->selectRecords(appellationStr,regionId,appellationTypeId,wineTypeId);
    adjustViewSize();
}

void AppellationQueryDialog::on_queryComboBox2_activated(const QString &text)
{
    // Retrieve Data
    QString appellationStr  = queryLineEdit().at(0)->text();
    int regionId = findRegionId(queryCombo().at(0)->currentText());
    int appellationTypeId = findAppellationTypeId(text);
    int wineTypeId = findWineTypeId(queryCombo().at(2)->currentText());
    appellationModel()->selectRecords(appellationStr,regionId,appellationTypeId,wineTypeId);
    adjustViewSize();
}

void AppellationQueryDialog::on_queryComboBox3_activated(const QString &text)
{
    // Retrieve Data
    QString appellationStr  = queryLineEdit().at(0)->text();
    int regionId = findRegionId(queryCombo().at(0)->currentText());
    int appellationTypeId = findAppellationTypeId(queryCombo().at(1)->currentText());
    int wineTypeId = findWineTypeId(text);
    appellationModel()->selectRecords(appellationStr,regionId,appellationTypeId,wineTypeId);
    adjustViewSize();
}

int AppellationQueryDialog::findRegionId(const QString &text)
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

int AppellationQueryDialog::findAppellationTypeId(const QString &text)
{
    int appellationTypeId=0;
    if (!text.isEmpty()) {
    QSqlQuery query;
    query.prepare(QString("SELECT Id FROM AppellationType WHERE Type = '%1'").arg(text));
    query.exec();
    if (query.first())
        appellationTypeId = query.value("Id").toInt();}
    return appellationTypeId;
}


int AppellationQueryDialog::findWineTypeId(const QString &text)
{
    int wineTypeId=0;
     if (!text.isEmpty()) {
    QSqlQuery query;
    query.prepare(QString("SELECT Id FROM WineType WHERE Type = '%1'").arg(text));
    query.exec();
    if (query.first())
        wineTypeId = query.value("Id").toInt();}
    return wineTypeId;
}
