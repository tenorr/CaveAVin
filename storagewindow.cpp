#include "storagewindow.h"
#include <QDebug>

StorageWindow::StorageWindow(int storageId, Cellar *cellar, QWidget *parent)
: QMainWindow(parent)
{
    // Install Delegate to communicate with the different storage Type
    setDelegate(new StorageDelegate(storageId,cellar));

    setStorageId(storageId);
    setStorageType();
    setCellar(cellar);

    // Retrieve geometry from DataBase
    QRect geometryRect = getGeometryFromDatabase(storageId);
    setGeometry(geometryRect);

    setWindowModality(Qt::WindowModal);
    setWindowTitle(tr("Storage #")+QString::number(storageId));

    // Create Menu
    setMenus(new QMenu *);

     // Create Graphics View and affect to central Widget
    setGraphicsView(new QGraphicsView);

    // Create Storage Scene
    setStorageScene(findStorageTypeScene());
}

QGraphicsView *StorageWindow::graphicsView() const
{
    return m_graphicsView;
}

void StorageWindow::setGraphicsView(QGraphicsView *graphicsView)
{
    // Set View Characteristics
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setFixedSize(width(),height());

    setCentralWidget(graphicsView);
    m_graphicsView = graphicsView;
}

Cellar *StorageWindow::cellar() const
{
    return m_cellar;
}

void StorageWindow::setCellar(Cellar *cellar)
{
    m_cellar = cellar;
}

QMenu **StorageWindow::menus() const
{
    return m_menus;
}

void StorageWindow::setMenus(QMenu **menus)
{
    // Create Menus and Actions
    QStringList menuTitles;
    menuTitles << tr("File");

    for (int i=0;i<menuTitles.length();i++) {
        menus[i]=new QMenu(menuTitles.at(i));
        switch (i) {
        case 0:
             menus[i]->addAction(tr("Close"),this,SLOT(close()),QKeySequence(Qt::CTRL+Qt::Key_Q));
            break;
        default:
            break;
                   }
            // Assign Menu to MenuBar
            menuBar()->addMenu(menus[i]);
      }
    m_menus = menus;
}

AbstractStorageScene *StorageWindow::storageScene() const
{
    return m_storageScene;
}

void StorageWindow::setStorageScene(AbstractStorageScene *storageScene)
{   
    // Assign Storage To Graphics View
    graphicsView()->setScene(storageScene);

    m_storageScene = storageScene;
}


void StorageWindow::moveEvent(QMoveEvent *event)
{
    if (event) {
        QSqlQuery query;
        query.prepare(QString("SELECT QRect FROM Storage WHERE Id = %1").arg(QString::number(storageId())));
        query.exec();
        if (query.first()) {
            QRect rect = StorageTableModel::decodeToQRect(query.value("QRect").toString());
            rect.moveTopLeft(event->pos());
            QString rectStr = StorageTableModel::encodeFromQRect(rect);
            query.prepare(QString("UPDATE Storage "
                                  "SET QRect = '%1' "
                                  "WHERE Id = %2").arg(rectStr).arg(QString::number(storageId())));
            query.exec();
            }
    }
}

QRect StorageWindow::getGeometryFromDatabase(int storageId)
{
    // Retrieve Data
    QSqlQuery query;
    query.prepare(QString("SELECT QRect, Width, Height, RowCount, ColumnCount, RackHint FROM Storage WHERE Id =%1").arg(storageId));
    query.exec();
    if (query.first()) {
        QString rectStr = query.value("QRect").toString();
        int w = query.value("Width").toInt();
        int h = query.value("Height").toInt();
        delegate()->setStorageRowCount(query.value("RowCount").toInt());
        delegate()->setStorageColumnCount(query.value("ColumnCount").toInt());
        int rackHint = query.value("RackHint").toInt();
        delegate()->setRackHint(rackHint);
        if (rackHint == 0)  // Set RACK_DEFAULT value if null
            rackHint = RACK_DEFAULT;

    // Check if the storage is matrix based
    if (delegate()->storageRowCount()*delegate()->storageColumnCount() ==0) {
        // Deal with non matrix based storage
            // Find minimum ratio
            qreal ratio = qMin(cellar()->width()/w , cellar()->height()/h);
            // Store QRect Value
            if (rectStr.isEmpty()) {
                rectStr = StorageTableModel::encodeFromQRect(QRect(50,50,int(w*ratio), int(h*ratio)));
                }
            else
            {
                QRect rect = StorageTableModel::decodeToQRect(rectStr);
                rect.setWidth(int(w*ratio));
                rect.setHeight(int(h*ratio));
                rectStr = StorageTableModel::encodeFromQRect(rect);}

            // Update DataBase
            query.prepare(QString("UPDATE Storage "
                                  "SET QRect = '%1' "
                                  "WHERE Id = %2").arg(rectStr).arg(QString::number(storageId)));
            query.exec();
            return StorageTableModel::decodeToQRect(rectStr);
             }
    else {
         // Deal with matrix based storage
             if (!rectStr.isEmpty())
                return StorageTableModel::decodeToQRect(rectStr);
        // If QRect null
        // Calculate theorical size with rack hint value
        QSizeF rectSize = QSizeF(delegate()->storageColumnCount()+1,delegate()->storageRowCount()+1) * rackHint; // +1 is for the window margins
        rectSize += QSizeF(0,100); // Add 100 for text name and bottles to be position

        // Recalculate rack hint value if one at least of the window dimensions is bigger than screen ones.
        if (rectSize.width() > cellar()->width())
            rackHint = int((cellar()->width()-40)/(delegate()->storageColumnCount()+1));

        if (rectSize.height() > cellar()->height())
            rackHint = qMin(int((cellar()->height()-120)/(delegate()->storageRowCount()+1)),rackHint);

        // Set RankHint in delegate
        delegate()->setRackHint(rackHint);

         // Set the correct size with a minimum of 1000 per dimension
        rectSize.setWidth(qMax(40+rackHint*delegate()->storageColumnCount(),1000));
        rectSize.setHeight(qMax(120 + rackHint*delegate()->storageRowCount(),1000));

        // Update QRect and RackHint in DataBase
        QString rectStr = StorageTableModel::encodeFromQRect(QRect(QPoint(50,50),rectSize.toSize()));
        query.prepare(QString("UPDATE Storage "
                              "SET QRect = '%1' , RackHint = %2 "
                              "WHERE Id = %3").arg(rectStr).arg(rackHint).arg(storageId));
        query.exec();

        return QRect(QPoint(50,50),rectSize.toSize());
        }
    }
    return QRect();
}

AbstractStorageScene *StorageWindow::findStorageTypeScene()
{
    // Retrieve Storage Type
        switch (storageType()) {
        // Create the storageType Classes
        case 1:
            return new MatrixScene(storageId(),delegate());
            break;
        }
    return new Container(storageId(),delegate());
}

int StorageWindow::storageType() const
{
    return m_storageType;
}

void StorageWindow::setStorageType()
{
    // Retrieve Storage Type
    QSqlQuery query;
    query.prepare(QString("SELECT StorageType FROM Storage WHERE Id =%1").arg(storageId()));
    query.exec();
        m_storageType = (query.first())? query.value("StorageType").toInt() : 0 ;
}

StorageDelegate *StorageWindow::delegate()
{
    return m_delegate;
}

void StorageWindow::setDelegate(StorageDelegate *delegate)
{
    m_delegate = delegate;
}

int StorageWindow::storageId() const
{
    return m_storageId;
}

void StorageWindow::setStorageId(int storageId)
{
    m_storageId = storageId;
}
