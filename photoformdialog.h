#ifndef PHOTOFORMDIALOG_H
#define PHOTOFORMDIALOG_H

#include "abstractmodelformdialog.h"

#include <QLabel>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>

class PhotoFormDialog : public AbstractModelFormDialog
{
    Q_OBJECT
public:
    PhotoFormDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    QLabel *photo() const;
    void setPhoto();

    void loadPhotoFromFile(const QString &text);
    void changePhotoFile();

 // void replyFinished(QNetworkReply *reply);

     bool eventFilter(QObject *watched, QEvent *event);

protected slots:
     void on_labelImageLineEdit_textChanged(const QString &text);

protected:
    QLabel *m_photo;
};

#endif // PHOTOFORMDIALOG_H
