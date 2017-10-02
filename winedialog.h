#ifndef WINEDIALOG_H
#define WINEDIALOG_H

#include "wineryquerydialog.h"
#include "appellationquerydialog.h"
#include "regionquerydialog.h"
#include "winemodel.h"
#include "grapevarietyselectiondialog.h"
#include "winetypeevent.h"
#include "photoformdialog.h"
#include "wineprizemodel.h"

#include <QSqlQuery>


class WineDialog : public PhotoFormDialog
{
    Q_OBJECT
public:
    WineDialog(QSqlDatabase db, int selectedId=-1, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    int wineId();
    int appellationId();
    QString appellationStr(bool fQuery=true);
    int regionId();
    int wineTypeId();
    int wineryId(const QString &text, bool &fUnique);

    WinePrizeModel *prizeModel() const;
    void setPrizeModel(WinePrizeModel *prizeModel);

protected:
    virtual void setInitialData(int id);

private slots:
    void doAction();
    void on_wineryButton_clicked();
    void on_wineryLineEdit_textEdited(const QString &text);
    void on_appellationButton_clicked();
    void on_appellationComboBox_activated(int index);
    void on_regionComboBox_activated(int index);
    void on_typeComboBox_activated(int index);
    void on_appellationIdLineEdit_textChanged(const QString &text);
    void on_wineryIdLineEdit_textChanged(const QString &text);
    void on_grapeVarietyLineEdit_textChanged(const QString &text);
    void on_grapeVarietyButton_clicked();
    void on_addPrizeButton_clicked();
    void on_deletePrizeButton_clicked();

private:
    WineModel *wineModel();
    int initialWineType() const;
    void setInitialWineType(int initialWineType);
    void populateAppellationCombo(int wineTypeId=0, int regionId=0);
    void setAppellationFields(int appellationId=0);
    void setActionButtonEnabled();
    void setCombosFromAppellationId(const int & appellationId);
    QTableView * prizeView();


private:
    int m_initialWineType;
    WinePrizeModel * m_prizeModel;
};

#endif // WINEDIALOG_H
