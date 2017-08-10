#include "querydialog.h"
#include <QDebug>

QueryDialog::QueryDialog(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{
    // Set Form and connect the field with variable
    setForm();

    setQueryLabel();
    setQueryLineEdit();
    setQueryCombo();
    setDialogButtonBox(findChild<QDialogButtonBox*>("buttonBox"));
}


QTableView *QueryDialog::view() const
{
    return m_view;
}

void QueryDialog::setView(QTableView *view)
{
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_view = view;
}

AbstractWineTableModel *QueryDialog::model() const
{
    return m_model;
}

void QueryDialog::setModelAndView(AbstractWineTableModel *model)
{
    setView(findChild<QTableView *>("queryView"));
    view()->setModel(model);
    m_model = model;
}

int QueryDialog::selectedRow()
{
    QItemSelectionModel *selectionModel = view()->selectionModel();
    if (selectionModel->hasSelection()) {
        return selectionModel->selectedRows().at(0).row();
    }
    return -1;
}

QString QueryDialog::selectedName()
{
    QString str;
    int sRow = selectedRow();
    if (sRow !=-1)
        str = model()->record(sRow).value(selectedFieldName()).toString();
    return str;
}

int QueryDialog::selectedId()
{
    int id=-1;
    int sRow = selectedRow();
    if (sRow !=-1)
        id = model()->record(sRow).value("Id").toInt();
    return id;
}

int QueryDialog::selectedIntField(QString fieldName)
{
    int result=-1;
    int sRow = selectedRow();
    if (sRow !=-1)
        result = model()->record(sRow).value(fieldName).toInt();
    return result;
}

QString QueryDialog::selectedStringField(QString fieldName)
{
    QString str;
    int sRow = selectedRow();
    if (sRow !=-1)
        str = model()->record(sRow).value(fieldName).toString();
    return str;

}

void QueryDialog::setForm()
{
    QUiLoader loader;
      QFile file(":/form/queryForm.ui");

      //Open File and load Form
      file.open(QFile::ReadOnly);
      QWidget *w = loader.load(&file,this);
      file.close();

       setMinimumHeight(w->height());
       setMinimumWidth(w->width());

      // Create a layout and add the form
      QVBoxLayout * layout =new QVBoxLayout(this);
      layout->addWidget(w);
}

void QueryDialog::setColumnWidth(QString fieldName, int width)
{
   int index = model()->fieldIndex(fieldName);
   if (index !=-1)
       view()->setColumnWidth(index,width);
}

void QueryDialog::populateCombo(const QString &fieldName, const QString &tableName,int comboIndex)
{
    QStringList list;
    list <<"";
    QSqlQuery query;
    query.prepare("SELECT "+fieldName+" FROM "+tableName+ " ORDER BY Id");
    query.exec();
    while (query.next())
           list << query.value(0).toString();

     queryCombo().at(comboIndex)->addItems(list);
     queryCombo().at(comboIndex)->setCurrentIndex(0);
}

void QueryDialog::selectRowId(int id)
{
    // Retrieve row position
    int row = model()->rowPosition(id);
    if (row !=-1)
        view()->selectRow(row);
}

void QueryDialog::setLabelText(const QStringList &textList)
{
   int maxSize=queryLabel().size();
   int lagArray[maxSize];
   int lag =0;
   int iList=0;
   foreach (QString str, textList) {
       if (str.isEmpty()) {
           hideQueryRow(iList);
           lag++;}
       else
        {queryLabel().at(iList)->setText(str);}
       lagArray[iList]=lag;
       iList++;
    }
   // Hide unemployed widgets
   for (int i=iList;i<maxSize;i++)
       hideQueryRow(i);

   // Find the last shown widgets
   while ((rowIsHidden(iList)) && (iList>0))
       iList--;
    // Move shown widget
   while (iList>0) {
           moveRow(iList,lagArray[iList]);
           iList--;}

   // Adjust GroupBox height
    QGroupBox *groupBox = findChild<QGroupBox *>("groupBox");
    int oldHeight = groupBox->height();
    groupBox->adjustSize();
    int moveStep = oldHeight - groupBox->height();
    // Move view and resize
     view()->move(view()->pos() - QPoint(0,moveStep));
     view()->resize(view()->size()+QSize(0,moveStep));
}

void QueryDialog::hideQueryRow(int index)
{
    // Get number of LineEdit Rows
    int sQueryLineEdit = queryLineEdit().size();
    // Hide Label
    queryLabel().at(index)->hide();

    // Hide Label or Combo
    if (index < sQueryLineEdit)
        queryLineEdit().at(index)->hide();
    else
        queryCombo().at(index-sQueryLineEdit)->hide();
}

void QueryDialog::moveRow(int index,int lag)
{
    if (lag ==0)
        return;

    int newIndex = index-lag;
    if (newIndex<0)
        return;

    int sQueryLineEdit = queryLineEdit().size();
    queryLabel().at(index)->move(queryLabel().at(newIndex)->pos());
    QWidget * oldWidget =(index< sQueryLineEdit)? static_cast<QWidget *>(queryLineEdit().at(index)) : static_cast<QWidget *>(queryCombo().at(index-sQueryLineEdit));
    QWidget * newWidget = (newIndex< sQueryLineEdit)? static_cast<QWidget *>(queryLineEdit().at(newIndex)) : static_cast<QWidget *>(queryCombo().at(newIndex-sQueryLineEdit));
    oldWidget->move(newWidget->pos());
}

bool QueryDialog::rowIsHidden(int index)
{
    return queryLabel().at(index)->isHidden();
}

QDialogButtonBox *QueryDialog::dialogButtonBox() const
{
    return m_dialogButtonBox;
}

void QueryDialog::setDialogButtonBox(QDialogButtonBox *dialogButtonBox)
{
    dialogButtonBox->addButton(tr("Create"),QDialogButtonBox::ActionRole);
    dialogButtonBox->addButton(tr("Show"),QDialogButtonBox::ActionRole);

    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QueryDialog::accept);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QueryDialog::reject);
     m_dialogButtonBox = dialogButtonBox;
}

QList<QLabel *> QueryDialog::queryLabel() const
{
    return m_queryLabel;
}

void QueryDialog::setQueryLabel()
{
    m_queryLabel << findChild<QLabel *>("queryLabel") << findChild<QLabel *>("queryLabel2") << findChild<QLabel *>("queryComboLabel") << findChild<QLabel *>("queryComboLabel2") <<  findChild<QLabel *>("queryComboLabel3");
}

QList<QLineEdit *> QueryDialog::queryLineEdit() const
{
    return m_queryLineEdit;
}

void QueryDialog::setQueryLineEdit()
{

    m_queryLineEdit << findChild<QLineEdit *>("queryLineEdit") << findChild<QLineEdit *>("queryLineEdit2");
}

QList<QComboBox *> QueryDialog::queryCombo() const
{
    return m_queryCombo;
}

void QueryDialog::setQueryCombo()
{
    m_queryCombo << findChild<QComboBox *>("queryComboBox") << findChild<QComboBox *>("queryComboBox2")<< findChild<QComboBox *>("queryComboBox3");
}

QStringList QueryDialog::shownFieldNames() const
{
    return m_shownFieldNames;
}

void QueryDialog::setShownFieldNames(const QStringList &shownFieldNames)
{
    // Hide all columns
    for(int i=0, n=model()->columnCount(); i<n ; i++)
        view()->hideColumn(i);

    // Show particular column
    foreach (QString str, shownFieldNames) {
        int index = model()->fieldIndex(str);
        if (index!=-1)
            view()->showColumn(index);
    }
    m_shownFieldNames = shownFieldNames;
}

QString QueryDialog::selectedFieldName() const
{
    return m_selectedFieldName;
}

void QueryDialog::setSelectedFieldName(const QString &selectedFieldName)
{
    m_selectedFieldName = selectedFieldName;
}
