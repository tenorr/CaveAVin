#ifndef APPELATIONMODEL_H
#define APPELATIONMODEL_H

#include "abstractwinetablemodel.h"


class AppelationModel : public AbstractWineTableModel
{
public:
    AppelationModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());
    void selectRecords(const QString &text, int regionId=0, int appelationTypeId=0, int wineTypeId=0);
    AbstractWineTableModel *wineTypeModel() const;
    void setWineTypeModel(AbstractWineTableModel *wineTypeModel);
    int fieldIndex(const QString &fieldName) const;
    int columnCount(const QModelIndex &parent= QModelIndex()) const;

protected:
    QVariant data(const QModelIndex &item, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
     AbstractWineTableModel *m_wineTypeModel;
};

#endif // APPELATIONMODEL_H
