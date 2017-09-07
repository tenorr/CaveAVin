#ifndef GRAPEVARIETYSELECTIONDIALOG_H
#define GRAPEVARIETYSELECTIONDIALOG_H

#include "abstractmodelformdialog.h"
#include <QHeaderView>
#include <QMenu>


class GrapeVarietySelectionDialog : public AbstractFormDialog
{
    Q_OBJECT
public:
    GrapeVarietySelectionDialog(QSqlDatabase db, QString varietyStr , QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    QString getVarietyString();

private:
    void setInitialData(QSqlDatabase db, QString varietyList);
    void setTableWidgetAttributes();
    int activeTableWidgetIndex();
    void removeFavorite(int row);
    void addFavorite(int globalRow);

private slots:
    void on_favoriteTableWidget_itemSelectionChanged();
    void on_globalTableWidget_itemSelectionChanged();
    void on_queryTableWidget_itemSelectionChanged();
    void on_favoriteTableWidget_customContextMenuRequested(const QPoint &pos);
    void on_globalTableWidget_customContextMenuRequested(const QPoint &pos);
    void on_queryTableWidget_customContextMenuRequested(const QPoint &pos);
    void on_lineEdit_textEdited(const QString &text);
    void on_tabWidget_currentChanged(int index);

private:
    bool fSelectionActive;

};

#endif // GRAPEVARIETYSELECTIONDIALOG_H
