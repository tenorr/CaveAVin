#include "winecavemainwindow.h"

WineCaveMainWindow::WineCaveMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // Set Window Title and Geometry
    setWindowTitle(tr("Wine Cave ") + VERSION);
    setGeometry(100,100,2000,1200);

    // Add Menus and MenuBar
    setMenus(new QMenu*);

    // Open Acess DataBase
      setDb();

     // Create TableModels
      setContainerTableModel(new ContainerTableModel(this,db()));
      setBottleTableModel(new BottleTableModel(this,db()));
      setZoneTableModel(new ZoneTableModel(this,db()));

    // Create Graphics View and affect to central Widget
    setGraphicsView(new QGraphicsView);

    // Create Room (Default Id =1)
    setRoom(new Room(containerTableModel(),bottleTableModel(),zoneTableModel()));
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
    graphicsView->setFixedSize(1980,1220);

    setCentralWidget(graphicsView);

    m_graphicsView = graphicsView;
}

Room *WineCaveMainWindow::room() const
{
    return m_room;
}

void WineCaveMainWindow::setRoom(Room *room)
{
    // Assign Room To Graphics View
    graphicsView()->setScene(room);

    m_room = room;
}

void WineCaveMainWindow::defineWineTypes()
{
    WineTypeDialog *dialog = new WineTypeDialog(db());
    connect(dialog->view(),&WineTypeView::colorChanged,room(),&Room::changeWineColor);
    connect(dialog->view(),&WineTypeView::brushStyleChanged,room(),&Room::changeWinebrushStyle);
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

ContainerTableModel *WineCaveMainWindow::containerTableModel() const
{
    return m_containerTableModel;
}

void WineCaveMainWindow::setContainerTableModel(ContainerTableModel *containerTableModel)
{
    m_containerTableModel = containerTableModel;
}

QSqlDatabase WineCaveMainWindow::db() const
{
    return m_db;
}

void WineCaveMainWindow::setDb()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)}; DBQ=C:/Users/olivi_000/OneDrive/Documents/Qt/WineCave/CaveAVin/WineDatabase.mdb");
    db.open();
    m_db = db;
}
