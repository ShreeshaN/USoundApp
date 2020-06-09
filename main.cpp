#include "homescreen.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Homescreen w;
    w.setWindowIcon(QIcon("://icons/wpi_logo.ico"));
    w.show();
    return a.exec();
}
