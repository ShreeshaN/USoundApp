#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QMainWindow>
#include <QLabel>
#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "Halcon.h"
#  include "HFramegrabber.h"
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#  endif
#endif
#include <QMap>
#include <map>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSpinBox>
#include "cameracontrols.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include<QPixmap>
#include <QGraphicsView>
#include <imagestreamwindow.h>
#include<QtWidgets/QPlainTextEdit>
#include <QDir>


QT_BEGIN_NAMESPACE
namespace Ui { class Homescreen; }
QT_END_NAMESPACE

class Homescreen : public QMainWindow
{
    Q_OBJECT
public:
    Homescreen(QWidget *parent = nullptr);
    ~Homescreen();

    // variables
    QList<QString> devices;
    ImageStreamWindow *windowWidget;
    ImageAcquisition *imageAcquisitionThread;
    QLabel *imageLabel;

    static QPlainTextEdit *globalMessageBox;

    QDir qDir;


    // functions
    void onApplicationStartup();
    void setupDevicesUI();
    void detectAttachedDevices();



public slots:
    void on_devicesRefresh_clicked();
    void connectToCamera(QString deviceName);
    void on_devicesTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
//    void renderImage(QImage qImage, int widgetIndex);
    void onCameraWindowClose();
    void pushToMessageBoxSlot(QString message);

signals:
    void pushToMessageBoxSignal(QString message);

private:
    Ui::Homescreen *ui;

};



#endif // HOMESCREEN_H
