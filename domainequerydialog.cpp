#include "domainequerydialog.h"
#include <QDebug>

DomaineQueryDialog::DomaineQueryDialog(QString domaineStr, QSqlDatabase db, QWidget *parent, Qt::WindowFlags f)
    : QueryDialog(parent,f)
{
    // Create Title
    setWindowTitle(tr("Find Domaine"));

    // Set model and view
    setModelAndView(new DomaineModel(this,db));

    // Define Text and Hide selection

    QStringList textList {tr("Domaine :"), "", tr("Country :")};
    setLabelText(textList);

    // Populate Country Combo
    populateCombo("Country","Country");

   // queryLabel().at(2)->move(queryLabel().at(1)->pos());
   // queryCombo().at(0)->move(queryLineEdit().at(1)->pos());

    // Field to be return
    setSelectedFieldName("Domaine");

    // Fields to be shown
    QStringList shownFieldNameList {"Domaine","Country"};
    setShownFieldNames(shownFieldNameList);

    // Set the specific field size
    setColumnWidth("Domaine",380);

    // Set the initial Domaine string
    queryLineEdit().at(0)->setText(domaineStr);

    //Connect matching signals from form to slots of object
     QMetaObject::connectSlotsByName(this);
     connect(dialogButtonBox(),SIGNAL(clicked(QAbstractButton*)),this,SLOT(doAction(QAbstractButton*)));

     // Select the Domaine if any
     domaineModel()->selectRecords(domaineStr);
}

DomaineModel *DomaineQueryDialog::domaineModel() const
{
    return static_cast<DomaineModel *>(model());
}

void DomaineQueryDialog::on_queryLineEdit_textEdited(const QString &text)
{
    domaineModel()->selectRecords(text,queryCombo().at(0)->currentIndex());
}

void DomaineQueryDialog::on_queryComboBox_currentIndexChanged(int index)
{
     domaineModel()->selectRecords(queryLineEdit().at(0)->text(),index);
}

void DomaineQueryDialog::doAction(QAbstractButton *button)
{
    if (dialogButtonBox()->buttonRole(button) != QDialogButtonBox::ActionRole)
        return;
        // Do nothing if show and no selected item
        if ((button->text() == tr("Show")) && (selectedId()==-1))
            return;

        // Retrieve selected Id
        int sId = (button->text() == tr("Create"))? -1: selectedId();
        DomaineDialog *dialog = new DomaineDialog(domaineModel()->database(),sId);
        if (dialog->exec() == QDialog::Accepted) {
            model()->select();
            selectRowId(dialog->getId());
        }
    dialog->deleteLater();
}

