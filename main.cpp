#include "homescreen.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);
        Homescreen w;

        w.setWindowIcon(QIcon("://icons/wpi_logo.ico"));
        w.show();
        return a.exec();
    } catch (std::exception &e) {

        qDebug() << e.what();
    }

}
