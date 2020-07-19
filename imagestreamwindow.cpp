#include "imagestreamwindow.h"
#include "imageacquisition.h"
#include <QMessageBox>
#include "homescreen.h"

ImageStreamWindow::ImageStreamWindow(QWidget *parent) : QMainWindow(parent)
{

}

void ImageStreamWindow::closeEvent(QCloseEvent *event)
{
    ImageAcquisition *imageAcquisitionThread = this->imageAcquisitionThread;
    imageAcquisitionThread->setStop(true);

    try {
        qDebug() << "Closing device "<< imageAcquisitionThread->getDeviceName()<< ". . . ";
        imageAcquisitionThread->getAcq().CloseFramegrabber();
    } catch (HalconCpp::HException &) {
        //todo: add dialog box here
        qDebug() << "Error: Could not close message box";
        QMessageBox::critical(this, "Close Camera","Error. Could not close camera",QMessageBox::Ok);
    }
    imageAcquisitionThread->exit();
    qDebug() << "COunter valalue: "<<imageAcquisitionThread->getCounter();



}
