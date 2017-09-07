#include "winecavemainwindow.h"


WineCaveMainWindow::WineCaveMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // Set Window Title and Geometry
    setWindowTitle(tr("Wine Cave ") + VERSION);

    // Retieve screen geometry and adapt window size (85% of screen)
    QSize screenDimensions = QApplication::primaryScreen()->geometry().size()*=0.85;
    setGeometry(250,80,screenDimensions.width(),screenDimensions.height());

    // Add Menus and MenuBar
    setMenus(new QMenu*);

    // Open Acess DataBase
      setDb();

     // Create TableModels
      setStorageTableModel(new StorageTableModel(this,db()));
      setBottleTableModel(new BottleTableModel(this,db()));
      setZoneTableModel(new ZoneTableModel(this,db()));

    // Create Graphics View and affect to central Widget
    setGraphicsView(new QGraphicsView);

    // Create Cellar (Default Id =1)
    setCellar(new Cellar(storageTableModel(),bottleTableModel(),zoneTableModel(),1,this));
}

WineCaveMainWindow::~WineCaveMainWindow()
{
    // Close Access DataBase
      db().close();
}

QMenu **WineCaveMainWindow::menus() const
{
    return m_menus;
}

void WineCaveMainWindow::setMenus(QMenu **menus)
{
    // Create Menus and Actions
    QStringList menuTitles;
    menuTitles << tr("File") << tr("Global Data");

    for (int i=0;i<menuTitles.length();i++) {
        menus[i]=new QMenu(menuTitles.at(i));
        switch (i) {
        case 0:
             menus[i]->addAction(tr("Quit"),qApp,SLOT(quit()),QKeySequence(Qt::CTRL+Qt::Key_Q));
            break;
        case 1:
            menus[i]->addAction(tr("Define Wine Types"),this,SLOT(defineWineTypes()));
            break;
        default:
            break;
                   }
            // Assign Menu to MenuBar
            menuBar()->addMenu(menus[i]);
      }
    m_menus = menus;
}

QGraphicsView *WineCaveMainWindow::graphicsView() const
{
    return m_graphicsView;
}

void WineCaveMainWindow::setGraphicsView(QGraphicsView *graphicsView)
{
    // Set View Characteristics
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    graphicsView->setFixedSize(geometry().size());
    setCentralWidget(graphicsView);

    m_graphicsView = graphicsView;
}

Cellar *WineCaveMainWindow::cellar() const
{
    return m_cellar;
}

void WineCaveMainWindow::setCellar(Cellar *cellar)
{
    // Assign Cellar To Graphics View
    graphicsView()->setScene(cellar);

    m_cellar = cellar;
}

void WineCaveMainWindow::defineWineTypes()
{
    WineTypeDialog *dialog = new WineTypeDialog(db(),this);
    if (dialog->exec() == QDialog::Accepted)
        dialog->model()->submitAll();
     dialog->deleteLater();
}

ZoneTableModel *WineCaveMainWindow::zoneTableModel() const
{
    return m_zoneTableModel;
}

void WineCaveMainWindow::setZoneTableModel(ZoneTableModel *zoneTableModel)
{
    m_zoneTableModel = zoneTableModel;
}

BottleTableModel *WineCaveMainWindow::bottleTableModel() const
{
    return m_bottleTableModel;
}

void WineCaveMainWindow::setBottleTableModel(BottleTableModel *bottleTableModel)
{
    m_bottleTableModel = bottleTableModel;
}

StorageTableModel *WineCaveMainWindow::storageTableModel() const
{
    return m_storageTableModel;
}

void WineCaveMainWindow::setStorageTableModel(StorageTableModel *storageTableModel)
{
    m_storageTableModel = storageTableModel;
}

QSqlDatabase WineCaveMainWindow::db() const
{
    return m_db;
}

void WineCaveMainWindow::setDb()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)}; DBQ=C:/Users/olivi/OneDrive/Documents/Qt/WineCave/CaveAVin/WineDatabase.mdb");
    db.open();
    m_db = db;
}
