#include "winedialog.h"
#include <QDebug>

WineDialog::WineDialog(QSqlDatabase db, int selectedId, QWidget *parent, Qt::WindowFlags f)
    :PhotoFormDialog(parent,f)
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
    populateCombo("Type","Type","WineType");
    populateCombo("Region","Region","Region");
    populateAppellationCombo();
    setInitialData(selectedId);
    setInitialWineType(combo().at(indexOf("Type"))->currentIndex());
    setActionButtonEnabled();

   lineEdit().at(indexOf("AppellationId"))->hide();
   lineEdit().at(indexOf("WineryId"))->hide();
   lineEdit().at(indexOf("GrapeVariety"))->hide();
   lineEdit().at(indexOf("LabelImage"))->hide();
}

WineModel *WineDialog::wineModel() const
{
    return static_cast<WineModel *>(model());
}

int WineDialog::wineryId(const QString &text, bool &fUnique)
{
    int id=-1;
    QString str = text;
    str = str.replace("'","''");
    QSqlQuery query;
    query.prepare(QString("SELECT Id FROM Winery WHERE Winery = '%1'").arg(str));
    query.exec();
    fUnique = query.first();
    if (fUnique) {
        id = query.value("Id").toInt();
        fUnique = !query.next();}
    return id;
}

void WineDialog::populateAppellationCombo(int wineTypeId, int regionId)
{
    // Save Appellation Name
    QString str = appellationStr(false);
    // Update combo
    combo().at(indexOf("Appellation"))->clear();
    QString filter = (wineTypeId==0)? QString() :QString("Colour = %1").arg(wineTypeId);
    if (regionId!=0) {
        filter.append((wineTypeId==0)? QString() : " AND ");
        filter.append(QString("Region = %1").arg(regionId));}
    populateCombo("Appellation","Appellation","Appellation",filter);

    // Restore Appellation Name, if possible
    if(!str.isEmpty()) {
        combo()[indexOf("Appellation")]->setCurrentText(str);
    // Clear Appellation Type Line Edit if Appellation is null
    if (combo().at(indexOf("Appellation"))->currentIndex()==0) {
        lineEdit().at(indexOf("AppellationType"))->clear();
        lineEdit().at(indexOf("AppellationId"))->clear();}
    }
}

void WineDialog::setAppellationFields(int AppellationId)
{
    if (AppellationId !=0) {
    QSqlQuery query;
    query.prepare(QString("SELECT a.Region, a.Colour, t.Type FROM Appellation AS a "
                          "INNER JOIN AppellationType AS t ON t.Id = a.type "
                          "WHERE a.Id = %1").arg(AppellationId));
    query.exec();
    if (query.first()) {
        lineEdit().at(indexOf("AppellationType"))->setText(query.value("Type").toString());
        combo().at(indexOf("Region"))->setCurrentIndex(query.value("Region").toInt());
        combo().at(indexOf("Type"))->setCurrentIndex(query.value("Colour").toInt());
        populateAppellationCombo(query.value("Colour").toInt(),query.value("Region").toInt());
    }
    }
    else {
     lineEdit().at(indexOf("AppellationType"))->clear();
     combo().at(indexOf("Region"))->setCurrentIndex(0);}
}

int WineDialog::appellationId()
{
    if (!appellationStr().isEmpty()) {
    QString str = QString("SELECT Id FROM Appellation WHERE (Appellation = '%1') ").arg(appellationStr());

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

QString WineDialog::appellationStr(bool fQuery)
{
    QString str = combo().at(indexOf("Appellation"))->currentText();
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
    setPhoto();

    // Execute standard settings
    AbstractModelFormDialog::setInitialData(id);

    // Find Winery Name and fill the form field
    int wineryId = lineEdit().at(indexOf("Winery"))->text().toInt();
    QSqlQuery query;
    query.prepare(QString("SELECT Winery FROM Winery WHERE Id = %1").arg(wineryId));
    query.exec();
    if (query.first()) {
        lineEdit().at(indexOf("Winery"))->setText(query.value("Winery").toString());
        lineEdit().at(indexOf("WineryId"))->setText(QString::number(wineryId));}
    else
        lineEdit().at(indexOf("Winery"))->clear();

    int AppellationId = combo().at(indexOf("Appellation"))->currentIndex();

   // Set the Appellation fields and Populate the Appellation Combo with Filter
    if (AppellationId !=0) {
        lineEdit().at(indexOf("AppellationId"))->setText(QString::number(AppellationId));
        setAppellationFields(AppellationId);}
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

        if (fieldName =="Winery") {
            rec.setValue("Winery",lineEdit().at(indexOf("WineryId"))->text().toInt());
            continue;}

        if (fieldName =="Appellation") {
            rec.setValue("Appellation",lineEdit().at(indexOf("AppellationId"))->text().toInt());
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
    else {
        // Send WineType Event if wine type is changed
        if (wineTypeId() != initialWineType()) {
            // Create Event and sent it
            WineTypeEvent *event = new WineTypeEvent(wineTypeId(),rec.value("Id").toInt());
            event->sendEvent();
        }
        wineModel()->setRecord(wineModel()->rowPosition(rec.value("Id").toInt()),rec);}

    wineModel()->submitAll();
    accept();
}

void WineDialog::on_wineryButton_clicked()
{
    WineryQueryDialog * dialog = new WineryQueryDialog(lineEdit().at(indexOf("Winery"))->text(),wineModel()->database());
     if (dialog->exec() == QDialog::Accepted) {
         lineEdit().at(indexOf("WineryId"))->setText(QString::number(dialog->selectedId()));
         lineEdit().at(indexOf("Winery"))->setText(dialog->selectedName());}
     dialog->deleteLater();
}

void WineDialog::on_wineryLineEdit_textEdited(const QString &text)
{
    bool fUnique;
    int dId = wineryId(text,fUnique);
    if (fUnique)
        lineEdit().at(indexOf("WineryId"))->setText(QString::number(dId));
    else
        lineEdit().at(indexOf("WineryId"))->clear();
}

void WineDialog::on_appellationButton_clicked()
{
    AppellationQueryDialog *dialog = new AppellationQueryDialog(combo().at(indexOf("Appellation"))->currentText(),wineModel()->database());
    if (dialog->exec() == QDialog::Accepted) {
       int aId = dialog->selectedId();
       setCombosFromAppellationId(aId);
       lineEdit().at(indexOf("AppellationId"))->setText(QString::number(aId));
    }
    dialog->deleteLater();
}

void WineDialog::on_appellationComboBox_activated(int index)
{
    if (index!=-1) {
        int aId = appellationId();
        if (aId !=0)
            lineEdit().at(indexOf("AppellationId"))->setText(QString::number(aId));
        else
            lineEdit().at(indexOf("AppellationId"))->clear();
        setAppellationFields(aId);
    }
}

void WineDialog::on_regionComboBox_activated(int index)
{
    if (index !=-1)
       // Populate Appellation Combo according to Region and Wine Type
        populateAppellationCombo(wineTypeId(),index);
}

void WineDialog::on_typeComboBox_activated(int index)
{
    if (index !=-1)
        // Populate Appellation Combo according to Region and Wine Type
        populateAppellationCombo(index,regionId());
}

void WineDialog::on_appellationIdLineEdit_textChanged(const QString &text)
{
       setActionButtonEnabled();
    Q_UNUSED(text)
}

void WineDialog::on_wineryIdLineEdit_textChanged(const QString &text)
{
        setActionButtonEnabled();
        Q_UNUSED(text)
}

void WineDialog::on_grapeVarietyLineEdit_textChanged(const QString &text)
{
    QString decodedText;
    if (!text.isEmpty()) {
        QSqlQuery query;
        query.prepare(QString("SELECT Variety FROM GrapeVariety WHERE Id IN %1").arg(text));
        query.exec();
        query.first();
        decodedText = query.value("Variety").toString();
        while (query.next())
            decodedText.append(", " + query.value("Variety").toString());
    }
    textEdit().at(indexOf("Variety"))->setPlainText(decodedText);
}

void WineDialog::on_grapeVarietyButton_clicked()
{
    GrapeVarietySelectionDialog *dialog = new GrapeVarietySelectionDialog(wineModel()->database(),lineEdit().at(indexOf("GrapeVariety"))->text());
    if (dialog->exec() == QDialog::Accepted) {
        QString str = dialog->getVarietyString();
        if (str != lineEdit().at(indexOf("GrapeVariety"))->text())
            lineEdit().at(indexOf("GrapeVariety"))->setText(str);
    }
}

void WineDialog::setActionButtonEnabled()
{
    QPushButton *button = buttonBox()->findChild<QPushButton *>("actionButton");
    button->setEnabled(!lineEdit().at(indexOf("AppellationId"))->text().isEmpty() && !lineEdit().at(indexOf("WineryId"))->text().isEmpty());
}

void WineDialog::setCombosFromAppellationId(const int &AppellationId)
{
    int wineTypeId =0;
    int regionId=0;
    QString AppellationStr;

    if (AppellationId > 0) {
        // Find Wine Type and Region from Appellation
        QSqlQuery query;
        query.prepare(QString("SELECT Appellation, Region, Colour FROM Appellation WHERE Id = %1").arg(AppellationId));
        query.exec();

        if (query.first()) {
            wineTypeId = query.value("Colour").toInt();
            regionId = query.value("Region").toInt();
            AppellationStr = query.value("Appellation").toString();
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

    populateAppellationCombo(wineTypeId,regionId);
    if (!AppellationStr.isEmpty()) {
        combo().at(indexOf("Appellation"))->setCurrentText(AppellationStr);
        setAppellationFields(AppellationId);}

    setActionButtonEnabled();
}

int WineDialog::initialWineType() const
{
    return m_initialWineType;
}

void WineDialog::setInitialWineType(int initialWineType)
{
    m_initialWineType = initialWineType;
}
