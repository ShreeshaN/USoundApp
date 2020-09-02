#include "histogramwindow.h"

HistogramWindow::HistogramWindow(QAction* histogramButton, QWidget *parent) : QMainWindow(parent)
{
this->grayHistogramButton = histogramButton;

}

QAction *HistogramWindow::getGrayHistogramButton()
{
    return grayHistogramButton;
}

void HistogramWindow::setGrayHistogramButton(QAction *value)
{
    grayHistogramButton = value;
}

void HistogramWindow::closeEvent(QCloseEvent *event)
{
    this->imageAcquisitionThread->setSupplyHistogramData(false);
    this->imageAcquisitionThread->setSupplyHistogramDataForLine(false);
//    Sleep(200);
    this->getGrayHistogramButton()->setDisabled(false);
}


// Setters and getters
ImageAcquisition *HistogramWindow::getImageAcquisitionThread() const
{
    return imageAcquisitionThread;
}

void HistogramWindow::setImageAcquisitionThread(ImageAcquisition *value)
{
    imageAcquisitionThread = value;
}
