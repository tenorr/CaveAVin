#ifndef BOTTLEDIALOG_H
#define BOTTLEDIALOG_H

#include "domainequerydialog.h"
#include "winequerydialog.h"
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFileDialog>
#include <QMouseEvent>

//#include <QNetworkAccessManager>
//#include <QNetworkReply>


class BottleDialog : public AbstractFormDialog
{
    Q_OBJECT
public:
    BottleDialog(QSqlDatabase db, QSqlRecord rec, int iProcess=0,QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    QSqlDatabase db() const;
    void setDb(const QSqlDatabase &db);

    int wineId() const;
    void setWineId(int wineId);

    int index(const QString &text);

    int wineType();

private:
    QLabel *photo() const;
    void setPhoto();

    void setInitialData(QSqlRecord rec);
    void setEnabledActionButton(bool fEnabled);
    void setWineIdFields();
    void findWineId();
    void loadPhotoFromFile(const QString &text);
    void changePhotoFile();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void on_purchaseDateClearButton_clicked();
    void on_purchaseDateTodayButton_clicked();
    void on_wineryButton_clicked();
    void on_wineNameButton_clicked();
    void doAction(QAbstractButton *button);
    void on_wineIdLineEdit_textChanged(const QString &text);
    void on_wineryLineEdit_textChanged(const QString &text);
    void on_wineNameLineEdit_textChanged(const QString &text);
    void on_labelImageLineEdit_textChanged(const QString &text);
    // void replyFinished(QNetworkReply *reply);

private:
    QSqlDatabase m_db;
    int m_wineId;
    QLabel *m_photo;
    bool fConnectWineId;
};

#endif // BOTTLEDIALOG_H
