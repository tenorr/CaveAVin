#ifndef STORAGETABLEMODEL_H
#define STORAGETABLEMODEL_H


#include "abstractrectmodel.h"

class StorageTableModel : public  AbstractRectModel
{
    Q_OBJECT
public:
    StorageTableModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());

    static QRect decodeToQRect(const QString &text, bool fNull=false);
    static QString encodeFromQRect(const QRect rect);

};

#endif // STORAGETABLEMODEL_H
