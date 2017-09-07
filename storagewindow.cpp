#include "storagewindow.h"
#include <QDebug>

StorageWindow::StorageWindow(int storageId, Cellar *cellar, QWidget *parent)
: QMainWindow(parent)
{
    // Install Delegate to communicate with the different storage Type
    setDelegate(new StorageDelegate(storageId,cellar));

    setStorageId(storageId);
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
    setStorageScene(findStorageType());
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
            rect.setTopLeft(event->pos());
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
    QSqlQuery query;
    query.prepare(QString("SELECT QRect, Width, Height FROM Storage WHERE Id = %1").arg(QString::number(storageId)));
    query.exec();
    if (query.first()) {
       QString rectStr = query.value("QRect").toString();

       // Calculate window Dimensions
       int w = query.value("Width").toInt();
       int h = query.value("Height").toInt();
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
    return QRect();
}

AbstractStorageScene *StorageWindow::findStorageType()
{
    // Retrieve Storage Type
    QSqlQuery query;
    query.prepare(QString("SELECT StorageType FROM Storage WHERE Id =%1").arg(storageId()));
    query.exec();
    if (query.first()) {
        switch (query.value("StorageType").toInt()) {
        case 1:
         // Create the storageType Class
            break;
        }
    }
    return new Container(storageId(),delegate());
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
