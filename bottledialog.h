#ifndef BOTTLEDIALOG_H
#define BOTTLEDIALOG_H

#include "wineryquerydialog.h"
#include "winequerydialog.h"
#include "photoformdialog.h"
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFileDialog>
#include <QMouseEvent>
#include <QMessageBox>

//#include <QNetworkAccessManager>
//#include <QNetworkReply>


class BottleDialog : public PhotoFormDialog
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
    void setInitialData(QSqlRecord rec);
    void setEnabledActionButton(bool fEnabled);
    void setWineIdFields();
    void findWineId();

private slots:
    void on_purchaseDateClearButton_clicked();
    void on_purchaseDateTodayButton_clicked();
    void on_wineryButton_clicked();
    void on_wineNameButton_clicked();
    void doAction(QAbstractButton *button);
    void on_wineIdLineEdit_textChanged(const QString &text);
    void on_wineryLineEdit_textChanged(const QString &text);
    void on_wineNameLineEdit_textChanged(const QString &text);
    void on_bottleTypeComboBox_currentIndexChanged(int index);

private:
    QSqlDatabase m_db;
    int m_wineId;
    int m_initialWineType;
    bool fConnectWineId;
};

#endif // BOTTLEDIALOG_H
