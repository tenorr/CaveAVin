#ifndef WINETYPEDELEGATE_H
#define WINETYPEDELEGATE_H

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QSqlTableModel>
#include <QSqlRecord>


class WineTypeDelegate : public QStyledItemDelegate
{
public:
    WineTypeDelegate(QObject *parent = Q_NULLPTR);

protected:
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;


};

#endif // WINETYPEDELEGATE_H
