#include "wineryquerydialog.h"
#include <QDebug>

WineryQueryDialog::WineryQueryDialog(QString wineryStr, QSqlDatabase db, QWidget *parent, Qt::WindowFlags f)
    : QueryDialog(parent,f)
{
    // Create Title
    setWindowTitle(tr("Find Winery"));

    // Set model and view
    setModelAndView(new WineryModel(this,db));

    // Define Text and Hide selection

    QStringList textList {tr("Winery :"), "", tr("Country :")};
    setLabelText(textList);

    // Populate Country Combo
    populateCombo("Country","Country");

    // Field to be return
    setSelectedFieldName("Winery");

    // Fields to be shown
    QStringList shownFieldNameList {"Winery","Country"};
    setShownFieldNames(shownFieldNameList);

    // Set the initial Winery string
    queryLineEdit().at(0)->setText(wineryStr);

    //Connect matching signals from form to slots of object
     QMetaObject::connectSlotsByName(this);
     connect(dialogButtonBox(),SIGNAL(clicked(QAbstractButton*)),this,SLOT(doAction(QAbstractButton*)));

     // Select the Winery if any
     wineryModel()->selectRecords(wineryStr);
     adjustViewSize();
}

WineryModel *WineryQueryDialog::wineryModel() const
{
    return static_cast<WineryModel *>(model());
}

void WineryQueryDialog::on_queryLineEdit_textEdited(const QString &text)
{
    wineryModel()->selectRecords(text,queryCombo().at(0)->currentIndex());
    adjustViewSize();
}

void WineryQueryDialog::on_queryComboBox_currentIndexChanged(int index)
{
     wineryModel()->selectRecords(queryLineEdit().at(0)->text(),index);
     adjustViewSize();
}

void WineryQueryDialog::doAction(QAbstractButton *button)
{
    if (dialogButtonBox()->buttonRole(button) != QDialogButtonBox::ActionRole)
        return;

        // Retrieve selected Id
        int sId = (button->text() == tr("Create"))? -1: selectedId();
        WineryDialog *dialog = new WineryDialog(wineryModel()->database(),sId);
        if (dialog->exec() == QDialog::Accepted) {
            model()->select();
            selectRowId(dialog->getId());
        }
    dialog->deleteLater();
}


