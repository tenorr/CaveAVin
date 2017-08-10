#ifndef ABSTRACTMODELFORMDIALOG_H
#define ABSTRACTMODELFORMDIALOG_H

#include "abstractformdialog.h"
#include "abstractwinetablemodel.h"
#include <QSqlField>



class AbstractModelFormDialog : public AbstractFormDialog
{
public:
    AbstractModelFormDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    AbstractWineTableModel *model() const;
    void setModel(AbstractWineTableModel *model);

    void populateCombo(const QString &comboName, const QString &fieldName, const QString &tableName, const QString &filter=QString());

    virtual void setInitialData(int id);

protected:
    AbstractWineTableModel *m_model;
};

#endif // ABSTRACTMODELFORMDIALOG_H
