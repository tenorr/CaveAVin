#include "abstractwinetablemodel.h"

AbstractWineTableModel::AbstractWineTableModel(QString tableName, QObject *parent, QSqlDatabase db)
    :QSqlRelationalTableModel(parent,db)
{
    setTable(tableName);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();
}

int AbstractWineTableModel::newId()
{
    QSqlRecord rec = record(rowCount()-1);
    return rec.value("Id").toInt()+1;
}

int AbstractWineTableModel::rowPosition(int id)
{
   for (int i=0, n=rowCount();i<n;i++) {
            if (record(i).value("Id").toInt() == id)
                return i;
        }
   return -1;
}
