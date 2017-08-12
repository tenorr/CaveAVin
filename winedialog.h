#ifndef WINEDIALOG_H
#define WINEDIALOG_H

#include "abstractmodelformdialog.h"
#include "domainequerydialog.h"
#include "appelationquerydialog.h"
#include "regionquerydialog.h"
#include "winemodel.h"

#include <QSqlQuery>


class WineDialog : public AbstractModelFormDialog
{
    Q_OBJECT
public:
    WineDialog(QSqlDatabase db, int selectedId=-1, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    virtual void setInitialData(int id);    

private slots:
    void doAction(QAbstractButton *button);
    void on_domaineButton_clicked();
    void on_appelationButton_clicked();
    void on_appelationComboBox_activated(int index);
    void on_appelationComboBox_currentIndexChanged(int index);
    void on_regionComboBox_activated(int index);
    void on_typeComboBox_activated(int index);

private:
    WineModel *wineModel() const;
    int domaineId(const QString &text, bool &fUnique);
    void populateAppelationCombo(int wineTypeId=0);
    void setAppellationFields();

};

#endif // WINEDIALOG_H
