#include "abstractbottle.h"
#include <QDebug>

AbstractBottle::AbstractBottle(QString locationName, QString subLocationName, QSqlRecord rec, QGraphicsItem *parent)
    : GraphicsObject(parent)
{
    // Set Name of location
    setLocationName(locationName);

    // Set Z coordinate to be in the foreground
    setZValue(1);

    setId(rec.value("Id").toInt());
    setLocationId(rec.value(locationName).toInt());
    setSublocationId(rec.value(subLocationName).toInt());

    // Set Radius according to location
    setRect(QRect(0,0,rec.value(locationName+"R").toInt(),rec.value(locationName+"R").toInt()));

    // Set Wine Type, Color and brushStyle
    int wineType = findWineTypeOf(rec.value("Wine").toInt());
     setWineType(wineType);

   // Create Context Menu
       QMenu *menu = new QMenu;
       menu->addAction(tr("Change Data"),this,SLOT(changeData()));
       menu->addAction(tr("Drink Bottle"), this,SLOT(drinkBottle()));
       setContextMenu(menu);
}

QString AbstractBottle::locationName() const
{
    return m_locationName;
}

void AbstractBottle::setLocationName(const QString &sceneName)
{
    m_locationName = sceneName;
}

int AbstractBottle::locationId() const
{
    return m_locationId;
}

void AbstractBottle::setLocationId(int locationId)
{
    m_locationId = locationId;
}

int AbstractBottle::sublocationId() const
{
    return m_sublocationId;
}

void AbstractBottle::setSublocationId(int sublocationId)
{
    m_sublocationId = sublocationId;
}

int AbstractBottle::wineType() const
{
    return m_wineType;
}

void AbstractBottle::setWineType(int wineType)
{
    QColor color = Qt::black;
    Qt::BrushStyle brushStyle = Qt::NoBrush;
    if (wineType !=0) {
         // Retrieve color and Brushtype
        QSqlQuery query;
        query.prepare("SELECT Red, Green, Blue, BrushStyle FROM Wine_Type WHERE Id = :id");
        query.bindValue(":id",wineType);
        if (query.exec()) {
            query.next();
            color = QColor(query.value("Red").toInt(),query.value("Green").toInt(),query.value("Blue").toInt() );
            brushStyle = (Qt::BrushStyle) query.value("BrushStyle").toInt();}
    }
    setColor(color);
    setBrushStyle(brushStyle);
    m_wineType = wineType;
}

BottleTableModel *AbstractBottle::bottleModel() const
{
    return m_bottleModel;
}

void AbstractBottle::setBottleModel(BottleTableModel *bottleModel)
{
    m_bottleModel = bottleModel;
}

void AbstractBottle::drinkBottle()
{
    emit bottleToBeDeleted(id());
}

QRectF AbstractBottle::boundingRect() const
{
    // Include half the pen to paint an outline
        qreal penWidth = 1;

       // Return an estimate of the area painted by the item
        return QRectF(- penWidth / 2, - penWidth / 2, rect().width()+ penWidth, rect().height() + penWidth);
}

void AbstractBottle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QBrush(color(),brushStyle()));
    painter->drawEllipse(QRect(0,0,rect().width(),rect().height()));
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void AbstractBottle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // Execute selected item of Context Menu
    contextMenu()->exec(event->screenPos());
}

void AbstractBottle::changeRectangleData()
{
    // Adjust size with average of width and height
   QSize size=rect().size();
   int radius = (size.height()+size.width())/2;
   m_rect.setSize(QSize(radius,radius));
   GraphicsObject::changeRectangleData();
}

QMenu *AbstractBottle::contextMenu() const
{
    return m_contextMenu;
}

void AbstractBottle::setContextMenu(QMenu *contextMenu)
{
    m_contextMenu = contextMenu;
}

void AbstractBottle::changeData()
{
    // Retrieve record
        int row = bottleModel()->rowPosition(id());
        QSqlRecord rec = bottleModel()->record(row);
        changeBottleData(rec,row);
}

bool AbstractBottle::changeBottleData(QSqlRecord &rec, int row)
{
    BottleDialog *dialog = new BottleDialog(bottleModel()->database(),rec,(row==-1)? 0:1);
    if (dialog->exec() != QDialog::Accepted) {
        dialog->deleteLater();
        return false;}

        // Update Bottle Model
        int index;
        rec.setValue("Wine",dialog->wineId());

        index = dialog->index("Millesime");
        int millesime = dialog->combo().at(index)->currentText().toInt();
        rec.setValue("Millesime",millesime);

        index = dialog->index("PurchaseLocation");
        rec.setValue("Purchase_Location",dialog->lineEdit().at(index)->text());

         index = dialog->index("PurchaseDate");
         QDate date = dialog->dateEdit().at(index)->date();
         if (date != dialog->dateEdit().at(index)->minimumDate())
             rec.setValue("Purchase_Date",date);
          else rec.setNull("Purchase_Date");

          index = dialog->index("PurchasePrice");
          rec.setValue("Purchase_Price",dialog->doubleSpinBox().at(index)->value());

          // Manage Label Image
          index = dialog->index("LabelImage");
          rec.setValue("LabelImage",dialog->lineEdit().at(index)->text());

        // Create New Record
        if (row == -1) {
            rec.setValue("Id",id());
            bottleModel()->insertRecord(-1,rec);}
        // Or update Record
        else {
            bottleModel()->setRecord(row,rec);
            // Change Wine Type if needed
            int wineTypeId = dialog->wineType();
;           if (wineType() != wineTypeId)
                setWineType(wineTypeId);
            }

    bottleModel()->submitAll();
    dialog->deleteLater();
    return true;
}

int AbstractBottle::findWineTypeOf(int wineId)
{
    if (wineId==0)
        return 0;
    QSqlQuery query;
    query.prepare(QString("SELECT Type FROM Wine WHERE Id = %1").arg(wineId));
    query.exec();
    query.first();
    return query.value(0).toInt();
}
