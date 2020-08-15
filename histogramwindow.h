#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QMainWindow>
#include "imageacquisition.h"
#include <QAction>

class HistogramWindow: public QMainWindow
{
    Q_OBJECT
public:
    HistogramWindow(QAction* histogramButton, QWidget *parent);

    QAction *getGrayHistogramButton();
    void setGrayHistogramButton(QAction *value);

    ImageAcquisition *getImageAcquisitionThread() const;
    void setImageAcquisitionThread(ImageAcquisition *value);

private:
    ImageAcquisition *imageAcquisitionThread;
    QAction *grayHistogramButton;

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // HISTOGRAMWINDOW_H
