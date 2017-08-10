#ifndef ABSTRACTRECTMODEL_H
#define ABSTRACTRECTMODEL_H

#include "abstractwinetablemodel.h"

#include <QColor>
#include <QRect>

class AbstractRectModel : public AbstractWineTableModel
{
        Q_OBJECT
public:
    AbstractRectModel(QString tableName, QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());

     QSqlRecord createNew(int parentId, QPointF pos);
     bool deleteRecordId(int id);

     QString parentName() const;
     void setParentName(const QString &parentName);

public slots:
     void changeColor(QColor color, int id);
     void changeBrushStyle(Qt::BrushStyle bs, int id);
     void changeRectangleData(QRect data, int id);

private:
    QString m_parentName;

};

#endif // ABSTRACTRECTMODEL_H
