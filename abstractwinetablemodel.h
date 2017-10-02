#ifndef ABSTRACTWINETABLEMODEL_H
#define ABSTRACTWINETABLEMODEL_H

#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include <QSqlQuery>


class AbstractWineTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    AbstractWineTableModel(QString tableName,QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());

    int newId();
    int rowPosition(int id);
};

#endif // ABSTRACTWINETABLEMODEL_H
