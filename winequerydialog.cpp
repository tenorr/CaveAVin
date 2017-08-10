#include "winequerydialog.h"

WineQueryDialog::WineQueryDialog(QString domaineStr, QString wineStr, QSqlDatabase db, QWidget *parent, Qt::WindowFlags f)
    :QueryDialog(parent,f)
{
    // Create Title
    setWindowTitle(tr("Find Wine"));

    // Set model and view
    setModelAndView(new WineModel(this,db));

    // Define Text and Hide selection
    QStringList textList {tr("Domaine :"), tr("Wine :"), tr("Type :")};
    setLabelText(textList);

    // Populate Country Combo
    populateCombo("Type","Wine_Type");

    // Field to be return
    setSelectedFieldName("Wine");

    // Fields to be shown
    QStringList shownFieldNameList {"Wine","Domaine","Type"};
    setShownFieldNames(shownFieldNameList);

    // Set the initial Domaine string
    queryLineEdit().at(1)->setText(wineStr);
    queryLineEdit().at(0)->setText(domaineStr);

    //Connect matching signals from form to slots of object
     QMetaObject::connectSlotsByName(this);
     connect(dialogButtonBox(),SIGNAL(clicked(QAbstractButton*)),this,SLOT(doAction(QAbstractButton*)));

     // Select the Domaine if any
     wineModel()->selectRecords(domaineStr);
}

WineModel *WineQueryDialog::wineModel() const
{
    return static_cast<WineModel *>(model());
}

void WineQueryDialog::on_queryLineEdit_textEdited(const QString &text)
{
    wineModel()->selectRecords(text,queryLineEdit().at(1)->text(), queryCombo().at(0)->currentIndex());
}

void WineQueryDialog::on_queryLineEdit2_textEdited(const QString &text)
{
    wineModel()->selectRecords(queryLineEdit().at(0)->text(),text, queryCombo().at(0)->currentIndex());
}

void WineQueryDialog::on_queryComboBox_currentIndexChanged(int index)
{
    wineModel()->selectRecords(queryLineEdit().at(0)->text(),queryLineEdit().at(1)->text(), index);
}

void WineQueryDialog::doAction(QAbstractButton *button)
{
    if ((button->text() == tr("Show")) || (button->text() == tr("Create"))) {
        // Do nothing if show and no selected item
        if ((button->text() == tr("Show")) && (selectedId()==-1))
            return;

        // Retrieve selected Id
        int sId = (button->text() == tr("Create"))? -1: selectedId();
        WineDialog *dialog = new WineDialog(wineModel()->database(),sId);
        if (dialog->exec() == QDialog::Accepted) {
            model()->select();
            selectRowId(dialog->getId());
        }
        dialog->deleteLater();
    }

}

