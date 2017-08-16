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

    int appelationId();
    QString appelationStr(bool fQuery=true);
    int regionId();
    int wineTypeId();
    int domaineId(const QString &text, bool &fUnique);


protected:
    virtual void setInitialData(int id);

private slots:
    void doAction();
    void on_domaineButton_clicked();
    void on_domaineLineEdit_textEdited(const QString &text);
    void on_appelationButton_clicked();
    void on_appelationComboBox_activated(int index);
    void on_regionComboBox_activated(int index);
    void on_typeComboBox_activated(int index);
    void on_appelationIdLineEdit_textChanged(const QString &text);
    void on_domaineIdLineEdit_textChanged(const QString &text);

private:
    WineModel *wineModel() const;
    void populateAppelationCombo(int wineTypeId=0, int regionId=0);
    void setAppellationFields(int appelationId=0);
    void setActionButtonEnabled();

};

#endif // WINEDIALOG_H
