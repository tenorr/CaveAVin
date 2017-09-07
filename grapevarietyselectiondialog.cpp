#include "grapevarietyselectiondialog.h"
#include <QDebug>

GrapeVarietySelectionDialog::GrapeVarietySelectionDialog(QSqlDatabase db, QString varietyStr, QWidget *parent, Qt::WindowFlags f)
    :AbstractFormDialog(parent,f)
{
    setWindowTitle(tr("Select Grape Variety"));
    setForm(":/form/grapevarietyform.ui");

    fSelectionActive = false;
    setInitialData(db,varietyStr);
    fSelectionActive = true;

    // Connect dialog buttons box
    connect(buttonBox(), &QDialogButtonBox::accepted,this,&GrapeVarietySelectionDialog::accept);
    connect(buttonBox(), &QDialogButtonBox::rejected,this,&GrapeVarietySelectionDialog::reject);
}

QString GrapeVarietySelectionDialog::getVarietyString()
{
    QString str = QString();
    // Find selected items
    for (int i=0, n=tableWidget().at(indexOf("Global"))->rowCount();i<n;i++) {
        if (tableWidget().at(indexOf("Global"))->item(i,0)->isSelected())
            str.append(tableWidget().at(indexOf("Global"))->item(i,1)->text() +',');
    }
    // Add the parenthesis and delete the last coma if string not empty
    if (!str.isEmpty()) {
        str.prepend('(');
        str.chop(1);
        str.append(')');}
    return str;
}

void GrapeVarietySelectionDialog::setInitialData(QSqlDatabase db, QString varietyStr)
{
    // Set Table Widgets Id Column hidden
        tableWidget().at(indexOf("Favorite"))->setColumnHidden(1,true);
        tableWidget().at(indexOf("Global"))->setColumnHidden(1,true);
        tableWidget().at(indexOf("Query"))->setColumnHidden(1,true);

    // Remove then parenthesis characters
     varietyStr.chop(1);
     varietyStr.remove(0,1);

    QStringList varietyList = varietyStr.split(',');

    // Query Grape Variety Table
    QSqlQuery query(db);
    query.prepare(QString("SELECT * FROM GrapeVariety"));
    query.exec();

    int rowFavorite =0;
    int rowGlobal=0;

    // Fill table Widgets
    while (query.next()) {
        QTableWidgetItem *item = new QTableWidgetItem(query.value("Variety").toString());
        tableWidget().at(indexOf("Global"))->insertRow(rowGlobal);
        tableWidget().at(indexOf("Global"))->setItem(rowGlobal, 0,item);
        tableWidget().at(indexOf("Global"))->setItem(rowGlobal++, 1,
                                              new QTableWidgetItem(QString::number(query.value("Id").toInt())));
        if(query.value("Favorite").toBool()) {
            tableWidget().at(indexOf("Favorite"))->insertRow(rowFavorite);
            tableWidget().at(indexOf("Favorite"))->setItem(rowFavorite, 0, item->clone());
            tableWidget().at(indexOf("Favorite"))->setItem(rowFavorite++, 1,
                                                  new QTableWidgetItem(QString::number(query.value("Id").toInt())));
        }
    }

    QStringList list{"Favorite", "Global", "Query"};
    foreach (QString str, list) {

        // Set Sorting Enabled
        tableWidget().at(indexOf(str))->setSortingEnabled(true);
        tableWidget().at(indexOf(str))->sortByColumn(0,Qt::AscendingOrder);

        // Select the items
        int row = tableWidget().at(indexOf(str))->rowCount() -1;
        while (row >=0) {
            if (varietyList.contains(tableWidget().at(indexOf(str))->item(row,1)->text()))
                tableWidget().at(indexOf(str))->selectRow(row);
            row--;
        }

    // Give focus to tableWidget
         tableWidget().at(indexOf("Favorite"))->setFocus();

    // Set Global Tab if Favorite is empty
         if (tableWidget().at(indexOf("Favorite"))->rowCount() == 0) {
             QTabWidget *t = findChild<QTabWidget *>("tabWidget");
             t->setCurrentIndex(1);
             tableWidget().at(indexOf("Global"))->setFocus();
         }
    }
}

int GrapeVarietySelectionDialog::activeTableWidgetIndex()
{
    QTabWidget *tabWidget = findChild<QTabWidget *>("tabWidget");
    return tabWidget->currentIndex();
}

void GrapeVarietySelectionDialog::on_favoriteTableWidget_itemSelectionChanged()
{
    if ((fSelectionActive) && (activeTableWidgetIndex()==0)) {
        QString idStr;
        bool fSelected;
        for (int i=0, n= tableWidget().at(indexOf("Favorite"))->rowCount();i<n;i++) {
            // Retrieve Selection and Id
            fSelected = tableWidget().at(indexOf("Favorite"))->item(i,0)->isSelected();
            idStr = tableWidget().at(indexOf("Favorite"))->item(i,1)->text();
            int globalRow = tableWidget().at(indexOf("Global"))->findItems(idStr,Qt::MatchFixedString).at(0)->row();
            // Apply selected status
            tableWidget().at(indexOf("Global"))->item(globalRow,0)->setSelected(fSelected);
        }
    }
}

void GrapeVarietySelectionDialog::on_globalTableWidget_itemSelectionChanged()
{
    if ((fSelectionActive) && (activeTableWidgetIndex()==1)) {
        QString idStr;
        bool fSelected;
        for (int i=0, n= tableWidget().at(indexOf("Favorite"))->rowCount();i<n;i++) {
            // Retrieve Id and look to the selected Status
            idStr = tableWidget().at(indexOf("Favorite"))->item(i,1)->text();
            int globalRow = tableWidget().at(indexOf("Global"))->findItems(idStr,Qt::MatchFixedString).at(0)->row();
            fSelected = tableWidget().at(indexOf("Global"))->item(globalRow,0)->isSelected();

            // Apply selected status
            tableWidget().at(indexOf("Favorite"))->item(i,0)->setSelected(fSelected);
        }
    }
}

void GrapeVarietySelectionDialog::on_queryTableWidget_itemSelectionChanged()
{
    if ((fSelectionActive) && (activeTableWidgetIndex()==2)) {
        QString idStr;
        bool fSelected;
        for (int i=0, n= tableWidget().at(indexOf("Query"))->rowCount();i<n;i++) {
            // Retrieve Id and look to the selected Status
            idStr = tableWidget().at(indexOf("Query"))->item(i,1)->text();
            int globalRow = tableWidget().at(indexOf("Global"))->findItems(idStr,Qt::MatchFixedString).at(0)->row();
            fSelected = tableWidget().at(indexOf("Query"))->item(i,0)->isSelected();

            // Apply selected status to Global
            tableWidget().at(indexOf("Global"))->item(globalRow,0)->setSelected(fSelected);
            if (!tableWidget().at(indexOf("Favorite"))->findItems(idStr,Qt::MatchFixedString).isEmpty()) {
                int favoriteRow = tableWidget().at(indexOf("Favorite"))->findItems(idStr,Qt::MatchFixedString).at(0)->row();
                tableWidget().at(indexOf("Favorite"))->item(favoriteRow,0)->setSelected(fSelected);
            }
        }
    }
}

void GrapeVarietySelectionDialog::on_favoriteTableWidget_customContextMenuRequested(const QPoint &pos)
{
    // Retrieve row position
    int row = tableWidget().at(indexOf("Favorite"))->itemAt(pos)->row();

    // Show Menu and execute action
    QMenu menu(tableWidget().at(indexOf("Favorite")));
    menu.addAction(tr("Remove From Favorite"));

    if (menu.exec(tableWidget().at(indexOf("Favorite"))->mapToGlobal(pos)))
        removeFavorite(row);
}

void GrapeVarietySelectionDialog::on_globalTableWidget_customContextMenuRequested(const QPoint &pos)
{
    // Retrieve row position
    int row = tableWidget().at(indexOf("Global"))->itemAt(pos)->row();
    // Retrieve Variety Id
    QString idStr = tableWidget().at(indexOf("Global"))->item(row,1)->text();
    bool fFavorite = ! tableWidget().at(indexOf("Favorite"))->findItems(idStr,Qt::MatchFixedString).isEmpty();

     // Show Menu and execute action
    QString menuStr = (fFavorite)?  tr("Remove From Favorite") : tr("Add to Favorite");
    QMenu menu(tableWidget().at(indexOf("Global")));
    menu.addAction(menuStr);
    if (menu.exec(tableWidget().at(indexOf("Global"))->mapToGlobal(pos))) {
        if (fFavorite)
            removeFavorite(tableWidget().at(indexOf("Favorite"))->findItems(idStr,Qt::MatchFixedString).at(0)->row());
        else
            addFavorite(row);
    }
}

void GrapeVarietySelectionDialog::on_queryTableWidget_customContextMenuRequested(const QPoint &pos)
{
    // Retrieve row position
    int row = tableWidget().at(indexOf("Query"))->itemAt(pos)->row();
    // Retrieve Variety Id
    QString idStr = tableWidget().at(indexOf("Query"))->item(row,1)->text();
    bool fFavorite = ! tableWidget().at(indexOf("Favorite"))->findItems(idStr,Qt::MatchFixedString).isEmpty();

     // Show Menu and execute action
    QString menuStr = (fFavorite)?  tr("Remove From Favorite") : tr("Add to Favorite");
    QMenu menu(tableWidget().at(indexOf("Query")));
    menu.addAction(menuStr);
    if (menu.exec(tableWidget().at(indexOf("Query"))->mapToGlobal(pos))) {
        if (fFavorite)
            removeFavorite(tableWidget().at(indexOf("Favorite"))->findItems(idStr,Qt::MatchFixedString).at(0)->row());
        else {
            int globalRow = tableWidget().at(indexOf("Global"))->findItems(idStr,Qt::MatchFixedString).at(0)->row();
            addFavorite(globalRow);}
    }
}

void GrapeVarietySelectionDialog::on_lineEdit_textEdited(const QString &text)
{
    if (activeTableWidgetIndex()==2) {
        fSelectionActive = false;
        tableWidget().at(indexOf("Query"))->clearContents();
        tableWidget().at(indexOf("Query"))->setRowCount(0);
        if (!text.isEmpty()) {
            // Find text into Global Table
            QList<QTableWidgetItem *>itemList = tableWidget().at(indexOf("Global"))->findItems(text,Qt::MatchContains);
            tableWidget().at(indexOf("Query"))->setRowCount(itemList.size());
            // Freeze Sorting Enabled
            tableWidget().at(indexOf("Query"))->setSortingEnabled(false);
            // Clone the item from Global
            int qRow = 0;
            foreach (QTableWidgetItem *gItem, itemList) {
                 for(int i=0; i<2;i++) {
                     QTableWidgetItem *cloneItem = tableWidget().at(indexOf("Global"))->item(gItem->row(),i)->clone();
                     tableWidget().at(indexOf("Query"))->setItem(qRow,i,cloneItem);
                 }
                 // Import selection
                 tableWidget().at(indexOf("Query"))->item(qRow,0)->setSelected(gItem->isSelected());
                 qRow++;
            }
            // Reset sorting Enabled
            tableWidget().at(indexOf("Query"))->setSortingEnabled(true);
            tableWidget().at(indexOf("Query"))->sortByColumn(0,Qt::AscendingOrder);
        }
        fSelectionActive=true;
    }
}

void GrapeVarietySelectionDialog::on_tabWidget_currentChanged(int index)
{
    if (index ==2) {
        QString text = lineEdit().at(0)->text();
        on_lineEdit_textEdited(text);
    }
}

void GrapeVarietySelectionDialog::removeFavorite(int row)
{
    // Retrieve Variety Id
    QString idStr = tableWidget().at(indexOf("Favorite"))->item(row,1)->text();

    // Update Database and remove row
    QSqlQuery query;
    query.prepare(QString("UPDATE GrapeVariety SET Favorite = 0 WHERE Id = %1").arg(idStr));
    if (query.exec()) {
        // Set the Global Table Widget item selected before removal
        int globalRow = tableWidget().at(indexOf("Global"))->findItems(idStr,Qt::MatchFixedString).at(0)->row();
        bool fSelected = tableWidget().at(indexOf("Favorite"))->item(row,0)->isSelected();
        tableWidget().at(indexOf("Favorite"))->removeRow(row);
        tableWidget().at(indexOf("Global"))->item(globalRow,0)->setSelected(fSelected);
    }
}

void GrapeVarietySelectionDialog::addFavorite(int globalRow)
{
    QString idStr = tableWidget().at(indexOf("Global"))->item(globalRow,1)->text();

    // Update Database and add row
    QSqlQuery query;
    query.prepare(QString("UPDATE GrapeVariety SET Favorite = 1 WHERE Id = %1").arg(idStr));
    if (query.exec()) {
        // Freeze sorting and insert row at first position
        tableWidget().at(indexOf("Favorite"))->setSortingEnabled(false);
        tableWidget().at(indexOf("Favorite"))->insertRow(0);
        // Clone the items
        for (int i=0;i<2;i++) {
            QTableWidgetItem *item = tableWidget().at(indexOf("Global"))->item(globalRow,i)->clone();
            tableWidget().at(indexOf("Favorite"))->setItem(0, i,item);
        }
        // Copy Selection
        tableWidget().at(indexOf("Favorite"))->item(0,0)->setSelected(tableWidget().at(indexOf("Global"))->item(globalRow,0)->isSelected());
       // Sort the table widget
        tableWidget().at(indexOf("Favorite"))->setSortingEnabled(true);
        tableWidget().at(indexOf("Favorite"))->sortByColumn(0,Qt::AscendingOrder);
    }
}

