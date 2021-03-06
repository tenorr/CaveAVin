#include "winequerydialog.h"
#include <QDebug>

WineQueryDialog::WineQueryDialog(QString wineryStr, QString wineStr, QSqlDatabase db, QWidget *parent, Qt::WindowFlags f)
    :QueryDialog(parent,f)
{
    // Create Title
    setWindowTitle(tr("Find Wine"));

    // Set model and view
    setModelAndView(new WineModel(this,db));

    // Define Text and Hide selection
    QStringList textList {tr("Winery :"), tr("Wine :"), tr("Type :")};
    setLabelText(textList);

    // Populate Country Combo
    populateCombo("Type","WineType");

    // Field to be return
    setSelectedFieldName("Wine");

    // Fields to be shown
    QStringList shownFieldNameList {"Wine","Winery","Type"};
    setShownFieldNames(shownFieldNameList);

    // Set the initial Winery string
    queryLineEdit().at(1)->setText(wineStr);
    queryLineEdit().at(0)->setText(wineryStr);

    //Connect matching signals from form to slots of object
     QMetaObject::connectSlotsByName(this);
     connect(dialogButtonBox(),SIGNAL(clicked(QAbstractButton*)),this,SLOT(doAction(QAbstractButton*)));

     // Select the Winery if any
     wineModel()->selectRecords(wineryStr,wineStr);
     adjustViewSize();
}

WineModel *WineQueryDialog::wineModel() const
{
    return static_cast<WineModel *>(model());
}

void WineQueryDialog::on_queryLineEdit_textEdited(const QString &text)
{
    wineModel()->selectRecords(text,queryLineEdit().at(1)->text(), queryCombo().at(0)->currentIndex());
    adjustViewSize();
}

void WineQueryDialog::on_queryLineEdit2_textEdited(const QString &text)
{
    wineModel()->selectRecords(queryLineEdit().at(0)->text(),text, queryCombo().at(0)->currentIndex());
    adjustViewSize();
}

void WineQueryDialog::on_queryComboBox_currentIndexChanged(int index)
{
    wineModel()->selectRecords(queryLineEdit().at(0)->text(),queryLineEdit().at(1)->text(), index);
    adjustViewSize();
}

void WineQueryDialog::doAction(QAbstractButton *button)
{
    if (dialogButtonBox()->buttonRole(button) == QDialogButtonBox::ActionRole) {

           // Retrieve selected Id
           int sId = (button->text() == tr("Create"))? -1: selectedId();
           WineDialog *dialog = new WineDialog(wineModel()->database(),sId);
           dialog->exec();

            // Reselect and update view
           wineModel()->select();
           selectRowId(dialog->getId());
           dialog->deleteLater();
         }
}

