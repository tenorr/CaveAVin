#include "winecavemainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WineCaveMainWindow w;
    w.show();

    return a.exec();
}
