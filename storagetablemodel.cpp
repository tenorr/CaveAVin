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

QVector<int> StorageTableModel::disableItems(const QString &text)
{
    QVector<int> itemList;
    if (!text.isEmpty()) {
        QString str = text;

        // Delete brackets
        str.remove(0,str.indexOf("{")+1);
        str.chop(1);

        // Split the list and convert to integer
        QStringList strList = str.split(",");

        for (int i=0, n= strList.size(); i<n;i++) {
            itemList.append(strList.at(i).toInt());
        }
    }
    return itemList;
}

QString StorageTableModel::removeDisableItem(const QString & text,int item)
{
     QString str = text;
     // Delete brackets
     str.remove(0,str.indexOf("{")+1);
     str.chop(1);

     // Split the list and find the position of the item in the list
     QStringList itemList = str.split(",");
     for (int i=0 , n= itemList.size();i<n;i++) {
         if (itemList.at(i).toInt() == item) {
             itemList.removeAt(i);
             break;
         }
     }
     if (itemList.isEmpty())
         return  QString();

     return  "{"+itemList.join(",")+"}";
}

QString StorageTableModel::addDisableItem(const QString & text,int item)
{
     if (text.isEmpty())
         return "{"+QString::number(item)+ "}";

     QString str = text;  
     // Delete brackets
     str.remove(0,str.indexOf("{")+1);
     str.chop(1);

     // Split the list and find the position of the item in the list
     QStringList itemList = str.split(",");
     for (int i=0, n= itemList.size();i<n;i++) {
         if (itemList.at(i).toInt() > item) {
             itemList.insert(i, QString::number(item));
             break;
         }
         // Insert at last if not found
         if (i==n-1)
             itemList.append(QString::number(item));
     }
     // Join the list
     return  "{"+itemList.join(",")+"}";
}
