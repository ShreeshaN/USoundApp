#include "homescreen.h"
#include "ui_homescreen.h"
#include <QDebug>
#include <QPushButton>

Homescreen::Homescreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Homescreen)
{
    ui->setupUi(this);
    QPushButton *m_button;
    ui->cameraControlsSplitter->addWidget(m_button);
}

Homescreen::~Homescreen()
{
    delete ui;
}


void Homescreen::on_devicesRefresh_clicked()
{
    qDebug() << "Clicked on refresh";
//    * Select a suitable image acquisition interface name AcqName
//    info_framegrabber(AcqName,'port',Information,Values)
//    * Open image acquisition device using the default settings, see
//    * documentation of the actually used interface for more details
//    open_framegrabber(AcqName,1,1,0,0,0,0,'default',-1,'default',-1.0, \
//                      'default','default','default',-1,-1,AcqHandle)
//    grab_image(Image,AcqHandle)
//    close_framegrabber(AcqHandle)\






}
