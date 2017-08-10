#include "containertablemodel.h"

ContainerTableModel::ContainerTableModel(QObject *parent, QSqlDatabase db)
     : AbstractRectModel("Container",parent,db)
{
    setParentName("Room");
}
