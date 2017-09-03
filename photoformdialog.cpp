#include "photoformdialog.h"


PhotoFormDialog::PhotoFormDialog(QWidget *parent, Qt::WindowFlags f)
    :AbstractModelFormDialog (parent,f)
{

}

QLabel *PhotoFormDialog::photo() const
{
    return m_photo;
}

void PhotoFormDialog::setPhoto()
{
    m_photo = findChild<QLabel *>("photoLabel");
    m_photo->installEventFilter(this);
}

void PhotoFormDialog::loadPhotoFromFile(const QString &text)
{
    // Check if file exists and format supported and put into the photo Label
   QPixmap pixmap;
   if (pixmap.load(text)) {
       pixmap = pixmap.scaled(photo()->width(),photo()->height(),Qt::KeepAspectRatio);
       photo()->setPixmap(pixmap);
   }
}

void PhotoFormDialog::changePhotoFile()
{
    QString currentFileName =  lineEdit().at(indexOf("LabelImage"))->text();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     currentFileName,
                                                     tr("Images (*.png *.xpm *.jpg)"),nullptr,QFileDialog::DontResolveSymlinks);

    if (!fileName.isEmpty() && (fileName != currentFileName))
        lineEdit().at(indexOf("LabelImage"))->setText(fileName);
}

bool PhotoFormDialog::eventFilter(QObject *watched, QEvent *event)
{
        if (watched == photo()) {
            if (event->type() == QEvent::KeyPress) {
                QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                if (keyEvent->key() == Qt::Key_Delete) {
                    QMessageBox msgBox;
                    msgBox.setText(tr("The photo is about to be removed."));
                    msgBox.setInformativeText(tr("Do you want to proceed?"));
                    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    if (msgBox.exec() == QMessageBox::Ok) {
                        lineEdit().at(indexOf("LabelImage"))->clear();
                        return true;
                    }
                    else
                        return false;
               }
            }
            if (event->type() == QEvent::MouseButtonDblClick) {
                 changePhotoFile();
            }
        }
        // pass the event on to the parent class
        return AbstractFormDialog::eventFilter(watched, event);
}

void PhotoFormDialog::on_labelImageLineEdit_textChanged(const QString &text)
{
    photo()->clear();
    if (!text.isEmpty()) {
    if (!text.contains("http"))
         loadPhotoFromFile(text);
    }
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
