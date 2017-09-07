#include "storagetablemodel.h"

StorageTableModel::StorageTableModel(QObject *parent, QSqlDatabase db)
     : AbstractRectModel("Storage",parent,db)
{
    setParentName("Cellar");
}

QRect StorageTableModel::decodeToQRect(const QString &text, bool fNull)
{
    // Decode QRect(x,y,w,h) or QRect(0,0,w,h) according to fNull boolean
    QRect rect;
    QString str = text;
    int n;
    if (fNull) {
        rect.setX(0);
        rect.setY(0);
    }
    else {
        str.remove(0,str.indexOf("(")+1);
        int n = (str.left(str.indexOf(","))).toInt();
        rect.setX(n);
        str.remove(0,str.indexOf(",")+1);
        n = (str.left(str.indexOf(" "))).toInt();
        rect.setY(n);
    }
    str.remove(0,str.indexOf(" ")+1);
    n = (str.left(str.indexOf("x"))).toInt();
    rect.setWidth(n);
    str.remove(0,str.indexOf("x")+1);
    n = (str.left(str.indexOf(")"))).toInt();
    rect.setHeight(n);
    return rect;
}

QString StorageTableModel::encodeFromQRect(const QRect rect)
{
    QString str("QRect(");
    str.append(QString::number(rect.x()));
    str.append(",");
    str.append(QString::number(rect.y()));
    str.append(" ");
    str.append(QString::number(rect.width()));
    str.append("x");
    str.append(QString::number(rect.height()));
    str.append(")");
    return str;
}
