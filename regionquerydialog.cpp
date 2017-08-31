#include "regionquerydialog.h"


RegionQueryDialog::RegionQueryDialog(QString regionStr, QSqlDatabase db,QWidget *parent, Qt::WindowFlags f)
    : QueryDialog(parent,f)
{
    // Create Title
    setWindowTitle(tr("Find Region"));

    // Set model and view
    setModelAndView(new RegionModel(this,db));

    // Define Text and Hide selection
    queryLabel().at(0)->setText(tr("Region :"));
    queryLabel().at(1)->hide();
    queryLabel().at(2)->setText(tr("Country :"));
    queryLineEdit().at(1)->hide();

    // Populate Country Combo
    populateCombo("Country","Country");

    // Field to be return
    setSelectedFieldName("Region");

    // Fields to be shown
    QStringList shownFieldNameList {"Region","Country"};
    setShownFieldNames(shownFieldNameList);

    // Set the initial Winery string
    queryLineEdit().at(0)->setText(regionStr);

    //Connect matching signals from form to slots of object
     QMetaObject::connectSlotsByName(this);

     // Select the Winery if any
     //regionModel()->selectRecords(wineryStr);
}

RegionModel *RegionQueryDialog::regionModel() const
{
    return static_cast<RegionModel *>(model());
}

void RegionQueryDialog::on_queryLineEdit_textEdited(const QString &text)
{
    regionModel()->selectRecords(text,queryCombo().at(0)->currentIndex());
}

void RegionQueryDialog::on_comboBox_currentIndexChanged(int index)
{
     regionModel()->selectRecords(queryLineEdit().at(0)->text(),index);
}


