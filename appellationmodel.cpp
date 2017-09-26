#include "appellationmodel.h"
#include <QHeaderView>
#include <QDebug>

AppellationModel::AppellationModel(QObject *parent, QSqlDatabase db)
 : AbstractWineTableModel("Appellation",parent,db)
{
    setRelation(fieldIndex("Region"),QSqlRelation("Region","Id","Region"));
    setRelation(fieldIndex("Type"),QSqlRelation("AppellationType","Id",tr("Type")));
    setWineTypeModel(new AbstractWineTableModel("WineType",parent,db));
}

void AppellationModel::selectRecords(const QString &text, int regionId, int appellationTypeId, int wineTypeId)
{
    bool fFirst = text.isEmpty();
    QString str = (text.isEmpty())? QString() : QString("Appellation LIKE '%%1%'").arg(text);
    if (regionId !=0) {
       if (!fFirst)
           str.append(" AND ");
       str.append(QString("Appellation.Region = %1").arg(regionId));
       fFirst = false;
    }
    if (appellationTypeId !=0) {
       if (!fFirst)
           str.append(" AND ");
       str.append(QString("Appellation.Type = %1").arg(appellationTypeId));
       fFirst = false;
    }
    if (wineTypeId !=0) {
       if (!fFirst)
           str.append(" AND ");
       str.append(QString("Appellation.Colour = %1").arg(wineTypeId));
       fFirst = false;
    }
    setFilter(str);
}


AbstractWineTableModel *AppellationModel::wineTypeModel() const
{
    return m_wineTypeModel;
}

void AppellationModel::setWineTypeModel(AbstractWineTableModel *wineTypeModel)
{
    m_wineTypeModel = wineTypeModel;
}

int AppellationModel::fieldIndex(const QString &fieldName) const
{
    if (fieldName == "Color")
        return columnCount()-1;
    return AbstractWineTableModel::fieldIndex(fieldName);
}

 QVariant AppellationModel::data(const QModelIndex &item, int role) const
{
   if (headerData(item.column(),Qt::Horizontal).toString() == tr("Color")) {
       if (role == Qt::DisplayRole) {
      // Find the wine Type/
      int wineType = record(item.row()).value("Colour").toInt();
      if (wineType >0)
        return wineTypeModel()->record(wineType-1).value("Type");}
        }
     return QSqlQueryModel::data(item, role);
 }

 int AppellationModel::columnCount(const QModelIndex &parent) const
 {
     // Add a new Column
     return QSqlRelationalTableModel::columnCount(parent)+1;
 }

 QVariant AppellationModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
     // Add new column Header
     if ((section == columnCount()-1) && (orientation==Qt::Horizontal)) {
         if (role == Qt::DisplayRole)
            return QVariant(QString("Color"));
     }
     return AbstractWineTableModel::headerData(section,orientation,role);
 }

 Qt::ItemFlags AppellationModel::flags(const QModelIndex &item) const
 {
     if (headerData(item.column(),Qt::Horizontal).toString() == tr("Color")) {
         if (index(0,0).isValid())
            return AbstractWineTableModel::flags(index(0,0));
     }
     return AbstractWineTableModel::flags(item);
 }
