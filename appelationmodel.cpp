#include "appelationmodel.h"
#include <QHeaderView>
#include <QDebug>

AppelationModel::AppelationModel(QObject *parent, QSqlDatabase db)
 : AbstractWineTableModel("Appelation",parent,db)
{
    setRelation(fieldIndex("Region"),QSqlRelation("Region","Id","Region"));
    setRelation(fieldIndex("Type"),QSqlRelation("Appelation_Type","Id",tr("Type")));
    setWineTypeModel(new AbstractWineTableModel("Wine_Type",parent,db));
}

void AppelationModel::selectRecords(const QString &text, int regionId, int appelationTypeId, int wineTypeId)
{
    bool fFirst = text.isEmpty();
    QString str = (text.isEmpty())? QString() : QString("Appelation LIKE '%%1%'").arg(text);
    if (regionId !=0) {
       if (!fFirst)
           str.append(" AND ");
       str.append(QString("Appelation.Region = %1").arg(regionId));
       fFirst = false;
    }
    if (appelationTypeId !=0) {
       if (!fFirst)
           str.append(" AND ");
       str.append(QString("Appelation.Type = %1").arg(appelationTypeId));
       fFirst = false;
    }
    if (wineTypeId !=0) {
       if (!fFirst)
           str.append(" AND ");
       str.append(QString("Appelation.Couleur = %1").arg(wineTypeId));
       fFirst = false;
    }
    setFilter(str);
}


AbstractWineTableModel *AppelationModel::wineTypeModel() const
{
    return m_wineTypeModel;
}

void AppelationModel::setWineTypeModel(AbstractWineTableModel *wineTypeModel)
{
    m_wineTypeModel = wineTypeModel;
}

int AppelationModel::fieldIndex(const QString &fieldName) const
{
    if (fieldName == "Color")
        return columnCount()-1;
    return AbstractWineTableModel::fieldIndex(fieldName);
}

 QVariant AppelationModel::data(const QModelIndex &item, int role) const
{
   if (headerData(item.column(),Qt::Horizontal).toString() == tr("Color")) {
       if (role == Qt::DisplayRole) {
      // Find the wine Type/
      int wineType = record(item.row()).value("Couleur").toInt();
      if (wineType >0)
        return wineTypeModel()->record(wineType-1).value("Type");}
        }
     return QSqlQueryModel::data(item, role);
 }

 int AppelationModel::columnCount(const QModelIndex &parent) const
 {
     // Add a new Column
     return QSqlRelationalTableModel::columnCount(parent)+1;
 }

 QVariant AppelationModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
     // Add new column Header
     if ((section == columnCount()-1) && (orientation==Qt::Horizontal)) {
         if (role == Qt::DisplayRole)
            return QVariant(QString("Color"));
     }
     return AbstractWineTableModel::headerData(section,orientation,role);
 }

 Qt::ItemFlags AppelationModel::flags(const QModelIndex &item) const
 {
     if (headerData(item.column(),Qt::Horizontal).toString() == tr("Color")) {
         if (index(0,0).isValid())
            return AbstractWineTableModel::flags(index(0,0));
     }
     return AbstractWineTableModel::flags(item);
 }
