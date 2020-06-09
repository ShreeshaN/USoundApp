#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QMainWindow>
#include <HalconCpp.h>
#include "HFramegrabber.h"
#include <HFramegrabber.h>


QT_BEGIN_NAMESPACE
namespace Ui { class Homescreen; }
QT_END_NAMESPACE

class Homescreen : public QMainWindow
{
    Q_OBJECT

public:
    Homescreen(QWidget *parent = nullptr);
    ~Homescreen();

private slots:
    void on_devicesRefresh_clicked();

private:
    Ui::Homescreen *ui;
};
#endif // HOMESCREEN_H
