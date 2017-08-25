#include "bottledialog.h"
#include <QDebug>


BottleDialog::BottleDialog(QSqlDatabase db, QSqlRecord rec, int iProcess, QWidget *parent, Qt::WindowFlags f)
    :AbstractFormDialog(parent, f)
{
    // Create Title and minimum width
    setWindowTitle(tr("Bottle Information"));
    setDb(db);

    setForm(":/form/bottleDialog.ui");
    setInitialData(rec);

    lineEdit().at(indexOf("DomaineId"))->hide();
    lineEdit().at(indexOf("LabelImage"))->hide();

    // Create Dialog Buttons
    QString processText =(iProcess ==0)? tr("Create") : tr("Update");
    QPushButton *button = new QPushButton(processText);
    button->setObjectName("actionButton");
    buttonBox()->addButton(button,QDialogButtonBox::ActionRole);
    connect(buttonBox(), &QDialogButtonBox::rejected, this, &BottleDialog::reject);
    connect(buttonBox(),SIGNAL(clicked(QAbstractButton*)),this,SLOT(doAction(QAbstractButton*)));
}

QSqlDatabase BottleDialog::db() const
{
    return m_db;
}

void BottleDialog::setDb(const QSqlDatabase &db)
{
    m_db = db;
}


void BottleDialog::on_purchaseDateClearButton_clicked()
{
    dateEdit().at(indexOf("PurchaseDate"))->setDate(dateEdit().at(indexOf("PurchaseDate"))->minimumDate());
}

void BottleDialog::on_purchaseDateTodayButton_clicked()
{
    dateEdit().at(indexOf("PurchaseDate"))->setDate(QDate::currentDate());
}

void BottleDialog::on_wineryButton_clicked()
{
    DomaineQueryDialog *dialog = new DomaineQueryDialog(lineEdit().at(indexOf("Winery"))->text(),db());
    if (dialog->exec()== QDialog::Accepted) {
        // Retrieve selected item and update line Edit
        QString str = dialog->selectedName();
        if (!str.isEmpty())
            lineEdit().at(indexOf("Winery"))->setText(str);
    }
    dialog->deleteLater();
}

void BottleDialog::on_wineNameButton_clicked()
{
    WineQueryDialog *dialog = new WineQueryDialog(lineEdit().at(indexOf("Winery"))->text(),lineEdit().at(indexOf("WineName"))->text(),db());
    if (dialog->exec()== QDialog::Accepted) {
        // Retrieve selected item and update line Edits and Combo
        QString str = dialog->selectedName();
        if (!str.isEmpty()) {
            fConnectWineId = false;
            lineEdit().at(indexOf("WineName"))->setText(str);
            lineEdit().at(indexOf("Winery"))->setText(dialog->selectedStringField("Domaine"));
            lineEdit().at(indexOf("WineId"))->setText(dialog->selectedStringField("Id"));
        // Retrieve appelation
            QSqlQuery query;
            query.prepare(QString("SELECT a.Appelation, t.type FROM Appelation AS a "
                                  "INNER JOIN Appelation_Type AS t ON a.Type = t.Id WHERE a.ID = %1").arg(dialog->selectedIntField("Appelation")));
            query.exec();
            if (query.first()) {
                lineEdit().at(indexOf("Appelation"))->setText(query.value("Appelation").toString());
                lineEdit().at(indexOf("AppelationType"))->setText(query.value("Type").toString());}
            fConnectWineId = true;
        }
    }
    dialog->deleteLater();
}

void BottleDialog::doAction(QAbstractButton *button)
{
   if (button->objectName() != "actionButton")
        return;
    accept();
}

void BottleDialog::on_wineIdLineEdit_textChanged(const QString &text)
{
   setWineId((text.isEmpty())? 0:text.toInt());
   bool fActionEnabled = !text.isEmpty();

   setEnabledActionButton(fActionEnabled);
   setWineIdFields();
}

void BottleDialog::on_wineryLineEdit_textChanged(const QString &text)
{
    if (fConnectWineId)
        findWineId();
    Q_UNUSED(text)
}

void BottleDialog::on_wineNameLineEdit_textChanged(const QString &text)
{
    if (fConnectWineId)
        findWineId();
    Q_UNUSED(text)
}

void BottleDialog::on_labelImageLineEdit_textChanged(const QString &text)
{
    photo()->clear();
    if (!text.contains("http"))
         loadPhotoFromFile(text);
   /*
    * For http use following code
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager,&QNetworkAccessManager::finished,this,&BottleDialog::replyFinished);
    QUrl url(text);
    QNetworkRequest request;
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::SslV3);
    request.setSslConfiguration(config);
    request.setUrl(url);
    manager->get(request);
*/

}

/*
void BottleDialog::replyFinished(QNetworkReply *reply)
{
    QByteArray jpegData = reply->readAll();
    QPixmap pixmap;
    qDebug() << pixmap.loadFromData(jpegData);
    QLabel *label = findChild<QLabel *>("photoLabel");
    label->setPixmap(pixmap);
}
*/
QLabel *BottleDialog::photo() const
{
    return m_photo;
}

void BottleDialog::setPhoto()
{
    m_photo = findChild<QLabel *>("photoLabel");
}

int BottleDialog::wineId() const
{
    return m_wineId;
}

void BottleDialog::setWineId(int wineId)
{
    m_wineId = wineId;
}

int BottleDialog::index(const QString &text)
{
    return indexOf(text);
}

int BottleDialog::wineType()
{
    int wType=0;
    QSqlQuery query;
    query.prepare(QString("SELECT Type FROM Wine WHERE Id = %1").arg(wineId()));
    query.exec();
    if (query.first())
        wType = query.value("Type").toInt();
    return wType;
}

void BottleDialog::setInitialData(QSqlRecord rec)
{
    fConnectWineId = false;

    //Initialise Widgets
    lineEdit().at(indexOf("Bottle"))->setText(QString::number(rec.value("Id").toInt()));
    lineEdit().at(indexOf("PurchaseLocation"))->setText(rec.value("Purchase_Location").toString());
    doubleSpinBox().at(indexOf("PurchasePrice"))->setValue(rec.value("Purchase_Price").toDouble());
    dateEdit().at(indexOf("PurchaseDate"))->setDate((rec.value("Purchase_Date").toDateTime()).date());

    // Set Photo Label and load image if exist
    setPhoto();
    lineEdit().at(indexOf("LabelImage"))->setText(rec.value("LabelImage").toString());

     // Initialise Millesime Combo
     // Find current Date and build from today downto 1900 years

    int yearIndex = QDate::currentDate().year();
    QStringList yearList;
    yearList << "";
    while (yearIndex>=1900)
        yearList.append(QString::number(yearIndex--));
    combo().at(indexOf("Millesime"))->addItems(yearList);
    int millesime = rec.value("Millesime").toInt();
    combo().at(indexOf("Millesime"))->setCurrentText((millesime==0)? "" : QString::number(millesime));

    // Set Data From Wine Table if any
    int wineId = rec.value("Wine").toInt();
    setWineId(wineId);

    if (wineId !=0) {
        QSqlQuery query;
        query.prepare(QString("SELECT w.Wine, d.Domaine FROM Wine AS w, Domaine AS d "
                              "WHERE (w.Domaine = d.Id) AND (w.Id = %1)").arg(wineId));
        query.exec();
        if (query.first()) {
            lineEdit().at(indexOf("Winery"))->setText(query.value("Domaine").toString());
            lineEdit().at(indexOf("WineName"))->setText(query.value("Wine").toString());
        }
        lineEdit().at(indexOf("WineId"))->setText(QString::number(wineId));
    }
    fConnectWineId = true;
}

void BottleDialog::setEnabledActionButton(bool fEnabled)
{
    QPushButton * button = buttonBox()->findChild<QPushButton *>("actionButton");
    if (button)
        button->setEnabled(fEnabled);
}

void BottleDialog::setWineIdFields()
{
    lineEdit().at(indexOf("Appelation"))->clear();
    lineEdit().at(indexOf("AppelationType"))->clear();
    lineEdit().at(indexOf("WineType"))->clear();
    if (wineId()!=0) {
        QSqlQuery query;
        query.prepare(QString("SELECT Type, Appelation FROM Wine WHERE ID = %1").arg(wineId()));
        query.exec();
        if (query.first()) {
            int typeId = query.value("Type").toInt();
            int appelationId = query.value("Appelation").toInt();
            if (typeId !=0) {
                query.prepare(QString("SELECT Type FROM Wine_Type WHERE Id = %1").arg(typeId));
                query.exec();
                if (query.first())
                    lineEdit().at(indexOf("WineType"))->setText(query.value("Type").toString());
            }
            if (appelationId !=0) {
                query.prepare(QString("SELECT a.Appelation, t.Type FROM Appelation AS a "
                                      "INNER JOIN Appelation_Type AS t ON a.Type = t.Id "
                                      "WHERE a.Id = %1").arg(appelationId));
                query.exec();
                if (query.first()) {
                    lineEdit().at(indexOf("Appelation"))->setText(query.value("Appelation").toString());
                    lineEdit().at(indexOf("AppelationType"))->setText(query.value("Type").toString());
                }
            }

        }
    }
}

void BottleDialog::findWineId()
{
    // Look for a unique wine Id
    QString wineryStr = lineEdit().at(indexOf("Winery"))->text();
    QString wineNameStr = lineEdit().at(indexOf("WineName"))->text();
    if ((wineryStr.isEmpty()) || wineNameStr.isEmpty()) {
        wineNameStr.append(wineryStr);
        setEnabledActionButton(wineNameStr.isEmpty());
        lineEdit().at(indexOf("WineId"))->clear();
        return;}

    QSqlQuery query;
    query.prepare(QString("SELECT w.Id FROM Wine AS w INNER JOIN Domaine AS d ON w.Domaine = d.Id "
                          "WHERE (w.Wine ='%1') AND (d.DOMAINE ='%2')").arg(wineNameStr,wineryStr));
    query.exec();
    if (query.first()) {
        int wId = query.value("Id").toInt();
        if (query.next())
             lineEdit().at(indexOf("WineId"))->clear();
        else
             lineEdit().at(indexOf("WineId"))->setText(QString::number(wId));
        }
    else {
         lineEdit().at(indexOf("WineId"))->clear();
    }
}

void BottleDialog::loadPhotoFromFile(QString const & text)
{
    // Check if file exists and format supported and put into the photo Label
   QPixmap pixmap;
   if (pixmap.load(text)) {
       pixmap = pixmap.scaled(photo()->width(),photo()->height(),Qt::KeepAspectRatio);
       photo()->setPixmap(pixmap);
   }
}

void BottleDialog::changePhotoFile()
{
    QString currentFileName =  lineEdit().at(indexOf("LabelImage"))->text();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     currentFileName,
                                                     tr("Images (*.png *.xpm *.jpg)"),nullptr,QFileDialog::DontResolveSymlinks);

    if (!fileName.isEmpty() && (fileName != currentFileName))
        lineEdit().at(indexOf("LabelImage"))->setText(fileName);
}

void BottleDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event) {
        // Check if event is above the photoLabel
        QRect rect = photo()->geometry();
        if (rect.contains(event->pos()))
            // If so change Photo
            changePhotoFile();
    }
}


