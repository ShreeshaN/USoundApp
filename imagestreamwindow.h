#ifndef IMAGESTREAMWINDOW_H
#define IMAGESTREAMWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "imageacquisition.h"

class ImageStreamWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ImageStreamWindow(QWidget *parent = nullptr);

    ImageAcquisition *imageAcquisitionThread;

signals:


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // IMAGESTREAMWINDOW_H
