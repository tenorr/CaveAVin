#include "abstractrectmodel.h"

AbstractRectModel::AbstractRectModel(QString tableName, QObject *parent, QSqlDatabase db)
    : AbstractWineTableModel(tableName,parent,db)
{

}

QSqlRecord AbstractRectModel::createNew(int parentId, QPointF pos)
{
    int id = newId();

    // Create new Record
    QSqlRecord rec = record();
    rec.setValue("Id",id);
    rec.setValue(tableName()+"_Name",tableName()+tr(" #")+QString::number(id));
    rec.setValue(parentName(),parentId);
    rec.setValue("XPos",pos.x());
    rec.setValue("YPos",pos.y());
    rec.setValue("Width",40);
    rec.setValue("Height",40);
    rec.setValue("Red",0);
    rec.setValue("Green",0);
    rec.setValue("Blue",0);
    rec.setValue("BrushStyle",0);
    insertRecord(-1,rec);
    submitAll();
    return rec;
}

bool AbstractRectModel::deleteRecordId(int id)
{
    // Find row of the record
    int row = rowPosition(id);

    if (row !=-1) {
      // Delete row
        removeRows(row,1);
        submitAll();
        return true;
    }
    else return false;
}

void AbstractRectModel::changeColor(QColor color, int id)
{
    // Change RGB data into Database
    int row = rowPosition(id);

    if (row !=-1) {
    QSqlRecord rec = record(row);
    rec.setValue("Red",color.red());
    rec.setValue("Green",color.green());
    rec.setValue("Blue",color.blue());
    setRecord(row,rec);
    submitAll();
    }
}

void AbstractRectModel::changeBrushStyle(Qt::BrushStyle bs, int id)
{
    // Change BrushStyle data into Database
    int row = rowPosition(id);

    if (row !=-1) {
    QSqlRecord rec = record(row);
    rec.setValue("BrushSTyle",(int) bs);
    setRecord(row,rec);
    submitAll();
    }
}

void AbstractRectModel::changeRectangleData(QRect data, int id)
{
    // Change Rectangle Data
    int row = rowPosition(id);

    if (row !=-1) {
    QSqlRecord rec = record(row);
    rec.setValue("XPos",data.x());
    rec.setValue("YPos",data.y());
    rec.setValue("Width",data.width());
    rec.setValue("Height",data.height());
    setRecord(row,rec);
    submitAll();
    }
}

QString AbstractRectModel::parentName() const
{
    return m_parentName;
}

void AbstractRectModel::setParentName(const QString &parentName)
{
    m_parentName = parentName;
}
