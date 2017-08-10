#include "bottledialog.h"
#include <QDebug>

BottleDialog::BottleDialog(QSqlDatabase db, QSqlRecord rec, int iProcess, QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent, f)
{
    // Create Title and minimum width
    setWindowTitle(tr("Bottle Information"));
    setDb(db);
    setRec(rec);

    setForm();

    // Create LineEdits, ComboBox and SpinBoxes
    setLineEdit(new QLineEdit*);
    setDateEdit(new QDateEdit *);
    setCombo(findChild<QComboBox *>("wineTypeComboBox"));
    setSpinBox(new QSpinBox*);
    setDoubleSpinBox(new QDoubleSpinBox*);

    // Create Dialog Buttons
    setDialogButtonBox(findChild<QDialogButtonBox *>("buttonBox"),iProcess);
}

QDialogButtonBox *BottleDialog::dialogButtonBox() const
{
    return m_dialogButtonBox;
}

void BottleDialog::setDialogButtonBox(QDialogButtonBox *dialogButtonBox, int iProcess)
{
    QString processText;
    switch (iProcess) {
    case 0:
        processText = tr("Create");
        break;
    case 1:
        processText = tr("Update");
        break;
    default:
        break;
    }
    dialogButtonBox->addButton(processText,QDialogButtonBox::AcceptRole);

    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &BottleDialog::reject);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &BottleDialog::accept);

    m_dialogButtonBox = dialogButtonBox;
}

QLineEdit **BottleDialog::lineEdit() const
{
    return m_lineEdit;
}

void BottleDialog::setLineEdit(QLineEdit **lineEdit)
{
    // connect the lineEdits with form
    QStringList strList;
    strList << "bottleLineEdit" << "vineyardLineEdit" << "wineNameLineEdit" << "purchaseLocationLineEdit";
    for (int i=0;i<strList.size();i++) {
        lineEdit[i] = findChild<QLineEdit *>(strList[i]); }

    //Initialise Id field
    lineEdit[0]->setText(QString::number(rec().value("Id").toInt()));
    lineEdit[1]->setText(rec().value("Domaine").toString());
    lineEdit[2]->setText(rec().value("Wine_Name").toString());
    lineEdit[3]->setText(rec().value("Purchase_Location").toString());

    m_lineEdit = lineEdit;
}

QComboBox *BottleDialog::combo() const
{
    return m_combo;
}

void BottleDialog::setCombo(QComboBox *combo)
{

   // Find all the wine type available
     QSqlQuery query(db());
      query.prepare("SELECT Type FROM Wine_Type");
      query.exec();
      // Insert a blank at index 0
     QString text="";
     combo->addItem(text);
     // Populate the combobox
     while (query.next()) {
            text = query.value(0).toString();
             combo->addItem(text);
          }
    combo->setCurrentIndex(rec().value("Wine_Type").toInt());

    m_combo = combo;
}

QSpinBox **BottleDialog::spinBox() const
{
    return m_spinBox;
}

void BottleDialog::setSpinBox(QSpinBox **spinBox)
{
    // connect the spinBox with form
    QStringList strList;
    strList << "millesimeSpinBox";

    // Define Button features
    for (int i=0;i< strList.size();i++) {
        spinBox[i]= findChild<QSpinBox *>(strList[i]);}

    spinBox[0]->setValue(rec().value("Millesime").toInt());
    m_spinBox = spinBox;
}

QSqlDatabase BottleDialog::db() const
{
    return m_db;
}

void BottleDialog::setDb(const QSqlDatabase &db)
{
    m_db = db;
}

QSqlRecord BottleDialog::rec()
{
    return m_rec;
}

void BottleDialog::setRec(const QSqlRecord &rec)
{
    m_rec = rec;
}

void BottleDialog::setForm()
{
    QUiLoader loader;
      QFile file(":/form/bottleDialog.ui");

      //Open File and load Form
      file.open(QFile::ReadOnly);
      QWidget *w = loader.load(&file,this);
      file.close();

      //Activate the slots by name to connect the form
       QMetaObject::connectSlotsByName(this);
       setMinimumHeight(w->height());
       setMinimumWidth(w->width());

      // Create a layout and add the form
      QVBoxLayout * layout =new QVBoxLayout(this);
      layout->addWidget(w);
}

void BottleDialog::on_purchaseDateClearButton_clicked()
{
    dateEdit()[0]->setDate(dateEdit()[0]->minimumDate());
}

void BottleDialog::on_purchaseDateTodayButton_clicked()
{
    dateEdit()[0]->setDate(QDate::currentDate());
}

void BottleDialog::on_vineyardButton_clicked()
{
    DomaineQueryDialog *dialog = new DomaineQueryDialog(lineEdit()[1]->text(),db());
    if (dialog->exec()== QDialog::Accepted) {
        // Retrieve selected item and update line Edit
        QString str = dialog->selectedName();
        if (!str.isEmpty())
            lineEdit()[1]->setText(str);
    }
    dialog->deleteLater();
}

void BottleDialog::on_wineNameButton_clicked()
{
    WineQueryDialog *dialog = new WineQueryDialog(lineEdit()[1]->text(),lineEdit()[1]->text(),db());
    if (dialog->exec()== QDialog::Accepted) {
        // Retrieve selected item and update line Edit
        QString str = dialog->selectedName();
        if (!str.isEmpty()) {
            lineEdit()[2]->setText(str);
            lineEdit()[1]->setText(dialog->selectedStringField("Domaine"));
            combo()->setCurrentText(dialog->selectedStringField("Type"));
        }
    }
    dialog->deleteLater();
}

int BottleDialog::domaineId() const
{
    return m_domaineId;
}

void BottleDialog::setDomaineId(int domaineId)
{
    m_domaineId = domaineId;
}

QDoubleSpinBox **BottleDialog::doubleSpinBox() const
{
    return m_doubleSpinBox;
}

void BottleDialog::setDoubleSpinBox(QDoubleSpinBox **doubleSpinBox)
{
    // connect the double spinBox with form
    QStringList strList;
    strList << "purchasePricedoubleSpinBox";

    // Define Button features
    for (int i=0;i< strList.size();i++) {
        doubleSpinBox[i]= findChild<QDoubleSpinBox *>(strList[i]);}

    doubleSpinBox[0]->setValue(rec().value("Purchase_Price").toDouble());
    m_doubleSpinBox = doubleSpinBox;
}

QDateEdit **BottleDialog::dateEdit() const
{
    return m_dateEdit;
}

void BottleDialog::setDateEdit(QDateEdit **dateEdit)
{
    // connect the dateEdit with form
    QStringList strList;
    strList << "purchaseDateEdit";

    // Define Button features
    for (int i=0;i< strList.size();i++) {
        dateEdit[i]= findChild<QDateEdit *>(strList[i]);}

    dateEdit[0]->setDate((rec().value("Purchase_Date").toDateTime()).date());

    m_dateEdit = dateEdit;
}

